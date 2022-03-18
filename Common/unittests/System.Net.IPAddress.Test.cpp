/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-15 11:04:20
 * @LastEditors: like
 * @LastEditTime: 2022-03-16 16:02:18
 */
#include <System.Net.IPAddress.hpp>

using namespace System;
using namespace System::Net;

void IPAddressTest()
{
    char buf[MAX_IP_LENGTH];
    {
        IPAddress any = IPAddress::Any;
        any.ToString(buf);
        printf("Any : %s\n", buf);

        IPAddress broadcast = IPAddress::Broadcast;
        broadcast.ToString(buf);
        printf("Broadcast : %s\n", buf);

        IPAddress loopback = IPAddress::Loopback;
        loopback.ToString(buf);
        printf("Loopback : %s\n", buf);
        
        IPAddress none = IPAddress::None;
        none.ToString(buf);
        printf("None : %s\n", buf);
        
        IPAddress ipv6any = IPAddress::IPv6Any;
        ipv6any.ToString(buf);
        printf("IPv6Any : %s\n", buf);

        IPAddress ipv6loopback = IPAddress::IPv6Loopback;
        ipv6loopback.ToString(buf);
        printf("IPv6Loopback : %s\n", buf);
        
        IPAddress ipv6none = IPAddress::IPv6None;
        ipv6none.ToString(buf);
        printf("IPv6None : %s\n", buf);
    }
    /* ipv4 */
    {
        IPAddress ipv4("143.24.20.36");
        ipv4.ToString(buf);
        byte* p = (byte*)&ipv4.ipv4Addr;
        printf
        (
            "Ipv4 : %s \nIpv4 network number : %d.%d.%d.%d \n", 
            buf, 
            p[0],
            p[1],
            p[2],
            p[3]
        );
    }
    /* ipv6 */
    {
        IPAddress ipv6("CDCD:910A:2222:5498:8475:1111:3900:2020");
        ipv6.ToString(buf);
        UInt32* p = (UInt32*)&ipv6.ipv6Addr;
        printf
        (
            "Ipv6 : %s \nIpv6 network number : 0x%x%x%x%x \n", 
            buf, 
            IPAddress::NetworkToHostOrder(p[0]),
            IPAddress::NetworkToHostOrder(p[1]),
            IPAddress::NetworkToHostOrder(p[2]),
            IPAddress::NetworkToHostOrder(p[3])
        );
    }
    /* port */
    {
        UInt16 nPort = 8000;
        UInt16 nNetworkPort = IPAddress::HostToNetworkOrder(nPort);
        printf("Local port : %d, network port : %d\n", IPAddress::NetworkToHostOrder(nNetworkPort), nNetworkPort);
    }
    {
        IPAddress ipAddress(IPAddress::Loopback);
        IPEndPoint ipLocalEndPoint(ipAddress, 11000);
        //Serializes the IPEndPoint.
        SocketAddress* socketAddress = ipLocalEndPoint.Serialize();
        //Verifies that ipLocalEndPoint is now serialized by printing its contents.
        printf("Contents of the socketAddress are: 0x%x ( value should be 0x100007f )\n", socketAddress->ipv4Addr.sin_addr.S_un.S_addr);
        //Checks the Family property.
        printf("The address family of the socketAddress is: %d\n", socketAddress->ipv4Addr.sin_family);
        //Checks the underlying buffer size.
        printf("The size of the underlying buffer is: %d\n", socketAddress->Size);
        IPEndPoint* pCreateEndPoint = (IPEndPoint*)ipLocalEndPoint.Create(*socketAddress);
        printf("SocketAddress serialize -> deserialize : %s\n", ipLocalEndPoint == *pCreateEndPoint ? "equal" : "not equal");
        delete pCreateEndPoint;
        delete socketAddress;
    }
}

int main()
{
    IPAddressTest();
    return 0;
}