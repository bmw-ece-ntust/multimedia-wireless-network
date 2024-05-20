
:::warning
**Table of Content**
[TOC]
:::
- reference: [hacdMD note](https://hackmd.io/@SylviaCh/SJSAewm7A)
# Simulation
## Run code
### 1. Navigate to the folder ns-allinone-3.40/ns-3.40/examples/wireless.
```bash=
cd Downloads/ns-allinone-3.40/ns-3.40/examples/wireless
```

### 2. Create the file wifi-txop-4QBSS.cc
   - [Full code](https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-M11202255-Sylvia/wifi-txop-4QBSS.cc)

### 3. Open CMakeList.txt and add the following line to build_example.
```
build_example(
  NAME wifi-txop-4QBSS
  SOURCE_FILES wifi-txop-4QBSS.cc
  LIBRARIES_TO_LINK ${libwifi}
                    ${libapplications}
)
```
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/67dceaa3-53c1-47f6-a110-bb8984f3b8e7)


### 4. Compile.
```bash=
./ns3 build
```
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/94882339-4321-4c55-8bac-1797d2ae3cb5)


```bash=
./ns3 run examples/wireless/wifi-txop-4QBSS.cc
```
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/29f041b4-2aca-4495-93cc-5c0aade61f10)


### 5. Upload the execution result <font color = "blue" >output_file.txt</font> to Google Drive and use [colab](https://colab.research.google.com/drive/13YoeMkiFhquJEpHV5tZKtOT7awAD9tll?usp=sharing) to plot the simulation graph.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/1d4a470d-a770-419b-8dd3-b8ba6747821a)
   
   - [Python code](https://colab.research.google.com/drive/13YoeMkiFhquJEpHV5tZKtOT7awAD9tll?usp=sharing)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/19ca486a-f7bf-41cc-9e4f-f8e81edf304e)


## Assumption 
- IEEE 802.11e MAC + IEEE 802.11a PHY
- Values for the EDCA parameter sets
    | | AC_VO | AC_VI | AC_BE | AC_BK |
    | -------- | -------- | -------- | -------- | -------- | 
    | AIFSN: | 2 | 2 | 3 | 7 | 
    | CWmin: | 3 | 7 | 15 | 15 | 
    | CWmax: | 7 | 15 | 1023 | 1023 | 

## result
- CAs: [34.2.1.5.1. Selection of the Access Category (AC)](https://www.nsnam.org/docs/models/html/wifi-user.html#selection-of-the-access-category-ac)
    - AC_VI: 0xa0
    - AC_VO: 0xc0
    - AC_BE: 0x00
    - AC_BK: 0x20
    
<div align="center">
	<img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/1b6be168-e817-4c45-bf02-62b2f8cf2e33" alt="Editor" width="600">
</div>

**Why didn't you simulate the figures from the paper, Fig. 9 and Fig. 10?**
> Because in the current ns-3 environment, I have not yet found a way to enable a single STA to have multiple TXOPs.

### Comparison and Analysis of Results:
By comparing with [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://ieeexplore.ieee.org/document/1265851), can identify some similarities and differences:

#### **Similarities**:
- The throughput curves for the four ACs exhibit a similar shape, initially increasing and then decreasing.
- The AC_VI curve reaches its peak in the mid-range of offered traffic.
- The AC_VO and AC_BE curves have similar shapes, tending to saturate at higher offered traffic levels.

#### **Differences**:

- In the simulation results, the throughput of AC_VO is higher than that of AC_VI, while the opposite is observed in the paper.
- The throughput of AC_BK shows some recovery at higher offered traffic levels, whereas it continuously declines in the paper.
- The absolute values of throughput differ, which may be due to different simulation parameter settings.
<div align="center">
	<img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/1b6be168-e817-4c45-bf02-62b2f8cf2e33" alt="Editor" width="500">
</div>
<div align="center">
	<img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/45db65c3-ca3a-4d5a-9c2f-f91f078f33b6" alt="Editor" width="500">
</div>

Despite some detailed discrepancies, the simulation broadly reflects the throughput trends of different ACs described in the paper, validating the EDCA mechanism's capability to manage various traffic types. To further improve simulation accuracy, should adjust the simulation parameters to more closely match the scenario described in the paper.

---
# Code explanation
## Flow chart
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/8079ca60-6e59-4bc1-997a-f80d6b248312)


## code step by step
Certainly! Let's go through the code step by step:

### Libraries and Namespace

```cpp
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
```

This section includes all necessary ns-3 libraries for creating WiFi simulations and defines the logging component name.

### Main Function and Output File

```cpp
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
```

Here, the main function starts by opening an output file to record the simulation results. It defines variables for the port number, payload size, and the range for offered traffic (start, end, step).

### Command-line Parsing and Initial Logging

```cpp
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
```

This section initializes the offered traffic variable and logs the initial state to both the output file and the console. It also sets up command-line parsing to allow for different offered traffic values.

### Main Simulation Loop

```cpp
    // Loop through offeredTraffic values
    for (offeredTrafficAC = offeredTrafficStart; offeredTrafficAC <= offeredTrafficEnd;
         offeredTrafficAC += offeredTrafficStep)
    {
        outputFile << "Offered Traffic (Mbps): " << offeredTrafficAC << std::endl;
        NS_LOG_INFO("Offered Traffic (Mbps): " << offeredTrafficAC);
```

The main loop iterates over the range of offered traffic values, logging the current value at each iteration.

### Node Creation

```cpp
        // 1. Create nodes. 1 AP and 4 STAs
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(4);
        NodeContainer wifiApNodes;
        wifiApNodes.Create(1);
```

This part creates the nodes for the simulation: one Access Point (AP) and four Station (STA) nodes.

### Channel and Physical Layer Setup

```cpp
        // 2. Create channel and phy
        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        YansWifiPhyHelper phy;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy.SetChannel(channel.Create());

        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211a);
        wifi.SetRemoteStationManager("ns3::IdealWifiManager");
        WifiMacHelper mac;
```

This part sets up the WiFi channel and physical layer. The `YansWifiChannelHelper` and `YansWifiPhyHelper` are used to create the wireless channel and physical layer, respectively.

### Device Installation

```cpp
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
```

This segment installs the WiFi devices on the AP and STA nodes. The SSID and QoS support are configured here.

### EDCA Configuration

```cpp
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
```

This part customizes the EDCA parameters for each Access Category (AC). The AIFSN, minimum, and maximum contention window (CWmin and CWmax) values are set for each AC to control their access to the medium.

### Mobility and Network Stack Setup

```cpp
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
        Ipv4InterfaceContainer staInterfaceD;
        staInterfaceD = address.Assign(staDeviceD);        
        Ipv4InterfaceContainer apInterfaceA;
        apInterfaceA = address.Assign(apDeviceA);
```

This segment sets up the mobility model and installs the network stack (TCP/IP) on the nodes. It also assigns IP addresses to the devices.

### Application Setup (UDP Servers and Clients)

```cpp
        UdpServerHelper serverA(port);
        ApplicationContainer serverAppA = serverA.Install(wifiStaNodes.Get(0));
        serverAppA.Start(Seconds(0.0));
        serverAppA.Stop(Seconds(3));

        UdpServerHelper serverB(port);
        ApplicationContainer

 serverAppB = serverB.Install(wifiStaNodes.Get(1));
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
```

This section sets up the UDP servers on the STA nodes.

### Client Configuration

```cpp
        InetSocketAddress destA(staInterfaceA.GetAddress(0), port);
        destA.SetTos(0xa0); // AC_VI 0xa0

        InetSocketAddress destB(staInterfaceB.GetAddress(0), port);
        destB.SetTos(0xc0); // AC_VO 0xc0

        InetSocketAddress destC(staInterfaceC.GetAddress(0), port);
        destC.SetTos(0x00); // AC_BE 0x00

        InetSocketAddress destD(staInterfaceD.GetAddress(0), port);
        destD.SetTos(0x20); // AC_BK 0x20

        OnOffHelper clientA("ns3::UdpSocketFactory", destA);
        clientA.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        clientA.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        clientA.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficAC) + "Mbps")));
        clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppA = clientA.Install(wifiApNodes.Get(0));
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
```

This part sets up the UDP clients on the AP node, specifying the destination address, traffic type (TOS), data rate, and packet size for each AC.

### Simulation Execution

```cpp
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
```

This section runs the simulation for 5 seconds and retrieves the number of packets received by each UDP server, indicating the throughput for each AC.

### Throughput Calculation and Logging

```cpp
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
```

This final section calculates the throughput for each AC in Mbps and logs the results to both the console and the output file.

---


# Study note
reference: 
- [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://ieeexplore.ieee.org/document/1265851)
## **ABSTRACT**
- This paper provides an overview of the new protocol for QoS offered by the 802.11e WLAN standard.
- It analyzes the enhancements of 802.11e and compares the performance with the legacy 802.11 standard.
- The paper evaluates the contention-based and polling-based medium access control mechanisms in the 802.11e Hybrid Coordination Function (HCF).
- It discusses the ability of 802.11e to support QoS through simulations.

## **INTRODUCTION**
- 802.11 WLANs are widely deployed in various environments such as enterprises, homes, and public access networks.
- The primary features of the 802.11 standard include the simplicity and fault tolerance of the distributed medium access control protocol.
- Currently, 802.11 only offers best-effort service, similar to wireless Ethernet.
- There is increasing interest in applications that require QoS support, leading to initiatives to enhance the 802.11 MAC protocol to support QoS.

## **LEGACY 802.11**
- An overview of the legacy 802.11 MAC protocol's Distributed Coordination Function (DCF) is provided:
  - Based on Carrier Sense Multiple Access (CSMA)
  - Collision Avoidance (CA) mechanism
  - Optional fragmentation and RTS/CTS mechanisms
- The limitations of using Point Coordination Function (PCF) in 802.11 for QoS support are explained:
  - Unpredictable beacon delays and polling station transmission times
  - Lack of control over parameters affecting QoS

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/c169008e-ca96-4f65-b93d-0dc25610bae3)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/65ca1700-9806-4c43-9249-9931cbcd9b2c)

## **LIMITED QOS SUPPORT APPLYING THE POINT COORDINATION FUNCTION**
- The legacy 802.11 used PCF to provide QoS support for time-sensitive services.
- PCF offers a mechanism for prioritizing access to the wireless medium, coordinated centrally by a station called the Point Coordinator (PC).
- The drawbacks include:
  - Unpredictable beacon delays leading to uncertain CFP start times.
  - Inability to control the transmission time of polled stations, affecting QoS.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/a938440a-d5f8-4571-a481-91011794ca84)

---
## **QOS SUPPORT MECHANISMS OF 802.11E**
- 802.11e introduces the Hybrid Coordination Function (HCF) to provide QoS support.
- HCF defines two types of medium access mechanisms:
  - Enhanced Distributed Channel Access (EDCA) - contention-based.
  - HCF Controlled Channel Access (HCCA) - includes polling.
- The concept of Transmission Opportunity (TXOP) is introduced, allowing the Hybrid Coordinator (HC) to control the length of TXOPs to manage QoS.

## **BASIC IMPROVEMENTS OF THE LEGACY 802.11 MAC**
- 802.11e stations do not transmit across the Target Beacon Transmission Time (TBTT), reducing beacon delays.
- 802.11e allows direct communication between stations within a QoS Basic Service Set (QBSS) without passing through the AP.
- The Block Acknowledgment (Block Ack) mechanism is introduced to improve transmission efficiency.
- The Direct Link Protocol (DLP) is defined to establish direct links between 802.11e stations.

## **HCF CONTENTION-BASED MEDIUM ACCESS**
- EDCA introduces access categories (AC) and multiple independent backoff entities.
- There are four ACs representing different priorities: AC_VO (voice), AC_VI (video), AC_BE (best effort), and AC_BK (background).
- Each AC uses different EDCA parameter sets (e.g., AIFS, CWmin, CWmax) to contend for medium access.
- If multiple backoff entities within the same station are ready to transmit simultaneously, a virtual collision occurs, and the higher priority AC is granted access.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/dcf30eda-f3ea-44f1-a0ba-34e629e8681e)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/a951f197-af7c-4ab3-bb55-3520f38b9da7)


## **HYBRID COORDINATION FUNCTION, CONTROLLED MEDIUM ACCESS**
- HCCA extends the EDCA rules, allowing the HC to have the highest priority access to the medium.
- During both CFP and CP, the HC can obtain a TXOP without backoff after sensing an idle PIFS.
- During CP, stations can obtain a TXOP by EDCA rules or by receiving a QoS CF-Poll from the HC.
- During CFP, only stations explicitly polled by the HC are allowed to transmit.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/de09bee1-9205-48c8-9a7d-5bbd471ef843)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/ba8c25de-b4fa-44f4-ab31-ea4a7e15b679)

### **IMPROVED EFFICIENCY**
- Block Acknowledgment (Block Ack) allows multiple MPDUs to be transmitted consecutively within a single TXOP and acknowledged only once.
- Direct Link Protocol (DLP) allows stations within a QBSS to transmit directly to each other without going through the AP.
- New mechanisms are introduced to enhance performance, such as limiting TXOP length and preventing transmissions that cross TBTT boundaries.

---

## **ACHIEVABLE EDCA THROUGHPUT IN AN ISOLATED QBSS WITH FOUR STATIONS**
- Evaluating the achievable EDCA throughput in an isolated QBSS with only four stations.
- It's observed that as the offered traffic increases, higher priority ACs limit the achievable throughput of lower priority ACs.
- This is because higher priority ACs use smaller AIFSN, CWmin, and CWmax values.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/0a0ae772-6226-4cc3-ab1b-b0bbbea8fd14)



## **ACHIEVABLE EDCA THROUGHPUT WITH INCREASING NUMBER OF STATIONS:**
- Assessing the throughput performance of EDCA as the number of stations increases to 16.
- It's noted that with default EDCA parameter settings, the throughput significantly decreases as the number of stations increases.
- This is due to the increased collision probability, especially for AC_VO with smaller CWmin and CWmax values.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/9eb25852-8c70-4352-9eb2-9925ce9abd59)



## **QOS GUARANTEES WITH PRIORITIZED ACCESS OF HC:**
- Evaluating the performance when allowing the HC to have the highest priority access.
- Within a single QBSS, HCCA's high priority transmissions can maintain below a certain latency threshold.
- However, in overlapping QBSS environments, the transmission latency and throughput of HCCA are affected due to the lack of coordination among HCs.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/f96fe571-b4a9-4d96-8deb-8b49d0aec83f)

Through simulations, it's observed that EDCA can meet various priority demands with fewer stations but requires parameter adjustments as the number of stations increases. HCCA can ensure the latency quality of high priority transmissions, but the issue of overlapping QBSSs remains unresolved.

---
## abbreviation
#### IEEE 802.11 WLAN
- IEEE 802.11 WLAN: IEEE 802.11 wireless local area network

#### MAC (Medium Access Control)
- MAC: Medium Access Control
- DC F: Distributed Coordination Function
- CSMA: Carrier Sense Multiple Access
- MSDU Delivery: MAC Service Data Unit Delivery
- MPDU: MAC Protocol Data Unit
- CA: Collision Avoidance

#### Modulation and Transmission Techniques
- CCK: Complementary Code Keying
- DSSS: Direct Sequence Spread Spectrum
- OFDM: Orthogonal Frequency-Division Multiplexing

#### Quality of Service (QoS)
- QoS: Quality of Service
- QBSS: QoS Supporting BSS
- EDCA: Enhanced Distributed Channel Access
- HCCA: HCF Controlled Channel Access
- TXOP: Transmission Opportunity
- HCCA-TXOP: A TXOP Obtained by the HC via Controlled Medium Access
- ACs: Access Categories
- AIFS[AC]: Arbitration Interframe Space
- AIFSN[AC]: Arbitration Interframe Space Number

#### Network Components and Structures
- BSS: Basic Service Set
- AP: Access Point
- PC: Point Coordinator
- HC: Hybrid Coordinator

#### Interframe Spaces and Coordination
- DIFS: DCF Interframe Space
- SIFS: Short Interframe Space
- PIFS: PCF Interframe Space

#### Protocols and Control Frames
- ACK: Acknowledgment
- RTS/CTS: Request-to-Send/Clear-to-Send
- NAV: Network Allocation Vector

#### Periods and Timing
- CFP: Contention-Free Period
- CP: Contention Period
- TBTTs: Target Beacon Transmission Time

#### Coordination Functions
- HCF: Hybrid Coordination Function

#### Miscellaneous
- ISM: Industrial, Scientific, and Medical
- CW: Contention Window
- DLP: Direct Link Protocol

#### PHY (Physical Layer)
- PHY: Physical Layer

#### Statistical and Error Metrics
- CDFs: Complementary Cumulative Distribution Functions
- LRE: Limited-Relative-Error

