#include "ns3/command-line.h"
#include "ns3/internet-stack-helper.h"
#include "ns3/ipv4-address-helper.h"
#include "ns3/flow-monitor-helper.h"
#include "ns3/ipv4-flow-classifier.h"
#include "ns3/log.h"
#include "ns3/mobility-helper.h"
#include "ns3/on-off-helper.h"
#include "ns3/pointer.h"
#include "ns3/qos-txop.h"
#include "ns3/ssid.h"
#include "ns3/string.h"
#include "ns3/udp-client-server-helper.h"
#include "ns3/udp-echo-helper.h"
#include "ns3/wifi-mac.h"
#include "ns3/wifi-net-device.h"
#include "ns3/yans-wifi-channel.h"
#include "ns3/yans-wifi-helper.h"

// Hidden Stations
#include "ns3/config.h"

// NetAnim
#include "ns3/netanim-module.h"

/**
 * \file
 *
 * Explain here what the example does.
 */

/**
 * -------------------------------------
 * Yosafat Marselino Agus (M11202819)
 * Multimedia Wireless Network Class 2024
 * -------------------------------------
 * The following node should have the following criteria:
 * 1. Hidden node terminal
 * 2. transmission range
 * 3. Sensing Range
 * 4. error-free channel 
 * 5. error-channel 
 * 6. Statistic collections
 */


using namespace ns3;

NS_LOG_COMPONENT_DEFINE("mwn-yma");

struct TxopDurationTracer
{
    /**
     * Callback connected to TXOP duration trace source.
     *
     * \param startTime TXOP start time
     * \param duration TXOP duration
     * \param linkId the ID of the link
     */
    void Trace(Time startTime, Time duration, uint8_t linkId);
    Time m_max{Seconds(0)}; //!< maximum TXOP duration
};

void
TxopDurationTracer::Trace(Time startTime, Time duration, uint8_t linkId)
{
    if (duration > m_max)
    {
        m_max = duration;
    }
}
int
main(int argc, char* argv[])
{

    uint32_t payloadSize = 1472; // bytes
    double simulationTime = 10;  // seconds
    double distance = 10;         // meters
    bool enablePcap = 0;
    bool verifyResults = 0; // used for regression
    Time txopLimit = MicroSeconds(4096);
    bool enableCtsRts = 0;
    bool animateResult = 0;


    CommandLine cmd(__FILE__);
    cmd.AddValue("payloadSize", "Payload size in bytes", payloadSize);
    cmd.AddValue("simulationTime", "Simulation time in seconds", simulationTime);
    cmd.AddValue("distance",
                 "Distance in meters between the station and the access point",
                 distance);
    cmd.AddValue("enablePcap", "Enable/disable pcap file generation", enablePcap);
    cmd.AddValue("verifyResults",
                 "Enable/disable results verification at the end of the simulation",
                 verifyResults);
    cmd.AddValue("enableCTSRTS","Enable CTS/RTS", enableCtsRts);
    cmd.AddValue("animResult","Enable NetAnim", animateResult);

    cmd.Parse(argc, argv);

    UintegerValue ctsThr = (enableCtsRts ? UintegerValue(100) : UintegerValue(2200));
    std::cout << "CTS/RTS Flag => " << enableCtsRts << "\n";
    Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", ctsThr);

    NodeContainer wifiStaNodes;
    wifiStaNodes.Create(3);
    NodeContainer wifiApNodes;
    wifiApNodes.Create(6);

    YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
    YansWifiPhyHelper phy;
    phy.SetPcapDataLinkType(WifiPhyHelper::DLT_IEEE802_11_RADIO);
    phy.SetChannel(channel.Create());

    WifiHelper wifi;
    wifi.SetStandard(WIFI_STANDARD_80211a);
    wifi.SetRemoteStationManager("ns3::IdealWifiManager");
    WifiMacHelper mac;

    NetDeviceContainer staDeviceA;
    NetDeviceContainer staDeviceB;
    NetDeviceContainer staDeviceC;
    NetDeviceContainer apDeviceA;
    NetDeviceContainer apDeviceB;
    NetDeviceContainer apDeviceC;
    Ssid ssid;

    std::cout << ". \n";
    std::cout << "===================================================== \n";
    std::cout << "Yosafat Marselino Agus (M11202819) \n";
    std::cout << "Multimedia Wireless Network Class, 2024 \n";
    std::cout << "===================================================== \n";
    std::cout << ". \n";
    std::cout << ". \n";

    // Network A
    ssid = Ssid("network-A");
    phy.Set("ChannelSettings", StringValue("{36, 20, BAND_5GHZ, 0}"));
    mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));
    staDeviceA = wifi.Install(phy, mac, wifiStaNodes.Get(0));

    mac.SetType("ns3::ApWifiMac",
                "QosSupported",
                BooleanValue(true),
                "Ssid",
                SsidValue(ssid),
                "EnableBeaconJitter",
                BooleanValue(false));
    apDeviceA = wifi.Install(phy, mac, wifiApNodes.Get(0));

    // Network B
    ssid = Ssid("network-B");
    phy.Set("ChannelSettings", StringValue("{40, 20, BAND_5GHZ, 0}"));
    mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));

    staDeviceB = wifi.Install(phy, mac, wifiStaNodes.Get(1));

    mac.SetType("ns3::ApWifiMac",
                "QosSupported",
                BooleanValue(true),
                "Ssid",
                SsidValue(ssid),
                "EnableBeaconJitter",
                BooleanValue(false));
    apDeviceB = wifi.Install(phy, mac, wifiApNodes.Get(1));

    // Network C
    ssid = Ssid("network-C");
    phy.Set("ChannelSettings", StringValue("{44, 20, BAND_5GHZ, 0}"));
    mac.SetType("ns3::StaWifiMac", "QosSupported", BooleanValue(true), "Ssid", SsidValue(ssid));

    staDeviceC = wifi.Install(phy, mac, wifiStaNodes.Get(2));

    mac.SetType("ns3::ApWifiMac",
                "QosSupported",
                BooleanValue(true),
                "Ssid",
                SsidValue(ssid),
                "EnableBeaconJitter",
                BooleanValue(false));
    apDeviceC = wifi.Install(phy, mac, wifiApNodes.Get(2));

    /* Setting mobility model */
    MobilityHelper mobility;
    Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
    mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");

    // Set position for APs
    positionAlloc->Add(Vector(0.0, 0.0, 0.0));
    positionAlloc->Add(Vector(10.0, 0.0, 0.0));
    positionAlloc->Add(Vector(20.0, 0.0, 0.0));
    // Set position for STAs
    positionAlloc->Add(Vector(distance,  0.0, 0.0));
    positionAlloc->Add(Vector(10+distance, 0.0, 0.0));
    positionAlloc->Add(Vector(20+distance, 0.0, 0.0));
    // Remark: while we set these positions 10 meters apart, the networks do not interact
    // and the only variable that affects transmission performance is the distance.

    mobility.SetPositionAllocator(positionAlloc);
    mobility.Install(wifiApNodes);
    mobility.Install(wifiStaNodes);

    /* Internet stack */
    InternetStackHelper stack;
    stack.Install(wifiApNodes);
    stack.Install(wifiStaNodes);

    Ipv4AddressHelper address;

    address.SetBase("192.168.1.0", "255.255.255.0");
    Ipv4InterfaceContainer StaInterfaceA;
    StaInterfaceA = address.Assign(staDeviceA);
    Ipv4InterfaceContainer ApInterfaceA;
    ApInterfaceA = address.Assign(apDeviceA);

    address.SetBase("192.168.2.0", "255.255.255.0");
    Ipv4InterfaceContainer StaInterfaceB;
    StaInterfaceB = address.Assign(staDeviceB);
    Ipv4InterfaceContainer ApInterfaceB;
    ApInterfaceB = address.Assign(apDeviceB);

    address.SetBase("192.168.3.0", "255.255.255.0");
    Ipv4InterfaceContainer StaInterfaceC;
    StaInterfaceC = address.Assign(staDeviceC);
    Ipv4InterfaceContainer ApInterfaceC;
    ApInterfaceC = address.Assign(apDeviceC);

    ApplicationContainer cbrApps;

    uint16_t cbrPort = 12345;
    OnOffHelper onOffHelper("ns3::UdpSocketFactory",
                            InetSocketAddress(Ipv4Address("192.168.1.3"), cbrPort));
    onOffHelper.SetAttribute("PacketSize", UintegerValue(1400));
    onOffHelper.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onOffHelper.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));

    // flow 1:  node 0 -> node 1
    onOffHelper.SetAttribute("DataRate", StringValue("3000000bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.000000)));
    cbrApps.Add(onOffHelper.Install(wifiApNodes.Get(0)));

    // flow 2:  node 2 -> node 1
    /** \internal
     * The slightly different start times and data rates are a workaround
     * for \bugid{388} and \bugid{912}
     */
    onOffHelper.SetAttribute("DataRate", StringValue("3001100bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
    cbrApps.Add(onOffHelper.Install(wifiApNodes.Get(2)));

    onOffHelper.SetAttribute("DataRate", StringValue("3001100bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
    cbrApps.Add(onOffHelper.Install(wifiApNodes.Get(3)));

    onOffHelper.SetAttribute("DataRate", StringValue("3001100bps"));
    onOffHelper.SetAttribute("StartTime", TimeValue(Seconds(1.001)));
    cbrApps.Add(onOffHelper.Install(wifiApNodes.Get(4)));

    uint16_t port = 5001;

    UdpServerHelper serverA(port);
    ApplicationContainer serverAppA = serverA.Install(wifiApNodes.Get(0));

    serverAppA.Start(Seconds(0.0));
    serverAppA.Stop(Seconds(simulationTime + 1));

    InetSocketAddress destA(ApInterfaceA.GetAddress(0), port);
    destA.SetTos(0x28); // AC_BK

    OnOffHelper clientA("ns3::UdpSocketFactory", destA);
    clientA.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientA.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientA.SetAttribute("DataRate", StringValue("100000kb/s"));
    clientA.SetAttribute("PacketSize", UintegerValue(payloadSize));


    ApplicationContainer clientAppA = clientA.Install(wifiStaNodes.Get(0));
    clientAppA.Start(Seconds(1.0));
    clientAppA.Stop(Seconds(simulationTime + 1));

    // Client B
    UdpServerHelper serverB(port);
    ApplicationContainer serverAppB = serverB.Install(wifiApNodes.Get(1));
    serverAppB.Start(Seconds(0.0));
    serverAppB.Stop(Seconds(simulationTime + 1));

    InetSocketAddress destB(ApInterfaceB.GetAddress(0), port);
    destB.SetTos(0x70); // AC_BE

    OnOffHelper clientB("ns3::UdpSocketFactory", destB);
    clientB.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    clientB.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    clientB.SetAttribute("DataRate", StringValue("100000kb/s"));
    clientB.SetAttribute("PacketSize", UintegerValue(payloadSize));

    ApplicationContainer clientAppB = clientB.Install(wifiStaNodes.Get(1));
    clientAppB.Start(Seconds(1.0));
    clientAppB.Stop(Seconds(simulationTime + 1));

    //std::vector<uint8_t> tosValues = {0x70, 0x28, 0xb8, 0xc0}; //AC_BE, AC_BK, AC_VI, AC_VO

    uint16_t echoPort = 9;
    UdpEchoClientHelper echoClientHelper(Ipv4Address("192.168.1.3"), echoPort);
    echoClientHelper.SetAttribute("MaxPackets", UintegerValue(1));
    echoClientHelper.SetAttribute("Interval", TimeValue(Seconds(0.1)));
    echoClientHelper.SetAttribute("PacketSize", UintegerValue(10));
    ApplicationContainer pingApps;

    // again using different start times to workaround Bug 388 and Bug 912
    echoClientHelper.SetAttribute("StartTime", TimeValue(Seconds(0.001)));
    pingApps.Add(echoClientHelper.Install(wifiApNodes.Get(0)));
    echoClientHelper.SetAttribute("StartTime", TimeValue(Seconds(0.006)));
    pingApps.Add(echoClientHelper.Install(wifiApNodes.Get(2)));

    if (enablePcap)
    {
        phy.EnablePcap("AP_A", apDeviceA.Get(0));
        phy.EnablePcap("STA_A", staDeviceA.Get(0));
        phy.EnablePcap("AP_B", apDeviceB.Get(0));
        phy.EnablePcap("STA_B", staDeviceB.Get(0));
        phy.EnablePcap("AP_C", apDeviceC.Get(0));
        phy.EnablePcap("STA_C", staDeviceC.Get(0));
    }

    if (animateResult) { 

        AnimationInterface::SetConstantPosition(wifiStaNodes.Get(0), 10, 30);

        AnimationInterface anim("contrib/mwn/animations/wireless-animation.xml"); // Mandatory
                                                           //
        for (uint32_t i = 0; i < wifiStaNodes.GetN(); ++i)
        {
            anim.UpdateNodeDescription(wifiStaNodes.Get(i), "STA"); // Optional
            anim.UpdateNodeColor(wifiStaNodes.Get(i), 255, 0, 0);   // Optional
        }
        for (uint32_t i = 0; i < wifiApNodes.GetN(); ++i)
        {
            anim.UpdateNodeDescription(wifiApNodes.Get(i), "AP"); // Optional
            anim.UpdateNodeColor(wifiApNodes.Get(i), 0, 255, 0);  // Optional
        }

        anim.EnablePacketMetadata(); // Optional
                                     //
        anim.EnableIpv4RouteTracking("contrib/mwn/animations/routingtable-wireless.xml",
                                     Seconds(0),
                                     Seconds(5),
                                     Seconds(0.25));         // Optional
        anim.EnableWifiMacCounters(Seconds(0), Seconds(10)); // Optional
        anim.EnableWifiPhyCounters(Seconds(0), Seconds(10)); // Optional
    }

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();

    Simulator::Stop(Seconds(simulationTime + 1));
    Simulator::Run();

    uint64_t totalPacketsThroughA = DynamicCast<UdpServer>(serverAppA.Get(0))->GetReceived();

    uint64_t totalPacketsThroughB = DynamicCast<UdpServer>(serverAppB.Get(0))->GetReceived();

    monitor->CheckForLostPackets();
    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon.GetClassifier());
    FlowMonitor::FlowStatsContainer stats = monitor->GetFlowStats();

    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator i = stats.begin();
         i != stats.end();
         ++i)
    {
        // first 2 FlowIds are for ECHO apps, we don't want to display them
        //
        // Duration for throughput measurement is 9.0 seconds, since
        //   StartTime of the OnOffApplication is at about "second 1"
        // and
        //   Simulator::Stops at "second 10".
            Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow(i->first);
            std::cout << "Flow " << i->first - 2 << " (" << t.sourceAddress << " -> "
                      << t.destinationAddress << ")\n";
            std::cout << "  Tx Packets: " << i->second.txPackets << "\n";
            std::cout << "  Tx Bytes:   " << i->second.txBytes << "\n";
            std::cout << "  TxOffered:  " << i->second.txBytes * 8.0 / 9.0 / 1000 / 1000
                      << " Mbps\n";
            std::cout << "  Rx Packets: " << i->second.rxPackets << "\n";
            std::cout << "  Rx Bytes:   " << i->second.rxBytes << "\n";
            std::cout << "  Throughput: " << i->second.rxBytes * 8.0 / 9.0 / 1000 / 1000
                      << " Mbps\n";
    }

    Simulator::Destroy();

    double throughput = totalPacketsThroughA * payloadSize * 8 / (simulationTime * 1000000.0);

    std::cout << "AC_BE with default TXOP limit (0ms): " << '\n'
              << "  Throughput = " << throughput << " Mbit/s" << '\n';

    throughput = totalPacketsThroughB * payloadSize * 8 / (simulationTime * 1000000.0);

    std::cout << "AC_BE with default TXOP limit (0ms): " << '\n'
              << "  Throughput = " << throughput << " Mbit/s" << '\n';

    if (verifyResults && (throughput < 28 || throughput > 29))
    {
        NS_LOG_ERROR("Obtained throughput " << throughput << " is not in the expected boundaries!");
        exit(1);
    }


    return 0;
}

