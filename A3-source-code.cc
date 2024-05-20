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
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-monitor-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211eTxop");

std::ofstream outputFile;

// Function to create nodes
void CreateNodes(NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, NodeContainer &staAP) {
    staNodesVO.Create(1);
    staNodesVI.Create(1);
    staNodesBE.Create(1);
    staNodesBK.Create(1);
    staAP.Create(1);
}

// Function to configure traffic load
void ConfigureTrafficLoad(double offeredTraffic, double &offeredTrafficVO, double &offeredTrafficVI, double &offeredTrafficBE, double &offeredTrafficBK) {
    offeredTrafficVO = offeredTraffic; //* 0.25;
    offeredTrafficVI = offeredTraffic; //* 0.25;
    offeredTrafficBE = offeredTraffic; //* 0.25;
    offeredTrafficBK = offeredTraffic; //* 0.25;
}

// Function to set up WiFi configuration
void SetupWifi(YansWifiPhyHelper &phy, WifiHelper &wifi, WifiMacHelper &mac, NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, NodeContainer &staAP, NetDeviceContainer &devStaVO, NetDeviceContainer &devStaVI, NetDeviceContainer &devStaBE, NetDeviceContainer &devStaBK, NetDeviceContainer &devAP) {
    Ssid ssid = Ssid("network");
    phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));

    // Stations
    mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
    devStaVO = wifi.Install(phy, mac, staNodesVO);
    devStaVI = wifi.Install(phy, mac, staNodesVI);
    devStaBE = wifi.Install(phy, mac, staNodesBE);
    devStaBK = wifi.Install(phy, mac, staNodesBK);

    // Access Point
    mac.SetType("ns3::ApWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid), "EnableBeaconJitter", BooleanValue(false));
    devAP = wifi.Install(phy, mac, staAP);
}

// Function to modify EDCA (Enhanced Distributed Channel Access) configuration
void ModifyEdcaConfiguration(Ptr<NetDevice> dev) {
    Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
    Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
    PointerValue ptr;
    Ptr<QosTxop> edca;

    // Voice
    wifi_mac->GetAttribute("VO_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(2);
    edca->SetMinCw(3);
    edca->SetMaxCw(7);

    // Video
    wifi_mac->GetAttribute("VI_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(2);
    edca->SetMinCw(7);
    edca->SetMaxCw(15);

    // Best Effort
    wifi_mac->GetAttribute("BE_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(3);
    edca->SetMinCw(15);
    edca->SetMaxCw(1023);

    // Background
    wifi_mac->GetAttribute("BK_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(7);
    edca->SetMinCw(15);
    edca->SetMaxCw(1023);
}

// Function to set up mobility model
void SetupMobility(NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, NodeContainer &staAP) {
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(staNodesVO);
    mobility.Install(staNodesVI);
    mobility.Install(staNodesBE);
    mobility.Install(staNodesBK);
    mobility.Install(staAP);
}

// Function to install the Internet stack on nodes
void InstallInternetStack(InternetStackHelper &stack, NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, NodeContainer &staAP) {
    stack.Install(staNodesVO);
    stack.Install(staNodesVI);
    stack.Install(staNodesBE);
    stack.Install(staNodesBK);
    stack.Install(staAP);
}

// Function to assign IP addresses
void AssignIPAddresses(Ipv4AddressHelper &address, NetDeviceContainer &devStaVO, NetDeviceContainer &devStaVI, NetDeviceContainer &devStaBE, NetDeviceContainer &devStaBK, NetDeviceContainer &devAP, Ipv4InterfaceContainer &staInterfaceVO, Ipv4InterfaceContainer &staInterfaceVI, Ipv4InterfaceContainer &staInterfaceBE, Ipv4InterfaceContainer &staInterfaceBK, Ipv4InterfaceContainer &APointInterface) {
    address.SetBase("192.168.1.0", "255.255.255.0");
    staInterfaceVO = address.Assign(devStaVO);
    staInterfaceVI = address.Assign(devStaVI);
    staInterfaceBE = address.Assign(devStaBE);
    staInterfaceBK = address.Assign(devStaBK);
    APointInterface = address.Assign(devAP);
}

// Function to set up UDP servers
void SetupUdpServer(uint16_t port, NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, ApplicationContainer &serverAppVO, ApplicationContainer &serverAppVI, ApplicationContainer &serverAppBE, ApplicationContainer &serverAppBK) {
    UdpServerHelper serverVO(port);
    serverAppVO = serverVO.Install(staNodesVO.Get(0));
    serverAppVO.Start(Seconds(0.0));
    serverAppVO.Stop(Seconds(4.0));

    UdpServerHelper serverVI(port);
    serverAppVI = serverVI.Install(staNodesVI.Get(0));
    serverAppVI.Start(Seconds(0.0));
    serverAppVI.Stop(Seconds(4.0));

    UdpServerHelper serverBE(port);
    serverAppBE = serverBE.Install(staNodesBE.Get(0));
    serverAppBE.Start(Seconds(0.0));
    serverAppBE.Stop(Seconds(4.0));

    UdpServerHelper serverBK(port);
    serverAppBK = serverBK.Install(staNodesBK.Get(0));
    serverAppBK.Start(Seconds(0.0));
    serverAppBK.Stop(Seconds(4.0));
}

// Function to set uo UDP clients
void SetupUdpClients(Ipv4InterfaceContainer &staInterfaceVO, Ipv4InterfaceContainer &staInterfaceVI, Ipv4InterfaceContainer &staInterfaceBE, Ipv4InterfaceContainer &staInterfaceBK, uint16_t port, double offeredTrafficVO, double offeredTrafficVI, double offeredTrafficBE, double offeredTrafficBK, uint32_t payloadSize, NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, ApplicationContainer &clientAppVO, ApplicationContainer &clientAppVI, ApplicationContainer &clientAppBE, ApplicationContainer &clientAppBK, NodeContainer &staAP) {
    // Voice
    InetSocketAddress destVO(staInterfaceVO.GetAddress(0), port);
    destVO.SetTos(0xc0); // AC_VO (Voice)

    OnOffHelper clientVO("ns3::UdpSocketFactory", destVO);
    clientVO.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientVO.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientVO.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficVO) + "Mbps")));
    clientVO.SetAttribute("PacketSize", UintegerValue(payloadSize));

    clientAppVO = clientVO.Install(staAP.Get(0));
    // clientAppVO = clientVO.Install(staNodesVO.Get(0));
    clientAppVO.Start(Seconds(1.0));
    clientAppVO.Stop(Seconds(5.0)); 

    // Video
    InetSocketAddress destVI(staInterfaceVI.GetAddress(0), port);
    destVI.SetTos(0xb8); // AC_VI (Video)

    OnOffHelper clientVI("ns3::UdpSocketFactory", destVI);
    clientVI.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientVI.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientVI.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficVI) + "Mbps")));
    clientVI.SetAttribute("PacketSize", UintegerValue(payloadSize));
    
    clientAppVI = clientVI.Install(staAP.Get(0));
    // clientAppVI = clientVI.Install(staNodesVI.Get(0));
    clientAppVI.Start(Seconds(1.0));
    clientAppVI.Stop(Seconds(5.0)); // 4.0

    // Best Effort
    InetSocketAddress destBE(staInterfaceBE.GetAddress(0), port);
    destBE.SetTos(0xa0); // AC_BE (Best Effort)

    OnOffHelper clientBE("ns3::UdpSocketFactory", destBE);
    clientBE.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientBE.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientBE.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficBE) + "Mbps")));
    clientBE.SetAttribute("PacketSize", UintegerValue(payloadSize));

    clientAppBE = clientBE.Install(staAP.Get(0));
    // clientAppBE = clientBE.Install(staNodesBE.Get(0));
    clientAppBE.Start(Seconds(1.0));
    clientAppBE.Stop(Seconds(5.0)); // 4.0

    // Background
    InetSocketAddress destBK(staInterfaceBK.GetAddress(0), port);
    destBK.SetTos(0x20); // AC_BK (Background)

    OnOffHelper clientBK("ns3::UdpSocketFactory", destBK);
    clientBK.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientBK.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientBK.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficBK) + "Mbps")));
    clientBK.SetAttribute("PacketSize", UintegerValue(payloadSize));

    clientAppBK = clientBK.Install(staAP.Get(0));
    //clientAppBK = clientBK.Install(staNodesBK.Get(0));
    clientAppBK.Start(Seconds(1.0));
    clientAppBK.Stop(Seconds(5.0)); 
}

// Function to collect throughput data
void CollectThroughputData(uint32_t payloadSize, ApplicationContainer &serverAppVO, ApplicationContainer &serverAppVI, ApplicationContainer &serverAppBE, ApplicationContainer &serverAppBK, double &offeredTraffic) {
    // Calculate the number of packets received
    uint32_t totalPacketsVO = DynamicCast<UdpServer>(serverAppVO.Get(0))->GetReceived();
    uint32_t totalPacketsVI = DynamicCast<UdpServer>(serverAppVI.Get(0))->GetReceived();
    uint32_t totalPacketsBE = DynamicCast<UdpServer>(serverAppBE.Get(0))->GetReceived();
    uint32_t totalPacketsBK = DynamicCast<UdpServer>(serverAppBK.Get(0))->GetReceived();

    // Calculate throughput in Mbps
    double throughputVO = (totalPacketsVO * payloadSize * 8.0) / (4 * 1000000.0);
    double throughputVI = (totalPacketsVI * payloadSize * 8.0) / (4 * 1000000.0);
    double throughputBE = (totalPacketsBE * payloadSize * 8.0) / (4 * 1000000.0);
    double throughputBK = (totalPacketsBK * payloadSize * 8.0) / (4 * 1000000.0);

    // Output throughput values
    std::cout<<offeredTraffic<<","<<throughputVO<<","<<throughputVI<<","<<throughputBE<<","<<throughputBK << std::endl;
    outputFile <<offeredTraffic<<","<<throughputVO<<","<<throughputVI<<","<<throughputBE<<","<<throughputBK;

}

// Main function
int main(int argc, char *argv[]) {
    
    outputFile.open("qos_tino_test1.csv");
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file!" << std::endl;
        return 1;
    }

    uint16_t port = 5001;
    uint32_t payloadSize = 1472; // bytes
    double offeredTrafficStart = 1.0;
    double offeredTrafficEnd = 30.0;
    double offeredTrafficStep = 3.0;

    // command line arguments
    CommandLine cmd;
    cmd.AddValue("offeredTrafficAC", "Offered traffic for AC(Mbps)", offeredTrafficStart);
    cmd.Parse(argc, argv);

    // Output text
    outputFile << "OfferedTraffic,VO,VI,BE,BK" << std::endl;
    std::cout << "TrafficOffered,VO,VI,BE,BK" << std::endl;

    // Loop over different offered traffic values
    for (double offeredTraffic = offeredTrafficStart; offeredTraffic <= offeredTrafficEnd; offeredTraffic += offeredTrafficStep) {
        
        // Create nodes
        NodeContainer staNodesVO, staNodesVI, staNodesBE, staNodesBK, staAP;
        CreateNodes(staNodesVO, staNodesVI, staNodesBE, staNodesBK, staAP);

        // Configure traffic load
        double offeredTrafficVO, offeredTrafficVI, offeredTrafficBE, offeredTrafficBK;
        ConfigureTrafficLoad(offeredTraffic, offeredTrafficVO, offeredTrafficVI, offeredTrafficBE, offeredTrafficBK);

        // Set up WiFi configuration
        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        YansWifiPhyHelper phy;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy.SetChannel(channel.Create());

        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211a);
        wifi.SetRemoteStationManager("ns3::IdealWifiManager");
        WifiMacHelper mac;

        NetDeviceContainer devStaVO, devStaVI, devStaBE, devStaBK, devAP;
        SetupWifi(phy, wifi, mac, staNodesVO, staNodesVI, staNodesBE, staNodesBK, staAP, devStaVO, devStaVI, devStaBE, devStaBK, devAP);

        // Modify EDCA configuration
        Ptr<NetDevice> dev = devAP.Get(0);
        ModifyEdcaConfiguration(dev);

        // Set up mobility model
        SetupMobility(staNodesVO, staNodesVI, staNodesBE, staNodesBK, staAP);

        // Install Internet stack
        InternetStackHelper stack;
        InstallInternetStack(stack, staNodesVO, staNodesVI, staNodesBE, staNodesBK, staAP);

        // Assign IP addresses
        Ipv4AddressHelper address;
        Ipv4InterfaceContainer staInterfaceVO, staInterfaceVI, staInterfaceBE, staInterfaceBK, APointInterface;
        AssignIPAddresses(address, devStaVO, devStaVI, devStaBE, devStaBK, devAP, staInterfaceVO, staInterfaceVI, staInterfaceBE, staInterfaceBK, APointInterface);

        // Set up UDP servers
        ApplicationContainer serverAppVO, serverAppVI, serverAppBE, serverAppBK;
        SetupUdpServer(port, staNodesVO, staNodesVI, staNodesBE, staNodesBK, serverAppVO, serverAppVI, serverAppBE, serverAppBK);

        // Set up UDP clients
        ApplicationContainer clientAppVO, clientAppVI, clientAppBE, clientAppBK;
        SetupUdpClients(staInterfaceVO, staInterfaceVI, staInterfaceBE, staInterfaceBK, port, offeredTrafficVO, offeredTrafficVI, offeredTrafficBE, offeredTrafficBK, payloadSize, staNodesVO, staNodesVI, staNodesBE, staNodesBK, clientAppVO, clientAppVI, clientAppBE, clientAppBK, staAP);

        // Run simulation
        Simulator::Stop(Seconds(4.0));
        Simulator::Run();

        // Collect throughput data
        CollectThroughputData(payloadSize, serverAppVO, serverAppVI, serverAppBE, serverAppBK, offeredTraffic);

        outputFile << std::endl;
        Simulator::Destroy();
    }

    outputFile.close();
    return 0;
}

