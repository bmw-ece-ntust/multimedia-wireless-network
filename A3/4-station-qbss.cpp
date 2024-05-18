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

Ptr<ExponentialRandomVariable>
CreateExponentialRandomVariable(double mean)
{
    Ptr<ExponentialRandomVariable> rv = CreateObject<ExponentialRandomVariable>();
    rv->SetAttribute("Mean", DoubleValue(mean));
    return rv;
}

void Log(std::string message, std::ofstream &outputFile)
{
    NS_LOG_INFO(message);
    outputFile << message << std::endl;
}

NS_LOG_COMPONENT_DEFINE("80211eTxop");

int main(int argc, char *argv[])
{
    // Open output file for recording characteristics
    std::ofstream outputFile("output_characteristics.txt");

    // Define all neccessary variables
    uint16_t port = 5001;
    uint32_t payloadSize = 1472;
    double offeredTrafficStart = 1.0;
    double offeredTrafficEnd = 120.0;
    double offeredTrafficStep = 3.0;

    // Command-line options for repeatability
    CommandLine cmd;
    cmd.AddValue("offeredTrafficAC", "Offered traffic for AC(Mbps)", offeredTrafficStart);
    cmd.Parse(argc, argv);

    // Loop through offeredTraffic values
    for (double offeredTrafficAC = offeredTrafficStart; offeredTrafficAC <= offeredTrafficEnd;
         offeredTrafficAC += offeredTrafficStep)
    {
        Log("Offered Traffic (Mbps): " + std::to_string(offeredTrafficAC), outputFile);

        // 1. Create nodes. 1 AP and 3 STAs
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(3);
        NodeContainer wifiApNodes;
        wifiApNodes.Create(1);

        // 2. Create channel and phy
        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        YansWifiPhyHelper phy;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy.SetChannel(channel.Create());
        phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));

        // 3. Create wifi helper and mac
        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211a);
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                     "DataMode",
                                     StringValue("OfdmRate24Mbps"),
                                     "ControlMode",
                                     StringValue("OfdmRate6Mbps"));
        WifiMacHelper mac;

        // 4. Install network devices
        NetDeviceContainer staDeviceA;
        NetDeviceContainer staDeviceB;
        NetDeviceContainer staDeviceC;
        NetDeviceContainer apDeviceA;
        Ssid ssid;

        // 5. Set up the network devices
        ssid = Ssid("network");
        mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
        staDeviceA = wifi.Install(phy, mac, wifiStaNodes.Get(0));
        staDeviceB = wifi.Install(phy, mac, wifiStaNodes.Get(1));
        staDeviceC = wifi.Install(phy, mac, wifiStaNodes.Get(2));

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
        wifi_mac->GetAttribute("BE_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);

        wifi_mac->GetAttribute("VO_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(3);
        edca->SetMaxCw(7);

        wifi_mac->GetAttribute("VI_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(7);
        edca->SetMaxCw(15);

        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(wifiStaNodes);
        mobility.Install(wifiApNodes);

        InternetStackHelper stack;
        stack.Install(wifiApNodes);
        stack.Install(wifiStaNodes);

        Ipv4AddressHelper address;
        address.SetBase("192.168.1.0", "255.255.255.0");
        Ipv4InterfaceContainer staInterfaceA;
        staInterfaceA = address.Assign(staDeviceA);
        Ipv4InterfaceContainer staInterfaceB;
        staInterfaceB = address.Assign(staDeviceB);
        Ipv4InterfaceContainer staInterfaceC;
        staInterfaceC = address.Assign(staDeviceC);
        Ipv4InterfaceContainer apInterfaceA;
        apInterfaceA = address.Assign(apDeviceA);

        UdpServerHelper serverA(port);
        ApplicationContainer serverAppA = serverA.Install(wifiStaNodes.Get(0));
        serverAppA.Start(Seconds(0.0));
        serverAppA.Stop(Seconds(4));

        UdpServerHelper serverB(port);
        ApplicationContainer serverAppB = serverB.Install(wifiStaNodes.Get(1));
        serverAppB.Start(Seconds(0.0));
        serverAppB.Stop(Seconds(4));

        UdpServerHelper serverC(port);
        ApplicationContainer serverAppC = serverC.Install(wifiStaNodes.Get(2));
        serverAppC.Start(Seconds(0.0));
        serverAppC.Stop(Seconds(4));

        InetSocketAddress destA(staInterfaceA.GetAddress(0), port);
        destA.SetTos(0xb8); // AC_VI

        InetSocketAddress destB(staInterfaceB.GetAddress(0), port);
        destB.SetTos(0xc0); // AC_VO

        InetSocketAddress destC(staInterfaceC.GetAddress(0), port);
        destC.SetTos(0xa0); // AC_BE

        OnOffHelper clientA("ns3::UdpSocketFactory", destA);
        clientA.SetAttribute("OnTime", PointerValue(CreateExponentialRandomVariable(0.02)));
        clientA.SetAttribute("OffTime", PointerValue(CreateExponentialRandomVariable(0.02)));
        clientA.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientB("ns3::UdpSocketFactory", destB);
        clientB.SetAttribute("OnTime", PointerValue(CreateExponentialRandomVariable(0.02)));
        clientB.SetAttribute("OffTime", PointerValue(CreateExponentialRandomVariable(0.02)));
        clientB.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientB.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientC("ns3::UdpSocketFactory", destC);
        clientC.SetAttribute("OnTime", PointerValue(CreateExponentialRandomVariable(0.02)));
        clientC.SetAttribute("OffTime", PointerValue(CreateExponentialRandomVariable(0.02)));
        clientC.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientC.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppA = clientA.Install(wifiApNodes.Get(0));
        clientAppA.Start(Seconds(1.0));
        clientAppA.Stop(Seconds(4));

        ApplicationContainer clientAppB = clientB.Install(wifiApNodes.Get(0));
        clientAppB.Start(Seconds(1.0));
        clientAppB.Stop(Seconds(4));

        ApplicationContainer clientAppC = clientC.Install(wifiApNodes.Get(0));
        clientAppC.Start(Seconds(1.0));
        clientAppC.Stop(Seconds(4));

        // Run the simulation
        Simulator::Stop(Seconds(4.0));
        Simulator::Run();

        uint totalPacketsThroughA =
            DynamicCast<UdpServer>(serverAppA.Get(0))->GetReceived(); // AC_VI. Sta 0
        uint totalPacketsThroughB =
            DynamicCast<UdpServer>(serverAppB.Get(0))->GetReceived(); // AC_VO. Sta 1
        uint totalPacketsThroughC =
            DynamicCast<UdpServer>(serverAppC.Get(0))->GetReceived(); // AC_BE. Sta 2

        Simulator::Destroy();

        double throughputA = (totalPacketsThroughA * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputB = (totalPacketsThroughB * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputC = (totalPacketsThroughC * payloadSize * 8.0) / (4 * 1000000.0);

        Log("Throughput A (VI): " + std::to_string(throughputA) + " Mbps", outputFile);
        Log("Throughput B (VO): " + std::to_string(throughputB) + " Mbps", outputFile);
        Log("Throughput C (BE): " + std::to_string(throughputC) + " Mbps", outputFile);
        Log("", outputFile);
    }

    outputFile.close();
    return 0;
}