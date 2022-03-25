/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-15 11:04:20
 * @LastEditors: like
 * @LastEditTime: 2022-03-24 10:14:11
 */
#include <System.Console.hpp>
#include <System.Net.Sockets.Socket.hpp>

using namespace System::Net;
using namespace System::Net::Sockets;

System::Console Console;

void SocketInfoTest()
{
    System::Net::Sockets::Socket tcpSocket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IPPROTO_TCP);

    // Don't allow another socket to bind to this port.
    tcpSocket.SetExclusiveAddressUse(true);

    // The socket will linger for 10 seconds after
    // Socket.Close is called.
    tcpSocket.SetLingerState(LingerOption{true, 10});

    // Disable the Nagle Algorithm for this tcp socket.
    tcpSocket.SetNoDelay(true);

    // Set the receive buffer size to 8k
    tcpSocket.SetReceiveBufferSize(8192);

    // Set the timeout for synchronous receive methods to
    // 1 second (1000 milliseconds.)
    tcpSocket.SetReceiveTimeout(1000);

    // Set the send buffer size to 8k.
    tcpSocket.SetSendBufferSize(8192);

    // Set the timeout for synchronous send methods
    // to 1 second (1000 milliseconds.)
    tcpSocket.SetSendTimeout(1000);

    // Set the Time To Live (TTL) to 42 router hops.
    tcpSocket.SetTtl(42);

    Console.WriteLine("Tcp Socket configured:");
    Console.WriteLine("  ExclusiveAddressUse ", tcpSocket.GetExclusiveAddressUse() ? "Enable" : "Disable");
    Console.WriteLine("  LingerState ", tcpSocket.GetLingerState().l_onoff ? "on" : "off", ", linger time : ", tcpSocket.GetLingerState().l_linger, "s");
    Console.WriteLine("  NoDelay ", tcpSocket.GetNoDelay() ? "Enable" : "Disable");
    Console.WriteLine("  ReceiveBufferSize ", tcpSocket.GetReceiveBufferSize());
    Console.WriteLine("  ReceiveTimeout ", tcpSocket.GetReceiveTimeout());
    Console.WriteLine("  SendBufferSize ", tcpSocket.GetSendBufferSize());
    Console.WriteLine("  SendTimeout ", tcpSocket.GetSendTimeout());
    Console.WriteLine("  Ttl ", (int)tcpSocket.GetTtl());
    // Console.WriteLine("  IsBound ", tcpSocket.IsBound);
    Console.WriteLine("");
}

int main()
{
    SocketInfoTest();
    return 0;
}