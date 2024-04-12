# A2 Study Notes


<!-- vim-markdown-toc Redcarpet -->

* [MWN: NS3 WiFi Simulation](#mwn-ns3-wifi-simulation)
    * [1 Prerequsites](#1-prerequsites)
        * [1.1 Paper Analysis](#1-1-paper-analysis)
            * [1.1.1 Compare 802.11 vs 802.11e](#1-1-1-compare-802-11-vs-802-11e)
            * [1.1.2 Unsolved problem of PCF on 802.11](#1-1-2-unsolved-problem-of-pcf-on-802-11)
            * [1.1.3 802.11e Enhancment](#1-1-3-802-11e-enhancment)
        * [1.2 NS3](#1-2-ns3)
            * [1.2.1 NS3 Docker Build & Installation](#1-2-1-ns3-docker-build-amp-installation)
            * [1.2.2 NS3 How to Use](#1-2-2-ns3-how-to-use)
    * [2.1 Evaluation](#2-1-evaluation)
        * [2.1.1 Scenario 1: Throughput per AC](#2-1-1-scenario-1-throughput-per-ac)
    * [NS3 WiFi 802.11a Simulation](#ns3-wifi-802-11a-simulation)
        * [Introduction](#introduction)
        * [Simulation Setup](#simulation-setup)
        * [Simulations Result](#simulations-result)
        * [Analysis](#analysis)

<!-- vim-markdown-toc -->

# MWN: NS3 WiFi Simulation

## 1 Prerequsites

### 1.1 Paper Analysis

The reference of this simulation will be taken from **Analysis of IEEE 802.11e for QoS support in wireless LANs**

> Published in: IEEE Wireless Communications ( Volume: 10, Issue: 6, December 2003) 

#### 1.1.1 Compare 802.11 vs 802.11e

1. **802.11**
   - Distributed Approach MAC
	- Best Effort
2. **802.11e**
	- Contention based MAC
	- Contention free MAC

#### 1.1.2 Unsolved problem of PCF on 802.11

1. Unpredictable beacons delays
2. Uknown transmission duration of the polled stations

#### 1.1.3 802.11e Enhancment

1. Hybrid Coordination Function
2. Enhanced Distribution Channel Access (EDCA)

### 1.2 NS3

To simulate NS3 on a Podman container, you simply need NS3 version 3.38 and a Podman environment set up on your system. By running NS3 within the Podman container, you can efficiently execute powerful network simulations without unnecessary complications.

> [!IMPORTANT] 
> *I used podman on my machine your might be docker. Its equivalent, don't get confused.*

#### 1.2.1 NS3 Docker Build & Installation

1. Build docker using the docker file bellow

    > Dockerfile
    > ```docker=
    > FROM ubuntu:22.04
    > 
    > ARG DEBIAN_FRONTEND=noninteractive
    > RUN apt-get update && \
    >     apt-get install -y \
    >         g++ \
    >         python3 \
    >         cmake \
    >         ninja-build \
    >         git \
    >         gir1.2-goocanvas-2.0 \
    >         python3-pip \
    >         net-tools \
    >         python3-gi \
    >         python3-gi-cairo \
    >         python3-pygraphviz \
    >         gir1.2-gtk-3.0 \
    >         ipython3 \
    >         tcpdump \
    >         wireshark \
    >         sqlite \
    >         sqlite3 \
    >         libsqlite3-dev \
    >         qtbase5-dev \
    >         qtchooser \
    >         qt5-qmake \
    >         qtbase5-dev-tools \
    >         openmpi-bin \
    >         openmpi-common \
    >         openmpi-doc \
    >         libopenmpi-dev \
    >         doxygen \
    >         graphviz \
    >         imagemagick \
    >         python3-sphinx \
    >         dia \
    >         imagemagick \
    >         texlive \
    >         dvipng \
    >         latexmk \
    >         texlive-extra-utils \
    >         texlive-latex-extra \
    >         texlive-font-utils \
    >         libeigen3-dev \
    >         gsl-bin \
    >         libgsl-dev \
    >         libgslcblas0 \
    >         libxml2 \
    >         libxml2-dev \
    >         libgtk-3-dev \
    >         lxc-utils \
    >         lxc-templates \
    >         vtun \
    >         uml-utilities \
    >         ebtables \
    >         bridge-utils \
    >         libxml2 \
    >         libxml2-dev \
    >         libboost-all-dev && \
    >         apt-get clean && \
    >         apt-get autoclean && \
    >         apt-get autoremove && \
    >         rm -rf /var/lib/apt
    > 
    > RUN mkdir -p /usr/ns3
    > WORKDIR /usr
    > 
    > RUN wget https://www.nsnam.org/release/ns-allinone-3.38.tar.bz2  && \
    >     tar -jxvf ns-allinone-3.38.tar.bz2
    > RUN cd ns-allinone-3.38 && ./build.py --enable-examples --enable-tests
    > RUN ln -s /usr/ns-allinone-3.38/ns-3.38/ /usr/ns3/
    > RUN rm ns-allinone-3.38.tar.bz2
    > RUN pip install --user cppyy==2.4.2
    > ```
    > 

2. Build the docker file

  ```bash=
   podman build -tag <IMAGE_NAME> . 
  ```


  ![image](https://hackmd.io/_uploads/ByiGRSOJ0.png)

3. Make sure the image is generated

    ```bash=
    podman images
    ```

    ![image](https://hackmd.io/_uploads/Hy_P0rdy0.png)


    >
    > You can also push the generated image into your private registry so you can pull the image from another machine.
    > 
    > 1. `podman tag localhost/infidel/ns3:3.38  quay.bmw.lab/infidel/ns3:3.38`
    > 2. `podman push quay.bmw.lab/infidel/ns3:3.38 --tls-verify=false`
    > ![image](https://hackmd.io/_uploads/SyGxMwu1C.png)
    > ![image](https://hackmd.io/_uploads/H1KfzDuJA.png)
    > ---

4. Try run the image interactively so you can start from the container images's shell

  ```bash=
  podman run -it <IMAGE_NAME>
  ```

  ![image](https://hackmd.io/_uploads/Syloy8u1R.png)

  


#### 1.2.2 NS3 How to Use

1. Execute the container image interactively
2. Go to the NS3 directory in my case its `/use/ns3/ns-3.38/`
3. Use `./ns3 run <Simulation Script>`

## 2.1 Evaluation 

### 2.1.1 Scenario 1: Throughput per AC 

1. Reference test results

  ![image](https://hackmd.io/_uploads/S1Q29HOk0.png)

2. NS3 802.11e default topx simulation

  ![image](https://hackmd.io/_uploads/r1nukw_10.png)

:::warning

**To Dos**

- [ ] Add AC_VO, AC_BE, and AC_BK into the simulation code.
- [ ] Adjust the simulation 
:::


## NS3 WiFi 802.11a Simulation

### Introduction

The NS3 simulation conducted to emulate legacy WiFi networks has demonstrated success. However, integrating combined cases posed notable challenges during the process. This report outlines the achievements of the simulation and discusses the encountered difficulties, highlighting the need for further investigation and resolution of combined cases.


### Simulation Setup

![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_31c311e5f183dd837bcf9c879698ecc6.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712917885&Signature=DSLEyoYbM9BWSF7xk9O%2BIx7dyE4%3D)

**Features**

1. Enable RTS/CTS
2. TxOP mode
3. Traffic flow monitoring
4. Traffic logging to tcpdump 

---

>  [!IMPORTANT]
> :information_source: Source Code is [here](https://github.com/bmw-ece-ntust/multimedia-wireless-network/blob/2024-M11202819-Yosafat-Marselino-Agus/codes/mwn-a2-80211a-sim.cc)


### Simulations Result

in this simulation i attempted to create 3 set of network 


![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_529b3b65aaec683facf550b48242dd04.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712917898&Signature=OyXdNhRqwX3pPQLhR3nFQ7H4tYg%3D)

1. Connection between adhoc with it's client is not succed, even if the adhoc network is already broadcasting beacon

![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_29fd126cc21358403193643df3fd1323.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712917920&Signature=0PoTe75AlfkH8iMpDgcqtodSaVc%3D)

![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_1e385b10efdbb7ef10a84717bd74e435.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712917946&Signature=NAWiN1q9vO9eNFQYESet9qxOEns%3D)

2. Bridge connection between AP A and AP C is not established.  Broadcast without any association.


3. Connection between AP A and STA A is normal

![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_0cd53a1fb390018cfb77bf32345578dc.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712917973&Signature=fOyT0lkN1LKhctTkxmsvymGYczA%3D)

4. Connection between AP B and STA B is normal


### Analysis

1. Broadcast 802.11a

![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_3c7e4e7f64007985a46ed3bc33270619.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712917987&Signature=2CEKdaXhHAlfsajyBLiepRmh0Kk%3D)

2. Hidden node scenario. 

    a. RTS and CTS Sequence detected by wireshark

    ![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_82514c50070835c4e5538a5c32ffc08d.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712917995&Signature=eNfMgBT7LBHCA7Hg9SDh%2Bt%2FHm9E%3D)
    
    b. RTS Payload
    
    ![image](https://hackmd.io/_uploads/Hy3_OqWgR.png)

    c. CTS Payload
    
    ![image](https://hackmd-prod-images.s3-ap-northeast-1.amazonaws.com/uploads/upload_ec59f5100da44f0f4f0db490ec595f5f.png?AWSAccessKeyId=AKIA3XSAAW6AWSKNINWO&Expires=1712918012&Signature=sxTRGj%2B3uvAGxbyyFY7qA2MQhcY%3D)

