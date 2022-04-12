/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-22 14:35:23
 * @LastEditors: like
 * @LastEditTime: 2022-03-26 20:12:40
 */
#include <System.Net.Sockets.UdpClient.hpp>
#include <string.h>

using namespace System::Net;
using namespace System::Net::Sockets;

#define SERVER_PORT 9999
#define GROUP0 "224.0.0.88"
#define GROUP1 "239.0.0.88"
IPAddress group(GROUP0);
IPAddress group1(GROUP1);

/**
 * @brief UDP 服务器
 * 
 */
void UdpServer()
{
    UdpClient server(IPAddress::Any, SERVER_PORT);
    // server.SetExclusiveAddressUse(false);
    server.GetClient()->SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReuseAddress, true);
    /**
     * @brief 禁止收到自己发的消息
     * 
     */
    // server.SetMulticastLoopback(false);
    /**
     * @brief 加入组播
     * 
     */
    server.JoinMulticastGroup(group);
    server.JoinMulticastGroup(group1);

    char buf[MAX_PATH];
    SocketAddress remoteEP;
    while(1)
    {
        printf("Receive : ");
        server.Receive(buf, MAX_PATH, remoteEP);
        printf("\"%s\"", buf);
        remoteEP.ToString(buf);
        printf(" from %s\n", buf);
        server.Send("ok", 3, remoteEP);
        Sleep(1000);
    }
    server.Close();
}
void UnicastTest()
{
    UdpClient client(IPAddress::Loopback, SERVER_PORT + 'u');
    client.Connect(IPAddress::Loopback, SERVER_PORT);
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
/**
 * @brief 组播测试
 * 
 */
void MulticastTest()
{
    UdpClient client;
    /**
     * @brief 给组播发消息， 组播服务器的回包接收不到，设置 1s 超时
     * 
     */
    client.GetClient()->SetReceiveTimeout(1000);
    client.Connect(IPAddress::Loopback, SERVER_PORT);
    SocketAddress remoteEP;
    SocketAddress socketGroup(group, SERVER_PORT);
    char buf[MAX_PATH];

    client.GetClient()->GetLocalEndPoint().ToString(buf);
    printf("%s connect sucess\n", buf);
    while(1)
    {
        client.Send("Hello world", 12, socketGroup);
        printf("Receive : ");
        client.Receive(buf, MAX_PATH, remoteEP);
        printf("\"%s\"", buf);
        remoteEP.ToString(buf);
        printf(" from %s\n", buf);
        Sleep(1000);
    }
    client.Close();
}
void BroadcastTest()
{
    UdpClient client;
    client.GetClient()->SetReceiveTimeout(1000);
    client.SetEnableBroadcast(true);
    client.Connect(IPAddress::Loopback, SERVER_PORT);
    SocketAddress remoteEP;
    SocketAddress broadcastGroup(IPAddress::Broadcast, SERVER_PORT);
    char buf[MAX_PATH];

    client.GetClient()->GetLocalEndPoint().ToString(buf);
    printf("%s connect sucess\n", buf);
    while(1)
    {
        client.Send("Hello world", 12, broadcastGroup);
        printf("Receive : ");
        client.Receive(buf, MAX_PATH, remoteEP);
        printf("\"%s\"", buf);
        remoteEP.ToString(buf);
        printf(" from %s\n", buf);
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
    else if(0 == strcmp("m", argv[1]))
    {
        MulticastTest();
    }
    else if(0 == strcmp("b", argv[1]))
    {
        BroadcastTest();
    }
    return 0;
}