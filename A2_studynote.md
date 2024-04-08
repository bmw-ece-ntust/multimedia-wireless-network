# Assignment2 Studynote
## About ns-3
ns-3 is a discrete-event network simulator for Internet systems, targeted primarily for research and educational use. It is free, open-source software, licensed under the GNU GPLv2 license.
### Simulation workflow
1. Topology definition: To ease the creation of basic facilities and define their interrelationships, ns-3 has a system of containers and helpers that facilitates this process.
2. Model development: Models are added to simulation (for example, UDP, IPv4, point-to-point devices and links, applications); most of the time this is done using helpers.
3. Node and link configuration: models set their default values (for example, the size of packets sent by an application or MTU of a point-to-point link); most of the time this is done using the attribute system.
4. Execution: Simulation facilities generate events, data requested by the user is logged.
5. Performance analysis: After the simulation is finished and data is available as a time-stamped event trace. This data can then be statistically analysed with tools like R to draw conclusions.
6. Graphical Visualization: Raw or processed data collected in a simulation can be graphed using tools like Gnuplot, matplotlib or XGRAPH.

reference: https://www.nsnam.org/wiki/Main_Page

## Install 
reference: https://hackmd.io/@2xIzdkQiS9K3Pfrv6tVEtA/S1LpzA51p#-3-Installation
## Scenerio
I referred to the scenario described in the paper "ANALYSIS OF IEEE 802.11E FOR QOS SUPPORT IN WIRELESS LANS" to observe the relationship between the number of stations and the throughput of AC. However, unlike the paper, I only have the AC_BE (Best Effort) and not an isolated QBSS. Additionally, due to hardware and time limitations, I reduced the number of STAs to 6.

###  Hybrid Coordinator (HC)
The station that operates as the central coordinator for all other stations within the same QoS supporting BSS (QBSS) is called the hybrid coordinator (HC). Similar to the PC, the HC resides within an 802.11e AP. A BSS that includes an 802.11e-compliant HC is referred to as a QBSS.


<img src='reference scenerio.png' width='200'>
 ▲ reference scenerio

### Parameter
| NAME | VALUE | 
| -------- | -------- | 
| WIFI     | 802.11n     |
|Band Width|40MHz|
| Modulation| OFDM  4.5 Mbps |
|Channel Model|YansWifiChannel|
|Application Protocol|UDP|
|Number of AP|1|
|Max Number of STA|6|
|Hidden Node|X|


## Flowchart & Code
<img src='flowchart.png' width='400'>

This code is based on ns3-3.39/examples/wireless/wifi-simple-ht-hidden-stations.cc. It includes the addition of a new feature to increase the number of stations (STAs), draw plots, and make some detailed modifications.

```c++ linenos
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
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/uinteger.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/gnuplot.h"
#include "ns3/core-module.h"


#include <cmath>

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

int main(int argc, char* argv[]) {
    uint32_t payloadSize = 1472; // bytes
    double simulationTime = 30;  // seconds
    uint32_t nMpdus = 1;
    uint32_t maxAmpduSize = 0;
    bool enableRts = 0;
    double minExpectedThroughput = 0;
    double maxExpectedThroughput = 0;
    uint32_t numStations=6;
    
    // Plot Setting
    std::string fileNameWithNoExtension = "plot";
    std::string graphicsFileName = fileNameWithNoExtension + ".png";
    std::string plotFileName = fileNameWithNoExtension + ".plt";
    std::string plotTitle = "";
    std::string dataTitle = "throughput";
    
    // Instantiate the plot and set its title.
    Gnuplot plot(graphicsFileName);
    plot.SetTitle(plotTitle);

    // Make the graphics file, which the plot file will create when it
    // is used with Gnuplot, be a PNG file.
    plot.SetTerminal("png");

    // Set the labels for each axis.
    plot.SetLegend("Number of Station", "AP Throughput(Mbps)");

    // Set the range for the x axis.
    plot.AppendExtra("set xrange [1:"+std::__cxx11::to_string(numStations)+"]");
    plot.AppendExtra("set xtics 1");
    
    // Instantiate the dataset, set its title, and make the points be
    // plotted along with connecting lines.
    Gnuplot2dDataset dataset;
    dataset.SetTitle(dataTitle);
    dataset.SetStyle(Gnuplot2dDataset::LINES_POINTS);

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

    for (uint32_t i = 1; i < numStations+1; ++i) {
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(i);
        NodeContainer wifiApNode;
        wifiApNode.Create(1);

        // Setting up wireless channel and physical layer
        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        channel.AddPropagationLoss("ns3::RangePropagationLossModel"); // wireless range limited to 5 meters!

        YansWifiPhyHelper phy;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy.SetChannel(channel.Create());
        phy.Set("ChannelSettings", StringValue("{0, 40, BAND_5GHZ, 0}"));

        // Setting up MAC layer
        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211n);
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                     "DataMode",
                                     StringValue("OfdmRate4_5MbpsBW5MHz"));
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

        // Setting up mobility model
        MobilityHelper mobility;
        Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();

        // AP is between the two stations, each station being located at 5 meters from the AP.
        // The distance between the two stations is thus equal to 10 meters.
        // Since the wireless range is limited to 5 meters, the two stations are hidden from each other.
        double AP_x = 10.0;
        double AP_y = 10.0;
        double radius = 4.99;
        positionAlloc->Add(Vector(AP_x, AP_y, 0.01234)); // AP position

        // Set random STA positions
        uint32_t seed = 1;
        RngSeedManager::SetSeed(seed);
        Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
        uint32_t min = 0;
        double max = 2 * M_PI;
        std::set<uint32_t> generatedNumbers;
        for (uint32_t j = 0; j < i; ++j) {
            double angle;
            do {
                angle = rand->GetValue(min, max);
            } while (generatedNumbers.count(angle) > 0);
            
            generatedNumbers.insert(angle);

            // Set position
            double STA_x = AP_x + radius * std::cos(angle);
            double STA_y = AP_y + radius * std::sin(angle);

            positionAlloc->Add(Vector(STA_x, STA_y, 0.0));
        }
     
        mobility.SetPositionAllocator(positionAlloc);
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

        mobility.Install(wifiApNode);
        mobility.Install(wifiStaNodes);
        
        // Setting up internet stack
        InternetStackHelper stack;
        stack.Install(wifiApNode);
        stack.Install(wifiStaNodes);

        Ipv4AddressHelper address;
        address.SetBase("192.168.1.0", "255.255.255.0");
        Ipv4InterfaceContainer StaInterface;
        StaInterface = address.Assign(staDevices);
        Ipv4InterfaceContainer ApInterface;
        ApInterface = address.Assign(apDevice);

        // Setting up applications
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

        // Enable packet capture for AP
        phy.EnablePcap("SimpleHtHiddenStations_Ap" + std::__cxx11::to_string(i), apDevice.Get(0));

        // Run the simulation
        Simulator::Stop(Seconds(simulationTime + 1));
        Simulator::Run();

        // Calculate the total packets received by AP
        uint64_t totalPacketsThrough = DynamicCast<UdpServer>(serverApp.Get(0))->GetReceived();
        
        // Destroy the simulation
        Simulator::Destroy();

        // Calculate the throughput at AP
        double throughput = totalPacketsThrough * payloadSize * 8 / (simulationTime * 1000000.0);
        std::cout << "Throughput at AP: " << throughput << " Mbit/s" << '\n' << '\n';
        dataset.Add(i, throughput);
    }

    // Add the dataset to the plot.
    plot.AddDataset(dataset);

    // Open the plot file.
    std::ofstream plotFile(plotFileName.c_str());

    // Write the plot file.
    plot.GenerateOutput(plotFile);

    // Close the plot file.
    plotFile.close();

    return 0;
}


```
### Compile
```bash
cd workspace/ns-3-allinone/ns-3.39/
./ns3 run [filename]

//after finish compile
//create plot.png from .plt
gnuplot plot.plt
```


## Result
>[!NOTE]
>CTS/RTS:
>```c++{.line-numbers start=68}
>    bool enableRts = 1;
>```
> Hidden Node:
>```c++{.line-numbers start=125}
>Config::SetDefault("ns3::RangePropagationLossModel::MaxRange", DoubleValue(999));
>```


### No CTS/RTS
<img src='X H X CR.png' width='400'>
From above figure can observe the throughput keep reduceing once the number of stations becomes larger since collision.

### Use CTS/RTS
<img src='X H O CR.png' width='400'>
The figure is more similar to the one in paper. With the STA increse, AP throughput can up to a point and reduce since collision still happen. But not that dramatically since for now there is only AC_BE in use.

<img src='paper result.png' width='400'>


