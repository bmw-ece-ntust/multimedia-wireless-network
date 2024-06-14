# Assigment 3 - IEEE 802.11e simmulation on NS-3
- **Name:** Cecilia Ruiz Diaz
- **ID:** B10902106

1. [Purpose](#Purpose)
2. [Paper analysis](#Paper-analysis)
3. [Simulation](#Simulation)
   
   -[Network Topology](#Network-Topology)
   -[Flow Chart](#Flow-Chart)
   -[Code](#Code)
   -[Results](#Results)

#  Purpose
Learn to identify the problem and verify the concepts using simulation and analytical results.

# Paper Analysis
The study examines the IEEE 802.11e standard, which improves the traditional 802.11 MAC protocol to enable quality of service (QoS) in wireless LANs. 
The primary drawbacks of QoS support in legacy 802.11 are examined, including a lack of service distinction and unexpected delays.
The foundation of 802.11e is the Hybrid Coordination Function (HCF), which defines two modes of operation.

Enhanced Distributed Channel Access (EDCA) is a contention-based channel access strategy in which numerous backoff entities per station compete for different priority depending on access category (AC) characteristics such as AIFSN, CWmin, and CWmax.
HCF Controlled Channel Access (HCCA) is a controlled/polled access managed by a Hybrid Coordinator (HC) that may assign transmission opportunities (TXOPs) to stations.

Key EDCA features like as virtual collisions, TXOP continuations, and AC-specific EDCA parameter sets that allow for service differentiation are discussed. The HCCA technique, which provides parameterized QoS by restricting TXOP durations and polling stations, is also discussed.
The research assesses 802.11e's performance using simulations for a variety of situations, including isolated QBSSs, increasing the number of contesting stations, prioritized HC access, and overlapping QBSSs. The results reveal that EDCA offers varied throughputs, but HCCA can limit delays for the highest priority traffic. However, the lack of coordination across overlapping QBSSs is identified as an outstanding issue.
Other advancements, such as block acknowledgments for efficiency and the Direct Link Protocol for direct station-to-station transfers, are briefly discussed.

# Simulation

My decision to work with C++ for ns-3 simulations rather than Python was purposeful, motivated by my prior experience with C++. This option enables me to use my knowledge and expertise with C++'s grammar, structure, and performance characteristics to the ns-3 framework. 

## Network Topology

<p align="center">
<img src=/assets/assigment-3-1.jpg>

Fig 1. Network Topology

The network topology consists of a single Access Point (AP) and four Stations (STAs) configured to provide varied Quality of Service (QoS) across a WiFi network. This configuration enables extensive examination of QoS methods, traffic prioritization, and performance evaluation under changing traffic scenarios. Each component is carefully set with NS-3 APIs to replicate realistic network behavior and evaluate the effect of QoS parameters on network performance.

## Flow Chart

<img src=/assets/flowchart.jpg>

Fig 2. Code Flow Chart

## Code

Initialization of the code

```bash
#include "ns3/command-line.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/log.h"
#include "ns3/mobility-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/udp-server.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/packet.h"
#include "ns3/flow-monitor.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/boolean.h"
#include "ns3/pointer.h"
#include "ns3/qos-txop.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include "ns3/socket.h"
#include "ns3/udp-socket-factory.h"
#include "ns3/inet-socket-address.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211eTxop");


uint16_t port = 6969;
uint32_t payloadSize = 1472; 
double trafficStart = 1.0;
double trafficEnd = 30.0;
double trafficIncrement = 3.0;

int iterations = trafficEnd / trafficIncrement;
```
According to the parameters used in the paper

<img src=/assets/parameters.png>
Fig3. Paper EDCA parameters

My code use the following process to set it 
```bash 
     void RunSimulation(int iteration)
{
    std::stringstream fileNameStream;
    fileNameStream << "/home/cecilurdg/ns-3-allinone/ns-3-dev/output" << std::setfill('0') << std::setw(2) << iteration << ".csv";
    std::string fileName = fileNameStream.str();
 
    std::ofstream outFileCSV(fileName);
    if (!outFileCSV.is_open()) {
        NS_LOG_ERROR("Unable to open output file.");
        return;
    }
    std::cout << "Output file " << fileName << " has been opened." << std::endl;

    outFileCSV << "Traffic,Throughput_VO,Throughput_VI,Throughput_BE,Throughput_BK\n";

    for (int i = 0; i <= iterations; i++)
    {
        double currentTrafficAC = trafficStart + (i * trafficIncrement);

        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(4); 
        NodeContainer wifiApNodes;
        wifiApNodes.Create(1);

        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        YansWifiPhyHelper phy;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy.SetChannel(channel.Create());

        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211a);
        wifi.SetRemoteStationManager("ns3::IdealWifiManager");
        WifiMacHelper mac;

        NetDeviceContainer staDevice[4];
        NetDeviceContainer apDevice;
        Ssid ssid = Ssid("network");
        phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));

        mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
        for (int j = 0; j < 4; j++) {
            staDevice[j] = wifi.Install(phy, mac, wifiStaNodes.Get(j));
        }

        mac.SetType("ns3::ApWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid), "EnableBeaconJitter", BooleanValue(false));
        apDevice = wifi.Install(phy, mac, wifiApNodes.Get(0));

        Ptr<NetDevice> dev = wifiApNodes.Get(0)->GetDevice(0);
        Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
        Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
        PointerValue ptr;
        Ptr<QosTxop> edca;

        wifi_mac->GetAttribute("VI_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(7);
        edca->SetMaxCw(15);

        wifi_mac->GetAttribute("VO_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(3);
        edca->SetMaxCw(7);

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

        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(wifiStaNodes);
        mobility.Install(wifiApNodes);

        InternetStackHelper stack;
        stack.Install(wifiApNodes);
        stack.Install(wifiStaNodes);

        Ipv4AddressHelper address;
        address.SetBase("192.168.1.0", "255.255.255.0");
        Ipv4InterfaceContainer staInterface[4];
        Ipv4InterfaceContainer apInterface;
        for (int j = 0; j < 4; j++) {
            staInterface[j] = address.Assign(staDevice[j]);
        }
        apInterface = address.Assign(apDevice);

        UdpServerHelper server[4];
        ApplicationContainer serverApp[4];
        for (int j = 0; j < 4; j++) {
            server[j] = UdpServerHelper(port);
            serverApp[j] = server[j].Install(wifiStaNodes.Get(j));
            serverApp[j].Start(Seconds(0.0));
            serverApp[j].Stop(Seconds(10.0)); 
        }


        InetSocketAddress dest[4] = {
            InetSocketAddress(staInterface[0].GetAddress(0), port), 
            InetSocketAddress(staInterface[1].GetAddress(0), port), 
            InetSocketAddress(staInterface[2].GetAddress(0), port), 
            InetSocketAddress(staInterface[3].GetAddress(0), port)  
        };


        ApplicationContainer clientApp[4];
        for (int j = 0; j < 4; j++) {
            OnOffHelper client("ns3::UdpSocketFactory", dest[j]);
            Ptr<Socket> socket = Socket::CreateSocket(wifiApNodes.Get(0), UdpSocketFactory::GetTypeId());
            switch (j) {
                case 0:
                    socket->SetIpTos(0xb8); 
                    break;
                case 1:
                    socket->SetIpTos(0xc0); 
                    break;
                case 2:
                    socket->SetIpTos(0xa0); 
                    break;
                case 3:
                    socket->SetIpTos(0x20); 
                    break;
            }
            client.SetAttribute("Remote", AddressValue(dest[j]));
            client.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
            client.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
            client.SetAttribute("DataRate", StringValue(std::to_string(currentTrafficAC) + "Mbps"));
            client.SetAttribute("PacketSize", UintegerValue(payloadSize));
            clientApp[j] = client.Install(wifiApNodes.Get(0));
            clientApp[j].Start(Seconds(1.0));
            clientApp[j].Stop(Seconds(10.0)); 
        }
```
Start simulation

```bash
        Simulator::Stop(Seconds(4.0));
        Simulator::Run();

        monitor->CheckForLostPackets();
        Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
        std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

        double throughput[4] = {0, 0, 0, 0};
        for (auto iter = stats.begin(); iter != stats.end(); ++iter) {
            Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
            for (int j = 0; j < 4; j++) {
                if (t.destinationAddress == staInterface[j].GetAddress(0)) {
                    throughput[j] += iter->second.rxBytes * 8.0 / (9.0 * 1000000.0); 
                }
            }
        }

        Simulator::Destroy();
```
Save througput results in CSV file
```bash
        outFileCSV << currentTrafficAC << "," << throughput[0] << "," << throughput[1] << "," << throughput[2] << "," << throughput[3] << "\n";
    }

    outFileCSV.close();
    std::cout << "Simulation " << iteration << " completed. Results saved to " << fileName << std::endl;
}
```
## Results

Result of the CSV file created:

| Current Traffic (Mbps) | Throughput AC_VO (Mbps) | Throughput AC_VI (Mbps) | Throughput AC_BE (Mbps) | Throughput AC_BK (Mbps) |
|------------------------|-------------------------|-------------------------|-------------------------|-------------------------|
| 1                      | 0.338667                | 0.229333                | 0.338667                | 0.338667                |
| 4                      | 1.35733                 | 1.35733                 | 1.35733                 | 1.35733                 |
| 7                      | 2.37467                 | 2.372                   | 2.37467                 | 1.58667                 |
| 10                     | 2.46133                 | 2.45733                 | 2.45067                 | 2.45467                 |
| 13                     | 2.45067                 | 2.45467                 | 2.448                   | 2.448                   |
| 16                     | 2.45733                 | 2.456                   | 2.444                   | 2.46133                 |
| 19                     | 2.832                   | 2.844                   | 1.30667                 | 2.844                   |
| 22                     | 2.46533                 | 2.45733                 | 2.436                   | 2.46267                 |
| 25                     | 2.472                   | 2.43733                 | 2.43733                 | 2.46667                 |
| 28                     | 2.82667                 | 2.84133                 | 2.824                   | 1.30267                 |
| 31                     | 2.44133                 | 2.46267                 | 2.476                   | 2.44933                 |

