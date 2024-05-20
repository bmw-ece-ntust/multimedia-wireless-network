#include "ns3/applications-module.h"
#include "ns3/command-line.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/log.h"
#include "ns3/mobility-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/pointer.h"
#include "ns3/qos-txop.h"
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/udp-server.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211eTxop");

int main(int argc, char *argv[])
{
    // Open output file for recording characteristics
    std::ofstream outputFile("output_file.txt");
    if (!outputFile.is_open())
    {
        std::cerr << "Error: Could not open output file!" << std::endl;
        return 1;
    }

    // Define offered traffic range and interval
    uint16_t port = 5001;
    uint32_t payloadSize = 1472; // bytes
    double offeredTrafficStart = 3.0;
    double offeredTrafficEnd = 30.0;
    double offeredTrafficStep = 3.0;

    double throughputA = 0;
    double throughputB = 0;
    double throughputC = 0;
    double throughputD = 0;

    // Log and write output to file
    double offeredTrafficAC = 0;
    outputFile << "Offered Traffic (Mbps): " << offeredTrafficAC << std::endl;
    NS_LOG_INFO("Offered Traffic (Mbps): " << offeredTrafficAC);    
    NS_LOG_INFO("AC_VI: " << throughputA );
    outputFile << "AC_VI: " << throughputA << std::endl;
    NS_LOG_INFO("AC_VO: " << throughputB );
    outputFile << "AC_VO: " << throughputB << std::endl;
    NS_LOG_INFO("AC_BE: " << throughputC );
    outputFile << "AC_BE: " << throughputC << std::endl;
    NS_LOG_INFO("AC_BK: " << throughputD );
    outputFile << "AC_BK: " << throughputD << std::endl;
    outputFile << std::endl;// Add an empty line 

    // Command-line options for repeatability
    CommandLine cmd;
    cmd.AddValue("offeredTrafficAC", "Offered traffic for AC(Mbps)", offeredTrafficStart);
    cmd.Parse(argc, argv);

    // Loop through offeredTraffic values
    for (offeredTrafficAC = offeredTrafficStart; offeredTrafficAC <= offeredTrafficEnd;
         offeredTrafficAC += offeredTrafficStep)
    {
        outputFile << "Offered Traffic (Mbps): " << offeredTrafficAC << std::endl;
        NS_LOG_INFO("Offered Traffic (Mbps): " << offeredTrafficAC);
        
        // 1. Create nodes. 1 AP and 4 STAs
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(4);
        NodeContainer wifiApNodes;
        wifiApNodes.Create(1);

        // 2. Create channel and phy
        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        YansWifiPhyHelper phy;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy.SetChannel(channel.Create());

        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211a);
        wifi.SetRemoteStationManager("ns3::IdealWifiManager");
        WifiMacHelper mac;

        NetDeviceContainer staDeviceA;
        NetDeviceContainer staDeviceB;
        NetDeviceContainer staDeviceC;
        NetDeviceContainer staDeviceD;
        NetDeviceContainer apDeviceA;
        Ssid ssid;

        // Network
        ssid = Ssid("network");
        phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));
        mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
        staDeviceA = wifi.Install(phy, mac, wifiStaNodes.Get(0));
        staDeviceB = wifi.Install(phy, mac, wifiStaNodes.Get(1));
        staDeviceC = wifi.Install(phy, mac, wifiStaNodes.Get(2));
        staDeviceD = wifi.Install(phy, mac, wifiStaNodes.Get(3));

        mac.SetType("ns3::ApWifiMac",
                    "QosSupported",
                    BooleanValue(true),
                    "Ssid",
                    SsidValue(ssid),
                    "EnableBeaconJitter",
                    BooleanValue(false));
        apDeviceA = wifi.Install(phy, mac, wifiApNodes.Get(0));

        // Modify EDCA configuration (TXOP limit) for each AC
        Ptr<NetDevice> dev = wifiApNodes.Get(0)->GetDevice(0);
        Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
        Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
        PointerValue ptr;
        Ptr<QosTxop> edca;
        
        wifi_mac->GetAttribute("VO_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(3);
        edca->SetMaxCw(7);

        wifi_mac->GetAttribute("VI_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(7);
        edca->SetMaxCw(15);

        wifi_mac->GetAttribute("BE_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);
        
        wifi_mac->GetAttribute("BK_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(7);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);

        // Set mobility models for the nodes
        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(wifiStaNodes);
        mobility.Install(wifiApNodes);

        // Install Internet stack on nodes
        InternetStackHelper stack;
        stack.Install(wifiApNodes);
        stack.Install(wifiStaNodes);

        // Assign IP addresses to the devices
        Ipv4AddressHelper address;
        address.SetBase("192.168.1.0", "255.255.255.0");
        Ipv4InterfaceContainer staInterfaceA;
        staInterfaceA = address.Assign(staDeviceA);
        Ipv4InterfaceContainer staInterfaceB;
        staInterfaceB = address.Assign(staDeviceB);
        Ipv4InterfaceContainer staInterfaceC;
        staInterfaceC = address.Assign(staDeviceC);
        Ipv4InterfaceContainer staInterfaceD;
        staInterfaceD = address.Assign(staDeviceD);        
        Ipv4InterfaceContainer apInterfaceA;
        apInterfaceA = address.Assign(apDeviceA);

        // Create UDP servers on each STA
        UdpServerHelper serverA(port);
        ApplicationContainer serverAppA = serverA.Install(wifiStaNodes.Get(0));
        serverAppA.Start(Seconds(0.0));
        serverAppA.Stop(Seconds(3));

        UdpServerHelper serverB(port);
        ApplicationContainer serverAppB = serverB.Install(wifiStaNodes.Get(1));
        serverAppB.Start(Seconds(0.0));
        serverAppB.Stop(Seconds(3));

        UdpServerHelper serverC(port);
        ApplicationContainer serverAppC = serverC.Install(wifiStaNodes.Get(2));
        serverAppC.Start(Seconds(0.0));
        serverAppC.Stop(Seconds(3));

        UdpServerHelper serverD(port);
        ApplicationContainer serverAppD = serverD.Install(wifiStaNodes.Get(3));
        serverAppD.Start(Seconds(0.0));
        serverAppD.Stop(Seconds(3));

        // Configure destination addresses with appropriate TOS values
        InetSocketAddress destA(staInterfaceA.GetAddress(0), port);
        destA.SetTos(0xa0); // AC_VI 0xa0

        InetSocketAddress destB(staInterfaceB.GetAddress(0), port);
        destB.SetTos(0xc0); // AC_VO 0xc0

        InetSocketAddress destC(staInterfaceC.GetAddress(0), port);
        destC.SetTos(0x00); // AC_BE 0x00

        InetSocketAddress destD(staInterfaceD.GetAddress(0), port);
        destD.SetTos(0x20); // AC_BK 0x20

        // Create and configure OnOff applications for each AC
        OnOffHelper clientA("ns3::UdpSocketFactory", destA);
        clientA.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        clientA.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        clientA.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficAC) + "Mbps")));
        clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppA = clientA.Install(wifiApNodes.Get(0));
        // clientAppVO = clientVO.Install(staNodesVO.Get(0));
        clientAppA.Start(Seconds(1.0));
        clientAppA.Stop(Seconds(4.0)); 

        OnOffHelper clientB("ns3::UdpSocketFactory", destB);
        clientB.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        clientB.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        clientB.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficAC) + "Mbps")));
        clientB.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppB = clientB.Install(wifiApNodes.Get(0));
        clientAppB.Start(Seconds(1.0));
        clientAppB.Stop(Seconds(5.0)); 

        OnOffHelper clientC("ns3::UdpSocketFactory", destC);
        clientC.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        clientC.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        clientC.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficAC) + "Mbps")));
        clientC.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppC = clientC.Install(wifiApNodes.Get(0));
        clientAppC.Start(Seconds(1.0));
        clientAppC.Stop(Seconds(5.0)); 

        OnOffHelper clientD("ns3::UdpSocketFactory", destD);
        clientD.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        clientD.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        clientD.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficAC) + "Mbps")));
        clientD.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppD = clientD.Install(wifiApNodes.Get(0));
        clientAppD.Start(Seconds(1.0));
        clientAppD.Stop(Seconds(5.0)); 


        // Run the simulation
        Simulator::Stop(Seconds(5.0));
        Simulator::Run();

        uint totalPacketsThroughA =
            DynamicCast<UdpServer>(serverAppA.Get(0))->GetReceived(); // AC_VI. Sta 0
        uint totalPacketsThroughB =
            DynamicCast<UdpServer>(serverAppB.Get(0))->GetReceived(); // AC_VO. Sta 1
        uint totalPacketsThroughC =
            DynamicCast<UdpServer>(serverAppC.Get(0))->GetReceived(); // AC_BE. Sta 2
        uint totalPacketsThroughD =
            DynamicCast<UdpServer>(serverAppD.Get(0))->GetReceived(); // AC_BK. Sta 3
        Simulator::Destroy();

        double throughputA = (totalPacketsThroughA * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputB = (totalPacketsThroughB * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputC = (totalPacketsThroughC * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputD = (totalPacketsThroughD * payloadSize * 8.0) / (4 * 1000000.0);

        // Log and write output to file
        NS_LOG_INFO("AC_VI: " << throughputA );
        outputFile << "AC_VI: " << throughputA << std::endl;
        NS_LOG_INFO("AC_VO: " << throughputB );
        outputFile << "AC_VO: " << throughputB << std::endl;
        NS_LOG_INFO("AC_BE: " << throughputC );
        outputFile << "AC_BE: " << throughputC << std::endl;
        NS_LOG_INFO("AC_BK: " << throughputD );
        outputFile << "AC_BK: " << throughputD << std::endl;
        outputFile << std::endl;// Add an empty line between iterations
    }

    outputFile.close();
    return 0;
}
