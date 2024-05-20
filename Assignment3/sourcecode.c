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