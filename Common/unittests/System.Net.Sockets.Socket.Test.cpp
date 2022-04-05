/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-15 11:04:20
 * @LastEditors: like
 * @LastEditTime: 2022-03-27 20:46:58
 */
#include <System.Net.Sockets.Socket.hpp>
#include <System.Threading.Interlocked.hpp>
#include <System.Threading.Semaphore.hpp>
#include <System.List.hpp>
#include <System.Console.hpp>

using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Threading;
using System::Object;
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
    tcpSocket.Close();
}
void AnsycSocket()
{
    System::Net::Sockets::Socket tcpSocket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IPPROTO_TCP);
    SocketAddress addr(IPAddress::Loopback, 9999);
    tcpSocket.Bind(addr);
    tcpSocket.Listen(1);
    //tcpSocket.SetBlocking(false);
    System::Net::Sockets::Socket* client = tcpSocket.Accept(addr);
    delete client;
    client = NULL;
    // tcpSocket.AcceptAnsyc(addr);
    // while (1)
    // {
    //     tcpSocket.Accept(addr);
    // }
    
}
// class AnsycSocketTest
// {
// private:
//     long m_numConnections;
//     long m_receiveBufferSize;
//     char* m_bufferManager;
// #define opsToPreAlloc 2
//     Socket listenSocket;
//     HeapList<SocketAsyncEventArgs*> m_readWritePool;
//     long m_totalBytesRead;           // counter of the total # bytes received by the server
//     long m_numConnectedSockets;      // the total number of clients connected to the server
//     Semaphore* m_maxNumberAcceptedClients;
//     System::EventHandler<SocketAsyncEventArgs&> IO_Completed;
//     System::EventHandler<SocketAsyncEventArgs&> AcceptEventArg_Completed;
    
// public:
//     AnsycSocketTest() : listenSocket(Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IPPROTO_TCP))
//     {
//         m_totalBytesRead = 0;
//         m_numConnectedSockets = 0;
//         m_numConnections = 3;
//         m_receiveBufferSize = 0xfff;
//         m_bufferManager = new char[m_receiveBufferSize * m_numConnections * opsToPreAlloc];
//         IO_Completed = [](Object sender, SocketAsyncEventArgs& e)->void
//         {
//             switch (e.GetLastOperation())
//             {
//                 case SocketAsyncOperation::Receive:
//                     ProcessReceive(e);
//                     break;
//                 case SocketAsyncOperation::Send:
//                     ProcessSend(e);
//                     break;
//                 default:
//                     ERROR_ASSERT(false, "The last operation completed on the socket was not a receive or send");
//             }
//         };
//         AcceptEventArg_Completed = [](Object sender, SocketAsyncEventArgs& e)->void
//         {
//             ProcessAccept(e);
//         };
//         for(int i = 0; i < m_numConnections; i++)
//         {
//             SocketAsyncEventArgs* readWriteEventArg = new SocketAsyncEventArgs;  
//             readWriteEventArg->Completed = IO_Completed;
//             m_readWritePool.Add(readWriteEventArg );
//         }
//         m_maxNumberAcceptedClients = new Semaphore(m_numConnections, m_numConnections);
//         SocketAddress addr(IPAddress::Loopback, 9999);
//         listenSocket.Bind(addr);
//         listenSocket.Listen(m_numConnections);
//         StartAccept(NULL);
//         Console.WriteLine("Press any key to terminate the server process....");
//         Console.ReadKey();
//     }
//     void StartAccept(SocketAsyncEventArgs* acceptEventArg)
//     {
//         if (acceptEventArg == NULL)
//         {
//             acceptEventArg = new SocketAsyncEventArgs();
//             acceptEventArg->Completed = AcceptEventArg_Completed;
//         }
//         else
//         {
//             Socket* p = acceptEventArg->GetAccecptSocket();
//             delete p;
//             p = NULL;
//             acceptEventArg->SetAccecptSocket(p);
//         }

//         m_maxNumberAcceptedClients->WaitOne();
//         bool willRaiseEvent = listenSocket.AcceptAsync(*acceptEventArg);
//         if (!willRaiseEvent)
//         {
//             ProcessAccept(*acceptEventArg);
//         }
//     }
//     void ProcessAccept(SocketAsyncEventArgs e)
//     {
//         Interlocked<long>::Increment(m_numConnectedSockets);
//         Console.WriteLine("Client connection accepted. There are ", m_numConnectedSockets, " clients connected to the server");

//         // Get the socket for the accepted client connection and put it into the
//         //ReadEventArg object user token
//         SocketAsyncEventArgs* readEventArgs = m_readWritePool[m_numConnectedSockets];
//         ((AsyncUserToken)readEventArgs.UserToken).Socket = e.AcceptSocket;

//         // As soon as the client is connected, post a receive to the connection
//         bool willRaiseEvent = e.AcceptSocket.ReceiveAsync(readEventArgs);
//         if(!willRaiseEvent){
//             ProcessReceive(readEventArgs);
//         }

//         // Accept the next connection request
//         StartAccept(e);
//     }
//     void IO_Completed(Object sender, SocketAsyncEventArgs& e)
//     {
//         // determine which type of operation just completed and call the associated handler
//         switch (e.LastOperation)
//         {
//             case SocketAsyncOperation.Receive:
//                 ProcessReceive(e);
//                 break;
//             case SocketAsyncOperation.Send:
//                 ProcessSend(e);
//                 break;
//             default:
//                 throw new ArgumentException("The last operation completed on the socket was not a receive or send");
//         }
//     }

// }
int main()
{
#ifdef _DEBUG
    MEMORYLEAK_ASSERT;
#endif
    SocketInfoTest();
    AnsycSocket();
    return 0;
}