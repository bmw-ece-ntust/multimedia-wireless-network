![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/95663e7e-c737-44d7-8dac-470a05a61fd3)
- reference: [[MWN] A2](https://hackmd.io/@SylviaCh/Sk4Iwhuy0)

**Outline**
- [ns3 Simulation](#ns3-simulation)
  - [ns3 installation](#ns3-installation)
    - [1. ns3 background knowledge](#1-ns3-background-knowledge)
    - [2. Download the release ](#2-download-the-release)
    - [3. Building and testing ns-3](#3-building-and-testing-ns-3)
  - [Wifi 802.11 Simulation](#wifi-802.11-simulation)
  - [1. Hidden terminal simulation](#1-hidden-terminal-simulation)
    - [1-1. 3 node RTC/CTS simulation](#1-1-3-node-rts/cts-simulation)
    - [1-2. 4 node RTS/CTS simulation](#1-2-4-node-rts/cts-simulation)
    - [1-3. Analysis](#1-3-analysis)
- [Paper study](#study-note)  
  - [wifi evolution](#wifi-evolution)
  - [Appendix](#appendix)
# ns3 Simulation
## ns3 installation
### 1. ns3 background knowledge
- NS-3 (Network Simulator 3) is an open-source tool and platform for simulating computer networks. It is widely used for research, development, and testing of various network protocols, applications, and technologies, as well as for understanding and evaluating the performance of different network designs and configurations. Here are some key features and information about NS-3:

    1. **Open Source and Free**: NS-3 is a free and open-source project, allowing users free access to and modification of the source code. This enables both academic research and industrial applications to fully leverage the tool.
   2. **Modular and Extensible**: NS-3 is a highly modular tool that allows users to add new modules or extend existing ones as needed to meet specific research requirements.
    3. **Support for Multiple Protocols**: NS-3 supports various network protocols, including TCP, UDP, IPv4, IPv6, 802.11 Wi-Fi, LTE, MANETs (mobile ad-hoc networks), and more. This allows users to simulate a wide range of network environments and technologies.
    4. **Powerful Simulation Capabilities**: NS-3 provides rich simulation capabilities, including network topology design, traffic generation, performance metrics, and result analysis. This enables users to conduct detailed experiments to assess network performance.
    5. **Academic Research**: NS-3 is highly popular in the academic community and is used in research areas such as wireless communication, network protocols, data center networks, sensor networks, and more, contributing to the advancement of network technologies.

### 2. Download the release 
>**Configurement**
> - OS : Ubuntu 22.04 LTS VM
> - g++ version : 11.4.0
> - ns3 version : 3.40
  
1.  Install the packages required for ns3.
    ```bash=
    sudo apt install g++ python3 python3-dev pkg-config sqlite3 cmake python3-setuptools git qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools gir1.2-goocanvas-2.0 python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython3 openmpi-bin openmpi-common openmpi-doc libopenmpi-dev autoconf cvs bzr unrar gsl-bin libgsl-dev libgslcblas0 wireshark tcpdump sqlite sqlite3 libsqlite3-dev libxml2 libxml2-dev libc6-dev libc6-dev-i386 libclang-dev llvm-dev automake python3-pip libxml2 libxml2-dev libboost-all-dev
    ```
2.  Download the release from: [ns-3.40](https://www.nsnam.org/releases/ns-3-40/) 
3. Unpack it in a working directory of your choice.
    ```bash=
    tar xjf ns-allinone-3.40.tar.bz2
    ```
4. Change into the ns-3 directory directly
    ```bash=
    cd ns-allinone-3.40/ns-3.40
    ```
### 3. Building and testing ns-3
- The below commands make use of a Python wrapper around CMake, called ns3, that simplifies the command-line syntax, resembling Waf syntax.
    ```bash=
    ./ns3 configure --enable-examples --enable-tests
    ```
- Then, use ns3 to build ns-3:
    ```bash=
    ./ns3 build
    ```
- Once complete, you can run the unit tests to check your build:
    ```bash=
    ./test.py
    ```
    - Screenshots of successful tests
    ![image](https://hackmd.io/_uploads/r1iCYgFk0.png)
- To run the first tutorial program, whose source code is located at examples/tutorial/first.cc, use ns3 to run it (by doing so, the ns-3 shared libraries are found automatically)
    ```bash=
    ./ns3 run first
    ```
    - Screenshots of successful tests
    ![image](https://hackmd.io/_uploads/rJ_chnuk0.png)

- Run the wireless 802.11e example
    ```bash=
    ./ns3 run examples/wireless/wifi-80211e-txop.cc 
    ```
    - Screenshots of successful tests
    ![image](https://hackmd.io/_uploads/B1MCjh_1A.png)


- reference: [ns-3 tutorial](https://www.nsnam.org/docs/release/3.40/tutorial/html/quick-start.html), [NS3 安裝流程](https://hackmd.io/@tomorrow3h/ns3)

## Wifi 802.11 Simulation
> **Show**
> 1. What are the assumptions used in the simulation? 
>    - Do we consider the hidden node terminal? What is the transmission range and the sensing range?
>    - Do we consider an error-free channel or an error channel (modeled as a channel model to simulate the effect of the wireless channel)?
> 2. How/Where do we set the configuration parameters for the simulation (# of STAs, arrival process of each STA, etc.)?
> 3. How/Where do we collect the statistics of the simulation (average delay, throughput, etc.)? 

**Goal**
- [x] 1. Hidden terminal simulation
    - [x] **Goal**: Add addition node and compare simulation results
- [ ] 2. Hidden station simulation

## 1. Hidden terminal simulation
> **Setting**
> - 802.11 version:
>    - 802.11b
> - Node IP setting:
>    - base address: 10.0.0.0
>    - subnet mask: 255.0.0.0
>    - destination IP: 10.0.0.2
>    - port: 12345
> - Data rate:
>    - 3000000 bps or 3001100 bps

#### Flow chart
<div align="center">
  <img src="https://hackmd.io/_uploads/B1DBWh-l0.png" alt="Editor" width="500">
</div>

```bash=
cd ns-alllinone-3.40/ns-3.40
./ns3 run examples/wireless/wifi-hidden-terminal.cc
```
### 1-1. 3 node RTS/CTS simulation

- Topology
    ![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/08862e55-e173-474a-9685-61ef463268b9)

#### 3 node RTC/CTS code
```bash=
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
        // first 2 FlowIds are for ECHO apps, we dont want to display them
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

int main(int argc, char** argv)
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

#### Simulation result:
![image](https://hackmd.io/_uploads/Sy6HDOZlC.png)

### 1-2. 4 node RTS/CTS simulation
1. Add an additional [node 3] directly below [node 1]. 
2. Add an additional flow 3, from node 3 to node 1.


#### Topology
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/2851f697-4cee-492c-b69f-6fa4b08025c9)


#### 4 node RTS/CTS code
```bash=
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
    nodes.Create(4);

    // 2. Place nodes somehow, this is required by every wireless simulation
   for (uint8_t i = 0; i < 4; ++i)
  {
    Ptr<ConstantPositionMobilityModel> mobility = CreateObject<ConstantPositionMobilityModel>();
    if (i == 3) {
        // Node 3 placed below Node 1 with -50 dB propagation loss
        Vector position(0.0, -50.0, 0.0);
        mobility->SetPosition(position);
    }
    nodes.Get(i)->AggregateObject(mobility);
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
    lossModel->SetLoss(nodes.Get(1)->GetObject<MobilityModel>(),
                      nodes.Get(3)->GetObject<MobilityModel>(),
                      50); // set loss from node 1 to node 3 to 50 dB
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

    // flow 3:  node 3 -> node 1
    onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.002))); // Start after flow 2
    cbrApps.Add(onOffHelper.Install(nodes.Get(3)));

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
        // first 2 FlowIds are for ECHO apps, we dont want to display them
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

#### Simulation result:
![image](https://hackmd.io/_uploads/HyAdPdbxA.png =60%x)

### 1-3. Analysis
1. **Comparison of results with and without RTS/CTS:**
- When RTS/CTS is disabled, the throughput of the flows is lower, and packet loss may occur, especially in the presence of hidden terminals.
- When RTS/CTS is enabled, the throughput of the flows significantly increases. This is because the RTS/CTS mechanism effectively reduces the impact of the hidden terminal problem, thus improving the success rate of packet transmission.

2. **Comparison of throughput between 3 nodes and 4 nodes:**
- The throughput of the newly added third flow (Flow 3) is affected by the RTS/CTS mechanism, resulting in an increase in throughput. However, it remains lower than Flow 1 and Flow 2. This could be due to competition and collisions, as well as more transmission attempts.


---
# Paper study
## wifi evolution
![image](https://hackmd.io/_uploads/Bk_10SF1R.png)

## study note
reference: 
- [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://ieeexplore.ieee.org/document/1265851)

<!---
### Simulation Key point
#### EVALUATION
- 用ns3模擬來評估允許最高達到54 Mb/s的5GHz的802.11a物理層（PHY）的802.11e MAC的性能
- 對於MSDU傳遞的延遲結果，我們使用離散有限相對誤差（LRE）算法給出結果的隨機數據的實際互補累積分佈函數（CDFs），該算法還測量隨機數據的局部相關性
- 所有結果都在最大有限相對誤差5％內
- 控制帧以6 Mb/s傳輸，數據帧以24 Mb/s傳輸
- 如果沒有另外說明，則在所有退避實體中，提供512字節的MSDU，並具有負指數分佈的到達時間。每個流量承載250 kb/s
- 使用Table 1中提供的EDCA參數。沒有使用RTS/CTS或分段。EDCA-TXOPs的持續時間允許在EDCA中贏得競爭後傳輸一個數據幀。不生成信標幀
- Table1
    | | AC_VO | AC_VI | AC_BE | AC_BK | High (AC H) | Medium (AC M)| Low (AC L) |
    | -------- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |
    | AIFSN: | 2 | 2 | 3 | 7 | 2 | 4 | 7 |
    | CWmin: | 3 | 7 | 15 | 15 | 7 | 10 | 15 |
    | CWmax: | 7 | 15 | 1023 | 1023 | 7 | 31 | 255 |
#### ACHIEVABLE EDCA THROUGHPUT IN AN ISOLATED QBSS WITH FOUR STATIONS
- 所有幀都以24 Mb/s傳輸
- 圖8是從一個單一站點（AP）向三個站點中的每個站點傳輸四個流的模擬導出的，每個優先順序一個流。圖8中說明了每個AC和情景的測量吞吐量。
- AP總共傳輸12個流
- 使用了四個AC的默認EDCF參數（AC_VI … AC_BK）（Table 1）
- 每個EDCF-TXOP傳輸一個數據幀

:::info
**Analysis**
已知802.11中可實現的吞吐量取決於傳遞的MSDU的大小和使用的PHY模式。MSDU大小越大，只要碰撞數量和干擾量較小，可實現的吞吐量就越高。PHY模式越高，只要通道條件足夠，可實現的吞吐量就越高。從圖8中我們觀察到，隨著提供的流量增加，優先級較高的AC通過較小的AIFSN、CWmin和CWmax值限制了優先級較低的AC的可實現吞吐量。在這種情況下，可實現吞吐量的每個AC的分析近似值在[3]中描述；其中它被稱為容量份額。
:::

#### ACHIEVABLE EDCA THROUGHPUT WITH INCREASING NUMBER OF STATIONS
- 如圖9所示。所有站點再次依賴EDCA上的優先級介質訪問（不使用基於輪詢的訪問）。為了研究802.11e在熱點中的性能，其中很可能有更多的站點與一個AP關聯，我們將競爭站點的數量增加到了16個，如圖9所示。圖9顯示了每個AC的測量吞吐量。
- 每個站點都使用所有四個退避實體AC（每個250 kb/s）進行傳輸；因此，每個站點試圖總共攜帶1 Mb/s的流量。
- 每個站點提供相同的流量。

:::info
**Analysis**
可以觀察到，使用EDCA參數集的默認值時，一旦站點的數量增加，吞吐量就會急剧下降。這是由於碰撞機率較高，特別是在AC_VO中，CWmin和CWmax是較小的數字。這顯然不是一個理想的結果。然而，在這種情況下，HC應該設置新的EDCA參數值，然後所有QBSS的站點都將使用這些值。值得注意的是，EDCA的最高優先級AC預計在實際情況下用於類似語音的通話，這意味著每個站點在AC_VO上提供的流量較少（假設每個站點一通話）
:::
#### QOS GUARANTEES WITH PRIORITIZED ACCESS OF HC
- 圖10顯示了對於孤立的QBSS和重疊的QBSS的結果MSDU傳遞延遲分佈。如圖10所示。可以看到，高優先級流的延遲超過了HC定義的TXOP限制，該限制旨在將HCCA流的MSDU傳遞延遲限制在300微秒內。請注意，給出的結果是在重疊的QBSS中觀察到的各種延遲和吞吐量的一個例子。如果兩個HC總是同時輪詢，則會發生一個可觀察到的例子。然後，所有輪詢幀都會發生碰撞，HCCA的吞吐量將降至零。對於重疊的QBSS問題，802.11工作組正在討論解決方案概念。一種解決方案是應用動態頻率選擇，讓QBSS動態選擇一個空閒的媒介。這種方法對於5 GHz頻段非常有趣，因為它具有許多可用的頻率通道。其他方法基於政策。
- 評估允許最高優先級給予HC介質訪問的影響，根據HCCA（Hybrid Coordination Channel Access）協議
- 這裡選擇了EDCA參數集的值，根據Table 1中給出的AC_H（高優先級）、AC_M（中優先級）和AC_L（低優先級）
- 對於128 kb/s的提供流量，這段時間為5毫秒
- 中優先級和低優先級的AC，提供帶有負指數分佈的到達時間的200字節的MSDU
- 每個流量承載160 kb/s。數據幀以24 Mb/s傳輸，控制幀（ACK）以6 Mb/s傳輸
- 包括AP在內的所有站點通過EDCA競爭介質訪問，現在AP通過HCCA優先級攜帶了額外的等時性下行流量（80字節/MSDU，128 kb/s），該流量具有比AC_H更高的優先級
--->

## Appendix
- IEEE 802.11 WLAN: IEEE 802.11 wireless local area network
- MAC: Medium Access Control
- ISM: Industrial, Scientific, and Medical
- CCK: Complementary Code keying
- DSSS: direct sequence spread spectrum
- OFDM: orthogonal frequency-division multiplexing
- QoS: quality of service
- BSS: basic service set
- AP: access point
- DCF: distributed coordination function
- CSMA: carrier sense multiple access
- MSDU delivery: MAC service data unit delivery
- MPDU: MAC protocol data unit
- CA: collision avoidance
- DIFS: DCF interframe space
- CW: contention window
