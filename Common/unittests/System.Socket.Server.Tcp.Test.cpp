/*
 * @Description: System::Net::Socket的单元测试
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-04 19:35:30
 * @LastEditors: like
 * @LastEditTime: 2021-08-08 18:24:28
 */
#include <System.Net.Socket.hpp>

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
    printf("--------Socket服务器--------\n");
    server = new Socket("127.0.0.1", 8888, Tcp_Server);
    if(!server->IsCreateSocketSucess())
    {
        printf("--------Socket服务器创建失败--------\n");
    }
    server->SetAnsynAccepet(true);/* 没有加同步锁，多个客户端异步连接时，StartServer中间需要间隔一小段 */
    int pipline;
    /* test 1 : Start Server At Pipline_0 */
    if(0 > (pipline = server->StartServer(ServerCallBack)))
    {
        printf("--------Socket服务器Pipline_%d启动失败--------\n", pipline);
    }
    else
    {
        printf("--------Socket服务器Pipline_%d启动成功--------\n", pipline);
    }
    /* test 2 : Exit Pipline_0 At Accecpt */
    // server->StopServer(pipline);
    /* test 3 : Exit Pipline_0 At Recv */
    // Client_0 Send cmd "exit"
    Thread::Sleep(50);/* 在设置自动管理通讯管道（AUTO_PIPLINE_MANAGE）的情况下， 如果要用异步的方式等待客户端accecpt，启动服务器的时候二者中间最好等一小会*/
    int pipline1;
    /* test 1 : Start Server At Pipline_1 */
    if(0 > (pipline1 = server->StartServer(ServerCallBack)))
    {
        printf("--------Socket服务器Pipline_%d启动失败--------\n", pipline);
    }
    else
    {
        printf("--------Socket服务器Pipline_%d启动成功--------\n", pipline1);
    }
    /* test 2 : Exit Pipline_1 At Accecpt */
    // server->StopServer(pipline1);
    /* test 3 : Exit Pipline_1 At Recv */
    // Client_1 Send cmd "exit"
    /* test 4 : Block current thread to wait Pipline_1 Finished*/
    printf("--------退出Pipline_%d消息管道%s--------\n", pipline1, server->WaitExitFlag(pipline1) ? "成功" : "失败");
    /* test 4 : Block current thread to wait Pipline_0 Finished*/
    printf("--------退出Pipline_%d消息管道%s--------\n", pipline, server->WaitExitFlag(pipline) ? "成功" : "失败");
    /* test 5 : Dispose Server All Memory */
    printf("--------释放Socket%s--------\n", server->Dispose() ? "成功" : "失败");
    
    return 0; 
}


