# Assignment 2
by Victor Cano

## Background And Introduction

NS3 is a framework for network simulations that can be used to test different kinds of environments and applications in such a way that allows for experimentation with performance, QoS, distance, WiFi Standard, etc.

The focus of this assignment is to simulate the 802.11 standard with no QoS implementation. In this case I selected the 802.11a generation of the standard and didn't implement the QoS seen in 802.11e.
## Using NS3

### Problems
When Starting to use NS3 I faced some issues, specifically related to the installation process and the pre-requisites.

When first trying to install NS3 my ubuntu virtual machine stopped working completely, prompting me to re-install a clean version and set up everything from the ground up.

Later, when I managed to test and slowly make my way through understanding the installation process I noticed that I was missing many libraries and pre-requisites used by NS3 that I had to install.

Finally after some time I managed to find and install are necessary requirements and I was able to run some test examples and simulations.

### Difficulties
NS3 poses a big difficulty for me because it is a fairly unique tool compared to what I'm used to, and even if I do have some experience with C based languages I struggled with understanding the structure and inner-workings of an NS3 simulation code because of how it is structured differently than other tools that I used before.

I managed to understand some parts of it by analyzing and experimenting with example code files and investigating online resources such as the documentation and youtube tutorials
### What Did I learn from NS3?
Using NS3 I learned and experimented with many concepts related to computer networks and programming.
For starters I learned about the tool itself, got some basic understanding of how to write and run a simulation.

I also learned about the effects different parameters such as distance and packet sizes can have in the final performance of the transmission.

Another thing I learned is related to some different kinds of networks, such as a P2P network or bus network, both of which I investigated while learning NS3.

I can still learn more as I get further experience with the tool and all the different kinds of simulations that acn be done.
## Code Analysis
```C++
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
#include "ns3/flow-monitor-module.h"
#include "ns3/mac48-address.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("80211");
  
int main(int argc, char* argv[])
{
	
	uint32_t payloadSize = 1536; // bytes
	double simulationTime = 10; // seconds
	double distance = 5; // meters
	
	CommandLine cmd(__FILE__);
	cmd.AddValue("payloadSize", "Payload size in bytes", payloadSize);
	cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
	cmd.AddValue("distance","Distance in meters between the station and the access point", distance);
	cmd.Parse(argc, argv);
	
	// Defining the Nodes used for the simulation: 3 Stations and 1 Access Point
	
	NodeContainer wifiStaNodes;
	wifiStaNodes.Create(3);
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
	
	  
	
	NetDeviceContainer staDeviceA;
	NetDeviceContainer staDeviceB;
	NetDeviceContainer staDeviceC;
	
	NetDeviceContainer apDeviceA;
	
	  
	
	Ssid ssid;
	
	//Network definition
	
	ssid = Ssid("network-A");
	
	phy.Set("ChannelSettings", StringValue("{48, 20, BAND_5GHZ, 0}"));
	
	mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(false), "Ssid", SsidValue(ssid));
	staDeviceA = wifi.Install(phy, mac, wifiStaNodes.Get(0));
	
	mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(false), "Ssid", SsidValue(ssid));
	staDeviceB = wifi.Install(phy, mac, wifiStaNodes.Get(1));
	
	mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(false), "Ssid", SsidValue(ssid));
	staDeviceC = wifi.Install(phy, mac, wifiStaNodes.Get(2));
	
	  
	
	mac.SetType("ns3::ApWifiMac", "QosSupported", BooleanValue(false), "Ssid", SsidValue(ssid), "EnableBeaconJitter", BooleanValue(false));
	apDeviceA = wifi.Install(phy, mac, wifiApNodes.Get(0));
	
	
	/* Setting mobility model */
	
	MobilityHelper mobility;
	
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
	
	mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	
	// Set position for APs
	
	positionAlloc->Add(Vector(0.0, 0.0, 0.0));
	
	// Set position for STAs
	
	positionAlloc->Add(Vector(distance, 0.0, 0.0));
	positionAlloc->Add(Vector(10 + distance, 0.0, 0.0));
	positionAlloc->Add(Vector(20 + distance, 0.0, 0.0));
	
	mobility.SetPositionAllocator(positionAlloc);
	mobility.Install(wifiApNodes);
	mobility.Install(wifiStaNodes);
	
	  
	
	/* Internet stack */
	
	InternetStackHelper stack;
	stack.Install(wifiApNodes);
	stack.Install(wifiStaNodes);
	
	  
	
	Ipv4AddressHelper address;
	address.SetBase("192.168.1.0", "255.255.255.0");
	Ipv4InterfaceContainer StaInterfaceA;
	StaInterfaceA = address.Assign(staDeviceA);
	
	  
	Ipv4InterfaceContainer ApInterfaceA;
	ApInterfaceA = address.Assign(apDeviceA);
	
	  
	
	Ipv4InterfaceContainer StaInterfaceB;
	StaInterfaceB = address.Assign(staDeviceB);
	
	  
	
	Ipv4InterfaceContainer StaInterfaceC;
	StaInterfaceC = address.Assign(staDeviceC);
	
	  
	
	/* Setting applications */
	
	uint16_t port = 5001;
	
	UdpServerHelper serverA(port);
	
	ApplicationContainer serverAppA = serverA.Install(wifiApNodes.Get(0));
	serverAppA.Start(Seconds(0.0));
	serverAppA.Stop(Seconds(simulationTime + 1));
	
	InetSocketAddress destA(ApInterfaceA.GetAddress(0), port);
	destA.SetTos(0x70);
	
	OnOffHelper clientA("ns3::UdpSocketFactory", destA);
	clientA.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
	clientA.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
	clientA.SetAttribute("DataRate", StringValue("100000kb/s"));
	clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));
	
	ApplicationContainer clientAppA = clientA.Install(wifiStaNodes.Get(0));
	clientAppA.Start(Seconds(1.0));
	clientAppA.Stop(Seconds(simulationTime + 1));
	
	OnOffHelper clientB("ns3::UdpSocketFactory", destA);
	clientB.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
	clientB.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
	clientB.SetAttribute("DataRate", StringValue("100000kb/s"));
	clientB.SetAttribute("PacketSize", UintegerValue(payloadSize));
	
	ApplicationContainer clientAppB = clientB.Install(wifiStaNodes.Get(1));
	clientAppB.Start(Seconds(1.0));
	clientAppB.Stop(Seconds(simulationTime + 1));
	
	
	OnOffHelper clientC("ns3::UdpSocketFactory", destA);
	clientC.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
	clientC.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
	clientC.SetAttribute("DataRate", StringValue("100000kb/s"));
	clientC.SetAttribute("PacketSize", UintegerValue(payloadSize));
	
	ApplicationContainer clientAppC = clientC.Install(wifiStaNodes.Get(2));
	clientAppC.Start(Seconds(1.0));
	clientAppC.Stop(Seconds(simulationTime + 1));
	
	FlowMonitorHelper flowmon;
	Ptr<FlowMonitor> monitor = flowmon.InstallAll();
	Simulator::Stop(Seconds(simulationTime + 1));
	Simulator::Run();
	
	  
	
	uint32_t SentPackets = 0;
	uint32_t ReceivedPackets = 0;
	uint32_t LostPackets = 0;
	int j=0;
	float AvgThroughput = 0;
	Time Jitter;
	Time Delay;
	
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
	
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
	{
	
		SentPackets = SentPackets +(iter->second.txPackets);
		ReceivedPackets = ReceivedPackets + (iter->second.rxPackets);
		LostPackets = LostPackets + (iter->second.txPackets-iter->second.rxPackets);
		
		AvgThroughput = AvgThroughput + (iter->second.rxBytes * 8.0/(iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds())/1024);
		
		Delay = Delay + (iter->second.delaySum);
		Jitter = Jitter + (iter->second.jitterSum);
		
		j = j + 1;
	
	}
	
	  
	
	/* Show results */
	
	double throughput = AvgThroughput / (j-1);
	double delay = Delay.GetNanoSeconds() / SentPackets;
	double jitter = Jitter.GetNanoSeconds() / SentPackets;
	
	std::cout << " Average Throughput = " << throughput/1000 << " Mbit/s" << "\n";
	std::cout << " Average Delay = " << delay << " ns" << "\n";
	std::cout << " Average Jitter = " << jitter << " ns" << "\n";
	
	Simulator::Destroy();
	return 0;
	
	}
```

This code is based on one of the example files included with the NS3 environment.

In this case I defined a network composed of 3 stations and 1 access point, using the 802.11a Wifi standard and without implementing any QoS features such as the ones included in 802.11e.

Using the flow monitor module I obtain and print certain statistics related to the simulation, such as the average delay, jitter and thoughput.

This simulation uses an approximate distance of 5 meters between the Access Point and each Station

#### Result
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/bd39fa70-f888-4ce6-904b-870944be934a)

#### Code Flowchart
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/37097892-ef64-4521-809a-9a0f52e436e0)

### Conclusion
The results show a rather interesting number for both the speed and delay, and my expectations are that by implementing the 802.11e QoS services I'll see an upgrade in terms of speed and delay, reducing the time it takes to send the most important packages and providing further stability to the transfer.
