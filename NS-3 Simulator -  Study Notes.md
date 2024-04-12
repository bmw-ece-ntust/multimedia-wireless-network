 # NS-3 Simulator -  Study Notes

### :question: Purpose
:::success
The following Study Note has the goal in share my learnings about NS-3 Simulator and show some development made it.
::: 

:::info
## Outline
**Part 1: Study**
1. About NS-3 Simulator
1. Installation Guide

**Part 2: Simulation**
1. Goal
1. Code 
    * Code 1: Simulation with Hidden Nodes 
    * Code 2: Simulation with 802.11 
1. Flow Chart
1. Learning 

**References**
:::

## Part 1: Study 
 
### :book:About NS-3 Simulator
Ns-3 is like a computer program that pretends to be a network. It keeps a list of things that need to happen at certain times. 

### :feet: Installation Guide (Steps)
- ***Simulator version***: NS3-3.41
- ***Operating System***: Linux Ubuntu kernel version 6.5.0-26-generic 
 
:::success
**Step 1:** Run the commands into the terminal
:::
```
sudo apt update.
```
```
sudo apt install build-essential autoconf automake libxmu-dev.
```
```
sudo apt install g++ python3 cmake ninja-build git.
```

:::danger
**Step 2:** Run the command for the following additional packages
:::

:bulb: In order to avoid some bugs during runnig time, we install the following packages.

`$ sudo apt install g++ python3 cmake ninja-build git gir1.2-goocanvas-2.0 python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython3 tcpdump wireshark sqlite sqlite3 libsqlite3-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools openmpi-bin openmpi-common openmpi-doc libopenmpi-dev doxygen graphviz imagemagick python3-sphinx dia imagemagick texlive dvipng latexmk texlive-extra-utils texlive-latex-extra texlive-font-utils libeigen3-dev gsl-bin libgsl-dev libgslcblas0 libxml2 libxml2-dev libgtk-3-dev lxc-utils lxc-templates vtun uml-utilities ebtables bridge-utils libxml2 libxml2-dev libboost-all-dev`

**Step 3:** From the page of ns3 (https://www.nsnam.org) download the latest version (3.41) from the date.

**Step 4:** Copy the downloaded zip (software) into "Home" in Linux.

**Step 5:** Extract the downloaded zip, which gives a folder with the name:  ns-allionone-3.40/


**Step 6:** From a new terminal, access to the new folder that is in the home directory: *cd ns-allionone-3.40/*

**Step 7:** Execute the following prompt: *./build.py --enable-examples --enable-tests

:::success
In order to check if everything is okay, we open a new terminal
:::

**Step 8:** Access to the directory cd  ns-allionone-3.40/ns-3.40

**Step 9:** Execute ./ns3 run hello-simulator
 **Output**: Hello Simulator
        
**Step 10:** ./ns3 run first.cc


## **Part 2: Simulation**

### Goal :checkered_flag: 
Veriyfy the ideas learned through the simulator.

### Code :alien: 
In order to understand more the simulations and also the assignments, two codes are going to be provided in order to open a discussion about them. 

:bulb: *Referential Image*
![image](https://hackmd.io/_uploads/HynJSlfeR.png)

### **Code 1: Simulation with Hidden Nodes**

In order to increase efficiency, 802.11n networks employ a technique called MPDU aggregation, or MAC Protocol Data Unit aggregation, which combines several MAC frames into a single transmission unit. Because there are fewer frame headers and inter-frame intervals, throughput is increased and overhead is decreased.

In wireless networks, the RTS/CTS (Request to Send/Clear to Send) method is employed to lessen the chance of collisions and the hidden node issue. When enabled, a station asks the access point (AP) for permission to communicate data by sending an RTS frame. Permission is granted by the AP through a CTS frame in response. This lowers the possibility of accidents by guaranteeing that other stations in the sender's transmission range will postpone transmission.

* **RTS/CTS**
  If enabled, before transmitting data, a station will send an RTS frame to the AP requesting permission to send. The AP may respond with a CTS frame granting permission. This helps to reduce collisions, especially in scenarios where hidden nodes might otherwise cause interference.

* **Number of Aggregated MPDUs**
 MPDU aggregation allows multiple MAC frames to be combined into a single transmission unit. The user can specify how many MAC frames are aggregated together before transmission. This increases efficiency by reducing the overhead associated with transmitting individual frames.
```cpp=
#include "ns3/boolean.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/double.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/log.h"
#include "ns3/mobility-helper.h"
#include "ns3/rng-seed-manager.h"
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/udp-server.h"
#include "ns3/uinteger.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimplesHtHiddenStations");

int
main(int argc, char* argv[])
{
    uint32_t payloadSize = 1472; // bytes
    double simulationTime = 10;  // seconds
    uint32_t nMpdus = 1;
    uint32_t maxAmpduSize = 0;
    bool enableRts = false;
    double minExpectedThroughput = 0;
    double maxExpectedThroughput = 0;

    RngSeedManager::SetSeed(1);
    RngSeedManager::SetRun(5);

    CommandLine cmd(__FILE__);
    cmd.AddValue("nMpdus", "Number of aggregated MPDUs", nMpdus);
    cmd.AddValue("payloadSize", "Payload size in bytes", payloadSize);
    cmd.AddValue("enableRts", "Enable RTS/CTS", enableRts);
    cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
    cmd.AddValue("minExpectedThroughput",
                 "if set, simulation fails if the lowest throughput is below this value",
                 minExpectedThroughput);
    cmd.AddValue("maxExpectedThroughput",
                 "if set, simulation fails if the highest throughput is above this value",
                 maxExpectedThroughput);
    cmd.Parse(argc, argv);

    if (!enableRts)
    {
        Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("999999"));
    }
    else
    {
        Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", StringValue("0"));
    }

    // Set the maximum size for A-MPDU with regards to the payload size
    maxAmpduSize = nMpdus * (payloadSize + 200);

    // Set the maximum wireless range to 5 meters in order to reproduce a hidden nodes scenario,
    // i.e. the distance between hidden stations is larger than 5 meters
    Config::SetDefault("ns3::RangePropagationLossModel::MaxRange", DoubleValue(5));

    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(2);
    NodeContainer wifiApNode;
    wifiApNode.Create(1);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    channel.AddPropagationLoss(
        "ns3::RangePropagationLossModel"); // wireless range limited to 5 meters!

    YansWifiPhyHelper phy;
    phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
    phy.SetChannel(channel.Create());
    phy.Set("ChannelSettings", StringValue("{36, 0, BAND_5GHZ, 0}"));

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211n);
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode",
                                 StringValue("HtMcs7"),
                                 "ControlMode",
                                 StringValue("HtMcs0"));
    WifiMacHelper mac;

    Ssid ssid = Ssid("simple-mpdu-aggregation");
    mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid));

    NetDeviceContainer staDevices;
    staDevices = wifi.Install(phy, mac, wifiStaNodes);

    mac.SetType("ns3::ApWifiMac",
                "Ssid",
                SsidValue(ssid),
                "EnableBeaconJitter",
                BooleanValue(false));

    NetDeviceContainer apDevice;
    apDevice = wifi.Install(phy, mac, wifiApNode);

    Config::Set("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Mac/BE_MaxAmpduSize",
                UintegerValue(maxAmpduSize));

    // Setting mobility model
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();

    // AP is between the two stations, each station being located at 5 meters from the AP.
    // The distance between the two stations is thus equal to 10 meters.
    // Since the wireless range is limited to 5 meters, the two stations are hidden from each other.
    positionAlloc->Add(Vector(5.0, 0.0, 0.0));
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));
    positionAlloc->Add(Vector(10.0, 0.0, 0.0));
    mobility.SetPositionAllocator(positionAlloc);

    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    mobility.Install(wifiApNode);
    mobility.Install(wifiStaNodes);

    // Internet stack
    InternetStackHelper stack;
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);

    Ipv4AddressHelper address;
    address.SetBase("192.168.1.0", "255.255.255.0");
    Ipv4InterfaceContainer StaInterface;
    StaInterface = address.Assign(staDevices);
    Ipv4InterfaceContainer ApInterface;
    ApInterface = address.Assign(apDevice);

    // Setting applications
    uint16_t port = 9;
    UdpServerHelper server(port);
    ApplicationContainer serverApp = server.Install(wifiApNode);
    serverApp.Start(Seconds(0.0));
    serverApp.Stop(Seconds(simulationTime + 1));

    UdpClientHelper client(ApInterface.GetAddress(0), port);
    client.SetAttribute("MaxPackets", UintegerValue(4294967295U));
    client.SetAttribute("Interval", TimeValue(Time("0.0001"))); // packets/s
    client.SetAttribute("PacketSize", UintegerValue(payloadSize));

    // Saturated UDP traffic from stations to AP
    ApplicationContainer clientApp1 = client.Install(wifiStaNodes);
    clientApp1.Start(Seconds(1.0));
    clientApp1.Stop(Seconds(simulationTime + 1));

    phy.EnablePcap("SimpleHtHiddenStations_Ap", apDevice.Get(0));
    phy.EnablePcap("SimpleHtHiddenStations_Sta1", staDevices.Get(0));
    phy.EnablePcap("SimpleHtHiddenStations_Sta2", staDevices.Get(1));

    AsciiTraceHelper ascii;
    phy.EnableAsciiAll(ascii.CreateFileStream("SimpleHtHiddenStations.tr"));

    Simulator::Stop(Seconds(simulationTime + 1));

    Simulator::Run();

    uint64_t totalPacketsThrough = DynamicCast<UdpServer>(serverApp.Get(0))->GetReceived();

    Simulator::Destroy();

    double throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
    std::cout << "Throughput: " << throughput << " Mbit/s" << '\n';
    if (throughput < minExpectedThroughput ||
        (maxExpectedThroughput > 0 && throughput > maxExpectedThroughput))
    {
        NS_LOG_ERROR("Obtained throughput " << throughput << " is not in the expected boundaries!");
        exit(1);
    }
    return 0;
}
];
```
**Output:**
![image](https://hackmd.io/_uploads/Hk3G6kGeC.png)

**Assumptions in the Simulation**
We can  assumes the presence of hidden nodes. This is evident from the setup where two stations are placed at a distance greater than the wireless range from each other. This arrangement creates a scenario where the stations are hidden from each other, leading to potential interference.

**Transmission Range and Sensing Range**
The transmission range is implicitly defined by the configuration of the wireless channel, which is set to a maximum range of 5 meters. This defines the range within which nodes can communicate directly. Talking about Sensing range, which typically refers to the range within which nodes can detect each other's transmissions, isn't in the  code.

**Channel Model**
The code does not explicitly model an error-free or error-prone channel. However, it configures parameters related to MPDU aggregation, RTS/CTS, payload size, etc., which indirectly affect the performance in the presence of channel errors. Additionally, it enables pcap tracing and ASCII trace, which can capture the details of packet transmissions and receptions, potentially including any channel errors.

### **Code 2: Simulation with 802.11**

The code 2 follow the idea of the topology presented at the paper [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://https://ieeexplore.ieee.org/document/1265851) . 
We got 3 stations with one access point, and the outputs are the satistical values like the trhoughput.

:::success
// Network topology:
//
//   Wifi 192.168.1.0
//
//        AP
//   *    *    *
//   |    |    |
//   n1   n2   n3
//
:::

```cpp=
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

NS_LOG_COMPONENT_DEFINE("80211");

int
main(int argc, char* argv[])
{
    uint32_t payloadSize = 1472; // bytes
    double simulationTime = 10;  // seconds
    bool enablePcap = false;
    bool verifyResults = false; // used for regression

    CommandLine cmd(__FILE__);
    cmd.AddValue("payloadSize", "Payload size in bytes", payloadSize);
    cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
    cmd.AddValue("enablePcap", "Enable/disable pcap file generation", enablePcap);
    cmd.AddValue("verifyResults",
                 "Enable/disable results verification at the end of the simulation",
                 verifyResults);
    cmd.Parse(argc, argv);


    // Defining the Nodes used for the simulation: 3 Stations and 1 Access Point
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(3);
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

    NetDeviceContainer staDeviceA;
    NetDeviceContainer staDeviceB;
    NetDeviceContainer staDeviceC;
    NetDeviceContainer apDeviceA;

    Ssid ssid;

    //Network definition
    ssid = Ssid("network-A");
    phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));
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

    //Preliminar QoS Settings

    /* Setting mobility model */
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    // Set position for APs
    positionAlloc->Add(Vector(0.0, 10.0, 0.0));
    // Set position for STAs
    positionAlloc->Add(Vector(-10, 0.0, 0.0));
    positionAlloc->Add(Vector(0, 0.0, 0.0));
    positionAlloc->Add(Vector(10, 0.0, 0.0));

    mobility.SetPositionAllocator(positionAlloc);
    mobility.Install(wifiApNodes);
    mobility.Install(wifiStaNodes);

    /* Internet stack */
    InternetStackHelper stack;
    stack.Install(wifiApNodes);
    stack.Install(wifiStaNodes);

    Ipv4AddressHelper address;
    address.SetBase("192.168.1.0", "255.255.255.0");
    Ipv4InterfaceContainer StaInterfaceA;
    StaInterfaceA = address.Assign(staDeviceA);

    Ipv4InterfaceContainer ApInterfaceA;
    ApInterfaceA = address.Assign(apDeviceA);

    Ipv4InterfaceContainer StaInterfaceB;
    StaInterfaceB = address.Assign(staDeviceB);

    Ipv4InterfaceContainer StaInterfaceC;
    StaInterfaceC = address.Assign(staDeviceC);

    /* Setting applications */
    uint16_t port = 5001;

    //Access Point definition

    UdpServerHelper serverA(port);
    ApplicationContainer serverAppA = serverA.Install(wifiApNodes.Get(0));
    serverAppA.Start(Seconds(0.0));
    serverAppA.Stop(Seconds(simulationTime + 1));

    InetSocketAddress destA(ApInterfaceA.GetAddress(0), port);
    destA.SetTos(0x70);

    //Client Definitions

    OnOffHelper clientA("ns3::UdpSocketFactory", destA);
    clientA.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientA.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientA.SetAttribute("DataRate", StringValue("100000kb/s"));
    clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));

    ApplicationContainer clientAppA = clientA.Install(wifiStaNodes.Get(0));
    clientAppA.Start(Seconds(1.0));
    clientAppA.Stop(Seconds(simulationTime + 1));

    OnOffHelper clientB("ns3::UdpSocketFactory", destA);
    clientB.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientB.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientB.SetAttribute("DataRate", StringValue("100000kb/s"));
    clientB.SetAttribute("PacketSize", UintegerValue(payloadSize));

    ApplicationContainer clientAppB = clientB.Install(wifiStaNodes.Get(1));
    clientAppB.Start(Seconds(1.0));
    clientAppB.Stop(Seconds(simulationTime + 1));

    OnOffHelper clientC("ns3::UdpSocketFactory", destA);
    clientC.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientC.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientC.SetAttribute("DataRate", StringValue("100000kb/s"));
    clientC.SetAttribute("PacketSize", UintegerValue(payloadSize));

    ApplicationContainer clientAppC = clientC.Install(wifiStaNodes.Get(2));
    clientAppC.Start(Seconds(1.0));
    clientAppC.Stop(Seconds(simulationTime + 1));

    Simulator::Stop(Seconds(simulationTime + 1));
    Simulator::Run();

    /* Show results */
    uint64_t totalPacketsThroughA = DynamicCast<UdpServer>(serverAppA.Get(0))->GetReceived();

    Simulator::Destroy();

    double throughput = totalPacketsThroughA * payloadSize * 8 / (simulationTime * 1000000.0);
    std::cout << "  Throughput = " << throughput << " Mbit/s" << '\n';
    if (verifyResults && (throughput < 28 || throughput > 29))
    {
        NS_LOG_ERROR("Obtained throughput " << throughput << " is not in the expected boundaries!");
        exit(1);
    }

    return 0;
}

```
**The output of the code would be:**

![code](https://hackmd.io/_uploads/Byx52pbe0.png)

**Collection of Simulation Statistics**

Simulation statistics, such as throughput, are collected. Throughput is calculated based on the total packets received by the UDP server application (totalPacketsThrough), payload size, and simulation time. This calculation is performed after the simulation is executed using *Simulator::Run()*.
If the throughput falls outside of the expected boundaries (configured using command-line arguments), an error message is logged, and the simulation exits. This is done to ensure that the obtained results meet the expected criteria.

### Flowchart
**Code 1**
![first](https://hackmd.io/_uploads/SkwFI0bgA.png)

 It establishes a wireless network, performs a simulation, determines throughput, or not, and checks the results if desired. After setting up the network and WiFi, a simulation is run to gauge data speed. Next, it asks if the user wishes to confirm the findings, and if so, it compares the actual values with the expected ones. At last, the show comes to a close.


**Code 2**

![second](https://hackmd.io/_uploads/ByAFURWeC.png)
The primary phases of the code, such as configuring WiFi, network configuration, Internet stack installation, IP address assignment, UDP server and client setup, simulation execution, throughput calculation, and result verification, are depicted in this flowchart. In the event that verification is selected, the program terminates and the throughput is compared to predetermined limits.
Statistics of the simulation (average delay, throughput, etc.)

### Parameters for the simulation
**code 1:**

![image](https://hackmd.io/_uploads/SJfJtCbxC.png)

Initialize key configuration parameters directly within the code for a simulation. Parameters like payloadSize and simulationTime are defined to set the size of data packets and the duration of the simulation, respectively. 

**code 2:**

![image](https://hackmd.io/_uploads/HJ0nuAZeR.png)
Initializes configuration parameters directly within the code for a simulation. Parameters such as payloadSize, simulationTime, nMpdus, enableRts, and minExpectedThroughput are defined, allowing users to adjust settings like packet size, simulation duration, RTS/CTS enablement, and expected throughput boundaries according to their specific requirements. This approach offers flexibility in configuring simulations without relying solely on command-line arguments, enabling precise control over the simulation environment.

### Learning 

I have found that NS3 simulator is an incredibly valuable tool for understanding and simulating wireless networks. One of the standout features for me is its flexibility and extensibility. I've been able to customize simulations according to specific scenarios and experiment with various network configurations easily. The availability of detailed documentation and extensive examples has been immensely helpful in grasping the concepts and getting started with simulations. Additionally, the active community surrounding ns-3 has been a great resource for asking questions, sharing insights, and learning from others' experiences. I appreciate how ns-3 allows me to visualize and analyze complex network behaviors in a controlled environment, providing valuable insights into network performance and behavior.



The primary phases of the code, such as configuring WiFi, network configuration, Internet stack installation, IP address assignment, UDP server and client setup, simulation execution, throughput calculation, and result verification, are depicted in this flowchart. In the event that verification is selected, the program terminates and the throughput is compared to predetermined limits.


## REFERENCES
1. [NS-3 Simulator](https://www.nsnam.org/docs/manual/html/events.html)
1. [Installation NS-3 Simulator.](https://www.youtube.com/watch?v=crIXDeRL3tE)
1. [Wifi Models](https://https://www.nsnam.org/docs/release/3.8/doxygen/group___wifi.html)
2. [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://https://ieeexplore.ieee.org/document/1265851)