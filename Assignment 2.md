![](./assets/lab-logo.jpg)

# <center>Assignment 2</center>

<center></center>

- Name: Jerry
- Student ID: M11202259

**<center>Table of Content</center>**

- [Assignment 2](#assignment-2)
- [Submission Time](#submission-time)
  - [1. What is NS3?](#1-what-is-ns3)
  - [2. Install the NS3 in docker](#2-install-the-ns3-in-docker)
    - [2-1. Testing NS3](#2-1-testing-ns3)
  - [3. Simulate wifi-80211e-txop.cc](#3-simulate-wifi-80211e-txopcc)
  - [4. Simulate wifi-hidden-terminal.cc](#4-simulate-wifi-hidden-terminalcc)
    - [4-1. Configuration Parameters Setting](#4-1-configuration-parameters-setting)
      - [4-1-1. Setting Node number](#4-1-1-setting-node-number)
      - [4-1-2. Setting Wifi standard](#4-1-2-setting-wifi-standard)
      - [4-1-3. Setting the default IP for your node](#4-1-3-setting-the-default-ip-for-your-node)
      - [4-1-4. Setting middle node to simluate](#4-1-4-setting-middle-node-to-simluate)
      - [4-1-5. Setting flow start time \&\& data rate](#4-1-5-setting-flow-start-time--data-rate)
    - [4-2. Statistics of the simulation](#4-2-statistics-of-the-simulation)
    - [4-3. Simulate the result](#4-3-simulate-the-result)
      - [4-3-1. Default (3 Node)](#4-3-1-default-3-node)
      - [4-3-2. (5 Node) Simulation](#4-3-2-5-node-simulation)
        - [4-3-2-1. Architecture](#4-3-2-1-architecture)
        - [4-3-2-2. Simulation Result](#4-3-2-2-simulation-result)
        - [4-3-2-3. Source Code](#4-3-2-3-source-code)
      - [4-3-3. (5 Node, 2 Access Point) Simulation](#4-3-3-5-node-2-access-point-simulation)
        - [4-3-3-1. Architecture](#4-3-3-1-architecture)
        - [4-3-3-2. Simulation Result](#4-3-3-2-simulation-result)
        - [4-3-3-3. Source Code](#4-3-3-3-source-code)


# Submission Time
![A2 Submission Time](https://imgur.com/DuCKL6n.png)

## 1. What is NS3?
ns-3 is an open-source simulation platform for modeling, simulating, and researching network and communication systems. It is a C++ programming framework designed to provide powerful tools for researchers and developers to study and test network and communication protocols. Here are some key features and information about ns-3:

- **Open Source Nature** : ns-3 is an open-source project, available for free use and customization. This means that researchers and developers can modify the code to suit their needs.
- **High Extensibility** : ns-3 provides a highly extensible architecture that allows users to add custom modules and protocols. This makes it easy for researchers to implement new ideas in simulations.
- **Realism** : ns-3 is based on real-world network and communication systems, making it an effective tool for evaluating new protocols and technologies.
- **Layered Models** : ns-3 supports modeling and simulation of various network layers, including the physical layer, data link layer, network layer, and more.
- **Wide Range of Applications** : ns-3 is widely used in various communication domains, including wireless communication, internet protocols, sensor networks, vehicular communication, and more.
- **Documentation and Community** : ns-3 has an active community and provides rich documentation and tutorials, making it easier for users to get started and use the platform effectively.
- **Cross-Platform Support** : ns-3 can run on different operating systems, including Linux, Windows, and macOS.
## 2. Install the NS3 in docker
Add this configure to your local docker.

```javascript=
{
  "insecure-registries" : ["quay.bmw.lab"]
}
```
![Screenshot 2024-04-02 at 12.25.40 AM](https://hackmd.io/_uploads/rJnfgPuJA.png)

```javascript=
docker pull quay.bmw.lab/infidel/ns3:3.38
```
![Screenshot 2024-04-01 at 11.39.16 PM](https://hackmd.io/_uploads/r184BIdyC.png)

**Result :**

![Screenshot 2024-04-02 at 12.02.48 AM](https://hackmd.io/_uploads/rJbVjLuJ0.png)

**Mention :** Only lab member can access this repository.

### 2-1. Testing NS3
```javascript=
docker run -it quay.bmw.lab/infidel/ns3:3.38 bash
```
```javascript=
docker run -it -v ./task2:/usr/ns3/ns-3.38/contrib/mwn-task2:Z quay.bmw.lab/infidel/ns3:3.38 bash
```
The below is run the command inside in the docker container.

```javascript=
./ns3 run hello-simulator
```
![Screenshot 2024-04-01 at 11.46.31 PM](https://hackmd.io/_uploads/Hyk4P8Oy0.png)
![Screenshot 2024-04-01 at 11.47.11 PM](https://hackmd.io/_uploads/B1I9vId1A.png)

## 3. Simulate wifi-80211e-txop.cc
```javascript=
./ns3 run examples/wireless/wifi-80211e-txop.cc
```
![Screenshot 2024-04-01 at 11.53.03 PM](https://hackmd.io/_uploads/HJe2uUdyA.png)

## 4. Simulate wifi-hidden-terminal.cc
:::info
**Introduce :**

- Simulated hidden stations and tested the scenarios with RTS/CTS enabled and disabled.
:::

![1*gD7sywJBBnvfgAHdghHa0A](https://hackmd.io/_uploads/HJhEu_Wg0.png)


### 4-1. Configuration Parameters Setting

- source code of wifi-hidden-terminal.cc

#### 4-1-1. Setting Node number
![Screenshot 2024-04-08 at 8.22.52 PM](https://hackmd.io/_uploads/r1ZnbPZgA.png)

- This part let you can chenge the node you wanted to created.
    - **At 64.** nodes.Create(**Node you want to simluate**)
    - **At 67.** Revise the for loop parameter according to your node number.
    - **At 75. ~ 80.** Setting the nodes.Get().
    - EX : If I have four node, the code need to revise as below. 
```javascript=
experiment(bool enableCtsRts, std::string wifiManager)
{
    // 0. Enable or disable CTS/RTS
    UintegerValue ctsThr = (enableCtsRts ? UintegerValue(100) : UintegerValue(2200));
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

    // 1. Create 3 nodes
    NodeContainer nodes;
    nodes.Create(5);

    // 2. Place nodes somehow, this is required by every wireless simulation
    for (uint8_t i = 0; i < 5; ++i)
    {
        nodes.Get(i)->AggregateObject(CreateObject<ConstantPositionMobilityModel>());
    }

    // 3. Create propagation loss matrix
    Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel>();
    lossModel->SetDefaultLoss(200); // set default loss to 200 dB (no link)
    lossModel->SetLoss(nodes.Get(0)->GetObject<MobilityModel>(),
                       nodes.Get(1)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 0 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(2)->GetObject<MobilityModel>(),
                       nodes.Get(1)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(2)->GetObject<MobilityModel>(),
                       nodes.Get(3)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(3)->GetObject<MobilityModel>(),
                       nodes.Get(4)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB
```
![Screenshot 2024-04-08 at 11.45.22 PM](https://hackmd.io/_uploads/Sy5XZqWlC.png)


#### 4-1-2. Setting Wifi standard
![Screenshot 2024-04-08 at 8.44.17 PM](https://hackmd.io/_uploads/S1UaLvWeC.png)

- This part you can change the standard to below selection.
    - **WIFI_STANDARD_80211a**
    - **WIFI_STANDARD_80211b**

#### 4-1-3. Setting the default IP for your node

![Screenshot 2024-04-08 at 8.54.16 PM](https://hackmd.io/_uploads/BJpWFDZeR.png)

- **At 110.** Set your default IP address && subnet mask.
    - EX : ("10.0.0.24", "255.255.255.0")

#### 4-1-4. Setting middle node to simluate

![Screenshot 2024-04-08 at 9.14.53 PM](https://hackmd.io/_uploads/BJSHCwZeR.png)

- middel node IP : **10.0.0.2** 

![Screenshot 2024-04-08 at 11.46.43 PM](https://hackmd.io/_uploads/BkFtWqWeR.png)


#### 4-1-5. Setting flow start time && data rate

![Screenshot 2024-04-08 at 9.01.51 PM](https://hackmd.io/_uploads/SJw1iPbg0.png)

- **At 123. ~ 124** Set your first flow config.
    - Data Rate : 3000000bps
    - Start Time : 1.000000 sec
    - EX : If you have five nodes.

```javascript=
// flow 1:  node 0 -> node 1
onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
cbrApps.Add(onOffHelper.Install(nodes.Get(0)));

// flow 2:  node 2 -> node 1
onOffHelper.SetAttribute("DataRate", StringValue("3001100bps"));
onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
cbrApps.Add(onOffHelper.Install(nodes.Get(2)));

// flow 3:  node 2 -> node 1
onOffHelper.SetAttribute("DataRate", StringValue("30514100bps"));
onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
cbrApps.Add(onOffHelper.Install(nodes.Get(3)));

// flow 3:  node 4 -> node 1
onOffHelper.SetAttribute("DataRate", StringValue("30551100bps"));
onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
cbrApps.Add(onOffHelper.Install(nodes.Get(4)));
```

![Screenshot 2024-04-08 at 11.41.22 PM](https://hackmd.io/_uploads/rJ6Nx5bgA.png)


### 4-2. Statistics of the simulation
![Screenshot 2024-04-08 at 9.39.39 PM](https://hackmd.io/_uploads/r10o7_-gR.png)

- **At. 181~188** This part will print the collection of the statistics in the simulation, when the node1 first transimited the data to the node2, the simulator will print the Tx, Rx, and there are some matrics you can collect :
    - **Tx Packets**
    - **Tx Bytes**
    - **TxOffered**
    - **Rx Packets**
    - **Rx Bytes**
    - **Throughput**

### 4-3. Simulate the result
#### 4-3-1. Default (3 Node)
```javascript=
./ns3 run examples/wireless/wifi-hidden-terminal.cc
```
![Screenshot 2024-04-08 at 8.04.28 PM](https://hackmd.io/_uploads/HkT8T8Wx0.png)

:::info
**Conclusion :**

Based on the usage of CTS/RTS and non-usage, in flow 1 (node1 -> node2) we set the same Tx Packets: 2410, Tx Bytes: 3441480, TxOffered: 3.05909 Mbps, and similarly in flow 2 (node3 -> node2). You can see that when CTS/RTS mechanism is not used, the overall throughput is very low and fewer packets are received compared to when CTS/RTS is enabled. However, when CTS/RTS is enabled, there is a significant improvement in throughput, and the runtime is longer, indicating the importance of this mechanism during transmission between both parties.
:::

#### 4-3-2. (5 Node) Simulation
##### 4-3-2-1. Architecture
![Screenshot 2024-04-08 at 11.55.45 PM](https://hackmd.io/_uploads/ryK7N9-e0.png)

##### 4-3-2-2. Simulation Result
![Screenshot 2024-04-08 at 11.51.59 PM](https://hackmd.io/_uploads/ryfnzqZl0.png)

##### 4-3-2-3. Source Code

link : https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-M11202259-Jerry/wifi-hidden-terminal(5%20Nodes).cc

```javascript=
#include "ns3/boolean.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/on-off-helper.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/string.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/uinteger.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

void
experiment(bool enableCtsRts, std::string wifiManager)
{
    // 0. Enable or disable CTS/RTS
    UintegerValue ctsThr = (enableCtsRts ? UintegerValue(100) : UintegerValue(2200));
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

    // 1. Create 3 nodes
    NodeContainer nodes;
    nodes.Create(5);

    // 2. Place nodes somehow, this is required by every wireless simulation
    for (uint8_t i = 0; i < 5; ++i)
    {
        nodes.Get(i)->AggregateObject(CreateObject<ConstantPositionMobilityModel>());
    }

    // 3. Create propagation loss matrix
    Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel>();
    lossModel->SetDefaultLoss(200); // set default loss to 200 dB (no link)
    lossModel->SetLoss(nodes.Get(0)->GetObject<MobilityModel>(),
                       nodes.Get(1)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 0 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(2)->GetObject<MobilityModel>(),
                       nodes.Get(1)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(2)->GetObject<MobilityModel>(),
                       nodes.Get(3)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(3)->GetObject<MobilityModel>(),
                       nodes.Get(4)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB

    // 4. Create & setup wifi channel
    Ptr<YansWifiChannel> wifiChannel = CreateObject<YansWifiChannel>();
    wifiChannel->SetPropagationLossModel(lossModel);
    wifiChannel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

    // 5. Install wireless devices
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);
    wifi.SetRemoteStationManager("ns3::" + wifiManager + "WifiManager");
    YansWifiPhyHelper wifiPhy;
    wifiPhy.SetChannel(wifiChannel);
    WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac"); // use ad-hoc MAC
    NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);

    // 6. Install TCP/IP stack & assign IP addresses
    InternetStackHelper internet;
    internet.Install(nodes);
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.0.0.0", "255.0.0.0");
    ipv4.Assign(devices);

    // 7. Install applications: two CBR streams each saturating the channel
    ApplicationContainer cbrApps;
    uint16_t cbrPort = 12345;
    OnOffHelper onOffHelper("ns3::UdpSocketFactory",
                            InetSocketAddress(Ipv4Address("10.0.0.2"), cbrPort));
    onOffHelper.SetAttribute("PacketSize", UintegerValue(1400));
    onOffHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    // flow 1:  node 0 -> node 1
    onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
    cbrApps.Add(onOffHelper.Install(nodes.Get(0)));

    // flow 2:  node 2 -> node 1
    /** \internal
     * The slightly different start times and data rates are a workaround
     * for \bugid{388} and \bugid{912}
     */
    onOffHelper.SetAttribute("DataRate", StringValue("3001100bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
    cbrApps.Add(onOffHelper.Install(nodes.Get(2)));

    onOffHelper.SetAttribute("DataRate", StringValue("4475200bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(2.001)));
    cbrApps.Add(onOffHelper.Install(nodes.Get(3)));

    onOffHelper.SetAttribute("DataRate", StringValue("3452100bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(3.001)));
    cbrApps.Add(onOffHelper.Install(nodes.Get(4)));

    /** \internal
     * We also use separate UDP applications that will send a single
     * packet before the CBR flows start.
     * This is a workaround for the lack of perfect ARP, see \bugid{187}
     */
    uint16_t echoPort = 9;
    UdpEchoClientHelper echoClientHelper(Ipv4Address("10.0.0.2"), echoPort);
    echoClientHelper.SetAttribute("MaxPackets", UintegerValue(1));
    echoClientHelper.SetAttribute("Interval", TimeValue(Seconds(0.1)));
    echoClientHelper.SetAttribute("PacketSize", UintegerValue(10));
    ApplicationContainer pingApps;

    // again using different start times to workaround Bug 388 and Bug 912
    echoClientHelper.SetAttribute("StartTime", TimeValue(Seconds(0.001)));
    pingApps.Add(echoClientHelper.Install(nodes.Get(0)));
    echoClientHelper.SetAttribute("StartTime", TimeValue(Seconds(0.006)));
    pingApps.Add(echoClientHelper.Install(nodes.Get(2)));

    // 8. Install FlowMonitor on all nodes
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // 9. Run simulation for 10 seconds
    Simulator::Stop(Seconds(10));
    Simulator::Run();

    // 10. Print per flow statistics
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();
         i != stats.end();
         ++i)
    {
        if (i->first > 2)
        {
            Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
            std::cout << "Flow " << i->first - 2 << " (" << t.sourceAddress << " -> "
                      << t.destinationAddress << ")\n";
            std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
            std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
            std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000
                      << " Mbps\n";
            std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
            std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
            std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000
                      << " Mbps\n";
        }
    }

    // 11. Cleanup
    Simulator::Destroy();
}

int
main(int argc, char** argv)
{
    std::string wifiManager("Arf");
    CommandLine cmd(__FILE__);
    cmd.AddValue(
        "wifiManager",
        "Set wifi rate manager (Aarf, Aarfcd, Amrr, Arf, Cara, Ideal, Minstrel, Onoe, Rraa)",
        wifiManager);
    cmd.Parse(argc, argv);

    std::cout << "Hidden station experiment with RTS/CTS disabled:\n" << std::flush;
    experiment(false, wifiManager);
    std::cout << "------------------------------------------------\n";
    std::cout << "Hidden station experiment with RTS/CTS enabled:\n";
    experiment(true, wifiManager);
    return 0;
}
```

#### 4-3-3. (5 Node, 2 Access Point) Simulation

##### 4-3-3-1. Architecture

![Screenshot 2024-04-09 at 12.37.06 AM](https://hackmd.io/_uploads/r1rBT9WeA.png)


##### 4-3-3-2. Simulation Result

![Screenshot 2024-04-09 at 12.33.09 AM](https://hackmd.io/_uploads/SyUL29ZxR.png)
![Screenshot 2024-04-09 at 12.33.30 AM](https://hackmd.io/_uploads/rJiD3cblR.png)

##### 4-3-3-3. Source Code

link : https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-M11202259-Jerry/wifi-hidden-terminal(two%20station%20with%205%20nodes).cc

```javascript=
#include "ns3/boolean.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/constant-position-mobility-model.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/on-off-helper.h"
#include "ns3/propagation-delay-model.h"
#include "ns3/propagation-loss-model.h"
#include "ns3/string.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/uinteger.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

/**
 * Run single 10 seconds experiment
 *
 * \param enableCtsRts if true, enable RTS/CTS for packets larger than 100 bytes.
 * \param wifiManager WiFi manager to use.
 */
void
experiment(bool enableCtsRts, std::string wifiManager)
{
    // 0. Enable or disable CTS/RTS
    UintegerValue ctsThr = (enableCtsRts ? UintegerValue(100) : UintegerValue(2200));
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

    // 1. Create 3 nodes
    NodeContainer nodes;
    nodes.Create(5);

    // 2. Place nodes somehow, this is required by every wireless simulation
    for (uint8_t i = 0; i < 5; ++i)
    {
        nodes.Get(i)->AggregateObject(CreateObject<ConstantPositionMobilityModel>());
    }

    // 3. Create propagation loss matrix
    Ptr<MatrixPropagationLossModel> lossModel = CreateObject<MatrixPropagationLossModel>();
    lossModel->SetDefaultLoss(200); // set default loss to 200 dB (no link)
    lossModel->SetLoss(nodes.Get(0)->GetObject<MobilityModel>(),
                       nodes.Get(1)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 0 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(2)->GetObject<MobilityModel>(),
                       nodes.Get(1)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(2)->GetObject<MobilityModel>(),
                       nodes.Get(3)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB
    lossModel->SetLoss(nodes.Get(4)->GetObject<MobilityModel>(),
                       nodes.Get(3)->GetObject<MobilityModel>(),
                       50); // set symmetric loss 2 <-> 1 to 50 dB

    // 4. Create & setup wifi channel
    Ptr<YansWifiChannel> wifiChannel = CreateObject<YansWifiChannel>();
    wifiChannel->SetPropagationLossModel(lossModel);
    wifiChannel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

    // 5. Install wireless devices
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);
    wifi.SetRemoteStationManager("ns3::" + wifiManager + "WifiManager");
    YansWifiPhyHelper wifiPhy;
    wifiPhy.SetChannel(wifiChannel);
    WifiMacHelper wifiMac;
    wifiMac.SetType("ns3::AdhocWifiMac"); // use ad-hoc MAC
    NetDeviceContainer devices = wifi.Install(wifiPhy, wifiMac, nodes);

    // uncomment the following to have athstats output
    // AthstatsHelper athstats;
    // athstats.EnableAthstats(enableCtsRts ? "rtscts-athstats-node" : "basic-athstats-node" ,
    // nodes);

    // uncomment the following to have pcap output
    // wifiPhy.SetPcapDataLinkType (WifiPhyHelper::DLT_IEEE802_11_RADIO);
    // wifiPhy.EnablePcap (enableCtsRts ? "rtscts-pcap-node" : "basic-pcap-node" , nodes);

    // 6. Install TCP/IP stack & assign IP addresses
    InternetStackHelper internet;
    internet.Install(nodes);
    Ipv4AddressHelper ipv4;
    ipv4.SetBase("10.0.0.0", "255.0.0.0");
    ipv4.Assign(devices);

    // 7. Install applications: two CBR streams each saturating the channel
    ApplicationContainer cbrApps, cbrApps2;
    uint16_t cbrPort = 12345;
    uint16_t cbrPort2 = 45454;
    OnOffHelper onOffHelper("ns3::UdpSocketFactory",
                            InetSocketAddress(Ipv4Address("10.0.0.2"), cbrPort));
    onOffHelper.SetAttribute("PacketSize", UintegerValue(1400));
    onOffHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    OnOffHelper onOffHelper1("ns3::UdpSocketFactory",
                            InetSocketAddress(Ipv4Address("10.0.0.4"), cbrPort2));
    onOffHelper1.SetAttribute("PacketSize", UintegerValue(1400));
    onOffHelper1.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper1.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    // flow 1:  node 0 -> node 1
    onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
    cbrApps.Add(onOffHelper.Install(nodes.Get(0)));

    // flow 2:  node 2 -> node 1
    /** \internal
     * The slightly different start times and data rates are a workaround
     * for \bugid{388} and \bugid{912}
     */
    onOffHelper.SetAttribute("DataRate", StringValue("3082200bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
    cbrApps.Add(onOffHelper.Install(nodes.Get(2)));

    /** \internal
     * We also use separate UDP applications that will send a single
     * packet before the CBR flows start.
     * This is a workaround for the lack of perfect ARP, see \bugid{187}
     */
    
    onOffHelper1.SetAttribute("DataRate", StringValue("3004120bps"));
    onOffHelper1.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
    cbrApps2.Add(onOffHelper1.Install(nodes.Get(2)));

    onOffHelper1.SetAttribute("DataRate", StringValue("3004890bps"));
    onOffHelper1.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
    cbrApps2.Add(onOffHelper1.Install(nodes.Get(4)));

    uint16_t echoPort = 9;
    UdpEchoClientHelper echoClientHelper(Ipv4Address("10.0.0.2"), echoPort);
    echoClientHelper.SetAttribute("MaxPackets", UintegerValue(1));
    echoClientHelper.SetAttribute("Interval", TimeValue(Seconds(0.1)));
    echoClientHelper.SetAttribute("PacketSize", UintegerValue(10));
    ApplicationContainer pingApps;

    uint16_t echoPort1 =10;
    UdpEchoClientHelper echoClientHelper1(Ipv4Address("10.0.0.4"), echoPort1);
    echoClientHelper1.SetAttribute("MaxPackets", UintegerValue(1));
    echoClientHelper1.SetAttribute("Interval", TimeValue(Seconds(0.1)));
    echoClientHelper1.SetAttribute("PacketSize", UintegerValue(10));
    ApplicationContainer pingApps1;

    // again using different start times to workaround Bug 388 and Bug 912
    echoClientHelper.SetAttribute("StartTime", TimeValue(Seconds(0.001)));
    pingApps.Add(echoClientHelper.Install(nodes.Get(0)));
    echoClientHelper.SetAttribute("StartTime", TimeValue(Seconds(0.006)));
    pingApps.Add(echoClientHelper.Install(nodes.Get(2)));

    // again using different start times to workaround Bug 388 and Bug 912
    echoClientHelper1.SetAttribute("StartTime", TimeValue(Seconds(0.012)));
    pingApps1.Add(echoClientHelper1.Install(nodes.Get(2)));
    echoClientHelper.SetAttribute("StartTime", TimeValue(Seconds(0.017)));
    pingApps1.Add(echoClientHelper1.Install(nodes.Get(4)));

    // 8. Install FlowMonitor on all nodes
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // 9. Run simulation for 10 seconds
    Simulator::Stop(Seconds(10));
    Simulator::Run();

    // 10. Print per flow statistics
    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();
         i != stats.end();
         ++i)
    {
        // first 2 FlowIds are for ECHO apps, we don't want to display them
        //
        // Duration for throughput measurement is 9.0 seconds, since
        //   StartTime of the OnOffApplication is at about "second 1"
        // and
        //   Simulator::Stops at "second 10".
        if (i->first > 2)
        {
            Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
            std::cout << "Flow " << i->first - 2 << " (" << t.sourceAddress << " -> "
                      << t.destinationAddress << ")\n";
            std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
            std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
            std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000
                      << " Mbps\n";
            std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
            std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
            std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000
                      << " Mbps\n";
        }
    }

    // 11. Cleanup
    Simulator::Destroy();
}

int
main(int argc, char** argv)
{
    std::string wifiManager("Arf");
    CommandLine cmd(__FILE__);
    cmd.AddValue(
        "wifiManager",
        "Set wifi rate manager (Aarf, Aarfcd, Amrr, Arf, Cara, Ideal, Minstrel, Onoe, Rraa)",
        wifiManager);
    cmd.Parse(argc, argv);

    std::cout << "Hidden station experiment with RTS/CTS disabled:\n" << std::flush;
    experiment(false, wifiManager);
    std::cout << "------------------------------------------------\n";
    std::cout << "Hidden station experiment with RTS/CTS enabled:\n";
    experiment(true, wifiManager);
    return 0;
}
```