# <center> Assignment 2 </center>

- [Introduction](#Introduction)
- [Installation process](#Install-process)
- [Paper analysis](#Paper-analysis)
- [Simulation](#Simulation)


# Introduction
## Ns-3 definition
ns-3 is a discrete-event simulator typically run from the command line. It is written directly in C++, not in a high-level modeling language; simulation events are simply C++ function calls, organized by a scheduler.

# Installation procedures
## Installation of NS3 (ns-3.36.1)
**References:** [Installing ns-3 in Ubuntu 22.04](https://www.nsnam.com/2022/06/ns3-installation-in-ubuntu-2204.html)

Tools used in this simulation:
NS3 version ns-3.36.1 
OS Used: Ubuntu 22.04 LTS
Open terminal and follow the steps

### Step 1: Prerequisites

First run this line of code

```bash 
$ sudo apt update
```

Next install all these packages for the complete use of ns3.

```bash 
$ sudo apt install g++ python3 python3-dev pkg-config sqlite3 cmake python3-setuptools git qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools gir1.2-goocanvas-2.0 python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython3 openmpi-bin openmpi-common openmpi-doc libopenmpi-dev autoconf cvs bzr unrar gsl-bin libgsl-dev libgslcblas0 wireshark tcpdump sqlite sqlite3 libsqlite3-dev  libxml2 libxml2-dev libc6-dev libc6-dev-i386 libclang-dev llvm-dev automake python3-pip libxml2 libxml2-dev libboost-all-dev
```

### Step 2: Download ns-allinone-3.36.1.tar.bz2 from the website nsnam.org. 
[Download here](https://www.nsnam.org/releases/ns-allinone-3.36.1.tar.bz2)
### Step 3:
Unzip the above file content to the home folder. Check your home folder and do it accordingly. 
To unzip use the GUI with click right to extract and then select /home/your user name/folder. 
Otherwise you can use the command
```bash
$ tar jxvf ns-allinone-3.36.1.tar.bz2
```
### Step 4: 
```bash
 $ cd ns-allinone-3.36.1/ 
$ ./build.py --enable-examples --enable-tests
```

Once the installation is done. You can run the example as shown
```bash
$ cd ns-3.36.1/
$ ./ns3 run hello-simulator
```
Output will be
<p align="center">
<img src=/assets/Screenshot-1.png>

</p>


# Paper analysis

The paper provides an ovierw and analysis of the IEEE 802.11e standard, which introduces quality of service (QoS) support for wireless local area networks (WLANs)

Some points are:

Background on the legacy 802.11 protocol and its limitations in providing QoS support for different types of traffic (voice, video, best-effort, background).
Description of the new mechanisms introduced in 802.11e for QoS support: a. Enhanced Distributed Channel Access (EDCA) - Contention-based channel access with prioritized access categories and adjustable parameters. b. Hybrid Coordination Function Controlled Channel Access (HCCA) - Controlled channel access with polling by a centralized coordinator.

# Simulation

The simulations are part of the examples already set for ns-3. This simulation focuses on modeling and evaluating the performance of a wireless network adhering to the IEEE 802.11 standard. By examining the example files, we aim to gain a deeper understanding of how to configure, execute, and analyze network simulations within the ns-3 framework.

As shown below the list of examples provided by ns-3 simulator

<p align="center">
<img src=/assets/Screenshot-2.png>

</p>

In which example wifi-80211e-txop is used for study purpose.

Through the exploration of these example files, we delve into various aspects of wireless networking, including:

Network Topology Definition: Understanding how to define and create network topologies comprising access points and stations.

```
// Network topology description
NodeContainer wifiStaNodes;
wifiStaNodes.Create(4);
NodeContainer wifiApNodes;
wifiApNodes.Create(4);
Quality of Service (QoS) Configuration: Configuring QoS parameters such as Transmission Opportunity (TXOP) limits for different Access Categories (AC).
cpp
Copy code
// EDCA Configuration
// Modify EDCA configuration (TXOP limit) for AC_BE
Ptr<NetDevice> dev = wifiApNodes.Get(1)->GetDevice(0);
Ptr<WifiNetDevice> wifi_dev = DynamicCast<WifiNetDevice>(dev);
Ptr<WifiMac> wifi_mac = wifi_dev->GetMac();
PointerValue ptr;
Ptr<QosTxop> edca;
wifi_mac->GetAttribute("BE_Txop", ptr);
edca = ptr.Get<QosTxop>();
edca->SetTxopLimit(txopLimit);
```

```
Application Setup: Configuring applications for generating traffic and evaluating network performance.
// Application Configuration
// Configure UDP client application for STA A
InetSocketAddress destA(ApInterfaceA.GetAddress(0), port);
OnOffHelper clientA("ns3::UdpSocketFactory", destA);
clientA.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
clientA.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
clientA.SetAttribute("DataRate", StringValue("100000kb/s"));
clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));
clientA.SetAttribute("Tos", UintegerValue(0x70)); // AC_BE
````
Simulation Execution: Running the simulation and collecting data to analyze network behavior.

```
// Simulation Execution
// Start the applications and run the simulation
ApplicationContainer clientAppA = clientA.Install(wifiStaNodes.Get(0));
clientAppA.Start(Seconds(1.0));
clientAppA.Stop(Seconds(simulationTime + 1));
Simulator::Stop(Seconds(simulationTime + 1));
Simulator::Run();
```
Result Analysis: Assessing throughput and other performance metrics to gain insights into network performance.
```
// Results
// Measure throughput for each network and output results
uint64_t totalPacketsThroughA = DynamicCast<UdpServer>(serverAppA.Get(0))->GetReceived();
double throughput = totalPacketsThroughA * payloadSize * 8 / (simulationTime * 1000000.0);
std::cout << "AC_BE with default TXOP limit (0ms): " << '\n'
          << "  Throughput = " << throughput << " Mbit/s" << '\n';
```
By leveraging these example files, we aim to enhance our understanding of network simulation methodologies and techniques, ultimately enabling us to design and evaluate more complex network scenarios effectively.
