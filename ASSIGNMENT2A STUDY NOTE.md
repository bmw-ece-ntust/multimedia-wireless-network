##  <center>Study note: WiFi 802.11 Simmulation using NS-3 </center>

Reference:
- [ns-3](https://www.nsnam.org)
- [Download NS-3](https://www.nsnam.org/releases/ns-allinone-3.38.tar.bz2)
- [ns-3 Tutorial](https://www.nsnam.org/docs/tutorial/html/)
- [The ns-3 Wifi Network Simulator](https://www.youtube.com/watch?v=qDseQLXtEKE&list=PLY_7h_846SGmOsy_5_wSeV4WMt9UNufyR&index=3)
- [ns-3 LTE module](https://www.nsnam.org/docs/models/html/lte-user.html)

## 1. Introduction

### 1.1 What is ns-3
ns-3 stands for Network Simulator 3,  is an open-source network simulator widely used for researching and simulating the behavior of computer networks and communication systems. It provides a platform for conducting network-related experiments and evaluations.

### A. Advantages of ns-3

- ***Accuracy:*** ns-3 is designed to offer high precision in network simulations, allowing for the modeling of various network protocols and behaviors with a high degree of accuracy.
- ***Extensibility:*** ns-3 provides a flexible, modular framework that enables users to customize and extend the simulator to meet specific research requirements.
- Active Community: ns-3 boasts an active user and developer community, providing support, documentation, and regular updates with new features.
- ***Support for Multiple Communication Layer Protocols:*** ns-3 supports the simulation of various network protocols, including TCP, UDP, IPv4, IPv6, as well as different communication technologies such as Wi-Fi, LTE, etc.

### B. Disadvantages of ns-3
- ***Steep Learning Curve:*** For beginners, ns-3 can have a steep learning curve due to its powerful but complex nature, which may require some time to master.
- ***Resource Intensive:*** Due to its highly accurate simulations, ns-3 often demands significant computational resources, which can limit its use in certain environments.
- ***Lack of Graphical Interface:*** ns-3 primarily operates as a command-line tool and lacks an intuitive graphical user interface, which might be inconvenient for some users.

## 2. ns-3 (3.38) installation Steps
### 2.1 My Computer Specification

Hardware requirement for ns-3：
RAM：8G RAM
CPU：6 core
Disk：40G Storage
Installation Environment：
Host：Windows 10
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

Now we need to install it by running the command 
Open a new terminal

$ cd ns-allinone-3.38/
$ ./build.py --enable-examples --enable-tests 

### 2.6 Running a Script

./ns3 run hello-simulator

### 2.7 Check the configure

cd ~/ns-3-allinone/ns-3.39/
./ns3 configure --build-profile=debug --enable-examples --enable-tests

![Screenshot from 2024-04-08 23-35-46](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/d623dc78-25b2-4d4f-b464-895f69db1906)


### 2.8 Check the ns3 version

./ns3 show version
![Screenshot from 2024-04-08 23-36-20](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/156c6900-de0f-44da-9986-d0ebb5deea68)


### 2.9 NetAnim tool

NetAnim is a tool used to visualize network simulation results, primarily for the ns-3 network simulation platform. It provides an intuitive interface for users to display various data and events in network simulations through animation and graphics, helping users better understand and analyze simulation results.

NetAnim has the following main features:

- Network Topology Display: NetAnim can display the topology structure of the network simulation, including nodes (such as routers, hosts, etc.), connections, and connection attributes.

- Animation Effects: NetAnim displays events such as packet transmission, node movement, connection establishment, and disconnection through animation effects, making simulation results more intuitive and vivid.

- Data Graph Display: NetAnim can display graphs of various data and statistical information in the simulation, such as packet transmission rates, node throughput, connection latency, etc.

- Event Analysis: NetAnim can be used to analyze various events and situations in the simulation, such as node movement paths, routing protocol operation, etc., helping users perform performance evaluation and troubleshooting.

***In summary***, NetAnim is a powerful tool that helps users visualize and analyze ns-3 network simulation results, allowing for better understanding of network behavior and performance characteristics.


**1. NetAnim tool install**
It is capable of animating simulations with PointToPoint links in ns-3.

```shell=
# NetAnim prerequisites
sudo apt-get -y install mercurial
sudo apt-get -y install qt5-default
sudo apt install -y qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools
```

**2. Downloading NetAnim**
```shell=
#Downloading NetAnim
git clone http://code.nsnam.org/netanim
```


**3. Building and Starting NetAnim**
NetAnim uses a QT build tool called qmake. Qt versions from 5.4 are supported. Please read the [#Prerequisites](https://www.nsnam.org/wiki/NetAnim_3.108#Prerequisites) before proceeding

```shell=
cd netanim
make clean
qmake NetAnim.pro
make
```

**4. Excute netanim**

./NetAnim



## 3. Use ns-3 to simulate WiFi model

### 3.1 Move the example model to  scratch directory

cd ~/workspace/ns-3-allinone/ns-3.39/examples/wireless/   #You can use pwd to find the scratch path
cp wifi-ap.cc /home/dieta/workspace/ns-3-allinone/ns-3.39/scratch/   #Copy file to scratch directory
cd ~/workspace/ns-3-allinone/ns-3.39/scratch/


### 3.2 Modify the code to support NetAnim tool

cd ~/workspace/ns-3-allinone/ns-3.39/examples/wireless/
vim wifi-ap.cc

#### 3.2.1 Include the netanim-moudle

#include "ns3/netanim-moudle.h"

#### 3.2.2 Save the simulated result in xml file

AnimationInterface anim ("WiFi-AP.xml");

### 3.3 Run the wifi-model 
shell=./ns    
cd ~/workspace/ns-3-allinone/ns-3.39/
./ ns3 run scratch/wifi-ap.cc

![Screenshot from 2024-04-09 00-19-33](https://github.com/bmw-ece-ntust/multimedia-wireless-network/assets/161825233/ef86a7e7-6e25-46dd-99f5-ade1cf6de46d)

## 4. Simulate the wifi-80211e-txop.cc

### 4.1 Network topology

### 4.2 Move the example model to  scratch directory
cd ~/ns-3-allinone/ns-3.39/examples/wireless/   #You can use pwd to find the scratch path
cp wifi-ap.cc /home/dieta/ns-3-allinone/ns-3.39/scratch/   #Copy file to scratch directory
cd ~/workspace/ns-3-allinone/ns-3.39/scratch/

### 4.3 Run wifi-80211e-txop.cc
 
 shell=./ns    
cd ~/workspace/ns-3-allinone/ns-3.39/
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
#### 4.4.2 Explanation

These data indicate that for the AC_BE Access Category, increasing the TXOP limit from the default value of 0ms to a non-default value of 4.096ms significantly improves throughput (from 28.8936 Mbit/s to 36.8012 Mbit/s). For the AC_VI Access Category, the default TXOP limit is 4.096ms, resulting in a stable throughput of 37.2122 Mbit/s. However, when reducing the TXOP limit to a non-default value of 0ms, the throughput decreases to 32.5206 Mbit/s.

Thus, these results demonstrate the impact of adjusting the TXOP limit on wireless network performance, which varies depending on the access category and other factors.
:::

### 4.3 Appendix

#### 4.3.1 Setup phy
```shell=
phy.Set("ChannelSettings", StringValue("{48, 20, BAND_5GHZ, 0}"));
```

:::info
which denotes that channel 48 is used on a 20 MHz channel in the 5GHz band, and because a larger channel width greater than 20 MHz is not being used, there is no need to indicate the primary 20 MHz channel so it is set to zero in the last argument
:::
#### 4.3.2 Setup number of STA & AP 
