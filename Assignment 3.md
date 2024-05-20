# <center> QoS in WLAN using IEEE 802.11e </center>

- [Introduction](#introduction)
- [Overview of IEEE 802.11e](#Overview of IEEE 802.11e)
- [Key Features of IEEE 802.11e][#Key Features of IEEE 802.11e]
    - [Enhanced Distributed Channel Access (EDCA)][#Enhanced Distributed Channel Access (EDCA)]
    - [HCF Controlled Channel Access (HCCA)][#HCF Controlled Channel Access (HCCA)]
- 
- [Conclusion](#Conclusion)


# Introduction

This assignment will present a detailed overview of the new attributes of a long term effective standard IEEE 802.11e to aid Quality of Service (QoS)
with Wireless Local area Networks (WLANs) is presented. I will address Medium Access Control (MAC) enhancements revealed in the present 802.11e draft standards by centering the problems with the legacy 802.11 standard. Cutting edge mechanisms for QoS assistance, specifically Enhanced Distributed Coordination Function (EDCF) and Hybrid Coordination Function (HCF), defined within the 802.11e drafts are usually evaluated. The effectiveness of new schemes is layed out via simulation outcomes

#  Overview of IEEE 802.11e
IEEE 802.11e, ratified in 2005, is designed to enhance the original 802.11 MAC (Medium Access Control) layer to support QoS. The standard introduces new features that prioritize different types of traffic, ensuring that time-sensitive data gets preferential treatment over less critical information. The primary enhancements in IEEE 802.11e include the Hybrid Coordination Function (HCF), which encompasses both the Enhanced Distributed Channel Access (EDCA) and HCF Controlled Channel Access (HCCA).

# Key Features of IEEE 802.11e

    1. ## Enhanced Distributed Channel Access (EDCA):
    Key parameters and characteristics of the Enhanced Distributed Channel Access (EDCA) mechanism in IEEE 802.11e:

    ![EDCA](image.png)

    Key mechanisms of EDCA include:

    Arbitration Interframe Space (AIFS): Different AIFS values for different ACs, allowing higher priority traffic to access the medium more quickly.
    Transmission Opportunity (TXOP): Specifies the maximum duration a station can transmit after gaining access to the medium, allowing for efficient bandwidth utilization.
    Contention Window (CW): Adjusts the contention window size based on the priority of the traffic, with higher priority traffic having a smaller contention window.

    

   ##  2. HCF Controlled Channel Access (HCCA):

    HCCA provides a contention-free access method, enabling deterministic and predictable transmission opportunities. This is achieved through a central coordinator, typically the access point (AP), which allocates transmission opportunities (TXOPs) to stations based on their QoS requirements. HCCA allows for the reservation of medium time, ensuring that high-priority traffic receives the necessary bandwidth and minimal delays.

    Key mechanisms of HCCA include:

    Controlled Access Periods (CAPs): Specific periods during which the AP can allocate TXOPs to stations, ensuring predictable service for time-sensitive applications.
    Traffic Specification (TSPEC): Defines the QoS parameters for a traffic stream, including data rate, delay bounds, and jitter requirements, allowing the AP to manage resources efficiently.
    Benefits of IEEE 802.11e
    The introduction of IEEE 802.11e brings several significant benefits to WLANs, particularly in terms of QoS:

1. Prioritization of Critical Traffic:
IEEE 802.11e ensures that high-priority traffic, such as voice and video, is transmitted with minimal delay and jitter, providing a smoother and more reliable user experience.

2. Improved Network Efficiency:
By utilizing mechanisms such as TXOP and AIFS, IEEE 802.11e optimizes the use of available bandwidth, reducing collisions and retransmissions, and thereby increasing overall network throughput.

3. Scalability and Flexibility:
The standard supports a wide range of applications with varying QoS requirements, from low-latency voice calls to high-throughput video streams, making WLANs more versatile and capable of handling diverse traffic loads.

4. Enhanced User Experience:
With better QoS management, users experience fewer interruptions and higher quality in multimedia applications, leading to increased satisfaction and productivity.

Implementation Considerations
To fully leverage the benefits of IEEE 802.11e, network administrators should consider the following implementation aspects:

Achievble EDCA Throughput with Increasing Number of Stations.

![alt text](image-1.png)

![alt text](image-2.png)

Hardware and Software Support: Ensure that both access points and client devices support IEEE 802.11e.
QoS Policies: Configure appropriate QoS policies and traffic prioritization rules based on the specific requirements of the network and its applications.
Regular Monitoring: Continuously monitor network performance and QoS metrics to identify and address any issues that may arise.
