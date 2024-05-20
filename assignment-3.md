
:::warning
**Table of Content**
[TOC]
:::
- reference: [hacdMD note](https://hackmd.io/@SylviaCh/SJSAewm7A)
# Simulation
## Run code
### 1. Navigate to the folder ns-allinone-3.40/ns-3.40/examples/wireless.
```bash=
cd Downloads/ns-allinone-3.40/ns-3.40/examples/wireless
```

### 2. Create the file wifi-txop-4QBSS.cc.
   - [Full code](https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-M11202255-Sylvia/wifi-txop-4QBSS.cc)

### 3. Open CMakeList.txt and add the following line to build_example.
```
build_example(
  NAME wifi-txop-4QBSS
  SOURCE_FILES wifi-txop-4QBSS.cc
  LIBRARIES_TO_LINK ${libwifi}
                    ${libapplications}
)
```
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/67dceaa3-53c1-47f6-a110-bb8984f3b8e7)


### 4. Compile.
```bash=
./ns3 build
```
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/94882339-4321-4c55-8bac-1797d2ae3cb5)


```bash=
./ns3 run examples/wireless/wifi-txop-4QBSS.cc
```
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/29f041b4-2aca-4495-93cc-5c0aade61f10)


### 5. Upload the execution result output_file.txt to Google Drive and use [colab](https://colab.research.google.com/drive/13YoeMkiFhquJEpHV5tZKtOT7awAD9tll?usp=sharing) to plot the simulation graph.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/1d4a470d-a770-419b-8dd3-b8ba6747821a)
   
   - [Python code](https://colab.research.google.com/drive/13YoeMkiFhquJEpHV5tZKtOT7awAD9tll?usp=sharing)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/19ca486a-f7bf-41cc-9e4f-f8e81edf304e)


## Assumption 
- IEEE 802.11e MAC + IEEE 802.11a PHY
- Values for the EDCA parameter sets
    | | AC_VO | AC_VI | AC_BE | AC_BK |
    | -------- | -------- | -------- | -------- | -------- | 
    | AIFSN: | 2 | 2 | 3 | 7 | 
    | CWmin: | 3 | 7 | 15 | 15 | 
    | CWmax: | 7 | 15 | 1023 | 1023 | 

## result
- CAs
    - AC_VI: 0xa0
    - AC_VO: 0xc0
    - AC_BE: 0x00
    - AC_BK: 0x20
    
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/76e4824c-7e38-40ea-b123-03b514d811a0)

---

# Study note
reference: 
- [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://ieeexplore.ieee.org/document/1265851)
## **ABSTRACT**
- This paper provides an overview of the new protocol for QoS offered by the 802.11e WLAN standard.
- It analyzes the enhancements of 802.11e and compares the performance with the legacy 802.11 standard.
- The paper evaluates the contention-based and polling-based medium access control mechanisms in the 802.11e Hybrid Coordination Function (HCF).
- It discusses the ability of 802.11e to support QoS through simulations.

## **INTRODUCTION**
- 802.11 WLANs are widely deployed in various environments such as enterprises, homes, and public access networks.
- The primary features of the 802.11 standard include the simplicity and fault tolerance of the distributed medium access control protocol.
- Currently, 802.11 only offers best-effort service, similar to wireless Ethernet.
- There is increasing interest in applications that require QoS support, leading to initiatives to enhance the 802.11 MAC protocol to support QoS.

## **LEGACY 802.11**
- An overview of the legacy 802.11 MAC protocol's Distributed Coordination Function (DCF) is provided:
  - Based on Carrier Sense Multiple Access (CSMA)
  - Collision Avoidance (CA) mechanism
  - Optional fragmentation and RTS/CTS mechanisms
- The limitations of using Point Coordination Function (PCF) in 802.11 for QoS support are explained:
  - Unpredictable beacon delays and polling station transmission times
  - Lack of control over parameters affecting QoS

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/c169008e-ca96-4f65-b93d-0dc25610bae3)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/65ca1700-9806-4c43-9249-9931cbcd9b2c)

## **LIMITED QOS SUPPORT APPLYING THE POINT COORDINATION FUNCTION**
- The legacy 802.11 used PCF to provide QoS support for time-sensitive services.
- PCF offers a mechanism for prioritizing access to the wireless medium, coordinated centrally by a station called the Point Coordinator (PC).
- The drawbacks include:
  - Unpredictable beacon delays leading to uncertain CFP start times.
  - Inability to control the transmission time of polled stations, affecting QoS.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/a938440a-d5f8-4571-a481-91011794ca84)

---
## **QOS SUPPORT MECHANISMS OF 802.11E**
- 802.11e introduces the Hybrid Coordination Function (HCF) to provide QoS support.
- HCF defines two types of medium access mechanisms:
  - Enhanced Distributed Channel Access (EDCA) - contention-based.
  - HCF Controlled Channel Access (HCCA) - includes polling.
- The concept of Transmission Opportunity (TXOP) is introduced, allowing the Hybrid Coordinator (HC) to control the length of TXOPs to manage QoS.

## **BASIC IMPROVEMENTS OF THE LEGACY 802.11 MAC**
- 802.11e stations do not transmit across the Target Beacon Transmission Time (TBTT), reducing beacon delays.
- 802.11e allows direct communication between stations within a QoS Basic Service Set (QBSS) without passing through the AP.
- The Block Acknowledgment (Block Ack) mechanism is introduced to improve transmission efficiency.
- The Direct Link Protocol (DLP) is defined to establish direct links between 802.11e stations.

## **HCF CONTENTION-BASED MEDIUM ACCESS**
- EDCA introduces access categories (AC) and multiple independent backoff entities.
- There are four ACs representing different priorities: AC_VO (voice), AC_VI (video), AC_BE (best effort), and AC_BK (background).
- Each AC uses different EDCA parameter sets (e.g., AIFS, CWmin, CWmax) to contend for medium access.
- If multiple backoff entities within the same station are ready to transmit simultaneously, a virtual collision occurs, and the higher priority AC is granted access.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/dcf30eda-f3ea-44f1-a0ba-34e629e8681e)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/a951f197-af7c-4ab3-bb55-3520f38b9da7)


## **HYBRID COORDINATION FUNCTION, CONTROLLED MEDIUM ACCESS**
- HCCA extends the EDCA rules, allowing the HC to have the highest priority access to the medium.
- During both CFP and CP, the HC can obtain a TXOP without backoff after sensing an idle PIFS.
- During CP, stations can obtain a TXOP by EDCA rules or by receiving a QoS CF-Poll from the HC.
- During CFP, only stations explicitly polled by the HC are allowed to transmit.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/de09bee1-9205-48c8-9a7d-5bbd471ef843)
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/ba8c25de-b4fa-44f4-ab31-ea4a7e15b679)

### **IMPROVED EFFICIENCY**
- Block Acknowledgment (Block Ack) allows multiple MPDUs to be transmitted consecutively within a single TXOP and acknowledged only once.
- Direct Link Protocol (DLP) allows stations within a QBSS to transmit directly to each other without going through the AP.
- New mechanisms are introduced to enhance performance, such as limiting TXOP length and preventing transmissions that cross TBTT boundaries.

---

## **ACHIEVABLE EDCA THROUGHPUT IN AN ISOLATED QBSS WITH FOUR STATIONS**
- Evaluating the achievable EDCA throughput in an isolated QBSS with only four stations.
- It's observed that as the offered traffic increases, higher priority ACs limit the achievable throughput of lower priority ACs.
- This is because higher priority ACs use smaller AIFSN, CWmin, and CWmax values.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/0a0ae772-6226-4cc3-ab1b-b0bbbea8fd14)



## **ACHIEVABLE EDCA THROUGHPUT WITH INCREASING NUMBER OF STATIONS:**
- Assessing the throughput performance of EDCA as the number of stations increases to 16.
- It's noted that with default EDCA parameter settings, the throughput significantly decreases as the number of stations increases.
- This is due to the increased collision probability, especially for AC_VO with smaller CWmin and CWmax values.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/9eb25852-8c70-4352-9eb2-9925ce9abd59)



## **QOS GUARANTEES WITH PRIORITIZED ACCESS OF HC:**
- Evaluating the performance when allowing the HC to have the highest priority access.
- Within a single QBSS, HCCA's high priority transmissions can maintain below a certain latency threshold.
- However, in overlapping QBSS environments, the transmission latency and throughput of HCCA are affected due to the lack of coordination among HCs.

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/80584940/f96fe571-b4a9-4d96-8deb-8b49d0aec83f)

Through simulations, it's observed that EDCA can meet various priority demands with fewer stations but requires parameter adjustments as the number of stations increases. HCCA can ensure the latency quality of high priority transmissions, but the issue of overlapping QBSSs remains unresolved.

---
## abbreviation
#### IEEE 802.11 WLAN
- IEEE 802.11 WLAN: IEEE 802.11 wireless local area network

#### MAC (Medium Access Control)
- MAC: Medium Access Control
- DC F: Distributed Coordination Function
- CSMA: Carrier Sense Multiple Access
- MSDU Delivery: MAC Service Data Unit Delivery
- MPDU: MAC Protocol Data Unit
- CA: Collision Avoidance

#### Modulation and Transmission Techniques
- CCK: Complementary Code Keying
- DSSS: Direct Sequence Spread Spectrum
- OFDM: Orthogonal Frequency-Division Multiplexing

#### Quality of Service (QoS)
- QoS: Quality of Service
- QBSS: QoS Supporting BSS
- EDCA: Enhanced Distributed Channel Access
- HCCA: HCF Controlled Channel Access
- TXOP: Transmission Opportunity
- HCCA-TXOP: A TXOP Obtained by the HC via Controlled Medium Access
- ACs: Access Categories
- AIFS[AC]: Arbitration Interframe Space
- AIFSN[AC]: Arbitration Interframe Space Number

#### Network Components and Structures
- BSS: Basic Service Set
- AP: Access Point
- PC: Point Coordinator
- HC: Hybrid Coordinator

#### Interframe Spaces and Coordination
- DIFS: DCF Interframe Space
- SIFS: Short Interframe Space
- PIFS: PCF Interframe Space

#### Protocols and Control Frames
- ACK: Acknowledgment
- RTS/CTS: Request-to-Send/Clear-to-Send
- NAV: Network Allocation Vector

#### Periods and Timing
- CFP: Contention-Free Period
- CP: Contention Period
- TBTTs: Target Beacon Transmission Time

#### Coordination Functions
- HCF: Hybrid Coordination Function

#### Miscellaneous
- ISM: Industrial, Scientific, and Medical
- CW: Contention Window
- DLP: Direct Link Protocol

#### PHY (Physical Layer)
- PHY: Physical Layer

#### Statistical and Error Metrics
- CDFs: Complementary Cumulative Distribution Functions
- LRE: Limited-Relative-Error

