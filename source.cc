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

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211eTxop");


// Simulation parameters
uint16_t port = 6969;
uint32_t payloadSize = 1472; // bytes
double trafficStart = 1.0;
double trafficEnd = 30.0;
double trafficIncrement = 3.0;

int iterations = trafficEnd / trafficIncrement;


void RunSimulation(int iteration)
{
    std::stringstream fileNameStream;
    fileNameStream << "/home/mateoalvarenga/Desktop/simulations" << std::setfill('0') << std::setw(2) << iteration << ".csv";
    std::string fileName = fileNameStream.str();


    // Open the output file 
    std::ofstream outFileCSV(fileName);
    if (!outFileCSV.is_open()) {
        NS_LOG_ERROR("Unable to open output file.");
        return;
    }
    std::cout << "Output file " << fileName << " has been opened." << std::endl;

    for(int i = 0; i <= iterations; i++)
    {
        double currentTrafficAC = trafficStart + (i * trafficIncrement);

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
        mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
        for (int j = 0; j < 4; j++) {
            staDevice[j] = wifi.Install(phy, mac, wifiStaNodes.Get(j));
        }

        // Setup QoS MAC for AP
        mac.SetType("ns3::ApWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid), "EnableBeaconJitter", BooleanValue(false));
        apDevice = wifi.Install(phy, mac, wifiApNodes.Get(0));

        // QoS configuration for AP
        Ptr<NetDevice> dev = wifiApNodes.Get(0)->GetDevice(0);
        Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
        Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
        PointerValue ptr;
        Ptr<QosTxop> edca;

        // Configure EDCA for AC_VI (Video)
        wifi_mac->GetAttribute("VI_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(7);
        edca->SetMaxCw(15);

        // Configure EDCA for AC_VO (Voice)
        wifi_mac->GetAttribute("VO_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(3);
        edca->SetMaxCw(6);

        // Configure EDCA for AC_BE (Best Effort)
        wifi_mac->GetAttribute("BE_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(4);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);

        // Configure EDCA for AC_BK (Background)
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
            serverApp[j].Stop(Seconds(4.0));
        }

        // Initialize the InetSocketAddress array with valid IPv4 addresses and ports
        InetSocketAddress dest[4] = {
            InetSocketAddress(staInterface[0].GetAddress(0), port), // AC_VI
            InetSocketAddress(staInterface[1].GetAddress(0), port), // AC_VO
            InetSocketAddress(staInterface[2].GetAddress(0), port), // AC_BE
            InetSocketAddress(staInterface[3].GetAddress(0), port)  // AC_BK
        };

        dest[0] = InetSocketAddress(staInterface[0].GetAddress(0), port); // AC_VI
        dest[0].SetTos(0xb8);
        dest[1] = InetSocketAddress(staInterface[1].GetAddress(0), port); // AC_VO
        dest[1].SetTos(0xc0);
        dest[2] = InetSocketAddress(staInterface[2].GetAddress(0), port); // AC_BE
        dest[2].SetTos(0xa0);
        dest[3] = InetSocketAddress(staInterface[3].GetAddress(0), port); // AC_BK
        dest[3].SetTos(0x20);

        Ptr<ExponentialRandomVariable> onTimeRv = CreateObject<ExponentialRandomVariable>();
        onTimeRv->SetAttribute("Mean", DoubleValue(0.02));

        Ptr<ExponentialRandomVariable> offTimeRv = CreateObject<ExponentialRandomVariable>();
        offTimeRv->SetAttribute("Mean", DoubleValue(0.02));

        // Clients Configuration and Simulation
        
        // Initialize the OnOffHelper array with the required arguments
        OnOffHelper client[4] = {
            OnOffHelper("ns3::UdpSocketFactory", dest[0]), // AC_VI
            OnOffHelper("ns3::UdpSocketFactory", dest[1]), // AC_VO
            OnOffHelper("ns3::UdpSocketFactory", dest[2]), // AC_BE
            OnOffHelper("ns3::UdpSocketFactory", dest[3])  // AC_BK
        };
        ApplicationContainer clientApp[4];
        for (int j = 0; j < 4; j++) {
            client[j] = OnOffHelper("ns3::UdpSocketFactory", dest[j]);
            client[j].SetAttribute("OnTime", PointerValue(onTimeRv));
            client[j].SetAttribute("OffTime", PointerValue(offTimeRv));
            client[j].SetAttribute("DataRate", StringValue(std::to_string(currentTrafficAC) + "Mbps"));
            client[j].SetAttribute("PacketSize", UintegerValue(payloadSize));
            clientApp[j] = client[j].Install(wifiApNodes.Get(0));
            clientApp[j].Start(Seconds(1.0));
            clientApp[j].Stop(Seconds(4.0));
        }

        // Run the simulation
        Simulator::Stop(Seconds(4.0));
        Simulator::Run();

        uint32_t totalPacketsThrough[4];
        for (int j = 0; j < 4; j++) {
            totalPacketsThrough[j] = DynamicCast<UdpServer>(serverApp[j].Get(0))->GetReceived();
        }

        Simulator::Destroy();

        // Calculate throughput
        double throughput[4];
        for (int j = 0; j < 4; j++) {
            throughput[j] = (totalPacketsThrough[j] * payloadSize * 8.0) / (3 * 1000000.0); // Mbps
        }

        // Display Results in console
        std::cout << "Current Traffic: " << currentTrafficAC << " Mbps" << std::endl;
        std::cout << "Throughput (AC_VO): " << throughput[0] << " Mbps" << std::endl;
        std::cout << "Throughput (AC_VI): " << throughput[1] << " Mbps" << std::endl;
        std::cout << "Throughput (AC_BE): " << throughput[2] << " Mbps" << std::endl;
        std::cout << "Throughput (AC_BK): " << throughput[3] << " Mbps\n" << std::endl;

        // Save results to the CSV file
        outFileCSV << currentTrafficAC << "," << throughput[0] << "," << throughput[1] << "," << throughput[2] << "," << throughput[3] << "\n";
    }

    // Close the CSV file
    outFileCSV.close();
    std::cout << "Simulation " << iteration << " completed. Results saved to " << fileName << std::endl;

    return;
}

int main(int argc, char* argv[])
{
    NS_LOG_UNCOND("\nStarting Simulator...\n");

    std::cout << "Total Iterations: " << iterations << std::endl;
    std::cout << "Estimated time: " << iterations * 5 * 5 << " seconds" << std::endl;

    // Loop to run the simulation multiple times
    for (int sim_i = 1; sim_i <= 5; sim_i++)
    {
        RunSimulation(sim_i);

    }

    std::cout << "All simulations complete\n";

    
    return 0;
}
