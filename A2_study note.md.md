:::success
Refrence
- [【ns-3】零基础安装教程](https://blog.csdn.net/Graduate2015/article/details/129027889?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522169678365616800225553173%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=169678365616800225553173&biz_id=0&spm=1018.2226.3001.4187)
- [ubuntu 18.04安装NS-3教程（详细安装过程）](https://blog.csdn.net/qq_39346534/article/details/107298205?spm=1001.2101.3001.4242.6&utm_relevant_index=12)
- [NS3-wiki](https://www.nsnam.org/wiki/Installation)
- [NS3的範例運行](https://blog.csdn.net/hhhhh11123/article/details/115530856?spm=1001.2101.3001.6661.1&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7EBlogCommendFromBaidu%7ERate-1-115530856-blog-52386462.235%5Ev38%5Epc_relevant_sort_base2&utm_relevant_index=1)
- [利用NS3部屬一個LTE網路](https://blog.csdn.net/Shmily1107/article/details/110919007?ops_request_misc=&request_id=&biz_id=102&spm=1018.2226.3001.4187)
- [NS-3 Model Library](https://www.nsnam.org/docs/models/html/index.html)
- [NS-3 22.Mobility](https://www.nsnam.org/docs/models/html/mobility.html)
:::
# Weekly plan
:::success
**Week1**
- NS3-Install
- Read research paper 

**Week2**
- Study NS-3 Events and Simulator
- Study NS-3 Wifi - Models
- Trace code & Simulator result

**Week3**
- Numercial results & Oral presentation
- Flowchart + source code for analysis
- Midterm Progress Report Presentation

:::
# Studying note
## *Definition of NS3*
NS-3 (Network Simulator 3) is an open source software used for network communication research and simulation. It is a powerful tool for simulating and evaluating the performance of various communication networks, including wired and wireless networks, protocols and applications.

Its open source nature and modular architecture make it an important tool in academia and industry for researching new communication protocols, testing network performance, and conducting experiments.
## *The difference between NS3 and NS2*
NS-3 is not an expansion kit for NS-2, but a brand new simulator.

NS-2 is written in C++ and OTcl program code, and you need to learn the new OTcl language during the learning process. NS-3 is all written in C++ language, and has optional Python language binding. Users can choose C++ or Python language to write scripts, making it more flexible to use.

NS-3 does not support NS-2 API. Although some modules in NS-2 have been ported to NS-3, NS-3 is far from complete as NS-2. During the development of NS-3, the NS-3 task force will continue to maintain NS-2 and also study transition and integration mechanisms. NS-3 does not include all the features of the current NS-2, but it has some new features, such as correct handling of multiple network cards, use of IP addressing strategies, more detailed 802.11 modules, etc.



# Install NS-3 

## Create-Deployment for NS-3
```
sudo apt-get install gcc g++ python python3
```
```
sudo apt-get install gcc g++ python python3 python3-dev
```
```
sudo apt-get install python3-setuptools git mercurial
```
```
sudo apt-get install qt5-default mercurial
```
```
sudo apt-get install gir1.2-goocanvas-2.0 python-gi python-gi-cairo python-pygraphviz python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython ipython3  
```
```
sudo apt-get install openmpi-bin openmpi-common openmpi-doc libopenmpi-dev
```
```
sudo apt-get install autoconf cvs bzr unrar
```
```
sudo apt-get install gdb valgrind 
```
```
sudo apt-get install uncrustify
```
```
sudo apt-get install doxygen graphviz imagemagick
```
```
sudo apt-get install texlive texlive-extra-utils texlive-latex-extra texlive-font-utils dvipng latexmk
```
```
sudo apt-get install python3-sphinx dia 
```
```
sudo apt-get install gsl-bin libgsl-dev libgsl23 libgslcblas0
```
```
sudo apt-get install tcpdump
```
```
sudo apt-get install sqlite sqlite3 libsqlite3-dev
```
```
sudo apt-get install libxml2 libxml2-dev
```
```
sudo apt-get install cmake libc6-dev libc6-dev-i386 libclang-6.0-dev llvm-6.0-dev automake 
```
```
sudo apt-get install libgtk2.0-0 libgtk2.0-dev
```
```
sudo apt-get install vtun lxc uml-utilities
```
```
sudo apt-get install libboost-signals-dev libboost-filesystem-dev
```

## Download NS-3 (by git)

```c=
cd
mkdir repos 
cd repos
git clone https://gitlab.com/nsnam/ns-3-allinone.git 
```
```c=
cd ns-3-allinone 
python3 download.py -n ns-3.39 // download 3.39 version
//You also can download other version by change the number 
//Ex: 3.39 >> 3.38
```

## Compile NS-3
``` c=
./build.py --enable-examples --enable-tests
```
```c=
cd ns-3.39
./ns3 distclean

./ns3 configure --build-profile=debug --enable-examples --enable-tests

./ns3 build
```

## Text NS-3

In order to allow users to quickly verify whether ns-3 has been built correctly, ns-3 comes with the test.py script project (in the ns main project ns-3.37 directory). By executing the test examples included in ns-3, test.py can test each module of ns-3 one by one:

```c=
cd ns-3.39
./test.py
```

```c=
./ns3  run hello-simulator
```
![image](https://hackmd.io/_uploads/B17v_dZeA.png)

## NS3 Topology

![image](https://hackmd.io/_uploads/BkIosuZeR.png)

# Simulate wifi-hidden-terminal.cc 
## Topology
[node 0] <-- -50 dB --> [node 1] <-- -50 dB --> [node 2]
![image](https://hackmd.io/_uploads/H1UKxFZxA.png)


## Why use Ad-hoc instead of Access Point?
Ad-hoc network is a wireless network that does not require an access point (Access Point), in which wireless devices communicate directly in a point-to-point manner. Such networks are also known as multi-hop wireless networks, ad hoc networks, or unstructured networks. An important feature of an Ad-hoc network is that all nodes have equal status and have the ability to forward messages.

In an Ad-hoc network, communication between nodes is limited by the propagation range of the nodes. When two nodes are far apart, such as between Node 0 and Node 2, we can communicate between them through the intermediate node Node 1. This propagation mode is only possible in Ad-hoc networks.

:::spoiler
Ad-hoc網絡是一種無需接入點（Access Point）的無線網絡，其中各無線設備以點對點的方式直接通信。這種網絡也被稱為多跳無線網絡、自組織網絡或無固定設施的網絡。Ad-hoc網絡的重要特點是所有節點地位平等，且具有報文轉發能力。

在Ad-hoc網絡中，節點之間的通信受到節點傳播範圍的限制。當兩個節點距離較遠時，例如Node 0和Node 2之間，我們可以通過中間節點Node 1來實現它們之間的通信。這種傳播模式僅在Ad-hoc網絡中才能實現。
:::

![image](https://hackmd.io/_uploads/BkWobKWlC.png)
## Matrix propagation loss model
[Reference](https://www.nsnam.org/docs/release/3.18/doxygen/classns3_1_1_matrix_propagation_loss_model.html)

![image](https://hackmd.io/_uploads/HkfpEobl0.png)
- ns3::Matrix Propagation Loss Model：矩陣傳播損耗模型類別。
- Set Loss：設定損耗。
- Print Psr(Packet Success Ratio)
----
### [Print PsrVsCollisionInterval](https://www.nsnam.org/docs/release/3.16/doxygen/wifi-phy-test_8cc_source.html)：列印Psr與碰撞間隔的關係。
:::spoiler
這段程式碼用於模擬不同碰撞間隔下的封包成功率。透過設定發射器數量、位置和不同間隔，計算並輸出每個時間點的 A 和 B 的封包成功率。這有助於分析碰撞間隔對封包成功率的影響。

程式功能： 程式會遍歷不同的碰撞間隔（從0到4000微秒），並對每個碰撞間隔進行模擬，計算相應的封包成功率（PSR）。

設定參數： 程式使用 CollisionExperiment::Input 來設定模擬所需的參數，包括每個發射器發送的封包數量（nPackets）、發射器A的位置（xA）、發射器B的位置（xB）等。

遍歷不同碰撞間隔： 程式使用兩個 for 迴圈，第一個迴圈從碰撞間隔0到100微秒，每次增加1微秒；第二個迴圈從碰撞間隔100到4000微秒，每次增加50微秒。這樣可以遍歷不同的碰撞間隔情況。

執行模擬並輸出結果： 在每個碰撞間隔下，程式會執行模擬並計算封包成功率（PSR），然後將結果輸出到控制台。
:::

This code is used to simulate the packet success rate under different collision intervals. By setting the number, location and different intervals of transmitters, the packet success rate of A and B at each time point is calculated and output. This helps analyze the impact of collision intervals on packet success rates.

1. Traverse different collision intervals (from 0 to 4000 microseconds), simulate each collision interval, and calculate the corresponding packet success rate (PSR).

2. Use CollisionExperiment::Input to set the parameters required for simulation, including the number of packets sent by each transmitter (nPackets), the position of transmitter A (xA), the position of transmitter B (xB), etc.

3. Use two for loops. The first loop ranges from the collision interval from 0 to 100 microseconds, increasing by 1 microsecond each time; the second loop ranges from the collision interval from 100 to 4000 microseconds, increasing by 50 microseconds each time. . This allows you to traverse different collision interval situations.

Execute the simulation and output the results: At each collision interval, the program will execute the simulation and calculate the Packet Success Rate (PSR), and then output the results to the console.
```bash=
碰撞間隔  PSR(A)  PSR(B)
0          0.85    0.92
1          0.83    0.91
2          0.82    0.89
3          0.81    0.88
...        ...     ...
100        0.7     0.75
150        0.65    0.72
200        0.6     0.68
250        0.55    0.64
...        ...     ...
4000       0.1     0.15
```


---
### [Print PsrVs Distance](https://www.nsnam.org/docs/release/3.16/doxygen/wifi-phy-test_8cc_source.html)：列印Psr與距離的關係
:::spoiler
用於列印封包成功率（Packet Success Ratio，PSR）與距離的關係。透過設定不同的距離和傳輸模式，計算並輸出每個距離點的 PSR

程式功能： 程式會遍歷不同的通信距離（1.0 到 165.0，每次增加 2.0），並對每個距離進行模擬，計算相應的封包成功率（PSR）。

設定參數： 程式使用了 PsrExperiment::Input 來設定模擬所需的參數，包括發射功率等級、傳輸模式、發送封包數量和封包大小等。

遍歷不同傳輸模式： 程式在每個通信距離下，使用不同的傳輸模式（OfdmRate）來進行模擬，包括 6Mbps、9Mbps、12Mbps、18Mbps、24Mbps、36Mbps、48Mbps 和 54Mbps。

計算 PSR： 程式在每個通信距離和傳輸模式下，執行模擬並計算封包成功率（PSR），並將結果輸出到控制台。
:::


Used to print the relationship between Packet Success Ratio (PSR) and distance. Calculate and output the PSR of each distance point by setting different distances and transmission modes

1. It will traverse different communication distances (1.0 to 165.0, increasing by 2.0 each time), simulate each distance, and calculate the corresponding packet success rate (PSR).

2. Use PsrExperiment::Input to set the parameters required for simulation, including transmit power level, transmission mode, number of packets sent, packet size, etc.

3. Under each communication distance, use different transmission modes (OfdmRate) for simulation, including 6Mbps, 9Mbps, 12Mbps, 18Mbps, 24Mbps, 36Mbps, 48Mbps and 54Mbps.

Calculate PSR: The program performs simulation and calculates the packet success rate (PSR) under each communication distance and transmission mode, and outputs the results to the console.
```bash=
距離     PSR (OfdmRate6Mbps)
1.0      0.8
3.0      0.75
5.0      0.7
7.0      0.68
9.0      0.65
11.0     0.63
13.0     0.6
15.0     0.58
...
163.0    0.05
```
---
### [PrintSize Vs Range](https://www.nsnam.org/docs/release/3.16/doxygen/wifi-phy-test_8cc_source.html)：列印大小與範圍的關係。
:::spoiler
目標封包成功率（Target PSR）設定： 在程式中，設定了目標封包成功率為0.05（即5%的成功率）。

參數設定： 程式建立了一個 PsrExperiment::Input 的物件 input，並設定了發送封包所需的參數，如發射功率等級、傳輸模式、發送封包數量等。

遍歷不同封包大小： 程式使用一個 for 迴圈，從封包大小為10開始，每次增加40，直到封包大小為3000。這樣可以遍歷不同的封包大小情況。

計算通信範圍： 在每個封包大小下，程式使用二分法來計算所需的通信範圍。具體來說，程式初始化了一個低值（low）和高值（high），然後在這個範圍內進行二分搜索，直到找到一個通信範圍，在這個範圍內可以達到目標封包成功率。

輸出結果： 當找到符合條件的通信範圍時，程式會輸出封包大小和對應的通信範圍，這樣可以用來分析在不同封包大小下，系統需要的通信範圍情況。

當封包大小為100時，所需通信範圍為50米
當封包大小為500時，所需通信範圍為120米
當封包大小為1000時，所需通信範圍為180米
:::

Target packet success rate (Target PSR) setting: In the program, the target packet success rate is set to 0.05 (ie, 5% success rate).

1. Create an object input of PsrExperiment::Input, and set the parameters required to send packets, such as transmit power level, transmission mode, number of packets sent, etc.

2. The program uses a for loop, starting from the packet size of 10 and increasing by 40 each time until the packet size is 3000. This allows you to traverse different packet size situations.

3. At each packet size, the program uses the bisection method to calculate the required communication range. Specifically, the program initializes a low value (low) and a high value (high), and then performs a binary search within this range until it finds a communication range within which the target packet success rate can be achieved.

4. When a qualified communication range is found, the program will output the packet size and corresponding communication range, which can be used to analyze the communication range required by the system under different packet sizes.

EX:
- When the packet size is ==100==, the required communication range is ==50== meters
- When the packet size is ==500==, the required communication range is ==120== meters
- When the packet size is ==1000==, the required communication range is ==180== meters
# source code for simulation and analysis
Function:
Simulate with and without the execution of the RTS/CTS protocol, and observe the sending and receiving between nodes by establishing a mode for monitoring flow1 and flow2.
```c=
void experiment (bool enableCtsRts, std::string wifiManager)

input:
enableCtsRts whether to enable RTS/CTS protocol,
wifiManager traffic management, set up remote site manager, (mode Aarf, Aarfcd, Amrr, Arf, Cara, Ideal, Minstrel, Onoe, Rraa, command line interactive control)
```
---

Whether to enable the RTS/CTS protocol according to the ==enableCtsRts== just now (it will be enabled if it exceeds the threshold, so set the threshold, the small one is 100 to enable it, the large one is 2200 and it is not enabled)
```c=
  UintegerValue ctsThr = (enableCtsRts ? UintegerValue (100) : UintegerValue (2200));
  Config::SetDefault ("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);
```
---
Create network node
```c=
  NodeContainer nodes;
  nodes.Create (3);
```
---
Use the Poisson model to move each node (According to the number of network nodes)

```c=
  for (uint8_t i = 0; i < 3; ++i)
    {
      nodes.Get (i)->AggregateObject (CreateObject<ConstantPositionMobilityModel> ());   //???????
    }
```
---
Install two communication flows based on On-Off applications, as follows:

First, an application container cbrApps and a UDP port cbrPort (port 12345) are created.

Then use the OnOffHelper class to configure the On-Off application, specifying the use of UDP transmission, the destination IP address is "10.0.0.2", the destination port is cbrPort, and the size of the sent packet is set to 1400 bytes.

- For flow 1, the data rate is set to 3000000bps, the startup time is 1 second, and the application is installed on node 0.

- For flow 2, a slightly different data rate is set to 3001100bps, the startup time is 1.001 seconds, and the application is installed on node 2.

>OnOffApplication: The OnOff application is a switch application. It generates traffic when it is turned on and does not generate traffic when it is turned off.

```c=
  ApplicationContainer cbrApps;
  uint16_t cbrPort = 12345;
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address ("10.0.0.2"), cbrPort));
  onOffHelper.SetAttribute ("PacketSize", UintegerValue (1400));
  onOffHelper.SetAttribute ("OnTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
  onOffHelper.SetAttribute ("OffTime", StringValue ("ns3::ConstantRandomVariable[Constant=0]"));

  // flow 1:  node 0 -> node 1
  onOffHelper.SetAttribute ("DataRate", StringValue ("3000000bps"));
  onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.000000)));
  cbrApps.Add (onOffHelper.Install (nodes.Get (0)));

  // flow 2:  node 2 -> node 1
  /** \internal
 * The slightly different start times and data rates are a workaround
 * for \bugid{388} and \bugid{912}
   */
  onOffHelper.SetAttribute ("DataRate", StringValue ("3001100bps"));
  onOffHelper.SetAttribute ("StartTime", TimeValue (Seconds (1.001)));
  cbrApps.Add (onOffHelper.Install (nodes.Get (2)));
```
---
:::spoiler
這段程式碼是用來建立 UDP 回顯應用，作為解決ARP（Address Resolution Protocol）不完整的一種解決方法。具體解釋如下：

定義了一個 UDP 回顯的目的地埠 echoPort，設置為 9。

使用 UdpEchoClientHelper 類別來設定 UDP 回顯客戶端，指定目的地 IP 地址為 "10.0.0.2"，目的地埠為 echoPort。

設定了最大發送封包數量為 1，發送間隔為 0.1 秒，發送封包大小為 10 字節。

創建了一個應用程式容器 pingApps，並設定不同的啟動時間來安裝 UDP 回顯客戶端應用程式在節點 0 和節點 2 上。

這段程式碼的作用是在開始CBR（Constant Bit Rate）通訊流之前，使用 UDP 回顯應用來發送一個單獨的封包，以確保網絡中的完整性。
:::
Used to build UDP echo applications as a solution to incomplete ARP (Address Resolution Protocol).

Defines a UDP echo destination port echoPort, set to 9. Use the UdpEchoClientHelper class to configure a UDP echo client, specifying the destination IP address as "10.0.0.2" and the destination port as echoPort. The maximum number of sent packets is set to 1, the sending interval is 0.1 seconds, and the sending packet size is 10 bytes. Created an application container pingApps and set different startup times to install the UDP echo client application on node 0 and node 2.

The function is to use the UDP echo application to send a separate packet before starting the CBR (Constant Bit Rate) communication flow to ensure the integrity of the network.

```c=
 /** \internal
 * We also use separate UDP applications that will send a single      
 * packet before the CBR flows start.
 * This is a workaround for the lack of perfect ARP, see \bugid{187}  

   */
  uint16_t  echoPort = 9;
  UdpEchoClientHelper echoClientHelper (Ipv4Address ("10.0.0.2"), echoPort);
  echoClientHelper.SetAttribute ("MaxPackets", UintegerValue (1));
  echoClientHelper.SetAttribute ("Interval", TimeValue (Seconds (0.1)));  
  echoClientHelper.SetAttribute ("PacketSize", UintegerValue (10)); 
  ApplicationContainer pingApps;

  // again using different start times to workaround Bug 388 and Bug 912 
  echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.001)));
  pingApps.Add (echoClientHelper.Install (nodes.Get (0)));  
  echoClientHelper.SetAttribute ("StartTime", TimeValue (Seconds (0.006)));
  pingApps.Add (echoClientHelper.Install (nodes.Get (2)));

```
---
Monitor some important parameters
```c=
  FlowMonitorHelper flowmon;
  Ptr<FlowMonitor> monitor = flowmon.InstallAll ();
```
---
Start simulation
```c=
  Simulator::Stop (Seconds (10));
  Simulator::Run ();
```
### Simulation result:
![image](https://hackmd.io/_uploads/H1Jzh2WxR.png)
