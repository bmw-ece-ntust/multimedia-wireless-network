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

uint16_t port = 6969;
uint32_t payloadSize = 1472;
double trafficStart = 1.0;
double trafficEnd = 30.0;
double trafficIncrement = 3.0;
int numSimulations = 50;

int iterations = trafficEnd / trafficIncrement;
std::vector<std::vector<double>> throughputResults(iterations + 1, std::vector<double>(4, 0.0));

void SetupNetworkDevices(NodeContainer& staNodes, NodeContainer& apNode, NetDeviceContainer* staDevices, NetDeviceContainer& apDevice) {
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);
    wifi.SetRemoteStationManager("ns3::IdealWifiManager");

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
    phy.SetChannel(channel.Create());

    WifiMacHelper mac;
    Ssid ssid = Ssid("network");
    phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));

    mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
    for (int j = 0; j < 4; j++) {
        staDevices[j] = wifi.Install(phy, mac, staNodes.Get(j));
    }

    mac.SetType("ns3::ApWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid), "EnableBeaconJitter", BooleanValue(false));
    apDevice = wifi.Install(phy, mac, apNode.Get(0));
}

void ConfigureQosTxop(Ptr<NetDevice> dev) {
    Ptr<WifiNetDevice> wifiDev = DynamicCast<WifiNetDevice>(dev);
    Ptr<WifiMac> wifiMac = wifiDev->GetMac();
    PointerValue ptr;
    Ptr<QosTxop> edca;

    wifiMac->GetAttribute("VI_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(2);
    edca->SetMinCw(7);
    edca->SetMaxCw(15);

    wifiMac->GetAttribute("VO_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(2);
    edca->SetMinCw(3);
    edca->SetMaxCw(7);

    wifiMac->GetAttribute("BE_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(3);
    edca->SetMinCw(15);
    edca->SetMaxCw(1023);

    wifiMac->GetAttribute("BK_Txop", ptr);
    edca = ptr.Get<QosTxop>();
    edca->SetAifsn(7);
    edca->SetMinCw(15);
    edca->SetMaxCw(1023);
}

void RunSim(int iteration) {
    std::stringstream fileNameStream;
    fileNameStream << "fff" << std::setfill('0') << std::setw(2) << iteration << ".csv";
    std::string fileName = fileNameStream.str();

    std::ofstream outFileCSV(fileName);
    if (!outFileCSV.is_open()) {
        NS_LOG_ERROR("Unable to open output file.");
        return;
    }
    std::cout << "Output file " << fileName << " has been opened." << std::endl;

    for (int i = 0; i <= iterations; i++) {
        double currentTrafficAC = trafficStart + (i * trafficIncrement);

        NodeContainer staNodes;
        staNodes.Create(4);
        NodeContainer apNode;
        apNode.Create(1);

        NetDeviceContainer staDevices[4];
        NetDeviceContainer apDevice;
        SetupNetworkDevices(staNodes, apNode, staDevices, apDevice);

        Ptr<NetDevice> dev = apNode.Get(0)->GetDevice(0);
        ConfigureQosTxop(dev);

        MobilityHelper mobility;
        mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
        mobility.Install(staNodes);
        mobility.Install(apNode);

        InternetStackHelper stack;
        stack.Install(apNode);
        stack.Install(staNodes);

        Ipv4AddressHelper address;
        address.SetBase("192.168.1.0", "255.255.255.0");
        Ipv4InterfaceContainer staInterfaces[4];
        for (int j = 0; j < 4; j++) {
            staInterfaces[j] = address.Assign(staDevices[j]);
        }
        Ipv4InterfaceContainer apInterface = address.Assign(apDevice);

        UdpServerHelper server[4];
        ApplicationContainer serverApps[4];
        for (int j = 0; j < 4; j++) {
            server[j] = UdpServerHelper(port);
            serverApps[j] = server[j].Install(staNodes.Get(j));
            serverApps[j].Start(Seconds(0.0));
            serverApps[j].Stop(Seconds(4.0));
        }

        InetSocketAddress dest[4] = {
            InetSocketAddress(staInterfaces[0].GetAddress(0), port),
            InetSocketAddress(staInterfaces[1].GetAddress(0), port),
            InetSocketAddress(staInterfaces[2].GetAddress(0), port),
            InetSocketAddress(staInterfaces[3].GetAddress(0), port)
        };

        dest[0].SetTos(0xa0);
        dest[1].SetTos(0xf0);
        dest[2].SetTos(0x70);
        dest[3].SetTos(0x28);

        Ptr<ExponentialRandomVariable> onTimeRv = CreateObject<ExponentialRandomVariable>();
        onTimeRv->SetAttribute("Mean", DoubleValue(0.02));

        Ptr<ExponentialRandomVariable> offTimeRv = CreateObject<ExponentialRandomVariable>();
        offTimeRv->SetAttribute("Mean", DoubleValue(0.02));

        OnOffHelper client[4] = {
            OnOffHelper("ns3::UdpSocketFactory", dest[0]),
            OnOffHelper("ns3::UdpSocketFactory", dest[1]),
            OnOffHelper("ns3::UdpSocketFactory", dest[2]),
            OnOffHelper("ns3::UdpSocketFactory", dest[3])
        };

        ApplicationContainer clientApps[4];
        for (int j = 0; j < 4; j++) {
            client[j].SetAttribute("OnTime", PointerValue(onTimeRv));
            client[j].SetAttribute("OffTime", PointerValue(offTimeRv));
            client[j].SetAttribute("DataRate", StringValue(std::to_string(currentTrafficAC) + "Mb/s"));
            client[j].SetAttribute("PacketSize", UintegerValue(payloadSize));
            clientApps[j] = client[j].Install(apNode.Get(0));
            clientApps[j].Start(Seconds(1.0));
            clientApps[j].Stop(Seconds(4.0));
        }

        Simulator::Stop(Seconds(4.0));
        Simulator::Run();

        uint32_t totalPacketsThrough[4];
        for (int j = 0; j < 4; j++) {
            totalPacketsThrough[j] = DynamicCast<UdpServer>(serverApps[j].Get(0))->GetReceived();
        }

        Simulator::Destroy();

        double throughput[4];
        for (int j = 0; j < 4; j++) {
            throughput[j] = (totalPacketsThrough[j] * payloadSize * 8.0) / (3 * 1000000.0);
            throughputResults[i][j] += throughput[j];
        }

        std::cout << "Current Traffic: " << currentTrafficAC << " Mbps" << std::endl;
        std::cout << "Throughput (AC_VO): " << throughput[0] << " Mbps" << std::endl;
        std::cout << "Throughput (AC_VI): " << throughput[1] << " Mbps" << std::endl;
        std::cout << "Throughput (AC_BE): " << throughput[2] << " Mbps" << std::endl;
        std::cout << "Throughput (AC_BK): " << throughput[3] << " Mbps\n" << std::endl;

        outFileCSV << currentTrafficAC << "," << throughput[1] << "," << throughput[0] << "," << throughput[2] << "," << throughput[3] << "\n";
    }

    outFileCSV.close();
    std::cout << "Simulation " << iteration << " completed. Results saved to " << fileName << std::endl;
}

int main(int argc, char* argv[]) {
    NS_LOG_UNCOND("\nStarting Simulator...\n");

    std::cout << "Total Iterations: " << iterations << std::endl;
    std::cout << "Estimated time: " << iterations * numSimulations * 5 << " seconds" << std::endl;

    for (int sim_i = 1; sim_i <= numSimulations; sim_i++) {
        RunSim(sim_i);
    }

    std::cout << "All simulations complete" << std::endl;

    std::ofstream outFileCSV("average.csv");
    if (!outFileCSV.is_open()) {
        NS_LOG_ERROR("Unable to open output file.");
        return 1;
    }

    for (int i = 0; i <= iterations; i++) {
        double currentTrafficAC = trafficStart + (i * trafficIncrement);
        outFileCSV << currentTrafficAC;

        for (int j = 0; j < 4; j++) {
            double avgThroughput = throughputResults[i][j] / static_cast<double>(numSimulations);
            outFileCSV << "," << avgThroughput;
        }
        outFileCSV << "\n";
    }

    outFileCSV.close();
    std::cout << "Average results saved to average.csv" << std::endl;

    return 0;
}
