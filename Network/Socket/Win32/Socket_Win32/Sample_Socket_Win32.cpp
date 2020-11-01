// Socket_Win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include "Socket_Win32.h"
#include <process.h>



using namespace UDP_Win32;
using namespace TCP_Win32;

BOOL WINAPI HandleCtrl(DWORD dwType)
{
    switch (dwType)
    {
    case CTRL_CLOSE_EVENT:
        //关闭占用
        break;
    default:
        break;
    }
    return false;
}

struct UDP_Server_Loop_Param
{
    bool runing;
    UDP_Service ser;
    I_Prroto clientCfg;
};
static unsigned __stdcall UDP_Server_Loop(void* params)
{
    UDP_Server_Loop_Param* p = (UDP_Server_Loop_Param*)params;
    int code;
    int size = sizeof(p->clientCfg);
    while (p->runing)
    {
        code = p->ser.RecvFrom(p->clientCfg, size);
        if (SOCKET_ERROR == code)
        {
            std::cout << "接收消息失败！错误码：" << code;
            return code;
        }
        else
        {
            std::cout << "[UDP][RecivedMsg]" << std::string(p->ser.PopDataFromBuffer()) << std::endl;
        }
    }
    return 1;
}
struct Param
{
    UDP_Service ser;
    SocketConfig client;
};
static unsigned __stdcall PrintPack(void* params)
{
    Param* p = (Param*)params;
    int size = sizeof(p->client);
    while (SOCKET_ERROR != p->ser.RecvFrom(p->client, size))
    {
        std::cout << "[UDP服务器发送数据]:" << p->ser.PopDataFromBuffer() << std::endl;
    }
    return -1;
}

static int UDP_TEST()
{
    SocketConfig serverCfg("127.0.0.1", 12345);
    SocketConfig clientCfg("127.0.0.1", 12346);
    int len = sizeof(serverCfg);
    UDP_Service ser = UDP_Service(serverCfg);
    UDP_Client client = UDP_Client();
    int size = sizeof(clientCfg);

    char* temp = new char[Max_Suit_Packed_Size];
    scanf("%s", temp);
    int code = client.SendTo(temp, Max_Suit_Packed_Size, ser);
    if (SOCKET_ERROR == code)
    {
        WORD erroCode = GetLastError();
        throw "";
    }
    while (1)
    {
        if (SOCKET_ERROR == ser.RecvFrom(clientCfg, size))
            throw"";
        std::cout << "[UDP服务器发送数据]:" << ser.PopDataFromBuffer() << std::endl;
        temp = new char[Max_Suit_Packed_Size];
        scanf("%s", temp);
        code = client.SendTo(temp, Max_Suit_Packed_Size, ser);
        if (SOCKET_ERROR == code)
            throw "";
    }
    Param cfg = { ser,clientCfg };
    HANDLE 	t = (HANDLE)_beginthreadex(0, 0, PrintPack, &cfg, CREATE_SUSPENDED, 0);
    ResumeThread(t);

    int idx = 0;
    while (idx<5)
    {
        char* temp = new char[Max_Suit_Packed_Size];
        scanf("%s", temp);
        client.SendTo(temp, Max_Suit_Packed_Size, ser);
        std::cout << "[UDP客户端发送数据]:" << temp << "\n";
        idx++;
    }
  

    Sleep(1);

    client.CloseSocket();
    ser.CloseSocket();
    return 1;
}


struct TCP_Server_Loop_Param
{
    bool runing;
    TCP_Client ser;
    SocketConfig clientCfg;
    int len;
};
static unsigned __stdcall TCP_Server_Loop(void* params)
{
    TCP_Server_Loop_Param* p = (TCP_Server_Loop_Param*)params;
    int code;
<<<<<<< Updated upstream
    //int code = client.Send("asdf", &serverCfg, sizeof(serverCfg));
    //if (SOCKET_ERROR == code)
    //{
    //    std::cout << "发送消息失败！错误码：" << code;
    //    return code;
    //}
    ////收消息
    //code = ser.ReciveFrom(&clientCfg, &len);
    //if (SOCKET_ERROR == code)
    //{
    //    std::cout << "接收消息失败！错误码：" << code;
    //    return code;
    //}
    //else
    //{
    //    std::cout << std::string(ser.PopDataFromBuffer());
    //}
    UDP_Server_Loop_Param cfg = { true,ser,clientCfg,len };
    
    HANDLE 	t = (HANDLE)_beginthreadex(0, 0, UDP_Server_Loop, &cfg, CREATE_SUSPENDED, 0);
=======
    while (p->runing)
    {
        code = p->ser.Recive();
        if (SOCKET_ERROR == code)
        {
            if (SOCKET_ERROR == p->ser.Connect())
                continue;
            else
            {
                std::cout << "接收消息失败！错误码：" << code;
                return code;
            }
        }
        std::cout << "接收到消息：" << std::string(p->ser.PopDataFromBuffer()) << std::endl;
    }
    return 1;
}

//服务端发送消息
static int TCP_TEST()
{
    SocketConfig serverCfg("127.0.0.1", 12345);
    SocketConfig clientCfg("127.0.0.1", 12346);
    
    TCP_Service ser = TCP_Service(serverCfg, clientCfg);
    TCP_Client client = TCP_Client(clientCfg, serverCfg);
    ser.CreateListen();
    client.Connect();
    int len = sizeof(clientCfg);
    TCP_Server_Loop_Param cfg = { true,client,clientCfg,len };
    HANDLE 	t = (HANDLE)_beginthreadex(0, 0, TCP_Server_Loop, &cfg, CREATE_SUSPENDED, 0);
>>>>>>> Stashed changes
    ResumeThread(t);
    DWORD dwExitCode;
    GetExitCodeThread(t, &dwExitCode);
    if (dwExitCode != STILL_ACTIVE)
    {
        throw "";
    }

    SocketConfig* temp = &serverCfg;
    int tempSize = sizeof(serverCfg);
    std::cout << "请输入字符串：";
    while (true)
    {
        char* input = new char[Max_Suit_Packed_Size];
        scanf("%s", input);
        //std::cout << ":完成,正在发送字符串：" << input << std::endl;
<<<<<<< Updated upstream
        int code = client.SendTo(input, Max_Suit_Packed_Size, temp, tempSize);
=======
        int code = client.Send(input, Max_Suit_Packed_Size);
>>>>>>> Stashed changes
        if (SOCKET_ERROR == code)
        {
            std::cout << "发送消息失败！错误码：" << code;
            return code;
        }
        std::cout << "消息发送完毕" << std::endl;
        if (input[0] == 48)  //48对应键盘0
        {
            cfg.runing = false;
            break;
        }
    }

    Sleep(1);
    GetExitCodeThread(t, &dwExitCode);
    if (dwExitCode == STILL_ACTIVE)
    {
        throw "";
    }
    client.CloseSocket();
    ser.CloseSocket();
    return 1;
}
int main()
{
    SetConsoleCtrlHandler(HandleCtrl, false);
    SocketLib_Win32::StartUp();
    std::cout << "Hello World!\n" << SocketLib_Win32::LibVersionA() << std::endl;

    //TCP_TEST();
    UDP_TEST();
    int code = SocketLib_Win32::Close();
    if (SOCKET_ERROR == code)
    {
        std::cout << "关闭网络库失败！错误码：" << code;
        return code;
    }
    return 1;
}
