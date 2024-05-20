
**Outline**
[TOC]
###### tags: `Multimedia Wireless Network`

# Objective of the Assignment: 
* A QoS supporting protocol extentions of the IEEE 802.11e is implemented.
* A performance evaluation of the IEEE 802.11e with the help of simulation.
* Throughput per AC 
    * Study 1:
        * Throughput per AC (3 streams each) 
        * Offered traffic per AC(3 streams each): ranging from 1 to 30 Mbps
    * Study 2:
        * Throughput per AC 
        * Number of transmitting stations each offering (4x1 Mbps): ranging from 1 to 16

## Simulation environment

    * Ubuntu Version: ubuntu-22.04.LTS-desktop-amd64
    * Disk: 50GB
    * Memory:8GB
    * CPU: Core i7
    * NS-3.41
    
# Part I: Introduction to EDCA (IEEE 802.11e)
<div style="text-align: justify"> 
Enhanced Distributed Channel Access (EDCA), IEEE 802.11e, offers a method for prioritizing various types of traffic according to their quality of service (QoS) needs. The default EDCA settings in ns-3 are the following:
 </div>
 
 
*  Four access categories exist: AC_BK (Background), AC_BE (Best Effort), AC_VI (Video), and AC_VO (Voice).
*  Every AC is equipped with specific contention parameters such as CWmin, CWmax, AIFSN, and TXOP limit.
*  The parameters' default settings are usually determined by the specifications in the IEEE 802.11 standard.

Here are the default EDCA parameters in ns-3:



| AC_BK (Background) |AC_BE (Best Effort)| AC_VI (Video) |AC_VO (Voice) |
| -------------------| ----------------- | ------------- |------------- |
| CWmin: 15          |CWmin: 15          | CWmin: 7      |CWmin: 3      |
| CWmax: 1023        |CWmax: 1023        | CWmax: 15     |CWmax: 7      |
| AIFSN: 7           |AIFSN: 3           | AIFSN: 2      |AIFSN: 2      |
| TXOP limit: 0      |TXOP limit: 0      | TXOP limit:3008 microsec| TXOP limit: 1504 microsec|

These default parameters are defined in the ns-3 source code and can be found in the file src/wifi/model/

Fig. 1 illustrates the parallel backoff entities and the EDCA parameter set defines the priorities in medium access by setting individual interframe spaces, contention windows, and many other parameters per AC.
<p align="center">
  <img src="https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-D11215805-Dawit-Hadush-Hailu/Resources%20for%20Assignment%203/node.PNG" /> 
</p>
Fig. 1: Legacy 802.11 station and 802.11e station with four ACs within one station

The QoS support in EDCA is enabled by the inclusion of access categories (ACs) and several separate backoff entities. MSDUs are sent by parallel backoff entities in a single 802.11e station, where backoff entities are ranked based on AC-specific contention parameters known as EDCA parameter set. Every 802.11e station has four backoff entities because there are four ACs. The ACs are categorized based on their intended use, such as AC_VO for voice, AC_VI for video, AC_BE for best effort, and AC_BK for background.

# Part II: Simulation of IEEE 802.11e MAC + IEEE 802.11a PHY
## Assumptions of the experiment 

* IEEE 802.11e MAC + IEEE 802.11a PHY.
* MSDUs of 512 byte with the negatively-exponentially distributed inter-arrival times.
* 1 station transmits to 3 stations, one stream per AC.
* Default EDCA parameters are used.
* Neither RTS/CTS nor fragmentation is used.
* Beacon frames are not generated.

## Network topology 
The network topology implemented to simulate the IEEE 802.11e Wi-Fi networks  are shown below, consisting of one access point and 3 stations as in Fig 2 (for simulation 1) or N stations as in Fig 3 (for simulation 2).


|Fig 2: Network topology for simulation 1 | Fig 3: Network topology for simulation 2 | 
| -------- | -------- |
| ![topo](https://hackmd.io/_uploads/H1hQt1P7C.png)      |   ![top2](https://hackmd.io/_uploads/S1VVKJw70.png)     | 


## Steps of the Simulation 
1. Set Up the Network: Create a network topology, set the standard, set type and Install the Wi-Fi stack on all nodes using the WifiHelper.
``` c++
	NodeContainer wifiStaNodes;
	wifiStaNodes.Create (nWifi);
	// AP
	NodeContainer wifiApNode;
	wifiApNode.Create (1);
        WifiHelper wifi;
	wifi.SetStandard (WIFI_STANDARD_80211a); // 802.11a standard used
	
         wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode", StringValue("OfdmRate24Mbps"),
                                 "ControlMode", StringValue("OfdmRate6Mbps"),"RtsCtsThreshold",
							 UintegerValue(useRts ? 0 : 999999));
	Ssid ssid = Ssid ("ns3-80211a");

	//Configuration of MAC
	mac.SetType ("ns3::StaWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true));

	NetDeviceContainer staDevices;
	staDevices = wifi.Install (phy, mac, wifiStaNodes);

	mac.SetType ("ns3::ApWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true),
				 "EnableBeaconJitter", BooleanValue (false));

	NetDeviceContainer apDevice;
	apDevice = wifi.Install (phy, mac, wifiApNode);
    
```
2. Configure QoS Parameters:
    * Specify the QoS metrics for various types of traffic classes (Access Categories, ACs) like voice, video, standard service, and background.
    ``` c++
    std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; //AC_BE, AC_BK, AC_VI, AC_VO
    ``` 
    * Establish parameters for contention, such as CWmin, CWmax, AIFSN, and TXOP limit for each Access Category (AC).
    *  Access the Wi-Fi MAC layer attributes directly to set up these parameters eg for Video traffic.

``` c++
                wifi_mac->GetAttribute("VO_Txop", ptr);
                edca = ptr.Get<QosTxop>();
                edca->SetAifsn(2);
                edca->SetMinCw(3);
                edca->SetMaxCw(7);
                edca->SetTxopLimit (MicroSeconds (1504));

                wifi_mac->GetAttribute("BE_Txop", ptr);
                edca = ptr.Get<QosTxop>();
                edca->SetAifsn(3);
                edca->SetMinCw(15);
                edca->SetMaxCw(1023);
                edca->SetTxopLimit (MicroSeconds (0));

                wifi_mac->GetAttribute("VO_Txop", ptr);
                edca = ptr.Get<QosTxop>();
                edca->SetAifsn(2);
                edca->SetMinCw(3);
                edca->SetMaxCw(7);
                edca->SetTxopLimit (MicroSeconds (1504));

                wifi_mac->GetAttribute("VI_Txop", ptr);
                edca = ptr.Get<QosTxop>();
                edca->SetAifsn(2);
                edca->SetMinCw(7);
                edca->SetMaxCw(15);
                edca->SetTxopLimit (MicroSeconds (3008));
``` 
3. Implement Traffic Differentiation

    * Describe various types of traffic with different quality of service needs (such as giving voice traffic high importance and treating best effort traffic as less critical).
    * Allocate the correct Quality of Service categories to each data stream depending on their specific Quality of Service needs.
    ``` c++
        std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; //AC_BE, AC_BK, AC_VI, AC_VO
        uint32_t portNumber = 5000;
        	for(uint32_t  i=0; i<nWifi;i++){

			for (uint8_t tosValue : tosValues){
                        InetSocketAddress sinkSocket (sinkaddressDL.GetAddress(i), portNumber++);
                        sinkSocket.SetTos (tosValue);
                    }
                }
     ``` 
4. Use QoS-Aware Applications:

    * Utilize applications that are aware of Quality of Service (QoS) and are capable of producing traffic with varying QoS needs.
    * One way to create UDP traffic with tailored QoS settings for voice, video, etc is by using OnOffHelper.
     ``` c++
     // Setting applications
	ApplicationContainer sourceApplications, sinkApplications;
	std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; //AC_BE, AC_BK, AC_VI, AC_VO
	uint32_t portNumber = 5000;
	
	// Generate traffic with  negatively exponential interarrival times
	
		for(uint32_t  i=0; i<nWifi;i++){

			for (uint8_t tosValue : tosValues){

				InetSocketAddress sinkSocket (sinkaddressDL.GetAddress(i), portNumber++);
				sinkSocket.SetTos (tosValue);
				OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
				onOffHelper.SetAttribute("OnTime", StringValue("ns3::ExponentialRandomVariable[Mean=" + std::to_string(meanInterArrivalTime) + "]"));
				onOffHelper.SetAttribute("OffTime", StringValue("ns3::ExponentialRandomVariable[Mean=" + std::to_string(meanInterArrivalTime) + "]"));
				onOffHelper.SetAttribute ("DataRate", DataRateValue (offeredrate));
				onOffHelper.SetAttribute ("PacketSize", UintegerValue (payloadSize)); //bytes
				sourceApplications.Add(onOffHelper.Install (wifiApNode.Get (0)));
				PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
				sinkApplications.Add (packetSinkHelper.Install (wifiStaNodes));
			}
		}

     ``` 
5. Run the Simulation:
    * Set up the length of the simulation and initiate the simulation with Simulator::Run().
   
     ``` c++
            Simulator::Run ();
     ``` 
     
6. Collect the throughput stastics from flow monitor
     * Assess simulation data to measure how well the IEEE 802.11e network is performing based on quality of service factors like latency, throughput, and packet loss.
     
    ``` c++
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());

	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter =
			stats.begin(); iter != stats.end(); ++iter)
	{
		// some metrics calculation
		statDurationRX = iter->second.timeLastRxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();
		statDurationTX = iter->second.timeLastTxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();

    collect the throughput per AC of each traffic
    }
    }
     ``` 
7. Customize QoS Mechanisms (Optional):
    * Customize QoS mechanisms like EDCA parameters and traffic type management according to your particular needs.
    * Adjust the QoS parameters' default settings to align with the intended performance of your IEEE 802.11e network.
    
## Simulation code

This is a simulation code that implements the paper [1] by configuring and extending IEEE 802.11a PHY standard to support QoS (IEEE 802.11e MAC) in Wi-Fi networks. It outputs the Throughput per AC as a function of offered traffic per AC.The second simulation code also outputs the Throughput per AC as a function of increasing number of stations. 

### Souce code for Throughput per AC Vs offered traffic (simulation 1)

 ``` c++
       #include <ns3/config-store-module.h>
#include "ns3/core-module.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/qos-txop.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-probe.h"
#include "ns3/wifi-mac.h"
#include <chrono>

using namespace std::chrono;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("IEEE 802.11e EDCA");

int main (int argc, char *argv[])
{
	uint32_t nWifi =3;
	uint32_t simulationTime =10.0;
	uint32_t offeredrate =10000000;
	bool useRts = false;
	uint32_t payloadSize = 512; // MSDU payload size
	double meanInterArrivalTime=2.0;

	//creating nodes
	NodeContainer wifiStaNodes;
	wifiStaNodes.Create (nWifi);
	// AP
	NodeContainer wifiApNode;
	wifiApNode.Create (1);

	// creating channels 
	
	YansWifiChannelHelper channel;
	channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel", "ReferenceLoss", DoubleValue (40.089)); 
	YansWifiPhyHelper phy;

	phy.SetChannel (channel.Create ());
	
	phy.Set("ChannelSettings", StringValue("{36, 0, BAND_5GHZ, 0}"));
	// Set transmission power levels
	phy.Set ("TxPowerStart", DoubleValue (23.0)); // Set transmission power in dBm
	phy.Set ("TxPowerEnd", DoubleValue (23.0));   // Set transmission power in dBm

	WifiMacHelper mac;
	// Creating a helper for Wi-Fi configuration
	WifiHelper wifi;
	wifi.SetStandard (WIFI_STANDARD_80211a); // 802.11a standard used
	
	wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode", StringValue("OfdmRate24Mbps"),
                                 "ControlMode", StringValue("OfdmRate6Mbps"),"RtsCtsThreshold",
							 UintegerValue(useRts ? 0 : 999999));
	Ssid ssid = Ssid ("ns3-80211a");

	//Configuration of MAC
	mac.SetType ("ns3::StaWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true));

	NetDeviceContainer staDevices;
	staDevices = wifi.Install (phy, mac, wifiStaNodes);

	mac.SetType ("ns3::ApWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true),
				 "EnableBeaconJitter", BooleanValue (false));

	NetDeviceContainer apDevice;
	apDevice = wifi.Install (phy, mac, wifiApNode);
	
	 // Modify EDCA configuration (TXOP limit) for each AC
        Ptr<NetDevice> dev = wifiApNode.Get(0)->GetDevice(0);
        Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
        Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
        PointerValue ptr;
        Ptr<QosTxop> edca;
		
		wifi_mac->GetAttribute("BK_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(7);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);
		edca->SetTxopLimit (MicroSeconds (0));
		
        wifi_mac->GetAttribute("BE_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);
		edca->SetTxopLimit (MicroSeconds (0));

        wifi_mac->GetAttribute("VO_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(3);
        edca->SetMaxCw(7);
		edca->SetTxopLimit (MicroSeconds (1504));

        wifi_mac->GetAttribute("VI_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(7);
        edca->SetMaxCw(15);
		edca->SetTxopLimit (MicroSeconds (3008));
	
	// mobility
	MobilityHelper mobilityAP;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
	positionAlloc->Add (Vector (0.0, 0.0, 0.0));
	mobilityAP.SetPositionAllocator (positionAlloc);
	mobilityAP.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobilityAP.Install (wifiApNode);

	MobilityHelper mobilitySTA;
	mobilitySTA.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobilitySTA.SetPositionAllocator("ns3::RandomRectanglePositionAllocator",
	"X", StringValue("ns3::UniformRandomVariable[Min=-10.0|Max=10.0] "),
	"Y", StringValue("ns3::UniformRandomVariable[Min=-10.0|Max=10.0]"));
	mobilitySTA.Install (wifiStaNodes);

	// Internet stack
	InternetStackHelper stack;
	stack.Install (wifiApNode);
	stack.Install (wifiStaNodes);
	Ipv4AddressHelper address;

	address.SetBase ("192.168.1.0", "255.255.255.0");
	Ipv4InterfaceContainer staNodeInterfaces, apNodeInterface;

	apNodeInterface = address.Assign (apDevice);
	staNodeInterfaces = address.Assign (staDevices);
	
	Ipv4InterfaceContainer sinkaddressDL;
	
	Ipv4InterfaceContainer::Iterator iteripv4;
	int k=0;
	for (iteripv4=staNodeInterfaces.Begin(); iteripv4 < staNodeInterfaces.End(); iteripv4++){
		sinkaddressDL.Add(staNodeInterfaces.Get(k));
		k++;
	}

	// Setting applications
	ApplicationContainer sourceApplications, sinkApplications;
	std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; //AC_BE, AC_BK, AC_VI, AC_VO
	uint32_t portNumber = 5000;
	
	// Generate traffic with  negatively exponential interarrival times
	
		for(uint32_t  i=0; i<nWifi;i++){

			for (uint8_t tosValue : tosValues){

				InetSocketAddress sinkSocket (sinkaddressDL.GetAddress(i), portNumber++);
				sinkSocket.SetTos (tosValue);
				OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
				onOffHelper.SetAttribute("OnTime", StringValue("ns3::ExponentialRandomVariable[Mean=" + std::to_string(meanInterArrivalTime) + "]"));
				onOffHelper.SetAttribute("OffTime", StringValue("ns3::ExponentialRandomVariable[Mean=" + std::to_string(meanInterArrivalTime) + "]"));
				onOffHelper.SetAttribute ("DataRate", DataRateValue (offeredrate));
				onOffHelper.SetAttribute ("PacketSize", UintegerValue (payloadSize)); //bytes
				sourceApplications.Add(onOffHelper.Install (wifiApNode.Get (0)));
				PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
				sinkApplications.Add (packetSinkHelper.Install (wifiStaNodes));
			}
		}

	sinkApplications.Start (Seconds (0.0));
	sinkApplications.Stop (Seconds (simulationTime + 1));
	sourceApplications.Start (Seconds (1.0));
	sourceApplications.Stop (Seconds (simulationTime + 1));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	//////////////////// Flow Monitor /////////////////////////
	AsciiTraceHelper asciiTraceHelper;
	Ptr<FlowMonitor> flowMonitor;
	FlowMonitorHelper flowHelper;
	flowMonitor = flowHelper.InstallAll();

	Simulator::Stop (Seconds (simulationTime + 2));
	Simulator::Run ();

	////////Flow Monitor data///////////////////////////////
	static bool verbose = true;

	std::string BE_results, BK_results, VO_results, VI_results;
	std::string Throughput_per_AC_BE, Throughput_per_AC_BK, Throughput_per_AC_VI, Throughput_per_AC_VO;
	
	BE_results = "Flows_Results_BE_edca.txt";
	BK_results = "Flows_Results_BK_edca.txt";
	VI_results = "Flows_Results_VI_edca.txt";
	VO_results = "Flows_Results_VO_edca.txt";
	
	Ptr<OutputStreamWrapper> BEstreamMetricsInit = asciiTraceHelper.CreateFileStream((BE_results));
	*BEstreamMetricsInit->GetStream()
            		  << "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;
					  
	Ptr<OutputStreamWrapper> BKstreamMetricsInit = asciiTraceHelper.CreateFileStream((BK_results));
	*BKstreamMetricsInit->GetStream()
            		<< "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;
	Ptr<OutputStreamWrapper> VIstreamMetricsInit = asciiTraceHelper.CreateFileStream((VI_results));
	*VIstreamMetricsInit->GetStream()
            	<< "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;

	Ptr<OutputStreamWrapper> VOstreamMetricsInit = asciiTraceHelper.CreateFileStream((VO_results));
	*VOstreamMetricsInit->GetStream()
            		<< "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;
		
	std::ofstream outFile1("Throughput_per_AC_BE.txt", std::ios::app);
	std::ofstream outFile2("Throughput_per_AC_BK.txt", std::ios::app);
	std::ofstream outFile3("Throughput_per_AC_VI.txt", std::ios::app);
	std::ofstream outFile4("Throughput_per_AC_VO.txt", std::ios::app);

	double statDurationTX = 0.0;
	double statDurationRX = 0.0;
	double totalThroughput1 = 0.0;
	double totalThroughput2= 0.0;
	double totalThroughput3= 0.0;
	double totalThroughput4= 0.0;
	
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());

	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter =
			stats.begin(); iter != stats.end(); ++iter)
	{
		// some metrics calculation
		statDurationRX = iter->second.timeLastRxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();
		statDurationTX = iter->second.timeLastTxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();
		double txTput, rxTput;
		if (statDurationTX > 0)
		{
			txTput = iter->second.txBytes * 8.0 / statDurationTX / 1000000;
		}
		else
		{
			txTput = 0;
		}
		if (statDurationRX > 0)
		{
			rxTput = iter->second.rxBytes * 8.0 / statDurationRX / 1000000;
		}
		else
		{
			rxTput = 0;
		}
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
		//
		if (verbose)
		{
			NS_LOG_UNCOND( "Flow ID: " << iter->first << ", Source Port: "
					<< t.sourcePort << ", Destination Port: " << t.destinationPort
					<< " (" << t.sourceAddress << " -> " << t.destinationAddress
					<< ")");
			NS_LOG_UNCOND( "TxOffered = " << txTput << " Mbps");
			NS_LOG_UNCOND( "T-put = " << rxTput << " Mbps");
		}
		Ptr<OutputStreamWrapper> streamMetricsInit = NULL;
		for (uint32_t k = 5000; k < (5000+nWifi*4); k = k + 4){
			if ((t.destinationPort == k))
			{
				streamMetricsInit = BEstreamMetricsInit;
				totalThroughput1 += rxTput;
			}
			// Get file pointer for UL, if UL flow (using port and IP address to assure correct result))
			else if ((t.destinationPort == k+1))
			{
				streamMetricsInit = BKstreamMetricsInit;
				totalThroughput2 += rxTput;
			}
			else if ((t.destinationPort == k+2))
			{
				streamMetricsInit = VIstreamMetricsInit;
				totalThroughput3 += rxTput;
			}
			else if ((t.destinationPort == k+3))
			{
				streamMetricsInit = VOstreamMetricsInit;
				totalThroughput4 += rxTput;
			}
		}
		if (streamMetricsInit)
		{
			*streamMetricsInit->GetStream() << (iter->first ) << ", " << txTput << ", " 	<< rxTput << ", " << std::endl;
		}
		else
		{
			//TODO: chance for an ASSERT
			if ( true )
			{
				std::cout << "Some problem to save metrics" << std::endl;
				std::cout << "Flow ID: " << iter->first << ", Source Port: "
						<< t.sourcePort << ", Destination Port: " << t.destinationPort
						<< " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
				std::cout << "AP Address: " << apNodeInterface.GetAddress (0) << std::endl;
			}
		}
	}
	double Throughput_per_AC_1 = totalThroughput1 / nWifi;
	double Throughput_per_AC_2 = totalThroughput2 / nWifi;
	double Throughput_per_AC_3 = totalThroughput3 / nWifi;
	double Throughput_per_AC_4 = totalThroughput4 / nWifi;
	
	NS_LOG_UNCOND("Throughput per AC_BE: " << Throughput_per_AC_1 << " Mbps");
	NS_LOG_UNCOND("Throughput per AC_BK: " << Throughput_per_AC_2 << " Mbps");
	NS_LOG_UNCOND("Throughput per AC_VI: " << Throughput_per_AC_3 << " Mbps");
	NS_LOG_UNCOND("Throughput per AC_VO: " << Throughput_per_AC_4 << " Mbps");
	
	outFile1 << offeredrate << "\t" << Throughput_per_AC_1<< std::endl;
	outFile2 << offeredrate << "\t" << Throughput_per_AC_2<< std::endl;
	outFile3 << offeredrate << "\t" << Throughput_per_AC_3<< std::endl;
	outFile4 << offeredrate << "\t" << Throughput_per_AC_4<< std::endl;

	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();

	Simulator::Destroy ();

	return 0;
}

```

### Souce code for Throughput per AC Vs increasing number of stations (simulation 2)

 ``` c++
       #include <ns3/config-store-module.h>
#include "ns3/core-module.h"
#include "ns3/command-line.h"
#include "ns3/config.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/double.h"
#include "ns3/string.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include "ns3/qos-txop.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"
#include "ns3/mobility-helper.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/packet-sink-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/packet-sink.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/flow-probe.h"
#include "ns3/wifi-mac.h"
#include <chrono>

using namespace std::chrono;
using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Stations_IEEE 802.11e_EDCA");

int main (int argc, char *argv[])
{
	uint32_t nWifi =5;
	uint32_t simulationTime =10.0;
	uint32_t offeredrate =4000000;
	bool useRts = false;
	uint32_t payloadSize = 512; // MSDU payload size
	double meanInterArrivalTime=1.0;

	//creating nodes
	NodeContainer wifiStaNodes;
	wifiStaNodes.Create (nWifi);
	// AP
	NodeContainer wifiApNode;
	wifiApNode.Create (1);

	// creating channels 
	
	YansWifiChannelHelper channel;
	channel.SetPropagationDelay("ns3::ConstantSpeedPropagationDelayModel");
	channel.AddPropagationLoss("ns3::LogDistancePropagationLossModel", "ReferenceLoss", DoubleValue (40.089)); 
	YansWifiPhyHelper phy;

	phy.SetChannel (channel.Create ());
	
	phy.Set("ChannelSettings", StringValue("{36, 0, BAND_5GHZ, 0}"));
	// Set transmission power levels
	phy.Set ("TxPowerStart", DoubleValue (23.0)); // Set transmission power in dBm
	phy.Set ("TxPowerEnd", DoubleValue (23.0));   // Set transmission power in dBm

	WifiMacHelper mac;
	// Creating a helper for Wi-Fi configuration
	WifiHelper wifi;
	wifi.SetStandard (WIFI_STANDARD_80211a); // 802.11a standard used
	
	wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                                 "DataMode", StringValue("OfdmRate54Mbps"),
                                 "ControlMode", StringValue("OfdmRate6Mbps"),"RtsCtsThreshold",
							 UintegerValue(useRts ? 0 : 999999));
	Ssid ssid = Ssid ("ns3-80211a");

	//Configuration of MAC
	mac.SetType ("ns3::StaWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true));

	NetDeviceContainer staDevices;
	staDevices = wifi.Install (phy, mac, wifiStaNodes);

	mac.SetType ("ns3::ApWifiMac",
			     "Ssid", SsidValue (ssid),
			     "QosSupported", BooleanValue (true),
				 "EnableBeaconJitter", BooleanValue (false));

	NetDeviceContainer apDevice;
	apDevice = wifi.Install (phy, mac, wifiApNode);
	
	 // Modify EDCA configuration (TXOP limit) for each AC
        Ptr<NetDevice> dev = wifiApNode.Get(0)->GetDevice(0);
        Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
        Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
        PointerValue ptr;
        Ptr<QosTxop> edca;
		
		wifi_mac->GetAttribute("BK_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(7);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);
		edca->SetTxopLimit (MicroSeconds (0));
		
        wifi_mac->GetAttribute("BE_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(15);
        edca->SetMaxCw(1023);
		edca->SetTxopLimit (MicroSeconds (0));

        wifi_mac->GetAttribute("VO_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(2);
        edca->SetMinCw(3);
        edca->SetMaxCw(7);
		edca->SetTxopLimit (MicroSeconds (1504));

        wifi_mac->GetAttribute("VI_Txop", ptr);
        edca = ptr.Get<QosTxop>();
        edca->SetAifsn(3);
        edca->SetMinCw(7);
        edca->SetMaxCw(15);
		edca->SetTxopLimit (MicroSeconds (3008));
	
	// mobility
	MobilityHelper mobilityAP;
	Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator> ();
	positionAlloc->Add (Vector (0.0, 0.0, 0.0));
	mobilityAP.SetPositionAllocator (positionAlloc);
	mobilityAP.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobilityAP.Install (wifiApNode);

	MobilityHelper mobilitySTA;
	mobilitySTA.SetMobilityModel("ns3::ConstantPositionMobilityModel");
	mobilitySTA.SetPositionAllocator("ns3::RandomRectanglePositionAllocator",
	"X", StringValue("ns3::UniformRandomVariable[Min=-10.0|Max=10.0] "),
	"Y", StringValue("ns3::UniformRandomVariable[Min=-10.0|Max=10.0]"));
	mobilitySTA.Install (wifiStaNodes);

	// Internet stack
	InternetStackHelper stack;
	stack.Install (wifiApNode);
	stack.Install (wifiStaNodes);
	Ipv4AddressHelper address;

	address.SetBase ("192.168.1.0", "255.255.255.0");
	Ipv4InterfaceContainer staNodeInterfaces, apNodeInterface;

	apNodeInterface = address.Assign (apDevice);
	staNodeInterfaces = address.Assign (staDevices);
	
	Ipv4InterfaceContainer sinkaddressDL;
	
	Ipv4InterfaceContainer::Iterator iteripv4;
	int k=0;
	for (iteripv4=staNodeInterfaces.Begin(); iteripv4 < staNodeInterfaces.End(); iteripv4++){
		sinkaddressDL.Add(staNodeInterfaces.Get(k));
		k++;
	}

	// Setting applications
	ApplicationContainer sourceApplications, sinkApplications;
	std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; //AC_BE, AC_BK, AC_VI, AC_VO
	uint32_t portNumber = 5000;
	
	// Generate traffic with  negatively exponential interarrival times
	
		for(uint32_t  i=0; i<nWifi;i++){

			for (uint8_t tosValue : tosValues){

				InetSocketAddress sinkSocket (sinkaddressDL.GetAddress(i), portNumber++);
				sinkSocket.SetTos (tosValue);
				OnOffHelper onOffHelper ("ns3::UdpSocketFactory", sinkSocket);
				onOffHelper.SetAttribute("OnTime", StringValue("ns3::ExponentialRandomVariable[Mean=" + std::to_string(meanInterArrivalTime) + "]"));
				onOffHelper.SetAttribute("OffTime", StringValue("ns3::ExponentialRandomVariable[Mean=" + std::to_string(meanInterArrivalTime) + "]"));
				onOffHelper.SetAttribute ("DataRate", DataRateValue (offeredrate));
				onOffHelper.SetAttribute ("PacketSize", UintegerValue (payloadSize)); //bytes
				sourceApplications.Add(onOffHelper.Install (wifiApNode.Get (0)));
				PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", sinkSocket);
				sinkApplications.Add (packetSinkHelper.Install (wifiStaNodes));
			}
		}

	sinkApplications.Start (Seconds (0.0));
	sinkApplications.Stop (Seconds (simulationTime + 1));
	sourceApplications.Start (Seconds (1.0));
	sourceApplications.Stop (Seconds (simulationTime + 1));

	Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

	//////////////////// Flow Monitor /////////////////////////
	AsciiTraceHelper asciiTraceHelper;
	Ptr<FlowMonitor> flowMonitor;
	FlowMonitorHelper flowHelper;
	flowMonitor = flowHelper.InstallAll();

	Simulator::Stop (Seconds (simulationTime + 2));
	Simulator::Run ();

	////////Flow Monitor data///////////////////////////////
	static bool verbose = true;

	std::string BE_results, BK_results, VO_results, VI_results;
	std::string Station_Throughput_per_AC_BE, Station_Throughput_per_AC_BK, Station_Throughput_per_AC_VI, Station_Throughput_per_AC_VO;
	
	BE_results = "Station_Flows_Results_BE_edca.txt";
	BK_results = "Station_Flows_Results_BK_edca.txt";
	VI_results = "Station_Flows_Results_VI_edca.txt";
	VO_results = "Station_Flows_Results_VO_edca.txt";
	
	Ptr<OutputStreamWrapper> BEstreamMetricsInit = asciiTraceHelper.CreateFileStream((BE_results));
	*BEstreamMetricsInit->GetStream()
            		  << "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;
					  
	Ptr<OutputStreamWrapper> BKstreamMetricsInit = asciiTraceHelper.CreateFileStream((BK_results));
	*BKstreamMetricsInit->GetStream()
            		<< "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;
	Ptr<OutputStreamWrapper> VIstreamMetricsInit = asciiTraceHelper.CreateFileStream((VI_results));
	*VIstreamMetricsInit->GetStream()
            	<< "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;

	Ptr<OutputStreamWrapper> VOstreamMetricsInit = asciiTraceHelper.CreateFileStream((VO_results));
	*VOstreamMetricsInit->GetStream()
            		<< "Flow_ID, TxOffered(Mbps), T_put(Mbps)" << std::endl;
		
	std::ofstream outFile1("Station_Throughput_per_AC_BE.txt", std::ios::app);
	std::ofstream outFile2("Station_Throughput_per_AC_BK.txt", std::ios::app);
	std::ofstream outFile3("Station_Throughput_per_AC_VI.txt", std::ios::app);
	std::ofstream outFile4("Station_Throughput_per_AC_VO.txt", std::ios::app);

	double statDurationTX = 0.0;
	double statDurationRX = 0.0;
	double totalThroughput1 = 0.0;
	double totalThroughput2= 0.0;
	double totalThroughput3= 0.0;
	double totalThroughput4= 0.0;
	
	Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowHelper.GetClassifier());

	std::map<FlowId, FlowMonitor::FlowStats> stats = flowMonitor->GetFlowStats();
	for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter =
			stats.begin(); iter != stats.end(); ++iter)
	{
		// some metrics calculation
		statDurationRX = iter->second.timeLastRxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();
		statDurationTX = iter->second.timeLastTxPacket.GetSeconds()
                    		- iter->second.timeFirstTxPacket.GetSeconds();
		double txTput, rxTput;
		if (statDurationTX > 0)
		{
			txTput = iter->second.txBytes * 8.0 / statDurationTX / 1000000;
		}
		else
		{
			txTput = 0;
		}
		if (statDurationRX > 0)
		{
			rxTput = iter->second.rxBytes * 8.0 / statDurationRX / 1000000;
		}
		else
		{
			rxTput = 0;
		}
		Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(iter->first);
		//
		if (verbose)
		{
			NS_LOG_UNCOND( "Flow ID: " << iter->first << ", Source Port: "
					<< t.sourcePort << ", Destination Port: " << t.destinationPort
					<< " (" << t.sourceAddress << " -> " << t.destinationAddress
					<< ")");
			NS_LOG_UNCOND( "TxOffered = " << txTput << " Mbps");
			NS_LOG_UNCOND( "T-put = " << rxTput << " Mbps");
		}
		Ptr<OutputStreamWrapper> streamMetricsInit = NULL;
		for (uint32_t k = 5000; k < (5000+nWifi*4); k = k + 4){
			if ((t.destinationPort == k))
			{
				streamMetricsInit = BEstreamMetricsInit;
				totalThroughput1 += rxTput;
			}
			// Get file pointer for UL, if UL flow (using port and IP address to assure correct result))
			else if ((t.destinationPort == k+1))
			{
				streamMetricsInit = BKstreamMetricsInit;
				totalThroughput2 += rxTput;
			}
			else if ((t.destinationPort == k+2))
			{
				streamMetricsInit = VIstreamMetricsInit;
				totalThroughput3 += rxTput;
			}
			else if ((t.destinationPort == k+3))
			{
				streamMetricsInit = VOstreamMetricsInit;
				totalThroughput4 += rxTput;
			}
		}
		if (streamMetricsInit)
		{
			*streamMetricsInit->GetStream() << (iter->first ) << ", " << txTput << ", " 	<< rxTput << ", " << std::endl;
		}
		else
		{
			//TODO: chance for an ASSERT
			if ( true )
			{
				std::cout << "Some problem to save metrics" << std::endl;
				std::cout << "Flow ID: " << iter->first << ", Source Port: "
						<< t.sourcePort << ", Destination Port: " << t.destinationPort
						<< " (" << t.sourceAddress << " -> " << t.destinationAddress << ")" << std::endl;
				std::cout << "AP Address: " << apNodeInterface.GetAddress (0) << std::endl;
			}
		}
	}
	double Throughput_per_AC_1 = totalThroughput1 / nWifi;
	double Throughput_per_AC_2 = totalThroughput2 / nWifi;
	double Throughput_per_AC_3 = totalThroughput3 / nWifi;
	double Throughput_per_AC_4 = totalThroughput4 / nWifi;
	
	NS_LOG_UNCOND("Throughput per AC_BE: " << Throughput_per_AC_1 << " Mbps");
	NS_LOG_UNCOND("Throughput per AC_BK: " << Throughput_per_AC_2 << " Mbps");
	NS_LOG_UNCOND("Throughput per AC_VI: " << Throughput_per_AC_3 << " Mbps");
	NS_LOG_UNCOND("Throughput per AC_VO: " << Throughput_per_AC_4 << " Mbps");
	
	outFile1 << nWifi << "\t" << Throughput_per_AC_1<< std::endl;
	outFile2 << nWifi << "\t" << Throughput_per_AC_2<< std::endl;
	outFile3 << nWifi << "\t" << Throughput_per_AC_3<< std::endl;
	outFile4 << nWifi << "\t" << Throughput_per_AC_4<< std::endl;

	outFile1.close();
	outFile2.close();
	outFile3.close();
	outFile4.close();

	Simulator::Destroy ();

	return 0;
}

```

## Simulation output

The simulation output comes from a simulation where one station (AP) sends four streams to three other stations, one stream for each priority level. Therefore, the AP sends a total of 12 streams. 

The simulation code output contains files for throughput and result for each flow of access catagories. 


Generated files for throughput per AC:

|In Simulation code 1       | In Simulation code 2               | 
| ------------------------| ------------------------------  | 
| Throughput_per_AC_BE.txt|Station_Throughput_per_AC_BE.txt | 
| Throughput_per_AC_BK.txt|Station_Throughput_per_AC_BK.txt | 
| Throughput_per_AC_VI.txt|Station_Throughput_per_AC_VI.txt | 
| Throughput_per_AC_VO.txt|Station_Throughput_per_AC_VO.txt | 

Generated files for each stream  flows: 

| Simulation code 1       | Simulation code 2                | 
| ------------------------| -------------------------------  | 
|Flows_Results_BE_edca.txt|Station_Flows_Results_BE_edca.txt | 
|Flows_Results_BK_edca.txt|Station_Flows_Results_BK_edca.txt | 
|Flows_Results_VI_edca.txt|Station_Flows_Results_VI_edca.txt | 
|Flows_Results_VO_edca.txt|Station_Flows_Results_VO_edca.txt | 

Sample output of each flow of the steam is shown below:

For Voice:

| Flow_ID | TxOffered(Mbps) | T_put(Mbps |
| --------| --------------- | ---------- |
| 2       |  6.68072        | 5.03958    |
| 8       |  4.76601        | 3.28594    |
| 9       |  7.69683        | 5.56178    |

For video: 

| Flow_ID | TxOffered(Mbps) | T_put(Mbps |
| --------| --------------- | ---------- |
| 6       | 9.71759         | 2.17772    |
| 11      | 10.5491         | 0.763069   |
| 12      | 10.5494         | 0.78951   |

For  best effort traffic: 

| Flow_ID | TxOffered(Mbps) | T_put(Mbps |
| --------| --------------- | ---------- |
| 1      |  6.40435        | 0.434658  |
| 4       |  8.0631        | 0.0722995   |
| 7       |  4.8972       | 0.016283   |

For background traffic:

| Flow_ID | TxOffered(Mbps) | T_put(Mbps |
| --------| --------------- | ---------- |
| 3      |  4.58235        | 0   |
| 5       |  5.85374        | 0    |
| 10       |  6.36072       | 0    |


The obtained 12 streams for simulation 1 is shown below: 

#### 12 streams for simulation 1

![s1](https://hackmd.io/_uploads/HkQ4k4DQ0.png)
![s2](https://hackmd.io/_uploads/ryh41EDXA.png)


# Part III: Results and Discussions

The results obtained in this experiment, achievable (saturation) throughput, depends on a several number of parameters. They are valid only for the MAC and PHY settings used in this simulation. 

## Comprasion of the results with the paper

### Throughput-Offered traffic per AC

Fig. 4/5 shows the measured throughput per AC. It can be seen that as the amount of offered traffic increases, ACs with higher priority limit the throughput of ACs with lower priority due to their smaller AIFSN, CWmin, and CWmax values.

![sm1](https://hackmd.io/_uploads/HJ2AuLwXA.png)
 
Fig 4: Throughput per AC with increasing offered traffic per AC for the illustrated scenario(My simulation) 

  ![res1](https://hackmd.io/_uploads/HyKzRZvX0.png)
 Fig 5: Throughput per AC with increasing offered traffic per AC for the illustrated scenario (Reference paper)

### Throughput-Number of station per AC

In order to study how well 802.11e performs in areas with high demand, where there are many devices connected to an access point, we raise the number of competing stations to 16, as shown in Figure 6 and 7. Every station provides equal traffic. Figure 6/7 shows the throughput measured per AC, is similar with the previous figure. It is noticeable that when the number of stations increases, the throughput decreases significantly with the default values of the EDCA parameter set. This is due to the increased chances of collisions, especially in AC_VO, when CWmin and CWmax are low values.

 ![sm2](https://hackmd.io/_uploads/rkwuYUD7R.png) 


Fig 6:Throughput per AC with increasing number of stations, and constant offered traffic per station (my simulation)

 ![res2](https://hackmd.io/_uploads/H1RFtLP7C.png)
 
Fig 7: Throughput per AC with increasing number of stations, and constant offered traffic per station (Reference paper)

# References:
1. [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://ieeexplore.ieee.org/document/1265851)
2. ns-3([[1]](https://www.nsnam.org/),[[2]](https://www.nsnam.org/documentation/))
3. [NS-3 Events and Simulator]( https://www.nsnam.org/docs/manual/html/events.html)
4. [NS-3 Wifi Models](https://www.nsnam.org/docs/release/3.8/doxygen/group___wifi.html)
5. [IEEE-802.11ah-ns-3](https://github.com/imec-idlab/IEEE-802.11ah-ns-3/blob/master/src/wifi/doc/source/wifi-design.rst)
6. [NS3-Simulation of wireless network]( https://www.youtube.com/watch?v=qDseQLXtEKE&t=1457s&ab_channel=Dr.YerriswamyT) 
