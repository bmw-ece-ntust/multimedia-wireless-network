 /* 
 *   Simple wifi Simulation having 1AP and 2STAs
 *
 *    Topology:  STA Node A       AP     STA Node B
 *              (-5,0) ------>   (0,0) <-----(5,0)
 *
 *   Author: Desalegn Aweke
 *
 *  - Wifi in an Error channel
 *  - Simulate throughput,delay
 *  - IP flow monitor
 */


#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/netanim-module.h"  // Include the netanim module
#include <fstream> // Include the header for file operations

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("WifiSimulation");

int main(int argc, char *argv[]) {
    // Setup parameters
    bool tracing = false;
    double txPower = 20;  // transmission power in dBm
    double ccaEdThreshold = -99;  // CCA-ED threshold in dBm
    bool errorModel = true;  // set to false for an error-free channel

    // Parse command line arguments
    CommandLine cmd;
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);
    cmd.Parse(argc, argv);

    // Create nodes
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(2);
    NodeContainer wifiApNode;
    wifiApNode.Create(1);

    // Setup WiFi channel and physical layer
    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetChannel(channel.Create());

    // Set transmission power and CCA-ED threshold
    phy.Set("TxPowerStart", DoubleValue(txPower));
    phy.Set("TxPowerEnd", DoubleValue(txPower));
    phy.Set("CcaEdThreshold", DoubleValue(ccaEdThreshold));

    // Setup WiFi
    WifiHelper wifi;

    // Set error model
    if (errorModel) {
        phy.SetErrorRateModel("ns3::NistErrorRateModel");
        wifi.SetRemoteStationManager("ns3::MinstrelHtWifiManager");
    }
    else {
        phy.SetErrorRateModel("ns3::YansErrorRateModel");
        wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager");
    }

    // Setup WiFi MAC
    WifiMacHelper mac;
    Ssid ssid = Ssid("ns-3-ssid");
    mac.SetType("ns3::StaWifiMac",
                 "Ssid", SsidValue(ssid),
                 "ActiveProbing", BooleanValue(false));

    // Install WiFi devices
    NetDeviceContainer staDevices = wifi.Install(phy, mac, wifiStaNodes);
    mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
    NetDeviceContainer apDevices = wifi.Install(phy, mac, wifiApNode);

    // Mobility
    MobilityHelper mobility;
    mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(wifiApNode);
    mobility.Install(wifiStaNodes);

    // Set velocities
    Ptr<ConstantVelocityMobilityModel> apMob = wifiApNode.Get(0)->GetObject<ConstantVelocityMobilityModel>();
    apMob->SetVelocity(Vector(1, 0, 0)); // Constant velocity of 1 m/s
    Ptr<ConstantVelocityMobilityModel> sta1Mob = wifiStaNodes.Get(0)->GetObject<ConstantVelocityMobilityModel>();
    sta1Mob->SetVelocity(Vector(1, 0, 0)); // Constant velocity of 1 m/s
    Ptr<ConstantVelocityMobilityModel> sta2Mob = wifiStaNodes.Get(1)->GetObject<ConstantVelocityMobilityModel>();
    sta2Mob->SetVelocity(Vector(-1, 0, 0)); // Constant velocity of -1 m/s

    // Setup internet stack
    InternetStackHelper stack;
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);

    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer staNodeInterfaces;
    staNodeInterfaces = address.Assign(staDevices);
    Ipv4InterfaceContainer apNodeInterface;
    apNodeInterface = address.Assign(apDevices);

    // Setup UDP echo server
    uint16_t port = 9;
    UdpEchoServerHelper server(port);
    ApplicationContainer serverApps = server.Install(wifiApNode.Get(0));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    // Setup UDP echo client
    UdpEchoClientHelper client(apNodeInterface.GetAddress(0), port);
    client.SetAttribute("MaxPackets", UintegerValue(1));
    client.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    client.SetAttribute("PacketSize", UintegerValue(1024));
    ApplicationContainer clientApps = client.Install(wifiStaNodes);
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    // Setup flow monitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // Stop simulator after 10 seconds
    Simulator::Stop(Seconds(10.0));

    // Add this before running the simulation
    AnimationInterface anim("Animwifi.xml");
    anim.SetConstantPosition(wifiApNode.Get(0), 0, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(0), -5, 0);
    anim.SetConstantPosition(wifiStaNodes.Get(1), 5, 0);

    // Enable tracing if specified
    if (tracing) {
        phy.EnablePcap("WifiSimulation", apDevices.Get(0));
        phy.EnablePcap("WifiSimulation", staDevices.Get(0));
    }

    // Run the simulation
    Simulator::Run();

    monitor->CheckForLostPackets();

    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

    double totalThroughput = 0;
    double totalDelay = 0;
    int flowCount = 0;

    // Open a CSV file for writing
    std::ofstream outputFile("results_wifi.csv");

    // Write header to CSV file
    outputFile << "Flow,Source,Destination,TxBytes,RxBytes,Throughput(Mbps),Delay(s)" << std::endl;

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i) {
        Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);

        double throughput = i->second.rxBytes * 8.0 / 9.0 / 1024 / 1024;

        double delay = 0;
        if (i->second.rxPackets > 1) {
            delay = (i->second.delaySum.GetSeconds() / i->second.rxPackets);
        }

        // Write flow statistics to txt file
        outputFile << flowCount << ","
                   << t.sourceAddress << ","
                   << t.destinationAddress << ","
                   << i->second.txBytes << ","
                   << i->second.rxBytes << ","
                   << throughput << ","
                   << delay << std::endl;

        flowCount++;

        totalThroughput += throughput;

        if (i->second.rxPackets > 1) {
            totalDelay += delay;
        }

        std::cout << "Flow " << flowCount << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")\n";
        std::cout << "Tx Bytes: " << i->second.txBytes << "\n";
        std::cout << "Rx Bytes: " << i->second.rxBytes << "\n";
        std::cout << "Throughput: " << throughput << " Mbps\n";
        std::cout << "Delay: " << delay << " s\n";
    }

    // Close the CSV file
    outputFile.close();

    double averageThroughput = totalThroughput / flowCount;
    double averageDelay = totalDelay / flowCount;

    std::cout << "Average Throughput: " << averageThroughput << " Mbps\n";
    std::cout << "Average Delay: " << averageDelay << " s\n";

    // Add this after running the simulation
    monitor->SerializeToXmlFile("flowwifi.xml", true, true);

    // Cleanup and exit
    Simulator::Destroy();
    return 0;
}

