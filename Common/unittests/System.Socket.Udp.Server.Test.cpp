/*
 * @Description: System::Net::Socket的单元测试
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-04 19:35:30
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 14:05:26
 */
#include <__System.Net.Sockets.hpp>

using namespace System::Net;
Socket* server;
bool ServerCallBack(const char* buf, int len, int pipline, const SocketClient& client)
{
    char ip[IP_STR_SIZE];
    DWORD port; 
    GetSocketAddress(client, ip, port);
    printf("receive from %s:%d, buffer:%s\n", ip, port, buf);
    
    if(0 == strcmp("exit", buf) && NULL != server)
    {
        return false;
    }
    return true;
}

int main()
{
    printf("--------Udp单播服务器--------\n");
    server = new Socket("127.0.0.1", 8888, Udp_Server);
    if(!server->IsCreateSocketSucess())
    {
        printf("--------Udp单播服务器创建失败--------\n");
    }
    server->SetAnsynAccepet(true);
    int pipline;
    if(0 > (pipline = server->StartServer(ServerCallBack)))
    {
        printf("--------Udp单播服务器Pipline_%d启动失败--------\n", pipline);
    }
    else
    {
        printf("--------Udp单播服务器Pipline_%d启动成功--------\n", pipline);
    }
    printf("--------Udp单播服务器退出Pipline_%d消息管道%s--------\n", pipline, server->WaitExitFlag(pipline) ? "成功" : "失败");
    printf("--------释放Udp单播服务器%s--------\n", server->Dispose() ? "成功" : "失败");
    
    return 0; 
}


