#  Study Note: IEEE 802.11e QoS Analysis

 Desalegn Aweke Wako
- [Study Note: IEEE 802.11e QoS Analysis](#study-note-ieee-80211e-qos-analysis)
  - [1. Introduction](#1-introduction)
  - [2. Assumptions](#2-assumptions)
  - [3. Experimental Set up](#3-experimental-set-up)
  - [4. Results and  Analysis](#4-results-and-analysis)
    - [4.1 Throughput Trends](#41-throughput-trends)
    - [4.2 Throughput as the number of stations increases](#42-throughput-as-the-number-of-stations-increases)
    - [4.3 Discussion](#43-discussion)
  - [5. Conclusions](#5-conclusions)
  - [References](#references)



## 1. Introduction
Quality of Service (QoS) is a crucial aspect of network management, ensuring that different types of traffic receive the appropriate level of service as per their requirements. In this experiment, we investigate the performance of various Access Categories (ACs) in a network environment by analyzing throughput concerning offered traffic.

IEEE 802.11e defines Enhanced Distributed Channel Access (EDCA) and Hybrid Coordination Function (HCF) to provide QoS mechanisms. The Qos features of the IEEE 802.11e MAC layer protocol is implemented using the NS-3 simulator. EDCA use parameters for different Access Categories (ACs) to ensure prioritized traffic handling. 

In EDCA, packets are classified into four access categories, each category having a different priority.  These are

    * Voice
    * Video
    * Best effort
    * Background


## 2. Assumptions
The following assumptions are used for simulation

    * MAC Protocol: IEEE 802.11e
    * PHY Protocol: IEEE 802.11a
    * Control Frame Rate: 6 Mbps
    * Data Frame Rate: 24 Mbps
    * Traffic Load: 250 kbps per streams

For Qos parameters simulation, we have utilized following  tools. 

		*	virtual machine to install the simultor
		*	ubuntu 20.04
		*	NS-3.38*

## 3. Experimental Set up

 Based on the article [1] and existing literature,the following EDCA parameters are used for the simulation. we configured the EDCA parameters for each access category as indicated in the table

| Access Category(AC)| AIFSN | CWmin |CWmax|
|----------          |-------|-------|------|
| AC_VO              | 2     |3      |7   |
| AC_VI              | 2     | 7     |15|
| AC_BE              | 3     | 15    |1023|
| AC_BK              | 7     | 15    |1023|
| High               | 2     | 7     | 7|
| Medium             | 4     | 10    |31|
| Low                | 7     | 31    |255|

- **Node Creation**

Three Station (STA) nodes and one Access Point (AP) node are created.
```
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(3); 
    NodeContainer wifiApNode;
    wifiApNode.Create(1); 
```
- **Wifi Configuration**

WiFi is configured to use the 802.11a standard with fixed data and control rates.
```
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode", StringValue ("OfdmRate24Mbps"),
                                "ControlMode", StringValue ("OfdmRate6Mbps"));
```

- **Qos Configuration**

Quality of Service (QoS) is configured for different Access Categories (ACs) - Voice (AC_VO), Video (AC_VI), Best Effort (AC_BE), and Background (AC_BK). Each category is given specific queue sizes, AIFS, CWmin, and CWmax values to prioritize traffic differently.
```
// Create an instance of QosWifiMacHelper
QosWifiMacHelper wifiMac;
wifiMac.SetType("ns3::QosWifiMac");

// Set maximum queue sizes for different Access Categories (AC)
wifiMac.SetMaxQueueSize("AC_VO", 100); 
wifiMac.SetMaxQueueSize("AC_VI", 50);  
wifiMac.SetMaxQueueSize("AC_BE", 50);  
wifiMac.SetMaxQueueSize("AC_BK", 20); 

// Set Arbitration Inter-Frame Space (AIFS) values for different ACs
wifiMac.SetAifs("AC_VO", 2); 
wifiMac.SetAifs("AC_VI", 2); 
wifiMac.SetAifs("AC_BE", 3); 
wifiMac.SetAifs("AC_BK", 7); 

// Set minimum contention window (CWmin) values for different ACs
wifiMac.SetMinCw("AC_VO", 3);  
wifiMac.SetMinCw("AC_VI", 7); 
wifiMac.SetMinCw("AC_BE", 15); 
wifiMac.SetMinCw("AC_BK", 15);

// Set maximum contention window (CWmax) values for different ACs
wifiMac.SetMaxCw("AC_VO", 7);   
wifiMac.SetMaxCw("AC_VI", 15);  
wifiMac.SetMaxCw("AC_BE", 1023); 
wifiMac.SetMaxCw("AC_BK", 1023);
```
- **Device Installation**

WiFi PHY and MAC layers are installed on the STA (HC) and AP nodes.
```
    InternetStackHelper stack;
    stack.Install(wifiStaNodes);
    stack.Install(wifiApNode);
```
- **Simulation Execution**

Applications are started at 1 second and stopped at 10 seconds. The simulation runs for 10 seconds, after which the throughput for each Access Category is calculated and printed.
```
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(10.0));

    Simulator::Stop(Seconds(10.0));
    Simulator::Run();

    Time startTime = Seconds(1.0);
    Time stopTime = Seconds(10.0);
    std::cout << "AC_VO Throughput: " << CalculateThroughput(sinks[0], startTime, stopTime) << " Mbps" << std::endl;
    std::cout << "AC_VI Throughput: " << CalculateThroughput(sinks[1], startTime, stopTime) << " Mbps" << std::endl;
    std::cout << "AC_BE Throughput: " << CalculateThroughput(sinks[2], startTime, stopTime) << " Mbps" << std::endl;
    std::cout << "AC_BK Throughput: " << CalculateThroughput(sinks[3], startTime, stopTime) << " Mbps" << std::endl;

    Simulator::Destroy();
```


## 4. Results and  Analysis

### 4.1 Throughput Trends

In this simulation, the HC transmits four distinct streams to each of the three stations, with each stream being assigned a specific priority level. The author's illustrated the relationship between offered traffic and throughput for each access category as follows.
- **AC_VO (Voice):**
    Throughput increases linearly up to 12 Mb/s of offered traffic and the throughput stabilizes after 12 Mb/s, indicating the saturation point for voice traffic.

- **AC_VI (Video):** Throughput increases steadily up to 9 Mb/s of offered traffic, peaking at 9 Mb/s. After 9 Mb/s, throughput gradually declines, showing a reduction in performance due to increased traffic load.
  
- **AC_BE (Best Effort):** Throughput increases up to 7 Mb/s of offered traffic. Beyond this point, throughput declines sharply, indicating a significant drop in performance under high traffic conditions.

- **AC_BK (Background):** Throughput follows a similar pattern to AC_BE, peaking at 5 Mb/s  of offered traffic.
A sharp decline in throughput is observed beyond this traffic load. 

The relationship between offered traffic and throughput for each access category is visualized in the following figure
![](./assets/Throughput%20vs%20Traffic.JPG)

AC_VO (voice) has the highest throughput initially but drops significantly with more stations.
AC_BK (background) has the lowest throughput but remains relatively stable.

### 4.2 Throughput as the number of stations increases

The throughput trends for each AC as the number of stations increases are illustrated below
  ![](./assets/Increased%20Sta%20plot.JPG)

  -  As the number of stations increases, the throughput decreases for all ACs.
  - The results suggests that the network prioritizes voice and video transmissions over best effort and background traffic. 
  - As the offered traffic per AC increases, the throughput for all categories declines, with voice being the most affected.


### 4.3 Discussion

- Voice (AC_VO): Exhibits a high throughput at lower traffic levels, indicating good performance for voice transmission. However, it experiences a sharp decline as traffic increases, suggesting that voice is highly sensitive to congestion.
- Video (AC_VI): Starts with a moderate throughput and decreases more gradually compared to voice. This trend shows that video transmission is more resilient to increased traffic but still affected by congestion.
- Best Effort (AC_BE) and Background (AC_BK): Both start at lower throughput levels and follow a similar downward trend as video, albeit at reduced throughputs. This indicates that these ACs are deprioritized in the network, likely to ensure better service for voice and video.
  
Ingeneral, the implication of  QOS in the network design are significant
  - Prioritization: Ensuring high-priority traffic (voice and video) receives sufficient bandwidth and low latency is crucial for maintaining service quality.
  - Resource Allocation: Allocating resources dynamically based on traffic type and load can optimize network performance and meet QoS requirements.
  - Traffic Management: Implementing advanced QoS mechanisms for video traffic can help maintain performance under varying load conditions.


## 5. Conclusions

The analysis of throughput versus offered traffic for various access categories demonstrates the critical role of QoS in maintaining network performance. AC_VO and AC_VI benefit from higher QoS prioritization, ensuring reliable performance for voice and video applications. 

Conversely, AC_BE and AC_BK experience significant performance degradation under high traffic loads, reflecting their lower QoS priority. These insights are essential for network design and traffic management strategies to ensure efficient and reliable communication services.


## References
 1. [S. Mangold, Sunghyun Choi, G. R. Hiertz, O. Klein and B. Walke, "Analysis of IEEE 802.11e for QoS support in wireless LANs," in IEEE Wireless Communications, vol. 10, no. 6, pp. 40-50,2003](https://ieeexplore.ieee.org/document/1265851)
 2. [NS-3 Simulator](https://www.nsnam.org/docs/manual/html/events.html)
 2. [Wifi Models](https://www.nsnam.org/docs/release/3.8/doxygen/group___wifi.html)
 3. [Wifi Design](https://github.com/imec-idlab/IEEE-802.11ah-ns-3/blob/master/src/wifi/doc/source/wifi-design.rst)
