- [Introduction](#Introduction)
- [Install ns-3](#Install-ns-3)


# Introduction
## Ns-3
ns-3 is a discrete-event simulator typically run from the command line. It is written directly in C++, not in a high-level modeling language; simulation events are simply C++ function calls, organized by a scheduler.
# Install ns-3
## Installation of NS3 (ns-3.36.1)
**References:** [Installing ns-3 in Ubuntu 22.04](https://www.nsnam.com/2022/06/ns3-installation-in-ubuntu-2204.html)

Tools used in this simulation:
NS3 version ns-3.36.1 
OS Used: Ubuntu 22.04 LTS
Open terminal and follow the steps

### Step 1: Prerequisites

First run this line of code

``$ sudo apt update``

Next install all these packages for the complete use of ns3.

``$ sudo apt install g++ python3 python3-dev pkg-config sqlite3 cmake python3-setuptools git qtbase5-dev qtchooser qt5-qmake qtbase5-dev-tools gir1.2-goocanvas-2.0 python3-gi python3-gi-cairo python3-pygraphviz gir1.2-gtk-3.0 ipython3 openmpi-bin openmpi-common openmpi-doc libopenmpi-dev autoconf cvs bzr unrar gsl-bin libgsl-dev libgslcblas0 wireshark tcpdump sqlite sqlite3 libsqlite3-dev  libxml2 libxml2-dev libc6-dev libc6-dev-i386 libclang-dev llvm-dev automake python3-pip libxml2 libxml2-dev libboost-all-dev``

### Step 2: Download ns-allinone-3.36.1.tar.bz2 from the website nsnam.org. 
[Download here](https://www.nsnam.org/releases/ns-allinone-3.36.1.tar.bz2)
### Step 3:
Unzip the above file content to the home folder. Check your home folder and do it accordingly. 
To unzip use the GUI with click right to extract and then select /home/your user name/folder. 
Otherwise you can use the command
`` $ tar jxvf ns-allinone-3.36.1.tar.bz2 ``
### Step 4: 
`` $ cd ns-allinone-3.36.1/ 
$ ./build.py --enable-examples --enable-tests``

Once the installation is done. You can run the example as shown
``$ cd ns-3.36.1/
$ ./ns3 run hello-simulator``

In your screen you should see

Hello Simulator
