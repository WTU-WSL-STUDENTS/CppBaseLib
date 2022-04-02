/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-28 13:59:00
 * @LastEditors: like
 * @LastEditTime: 2022-03-31 15:06:04
 */
#include <System.Net.Sockets.SocketAsync.hpp>
#include <System.Threading.Interlocked.hpp>
#include <System.Threading.Semaphore.hpp>
#include <System.List.hpp>
#include <System.Console.hpp>

using namespace System::Net;
using namespace System::Net::Sockets;
using namespace System::Threading;
using System::Object;
System::Console Console;

using SocketAsync = System::Net::Sockets::SocketAsyncExtension;

class Server
{
    long m_numConnections;
    long m_receiveBufferSize;
    char* m_bufferManager;
    size_t m_nBufferSize;
    const int opsToPreAlloc = 2;
    HeapList<SocketAsyncEventArgs*> m_readWritePool;
	SocketAsyncEventArgs           m_acceptEventArg;
	SocketAsync* listenSocket;
    long m_totalBytesRead;           // counter of the total # bytes received by the server
    long m_numConnectedSockets;      // the total number of clients connected to the server
    Semaphore m_maxNumberAcceptedClients;
    // System::EventHandler<SocketAsyncEventArgs&> IO_Completed;
    // System::EventHandler<SocketAsyncEventArgs&> AcceptEventArg_Completed;
public:
    Server(int numConnections = 4, int receiveBufferSize = 0x2000) :
        m_readWritePool(HeapList<SocketAsyncEventArgs*>(numConnections)),
        m_maxNumberAcceptedClients(Semaphore(numConnections, numConnections))
    {
        m_totalBytesRead = 0;
        m_numConnectedSockets = 0;
        m_numConnections = numConnections;
        m_receiveBufferSize = receiveBufferSize;
        m_nBufferSize = m_receiveBufferSize;
        m_nBufferSize *= m_numConnections;
        m_nBufferSize *= opsToPreAlloc;
        // allocate buffers such that the maximum number of sockets can have one outstanding read and
        //write posted to the socket simultaneously
		m_bufferManager = NEW_SOCKET_ASYNC_EVENT_ARGS_BUFFER(m_nBufferSize);
		memset(m_bufferManager, 0, m_nBufferSize);
        listenSocket = new SocketAsync(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IPPROTO_TCP);
    }
    ~Server()
	{
		for (int i = 0; i < m_numConnectedSockets; i++)
		{
			delete m_readWritePool[i]->GetAcceptSocket();
		}
		listenSocket->Close();
		delete listenSocket;
        listenSocket = NULL;
        FREE_SOCKET_ASYNC_EVENT_ARGS_BUFFER(m_bufferManager);
    }
    void Init()
    {
        SocketAsyncEventArgs* readWriteEventArg;
        for(int i = 0; i < m_numConnections; i++)
        {
            readWriteEventArg = new SocketAsyncEventArgs;  
            readWriteEventArg->SetUserToken(this);
            readWriteEventArg->SetBuffer(m_bufferManager, m_receiveBufferSize * m_numConnections * opsToPreAlloc);
            readWriteEventArg->Completed = [](Object sender, SocketAsyncEventArgs& e)->void
            {
                Server* p = static_cast<Server*>(sender);
                switch (e.GetLastOperation())
                {
                    case SocketAsyncOperation::Receive:
                        p->ProcessReceive(e);
                        break;
                    case SocketAsyncOperation::Send:
                        p->ProcessSend(e);
                        break;
                    default:
                        ERROR_ASSERT(false, "The last operation completed on the socket was not a receive or send");
                }
            };
            m_readWritePool.Add(readWriteEventArg);
        }
        m_acceptEventArg.Completed = [](Object sender, SocketAsyncEventArgs &e)->void
        {
            Server* p = static_cast<Server*>(sender);
            p->ProcessAccept(e);
        };
        m_acceptEventArg.SetBuffer(m_bufferManager, m_receiveBufferSize * m_numConnections * opsToPreAlloc);
        m_acceptEventArg.SetUserToken(this);
    }
    void Start(SocketAddress &addr)
    {
        listenSocket->Bind(addr);
        // start the server with a listen backlog of 100 connections
        listenSocket->Listen(100);
        listenSocket->m_socketAsynclibraryInitlializer = new IOCPExtensionInitlializer(listenSocket->GetHandle());
        // post accepts on the listening socket
        StartAccept(m_acceptEventArg);

        Console.WriteLine("Press any key to terminate the server process....");
        Console.ReadKey();
    }
private:
    void StartAccept(SocketAsyncEventArgs &acceptEventArg)
    {
        m_maxNumberAcceptedClients.WaitOne();
        bool willRaiseEvent = listenSocket->AcceptAsync(acceptEventArg);
        if (!willRaiseEvent)
        {
            printf("Will not raise accept event\n");
            ProcessAccept(acceptEventArg);
        }
    }
    void ProcessAccept(SocketAsyncEventArgs &e)
    {
        VOIDRET_ASSERT(0 == e.GetSocketError());
        SocketAsyncEventArgs* readEventArgs = m_readWritePool[m_numConnectedSockets];
        const SocketAsync* p = e.GetAcceptSocket();
        readEventArgs->SetAcceptSocket(p);/* 转移 socket 资源所有权*/
        Interlocked<long>::Increment(m_numConnectedSockets);
        Console.WriteLine("Client connection accepted. There are ", m_numConnectedSockets, " clients connected to the server");

        // Get the socket for the accepted client connection and put it into the
        //ReadEventArg object user token

        // As soon as the client is connected, post a receive to the connection
        // bool willRaiseEvent = e.GetAcceptSocket()->ReceiveAsync(readEventArgs);
        // if(!willRaiseEvent){
        //     ProcessReceive(*readEventArgs);
        // }

        // Accept the next connection request
         StartAccept(e);
    }
    // This method is invoked when an asynchronous receive operation completes.
    // If the remote host closed the connection, then the socket is closed.
    // If data was received then the data is echoed back to the client.
    //
    void ProcessReceive(SocketAsyncEventArgs &e)
    {
        // // check if the remote host closed the connection
        // // AsyncUserToken token = (AsyncUserToken)e.UserToken;
        // long byteCount = e.GetBytesTransferred();
        // ERROR_ASSERT(-1 < byteCount, "DWORD to long failed");
        // if (0 < byteCount && 0 == e.GetSocketError())
        // {
        //     //increment the count of the total bytes receive by the server
        //     Interlocked<long>::Add(m_numConnectedSockets, byteCount);
        //     Console.WriteLine("The server has read a total of ", m_totalBytesRead, " bytes");

        //     //echo the data received back to the client
        //     e.SetBuffer(e.Offset, e.BytesTransferred);
        //     bool willRaiseEvent = e.GetAcceptSocket()->SendAsync(e);
        //     if (!willRaiseEvent)
        //     {
        //         ProcessSend(e);
        //     }
        // }
        // else
        // {
        //     CloseClientSocket(e);
        // }
    }

    // This method is invoked when an asynchronous send operation completes.
    // The method issues another receive on the socket to read any additional
    // data sent from the client
    //
    // <param name="e"></param>
    void ProcessSend(SocketAsyncEventArgs e)
    {
        // if (0 == e.GetSocketError())
        // {
        //     // done echoing data back to the client
        //     AsyncUserToken token = (AsyncUserToken)e.UserToken;
        //     // read the next block of data send from the client
        //     bool willRaiseEvent = token.Socket.ReceiveAsync(e);
        //     if (!willRaiseEvent)
        //     {
        //         ProcessReceive(e);
        //     }
        // }
        // else
        // {
        //     CloseClientSocket(e);
        // }
    }

    void CloseClientSocket(SocketAsyncEventArgs e)
    {
        // AsyncUserToken token = e.UserToken as AsyncUserToken;

        // token.Socket.Shutdown(SocketShutdown.Send);
        // token.Socket.Close();

        // // decrement the counter keeping track of the total number of clients connected to the server
        // Interlocked<long>::Decrement(m_numConnectedSockets);

        // // Free the SocketAsyncEventArg so they can be reused by another client
        // m_readWritePool.Push(e);

        // m_maxNumberAcceptedClients.Release();
        // Console.WriteLine("A client has been disconnected from the server. There are {0} clients connected to the server", m_numConnectedSockets);
    }

};

int main(int argc, char** argv, char** argEnv)
{
#ifdef _DEBUG
    MEMORYLEAK_ASSERT;
#endif
	Server server;
	server.Init();
	SocketAddress addr(IPAddress::Loopback, 9999);
	server.Start(addr);
    return 0;
}