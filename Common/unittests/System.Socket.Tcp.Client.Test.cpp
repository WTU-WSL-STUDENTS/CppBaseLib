/*
 * @Description: System::Net::Socket�ĵ�Ԫ����
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
    printf("--------Socket�ͻ���--------\n");
    /* test 2 : Create Client With Server Addr & Port */
    Socket* client = new Socket("127.0.0.1", 8888, Tcp_Client);
    if(!client->IsCreateSocketSucess())
    {
        printf("--------Socket�ͻ��˴���ʧ��--------\n");
        return 1;
    }
    /* test 2 : Connect To Server */
    if(!client->ConnectToServer())
    {
        printf("--------Socket�ͻ������ӷ�����ʧ��--------\n");
        return 1;
    }
    printf("--------Socket�ͻ������ӷ������ɹ�--------\n");
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
    printf("--------�˳�Socket%s--------\n", client->WaitExitFlag() ? "�ɹ�" : "ʧ��");
    /* test 5 : Dispose Client All Memory */
    printf("--------�ͷ�Socket%s--------\n", client->Dispose() ? "�ɹ�" : "ʧ��");
    
    return 0;  
}