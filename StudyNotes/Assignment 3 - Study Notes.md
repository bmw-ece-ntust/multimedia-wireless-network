# Assignment 3: NS3 802.11e Simulation

## Introduction
The bases of the code I used were shown in my previous assignment, but I made substantial changes to optimize the code and add the QoS Implementations.

## Flow Chart
This flowchard shows a basic depiction of the functionality of the code
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/74629530-c948-4605-a8f6-522a5a926114)

## Code
### Basics
My code works by creating the access point and stations, one for each Access Category, to be used to test the environment with all of them at once in a single simulation instance.

### QoS Setup
Following the values given in the paper the different AC values were set up  accordingly
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/fd24c2ed-7763-479a-b788-22b10fed2cfe)

### Traffic Offer tests
The simulation is set into its own function, using OfferedTraffic as a variable that then its used during the setup for all the respective clients for each AC. The paper shows a specific range for these tests, but I expanded it for a better representation while still maintaining the same overall shape in a bigger scale.
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/52575824-e44b-4bd6-b3b3-fe88ab98fd2a)

### Noise Reduction
The simulations usually provided very noisy results, which caused graphs that didn't properly represent the true average case:
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/0fdb324a-cf10-4cb3-a60d-0945e2d36ffb)

To fix this issue I added a for loop to iterate over multiple simulations with the same offered traffic, and then average the results by dividing the aggregated result by the amount of iterations.
![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/0fe6de5a-6ad3-4372-977f-8c2bc47234ec)


### Results

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/e3fe5467-0b46-41cd-83d7-f6d6b9438c33)

#### Analysis
Comparing it with the paper results:

![image](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/137210756/59b529f3-19c5-4210-82bd-a0ecb909c3f0)

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
