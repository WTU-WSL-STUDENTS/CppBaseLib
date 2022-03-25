/*
 * @Description: System::Net::Socket�ĵ�Ԫ����
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
    printf("--------Udp����������--------\n");
    server = new Socket("127.0.0.1", 8888, Udp_Server);
    if(!server->IsCreateSocketSucess())
    {
        printf("--------Udp��������������ʧ��--------\n");
    }
    server->SetAnsynAccepet(true);
    int pipline;
    if(0 > (pipline = server->StartServer(ServerCallBack)))
    {
        printf("--------Udp����������Pipline_%d����ʧ��--------\n", pipline);
    }
    else
    {
        printf("--------Udp����������Pipline_%d�����ɹ�--------\n", pipline);
    }
    printf("--------Udp�����������˳�Pipline_%d��Ϣ�ܵ�%s--------\n", pipline, server->WaitExitFlag(pipline) ? "�ɹ�" : "ʧ��");
    printf("--------�ͷ�Udp����������%s--------\n", server->Dispose() ? "�ɹ�" : "ʧ��");
    
    return 0; 
}


