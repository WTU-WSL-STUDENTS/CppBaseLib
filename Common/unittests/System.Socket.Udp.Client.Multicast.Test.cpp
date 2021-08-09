/*
 * @Description: System::Net::Socket�ĵ�Ԫ����
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-04 19:35:30
 * @LastEditors: like
 * @LastEditTime: 2021-08-09 19:55:14
 */
#define _CRT_SECURE_NO_WARNINGS
#include <System.Net.Socket.hpp>

using namespace System::Net;
int main()
{
    printf("--------Udp�鲥�ͻ���--------\n");
    /* test 2 : Create Client With Server Addr & Port */
    Socket* client = new Socket("127.0.0.1", 8888, Udp_Client);
    if(!client->IsCreateSocketSucess())
    {
        printf("--------Udp�鲥�ͻ��˴���ʧ��--------\n");
        return 1;
    }
    int len;
    int sendLen;
    char buf[1024];// = "test buffer";
    while(0 < (len = scanf("%s", &buf)))
    {
        /* test 3 : Client Send Buffer */
        if(1 > (sendLen = client->SendUdpToServer(buf, strlen(buf))))
        {
            break;
        }
        printf("DataLen:%d, SendedLen:%d\n", strlen(buf), sendLen);
    }
    printf("--------�ͷ�Udp�鲥������%s--------\n", client->Dispose() ? "�ɹ�" : "ʧ��");
    
    return 0;  
}