#include "Socket_Win32.h"
using namespace Socket_Win32_Ult;

WSADATA SocketLib_Win32::wsaData;

using namespace TCP_Win32;
TCP_Packet::TCP_Packet(SocketConfig client) :SocketPacket()
{
    this->client = client;
    clientSize = sizeof(client);
    clientPtr = &this->client;
}

using namespace UDP_Win32;
int UDP_Service::LoopProcess()
{
    //isRunning = true;
    //while (SOCKET_ERROR != RecvFrom(p->client) && isRunning)
    //{
    //    (Proc_Fun*)p->func(this);
    //}
    return !isRunning;  //在运行的时候跳出循环，说明读取数据有问题
}