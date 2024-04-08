

# <center> Study Note on Wi-Fi Network simulation </center>
- [Objective of the Assignment](#Objective-of-the-Assignment:)
- [Five minute presentation](#Five-minute-presentation)
- [How to read a paper](#how-to-read-a-paper)
- [How to write a report](#how-to-write-a-report)
- [Ten tips for better teamwork](#Ten-tips-for-better-teamwork)
# Objective of the Assignment: 
* Execute an example for Wi-Fi network
* Configure an IEEE 802.11n Wi-Fi network.
* Verify the idea of Wi-Fi through the Ns3 simulation
* Generate  the average throughput and average delay  which depends on the number of stations
* Analyze the behavior of the system with the increase in number of stations 
    * Study 1:
        * Average throughput 
        * Number of station: ranging from 1 to 14
    * Study 2:
        * Mean delays 
        * Number of station: ranging from 1 to 14
# Part I: Learning basic concept of ns3
<p align="justify">
Ns-3 simulator is a discrete-event network simulator primarily designed for research and educational purposes, usually operated from the command line. It's coded in C++ and created to offer an open, customizable network simulation platform for both networking research and education. Essentially, ns-3 provides models depicting the functionality and performance of packet data networks, along with a simulation engine empowering users to conduct diverse simulation experiments. Its evolution hinges on the continuous contributions from the community, encompassing the development of novel models, debugging and upkeep of existing ones, and the dissemination of findings and outcomes.
 </p>
 

To install ns-3 (Network Simulator version 3) using virtual box in windows, you typically follow these steps:

* Step 1：Prerequisites:
    * Make sure your system meets the prerequisites for ns-3 installation. Make sure your system meets the prerequisites for ns-3 installation. The main features of ns-3 just require a C++ compiler (g++ or clang++), Python (version 3.6 or above), CMake and a build-system (e.g. make, ninja, Xcode Download ns-3
* Step 2：Download ns-3:
    * Visit the official ns-3 website (https://www.nsnam.org/) or the GitHub repository (https://github.com/nsnam/ns-3-dev-git) to download the latest version of ns-3.
* Step 3：Extract the archive:
    * Once the download is complete, unzip or untar the ns-allinone-3.38.tar.bz2 to the home folder
* Step 4：Configuration:
    * Open a terminal and navigate to the extracted ns-3 directory.
    * Run the configuration script. This script will check your system for necessary dependencies and set up the ns-3 build environment.

* Step 5：Build ns-3:
    * After configuration completes without errors, run the build command: ./build. This command compiles the ns-3 codebase and generates the necessary binaries.
* Step 6：Testing:
    * Optionally, you can run the test suite to ensure that ns-3 was built correctly. Execute: ./test.py or ./test.py -c (for a more detailed output).
* Step 7：Environment Setup:
    * Set up environment variables like NS3_HOME pointing to the ns-3 installation directory to make it easier to use ns-3 commands and scripts.
* Step 8：Additional Libraries and Modules:
    * Depending on your requirements, you might need to install additional libraries or ns-3 modules. These can usually be downloaded from the ns-3 website or GitHub repositories.
* Step 9：Documentation:
    * Refer to the ns-3 documentation (either online or in the downloaded package) for detailed information on using ns-3, including tutorials and examples.

## Environment requirements for ns3 installation
* Ubuntu Version: ubuntu-22.04.LTS-desktop-amd64
* Disk: 50GB
* Memory:8GB
* CPU: Core i7
* NS-3.41

# Part II: Simulated Wi-Fi network using ns3 

 ## Introduction
 <p align="justify">
IEEE 802.11 is the official IEEE term for wireless networking, encompassing various specifications. Several wireless networking specifications such as 802.11a, 802.11b, 802.11g, and 802.11n exist under the 802.11 banner. These standards employ the Ethernet protocol and utilize the CSMA/CA access method.
The most recent wireless standard listed in the Network+ objectives is 802.11n. This standard aims to notably enhance throughput in both the 2.4GHz and 5GHz frequency bands.  The baseline goal is initially targeting speeds of 100Mbps, under optimal conditions, it's projected that 802.11n could achieve speeds as high as 600Mbps. However, In practical operation, actual 802.11n speeds are likely to be considerably lower.
</p>

| IEEE Standard |Frequency/Medium   | Speed | Topology | Transmission Range | Access Method |
| -------- | -------- | -------- | -------- | -------- | -------- |
|    802.11n  |2.4GHz/5GHz    |  Up to 600Mbps    | Ad hoc/infrastructure      | 175+ feet indoors;  affected by building materials      | CSMA/CA      |

### Assumptions and models used in the simulation 
* LogDistancePropagationLossModel is employed to illustrate the channel is not error free. 
* No hidden terminal was considered
* ConstantSpeedPropagationDelayModel is used.
* The results shown are only for mcs value of 7(Modulation and coding scheme)
* Flow monitor Helper is used to gather the throughput and delay stastics 

### Steps to create Wi-Fi network in NS-3

* Step 1: Create Wi-Fi station and access nodes.
* Step 2: Call any wifiChannelHelper (YANS Wi-Fi) to create Wi-Fi networks.
* Step 3: Create the physical layer  (to set the channel) and Data link layer (to set SSID call any  remoteStation Manager) environment  to the Wi-Fi network  (IEEE 802.11 WLAN network).
* Step 4: Set the SSID Wi-Fi station nodes and install the device on the node using NetDeviceContainer.
* Step 5: Set the SSID Wi-Fi access point node and install the device on the node using NetDeviceContainer.
* Step 6: Set the position by calling SetPositionAllocator for the mobility of node signals.
* Step 7: Set mobility model (ConstantPositioningMobilityModel) for Wi-Fi Access point.
* Step 8:Set mobility model (ConstantPositioningMobility) for Wi-Fi station nodes

## Experimental setup
The topology I implement to simulate the IEEE 802.11 Wi-Fi throughput and is shown below, consisting of one access point and N stations.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-D11215805-Dawit-Hadush-Hailu/assets/topo.PNG)

## Simulation code
 <p align="justify">
This is simulation code that shows how to configure an IEEE 802.11n Wi-Fi network. It outputs the average throughput and average delay, which depends on the number of stations.  The user can select the distance between the stations, the AP and can enable/disable the RTS/CTS mechanism, the HT MCS value (0 to 7), the channel width (20 or 40 MHz) and the guard interval (long or short). The user can also specify whether RTS/CTS is used or not.
</p>

```c++
./ns3 run "wifi-network --numStations=5 --distance=5 --mcs=7 --useRts=false --simulationTime=10 --channelwidth=40 - useShortGuardInterval=false"
```

Below is the source code of the simulation. 
```c++
#include "ns3/core-module.h"
#include "ns3/mobility-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-probe.h"
#include "ns3/command-line.h"
#include "ns3/netanim-module.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("WifiDelay_throughput_Calculation");

int main(int argc, char *argv[]) {
	uint32_t numStations = 20;
	double distance = 5.0;      // meters
	uint16_t mcs = 7;
	bool useRts = false;
	double simulationTime = 10; // seconds
	uint8_t channelWidth = 20; // MHz
    bool useShortGuardInterval = false;
  	 uint32_t payloadSize = 1000; // bytes
	CommandLine cmd;
    cmd.AddValue("numStations", "Number of stations", numStations);
    cmd.AddValue("distance","Distance in meters between the stations and the access point",
                 distance);
    cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
    cmd.AddValue("useRts", "Enable/disable RTS/CTS", useRts);
    cmd.AddValue("mcs", "MCS value (0 - 7)", mcs);
    cmd.AddValue("channelWidth", "Channel width in MHz", channelWidth);
    cmd.AddValue("useShortGuardInterval",
                 "Enable/disable short guard interval",
                 useShortGuardInterval);
	cmd.Parse(argc, argv);
	// Create nodes
	NodeContainer wifiStaNodes;
	wifiStaNodes.Create(numStations);
	NodeContainer wifiApNode;
	wifiApNode.Create(1);

	YansWifiChannelHelper channel;
	channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel", "ReferenceLoss", DoubleValue (40.089)); 
	YansWifiPhyHelper phy;
	phy.SetChannel(channel.Create());

	// Set up Wi-Fi
	WifiMacHelper mac;
	WifiHelper wifi;
	wifi.SetStandard(WIFI_STANDARD_80211n);

	std::ostringstream oss;
	oss << "HtMcs" << mcs;

	wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
							 "DataMode",
							 StringValue(oss.str()),
							 "ControlMode",
							 StringValue(oss.str()),
							 "RtsCtsThreshold",
							 UintegerValue(useRts ? 0 : 999999));
	Ssid ssid = Ssid("ns3-80211n");
	mac.SetType("ns3::StaWifiMac", "Ssid", SsidValue(ssid));
	NetDeviceContainer staDevices = wifi.Install(phy, mac, wifiStaNodes);
	mac.SetType("ns3::ApWifiMac", "Ssid", SsidValue(ssid));
	NetDeviceContainer apDevice = wifi.Install(phy, mac, wifiApNode);
	// Set channel width
	Config::Set("/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/Phy/ChannelSettings",
		StringValue("{0, " + std::to_string(channelWidth) + ", BAND_2_4GHZ, 0}"));

	// Set guard interval
	Config::Set(
	"/NodeList/*/DeviceList/*/$ns3::WifiNetDevice/HtConfiguration/ShortGuardIntervalSupported",
	BooleanValue(useShortGuardInterval));

	// mobility
	MobilityHelper mobilityAP;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
	positionAlloc->Add(Vector(0.0, 0.0, 0.0));
	mobilityAP.SetPositionAllocator (positionAlloc);
	mobilityAP.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobilityAP.Install (wifiApNode);

	MobilityHelper mobilitySTA;

	for (uint32_t i = 0; i < numStations; i++)
	{
	positionAlloc->Add(Vector(distance, 0.0, 0.0));
	}
	mobilitySTA.SetPositionAllocator(positionAlloc);
	mobilitySTA.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobilitySTA.Install(wifiStaNodes);

	// Set up internet stack
	InternetStackHelper stack;
	stack.Install(wifiStaNodes);
	stack.Install(wifiApNode);

	// Assign IP addresses
	Ipv4AddressHelper address;
	address.SetBase("192.168.1.0", "255.255.255.0");
	
	Ipv4InterfaceContainer staInterfaces;
    Ipv4InterfaceContainer apNodeInterface;
    staInterfaces = address.Assign(staDevices);
    apNodeInterface = address.Assign(apDevice);


	// Set up applications
	uint16_t port = 5000;
	OnOffHelper onOffHelper("ns3::UdpSocketFactory",
					  Address(InetSocketAddress(staInterfaces.GetAddress(numStations - 1), port)));
	onOffHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
	onOffHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
	ApplicationContainer app = onOffHelper.Install(wifiStaNodes.Get(0));
	app.Start(Seconds(1.0));
	app.Stop(Seconds(simulationTime + 1));

	// Set up flow monitor

	FlowMonitorHelper flowMonitorhelper;
	Ptr<FlowMonitor> monitor = flowMonitorhelper.InstallAll();
	// Run simulation
	Simulator::Stop(Seconds(simulationTime + 1));
	Simulator::Run();

	// Calculate and output delay data
	monitor->CheckForLostPackets();
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowMonitorhelper.GetClassifier());
	std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats();

	double totalDelay = 0.0;
	uint32_t numFlows = 0;
	double totalThroughput = 0.0;
	for (auto it = stats.begin(); it != stats.end(); ++it) {
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(it->first);
		NS_LOG_UNCOND("Flow " << it->first << " (" << t.sourceAddress << " -> " << t.destinationAddress << ")");
		
		NS_LOG_UNCOND("  Tx Bytes: " << it->second.txBytes);
		NS_LOG_UNCOND("  Rx Bytes: " << it->second.rxBytes);
		double throughput = (it->second.rxBytes * 8.0*payloadSize) / (simulationTime*1000000.0);
		NS_LOG_UNCOND(" Total Throughput: " << throughput << " Mbps");
		totalThroughput += throughput;
		
		NS_LOG_UNCOND("Delay: " << (it->second.delaySum.GetSeconds()/it->second.rxPackets)*1000 << " ms");
		totalDelay += (it->second.delaySum.GetSeconds()/it->second.rxPackets)*1000;
		
		numFlows++;
	}
  
	double averageThroughput = totalThroughput / numStations;
	double averageDelay = totalDelay/numFlows;

	NS_LOG_UNCOND("Average Throughput: " << averageThroughput << " Mbps");
	// Output the Throughput and number of stations to a file
	std::ofstream outFile1("Throughput_vs_stations.txt", std::ios::app);
	outFile1 << numStations << "\t" << averageThroughput << std::endl;

	NS_LOG_UNCOND("Average Delay: " << averageDelay << " ms");

	// Output the delay and number of stations to a file
	std::ofstream outFile2("delay_vs_stations.txt", std::ios::app);
	outFile2 << numStations << "\t" << averageDelay << std::endl;

	outFile1.close();
	outFile2.close();

	// Clean up
	Simulator::Destroy();

	return 0;
}

```

### Output of the simulation code
 <p align="justify">
The simulation code outputs Throughput_vs_stations.txt and delay_vs_stations.txt into the ns3.41 folder and I took this file into matlab to plot the relationship between throughput and delay with number of stations. Below is a screenshot of a sample output. 
 </p>
 
![flow](https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-D11215805-Dawit-Hadush-Hailu/assets/flow.PNG)

## Results
### Number of stations versus Throughput 
For a simulation varying the stations from 1 to 14 and a simulation time of 10 seconds, I obtained the following result.

![Throughputvsstation](https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-D11215805-Dawit-Hadush-Hailu/assets/Throughputvsstation.png)

 <p align="justify">
When the number of stations are increased, the average throughput gets lower, and the output shows how it affects the throughput for the network. When the distance between wifi stations and access point is augmented, the throughput for the wifi-network is more affected. 
 </p>
### Number of stations versus Delay 

Similary, varying the stations  from 1 to 14,a simulation time of 10 seconds, we obtain the following result. 

![DelayVStation](https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-D11215805-Dawit-Hadush-Hailu/assets/DelayVStation.png)

 <p align="justify">
When the number of stations are increased, the average delay gets higher, and the output shows how it affects the delay for the network. When the distance between wifi stations and access point is augmented, the delay for the wifi-network is more affected. 
 </p>
 
# References:
1. ns-3([[1]](https://www.nsnam.org/),[[2]](https://www.nsnam.org/documentation/))
2. [NS-3 Events and Simulator]( https://www.nsnam.org/docs/manual/html/events.html)
3. [NS-3 Wifi Models](https://www.nsnam.org/docs/release/3.8/doxygen/group___wifi.html)
4. [IEEE-802.11ah-ns-3](https://github.com/imec-idlab/IEEE-802.11ah-ns-3/blob/master/src/wifi/doc/source/wifi-design.rst)
5. [NS3-SIMULATION OF WIRELESS NETWORK]( https://www.youtube.com/watch?v=qDseQLXtEKE&t=1457s&ab_channel=Dr.YerriswamyT) 
