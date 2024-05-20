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
