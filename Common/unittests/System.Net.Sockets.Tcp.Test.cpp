/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-22 14:35:23
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 16:12:17
 */
#include <System.Net.Sockets.TcpClient.hpp>
#include <System.Net.Sockets.TcpListener.hpp>
#include <string.h>

using namespace System::Net;
using namespace System::Net::Sockets;

void TcpServerTest()
{
    TcpListener server(IPAddress::Loopback, 9999);
    server.Start(1);
    server.SetExclusiveAddressUse(true);
    Socket* client;
    char buf[MAX_PATH];
    int nErrorCode = 0;
    while(true)
    {
        if(!server.Pending())
        {
            continue;
        }
        client = server.AcceptSocket();
        SocketAddress addr = client->GetLocalEndPoint();
        addr.ToString(buf);
        printf("%s accept sucess\n", buf);
        
        client->Receive(buf, sizeof(buf), SocketFlags::None, nErrorCode);
        if(nErrorCode)
        {
            printf("receive failed %d\n", nErrorCode);
            break;
        }
        printf("received : %s", buf);
    }
    server.GetServer()->Close();
}
void TcpClientTest()
{
    TcpClient client;
    client.Connect(IPAddress::Loopback, 9999);
    SocketAddress addr = client.GetClient()->GetLocalEndPoint();
    int errorcode = 0;
    client.GetClient()->Send("Hello world\n", 13, SocketFlags::None, errorcode);
    if(errorcode)
    {
        printf("connect failed\n");
        client.Close();
        return;
    }
    char buf[MAX_PATH];
    addr.ToString(buf);
    printf("%s connect sucess\n", buf);
    client.Close();
}

int main(int argc, char** argv)
{
    ERROR_ASSERT(2 == argc, "\n    Start ipv4 server / client : System.Net.Sockets.Tcp.Test.exe s[server]/c[client]");
    if(0 == strcmp("c", argv[1]))
    {
        TcpClientTest();
    }
    else if(0 == strcmp("s", argv[1]))
    {
        TcpServerTest();
    }
    return 0;
}