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

#define MAX_CONNECT_COUNT (4)

using SocketAsync = System::Net::Sockets::SocketAsyncExtension;
class Server;

struct UserToken
{
    WEAK_PTR(Server) server;
    int ClientId;
};
template<>
class NewItemMemoryDisposePolicy<SocketAsyncEventArgs*>/* 重写 HeapList<SocketAsyncEventArgs*> 的释放机制 */
{
public:
	inline void Free(SocketAsyncEventArgs** item)
	{
		SocketAsync* socket = (*item)->GetAcceptSocket();
        SAVE_DELETE_PTR(socket);
        (*item)->SetAcceptSocket(socket);
        UserToken* token = static_cast<UserToken*>((*item)->GetUserToken());
		SAVE_DELETE_PTR(token);
		(*item)->SetUserToken(token);
		SAVE_DELETE_PTR(*item);
	}
};
using SocketAsyncIOMessagePool = HeapList<SocketAsyncEventArgs*>;

class Server
{
    long m_numConnections;
    long m_receiveBufferSize;
    char* m_bufferManager;
    size_t m_nBufferSize;
    const int opsToPreAlloc = 2;
    SocketAsyncIOMessagePool m_readWritePool;
	SocketAsync* listenSocket;
    long m_totalBytesRead;           // counter of the total # bytes received by the server
    long m_numConnectedSockets;      // the total number of clients connected to the server
    CancellationTokenSource cts;
    Semaphore m_maxNumberAcceptedClients;

public:
    Server(int numConnections = MAX_CONNECT_COUNT, int receiveBufferSize = 0x2000) :
        m_readWritePool(HeapList<SocketAsyncEventArgs*>(numConnections)),
        listenSocket(NULL),
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
        m_maxNumberAcceptedClients.Dispose();
        cts.Cancel();
        if (listenSocket)
        {
			listenSocket->Close();
			delete listenSocket;
			listenSocket = NULL;
        }
		FREE_SOCKET_ASYNC_EVENT_ARGS_BUFFER(m_bufferManager);
    }
    void Init()
    {
        SocketAsyncEventArgs* readWriteEventArg;
        for(int i = 0; i < m_numConnections; i++)
        {
            readWriteEventArg = new SocketAsyncEventArgs;  
            readWriteEventArg->SetUserToken(new UserToken{this, i});
            readWriteEventArg->SetBuffer(m_bufferManager, m_receiveBufferSize * m_numConnections * opsToPreAlloc);
			readWriteEventArg->Completed = [](Object sender, SocketAsyncEventArgs& e)->void
			{
                UserToken* p = static_cast<UserToken*>(sender);
				p->server->ProcessAccept(e);
			};
            m_readWritePool.Add(readWriteEventArg);
        }
    }
    void Start(SocketAddress &addr)
    {
        listenSocket->Bind(addr);
        // start the server with a listen backlog of MAX_CONNECT_COUNT * 2 connections
        listenSocket->Listen(MAX_CONNECT_COUNT * 2/* 允许同时最高并发量一倍的 connect */);
        StartAccept(*m_readWritePool[m_numConnectedSockets]);
        Console.WriteLine("Press any key to terminate the server process....");
        Console.ReadKey();/* 服务端的主线程就此不再使用 */
    }
private:
    void StartAccept(SocketAsyncEventArgs &acceptEventArg)
    {
        while (true)
        {
            cts./*GetToken()->*/ThrowIfCancellationRequested();
            if (m_maxNumberAcceptedClients.WaitOne(500))
            {
                printf("Start accept\n");
                break;
            }
        }
        SocketAsync* p = acceptEventArg.GetAcceptSocket();
        SAVE_DELETE_PTR(p);
        acceptEventArg.SetAcceptSocket(p);

        bool willRaiseEvent = listenSocket->AcceptAsync(acceptEventArg);
        if (!willRaiseEvent)
        {
            printf("Will not raise accept event\n");
            ProcessAccept(acceptEventArg);
        }
    }
	void ProcessAccept(SocketAsyncEventArgs& e)
    {
        VOIDRET_ASSERT(0 == e.GetSocketError());
        Interlocked<long>::Increment(m_numConnectedSockets);
        Console.WriteLine("Client connection accepted. There are ", m_numConnectedSockets, " clients connected to the server");

        /* AcceptAsync finish if receive a message */
        e.Completed = [](Object sender, SocketAsyncEventArgs& e)->void
        {
            UserToken* p = static_cast<UserToken*>(sender);
            switch (e.GetLastOperation())
            {
                case SocketAsyncOperation::Receive:
                    p->server->ProcessReceive(e);
                    break;
                case SocketAsyncOperation::Send:
                    p->server->ProcessSend(e);
                    break;
                default:
                    ERROR_ASSERT(false, "The last operation completed on the socket was not a receive or send");
            }
        };
        e.SetBuffer(0, 24);
        if (!e.GetAcceptSocket()->ReceiveAsync(e))
        {
            ProcessReceive(e);
        }

        // Accept the next connection request
        StartAccept(*m_readWritePool[m_numConnectedSockets]);
    }
    // This method is invoked when an asynchronous receive operation completes.
    // If the remote host closed the connection, then the socket is closed.
    // If data was received then the data is echoed back to the client.
    //
    void ProcessReceive(SocketAsyncEventArgs &e)
    {
         long byteCount = e.GetBytesTransferred();
         if (0 < byteCount && 0 == e.GetSocketError())
         {
             //increment the count of the total bytes receive by the server
             Interlocked<long>::Add(m_totalBytesRead, byteCount);
             Console.WriteLine("The server has read a total of ", m_totalBytesRead, " bytes");

			 //echo the data received back to the client
			 e.SetBuffer(0, byteCount);
			 bool willRaiseEvent = e.GetAcceptSocket()->SendAsync(e);
			 if (!willRaiseEvent)
			 {
				 ProcessSend(e);
			 }
         }
         else
         {
             int n = GetLastError();
             CloseClientSocket(e);
         }
    }

    // This method is invoked when an asynchronous send operation completes.
    // The method issues another receive on the socket to read any additional
    // data sent from the client
    //
    // <param name="e"></param>
    void ProcessSend(SocketAsyncEventArgs& e)
    {
         if (e.GetSocketError())
		 {
			 CloseClientSocket(e);
             return;
         }
		 printf("Reply success\n");
		 if (!e.GetAcceptSocket()->GetConnected())
		 {
			 CloseClientSocket(e);
			 return;
		 }
		 bool willRaiseEvent = e.GetAcceptSocket()->ReceiveAsync(e);
		 if (!willRaiseEvent)
		 {
			 ProcessReceive(e);/* 进行下一轮交互 */
		 }
    }
    void CloseClientSocket(SocketAsyncEventArgs& e)
	{
        /* 只支持客户端先进后出 */
		Interlocked<long>::Decrement(m_numConnectedSockets);
		/*SocketAsync* s = m_readWritePool[m_numConnectedSockets]->GetAcceptSocket();
        delete s;
        s = NULL;
        m_readWritePool[m_numConnectedSockets]->SetAcceptSocket(s);*/
		m_maxNumberAcceptedClients.Release();
        UserToken* p = static_cast<UserToken*>(e.GetUserToken());
        Console.WriteLine("A client has been disconnected from the server. There are ", m_numConnectedSockets, " clients connected to the server");
    }
};

class Client
{
private:
    SocketAsync m_socket;
public:
    Client() : m_socket(SocketAsync(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IPPROTO_TCP))
    {
        /* 1. connect to server */
        SocketAddress addr(IPAddress::Loopback, 9999);
        m_socket.Connect(addr);
        printf("connect success\n");
        /* 2. send request*/
        char buffer[] = "Hello from async client";
        SocketError nErrorCode;
		WINAPI_ASSERT(sizeof(buffer) == m_socket.Send(buffer, sizeof(buffer), SocketFlags::None, nErrorCode), "send failed");
		WINAPI_ASSERT(0 == nErrorCode, "send failed");
		printf("send buffer : %zd , %s\n", sizeof(buffer), buffer);
        /* 3. recv reply */
		memset(buffer, 0, sizeof(buffer));
		int nReceivedLen = m_socket.Receive(buffer, sizeof(buffer), SocketFlags::None, nErrorCode);
		if (10054 == nErrorCode)
		{
			printf("An existing connection was forcibly closed by the remote host.\n");
			return;
		}
		WINAPI_ASSERT(0 == nErrorCode, "receive failed");
		printf("receive buffer : %d, %zd , %s\n", nReceivedLen, sizeof(buffer), buffer);
        /* 4. block until server exit */
        while (m_socket.GetConnected())
        {
        }
        printf("Server shutdown, terminate the client process\n");
    }
};

int main(int argc, char** argv, char** argEnv)
{
#ifdef _DEBUG
    MEMORYLEAK_ASSERT;
#endif
    if(1 == argc)
        goto SERVER;
	
    ERROR_ASSERT(2 == argc, "\n    Start ipv4 server / client : System.Net.Sockets.SocketAsync.Test.exe s[server]/c[client]");
	if (0 == strcmp("c", argv[1]))
	{
        printf("Begin async tcp client\n");
        Client c;
	}
	else if (0 == strcmp("s", argv[1]))
	{
SERVER:
		Server server;
		server.Init();
		SocketAddress addr(IPAddress::Loopback, 9999);
		server.Start(addr);
	}
    return 0;
}