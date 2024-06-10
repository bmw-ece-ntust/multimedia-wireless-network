
# Assignment 3: IEEE 802.11e simmulation on NS-3

1. **Objective**
    
    The assignment aims to create a network simulation to evaluate the performance of IEEE 802.11e using the NS-3 simulator. The results obtained from the simulation will be compared to those presented in the original IEEE 802.11e research paper to validate and analyze the findings.


2. **Introduction**
    
    This project revolves around the study titled "Analysis of IEEE 802.11e for QoS Support in Wireless LANs." The primary goal is to assess the performance of the IEEE 802.11e protocol and compare it to its predecessor, IEEE 802.11. The IEEE 802.11e protocol enhances the MAC sublayer by integrating Quality of Service (QoS) functionalities into Wi-Fi networks.

    One of the key improvements introduced by IEEE 802.11e is the prioritization of data packets according to their type, known as Enhanced Distributed Channel Access (EDCA). This mechanism classifies traffic into different Access Categories (AC) - Voice, Video, Best Effort, and Background. Each category is assigned a specific priority level based on its significance and urgency, ensuring efficient data handling and better network performance.

    ![image](order.png)


    The goal of this project is to assess the effectiveness of these QoS mechanisms in real-world scenarios, such as network congestion, interference, and varying traffic loads. While various performance metrics like throughput, delay, jitter, and packet loss are crucial, this project specifically focuses on throughput. The simulation setup includes four stations connected to an Access Point, each station representing a different Access Category: Voice, Video, Best Effort, and Background.

    Through this approach, the project aims to validate the improvements introduced by IEEE 802.11e in practical settings, comparing the results with those documented in the reference paper.

3. **Simulation/Implementation**
    
    - **Configuration**
    
        - During the simulation configuration, I set up various essential parameters. These include specifying the port number, payload size, start and end times for traffic, and increment values for traffic. Additionally, I open the CSV output file to log the simulation results, ensuring all data is accurately recorded for analysis.
        
    - **Flowchart**

        ![image](flowchart.png)

    - **Code**


    This NS-3 simulation code sets up a wireless network with three stations (STAs) and one access point (AP) using the IEEE 802.11e standard. The main purpose is to measure throughput across different Access Categories (ACs) - Video (VI), Voice (VO), Best Effort (BE), and Background (BK) - under varying offered traffic loads.

    Key steps include:

    **Node Creation:** Create nodes for three STAs and one AP.
    **Channel and PHY Setup:** Configure the physical layer and channel settings.
    **WiFi and MAC Configuration:** Set up WiFi standards, QoS, and MAC parameters.
    **Network Device Installation:** Install WiFi devices on STAs and AP.
    **EDCA Configuration:** Set TXOP limits and other parameters for different ACs.
    **Mobility and Stack Installation:** Set node mobility and install the internet stack.
    **Address Assignment:** Assign IP addresses to devices.
    **Server and Client Setup:** Configure UDP servers and OnOff clients for each AC.
    **Simulation Execution:** Run the simulation for different traffic loads.
    **Throughput Calculation:** Calculate and log throughput for each AC from all stations.
    The throughput results are saved to an output file for analysis.
<details><summary>Click to expand/collapse</summary>
    
```cpp


```
</details>




4. **Results and Analysis**
    
    - Display the results and provide a brief analysis of what they indicate.
    
    
    My results:
    
    ![image](reference)
    
    Paper results:
    
    ![image](paper.png)
    
    
    The final graph presented here is the result of running the simulation and averaging the values. It aligns well with the general concepts proposed in the research paper. The Voice Access Category (AC_VO) consistently shows the highest throughput, followed by the Video Access Category (AC_VI), the Best Effort Access Category (AC_BE), and lastly, the Background Access Category (AC_BK), which sees a decrease in throughput as traffic increases.

    This project has significantly enhanced my understanding of how QoS policies manage network congestion. Prioritizing voice traffic (AC_VO) ensures it maintains high throughput even as the load increases, which is crucial due to our sensitivity to delays in audio. Video traffic (AC_VI) and Best Effort traffic (AC_BE) initially benefit from QoS mechanisms but start to degrade under heavier loads. Background traffic (AC_BK) is most affected, with its throughput declining as the traffic load rises.
    
    
    
5. **References**

- [Analysis of IEEE 802.11e for QoS support in wireless LANs](https://ieeexplore.ieee.org/stamp/stamp.jsp?tp=&arnumber=1265851)
- [Documentation NS-3](https://www.nsnam.org/documentation/)
- [NS-3 Installation Guide PDF](https://www.nsnam.org/docs/installation/ns-3-installation.pdf)
- [Udemy Course: Getting Started with NS-3](https://www.udemy.com/course/getting-started-with-network-simulator-3/?couponCode=ST8MT40924)

6. **Credits**
   
   Special thanks to:
   - Professor
   - TA
   - Classmates
     
This assignment wouldn't be possible without you all. The most important lesson I learned in this course is: to learn from the others! Quite important lesson, thank you all.
