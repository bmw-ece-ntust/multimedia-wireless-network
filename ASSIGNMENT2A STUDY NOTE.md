##  <center>Study note: Simmulation for WiFi 802.11 By using NS-3 </center>

Reference:
- [ns-3 information](https://www.nsnam.org)
- [Download NS-3](https://www.nsnam.org/releases/ns-allinone-3.38.tar.bz2)
- [ns-3 Tutorial](https://www.nsnam.org/docs/tutorial/html/)
- [The review on ns-3 Wifi Network Simulator](https://www.youtube.com/watch?v=qDseQLXtEKE&list=PLY_7h_846SGmOsy_5_wSeV4WMt9UNufyR&index=3)
- [The review on ns-3 LTE module](https://www.nsnam.org/docs/models/html/lte-user.html)

## 1. Introduction

### 1.1 About ns-3
ns-3, short for Network Simulator 3, serves as an open-source tool for simulating computer networks and communication systems. It's extensively utilized in research to model and analyze network behaviors, offering a versatile platform for conducting experiments and assessments in the field of networking.

### A. Advantages of ns-3

**Precision:** ns-3 is engineered to ensure precise network simulations, facilitating the detailed modeling of diverse network protocols and behaviors with high accuracy.

**Flexibility:** ns-3 offers a versatile and modular framework, empowering users to adapt and expand the simulator according to specific research needs.

**Vibrant Community:** ns-3 benefits from an engaged community of users and developers who provide assistance, comprehensive documentation, and regular updates featuring new functionalities.

**Diverse Protocol Support**: ns-3 enables the simulation of a wide array of network protocols, encompassing TCP, UDP, IPv4, IPv6, and various communication technologies like Wi-Fi, LTE, among others.

### B. Drawback of ns-3
**Absence of Graphical Interface:** ns-3 predominantly functions through command-line interaction and does not offer a user-friendly graphical interface, which could pose inconvenience for certain users.
**Challenging Learning Process:** Novices may find ns-3 daunting to grasp initially because of its robust yet intricate design, necessitating a period of learning and practice to become proficient.
**Resource Demands:** The precise simulations conducted by ns-3 often require substantial computational power, potentially restricting its applicability in environments with limited resources.

## 2. ns-3 (3.38) installation Steps
### 2.1 My Computer Specification

Hardware requirement for ns-3：
RAM：8G RAM
CPU：6 core
Disk：40G Storage
Installation Environment：
Host：Windows 10; 
Hypervisor：VMware Workstation 16 Player
VM/os：Ubuntu 20.04 LTS (Focal Fossa)


### 2.2 Install the prerequisite tools in my linux/ubuntu PC

$ sudo apt update
$ sudo apt install g++ python3 cmake ninja-build git gir1.2-goocanvas-2.0 python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython3 tcpdump wireshark sqlite sqlite3 libsqlite3-dev qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools openmpi-bin openmpi-common openmpi-doc libopenmpi-dev doxygen graphviz imagemagick python3-sphinx dia imagemagick texlive dvipng latexmk texlive-extra-utils texlive-latex-extra texlive-font-utils libeigen3-dev gsl-bin libgsl-dev libgslcblas0 libxml2 libxml2-dev libgtk-3-dev lxc-utils lxc-templates vtun uml-utilities ebtables bridge-utils libxml2 libxml2-dev libboost-all-dev 
## All these packages are not compulsorily needed

### 2.3 Check my g++ and python version

python3 -V
g++ -v

| Tool | version | 
| -------- | -------- 
|      g++    |   3.8.10      |
|python   |  9.4.0  |

### 2.4 Downloading ns-3 
[Download NS-3](https://www.nsnam.org/releases/ns-allinone-3.38.tar.bz2)
Now unzip or untar the ns-allinone-3.38.tar.bz2 to the home folder

### 2.5 Building ns-3 with build.py

*Now we need to install it by running the command 
Open a new terminal*

$ cd ns-allinone-3.38/
$ ./build.py --enable-examples --enable-tests 

### 2.6 Running a Script

./ns3 run hello-simulator

### 2.7 Check the configure

cd ~/ns-3-allinone/ns-3.38/
./ns3 configure --build-profile=debug --enable-examples --enable-tests

![Screenshot from 2024-04-08 23-35-46](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/d623dc78-25b2-4d4f-b464-895f69db1906)


### 2.8 Check the ns3 version

./ns3 show version
![Screenshot from 2024-04-08 23-36-20](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/156c6900-de0f-44da-9986-d0ebb5deea68)


### 2.9 We need NetAnim tool

NetAnim serves as a visualization tool primarily utilized with the ns-3 network simulation platform. It offers an intuitive interface for users to present diverse data and events from network simulations through animation and graphics, facilitating improved comprehension and analysis of the results.

**Key features of NetAnim include:**

   **Network Topology Display:** NetAnim showcases the structural layout of the network simulation, encompassing nodes (such as routers, hosts, etc.), connections, and their attributes.

  **Animation Effects:** Events like packet transmission, node movement, connection establishment, and disconnection are
    visualized through animation effects, enhancing the clarity and vividness of simulation outcomes.

   **Data Graph Display:** NetAnim can generate graphs illustrating various data and statistical metrics within the simulation, such as packet transmission rates, node throughput, and connection latency.

   **Event Analysis:** It facilitates the analysis of different events and scenarios within the simulation, such as node movement paths and routing protocol operations, aiding users in performance evaluation and troubleshooting endeavors.

In essence, NetAnim stands as a robust tool empowering users to visualize and scrutinize ns-3 network simulation results, thereby fostering a deeper understanding of network behaviors and performance attributes.


**1. NetAnim tool install**
It is capable of animating simulations with PointToPoint links in ns-3.

# NetAnim prerequisites
sudo apt-get -y install mercurial
sudo apt-get -y install qt5-default
sudo apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools.

# Downloading NetAnim
git clone http://code.nsnam.org/netanim

**3. Building and Starting NetAnim**
NetAnim uses a QT build tool called qmake. Qt versions from 5.4 are supported. Please read the [#Prerequisites](https://www.nsnam.org/wiki/NetAnim_3.108#Prerequisites) before proceeding

cd netanim
make clean
qmake NetAnim.pro
make

**4. Excute netanim**

./NetAnim

## 3. Use ns-3 to simulate WiFi model

### 3.1 Move the example model to  scratch directory

cd ~/dieta/download/ns-3-allinone/ns-3.39/examples/wireless/   #You can use pwd to find the scratch path
cp wifi-ap.cc /home/dieta/ns-3-allinone/ns-3.38/scratch/   #Copy file to scratch directory
cd ~/dieta/home/ns-3-allinone/ns-3.38/scratch/


### 3.2 Modify the code to support NetAnim tool

cd ~/dieta/home/ns-3-allinone/ns-3.38/examples/wireless/
vim wifi-ap.cc

#### 3.2.1 Include the netanim-moudle

#include "ns3/netanim-moudle.h"

#### 3.2.2 Save the simulated result in xml file

AnimationInterface anim ("WiFi-AP.xml");

### 3.3 Run the wifi-model 
shell=./ns    
cd ~/dieta/home/ns-3-allinone/ns-3.38/
./ ns3 run scratch/wifi-ap.cc

![Screenshot from 2024-04-09 00-19-33](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/ef86a7e7-6e25-46dd-99f5-ade1cf6de46d)

## REFERENCE 
Mr Joe study note
## Acknoledgement
with much Appreciation, i thanks Mr JOE FOR his great support.
Much Appreciation To Professor Ray and TA Ian for more support.




## 4. Simulate the wifi-80211e-txop.cc(Pending task)

### 4.1 Network topology

### 4.2 Move the example model to  scratch directory
cd ~/ns-3-allinone/ns-3.38/examples/wireless/   #You can use pwd to find the scratch path
cp wifi-ap.cc /home/dieta/ns-3-allinone/ns-3.39/scratch/   #Copy file to scratch directory
cd ~/dieta/home/ns-3-allinone/ns-3.38/scratch/

### 4.3 Run wifi-80211e-txop.cc
 
 shell=./ns    
cd ~/dieta/home/ns-3-allinone/ns-3.38/
./ ns3 run scratch/wifi-80211e-txop.cc

### 4.4 Test result
#### 4.4.1 Data

AC_BE (Best Effort) Access Category:

    With default TXOP limit (0ms):

    Throughput = 28.8936 Mbit/s
    With non-default TXOP limit (4.096ms):
    
    Throughput = 36.8012 Mbit/s
    Maximum TXOP duration = 4056 microseconds (µs)

:::info


AC_VI (Video) Access Category:

    With default TXOP limit (4.096ms):

    Throughput = 37.2122 Mbit/s
    Maximum TXOP duration = 4056 microseconds (µs)
    With non-default TXOP limit (0ms):

    Throughput = 32.5206
#### 4.4.2 Explanation about above result

These findings indicate that adjusting the TXOP (Transmission Opportunity) limit has a notable impact on wireless network performance. Specifically, for the AC_BE (Best Effort) Access Category, increasing the TXOP limit from its default value of 0ms to a non-default value of 4.096ms leads to a significant enhancement in throughput, from 28.8936 Mbit/s to 36.8012 Mbit/s. Conversely, for the AC_VI (Video) Access Category, maintaining the default TXOP limit of 4.096ms results in a consistent throughput of 37.2122 Mbit/s. However, reducing the TXOP limit to a non-default value of 0ms causes a decrease in throughput to 32.5206 Mbit/s.

Thus, these results underscore the importance of TXOP limit adjustments in influencing wireless network performance, with outcomes varying across access categories and other influencing factors.

### 4.3 Appendix

#### 4.3.1 Setup phy

phy.Set("ChannelSettings", StringValue("{48, 20, BAND_5GHZ, 0}"));

which denotes that channel 48 is used on a 20 MHz channel in the 5GHz band, and because a larger channel width greater than 20 MHz is not being used, there is no need to indicate the primary 20 MHz channel so it is set to zero in the last argument

