# Study Notes: NS3 for Simulating 802.11e MAC Performance
## HITAYEZU Antoine, D11202811

### Simulating the duplicates results for paper: ANALYSIS OF IEEE 802.11E FOR
### QOS SUPPORT IN WIRELESS LANS
### Cite: S. Mangold, Sunghyun Choi, G. R. Hiertz, O. Klein and B. Walke, "Analysis of IEEE 802.11e for QoS support in wireless LANs," 
### in IEEE Wireless Communications, vol. 10, no. 6, pp. 40-50, Dec. 2003, doi: 10.1109/MWC.2003.1265851.

#### Introduction:
•	Evaluate the performance of the 802.11e MAC for the 802.11a PHY at 5GHz (up to 54 Mb/s).\
•	Methodology: Event-driven stochastic simulations.\
•	Focus: Delay results of MSDU delivery using empirical complementary cumulative distribution functions (CDFs).\
•	Analysis: Utilization of the discrete Limited-Relative-Error (LRE) algorithm for measuring local correlations.\
#### Simulation Environment:
•	Radio Channel Error Model: Ensures realistic transmission conditions.\
•	Radio Channel Error Model: Ensures realistic transmission conditions.\
•	Transmission Powers and Distances: Prevent hidden node scenarios.\
•	Transmission Rates: Control frames at 6 Mb/s, data frames at 24 Mb/s.\
•	Traffic Model: Uniform mixture of offered traffic from four data streams (ACs).\
•	Traffic Characteristics: Avoid influence on results (e.g., correlated packet arrival).\
•	Backoff Parameters: MSDUs of 512 bytes with negative-exponentially distributed inter-arrival times.\
•	EDCA Parameters: Utilize values from Table 1 (AIFSN, CWmin, CWmax).\
•	RTS/CTS and Fragmentation: Not used.\
•	EDCA-TXOPs: Allow one data frame transmission after contention.\
•	Beacon Frames: Not generated.\
#### Network Configuration:
•	Three nodes and one access point (AP).\
•	Configuration based on EDCA parameter sets for each AC.\
•	HC (Hybrid Coordinator) defines parameter values and announces changes via beacon frames.\
•	Each backoff entity within a station contends for a TXOP independently.\
•	AIFSN determines the duration of arbitration interframe space (AIFS).\
•	CWmin determines the minimum size of the contention window.\
•	CWmax limits the maximum size of the contention window.\
•	Retry counters limit the number of retransmissions.\
•	Maximum MSDU lifetime per AC ensures timely frame transmission.\
•	TXOPlimit defines the maximum duration of an EDCA-TXOP.\

![ADCA](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/83ec3142-0341-4dbe-afe3-2e08e0efac57)




## Backoff Entity Management:
•	Each station contains four backoff entities with different EDCA parameter sets.\
•	Virtual collisions occur when multiple backoff entities contend simultaneously.\
•	Priority-based transmission resolves collisions.\
•	Table 1 provides recommended default values for EDCA parameter sets.\
•	HC can dynamically adjust parameter values based on network conditions.\
## Figure 1. Throughput per AC with increasing offered traffic per AC for the illustrated scenario.\

![FIG1](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/51d93600-933b-4e07-9c35-ed76a265885f)



	
## Figure 2. Throughput per AC with increasing number of stations, and constant offered traffic per station for the illustrated scenario.

                          

![FIG2](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/d82467bd-b401-45d2-adf5-8e2974418d1e)


	
## Conclusion: 
•	NS3 simulation to evaluate 802.11e MAC performance under various conditions and Emphasis on accuracy \
and reliability through empirical data analysis and adherence to standardized parameters.\
•	Enables understanding of medium access behavior and collision resolution in 802.11e networks.\
## References:
[1]. https://www.nsnam.org/docs/release/3.41/tutorial/html/building-topologies.html. \
[2]. https://www.nsnam.org/docs/release/3.41/tutorial/html/index.html. \
[3]. S. Mangold, Sunghyun Choi, G. R. Hiertz, O. Klein and B. Walke, "Analysis of IEEE 802.11e for QoS support in wireless LANs,"\
in IEEE Wireless Communications, vol. 10, no. 6, pp. 40-50, Dec. 2003, doi: 10.1109/MWC.2003.1265851.
