/*
 * @Description: System::Net::Socket�ĵ�Ԫ����
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
    printf("--------Socket������--------\n");
    server = new Socket("127.0.0.1", 8888, Tcp_Server);
    if(!server->IsCreateSocketSucess())
    {
        printf("--------Socket����������ʧ��--------\n");
    }
    server->SetAnsynAccepet(true);/* û�м�ͬ����������ͻ����첽����ʱ��StartServer�м���Ҫ���һС�� */
    int pipline;
    /* test 1 : Start Server At Pipline_0 */
    if(0 > (pipline = server->StartServer(ServerCallBack)))
    {
        printf("--------Socket������Pipline_%d����ʧ��--------\n", pipline);
    }
    else
    {
        printf("--------Socket������Pipline_%d�����ɹ�--------\n", pipline);
    }
    /* test 2 : Exit Pipline_0 At Accecpt */
    // server->StopServer(pipline);
    /* test 3 : Exit Pipline_0 At Recv */
    // Client_0 Send cmd "exit"
    Thread::Sleep(50);/* �������Զ�����ͨѶ�ܵ���AUTO_PIPLINE_MANAGE��������£� ���Ҫ���첽�ķ�ʽ�ȴ��ͻ���accecpt��������������ʱ������м���õ�һС��*/
    int pipline1;
    /* test 1 : Start Server At Pipline_1 */
    if(0 > (pipline1 = server->StartServer(ServerCallBack)))
    {
        printf("--------Socket������Pipline_%d����ʧ��--------\n", pipline);
    }
    else
    {
        printf("--------Socket������Pipline_%d�����ɹ�--------\n", pipline1);
    }
    /* test 2 : Exit Pipline_1 At Accecpt */
    // server->StopServer(pipline1);
    /* test 3 : Exit Pipline_1 At Recv */
    // Client_1 Send cmd "exit"
    /* test 4 : Block current thread to wait Pipline_1 Finished*/
    printf("--------�˳�Pipline_%d��Ϣ�ܵ�%s--------\n", pipline1, server->WaitExitFlag(pipline1) ? "�ɹ�" : "ʧ��");
    /* test 4 : Block current thread to wait Pipline_0 Finished*/
    printf("--------�˳�Pipline_%d��Ϣ�ܵ�%s--------\n", pipline, server->WaitExitFlag(pipline) ? "�ɹ�" : "ʧ��");
    /* test 5 : Dispose Server All Memory */
    printf("--------�ͷ�Socket%s--------\n", server->Dispose() ? "�ɹ�" : "ʧ��");
    
    return 0; 
}


