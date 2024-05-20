# <center>Assignment 3</center>

## 1. Introduction
This project uses the ns-3 network simulator to simulate the Quality of Service (QoS) mechanism in an IEEE 802.11e wireless network. The main objective is to evaluate the throughput performance of various Access Categories (ACs) under different network traffic loads.

The simulation sets up one Access Point (AP) and four Stations (STAs), each representing a different AC:

- AC_VO (Voice): Voice traffic
- AC_VI (Video): Video traffic
- AC_BE (Best Effort): Best effort traffic
- AC_BK (Background): Background traffic

## 2. Flow Chart

<center>
    
![ttt.drawio](https://hackmd.io/_uploads/SkN63TuQ0.png)
</center>

This flowchart details the major steps in the program, including setting simulation parameters, setting up the WiFi network, configuring application traffic, running and collecting simulation results, calculating throughput, and outputting results. 

## 3. Simulation
The program configures the AIFS (Arbitration Inter-Frame Space), CWmin, and CWmax values for each AC based on the EDCA (Enhanced Distributed Channel Access) parameters to provide differentiated service quality.

During the simulation, the AP generates UDP traffic for all four ACs and gradually increases the total traffic load (from trafficStart to trafficEnd, with an increment of trafficIncrement). Each simulation round records the throughput for each AC and writes the results to a CSV file.

The program runs multiple simulations (numSimulations) to obtain average results and finally writes the average throughput to the "average.csv" file.

Through this simulation, we can observe how the IEEE 802.11e QoS mechanism affects the throughput performance of different traffic types under various network loads. This allows us to evaluate the effectiveness of EDCA parameter settings and provides insights for designing QoS in Wi-Fi networks.

**EDCA parameter sets :**

![Screenshot 2024-05-20 at 6.23.52 PM](https://hackmd.io/_uploads/HyWeSsdm0.png)

### 3-1. Scenario 1

![Screenshot 2024-05-20 at 12.10.38 AM](https://hackmd.io/_uploads/r168GiumR.png)

The simulation was run a total of 50 times, and the results of each run were averaged. For example, if we simulated five times, we would have results like this:

Simulation 1 Current Traffic: 1 Mbps Throughput (AC_VO): 0.541696 Mbps
Simulation 2 Current Traffic: 1 Mbps Throughput (AC_VO): 0.494592 Mbps
Simulation 3 Current Traffic: 1 Mbps Throughput (AC_VO): 0.443563 Mbps
...
We would then average the results for each traffic rate, such as 1 Mbps, and export the averaged results to a new file named average.csv. This method increases the accuracy of the results.

**EDCA parameter sets :**

```javascript=
// Configure EDCA for AC_VI (Video)
wifi_mac->GetAttribute("VI_Txop", ptr);
edca = ptr.Get<QosTxop>();
edca->SetAifsn(2);
edca->SetMinCw(7);
edca->SetMaxCw(15);

// Configure EDCA for AC_VO (Voice)
wifi_mac->GetAttribute("VO_Txop", ptr);
edca = ptr.Get<QosTxop>();
edca->SetAifsn(2);
edca->SetMinCw(3);
edca->SetMaxCw(7);

// Configure EDCA for AC_BE (Best Effort)
wifi_mac->GetAttribute("BE_Txop", ptr);
edca = ptr.Get<QosTxop>();
edca->SetAifsn(3);
edca->SetMinCw(15);
edca->SetMaxCw(1023);

// Configure EDCA for AC_BK (Background)
wifi_mac->GetAttribute("BK_Txop", ptr);
edca = ptr.Get<QosTxop>();
edca->SetAifsn(7);
edca->SetMinCw(15);
edca->SetMaxCw(1023);
```

**Calculate average throughput and write to average.csv**

```javascript=
std::ofstream outFileCSV("average.csv");
if (!outFileCSV.is_open()) {
    NS_LOG_ERROR("Unable to open output file.");
    return 1;
}

for (int i = 0; i <= iterations; i++)
{
    double currentTrafficAC = trafficStart + (i * trafficIncrement);
    outFileCSV << currentTrafficAC;

    for (int j = 0; j < 4; j++) {
        double avgThroughput = throughputResults[i][j] / static_cast<double>(numSimulations);
        outFileCSV << "," << avgThroughput;
    }
    outFileCSV << "\n";
}

outFileCSV.close();
std::cout << "Average results saved to average.csv" << std::endl;
```

### 3-1-1. Simulate 50/10/1 times :  

```javascript=
int numSimulations = 50;
```

Import the output data into Python and display the results.

```javascript=
import matplotlib.pyplot as plt
import numpy as np

# Data
traffic_load = [0, 1, 4, 7, 10, 13, 16, 19, 22, 25, 28, 31]
vo_throughput = [0, 0.48996, 1.99807, 3.54513, 4.89795, 6.43354, 8.00807, 9.42465, 10.8079, 12.4186, 13.6761, 15.2246]
vi_throughput = [0, 0.495613, 2.0144, 3.44315, 4.98525, 6.34444, 8.06263, 9.54178, 11.2008, 12.5169, 14.1095, 15.4735]
be_throughput = [0, 0.497732, 1.98881, 3.48546, 4.99028, 6.46424, 7.81479, 8.75436, 7.97588, 6.48591, 4.60975, 3.15181]
bk_throughput = [0, 0.491216, 1.9779, 3.45736, 5.02537, 6.30982, 5.71167, 3.42658, 2.48803, 1.83478, 1.44672, 1.03833]

# Calculate SMA
window_size = 3  # Moving average window size
vo_throughput_sma = np.convolve(vo_throughput, np.ones(window_size) / window_size, mode='valid')
vi_throughput_sma = np.convolve(vi_throughput, np.ones(window_size) / window_size, mode='valid')
be_throughput_sma = np.convolve(be_throughput, np.ones(window_size) / window_size, mode='valid')
bk_throughput_sma = np.convolve(bk_throughput, np.ones(window_size) / window_size, mode='valid')

# Plot the graph
plt.figure(figsize=(8, 6))
plt.plot(traffic_load[:len(vo_throughput_sma)], vo_throughput_sma, marker='o', linestyle='-', label='AC_VO (voice)')
plt.plot(traffic_load[:len(vi_throughput_sma)], vi_throughput_sma, marker='x', linestyle='-', label='AC_VI (video)')
plt.plot(traffic_load[:len(be_throughput_sma)], be_throughput_sma, marker='D', linestyle='-', label='AC_BE (best effort)')
plt.plot(traffic_load[:len(bk_throughput_sma)], bk_throughput_sma, marker='o', linestyle='-', label='AC_BK (background)')

# Add title, axis labels, and legend
plt.title('Throughput per AC (3 streams each), Simulation run 50 times', fontsize=14)
plt.xlabel('Offered traffic per AC (3 streams each) (Mb/s)', fontsize=12)
plt.ylabel('Throughput per AC (3 streams each) (Mb/s)', fontsize=12)
plt.legend(fontsize=10)

# Adjust axis ranges
plt.xlim(0, 31)
plt.ylim(0, 16)

# Set grid
plt.grid(True)

# Display the plot
plt.tight_layout()
plt.show()
```
**Result :**

![Screenshot 2024-05-20 at 8.25.05 PM](https://hackmd.io/_uploads/Hy8xVpumR.png)
![Screenshot 2024-05-20 at 8.34.22 PM](https://hackmd.io/_uploads/HyIx46OXC.png)
![Screenshot 2024-05-20 at 8.25.25 PM](https://hackmd.io/_uploads/Sy8gVpu7A.png)

## 4. Analysis

From my simulation results, we can see that as the offered traffic per Access Category (AC) increases, the throughput of each AC also increases accordingly. However, ACs with higher priority (such as AC_VO) can achieve higher throughput, while ACs with lower priority (such as AC_BE and AC_BK) are significantly limited and their throughput remains low. This reflects the protective effect of the 802.11e EDCA mechanism on high-priority services.

**There are a few differences compare to the paper:**
- In my simulation results, the throughput of AC_VO drops significantly under high offered traffic, while in the figure from the paper, AC_VO maintains a high level throughout.
- When the offered traffic is low, the throughput of AC_BK in my simulation is slightly higher than the results in the paper. As the offered traffic increases, AC_BK is more obviously limited and lower than the results in the paper.
- The throughput curves of AC_VI and AC_BE are closer together and less distinguishable than in Figure 8 of the paper.