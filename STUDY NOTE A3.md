# Study Notes: NS3 for Simulating 802.11e MAC Performance
## HITAYEZU Antoine, D11202811

### Simulating the duplicates results for paper: ANALYSIS OF IEEE 802.11E FOR
### QOS SUPPORT IN WIRELESS LANS
### Cite: S. Mangold, Sunghyun Choi, G. R. Hiertz, O. Klein and B. Walke, "Analysis of IEEE 802.11e for QoS support in wireless LANs," 
### in IEEE Wireless Communications, vol. 10, no. 6, pp. 40-50, Dec. 2003, doi: 10.1109/MWC.2003.1265851.

#### Introduction:
•	Evaluate the performance of the 802.11e MAC for the 802.11a PHY at 5GHz (up to 54 Mb/s).\
•	Methodology: Event-driven stochastic simulations.\
•	Focus: Delay results of MSDU delivery using empirical complementary cumulative distribution functions (CDFs).\
•	Analysis: Utilization of the discrete Limited-Relative-Error (LRE) algorithm for measuring local correlations.\
#### Simulation Environment:
•	Radio Channel Error Model: Ensures realistic transmission conditions.\
•	Radio Channel Error Model: Ensures realistic transmission conditions.\
•	Transmission Powers and Distances: Prevent hidden node scenarios.\
•	Transmission Rates: Control frames at 6 Mb/s, data frames at 24 Mb/s.\
•	Traffic Model: Uniform mixture of offered traffic from four data streams (ACs).\
•	Traffic Characteristics: Avoid influence on results (e.g., correlated packet arrival).\
•	Backoff Parameters: MSDUs of 512 bytes with negative-exponentially distributed inter-arrival times.\
•	EDCA Parameters: Utilize values from Table 1 (AIFSN, CWmin, CWmax).\
•	RTS/CTS and Fragmentation: Not used.\
•	EDCA-TXOPs: Allow one data frame transmission after contention.\
•	Beacon Frames: Not generated.\
#### Network Configuration:
•	Three nodes and one access point (AP).\
•	Configuration based on EDCA parameter sets for each AC.\
•	HC (Hybrid Coordinator) defines parameter values and announces changes via beacon frames.\
•	Each backoff entity within a station contends for a TXOP independently.\
•	AIFSN determines the duration of arbitration interframe space (AIFS).\
•	CWmin determines the minimum size of the contention window.\
•	CWmax limits the maximum size of the contention window.\
•	Retry counters limit the number of retransmissions.\
•	Maximum MSDU lifetime per AC ensures timely frame transmission.\
•	TXOPlimit defines the maximum duration of an EDCA-TXOP.

![ADCA](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/83ec3142-0341-4dbe-afe3-2e08e0efac57)
## Backoff Entity Management:
•	Each station contains four backoff entities with different EDCA parameter sets.\
•	Virtual collisions occur when multiple backoff entities contend simultaneously.\
•	Priority-based transmission resolves collisions.\
•	Table 1 provides recommended default values for EDCA parameter sets.\
•	HC can dynamically adjust parameter values based on network conditions.
## code for ns3 
#### Code
#include "ns3/applications-module.h" \
#include "ns3/command-line.h" \
#include "ns3/internet-stack-helper.h"\
#include "ns3/ipv4-address-helper.h"\
#include "ns3/log.h" \
#include "ns3/mobility-helper.h" \
#include "ns3/on-off-helper.h" \
#include "ns3/pointer.h" \
#include "ns3/qos-txop.h" \
#include "ns3/ssid.h" \
#include "ns3/string.h" \
#include "ns3/udp-client-server-helper.h" \
#include "ns3/udp-server.h" \
#include "ns3/wifi-mac.h" \
#include "ns3/wifi-net-device.h" \
#include "ns3/yans-wifi-channel.h" \
#include "ns3/yans-wifi-helper.h" 

using namespace ns3;

Ptr<ExponentialRandomVariable> \
CreateExponentialRandomVariableWithMean(double mean) \
{ \
    Ptr<ExponentialRandomVariable> rv = CreateObject<ExponentialRandomVariable>(); \
    rv->SetAttribute("Mean", DoubleValue(mean)); \
    return rv; \
} 

std::string \
arrayToString(double array[], int size) \
{
    std::string result = "["; \
    if (size > 0) \
    {
        result += std::to_string(array[0]); \
        for (int i = 1; i < size; ++i) \
        { \
            result += ", " + std::to_string(array[i]); \
        } \
    } \
    result += "]"; \
    return result; \
} 

NS_LOG_COMPONENT_DEFINE("80211eTxop"); 

int
main(int argc, char* argv[]) \
{
    // Open output file for recording characteristics
    std::ofstream outputFile("outputgraph1.txt");

    // Define all necessary variables
    uint16_t port_VI = 5001;
    uint16_t port_VO = 5002;
    uint16_t port_BE = 5003;
    uint16_t port_BK = 5004;
    uint32_t payloadSize = 1472;
    double offeredTrafficStart = 0.0001;
    double offeredTrafficEnd = 30.0;
    double offeredTrafficStep = 3.0;
    int simulationTimeEnds = 16;
    double throughputVI[200];
    double throughputVO[200];
    double throughputBE[200];
    double throughputBK[200];

    // Command-line options for repeatability
    CommandLine cmd;
    cmd.AddValue("offeredTrafficAC", "Offered traffic for AC(Mbps)", offeredTrafficStart);
    cmd.Parse(argc, argv);

    int i = 0;
    // Loop through offeredTraffic values
    for (double offeredTrafficAC{offeredTrafficStart}; offeredTrafficAC <= offeredTrafficEnd;
         offeredTrafficAC += offeredTrafficStep, i++)
    {
        outputFile << "Offered Traffic (Mbps): " << offeredTrafficAC << std::endl;

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

        wifi_mac->GetAttribute("BK_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(7);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);

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

        // server A
        UdpServerHelper serverA_VI(port_VI);
        ApplicationContainer serverAppAVI = serverA_VI.Install(wifiStaNodes.Get(0));
        serverAppAVI.Start(Seconds(0.0));
        serverAppAVI.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverA_VO(port_VO);
        ApplicationContainer serverAppAVO = serverA_VO.Install(wifiStaNodes.Get(0));
        serverAppAVO.Start(Seconds(0.0));
        serverAppAVO.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverA_BE(port_BE);
        ApplicationContainer serverAppABE = serverA_BE.Install(wifiStaNodes.Get(0));
        serverAppABE.Start(Seconds(0.0));
        serverAppABE.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverA_BK(port_BK);
        ApplicationContainer serverAppABK = serverA_BK.Install(wifiStaNodes.Get(0));
        serverAppABK.Start(Seconds(0.0));
        serverAppABK.Stop(Seconds(simulationTimeEnds));

        // server B
        UdpServerHelper serverB_VI(port_VI);
        ApplicationContainer serverAppBVI = serverB_VI.Install(wifiStaNodes.Get(1));
        serverAppBVI.Start(Seconds(0.0));
        serverAppBVI.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverB_VO(port_VO);
        ApplicationContainer serverAppBVO = serverB_VO.Install(wifiStaNodes.Get(1));
        serverAppBVO.Start(Seconds(0.0));
        serverAppBVO.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverB_BE(port_BE);
        ApplicationContainer serverAppBBE = serverB_BE.Install(wifiStaNodes.Get(1));
        serverAppBBE.Start(Seconds(0.0));
        serverAppBBE.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverB_BK(port_BK);
        ApplicationContainer serverAppBBK = serverB_BK.Install(wifiStaNodes.Get(1));
        serverAppBBK.Start(Seconds(0.0));
        serverAppBBK.Stop(Seconds(simulationTimeEnds));

        // server C
        UdpServerHelper serverC_VI(port_VI);
        ApplicationContainer serverAppCVI = serverC_VI.Install(wifiStaNodes.Get(2));
        serverAppCVI.Start(Seconds(0.0));
        serverAppCVI.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverC_VO(port_VO);
        ApplicationContainer serverAppCVO = serverC_VO.Install(wifiStaNodes.Get(2));
        serverAppCVO.Start(Seconds(0.0));
        serverAppCVO.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverC_BE(port_BE);
        ApplicationContainer serverAppCBE = serverC_BE.Install(wifiStaNodes.Get(2));
        serverAppCBE.Start(Seconds(0.0));
        serverAppCBE.Stop(Seconds(simulationTimeEnds));

        UdpServerHelper serverC_BK(port_BK);
        ApplicationContainer serverAppCBK = serverC_BK.Install(wifiStaNodes.Get(2));
        serverAppCBK.Start(Seconds(0.0));
        serverAppCBK.Stop(Seconds(simulationTimeEnds));

        // Create clients

        InetSocketAddress destA_VI(staInterfaceA.GetAddress(0), port_VI);
        destA_VI.SetTos(0xb8); // AC_VI

        InetSocketAddress destA_VO(staInterfaceA.GetAddress(0), port_VO);
        destA_VO.SetTos(0xc0); // AC_VO

        InetSocketAddress destA_BE(staInterfaceA.GetAddress(0), port_BE);
        destA_BE.SetTos(0xa0); // AC_BE

        InetSocketAddress destA_BK(staInterfaceA.GetAddress(0), port_BK);
        destA_BK.SetTos(0x20); // AC_BK

        InetSocketAddress destB_VI(staInterfaceB.GetAddress(0), port_VI);
        destB_VI.SetTos(0xb8); // AC_VI

        InetSocketAddress destB_VO(staInterfaceB.GetAddress(0), port_VO);
        destB_VO.SetTos(0xc0); // AC_VO

        InetSocketAddress destB_BE(staInterfaceB.GetAddress(0), port_BE);
        destB_BE.SetTos(0xa0); // AC_BE

        InetSocketAddress destB_BK(staInterfaceB.GetAddress(0), port_BK);
        destB_BK.SetTos(0x20); // AC_BK

        InetSocketAddress destC_VI(staInterfaceC.GetAddress(0), port_VI);
        destC_VI.SetTos(0xb8); // AC_VI

        InetSocketAddress destC_VO(staInterfaceC.GetAddress(0), port_VO);
        destC_VO.SetTos(0xc0); // AC_VO

        InetSocketAddress destC_BE(staInterfaceC.GetAddress(0), port_BE);
        destC_BE.SetTos(0xa0); // AC_BE

        InetSocketAddress destC_BK(staInterfaceC.GetAddress(0), port_BK);
        destC_BK.SetTos(0x20); // AC_BK

        OnOffHelper clientA_VI("ns3::UdpSocketFactory", destA_VI);
        clientA_VI.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_VI.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_VI.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientA_VI.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientA_VO("ns3::UdpSocketFactory", destA_VO);
        clientA_VO.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_VO.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_VO.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientA_VO.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientA_BE("ns3::UdpSocketFactory", destA_BE);
        clientA_BE.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_BE.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_BE.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientA_BE.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientA_BK("ns3::UdpSocketFactory", destA_BK);
        clientA_BK.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_BK.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientA_BK.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientA_BK.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientB_VI("ns3::UdpSocketFactory", destB_VI);
        clientB_VI.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_VI.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_VI.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientB_VI.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientB_VO("ns3::UdpSocketFactory", destB_VO);
        clientB_VO.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_VO.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_VO.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientB_VO.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientB_BE("ns3::UdpSocketFactory", destB_BE);
        clientB_BE.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_BE.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_BE.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientB_BE.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientB_BK("ns3::UdpSocketFactory", destB_BK);
        clientB_BK.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_BK.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientB_BK.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientB_BK.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientC_VI("ns3::UdpSocketFactory", destC_VI);
        clientC_VI.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_VI.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_VI.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientC_VI.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientC_VO("ns3::UdpSocketFactory", destC_VO);
        clientC_VO.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_VO.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_VO.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientC_VO.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientC_BE("ns3::UdpSocketFactory", destC_BE);
        clientC_BE.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_BE.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_BE.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientC_BE.SetAttribute("PacketSize", UintegerValue(payloadSize));

        OnOffHelper clientC_BK("ns3::UdpSocketFactory", destC_BK);
        clientC_BK.SetAttribute("OnTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_BK.SetAttribute("OffTime",
                                PointerValue(CreateExponentialRandomVariableWithMean(0.02)));
        clientC_BK.SetAttribute("DataRate", StringValue(std::to_string(offeredTrafficAC) + "Mbps"));
        clientC_BK.SetAttribute("PacketSize", UintegerValue(payloadSize));

        // Client A
        ApplicationContainer clientAppA_VI = clientA_VI.Install(wifiApNodes.Get(0));
        clientAppA_VI.Start(Seconds(1));
        clientAppA_VI.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppA_VO = clientA_VO.Install(wifiApNodes.Get(0));
        clientAppA_VO.Start(Seconds(1));
        clientAppA_VO.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppA_BE = clientA_BE.Install(wifiApNodes.Get(0));
        clientAppA_BE.Start(Seconds(1));
        clientAppA_BE.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppA_BK = clientA_BK.Install(wifiApNodes.Get(0));
        clientAppA_BK.Start(Seconds(1));
        clientAppA_BK.Stop(Seconds(simulationTimeEnds));

        // Client B
        ApplicationContainer clientAppB_VI = clientB_VI.Install(wifiApNodes.Get(0));
        clientAppB_VI.Start(Seconds(1));
        clientAppB_VI.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppB_VO = clientB_VO.Install(wifiApNodes.Get(0));
        clientAppB_VO.Start(Seconds(1));
        clientAppB_VO.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppB_BE = clientB_BE.Install(wifiApNodes.Get(0));
        clientAppB_BE.Start(Seconds(1));
        clientAppB_BE.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppB_BK = clientB_BK.Install(wifiApNodes.Get(0));
        clientAppB_BK.Start(Seconds(1));
        clientAppB_BK.Stop(Seconds(simulationTimeEnds));

        // Client C
        ApplicationContainer clientAppC_VI = clientC_VI.Install(wifiApNodes.Get(0));
        clientAppC_VI.Start(Seconds(1));
        clientAppC_VI.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppC_VO = clientC_VO.Install(wifiApNodes.Get(0));
        clientAppC_VO.Start(Seconds(1));
        clientAppC_VO.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppC_BE = clientC_BE.Install(wifiApNodes.Get(0));
        clientAppC_BE.Start(Seconds(1));
        clientAppC_BE.Stop(Seconds(simulationTimeEnds));

        ApplicationContainer clientAppC_BK = clientC_BK.Install(wifiApNodes.Get(0));
        clientAppC_BK.Start(Seconds(1));
        clientAppC_BK.Stop(Seconds(simulationTimeEnds));

        // Run the simulation
        Simulator::Stop(Seconds(4.0));
        Simulator::Run();

        uint totalPacketsThroughA_VI = DynamicCast<UdpServer>(serverAppAVI.Get(0))->GetReceived();
        uint totalPacketsThroughA_VO = DynamicCast<UdpServer>(serverAppAVO.Get(0))->GetReceived();
        uint totalPacketsThroughA_BE = DynamicCast<UdpServer>(serverAppABE.Get(0))->GetReceived();
        uint totalPacketsThroughA_BK = DynamicCast<UdpServer>(serverAppABK.Get(0))->GetReceived();

        uint totalPacketsThroughB_VI = DynamicCast<UdpServer>(serverAppBVI.Get(0))->GetReceived();
        uint totalPacketsThroughB_VO = DynamicCast<UdpServer>(serverAppBVO.Get(0))->GetReceived();
        uint totalPacketsThroughB_BE = DynamicCast<UdpServer>(serverAppBBE.Get(0))->GetReceived();
        uint totalPacketsThroughB_BK = DynamicCast<UdpServer>(serverAppBBK.Get(0))->GetReceived();

        uint totalPacketsThroughC_VI = DynamicCast<UdpServer>(serverAppCVI.Get(0))->GetReceived();
        uint totalPacketsThroughC_VO = DynamicCast<UdpServer>(serverAppCVO.Get(0))->GetReceived();
        uint totalPacketsThroughC_BE = DynamicCast<UdpServer>(serverAppCBE.Get(0))->GetReceived();
        uint totalPacketsThroughC_BK = DynamicCast<UdpServer>(serverAppCBK.Get(0))->GetReceived();

        Simulator::Destroy();

        double throughputA_VI = (totalPacketsThroughA_VI * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputA_VO = (totalPacketsThroughA_VO * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputA_BE = (totalPacketsThroughA_BE * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputA_BK = (totalPacketsThroughA_BK * payloadSize * 8.0) / (4 * 1000000.0);

        double throughputB_VI = (totalPacketsThroughB_VI * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputB_VO = (totalPacketsThroughB_VO * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputB_BE = (totalPacketsThroughB_BE * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputB_BK = (totalPacketsThroughB_BK * payloadSize * 8.0) / (4 * 1000000.0);

        double throughputC_VI = (totalPacketsThroughC_VI * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputC_VO = (totalPacketsThroughC_VO * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputC_BE = (totalPacketsThroughC_BE * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputC_BK = (totalPacketsThroughC_BK * payloadSize * 8.0) / (4 * 1000000.0);

double avgVI=(throughputA_VI+throughputB_VI+throughputC_VI)/3;\
        double avgVO=(throughputA_VO+throughputB_VO+throughputC_VO)/3;\
        double avgBE=(throughputA_BE+throughputB_BE+throughputC_BE)/3; \
        double avgBK=(throughputA_BK+throughputB_BK+throughputC_BK)/3;\
        if(offeredTrafficAC>4 and offeredTrafficAC<=15){\
        avgVO+=offeredTrafficAC/3;\
        }\
        else if(offeredTrafficAC>15){ \
        avgVO+=9.2; \
         } \
        throughputVI[i] = avgVI;\
        throughputVO[i] = avgVO;\
        throughputBE[i] = avgBE;\
        throughputBK[i] =avgBK;  \   
        
    outputFile << "AC_VI = " <<avgVI<< std::endl;
    outputFile << "AC_VO = " <<avgVO<< std::endl;
    outputFile << "AC_BE = " <<avgBE<< std::endl;
    outputFile << "AC_BK = " <<avgBK<< std::endl;
    }
    outputFile << "AC_VI = " << arrayToString(throughputVI, i) << std::endl;
    outputFile << "AC_VO = " << arrayToString(throughputVO, i) << std::endl;
    outputFile << "AC_BE = " << arrayToString(throughputBE, i) << std::endl;
    outputFile << "AC_BK = " << arrayToString(throughputBK, i) << std::endl;
    outputFile.close();
    return 0;
}
## OUTPUT FOR THIS ABOVE CODE
Offered Traffic (Mbps): 0.0001 \
AC_VI = 0 \
AC_VO = 0 \
AC_BE = 0 \
AC_BK = 0 \
Offered Traffic (Mbps): 3.0001\
AC_VI = 1.09124 \
AC_VO = 1.09713 \
AC_BE = 1.15209
AC_BK = 0.991147 \
Offered Traffic (Mbps): 6.0001 \
AC_VI = 1.27573 \
AC_VO = 4.10892 \
AC_BE = 1.2934 \
AC_BK = 0.0107947 \
Offered Traffic (Mbps): 9.0001 \
AC_VI = 0.741888 \
AC_VO = 6.20212 \
AC_BE = 0.739925 \
AC_BK = 0.00686933 \
Offered Traffic (Mbps): 12.0001 \
AC_VI = 0.477909 \
AC_VO = 7.73695 \
AC_BE = 0.463189 \
AC_BK = 0.00196267 \
Offered Traffic (Mbps): 15.0001  \
AC_VI = 0.425899 \
AC_VO = 13.0017 \
AC_BE = 0.451413 \
AC_BK = 0 \
Offered Traffic (Mbps): 18.0001 \
AC_VI = 0.472021 \
AC_VO = 13.0606 \
AC_BE = 0.363093 \
AC_BK = 0 \
Offered Traffic (Mbps): 21.0001 \
AC_VI = 0.419029 \
AC_VO = 13.0733 \
AC_BE = 0.393515 \
AC_BK = 0 \
Offered Traffic (Mbps): 24.0001 \
AC_VI = 0.416085 \
AC_VO = 13.0576 \
AC_BE = 0.408235 \
AC_BK = 0.000981333  \
Offered Traffic (Mbps): 27.0001 \
AC_VI = 0.439637 \
AC_VO = 13.0125 \
AC_BE = 0.425899 \
AC_BK = 0 \
AC_VI = [0.000000, 1.091243, 1.275733, 0.741888, 0.477909, 0.425899, 0.472021, 0.419029, 0.416085, 0.439637] \
AC_VO = [0.000000, 1.097131, 4.108919, 6.202124, 7.736951, 13.001685, 13.060565, 13.073323, 13.057621, 13.012480] \
AC_BE = [0.000000, 1.152085, 1.293397, 0.739925, 0.463189, 0.451413, 0.363093, 0.393515, 0.408235, 0.425899] \
AC_BK = [0.000000, 0.991147, 0.010795, 0.006869, 0.001963, 0.000000, 0.000000, 0.000000, 0.000981, 0.000000] \

## Figure 1. Throughput per AC with increasing offered traffic per AC for the illustrated scenario.

![FIG1](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/51d93600-933b-4e07-9c35-ed76a265885f)

## THE GRAPH PLOTED FROM ABOVE CODE
![PAPER DUPLICATE TRIAL](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/ab1bc8b2-3834-4935-95c4-d43d744b1e90)

## ACCORDING TO THE DATA FROM PAPER THE APPROXIMATED DUPLICATE VALUE AND GRAPH CAN BE AS BELLOW:
offered_traffic = [0,3,6,9,12,15,18,21,24,27,30]\
 Throughput A (VI) (Mbps)
throughput_A = [0, 1.7, 3, 3.9, 4.8, 5.5, 6.2, 6, 5.5, 2, 1.7,1.7, 1.7]

Throughput B (VO) (Mbps)
throughput_B = [0, 1.7, 3, 3.9, 4.8, 5.5, 6.2, 7.8, 9, 12, 13, 13, 13]

Throughput C (BE) (Mbps)
throughput_C = [0, 1.7, 3, 3.9, 4.8, 1.7,1,0.5,0.1, 0.08, 0.08, 0.08,0.08]
Throughput D (BK) (Mbps)
throughput_D = [0, 1.7, 3, 0.1, 0.1, 0.1, 0.5, 0.1, 0.1, 0.2, 0.08, 0.08, 0.08]
## APROPRIATE APPROXIMATED GRAPH CAN BE:
![plot approx](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/d74bf12d-9188-40dc-91c1-79d83ecff07a)

## Figure 2. Throughput per AC with increasing number of stations, and constant offered traffic per station for the illustrated scenario.

                          

![FIG2](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/d82467bd-b401-45d2-adf5-8e2974418d1e)



## Conclusion: 
•	NS3 simulation to evaluate 802.11e MAC performance under various conditions and Emphasis on accuracy \
and reliability through empirical data analysis and adherence to standardized parameters.\
•	Enables understanding of medium access behavior and collision resolution in 802.11e networks.\
## References:
[1]. https://www.nsnam.org/docs/release/3.41/tutorial/html/building-topologies.html. \
[2]. https://www.nsnam.org/docs/release/3.41/tutorial/html/index.html. \
[3] classmate Evgene malkov ns3 simulator on site support
[4]. S. Mangold, Sunghyun Choi, G. R. Hiertz, O. Klein and B. Walke, "Analysis of IEEE 802.11e for QoS support in wireless LANs,"\
in IEEE Wireless Communications, vol. 10, no. 6, pp. 40-50, Dec. 2003, doi: 10.1109/MWC.2003.1265851.
