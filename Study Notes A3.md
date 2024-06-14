# <center> Assignment 3 </center>

1. [Purpose](#Purpose)
2. [Paper analysis](#Paper-analysis)
3. [Simulation](#Simulation)

#  Purpose
Learn to identify the problem and verify the concepts using simulation and analytical results.

#Paper Analysis
The study examines the IEEE 802.11e standard, which improves the traditional 802.11 MAC protocol to enable quality of service (QoS) in wireless LANs. 
The primary drawbacks of QoS support in legacy 802.11 are examined, including a lack of service distinction and unexpected delays.
The foundation of 802.11e is the Hybrid Coordination Function (HCF), which defines two modes of operation.

Enhanced Distributed Channel Access (EDCA) is a contention-based channel access strategy in which numerous backoff entities per station compete for different priority depending on access category (AC) characteristics such as AIFSN, CWmin, and CWmax.
HCF Controlled Channel Access (HCCA) is a controlled/polled access managed by a Hybrid Coordinator (HC) that may assign transmission opportunities (TXOPs) to stations.

Key EDCA features like as virtual collisions, TXOP continuations, and AC-specific EDCA parameter sets that allow for service differentiation are discussed. The HCCA technique, which provides parameterized QoS by restricting TXOP durations and polling stations, is also discussed.
The research assesses 802.11e's performance using simulations for a variety of situations, including isolated QBSSs, increasing the number of contesting stations, prioritized HC access, and overlapping QBSSs. The results reveal that EDCA offers varied throughputs, but HCCA can limit delays for the highest priority traffic. However, the lack of coordination across overlapping QBSSs is identified as an outstanding issue.
Other advancements, such as block acknowledgments for efficiency and the Direct Link Protocol for direct station-to-station transfers, are briefly discussed.

#Simulation

My decision to work with C++ for ns-3 simulations rather than Python was purposeful, motivated by my prior experience with C++. This option enables me to use my knowledge and expertise with C++'s grammar, structure, and performance characteristics to the ns-3 framework. 

The network topology is detailed below
<p align="center">
<img src=/assets/assigment 3 (2).jpg>

Example of CSV file generated:

| Current Traffic (Mbps) | Throughput AC_VO (Mbps) | Throughput AC_VI (Mbps) | Throughput AC_BE (Mbps) | Throughput AC_BK (Mbps) |
|------------------------|-------------------------|-------------------------|-------------------------|-------------------------|
| 1                      | 0.338667                | 0.229333                | 0.338667                | 0.338667                |
| 4                      | 1.35733                 | 1.35733                 | 1.35733                 | 1.35733                 |
| 7                      | 2.37467                 | 2.372                   | 2.37467                 | 1.58667                 |
| 10                     | 2.46133                 | 2.45733                 | 2.45067                 | 2.45467                 |
| 13                     | 2.45067                 | 2.45467                 | 2.448                   | 2.448                   |
| 16                     | 2.45733                 | 2.456                   | 2.444                   | 2.46133                 |
| 19                     | 2.832                   | 2.844                   | 1.30667                 | 2.844                   |
| 22                     | 2.46533                 | 2.45733                 | 2.436                   | 2.46267                 |
| 25                     | 2.472                   | 2.43733                 | 2.43733                 | 2.46667                 |
| 28                     | 2.82667                 | 2.84133                 | 2.824                   | 1.30267                 |
| 31                     | 2.44133                 | 2.46267                 | 2.476                   | 2.44933                 |

