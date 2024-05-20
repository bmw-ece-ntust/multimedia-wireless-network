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
#include "ns3/packet-sink-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/flow-monitor.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/boolean.h"  
#include "ns3/pointer.h"  
#include "ns3/qos-txop.h" 
#include <fstream> 
#include <sstream>
#include <iomanip>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211eTxop");

int Case01(int maxStaNum, std::vector<uint8_t> tosValues, int iteration)
{
    // Simulation parameters
    uint16_t port = 6969;
    uint32_t payloadSize = 1472; // bytes
    double trafficStart = 1.0;
    double trafficEnd = 30.0;
    double trafficIncrement = 1.0;
    uint32_t numFlows = maxStaNum;
    
    int iterations = trafficEnd / trafficIncrement;

    std::stringstream fileNameStream;
    fileNameStream << "contrib/mwn/examples/logs/case-01-log_file_" << std::setfill('0') << std::setw(2) << iteration << ".csv";
    std::string fileName = fileNameStream.str();

    // Open the output file 
    std::ofstream outFileCSV(fileName);
    if (!outFileCSV.is_open()) {
        NS_LOG_ERROR("Unable to open output file.");
        return 0;
    }

    outFileCSV << "STA NUM,VI,VO,BE,BK" << "\n";

    for(int n = 1; n <= numFlows; n++)
    {
        double currentTrafficAC = trafficStart + (n * trafficIncrement);

        // Set up 1 AP and 4 STAs (Voice, Video, Best Effort, Background) and set channels & Network
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(n); // 4 STAs for 4 different ACs
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

        NetDeviceContainer staDevice[n];
        NetDeviceContainer apDevice;
        Ssid ssid = Ssid("network");
        phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));

        // Setup QoS MAC for STAs

        // Setup QoS MAC for AP
        mac.SetType("ns3::ApWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid), "EnableBeaconJitter", BooleanValue(false));
        apDevice = wifi.Install(phy, mac, wifiApNodes.Get(0));

        // QoS configuration for AP
        Ptr<NetDevice> dev = wifiApNodes.Get(0)->GetDevice(0);
        Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
        Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
        PointerValue ptr;
        Ptr<QosTxop> edca;

        mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));

        // Configure EDCA for AC_VI (Video)
        // Configure EDCA for AC_VO (Voice)
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

        // Configure EDCA for AC_BE (Best Effort)
        wifi_mac->GetAttribute("BE_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);

        // Configure EDCA for AC_BK (Background)
        wifi_mac->GetAttribute("BK_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(7);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);

        for (int j = 0; j < n; j++) {
            staDevice[j] = wifi.Install(phy, mac, wifiStaNodes.Get(j));
        }

        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(wifiStaNodes);
        mobility.Install(wifiApNodes);

        InternetStackHelper stack;
        stack.Install(wifiApNodes);
        stack.Install(wifiStaNodes);

        Ipv4AddressHelper address;
        address.SetBase("10.2.1.0", "255.255.255.0");
        Ipv4InterfaceContainer staInterface[n];
        Ipv4InterfaceContainer apInterface;
        for (int j = 0; j < n; j++) {
            staInterface[j] = address.Assign(staDevice[j]);
        }
        apInterface = address.Assign(apDevice);

        //-----------------------------------------------------------
        ApplicationContainer apps;
        ApplicationContainer sourceApplications, sinkApplications;
        
        for (uint32_t index = 0; index < n; index++) {
            for (uint8_t tosValue: tosValues) {
                auto ipv4 = wifiApNodes.Get (0)->GetObject<Ipv4> ();
                const auto address = ipv4->GetAddress (1, 0).GetLocal ();
                InetSocketAddress sinkSocket (address, port++);
                sinkSocket.SetTos (tosValue);
                OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
                onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
                onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
                onOffHelper.SetAttribute("DataRate", StringValue("1Mbps"));
                onOffHelper.SetAttribute("PacketSize", UintegerValue(payloadSize));
                sourceApplications.Add (onOffHelper.Install (wifiStaNodes.Get (index)));
                PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
                sinkApplications.Add (packetSinkHelper.Install (wifiApNodes.Get (0)));
  
            }
        }

        //-----------------------------------------------------------
        // Run the simulation
        phy.EnablePcap("A3-Case-01-MAC-Dump", apDevice.Get(0));

        FlowMonitorHelper flowmon;
        Ptr<FlowMonitor> monitor = flowmon.InstallAll();

        double tput_array[4*n];
        uint32_t v = 0;
        Simulator::Stop(Seconds(4.0));
        Simulator::Run();
        monitor->CheckForLostPackets();
        Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
        FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

        for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i) {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);

          std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "  Tx Bytes: " << i->second.txBytes << "\n";
          std::cout << "  Rx Bytes: " << i->second.rxBytes << "\n";

          tput_array[v] = i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 ;

          std::cout << "Throughput: "<< tput_array[v] << "\n";

          v=v+1;
        }
        std::cout << "-------------------------------------------------" << "\n";
        flowmon.SerializeToXmlFile("throughput_results.xml", true, true);
        monitor->SerializeToXmlFile("throughput_results.flowmon", true, true);
        Simulator::Destroy();

        //---------------------------------------------
        // CSV Processing
        //---------------------------------------------
        
        outFileCSV << currentTrafficAC-1 << ",";

        if (n > 1) {
            for (int j = 0; j < 4; j++) {
                double sum = 0.0;
                for (int i = j; i < 4*n; i += 4) {
                    sum += tput_array[i];
                }
                if (j < 3) {
                    outFileCSV << sum << ",";
                } else {
                    outFileCSV << sum;
                }
            }
        } else {
            for (int i = 0; i < 4*n; i++) {
                if (i < (4*n)-1) {
                    outFileCSV << tput_array[i] << ",";
                } else {
                    outFileCSV << tput_array[i];
                }
            }
        }

        outFileCSV << "\n";

        double throughput = 0;
        for (unsigned index = 0; index < sinkApplications.GetN (); ++index)
         {
           uint64_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApplications.Get (index))->GetTotalRx ();
           throughput += ((totalPacketsThrough * 8) / ( n * 1000000.0)); //Mbit/s
         }

        std::cout << "Aggregated throughput: " << throughput << " Mbit/s" << std::endl;

    }

    outFileCSV.close();
    std::cout << "Simulation " << iteration << " completed. Results saved to " << fileName << std::endl;

    return 0;

}

int Case00(std::vector<uint8_t> tosValues, int iteration)
{
    // Simulation parameters
    uint16_t port = 6969;
    uint32_t payloadSize = 1472; // bytes
    double trafficStart = 1.0;
    double trafficEnd = 30.0;
    double trafficIncrement = 1.0;
    uint32_t numFlows = 3;
    
    int iterations = trafficEnd / trafficIncrement;

    std::stringstream fileNameStream;
    fileNameStream << "contrib/mwn/examples/logs/case-00-log_file_" << std::setfill('0') << std::setw(2) << iteration << ".csv";
    std::string fileName = fileNameStream.str();


    // Open the output file 
    std::ofstream outFileCSV(fileName);
    if (!outFileCSV.is_open()) {
        NS_LOG_ERROR("Unable to open output file.");
        return 0;
    }

    outFileCSV << "TPUT,STA0,,,," << "STA1,,,," << "STA2,,,," << "STA3,,,," << "\n";
    outFileCSV << "TPUT,VI,VO,BE,BK," << "VI,VO,BE,BK," << "VI,VO,BE,BK," << "VI,VO,BE,BK" << "\n";

    for(int n = 0; n <= iterations; n++)
    {
        double currentTrafficAC = trafficStart + (n * trafficIncrement);

        // Set up 1 AP and 4 STAs (Voice, Video, Best Effort, Background) and set channels & Network
        NodeContainer wifiStaNodes;
        wifiStaNodes.Create(4); // 4 STAs for 4 different ACs
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

        // Setup QoS MAC for STAs

        // Setup QoS MAC for AP
        mac.SetType("ns3::ApWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid), "EnableBeaconJitter", BooleanValue(false));
        apDevice = wifi.Install(phy, mac, wifiApNodes.Get(0));

        // QoS configuration for AP
        Ptr<NetDevice> dev = wifiApNodes.Get(0)->GetDevice(0);
        Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
        Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
        PointerValue ptr;
        Ptr<QosTxop> edca;

        mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));

        // Configure EDCA for AC_VI (Video)
        wifi_mac->GetAttribute("VI_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(7);
        edca->SetMaxCw(15);
        staDevice[0] = wifi.Install(phy, mac, wifiStaNodes.Get(0));

        // Configure EDCA for AC_VO (Voice)
        wifi_mac->GetAttribute("VO_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(3);
        edca->SetMaxCw(7);
        staDevice[1] = wifi.Install(phy, mac, wifiStaNodes.Get(1));

        // Configure EDCA for AC_BE (Best Effort)
        wifi_mac->GetAttribute("BE_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);
        staDevice[2] = wifi.Install(phy, mac, wifiStaNodes.Get(2));

        // Configure EDCA for AC_BK (Background)
        wifi_mac->GetAttribute("BK_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(7);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);
        staDevice[3] = wifi.Install(phy, mac, wifiStaNodes.Get(3));

        //for (int j = 0; j < 4; j++) {
        //    staDevice[j] = wifi.Install(phy, mac, wifiStaNodes.Get(j));
        //}

        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(wifiStaNodes);
        mobility.Install(wifiApNodes);

        InternetStackHelper stack;
        stack.Install(wifiApNodes);
        stack.Install(wifiStaNodes);

        Ipv4AddressHelper address;
        address.SetBase("10.2.1.0", "255.255.255.0");
        Ipv4InterfaceContainer staInterface[4];
        Ipv4InterfaceContainer apInterface;
        for (int j = 0; j < 4; j++) {
            staInterface[j] = address.Assign(staDevice[j]);
        }
        apInterface = address.Assign(apDevice);

        //-----------------------------------------------------------
        ApplicationContainer apps;
        ApplicationContainer sourceApplications, sinkApplications;
        
        for (uint32_t index = 0; index < numFlows; index++) {
            for (uint8_t tosValue: tosValues) {
                auto ipv4 = wifiApNodes.Get (0)->GetObject<Ipv4> ();
                const auto address = ipv4->GetAddress (1, 0).GetLocal ();
                InetSocketAddress sinkSocket (address, port++);
                sinkSocket.SetTos (tosValue);
                OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
                onOffHelper.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
                onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));
                onOffHelper.SetAttribute("DataRate", StringValue(std::to_string(currentTrafficAC) + "Mbps"));
                onOffHelper.SetAttribute("PacketSize", UintegerValue(payloadSize));
                sourceApplications.Add (onOffHelper.Install (wifiStaNodes.Get (index)));
                PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
                sinkApplications.Add (packetSinkHelper.Install (wifiApNodes.Get (0)));
  
            }
        }

        //-----------------------------------------------------------
        // Run the simulation
        phy.EnablePcap("A3-Case-00-MAC-Dump", apDevice.Get(0));

        FlowMonitorHelper flowmon;
        Ptr<FlowMonitor> monitor = flowmon.InstallAll();

        double tput_array[16];
        uint32_t v = 0;
        Simulator::Stop(Seconds(4.0));
        Simulator::Run();
        monitor->CheckForLostPackets();
        Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
        FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

        for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i) {
          Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);

          std::cout << "Flow " << i->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
          std::cout << "  Tx Bytes: " << i->second.txBytes << "\n";
          std::cout << "  Rx Bytes: " << i->second.rxBytes << "\n";

          tput_array[v] = i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 ;

          std::cout << "Throughput: "<< tput_array[v] << "\n";

          //std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 / 1024 << " Mbps\n";
          v=v+1;
        }
        std::cout << "-------------------------------------------------" << "\n";
        flowmon.SerializeToXmlFile("throughput_results.xml", true, true);
        monitor->SerializeToXmlFile("throughput_results.flowmon", true, true);
        Simulator::Destroy();

        //---------------------------------------------
        // CSV Processing
        //---------------------------------------------
        
        outFileCSV << currentTrafficAC << ",";

        for (int i=0; i < 16; i++){
            if (i < 15) {
                outFileCSV << tput_array[i] << ",";
            }
            else {
                outFileCSV << tput_array[i];
            }
        }

        outFileCSV << "\n";

        double throughput = 0;
        for (unsigned index = 0; index < sinkApplications.GetN (); ++index)
         {
           uint64_t totalPacketsThrough = DynamicCast<PacketSink> (sinkApplications.Get (index))->GetTotalRx ();
           throughput += ((totalPacketsThrough * 8) / (n * 1000000.0)); //Mbit/s
         }

        std::cout << "Aggregated throughput: " << throughput << " Mbit/s" << std::endl;

    }

    outFileCSV.close();
    std::cout << "Simulation " << iteration << " completed. Results saved to " << fileName << std::endl;

    return 0;
}

int main(int argc, char* argv[])
{
    NS_LOG_UNCOND("\nStarting Simulator...\n");

    std::vector<uint8_t> tosValues = {0xa0, 0xc0, 0x00, 0x20}; // AC_VI, AC_VO, AC_BE, AC_BK
    std::string tosNames[] = {"AC_VI","AC_VO","AC_BE","AC_BK"};

    // Loop to run the simulation multiple times
    //
    std::cout << "Simulation 1" << "\n";
    for (int sim_iter = 1; sim_iter <= 1; sim_iter++)
    {
        Case00(tosValues,sim_iter);
    }
    
    std::cout << "Simulation 2" << "\n";

    int maxStaNum = 16;

    for (int sim_iter = 1; sim_iter <= 1; sim_iter++)
    {
        Case01(maxStaNum, tosValues, sim_iter);
    }
    
    return 0;
}
