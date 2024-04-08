## Working wih second.cc

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

// Default Network Topology
//
//       172.16.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 172.16.2.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("SecondScriptExample");

int
main(int argc, char* argv[])
{
    bool verbose = true;
    uint32_t nCsma = 3;

    CommandLine cmd(__FILE__);
    cmd.AddValue("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
    cmd.AddValue("verbose", "Tell echo applications to log if true", verbose);

    cmd.Parse(argc, argv);

    if (verbose)
    {
        LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
        LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }

    nCsma = nCsma == 0 ? 1 : nCsma;

    NodeContainer p2pNodes;
    p2pNodes.Create(2);

    NodeContainer csmaNodes;
    csmaNodes.Add(p2pNodes.Get(1));
    csmaNodes.Create(nCsma);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));

    NetDeviceContainer p2pDevices;
    p2pDevices = pointToPoint.Install(p2pNodes);

    CsmaHelper csma;
    csma.SetChannelAttribute("DataRate", StringValue("100Mbps"));
    csma.SetChannelAttribute("Delay", TimeValue(NanoSeconds(6560)));

    NetDeviceContainer csmaDevices;
    csmaDevices = csma.Install(csmaNodes);

    InternetStackHelper stack;
    stack.Install(p2pNodes.Get(0));
    stack.Install(csmaNodes);

    Ipv4AddressHelper address;
    address.SetBase("172.16.1.0", "255.255.255.0");
    Ipv4InterfaceContainer p2pInterfaces;
    p2pInterfaces = address.Assign(p2pDevices);

    address.SetBase("172.16.2.0", "255.255.255.0");
    Ipv4InterfaceContainer csmaInterfaces;
    csmaInterfaces = address.Assign(csmaDevices);

    UdpEchoServerHelper echoServer(9);

    ApplicationContainer serverApps = echoServer.Install(csmaNodes.Get(nCsma));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(csmaInterfaces.GetAddress(nCsma), 9);
    echoClient.SetAttribute("MaxPackets", UintegerValue(1));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(1.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(1024));

    ApplicationContainer clientApps = echoClient.Install(p2pNodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    Ipv4GlobalRoutingHelper::PopulateRoutingTables();

    pointToPoint.EnablePcapAll("second");
    csma.EnablePcap("second", csmaDevices.Get(1), true);

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}

$ ./ns3 run scratch/second

![image](https://hackmd.io/_uploads/ByEdQIAyA.png)

To send 2/3 packets use
`echoClient.SetAttribute("MaxPackets", UintegerValue(2/3));`



![image](https://hackmd.io/_uploads/ryLF5wRk0.png)





PCAP file generated after p2p and CSMA node packet tranfer.

### To generate pcap file code is below.

```
    pointToPoint.EnablePcapAll("p2p");
    csma.EnablePcap("csma1", csmaDevices.Get(1), true);
    csma.EnablePcap("csma3", csmaDevices.Get(3), true);
```


![image](https://hackmd.io/_uploads/rkTL2D0kC.png)

$ wireshark p2p-1-0.pcap

![image](https://hackmd.io/_uploads/r1uar50kA.png)

$ wireshark csma1-2-0.pcap
![image](https://hackmd.io/_uploads/SyfGD9CJR.png)

### Network Animation Code

```
#include "ns3/netanim-module.h"
 AnimationInterface anim("second.xml");
    anim.SetConstantPosition(p2pNodes.Get(0),10.0,10.0);
    anim.SetConstantPosition(csmaNodes.Get(0),20.0,20.0);
    anim.SetConstantPosition(csmaNodes.Get(0),30.0,30.0);
    anim.SetConstantPosition(csmaNodes.Get(0),40.0,40.0);
    anim.SetConstantPosition(csmaNodes.Get(0),50.0,50.0);
```

$ ./NetAnim

Select second.xml.

![image](https://hackmd.io/_uploads/rJkwraCk0.png)


### Ascii Trace-
```
AsciiTraceHelper ascii;
    pointToPoint.EnableAsciiAll(ascii.CreateFileStream("point2point.tr"));
    csma.EnableAsciiAll(ascii.CreateFileStream("csma2.tr"));
```
$ java -jar tracemetrics.jar 

![image](https://hackmd.io/_uploads/HkitiaC10.png)








