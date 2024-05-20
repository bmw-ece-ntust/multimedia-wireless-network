# Assignment 3 Studynote
## About ns-3
ns-3 is a discrete-event network simulator for Internet systems, targeted primarily for research and educational use. It is free, open-source software, licensed under the GNU GPLv2 license.
## Scenerio
### I.
The first scenerio has a 1 AP transmitting four streams to each of 3 STA, one stream per priority. Hence, the AP transmits 12 streams in total.

<img src='scenerio 3-1.png' width='200'>

### Parameter
| NAME | VALUE | 
| -------- | -------- | 
| WIFI     | 802.11n     |
|Band Width|20MHz|
|Channel Model|YansWifiChannel|
|Number of AP|1|
| Number of STA|3|


### II.
Based on the paper "ANALYSIS OF IEEE 802.11E FOR QOS SUPPORT IN WIRELESS LANS", I improved the code from my assignment 2, which with 1 AP and increasing number of STA. The main diffferent is ToS (Type of Service) form only 1 {AC_BE change} to 4 {AC_BE, AC_BK, AC_VI, AC_VO}. 

<img src='reference scenerio.png' width='200'>
 ▲ reference scenerio

### Parameter
| NAME | VALUE | 
| -------- | -------- | 
| WIFI     | 802.11n     |
|Band Width|40MHz|
| Modulation| OFDM  4.5 Mbps |
|Channel Model|YansWifiChannel|
|Number of AP|1|
|Max Number of STA|6|
|RTS|Yes|
|Hidden Node|NO|




## Flowchart & Code
<img src='flowchart3.png' width='500'>

This code is based on ns3-3.39/examples/wireless/wifi-simple-ht-hidden-stations.cc and wifi-multi-tos.cc. It includes the addition of a new feature to increase the number of stations (STAs), draw plots, and make some detailed modifications.
### I.
```c++ linenos
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

#include "ns3/gnuplot.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211eTxop");

int main(int argc, char *argv[])
{
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
    
    double offeredTrafficAC = 0;

    // Plot Setting
    std::string fileNameWithNoExtension = "plot";
    std::string graphicsFileName = fileNameWithNoExtension + ".png";
    std::string plotFileName = fileNameWithNoExtension + ".plt";
    std::string plotTitle = "";
    std::string dataTitle = "Throughput";
    
    // Instantiate the plot and set its title.
    Gnuplot plot(graphicsFileName);
    plot.SetTitle(plotTitle);

    // Make the graphics file, which the plot file will create when it
    // is used with Gnuplot, be a PNG file.
    plot.SetTerminal("png");

    // Set the labels for each axis.
    plot.SetLegend("Offered traffic per AC (3 streams each) (Mb/s)", "Throughput per AC (3 streams each) (Mb/s)");

    // Set the range for the x axis.
    plot.AppendExtra("set xrange [0:30]");
    plot.AppendExtra("set yrange [0:*]");
    
    // Instantiate the datasets
    Gnuplot2dDataset datasetBE;
    datasetBE.SetTitle("AC BE");
    datasetBE.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    Gnuplot2dDataset datasetBK;
    datasetBK.SetTitle("AC BK");
    datasetBK.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    Gnuplot2dDataset datasetVI;
    datasetVI.SetTitle("AC VI");
    datasetVI.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    Gnuplot2dDataset datasetVO;
    datasetVO.SetTitle("AC VO");
    datasetVO.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    // Command-line options for repeatability
    CommandLine cmd;
    cmd.AddValue("offeredTrafficAC", "Offered traffic for AC(Mbps)", offeredTrafficStart);
    cmd.Parse(argc, argv);

    // Loop through offeredTraffic values
    for (offeredTrafficAC = offeredTrafficStart; offeredTrafficAC <= offeredTrafficEnd; offeredTrafficAC += offeredTrafficStep)
    {
        // 1. Create nodes. 1 AP and 4 STAs
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(4);
        NodeContainer wifiApNodes;
        wifiApNodes.Create(1);

        // 2. Create channel and phy
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

        // Set mobility models for the nodes
        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(wifiStaNodes);
        mobility.Install(wifiApNodes);

        // Install Internet stack on nodes
        InternetStackHelper stack;
        stack.Install(wifiApNodes);
        stack.Install(wifiStaNodes);

        // Assign IP addresses to the devices
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

        // Create UDP servers on each STA
        UdpServerHelper serverA(port);
        ApplicationContainer serverAppA = serverA.Install(wifiStaNodes.Get(0));
        serverAppA.Start(Seconds(0.0));
        serverAppA.Stop(Seconds(3));

        UdpServerHelper serverB(port);
        ApplicationContainer serverAppB = serverB.Install(wifiStaNodes.Get(1));
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

        // Configure destination addresses with appropriate TOS values
        InetSocketAddress destA(staInterfaceA.GetAddress(0), port);
        destA.SetTos(0xa0); // AC_VI 0xa0

        InetSocketAddress destB(staInterfaceB.GetAddress(0), port);
        destB.SetTos(0xc0); // AC_VO 0xc0

        InetSocketAddress destC(staInterfaceC.GetAddress(0), port);
        destC.SetTos(0x00); // AC_BE 0x
        
        InetSocketAddress destD(staInterfaceD.GetAddress(0), port);
        destD.SetTos(0x20); // AC_BK 0x20

        // Create and configure OnOff applications for each AC
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
        clientAppB.Stop(Seconds(4.0)); 

        OnOffHelper clientC("ns3::UdpSocketFactory", destC);
        clientC.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        clientC.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        clientC.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficAC) + "Mbps")));
        clientC.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppC = clientC.Install(wifiApNodes.Get(0));
        clientAppC.Start(Seconds(1.0));
        clientAppC.Stop(Seconds(4.0)); 

        OnOffHelper clientD("ns3::UdpSocketFactory", destD);
        clientD.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
        clientD.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
        clientD.SetAttribute("DataRate", DataRateValue(DataRate(std::to_string(offeredTrafficAC) + "Mbps")));
        clientD.SetAttribute("PacketSize", UintegerValue(payloadSize));

        ApplicationContainer clientAppD = clientD.Install(wifiApNodes.Get(0));
        clientAppD.Start(Seconds(1.0));
        clientAppD.Stop(Seconds(4.0)); 

        // Run the simulation
        Simulator::Stop(Seconds(5.0));
        Simulator::Run();

        uint32_t totalPacketsThroughA = DynamicCast<UdpServer>(serverAppA.Get(0))->GetReceived(); // AC_VI. Sta 0
        uint32_t totalPacketsThroughB = DynamicCast<UdpServer>(serverAppB.Get(0))->GetReceived(); // AC_VO. Sta 1
        uint32_t totalPacketsThroughC = DynamicCast<UdpServer>(serverAppC.Get(0))->GetReceived(); // AC_BE. Sta 2
        uint32_t totalPacketsThroughD = DynamicCast<UdpServer>(serverAppD.Get(0))->GetReceived(); // AC_BK. Sta 3
        Simulator::Destroy();

        double throughputA = (totalPacketsThroughA * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputB = (totalPacketsThroughB * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputC = (totalPacketsThroughC * payloadSize * 8.0) / (4 * 1000000.0);
        double throughputD = (totalPacketsThroughD * payloadSize * 8.0) / (4 * 1000000.0);

        datasetVI.Add(offeredTrafficAC, throughputA);
        datasetVO.Add(offeredTrafficAC, throughputB);
        datasetBE.Add(offeredTrafficAC, throughputC);
        datasetBK.Add(offeredTrafficAC, throughputD);
    }

    // Add datasets to plot after the loop to avoid adding them multiple times
    plot.AddDataset(datasetVI);
    plot.AddDataset(datasetVO);
    plot.AddDataset(datasetBE);
    plot.AddDataset(datasetBK);

    // Open the plot file
    std::ofstream plotFile(plotFileName.c_str());

    // Write the plot file
    plot.GenerateOutput(plotFile);

    // Close the plot file
    plotFile.close();
    return 0;
}

```
This code is based on Sylvia's work.

### II. 
```c++ linenos
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
#include "ns3/sta-wifi-mac.h"
#include "ns3/on-off-helper.h"

#include "ns3/packet-sink-helper.h"
#include "ns3/packet-sink.h"

#include <cmath>
#include <set>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SimplesHtHiddenStations");

int main(int argc, char* argv[])
{
    bool enableRts = true;
    uint32_t payloadSize = 1472; // bytes
    double simulationTime = 10;  // seconds
    double minExpectedThroughput = 0;
    double maxExpectedThroughput = 0;
    uint32_t numStations = 6;
    
    //Plot Setting
    std::string fileNameWithNoExtension = "plot";
    std::string graphicsFileName = fileNameWithNoExtension + ".png";
    std::string plotFileName = fileNameWithNoExtension + ".plt";
    std::string plotTitle = "Throughput vs Number of Stations";
    std::string dataTitle = "Aggregated Throughput";
    bool draw_ac = true;
    
    // Instantiate the plot and set its title.
    Gnuplot plot(graphicsFileName);
    plot.SetTitle(plotTitle);

    // Make the graphics file, which the plot file will create when it
    // is used with Gnuplot, be a PNG file.
    plot.SetTerminal("png");

    // Set the labels for each axis.
    plot.SetLegend("Number of Stations", "Throughput (Mbps)");

    // Set the range for the x axis.
    plot.AppendExtra("set xrange [1:" + std::to_string(numStations) + "]");
    plot.AppendExtra("set xtics 1");
    plot.AppendExtra("set yrange [0:20]");
    
    // Instantiate the datasets
    Gnuplot2dDataset dataset;
    dataset.SetTitle(dataTitle);
    dataset.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    Gnuplot2dDataset datasetBE;
    datasetBE.SetTitle("AC BE");
    datasetBE.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    Gnuplot2dDataset datasetBK;
    datasetBK.SetTitle("AC BK");
    datasetBK.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    Gnuplot2dDataset datasetVI;
    datasetVI.SetTitle("AC VI");
    datasetVI.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    Gnuplot2dDataset datasetVO;
    datasetVO.SetTitle("AC VO");
    datasetVO.SetStyle(Gnuplot2dDataset::LINES_POINTS);

    CommandLine cmd(__FILE__);
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


    // Set the maximum wireless range to 5 meters in order to reproduce a hidden nodes scenario,
    // i.e. the distance between hidden stations is larger than 5 meters
    Config::SetDefault("ns3::RangePropagationLossModel::MaxRange", DoubleValue(999));

    for (uint32_t i = 1; i < numStations + 1; ++i) {
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(i);
        NodeContainer wifiApNode;
        wifiApNode.Create(1);

        YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
        channel.AddPropagationLoss("ns3::RangePropagationLossModel");

        YansWifiPhyHelper phy;
        phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
        phy.SetChannel(channel.Create());
        phy.Set("ChannelSettings", StringValue("{0, 40, BAND_5GHZ, 0}"));

        WifiHelper wifi;
        wifi.SetStandard(WIFI_STANDARD_80211n);
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                     "DataMode",
                                     StringValue("OfdmRate4_5MbpsBW5MHz"));
        WifiMacHelper mac;

        Ssid ssid = Ssid("ns3-80211n");
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


        // Setting mobility model
        MobilityHelper mobility;
        Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();

        double AP_x = 10.0;
        double AP_y = 10.0;
        double radius = 4.99;
        positionAlloc->Add(Vector(AP_x, AP_y, 0.01234));

        uint32_t seed = 1;
        RngSeedManager::SetSeed(seed);
        Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable>();
        double max = 2 * M_PI;
        std::set<uint32_t> generatedNumbers;

        for (uint32_t j = 0; j < i; ++j) {
            double angle;
            do {
                angle = rand->GetValue(0, max);
            } while (generatedNumbers.count(angle) > 0);

            generatedNumbers.insert(angle);
            double STA_x = AP_x + radius * std::cos(angle);
            double STA_y = AP_y + radius * std::sin(angle);
            positionAlloc->Add(Vector(STA_x, STA_y, 0.0));
        }

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
        Ipv4InterfaceContainer staInterface;
        Ipv4InterfaceContainer apNodeInterface;

        staInterface = address.Assign(staDevices);
        apNodeInterface = address.Assign(apDevice);

        // Setting applications
        uint16_t port = 9;
        UdpServerHelper server(port);
        ApplicationContainer sourceApplications;
        ApplicationContainer sinkApplications;
        std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; // AC_BE, AC_BK, AC_VI, AC_VO
        double AC_totalth[4] = {0};

        for (uint32_t index = 0; index < i; ++index) {
            for (uint8_t tosValue : tosValues) {
                auto ipv4 = wifiApNode.Get(0)->GetObject<Ipv4>();
                const auto address = ipv4->GetAddress(1, 0).GetLocal();
                InetSocketAddress sinkSocket(address, port++);
                sinkSocket.SetTos(tosValue);
                OnOffHelper onOffHelper("ns3::UdpSocketFactory", sinkSocket);
                onOffHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
                onOffHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
                onOffHelper.SetAttribute("DataRate", DataRateValue(50000000 / i));
                onOffHelper.SetAttribute("PacketSize", UintegerValue(1472));
                sourceApplications.Add(onOffHelper.Install(wifiStaNodes.Get(index)));
                PacketSinkHelper packetSinkHelper("ns3::UdpSocketFactory", sinkSocket);
                sinkApplications.Add(packetSinkHelper.Install(wifiApNode.Get(0)));
            }
        }

        sinkApplications.Start(Seconds(0.0));
        sinkApplications.Stop(Seconds(simulationTime + 1));
        sourceApplications.Start(Seconds(1.0));
        sourceApplications.Stop(Seconds(simulationTime + 1));

        Simulator::Stop(Seconds(simulationTime + 1));
        Simulator::Run();

        double throughput = 0;
        for (uint32_t index = 0; index < sinkApplications.GetN(); ++index) {
            uint64_t totalPacketsThrough = DynamicCast<PacketSink>(sinkApplications.Get(index))->GetTotalRx();
            double ac_throughput = ((totalPacketsThrough * 8) / (simulationTime * 1000000.0));
            uint16_t AC_type = index % 4;
            AC_totalth[AC_type] += ac_throughput;
            throughput += ac_throughput;
        }

        if (throughput > 0) {
            dataset.Add(i, throughput);
            if (draw_ac) {
                datasetBE.Add(i, AC_totalth[0]);
                datasetBK.Add(i, AC_totalth[1]);
                datasetVI.Add(i, AC_totalth[2]);
                datasetVO.Add(i, AC_totalth[3]);
            }
        } else {
            NS_LOG_ERROR("Obtained throughput is 0!");
        }

        Simulator::Destroy();
    }

    // Add the datasets to the plot
    plot.AddDataset(dataset);
    if (draw_ac) {
        plot.AddDataset(datasetBE);
        plot.AddDataset(datasetBK);
        plot.AddDataset(datasetVI);
        plot.AddDataset(datasetVO);
    }

    // Open the plot file
    std::ofstream plotFile(plotFileName.c_str());

    // Write the plot file
    plot.GenerateOutput(plotFile);

    // Close the plot file
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
### I.
<img src='Result 3-1.png' width='500'>
The plot is very similiar compared to the reference paper. Only AC_VI keeps the throughput, the other ToS drop. But in this plot, AC_VI is not that stable after Offered traffic increasing. 

### II.
<img src='Result 3.png' width='500'>
In above plot, the order of ToS is correct. , but the trend doesn't go up until reach the highpoint. Both AC_BE and AC_BK throughput is very small, at the bottom which hard to see as two seperate lines.

## Future Work
The throughput won't increase with the STA number, which should. Still need to find which parameter caused the problem

Also, since the ToS I use can not set detail parameter same as the reference paper, this might one reason the result is not identicel.
