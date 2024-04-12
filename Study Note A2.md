# Study Note: Simulation of Wireless Networks (802.11) in NS3 

 Desalegn Aweke Wako
 - [1. Introduction](#1-introduction)
 - [2. Configuration Parameters](#2-configuration-parameters)
 - [3. Simulation Execution](#3-simulation-execution)
 - [4. Simulation Analysis](#4-simulation-analysis)
 - [5. Conclusions](#5-conclusions)
 - [References](#references)



## 1. Introduction
- NS-3 simulator is organized into modules, with each module representing a specific aspect of networking (e.g., core, internet, mobility, WiFi).
- Users can build simulations by combining modules and configuring their parameters using C++ or Python scripts.
- In this simulation, we create a WiFi network with 1 AP and 2 stations, set up the channel and mobility models, assign IP addresses to the nodes, and simulate UDP traffic from the stations to the AP. It also collects and prints the statistics of the simulation, such as the number of transmitted and received bytes, the throughput and delay.
- To simulate the stated wifi network, we have installed the the following  on windows OS. 
		*	virtual machine to install the simultor
		*	ubuntu 20.04
		*	NS-3.38*
- Topology of the wifi Network
        **Node A (STA 1) ---- Node B(AP)----- Node C (STA 2)**
    -- Node B (AP) is at position (0,0,0)
	-- Node A (STA 1) is at position (-5,0,0)
   -- Node C (STA 2) is at position (5,0,0)

## 2. Configuration Parameters
 -  **Setup Parameters**
     -  **Transmission power** of the Wi-Fi devices in dBm (decibels relative to 1 milliwatt). A higher value means a stronger signal, which can increase the range and reliability of wireless communication.
     -   Clear Channel Assessment Energy Detect **(CCA-ED) threshold** is used in wireless communication to detect any type of Radio Frequency (RF) transmissions during the clear channel assessment (CCA). If the energy of the channel exceeds this threshold, the channel is considered busy.
     -   Errormodel: in our simulation,  error based model usd.
    ```python=1
    bool tracing = false;
    double txPower = 20;  // transmission power in dBm
    double ccaEdThreshold = -99;  // CCA-ED threshold in dBm
    bool errorModel = true;  // set to false for an error-free channel
    ```

 - Command line argument Parsing
     - The program parses command line arguments to enable packet capture **(pcap) tracing**. It determines whether or not to enable pcap tracing. If tracing is set to true, pcap tracing will be enabled, which means packet-level details will be logged for further analysis.
    ```python=1
    CommandLine cmd;
    cmd.AddValue("tracing", "Enable pcap tracing", tracing);
    cmd.Parse(argc, argv);
    ```

 - Node configuration
     - **NodeContainer** objects are used to hold the nodes in the network. Two containers are created: one for **WiFi stations** (wifiStaNodes) and one for the **access poin**t (wifiApNode).
     ```python=1
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(2);
    NodeContainer wifiApNode;
    wifiApNode.Create(1);
    ```

 - Wifi Configuration
     - **YansWifiChannelHelpe**r and **YansWifiPhyHelper** are used to configure the **WiFi channel** and **physical layer**.
     - **WifiHelper** is used to configure WiFi parameters like remote station manager.
     - **WifiMacHelper** is used to configure MAC layer parameters, such as SSID (Service Set Identifier) for both **stations** and **access point**.
     - WiFi devices are installed for both stations and access point using **wifi.Install()**.
    ```python=1
      YansWifiChannelHelper channel YansWifiChannelHelper::Default();
     WifiHelper wifi;
     WifiMacHelper mac;
    NetDeviceContainer staDevices = wifi.Install(phy, mac, wifiStaNodes);
    ```

  - Mobility Configuration
     - **MobilityHelper** is used to configure the mobility of nodes.
     - The position allocator is used to set the positions of nodes.
     - Constant position mobility model is used, and mobility is installed for both stations and the access point.
    ```python=1
    MobilityHelper mobility;
      mobility.SetMobilityModel("ns3::ConstantVelocityMobilityModel");
    mobility.Install(wifiApNode);
    mobility.Install(wifiStaNodes);
    ```

 - Internet Stack Configuration
     - **InternetStackHelper** is used to install the internet stack on nodes.
      ```python=1
    InternetStackHelper stack;
    stack.Install(wifiApNode);
    stack.Install(wifiStaNodes);
      ```
 - IP address Assignment
     - IP addresses are assigned to devices using Ipv4AddressHelper.
    ```python=1
   Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer staNodeInterfaces;
    staNodeInterfaces = address.Assign(staDevices);
    Ipv4InterfaceContainer apNodeInterface;
    apNodeInterface = address.Assign(apDevices);
    ```
 - Application Configuration
     - UDP echo server and client applications are configured. 
     - Server is installed on the access point, and client is installed on stations.
   ```python=1
    ApplicationContainer serverApps = server.Install(wifiApNode.Get(0));
    ApplicationContainer clientApps = client.Install(wifiStaNodes);
   ```

 - Flow monitoring Configuration
     - **FlowMonitorHelpe**r is used to monitor flows in the network.
     - Flow monitor is used to collect statistics like throughput and delay.
     - Statistics are collected for each flow and printed.
```python=1
    // Setup flow monitor
    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    // Stop simulator after 10 seconds
    Simulator::Stop(Seconds(10.0))
   ```

 - Open a CSV file for writing
    
     - At the end of simulation CSV file named “results_wifi.csv” is created to perform further Analysis. that contains the throughput and delay for each flow, as well as the average throughput and delay. Each row in the CSV file represents a flow, and the columns are “Flow”, “Source Address”, “Destination Address”, “Tx Bytes”, “Rx Bytes”, “Throughput (Mbps)”, and “Delay (s)”.
```python=1
// Open a CSV file for writing
    std::ofstream outputFile("results_wifi.csv");

    // Write header to CSV file
    outputFile << "Flow,Source,Destination,TxBytes,RxBytes,Throughput(Mbps),Delay(s)" << std::endl;

```
## 3. Simulation Execution
- Simulator::Run() is called to start the simulation.
- it simulates WiFi network with 1 AP and 2 stations, sets up the channel and mobility models, assigns IP addresses to the nodes, and simulates UDP traffic from the stations to the AP. It also collects and prints the statistics of the simulation, such as the number of transmitted and received bytes and the throughput.
The  simulation is configured and dispaly the result as illustrated in the following image. 

The statistics of four different flows in the network simulation is generated. A flow in this context refers to the stream of packets from a source node to a destination node.
After the simulation run, we observe the mobility of the node by using ./NetAnim feature.

![image](https://hackmd.io/_uploads/B1JEcpZeA.png)

- open XML tracer file and check the movement of nodes

![image](https://hackmd.io/_uploads/ByFTipbxR.png)

![image](https://hackmd.io/_uploads/By2onpbgR.png)

- open flow monitor to check the statistics





## 4. Simulation Analysis
Based on the result of our simulation, the breakdown of each flow is stated as follows
- Flow 1 (10.1.1.1 -> 10.1.1.3): This flow is from the node with IP address 10.1.1.1 (Node A) to the node with IP address 10.1.1.3 (Node C). The transmitted (Tx) and received (Rx) bytes are both 1052 bytes, and the throughput is approximately 0.000891791 Mbps.
- Flow 2 (10.1.1.2 -> 10.1.1.3): This flow is from the node with IP address 10.1.1.2 (Node B) to the node with IP address 10.1.1.3 (Node C). The transmitted (Tx) and received (Rx) bytes are both 1052 bytes, and the throughput is approximately 0.000891791 Mbps.
- Flow 3 (10.1.1.3 -> 10.1.1.1): This flow is from the node with IP address 10.1.1.3 (Node C) to the node with IP address 10.1.1.1 (Node A). The transmitted (Tx) and received (Rx) bytes are both 1052 bytes, and the throughput is approximately 0.000891791 Mbps.
- Flow 4 (10.1.1.3 -> 10.1.1.2): This flow is from the node with IP address 10.1.1.3 (Node C) to the node with IP address 10.1.1.2 (Node B). The transmitted (Tx) and received (Rx) bytes are both 1052 bytes, and the throughput is approximately 0.000891791 Mbps.
- In all four flows, the number of transmitted bytes equals the number of received bytes, which suggests that there is no packet loss in the network. The throughput is the same for all flows, indicating that the network resources are evenly distributed among the flows.
- Lost packets are checked in the flow monitor to assess network performance.
- Flow statistics are analyzed to determine throughput and delay for each flow. Results are written to a CSV file for further examination and comparison.

![image](https://hackmd.io/_uploads/BJbgvaWxA.png)

The results stored in csv file  is checked in barchart graph to observe throughput versus delay


![image](https://hackmd.io/_uploads/r1xkvY-gA.png)



## 5. Conclusions

This a basic  wifi simulation for studying basic WiFi networks in NS-3.
By observing the simuation structure and configurations, we gain a deeper understanding of network simulation principles.

This study note provides a comprehensive analysis for simulating a basic WiFi network in NS-3. It highlights key configurations, simulation steps, parameters used and observations contributing to a deeper understanding of wifi network simulation.


## References
 1. [NS-3 Simulator](https://www.nsnam.org/docs/manual/html/events.html)
 2. [Wifi Models](https://www.nsnam.org/docs/release/3.8/doxygen/group___wifi.html)
 3. [Wifi Design](https://github.com/imec-idlab/IEEE-802.11ah-ns-3/blob/master/src/wifi/doc/source/wifi-design.rst)