/*
 * @Description: System::Net::Socket的单元测试
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-04 19:35:30
 * @LastEditors: like
 * @LastEditTime: 2021-09-12 16:21:36
 */
#define _CRT_SECURE_NO_WARNINGS
#include <System.Net.Socket.hpp>

using namespace System::Net;
int main()
{
    printf("--------Socket客户端--------\n");
    /* test 2 : Create Client With Server Addr & Port */
    Socket* client = new Socket("127.0.0.1", 8888, Tcp_Client);
    if(!client->IsCreateSocketSucess())
    {
        printf("--------Socket客户端创建失败--------\n");
        return 1;
    }
    /* test 2 : Connect To Server */
    if(!client->ConnectToServer())
    {
        printf("--------Socket客户端连接服务器失败--------\n");
        return 1;
    }
    printf("--------Socket客户端连接服务器成功--------\n");
    int len;
    int sendLen;
    char buf[1024];
    while(0 < (len = scanf("%s", &buf)))
    {
        /* test 3 : Client Send Buffer */
        if(1 > (sendLen = client->SendTcpToServer(buf, strlen(buf))))
        {
            break;
        }
        printf("DataLen:%d, SendedLen:%d\n", strlen(buf), sendLen);
    }
    
    /* test 4 : Wait Client Pipline Failed */
    printf("--------退出Socket%s--------\n", client->WaitExitFlag() ? "成功" : "失败");
    /* test 5 : Dispose Client All Memory */
    printf("--------释放Socket%s--------\n", client->Dispose() ? "成功" : "失败");
    
    return 0;  
}