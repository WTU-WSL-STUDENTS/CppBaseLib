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
