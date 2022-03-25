/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-22 14:35:23
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 16:28:13
 */
#include <System.Net.Sockets.UdpClient.hpp>
#include <string.h>

using namespace System::Net;
using namespace System::Net::Sockets;
void UdpServer()
{
    UdpClient server(IPAddress::Loopback, 9999);
    server.SetExclusiveAddressUse(false);
    /**
     * @brief 禁止收到自己发的消息
     * 
     */
    server.SetMulticastLoopback(false);

    char buf[MAX_PATH];
    SocketAddress remoteEP;
    while(1)
    {
        server.Receive(buf, MAX_PATH, remoteEP);
        printf("Receive : \"%s", buf);
        remoteEP.ToString(buf);
        printf(" \" from %s\n", buf);
        server.Send("ok", 3, remoteEP);
        Sleep(1000);
    }
    server.Close();
}
void UnicastTest()
{
    UdpClient client;
    client.SetExclusiveAddressUse(false);
    /**
     * @brief 禁止收到自己发的消息
     * 
     */
    client.SetMulticastLoopback(false);
    client.Connect(IPAddress::Loopback, 9999);
    SocketAddress addrConnectTo = client.GetClient()->GetLocalEndPoint();
    SocketAddress remoteEP;
    char buf[MAX_PATH];

    addrConnectTo.ToString(buf);
    printf("%s connect sucess\n", buf);
    while(1)
    {
        client.Send("Hello world", 12, addrConnectTo);
        client.Receive(buf, MAX_PATH, remoteEP);
        printf("Receive : \"%s", buf);
        remoteEP.ToString(buf);
        printf(" \" from %s\n", buf);
        Sleep(1000);
    }
    client.Close();
}

int main(int argc, char** argv)
{
    ERROR_ASSERT(2 == argc, "\n    Start Udp : System.Net.Sockets.Udp.Test.exe s[server]/u[unicast]/m[multicast]/b[broadcast]");
    if(0 == strcmp("s", argv[1]))
    {
        UdpServer();
    }
    else if(0 == strcmp("u", argv[1]))
    {
        UnicastTest();
    }
    return 0;
}