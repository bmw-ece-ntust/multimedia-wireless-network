#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/applications-module.h"
#include <iostream>
#include "ns3/command-line.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/log.h"
#include "ns3/mobility-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/pointer.h"
#include "ns3/qos-txop.h"
#include "ns3/qos-wifi-mac-helper.h"
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/udp-server.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"

using namespace ns3;

// Function to calculate throughput
double CalculateThroughput (Ptr<PacketSink> sink, Time startTime, Time stopTime) {
    double totalPacketsReceived = sink->GetTotalRx();
    double duration = (stopTime - startTime).GetSeconds();
    double throughput = totalPacketsReceived * 8 / duration / 1000000; // in Mbps
    return throughput;
}

int main (int argc, char *argv[]) {
    // Enable logging
    LogComponentEnable("PacketSink", LOG_LEVEL_INFO);
    LogComponentEnable("OnOffApplication", LOG_LEVEL_INFO);

    // Create nodes
    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(3); // Three STAs
    NodeContainer wifiApNode;
    wifiApNode.Create(1); // One AP

    // Configure wireless PHY and MAC layers
    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a); // Change here
    wifi.SetRemoteStationManager("ns3::ConstantRateWifiManager",
                               "DataMode", StringValue ("OfdmRate24Mbps"),
                                "ControlMode", StringValue ("OfdmRate6Mbps"));

    // Configure MAC layer with QoS support (802.11e)
    QosWifiMacHelper wifiMac; // Change here
    wifiMac.SetType("ns3::QosWifiMac"); // Change here

    // Configure Access Categories (ACs) with priorities (High, Medium, Low)
    wifiMac.SetMaxQueueSize("AC_VO", 100); // Maximum queue size for AC_VO
    wifiMac.SetMaxQueueSize("AC_VI", 50);  // Maximum queue size for AC_VI
    wifiMac.SetMaxQueueSize("AC_BE", 50);  // Maximum queue size for AC_BE
    wifiMac.SetMaxQueueSize("AC_BK", 20);  // Maximum queue size for AC_BK

    // Set AIFSN (Arbitration Interframe Space Number) values for each AC
    wifiMac.SetAifs("AC_VO", 2); // AIFS for AC_VO
    wifiMac.SetAifs("AC_VI", 2); // AIFS for AC_VI
    wifiMac.SetAifs("AC_BE", 3); // AIFS for AC_BE
    wifiMac.SetAifs("AC_BK", 7); // AIFS for AC_BK

    // Set CWmin (Minimum Contention Window) values for each AC
    wifiMac.SetMinCw("AC_VO", 3); // CWmin for AC_VO
    wifiMac.SetMinCw("AC_VI", 7); // CWmin for AC_VI
    wifiMac.SetMinCw("AC_BE", 15); // CWmin for AC_BE
    wifiMac.SetMinCw("AC_BK", 15); // CWmin for AC_BK

    // Set CWmax (Maximum Contention Window) values for each AC
    wifiMac.SetMaxCw("AC_VO", 7); // CWmax for AC_VO
    wifiMac.SetMaxCw("AC_VI", 15); // CWmax for AC_VI
    wifiMac.SetMaxCw("AC_BE", 1023); // CWmax for AC_BE
    wifiMac.SetMaxCw("AC_BK", 1023); // CWmax for AC_BK

    NetDeviceContainer staDevices;
    NetDeviceContainer apDevice;
    staDevices = wifi.Install(wifiPhy, wifiMac, wifiStaNodes);
    apDevice = wifi.Install(wifiPhy, wifiMac, wifiApNode);

    // Install internet stack
    InternetStackHelper stack;
    stack.Install(wifiStaNodes);
    stack.Install(wifiApNode);

    // Assign IP addresses
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer staInterfaces;
    Ipv4InterfaceContainer apInterface;
    staInterfaces = address.Assign(staDevices);
    apInterface = address.Assign(apDevice);

    // Create OnOff applications for each STA
    OnOffHelper onoff("ns3::UdpSocketFactory", Address());
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    onoff.SetAttribute("PacketSize", UintegerValue(512));

    ApplicationContainer apps;
    std::vector<Ptr<PacketSink>> sinks;
    for (uint32_t i = 0; i < wifiStaNodes.GetN (); ++i) {
        AddressValue remoteAddress(InetSocketAddress(apInterface.GetAddress(0), 9));
        onoff.SetAttribute("DataRate", StringValue("250kb/s")); // Initial data rate
        onoff.SetAttribute("Remote", remoteAddress);
        apps.Add(onoff.Install(wifiStaNodes.Get(i)));

        // Create packet sink for each STA
        Ptr<Node> node = wifiApNode.Get(0);
        Ptr<PacketSink> sink = CreateObject<PacketSink>();
        node->AddApplication(sink);
        sinks.push_back(sink);
    }

    // Start applications
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(10.0));

    // Run simulation
    Simulator::Stop(Seconds(10.0));
    Simulator::Run();

    // Print throughput for each AC with increasing offered traffic per AC
    Time startTime = Seconds(1.0);
    Time stopTime = Seconds(10.0);
    std::cout << "AC_VO Throughput: " << CalculateThroughput(sinks[0], startTime, stopTime) << " Mbps" << std::endl;
    std::cout << "AC_VI Throughput: " << CalculateThroughput(sinks[1], startTime, stopTime) << " Mbps" << std::endl;
    std::cout << "AC_BE Throughput: " << CalculateThroughput(sinks[2], startTime, stopTime) << " Mbps" << std::endl;
    std::cout << "AC_BK Throughput: " << CalculateThroughput(sinks[3], startTime, stopTime) << " Mbps" << std::endl;

    // Cleanup and destroy simulation
    Simulator::Destroy();

    return 0;
}

