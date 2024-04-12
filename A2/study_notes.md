<center>
    
Assignment 2
===
    
</center>

# 1. Introduction to NS-3

Network Simulator 3 (NS-3) is a discrete-event network simulator, widely utilized in academic and research settings for the design and analysis of communication protocols and network topologies. Unlike its predecessor, NS-2, NS-3 is not backward compatible and was designed with a more modular and flexible architecture in mind.

## 1.1 Key Features

- **Highly detailed network simulation**: NS-3 offers detailed simulations of individual network packets, their journey, and their handling, allowing for a deep understanding of network behavior.
- **Wide range of supported protocols and technologies**: It supports simulation of TCP, UDP, IPv4, IPv6, LTE, Wi-Fi, and more, making it versatile for various network research needs.
- **Integration and extensibility**: NS-3 can be integrated with real-world systems and extended with new models or protocols developed by users.
- **Visualization tools**: It provides visualization tools like NetAnim and PyViz for easier analysis and debugging of network simulations.
- **Active community and documentation**: The NS-3 project benefits from an active community and comprehensive documentation, facilitating learning and advanced usage.


## 1.2 Getting Started

This section summarizes the ns-3 installation process that I followed in order to start running my simulations. First of all, these are my system specifications:
- **Host Operating System:** Windows 11
- **Virtualization Software:** VMware Workstation Player 17
- **Guest Operating System:** Ubuntu 22.04 LTS (running on VMware)



### Prerequisites

Make sure that your system has these prerequisites. Download can be via either git or via source archive download (via a web browser, wget, or curl).

| Purpose     | Tool            | Minimum Version |
|-------------|-----------------|-----------------|
| Compiler    | g++ or clang++  | >= 9 or >= 10   |
| Configuration | python3         | >= 3.5          |
| Build system | cmake, and at least one of: make, ninja, or Xcode | >= 3.10 |



In order to install these dependencies I used the following command:

    sudo apt install g++ clang python3 cmake make
From the command line, you can check the version of each of the above tools with version requirements as follows:

- `g++`: `$ g++ --version`
- `clang++`: `$ clang++ --version`
- `python3`: `$ python3 -V`
- `cmake`: `$ cmake --version`
- `make`: `$ make --version` 

### Installation


1. Download a release tarball. This will unpack to a directory such as `ns-allinone-3.41` containing ns-3 and some other programs:
    ```
    $ wget https://www.nsnam.org/releases/ns-allinone-3.41.tar.bz2
    $ tar xfj ns-allinone-3.41.tar.bz2
    $ cd ns-allinone-3.41/ns-3.41
    ```

2. Then, use the ns3 program to build the ns-3 module libraries and executables:
    ```
    $ ./ns3 build    
    ```
    > This process can take some time, but once completed, we're ready to go.

3. After building ns-3, it's a good idea to run the test suite to ensure that everything is working correctly. You can do this by running:
    ```
    $ ./test.py
    ```
4. Next, let's run the simplest simulation provided in the examples directory to verify that ns-3 is working as expected:
    ```
    $ ./ns3 run examples/tutorial/first
    ```
    We should get an output similar to this:
    ```
    At time +2s client sent 1024 bytes to 10.1.1.2 port 9
    At time +2.00369s server received 1024 bytes from 10.1.1.1 port 49153
    At time +2.00369s server sent 1024 bytes to 10.1.1.1 port 49153
    At time +2.00737s client received 1024 bytes from 10.1.1.2 port 9
    ```
    
    > At this point, we're done with the installation. Let's  start simulating Wi-Fi.
### Learning Resources

Here are some additional learning resources that helped me grasp the basic concepts of ns-3:

- **Official Website**: Visit the [ns-3 official website](https://www.nsnam.org/) for downloads, documentation, tutorials, and community forums.

- **ns-3 Wiki**: Explore the [ns-3 Wiki](https://www.nsnam.org/wiki/Main_Page) for additional resources, tutorials, and user-contributed content.
- **Online Courses**: Look for online courses and tutorials on platforms like Coursera, edX, and Udemy that cover ns-3 and network simulation.

# 2. Key Wi-Fi Concepts
Before dealing with the simulation right away, it's essential to grasp the key concepts surrounding Wi-fi networks. This section provides an overview of crucial concepts that will be useful when trying to understand the possible objectives when simulating a Wi-Fi network. 
## 2.1 Wi-Fi Standards and Their Relevance

Wi-Fi standards play a significant role in determining the capabilities and performance of Wi-Fi networks. The Institute of Electrical and Electronics Engineers (IEEE) has developed several standards under the 802.11 family to define the specifications for wireless local area networks (WLANs). Each standard introduces new features, improvements, and advancements in wireless technology. Here are some key Wi-Fi standards and their relevance in modern networks:

| Standard   | Description                                                                 | Relevance                                                                                                                                                                                                 |
|------------|-----------------------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 802.11 (Legacy) | The original Wi-Fi standard introduced in 1997, with data rates up to 2 Mbps. | Primarily of historical interest, but important for understanding the evolution of Wi-Fi technology. Its lower data rates and limited range are significantly surpassed by subsequent standards.       |
| 802.11a    | Introduced in 1999, operates in the 5 GHz band, data rates up to 54 Mbps.    | Less commonly used today due to limited range and compatibility issues with newer devices.                                                                                                                |
| 802.11b    | Released in 1999, operates in the 2.4 GHz band, data rates up to 11 Mbps.    | Despite slower speeds and susceptibility to interference, it remains relevant for legacy devices and low-bandwidth applications.                                                                          |
| 802.11g    | Introduced in 2003, operates in the 2.4 GHz band, supports data rates up to 54 Mbps. | Offers higher data rates while maintaining backward compatibility with 802.11b devices. Widely used in home and small office environments.                                                                |
| 802.11e    | Ratified in 2005, introduces QoS enhancements for prioritizing traffic.      | Essential for supporting real-time applications such as VoIP and video streaming, where consistent performance and low latency are crucial.                                                               |
| 802.11n    | Released in 2009, operates in both 2.4 GHz and 5 GHz bands, supports data rates up to 600 Mbps. | A significant advancement in Wi-Fi technology, widely deployed for increased speed and reliability in multimedia streaming and high-bandwidth applications.                                              |
| 802.11ac   | Released in 2014, operates in the 5 GHz band, supports data rates up to 3.46 Gbps. | Known as Wi-Fi 5, significantly enhances throughput across larger areas and more devices, particularly in the 5 GHz band.                                                                                 |
| 802.11ax   | Released in 2019, also known as Wi-Fi 6, introduces advanced features for improved efficiency and capacity. | Expected to become increasingly prevalent with Wi-Fi 6-enabled devices and infrastructure, addressing the needs of high-density environments.                                                             |

Wi-Fi standards have evolved over time to address the increasing demands of wireless communication, offering improved speed, reliability, and efficiency. As technology advances, new standards emerge, providing enhanced features and capabilities to meet the evolving needs of wireless networks.

## 2.2 Wi-Fi Network Challenges for QoS:

**Hidden Terminal Problem:** Occurs when nodes within the same network are unable to detect each other's transmissions, leading to collisions and degraded performance. Mitigation strategies such as RTS/CTS exchanges are needed to address this challenge.

**Quality of Service (QoS) Requirements:** Ensuring consistent performance for real-time applications like VoIP and video streaming, requiring prioritization mechanisms and traffic differentiation to meet QoS demands.

**Channel Access Methods:** Efficiently managing access to the shared wireless medium to minimize collisions and maximize throughput, especially in high-density environments.

**Propagation Loss and Interference:** Dealing with signal attenuation and environmental interference, which can degrade signal quality and impact network coverage and reliability.

**Legacy Device Compatibility:** Supporting older Wi-Fi standards and legacy devices while maintaining compatibility with newer technologies, presenting challenges in network design and optimization.

**Security Concerns:** Protecting Wi-Fi networks from unauthorized access, eavesdropping, and malicious attacks, requiring robust security protocols and encryption mechanisms to safeguard sensitive data and ensure privacy.





# 3. The Actual Simulation 
## 3.1 Simulation focus
The hidden terminal problem will addressed in the simulation, so an extra effort must be put into understanding this challenge thoroughly. Therefore, a more detailed explanation about the problem and its solution will be given below.

### Hidden Terminal Problem Explained

The hidden terminal problem arises in wireless networks when two nodes,  Node A and Node C, are outside each other's communication range but can both communicate with a central node, Node B. 

<p align="center">
<img src="https://hackmd.io/_uploads/ByOzg0ZlR.png" width="500" height="500">
</p>

This creates a situation where:

Node A transmits data to Node B.
Node C is unaware of Node A's transmission because they are out of range.
Node C also attempts to transmit data to Node B at the same time.
This simultaneous transmission from Node A and Node C collides at Node B, resulting in lost or corrupted data.

### Possible Solution: RTS/CTS Mechanism

The simulation explores how the Request To Send (RTS)/Clear To Send (CTS) mechanism can help mitigate the hidden terminal problem. 

<p align="center">
<img src="https://hackmd.io/_uploads/B1iqE0ZlC.png" width="300" height="200">
</p>

Here's how it works:

Node A wants to send data to Node B.
Node A broadcasts an RTS packet to Node B.
Upon receiving the RTS, Node B checks if it's busy. If not, it sends a CTS packet back to Node A.
Node A receives the CTS, indicating a clear channel, and proceeds to transmit data to Node B.
Nodes within the hearing range of Node B (including Node C) also receive the RTS and CTS packets.
Knowing Node B is busy, Node C defers its transmission until the CTS timeout expires, preventing collisions.
By implementing this handshake mechanism, Node A and Node B can coordinate their communication and avoid collisions caused by hidden terminals like Node C.


## 3.2 Simulation Program

The following program called `wifi-hidden-terminal.cc` is included with ns-3 as part of the `examples/wireless` directory. This simulation explores the hidden terminal problem in wireless networks and how enabling RTS/CTS can help mitigate this problem. The code simulates a scenario with a hidden terminal problem in a wireless network. Here's a breakdown of the key elements:

**Nodes:** There are three nodes (0, 1, and 2) set up in the simulation.

**Hidden Terminal:** Node 2 is positioned such that it can transmit to Node 1 (the central node) but is outside the communication range of Node 0 (another node trying to reach Node 1). This creates the hidden terminal problem.

**Data Flows:** Two data streams (Flow 1 and Flow 2) are created:
Flow 1: transmits data from Node 0 to Node 1.
Flow 2: transmits data from Node 2 (hidden terminal) to Node 1.

**RTS/CTS:** The code allows for enabling or disabling the RTS/CTS handshake mechanism.

**Metrics:** The simulation monitors various metrics like transmitted and received packets, throughput, etc., for both data flows. 
The goal is to compare how these data flows are affected by collisions caused by the hidden terminal when RTS/CTS is enabled and disabled. This helps evaluate the effectiveness of RTS/CTS in mitigating the hidden terminal problem.


<details><summary>Click to expand/collapse</summary>
    
```
/*
 * Copyright (c) 2010 IITP RAS
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
 * Author: Pavel Boyko <boyko@iitp.ru>
 *
 * Classical hidden terminal problem and its RTS/CTS solution.
 *
 * Topology: [node 0] <-- -50 dB --> [node 1] <-- -50 dB --> [node 2]
 *
 * This example illustrates the use of
 *  - Wifi in ad-hoc mode
 *  - Matrix propagation loss model
 *  - Use of OnOffApplication to generate CBR stream
 *  - IP flow monitor
 */

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
    nodes.Create(3);

    // 2. Place nodes somehow, this is required by every wireless simulation
    for (uint8_t i = 0; i < 3; ++i)
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

    // 4. Create & setup wifi channel
    Ptr<YansWifiChannel> wifiChannel = CreateObject<YansWifiChannel>();
    wifiChannel->SetPropagationLossModel(lossModel);
    wifiChannel->SetPropagationDelayModel(CreateObject<ConstantSpeedPropagationDelayModel>());

    // 5. Install wireless devices
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211b);
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
    for (auto i = stats.begin(); i != stats.end(); ++i)
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
</details>
    
## 3.3 Simulation Details
    
### Assumptions in the Simulation:

- **Hidden Terminal:** The simulation explicitly considers a hidden terminal scenario. Node 2 can transmit to the central node (Node 1) but is hidden from another node (Node 0) trying to reach Node 1.

- **Sensing Range:** Not explicitly defined. Nodes are assumed to sense transmissions only if they are within their communication range.

- **Transmission Range:** Not explicitly stated. A threshold in the `MatrixPropagationLossModel` defines connectivity based on loss values.

- **Channel Model:** The `YansWifiChannel` doesn't model inherent channel errors. However, the `MatrixPropagationLossModel` can simulate varying signal strengths impacting reception. An ideal error-free channel is not assumed.

### Configuration Parameters:

- **Number of Nodes:** Set in experiment function using `NodeContainer.Create`. 

- **Mobility Model:** Set in experiment function using `CreateObject<ConstantPositionMobilityModel>`. 

- **Data Flows:** Defined in experiment function using `OnOffHelper` instances. These specify source/destination, data rate, and packet size.

### Collecting Simulation Statistics:

- **`FlowMonitorHelper (flowmon)`:** Used to collect statistics on data flows.

- **`InstallAll` on all nodes:** Enables monitoring of transmitted/received packets per flow.

- **After simulation (`Simulator::Run()`):** `CheckForLostPackets` identifies packet losses.

- **`GetFlowStats`:** Retrieves statistics like throughput and packet counts, then printed to the console.


## Simulation Results
To run the  simulation provided in the wireless examples, the following command was used:
    
    $ ./ns3 run examples/wireless/wifi-hidden-terminal
    
Which yielded the following output:   
    
```
Hidden station experiment with RTS/CTS disabled:
Flow 1 (10.0.0.1 -> 10.0.0.2)
  Tx Packets: 2410
  Tx Bytes:   3441480
  TxOffered:  3.05909 Mbps
  Rx Packets: 93
  Rx Bytes:   132804
  Throughput: 0.118048 Mbps
Flow 2 (10.0.0.3 -> 10.0.0.2)
  Tx Packets: 2411
  Tx Bytes:   3442908
  TxOffered:  3.06036 Mbps
  Rx Packets: 113
  Rx Bytes:   161364
  Throughput: 0.143435 Mbps
------------------------------------------------
Hidden station experiment with RTS/CTS enabled:
Flow 1 (10.0.0.1 -> 10.0.0.2)
  Tx Packets: 2410
  Tx Bytes:   3441480
  TxOffered:  3.05909 Mbps
  Rx Packets: 572
  Rx Bytes:   816816
  Throughput: 0.726059 Mbps
Flow 2 (10.0.0.3 -> 10.0.0.2)
  Tx Packets: 2411
  Tx Bytes:   3442908
  TxOffered:  3.06036 Mbps
  Rx Packets: 474
  Rx Bytes:   676872
  Throughput: 0.601664 Mbps 
```
    

# 4. Conclusion
The simulation results clearly demonstrate the negative impact of the hidden terminal problem on wireless network throughput when RTS/CTS is disabled. Here's a breakdown:

**Disabled RTS/CTS:**

Both Flow 1 and Flow 2 experience significant packet loss (over 95% for both of them).
Throughput values for both flows are very low (around 0.1 Mbps), indicating severe performance degradation due to collisions caused by the hidden terminal (Node 2).
  

**Enabled RTS/CTS:**

Packet delivery is significantly improved for both flows. Flow 1 receives over 570 packets (compared to only 93 with disabled RTS/CTS), and Flow 2 receives over 470 packets.
Throughput values increase dramatically (around 0.7 Mbps for Flow 1 and 0.6 Mbps for Flow 2), showcasing a substantial improvement in data transfer efficiency.
These results highlight the effectiveness of the RTS/CTS mechanism in mitigating the hidden terminal problem. By coordinating transmissions and preventing collisions, RTS/CTS enables successful data delivery and significantly improves overall network performance.
    
    
>In conclusion, these simulations strongly support the use of RTS/CTS in scenarios susceptible to the hidden terminal problem, as it significantly improves data delivery and network throughput.
    
    
# 5. References
[NS-3 Documentation](https://www.nsnam.org/documentation/)   
    
[NS-3 Installation Guide PDF](https://www.nsnam.org/docs/installation/ns-3-installation.pdf)

[Udemy Course: Getting Started with NS-3](https://www.udemy.com/course/getting-started-with-network-simulator-3/?couponCode=ST8MT40924)
    
[What is the Hidden Terminal Problem?](https://www.educative.io/answers/what-is-the-hidden-terminal-problem)

[Hidden Station Problem Diagram](https://en.wikipedia.org/wiki/File:Wifi_hidden_station_problem.svg)
    
[RTS/CTS FlowChart](https://www.researchgate.net/figure/Simplified-Flowchart-of-CSMA-CA-influenced-by-Tan03_fig13_305807856)

