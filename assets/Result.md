

## Numerical Results 

 EDCA parameters used for each access category

| Access Category(AC)| AIFSN | CWmin |CWmax|
|----------          |-------|-------|------|
| AC_VO              | 2     |3      |7   |
| AC_VI              | 2     | 7     |15|
| AC_BE              | 3     | 15    |1023|
| AC_BK              | 7     | 15    |1023|
| High               | 2     | 7     | 7|
| Medium             | 4     | 10    |31|
| Low                | 7     | 31    |255|


we inteded to duplicate the result of the paper entitled "ANALYSIS OF IEEE 802.11E FOR QOS SUPPORT IN WIRELESS LANS". 

Thus, the numerical values of the simulation is illustrated as follow

| Offered Traffic (Mb/s) | Throughput AC_VO (Mb/s) | Throughput AC_VI (Mb/s) | Throughput AC_BE (Mb/s) | Throughput AC_BK (Mb/s) |
|------------------------|-------------------------|-------------------------|-------------------------|-------------------------|
| 0                      | 0                       | 0                       | 0                       | 0                       |
| 3                      | 3.5                     | 2.5                     | 2.0                     | 1.5                     |
| 6                      | 7.0                     | 5.5                     | 4.0                     | 3.0                     |
| 9                      | 12.0                    | 9.0                     | 7.0                     | 5.0                     |
| 12                     | 12.0                    | 8.5                     | 6.5                     | 4.5                     |
| 15                     | 12.0                    | 8.0                     | 6.0                     | 4.0                     |
| 18                     | 12.0                    | 7.5                     | 2.0                     | 2.0                     |
| 21                     | 12.0                    | 7.0                     | 1.0                     | 1.0                     |
| 24                     | 12.0                    | 6.5                     | 0.5                     | 0.5                     |
| 27                     | 12.0                    | 6.0                     | 0.5                     | 0.5                     |
| 30                     | 12.0                    | 5.5                     | 0.5                     | 0.5                     |


Throughput results per AC with increasing offered traffic per AC is shown below

- our result

![](Throughput%20vs%20Traffic.JPG)

- article result
  
![](Hc%20to%203sta.JPG)

Throughput result for each AC as the number of stations increases are illustrated below

 - our result

![](Increased%20Sta%20plot.JPG)

- Literature/article result
  
![](increase%20sta.JPG)

This, implies that there is a deviation between our simulation results and the findings in the article. This can be due to 

- Variations in simulation tools, and configurations can lead to different outcomes.



## References
 1. [S. Mangold, Sunghyun Choi, G. R. Hiertz, O. Klein and B. Walke, "Analysis of IEEE 802.11e for QoS support in wireless LANs," in IEEE Wireless Communications, vol. 10, no. 6, pp. 40-50,2003](https://ieeexplore.ieee.org/document/1265851)
 
