# Introduction

In this Project we have to create a WiFi Simmulation using ns-3. The goal is to create a network that implements the 802.11 standard, and try to replicate the results of the Research Paper provided.

## Research Paper

In the simulation discussed in this [research paper](https://ieeexplore.ieee.org/document/1265851), all frames are transmitted at 24Mb/s. And the simulation consists of a single Access Point (AP) transmitting four streams to each of  the three stations. 

Research Paper Results:
![image](https://hackmd.io/_uploads/HyN0-rZlC.png)


## Implementation of IEEE 802.11

To implement the IEEE 802.11, I utilized an example code provided in the simulator as a reference.

The example code consisted of a 4 stations and 4 Access Points (AP) Network. So I had to modify this code and create a network that would consist of one Access Point and 3 stations, all recieveing packets from the AP (just like the simulation in the research paper).

### Network Topology

- Access Point (AP): One access point node (wifiApNodes) is created.
- Stations (STAs): Three station nodes (wifiStaNodes) are created.

```
             AP
   *---------*---------*
   |         |         |        
 STA A     STA B     STA C
```



### Code

```cpp
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
#include "ns3/packet.h"
#include "ns3/udp-client.h"
#include "ns3/flow-monitor.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/flow-monitor-helper.h"



// This is an example that illustrates 802.11
// The network contains one access point and three stations. Each station continuously
// transmits data packets to the AP.
//
// Network topology :
//
//             AP
//   *---------*----------*
//   |         |          |        
//  STA A     STA B     STA C
//
// The output prints the throughput.

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211eTxop");

/**
 * Keeps the maximum duration among all TXOPs
 */
struct TxopDurationTracer
{
    /**
     * Callback connected to TXOP duration trace source.
     *
     * \param startTime TXOP start time
     * \param duration TXOP duration
     * \param linkId the ID of the link
     */
    void Trace(Time startTime, Time duration, uint8_t linkId);
    Time m_max{Seconds(0)}; //!< maximum TXOP duration
};

// Define variables to store send and receive times
std::map<uint32_t, Time> sendTimes;
std::map<uint32_t, Time> receiveTimes;




// Function to record the send time of each packet
void PacketSend(uint32_t sequenceNumber)
{
    sendTimes[sequenceNumber] = Simulator::Now();
}

void PacketRecieved(uint32_t sequenceNumber)
{
    receiveTimes[sequenceNumber] = Simulator::Now();
}


void
TxopDurationTracer::Trace(Time startTime, Time duration, uint8_t linkId)
{
    if (duration > m_max)
    {
        m_max = duration;
    }
}

int
main(int argc, char* argv[])
{
    uint32_t payloadSize = 1472; // bytes
    double simulationTime = 10;  // seconds
    double distance = 5;         // meters
    bool enablePcap = false;
    bool verifyResults = false; // used for regression
    Time txopLimit = MicroSeconds(4096);

    FlowMonitorHelper flowmonHelper;

    //Ptr<FlowMonitor> flowmon = flowmonHelper.InstallAll();

    Ptr<FlowMonitor> flowmon;
    flowmon = CreateObject<FlowMonitor>();


    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon);
    std::map<FlowId, FlowMonitor::FlowStats> stats = flowmon->GetFlowStats();

    Time Delay;


    CommandLine cmd(__FILE__);
    cmd.AddValue("payloadSize", "Payload size in bytes", payloadSize);
    cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
    cmd.AddValue("distance",
                 "Distance in meters between the station and the access point",
                 distance);
    cmd.AddValue("enablePcap", "Enable/disable pcap file generation", enablePcap);
    cmd.AddValue("verifyResults",
                 "Enable/disable results verification at the end of the simulation",
                 verifyResults);
    cmd.Parse(argc, argv);

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

    // Network A
    ssid = Ssid("network-A");
    phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));
    mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
    staDeviceA = wifi.Install(phy, mac, wifiStaNodes.Get(0));
    staDeviceB = wifi.Install(phy, mac, wifiStaNodes.Get(1));
    staDeviceB = wifi.Install(phy, mac, wifiStaNodes.Get(2));

    mac.SetType("ns3::ApWifiMac",
                "QosSupported",
                BooleanValue(true),
                "Ssid",
                SsidValue(ssid),
                "EnableBeaconJitter",
                BooleanValue(false));
    apDeviceA = wifi.Install(phy, mac, wifiApNodes.Get(0));


    /* Setting mobility model */
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    // Set position for APs
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));
    // Set position for STAs
    positionAlloc->Add(Vector(distance, 0.0, 0.0));
    positionAlloc->Add(Vector(10 + distance, 0.0, 0.0));
    positionAlloc->Add(Vector(20 + distance, 0.0, 0.0));
    // Remark: while we set these positions 10 meters apart, the networks do not interact
    // and the only variable that affects transmission performance is the distance.

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
    Ipv4InterfaceContainer StaInterfaceB;
    Ipv4InterfaceContainer StaInterfaceC;
    StaInterfaceA = address.Assign(staDeviceA);
    StaInterfaceB = address.Assign(staDeviceB);
    StaInterfaceC = address.Assign(staDeviceC);
    Ipv4InterfaceContainer ApInterfaceA;
    ApInterfaceA = address.Assign(apDeviceA);

    
    /* Setting applications */
    uint16_t port = 5001;
    UdpServerHelper serverA(port);
    ApplicationContainer serverAppA = serverA.Install(wifiApNodes.Get(0));
    serverAppA.Start(Seconds(0.0));
    serverAppA.Stop(Seconds(simulationTime + 1));

    InetSocketAddress destA(ApInterfaceA.GetAddress(0), port);
    destA.SetTos(0x70); // AC_BE

    OnOffHelper clientA("ns3::UdpSocketFactory", destA);
    clientA.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientA.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientA.SetAttribute("DataRate", StringValue("100000kb/s"));
    clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));


    ApplicationContainer clientAppA = clientA.Install(wifiStaNodes.Get(0));
    clientAppA.Start(Seconds(1.0));
    clientAppA.Stop(Seconds(simulationTime + 1));



    if (enablePcap)
    {
        phy.EnablePcap("AP_A", apDeviceA.Get(0));
        phy.EnablePcap("STA_A", staDeviceA.Get(0));
        phy.EnablePcap("STA_B", staDeviceB.Get(0));
        phy.EnablePcap("STA_C", staDeviceC.Get(0));
    }

    Simulator::Stop(Seconds(simulationTime + 1));
    Simulator::Run();

    // Define a variable to store the total delay sum
    Time totalDelaySum = Seconds(0);

    // Iterate over the flow statistics
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin(); iter != stats.end(); ++iter)
    {
        // Add the delay sum of each flow to the total delay sum
        totalDelaySum += iter->second.delaySum;
        std::cout << "iter delay: " << iter->second.delaySum;
    }

    /* Show results */
    uint64_t totalPacketsThroughA = DynamicCast<UdpServer>(serverAppA.Get(0))->GetReceived();
    
    Simulator::Destroy();


    // Print the total delay sum
    std::cout << "Total Delay Sum: " << totalDelaySum << std::endl;


    double throughput = totalPacketsThroughA * payloadSize * 8 / (simulationTime * 1000000.0);
    std::cout << "AC_BE with default TXOP limit (0ms): " << '\n'
              << "  Throughput = " << throughput << " Mbit/s" << '\n';
    if (verifyResults && (throughput < 28 || throughput > 29))
    {
        NS_LOG_ERROR("Obtained throughput " << throughput << " is not in the expected boundaries!");
        exit(1);
    }

    return 0;
}

```

### Flowchart of simulation:

![image](https://hackmd.io/_uploads/SJmPNnWg0.png)


### Output

![image](https://hackmd.io/_uploads/Hy-n6a-gA.png)



## Hidden Nodes Simulation

The following code is a example of the ns-3 simulator, and it simulates the behavior of hidden stations in an 802.11 wireless network.

### Network Topology

- Access Point (AP): One access point node (wifiApNode) is created.
- Hidden Stations: Two hidden station nodes (wifiStaNodes) are created.

```
         AP
   *     *     *
   |     |     |
   n1    n2    n3
```


The stations n1 and n2 are positioned in such a way that they are beyond the wireless range of each other. This creates a scenario where the stations are "hidden" from each other, 

### Code


```cpp=
/*
 * Copyright (c) 2015 Sébastien Deronne
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Sébastien Deronne <sebastien.deronne@gmail.com>
 */

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

// This example considers two hidden stations in an 802.11n network which supports MPDU aggregation.
// The user can specify whether RTS/CTS is used and can set the number of aggregated MPDUs.
//
// Example: ./ns3 run "wifi-simple-ht-hidden-stations --enableRts=1 --nMpdus=8"
//
// Network topology:
//
//   Wifi 192.168.1.0
//
//        AP
//   *    *    *
//   |    |    |
//   n1   n2   n3
//
// Packets in this simulation belong to BestEffort Access Class (AC_BE).

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

```


### Flowchart


![image](https://hackmd.io/_uploads/S13RsaWeC.png)

### Output

![image](https://hackmd.io/_uploads/SJpf0TWxC.png)





