%%
[[UPTP - Taiwan Tech]]
#studies/uptp-taiwantech/year3/multimedia-wireless-networks 
%%

by Victor Cano

## Introduction
The bases of the code I used were shown in my previous assignment, but I made substantial changes to optimize the code and add the QoS Implementations.

## Flow Chart
This flowchard shows a basic depiction of the functionality of the code
![[Pasted image 20240519005647.png]]

## Code
### Basics
My code works by creating the access point and stations, one for each Access Category, to be used to test the environment with all of them at once in a single simulation instance.

### QoS Setup
Following the values given in the paper the different AC values were set up  accordingly
![[Pasted image 20240518224759.png]]
### Traffic Offer tests
The simulation is set into its own function, using OfferedTraffic as a variable that then its used during the setup for all the respective clients for each AC. The paper shows a specific range for these tests, but I expanded it for a better representation while still maintaining the same overall shape in a bigger scale.

![[Pasted image 20240518225507.png]]

### Noise Reduction
The simulations usually provided very noisy results, which caused graphs that didn't properly represent the true average case:
![[Pasted image 20240518221936.png]]

To fix this issue I added a for loop to iterate over multiple simulations with the same offered traffic, and then average the results by dividing the aggregated result by the amount of iterations.
![[Pasted image 20240518225421.png]]

### Results
![[Pasted image 20240519001701.png]]
#### Analysis
Comparing it with the paper results:
![[Pasted image 20240518225713.png]]

We can see that the overall shape is pretty similar, but it's not exactly the same.
This can be due to fluctuations in the randomized results or small differences in code because of my low experience with the ns3 environment.

We can observe that as expected the Voice Access Category receives the highest priority, without limiting itself even at higher traffic.

The Video Access Category also has high priority but starts to be limited with the increase of traffic.

The Best Effort Access Category is the next one to decline after the traffic increases a little bit more. But it shows a great similariity to the AC_VA, showing a small inaccuracy.

The Background Access Category is the first to get limited to give higher priority to the other categories.
### Conclusion
I learned a lot about the way the QoS applied in 802.11e works to highly optimize the experience of the users connected in a network.

I also learned a lot about NS3 and how to use it with C++ to simulate networks as a way to create prototypes and test environments easily and fast.

This experiment proved useful for both learning about the topic at hand and the analysis of the paper to extract the most important details.