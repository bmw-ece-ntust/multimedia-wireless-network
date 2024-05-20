# Assigment 3 - IEEE 802.11e simmulation on NS-3
- **Name:** Emiliano Nazareno Rojas Marcelli
- **ID:** F11215120

## Outline
- <a href="#Objective">Objective</a> 
- <a href="#Introduction">Introduction </a>
- <a href="#Simulation">Simulation </a>
  - <a href="#Topology">Topology </a>
  - <a href="#Flowchart">Flow chart </a>
  -  <a href="#Code">Code</a>
- <a href="#Results">Results</a>
-  <a href="#Conclusion">Conclusion and Challenges</a>
- <a href="#References">References</a>
---

<h2 align="center" id="Objective">
Objective
</h2>

- Create a network simulation where we evaluate 802.11e using NS-3. 
- Obtain the results from the paper IEEE 802.11e by simulating on NS-3.

<br>

<h2 align="center" id="Introduction">
Introduction
</h2>

<p align="justify"> 
For this project the studied paper was [Analysisi of IEE 802.11e for QoS Support in Wireless LANs] , which goal is to evaluate the protocol 802.11e and compare it with its legacy 802.11. The protocol 802.11e is an enhancement at the MAC sublayer 
which adds QoS functionalities to Wi-Fi networks. Now the question is, how does 802.11e do this enhancement? The enhancement is done by applying the following supports:

1) Prioritize Data packets based on their types ( EDCA/AC ).
2) Allowing end stations to communicate their QoS requirements an Access Point ( AP ).

</p>
<p align="justify"> 
Regarding the support **number 1)**, which introduce a mechanism for prioritize different types of traffics that is called "Enhanced Distributed Channel Access" (EDCA) and define the concept of Access Category (AC), that show a way to prioritize
different types of traffic based on their importance and urgency. The traffics proposed for the study were Access Category Voice, Video, Best effort and Background. The mentioned traffics follow a herarchy of importance and urgency based
on the following image as reference:  
</p>

<p align="center">
  <img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/e13e164d-297a-44dc-87a6-7bc8210ec4cb" >
</p>  
<p align="center">
<strong>Figure 1. </strong> Access Category Herarchy
</p>
<p align="justify"> 
By following the apporach taken by the paper of study, it tries to check the effectiveness of the mentioned mechanisms but in real world scenarios ( Networking cogestion, Interference and trafficloads variation ) by using Throughput, delay, jitter and packet loss as parameters. 
For simplification and objective purpose, the parameter to be taken would be the througphut by applying a variation in traffic. The project will simulate 4 stations connected to an Acces Point, where each of the station will rerpesent a different Acces Category (Voice, Video, best effort and background).
</p>

<br>
<h2 align="center" id="Simulation">
Simulation
</h2>

<h3 align="center" id="Topology">
Topology
</h3>

<p align="center">
  <img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/92b0abb2-812f-407f-8a1d-89cbdab9aa8c" >
</p>  
<p align="center">
<strong>Figure 2. </strong> Reference of the used topology.
</p>

**Characteristics:**
- 1 Access Point and 4 Stations
- Each Stations represents an Access Category (Voice, Video, BE = Best Effort, BK = Background)

<h3 align="center" id="Flowchart">
Flow chart
</h3>
<p align="center">
  <img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/24e76c02-b001-4f63-8aba-6ea737337b04" >
</p> 
<p align="center">
<strong>Figure 3. </strong> Flowchart representation of the simulation code. 
</p>

<h3 align="center" id="Code">
Code
</h3>

The following code is the one used for the simulation. Based on the flow chart order, each scope of the code would be shown in detailed.  

#### <code style="Red:name_color">Define Modules</code>
```
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

```

#### <code style="Aqua:name_color">Main function</code>
```
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
```
#### <code style="Green:name_color">Loop</code>

```
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
```
### Functions
#### <code style="Cyan:name_color">Function to create nodes</code>
```
void CreateNodes(NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, NodeContainer &staAP) {
    staNodesVO.Create(1);
    staNodesVI.Create(1);
    staNodesBE.Create(1);
    staNodesBK.Create(1);
    staAP.Create(1);
}
```
#### <code style="Cyan:name_color">Function to configure traffic load</code>
```
void ConfigureTrafficLoad(double offeredTraffic, double &offeredTrafficVO, double &offeredTrafficVI, double &offeredTrafficBE, double &offeredTrafficBK) {
    offeredTrafficVO = offeredTraffic; //* 0.25;
    offeredTrafficVI = offeredTraffic; //* 0.25;
    offeredTrafficBE = offeredTraffic; //* 0.25;
    offeredTrafficBK = offeredTraffic; //* 0.25;
}
```
#### <code style="Cyan:name_color">Function to set up WiFi configuration</code>
```
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
```
#### <code style="Cyan:name_color">Function to modify EDCA configuration</code>

<p align="center">
  <img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/7177fa14-4a0a-4261-86cb-6071753dc53d" >
</p> 

<p align = "justify">
Based on the following figure, referenced from the paper, the parameters were adjusted in order to achieve a similar result as the study did.  
</p>
 
```
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
```
#### <code style="Cyan:name_color">Function to set up mobility model</code>
```
void SetupMobility(NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, NodeContainer &staAP) {
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
    mobility.Install(staNodesVO);
    mobility.Install(staNodesVI);
    mobility.Install(staNodesBE);
    mobility.Install(staNodesBK);
    mobility.Install(staAP);
}
```
#### <code style="Cyan:name_color">Function to install the Internet stack on nodes</code>
```
void InstallInternetStack(InternetStackHelper &stack, NodeContainer &staNodesVO, NodeContainer &staNodesVI, NodeContainer &staNodesBE, NodeContainer &staNodesBK, NodeContainer &staAP) {
    stack.Install(staNodesVO);
    stack.Install(staNodesVI);
    stack.Install(staNodesBE);
    stack.Install(staNodesBK);
    stack.Install(staAP);
}
```
#### <code style="Cyan:name_color">Function to assign IP addresses</code>
```
void AssignIPAddresses(Ipv4AddressHelper &address, NetDeviceContainer &devStaVO, NetDeviceContainer &devStaVI, NetDeviceContainer &devStaBE, NetDeviceContainer &devStaBK, NetDeviceContainer &devAP, Ipv4InterfaceContainer &staInterfaceVO, Ipv4InterfaceContainer &staInterfaceVI, Ipv4InterfaceContainer &staInterfaceBE, Ipv4InterfaceContainer &staInterfaceBK, Ipv4InterfaceContainer &APointInterface) {
    address.SetBase("192.168.1.0", "255.255.255.0");
    staInterfaceVO = address.Assign(devStaVO);
    staInterfaceVI = address.Assign(devStaVI);
    staInterfaceBE = address.Assign(devStaBE);
    staInterfaceBK = address.Assign(devStaBK);
    APointInterface = address.Assign(devAP);
}
```
#### <code style="Cyan:name_color">Function to set up UDP servers</code>
```
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
```

#### <code style="Cyan:name_color">Function to set uo UDP clients</code>
```
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
```
#### <code style="Cyan:name_color">Function to collect throughput data</code>
```
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
```

### <code style="Gold:name_color">Output</code>
```
    outputFile.close();
    return 0;
}
```
<h2 align="center" id="Results">Results</h2>
The simulation will show the results at the terminal and at the same time create a csv file which can be used for analysis later. 
<br>
<p align="center">
  <img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/c6b61341-6934-47a1-b152-574938d8446d" >
</p>  
<p align="center">
<strong>Figure 4. </strong> Average throughput per AC (access point) for each traffic type .
</p>

The following chart shwows how each traffic type vs the offered traffic per AC is increased. The x-axis of the chart represents the offered traffic per AC in Mbps, and the y-axis represents the throughput per AC in Mbps. Different lines represents different  different traffic types on the chart.
- Red line is the throughput for Voice (VO) traffic.
- Blue line represents the throughput for Video (VI) traffic.
- Green line shows the throughput for Best Effort (BE) traffic. 
- Black line is the  for Background (BK) traffic.

It can be seen that, the throughput for all four traffic types increases as the offered traffic increases. However, at some point when the rate of the throughput increases some variations on the traffic types can be seen. This is beccause the EDCA mechanism. So it can be said:
1. VO traffic has the highest priority, and it receives the most bandwidth even when the network is congested.
2. VI traffic has the second highest priority, and it receives more bandwidth than BE and BK traffic.  
3. BE traffic has a lower priority than VO and VI traffic, but it receives more bandwidth than BK traffic.
4. BK traffic has the lowest priority, and it only receives bandwidth when the network is not congested.

The following graph is extracted from the paper of study, which also show the results of applying a variation of throughput into the different stations.   
<p align="center">
  <img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/c2738a54-2f1d-46dd-b2e4-37327a3e62ab" >
</p>  
<p align="center">
<strong>Figure 5. </strong> Results from the same simulation obtained at the paper of study.
</p>

<h2  align="center" id="Conclusion">Conclusion and Challenges </h2> 

The results obtained from this project are quite similar to those presented in the study's paper. Some variations are observed, which may be attributed to differences in parameter settings. The primary goal of achieving a simulation that accurately reflects the behavior of EDCA/AC was successfully accomplished.
Some of the encountered challenges were understanding the idea in simulate the results from the paper of study and also obtain some information related to NS-3. 


<h2  align="center" id="References">References</h2>

- [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1265851)
- [Documentation NS-3](https://www.nsnam.org/documentation/)





 
