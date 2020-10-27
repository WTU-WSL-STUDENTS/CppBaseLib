// Socket_Win32.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Socket_Win32.h"
#include <process.h>
using namespace UDP_Win32;

struct UDP_Server_Loop_Param
{
    bool runing;
    UDP_Service ser;
    SocketConfig clientCfg;
    int len;
};
static unsigned __stdcall UDP_Server_Loop(void* params)
{
    UDP_Server_Loop_Param* p = (UDP_Server_Loop_Param*)params;
    int code;
    while (p->runing)
    {
        code = p->ser.ReciveFrom(&p->clientCfg, &p->len);
        if (SOCKET_ERROR == code)
        {
            std::cout << "接收消息失败！错误码：" << code;
            return code;
        }
        else
        {
            std::cout << "接收到消息：" << std::string(p->ser.PopDataFromBuffer()) << std::endl;
        }
    }
    return 1;
}

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

int main()
{
    SetConsoleCtrlHandler(HandleCtrl, false);
    SocketLib_Win32::StartUp();
    std::cout << "Hello World!\n" << SocketLib_Win32::LibVersionA() << std::endl;

    SocketConfig serverCfg("127.0.0.1", 12345);
    SocketConfig clientCfg("127.0.0.1", 12346);
    int len = sizeof(clientCfg);
    UDP_Service ser =  UDP_Service(serverCfg);

    //发消息
    UDP_Client client = UDP_Client(clientCfg);
    int code;
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
        int code = client.SendTo(input, Max_Suit_Packed_Size, temp, tempSize);
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
    code = SocketLib_Win32::Close();
    if (SOCKET_ERROR == code)
    {
        std::cout << "关闭网络库失败！错误码：" << code;
        return code;
    }
    return 1;
}
