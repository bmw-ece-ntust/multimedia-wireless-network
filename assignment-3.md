# Assigment 3 - IEEE 802.11e simmulation on NS-3
- **Name:** Emiliano Nazareno Rojas Marcelli
- **ID:** F11215120

## Outline
- Objective
- Introduction
- Simulation
  - Topology
  - Flow chart
  - Code
  - Output  
- Results
- Analysis 
- References
---

## Objective
- Create a network simulation where we evaluate 802.11e using NS-3.
- Obtain the results from the paper IEEE 802.11e by simulating on NS-3.

## Introduction
For this project the studied paper was [Analysisi of IEE 802.11e for QoS Support in Wireless LANs] , which goal is to evaluate the protocol 802.11e and compare it with its legacy 802.11. The protocol 802.11e is an enhancement at the MAC sublayer 
which adds QoS functionalities to Wi-Fi networks. Now the question is, how does 802.11e do this enhancement? The enhancement is done by applying the following supports:
1) Prioritize Data packets based on their types ( EDCA/AC ).
2) Allowing end stations to communicate their QoS requirements an Access Point ( AP ).

Regarding the support **number 1)**, which introduce a mechanism for prioritize different types of traffics that is called "Enhanced Distributed Channel Access" (EDCA) and define the concept of Access Category (AC), that show a way to prioritize
different types of traffic based on their importance and urgency. The traffics proposed for the study were Access Category Voice, Video, Best effort and Background. The mentioned traffics follow a herarchy of importance and urgency based
on the following image as reference:  

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/6a035053-4ba4-4762-b754-488cf2d4fce3)

By following the apporach taken by the paper of study, it tries to check the effectiveness of the mentioned mechanisms but in real world scenarios ( Networking cogestion, Interference and trafficloads variation ) by using Throughput, delay, jitter and packet loss as parameters. 
For simplification and objective purpose, the parameter to be taken would be the througphut by applying a variation in traffic. The project will simulate 4 stations connected to an Acces Point, where each of the station will rerpesent a different Acces Category (Voice, Video, best effort and background).

## Simulation

### Topology 
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/128239705/53199fc3-945c-4267-be72-4a5ee4c53320)

Reference of the used topology.

**Characteristics:**
- 1 Access Point and 4 Stations
- Each Stations represents an Access Category (Voice, Video, BE = Best Effort, BK = Background)
### Flow chart

### Code

### Output  
## Results
## Analysis 
## References



 Problem
 Importance
 Challenge

# Strong evidence


Purpose: Learn to identify the problem and verify the concepts using simulation and analytical results.
Topics: IEEE 802.11e simmulation on NS-3
References: TBD.
Score (15%):
Study note: (Up to 6 pt):
Topic & Plan Presentation: 
 (1pt)

Numercial Results (should be identical as the paper): (2 pt)



Report Script

Objectives



Introduction

Code
#Explain scopes

Conclusion
