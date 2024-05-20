#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/mobility-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("EdcaThroughputSimulation");

void SetupEdca(Ptr<WifiMac> mac, enum AcIndex ac)
{
  PointerValue ptr;
  mac->GetAttribute("EdcaTxopN", ptr);
  Ptr<EdcaTxopN> edca = ptr.Get<EdcaTxopN>();
  edca->SetMinCw(7);  // Example for Voice (AC_VO)
  edca->SetMaxCw(15);
  edca->SetAifsn(2);
}

int main(int argc, char *argv[])
{
  uint32_t nStations = 10;
  CommandLine cmd;
  cmd.AddValue("nStations", "Number of stations", nStations);
  cmd.Parse(argc, argv);
  
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create(nStations);
  NodeContainer wifiApNode;
  wifiApNode.Create(1);

  YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
  YansWifiPhyHelper phy = YansWifiPhyHelper::Default();
  phy.SetChannel(channel.Create());

  WifiHelper wifi;
  wifi.SetStandard(WIFI_STANDARD_80211g);

  WifiMacHelper mac;
  Ssid ssid = Ssid("ns3-80211e-edca");

  mac.SetType("ns3::StaWifiMac",
              "Ssid", SsidValue(ssid),
              "ActiveProbing", BooleanValue(false));
  NetDeviceContainer staDevices = wifi.Install(phy, mac, wifiStaNodes);

  mac.SetType("ns3::ApWifiMac",
              "Ssid", SsidValue(ssid));
  NetDeviceContainer apDevice = wifi.Install(phy, mac, wifiApNode);

  MobilityHelper mobility;
  mobility.SetPositionAllocator("ns3::GridPositionAllocator",
                                "MinX", DoubleValue(0.0),
                                "MinY", DoubleValue(0.0),
                                "DeltaX", DoubleValue(5.0),
                                "DeltaY", DoubleValue(10.0),
                                "GridWidth", UintegerValue(3),
                                "LayoutType", StringValue("RowFirst"));
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(wifiStaNodes);
  mobility.Install(wifiApNode);

  InternetStackHelper stack;
  stack.Install(wifiApNode);
  stack.Install(wifiStaNodes);

  Ipv4AddressHelper address;
  address.SetBase("192.168.1.0", "255.255.255.0");
  Ipv4InterfaceContainer staInterfaces = address.Assign(staDevices);
  Ipv4InterfaceContainer apInterface = address.Assign(apDevice);

  // Install applications: UDP traffic generation for simplicity
  uint16_t port = 9;
  ApplicationContainer serverApp, clientApp;

  for (uint32_t i = 0; i < nStations; ++i)
  {
    UdpServerHelper server(port);
    serverApp.Add(server.Install(wifiStaNodes.Get(i)));

    UdpClientHelper client(apInterface.GetAddress(0), port);
    client.SetAttribute("MaxPackets", UintegerValue(1000000));
    client.SetAttribute("Interval", TimeValue(Seconds(0.01)));
    client.SetAttribute("PacketSize", UintegerValue(1024));
    clientApp.Add(client.Install(wifiStaNodes.Get(i)));
  }

  serverApp.Start(Seconds(1.0));
  clientApp.Start(Seconds(1.0));
  serverApp.Stop(Seconds(10.0));
  clientApp.Stop(Seconds(10.0));

  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll();

  Simulator::Stop(Seconds(10.0));
  Simulator::Run();

  monitor->CheckForLostPackets();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin(); i != stats.end(); ++i)
  {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
    NS_LOG_UNCOND("Flow ID: " << i->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
    NS_LOG_UNCOND("Tx Packets = " << i->second.txPackets);
    NS_LOG_UNCOND("Rx Packets = " << i->second.rxPackets);
    NS_LOG_UNCOND("Throughput: " << i->second.rxBytes * 8.0 / (i->second.timeLastRxPacket.GetSeconds() - i->second.timeFirstTxPacket.GetSeconds()) / 1024 << " Kbps");
  }

  Simulator::Destroy();
  return 0;
}


// Explanation of the Script:
// 1- Setting Up Nodes: Creates nStations number of station nodes and one access point node.

// 2- Configuring Wi-Fi: Uses YansWifiChannelHelper and YansWifiPhyHelper for the physical layer setup. Configures the Wi-Fi standard to 802.11g.

//3- Setting Up EDCA: Configures the EDCA parameters (CWmin, CWmax, AIFS) for different access categories using the SetupEdca function.

//4- Mobility Model: Sets up a grid position allocator to position the nodes in a grid layout.

//5- Network Stack and IP Addressing: Installs the Internet stack and assigns IP addresses.

//6- Applications: Sets up UDP server and client applications on each station node to generate traffic.

//7- Flow Monitoring: Uses FlowMonitorHelper to monitor and report flow statistics, including throughput.

// 8- Simulation Execution: Runs the simulation for 10 seconds and outputs the throughput for each flow. 

//Code to plot-

#include <stdio.h>
#include <stdlib.h>

// Function to create data files for each throughput data
void create_data_file(const char *filename, double *traffic_load, double *throughput, int size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < size; i++) {
        fprintf(file, "%lf %lf\n", traffic_load[i], throughput[i]);
    }
    
    fclose(file);
}

// Function to generate the plot using GNUplot
void plot_throughput() {
    FILE *gnuplot = popen("gnuplot -persistent", "w");
    if (gnuplot == NULL) {
        perror("Error opening GNUplot");
        exit(EXIT_FAILURE);
    }
    
    fprintf(gnuplot, "set title 'Throughput per AC (3 streams each), Simulation run 50 times'\n");
    fprintf(gnuplot, "set xlabel 'Offered traffic per AC (3 streams each) (Mb/s)'\n");
    fprintf(gnuplot, "set ylabel 'Throughput per AC (3 streams each) (Mb/s)'\n");
    fprintf(gnuplot, "set xrange [0:31]\n");
    fprintf(gnuplot, "set yrange [0:16]\n");
    fprintf(gnuplot, "set grid\n");
    fprintf(gnuplot, "plot 'vo_throughput.dat' using 1:2 with linespoints title 'AC_VO (voice)', \\\n");
    fprintf(gnuplot, "'vi_throughput.dat' using 1:2 with linespoints title 'AC_VI (video)', \\\n");
    fprintf(gnuplot, "'be_throughput.dat' using 1:2 with linespoints title 'AC_BE (best effort)', \\\n");
    fprintf(gnuplot, "'bk_throughput.dat' using 1:2 with linespoints title 'AC_BK (background)'\n");
    
    pclose(gnuplot);
}

int main() {
    // Data
    double traffic_load[] = {0, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31};
    double vo_throughput[] = {0, 0.48996, 1.99807, 3.54513, 4.89795, 6.43354, 8.00807, 9.42465, 10.8079, 12.4186, 13.6761, 15.2246};
    double vi_throughput[] = {0, 0.495613, 2.0144, 3.44315, 4.98525, 6.34444, 8.06263, 9.54178, 11.2008, 12.5169, 14.1095, 15.4735};
    double be_throughput[] = {0, 0.497732, 1.98881, 3.48546, 4.99028, 6.46424, 7.81479, 8.75436, 7.97588, 6.48591, 4.60975, 3.15181};
    double bk_throughput[] = {0, 0.491216, 1.9779, 3.45736, 5.02537, 6.30982, 5.71167, 3.42658, 2.48803, 1.83478, 1.44672, 1.03833};

    int size = sizeof(traffic_load) / sizeof(traffic_load[0]);

    // Create data files
    create_data_file("vo_throughput.dat", traffic_load, vo_throughput, size);
    create_data_file("vi_throughput.dat", traffic_load, vi_throughput, size);
    create_data_file("be_throughput.dat", traffic_load, be_throughput, size);
    create_data_file("bk_throughput.dat", traffic_load, bk_throughput, size);

    // Generate the plot using GNUplot
    plot_throughput();

    return 0;
}

// Code for plot_gnupplot.gp

set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output 'edca_throughput.png'

set title "Throughput per AC with Increasing Number of Stations (Constant Offered Traffic per Station)"
set xlabel "Number of Stations"
set ylabel "Throughput (Mbps)"
set grid

plot "edca_throughput.dat" using 1:2 with linespoints lt rgb "red" lw 2 pt 7 ps 1.5 title "Voice (AC_VO)", \
     "edca_throughput.dat" using 1:3 with linespoints lt rgb "blue" lw 2 pt 7 ps 1.5 title "Video (AC_VI)", \
     "edca_throughput.dat" using 1:4 with linespoints lt rgb "green" lw 2 pt 7 ps 1.5 title "Best Effort (AC_BE)", \
     "edca_throughput.dat" using 1:5 with linespoints lt rgb "purple" lw 2 pt 7 ps 1.5 title "Background (AC_BK)" 




$  gcc -o generate_data generate_data.c
$ ./generate_data

$gnuplot plot_edca_throughput.gp
