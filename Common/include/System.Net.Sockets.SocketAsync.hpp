/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-28 10:30:52
 * @LastEditors: like
 * @LastEditTime: 2022-03-31 15:02:07
 */
#ifndef SYSTEM_NET_SOCKET_SOCKET_ASYNC_HPP
#define SYSTEM_NET_SOCKET_SOCKET_ASYNC_HPP
#include <System.Net.Sockets.Socket.hpp>
#include <System.Threading.Interlocked.hpp>
#include <System.Threading.Tasks.Task.hpp>
#include <System.Action.hpp>
#include <mswsock.h>

/**
 * @brief AcceptEx 中的 dwLocalAddressLength / dwRemoteAddressLength 参数
 * https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex
 * This value must be at least 16 bytes more than the maximum address length for the transport protocol in use
 * 
 */
#define IOCP_SOCKET_ADDR_BUF_SIZE (sizeof(sockaddr_in) + 16)
/**
 * @brief 依据客户定义的 bufferSize 计算完成端口实际的 BufferSize 
 * 
 */
#define GET_IOCP_SOCKET_BUFFER(bufferSize) ((bufferSize) + IOCP_SOCKET_ADDR_BUF_SIZE * 2)
#define NEW_SOCKET_ASYNC_EVENT_ARGS_BUFFER(bufferSize) (new char[GET_IOCP_SOCKET_BUFFER(bufferSize)])
#define FREE_SOCKET_ASYNC_EVENT_ARGS_BUFFER(bufferOwner)\
    do{                                                 \
        delete[] bufferOwner;                           \
        bufferOwner = NULL;                             \
    }while(0)
namespace System::Net::Sockets
{  
    class SocketAsyncEventArgs;
};

class System::Net::Sockets::SocketAsyncEventArgs
{
friend class SocketAsyncExtension;
private:
    WEAK_PTR(SocketAsyncExtension)  m_Socket;
	WSAOVERLAPPED           m_olOverlap;
    WSABUF                  m_buffer;
    int                     m_nbyteTransfered;
public:
    EventHandler<SocketAsyncEventArgs&> Completed;

    //DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(SocketAsyncEventArgs)
    SocketAsyncEventArgs() : m_Socket(NULL), m_olOverlap({ 0 }), m_buffer({0}), m_nbyteTransfered(0),
        Completed(NULL), m_LastOperation(SocketAsyncOperation::None),
        m_SocketError(0), m_SocketFlags(SocketFlags::None), m_UserToken(NULL)
    {
        //SOCKETAPI_ASSERT(m_olOverlap.hEvent = WSACreateEvent(), "Create overlap failed");
    }
    ~SocketAsyncEventArgs()
    {
        //WSACloseEvent(m_olOverlap.hEvent);
    }
    /**
     * @brief AcceptAsync 接口返回的 Socket 对象
     * 
     */
    DECLARE_INDEXER(SocketAsyncExtension*, AcceptSocket, 
    { 
        return m_Socket; 
    }, 
    {
        m_Socket = const_cast<SocketAsyncExtension*>(SETTER_VALUE);
        MemoryBarrier(); 
    })
    /**
     * @brief 异步套接字方法的用户缓冲区
     * system owns these buffers and the application may not access them
     * 
     */
    DECLARE_GETTER(WEAK_PTR(char), Buffer, 
    {
        return m_buffer.buf;
    })
    /**
     * @brief 获取在完成端口中传输的字节数, -1 代表 IOCP 出错
     * 
     */
	DECLARE_GETTER(DWORD, BytesTransferred,
	{
		return m_nbyteTransfered;
	})
    /**
     * @brief 用户缓冲区的长度
     * 
     */
	DECLARE_GETTER(int, Count,
	{
		return m_buffer.len;
	})
    /**
     * @brief ConnectAsync 接口返回的 Socket 对象
     * 
     */
    DECLARE_GETTER(SocketAsyncExtension*, ConnectSocket,
    {
        return m_Socket;
    })
    /**
	 * @brief 获取 IO 操作的类型
	 *
	 */
    DECLARE_DATAWRAPPER_INDEXER(SocketAsyncOperation, LastOperation)
    /**
     * @brief 获取或设置异步套接字操作的结果
     * 1. IOCP 无数据， 返回异步等待结果
     * 2. IOCP 有数据， 返回同步 IO 操作结果
     * 
     */
    DECLARE_DATAWRAPPER_INDEXER(DWORD, SocketError)
    
    DECLARE_DATAWRAPPER_INDEXER(SocketFlags, SocketFlags)
    DECLARE_DATAWRAPPER_INDEXER(Object, UserToken)

    void SetBuffer(WEAK_PTR(char) buffer, int count)
    {
        m_buffer.buf = buffer;
        m_buffer.len = count;
    }
    void SetBuffer(int offset, int count)
    {
		ERROR_ASSERT(m_buffer.buf, "need SetBuffer first");
        m_buffer.buf += offset;
		m_buffer.len = count;
    }
};  
/**
 * @brief 完成端口函数指针初始化
 * 
 */
class IOCPExtensionInitlializer
{
#define INIT_SOCKET_IOCP_FUNC(capitalFuncName) (GetSocketAsyncFunctionFromGuid<LPFN_##capitalFuncName>(socket, GUID(WSAID_##capitalFuncName)))
public:
    LPFN_ACCEPTEX               AcceptEX;      
	/*LPFN_CONNECTEX              ConnectEX;
	LPFN_DISCONNECTEX           DisconnectEX;
	LPFN_GETACCEPTEXSOCKADDRS   GetAcceptExSockAddrs;
	LPFN_TRANSMITFILE           TransmitFile;
	LPFN_TRANSMITPACKETS        TransmitPackets;
	LPFN_WSARECVMSG             WSARecvMsg;
	LPFN_WSASENDMSG             WSASendMsg;*/
    IOCPExtensionInitlializer(SOCKET socket)
    {  
        AcceptEX            = INIT_SOCKET_IOCP_FUNC(ACCEPTEX);
        /*ConnectEX           = INIT_SOCKET_IOCP_FUNC(CONNECTEX);
        DisconnectEX        = INIT_SOCKET_IOCP_FUNC(DISCONNECTEX);
        GetAcceptExSockAddrs= INIT_SOCKET_IOCP_FUNC(GETACCEPTEXSOCKADDRS);
        TransmitFile        = INIT_SOCKET_IOCP_FUNC(TRANSMITFILE);
        TransmitPackets     = INIT_SOCKET_IOCP_FUNC(TRANSMITPACKETS);
        WSARecvMsg          = INIT_SOCKET_IOCP_FUNC(WSARECVMSG);
        WSASendMsg          = INIT_SOCKET_IOCP_FUNC(WSASENDMSG);*/
	}
private:
    template<typename TFuncPtr>
    TFuncPtr GetSocketAsyncFunctionFromGuid(SOCKET socket, const GUID& guid)
    {
        TFuncPtr lpfn = NULL;
        DWORD dwBytes;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != WSAIoctl
            (
                socket,
                SIO_GET_EXTENSION_FUNCTION_POINTER,
                const_cast<GUID*>(&guid), sizeof (guid), 
                &lpfn, sizeof (lpfn), 
                &dwBytes, NULL, NULL
            ), 
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaioctl"
        );
        return lpfn;
    }
}; 
#define ASYNC_POLL_INTERVEL (1000 * 100/* 100 ms */)
/**
 * @brief 基于完成端口的异步 Socket 扩展
 * 
 */
class System::Net::Sockets::SocketAsyncExtension final : public Socket
{
private:
#define AcceptEX             m_socketAsynclibraryInitlializer->AcceptEX
#define ConnectEX            m_socketAsynclibraryInitlializer->ConnectEX
#define DisconnectEX         m_socketAsynclibraryInitlializer->DisconnectEX
#define GetAcceptExSockAddrs m_socketAsynclibraryInitlializer->GetAcceptExSockAddrs
#define TransmitFile         m_socketAsynclibraryInitlializer->TransmitFile
#define TransmitPackets      m_socketAsynclibraryInitlializer->TransmitPackets
#define WSARecvMsg           m_socketAsynclibraryInitlializer->WSARecvMsg
#define WSASendMsg           m_socketAsynclibraryInitlializer->WSASendMsg
#define WSAPoll              m_socketAsynclibraryInitlializer->WSAPoll

private:
    HANDLE m_hCompPort;
    bool m_bOwnerIOCP;
    System::Threading::Tasks::Task* m_AcceptAsyncTask = new System::Threading::Tasks::Task([](AsyncState c)->void
    {
        /* 通过调用 m_AcceptAsyncTask.Start(args) 复用 Task */
		CANARY_ASSERT(c);
        SocketAsyncExtension* p = static_cast<SocketAsyncExtension*>(c[0]);
        SocketAsyncEventArgs* e = static_cast<SocketAsyncEventArgs*>(c[1]);
		SocketAsyncExtension* client = static_cast<SocketAsyncExtension*>(c[2]);
		CANARY_ASSERT(NULL == e->GetAcceptSocket());

        /* ERROR_NETNAME_DELETED : AcceptAsync 启用接收第一个包， 即使客户端 connect 成功但是没有发消息, AcceptAsync 此时不会完成。 如果此时客户端终止, 会出现该错误 */
        e->m_nbyteTransfered = p->WaitOne(*e);
        if (SOCKET_ERROR == e->m_nbyteTransfered)
		{
			delete client;
			client = NULL;
			throw std::exception("Accept task canceled");
		}
		e->m_SocketError = 0;/* ERROR_INVALID_PARAMETER */
        e->SetAcceptSocket(client);/* MemoryBarrier */

        e->Completed(e->GetUserToken(), *e);
    },
	CreateAsyncState(this, (Object)NULL/*SocketAsyncEventArgs*/, (Object)NULL/*SOCKET*/));

    System::Threading::Tasks::Task* m_IOAsyncTask = new System::Threading::Tasks::Task([](AsyncState c)->void
    {
        /* 通过调用 m_AcceptAsyncTask.Start(args) 复用 Task */
        CANARY_ASSERT(c);
        SocketAsyncExtension* p = static_cast<SocketAsyncExtension*>(c[0]);
        SocketAsyncEventArgs* e = static_cast<SocketAsyncEventArgs*>(c[1]);
        SocketAsyncOperation op = (SocketAsyncOperation)(long)c[2];

		e->m_nbyteTransfered = p->WaitOne(*e);
		/*if (SOCKET_ERROR == e->m_nbyteTransfered)
		{
			throw std::exception("Async IO task canceled");
		}*/
        ERROR_ASSERT(0 == GetLastError() || -1 == e->m_nbyteTransfered, "Catched rare bug !");
		e->m_SocketError = 0;
		e->SetLastOperation(op);
        MemoryBarrier();
            
		e->Completed(e->GetUserToken(), *e);
	},
	CreateAsyncState(this, (Object)NULL/*SocketAsyncEventArgs*/, (Object)SocketAsyncOperation::None));

	SocketAsyncExtension(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType, HANDLE serverIOCP/* 是否将 Accept 和 IO 共用一个完成端口，默认禁用 */) : 
        Socket(addressFamily, socketType, protocolType), m_socketAsynclibraryInitlializer(NULL), m_bOwnerIOCP(false)
	{
#   ifdef SHARE_ASYNC_ACCEPT_IOCP_WITH_ASYNC_IO
		WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort((HANDLE)m_socket, serverIOCP, (u_long)0, 0)), "SocketAsyncExtension construction failed");
#   else
	    WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0)), "SocketAsyncExtension construction failed");
	    WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort((HANDLE)m_socket, m_hCompPort, (u_long)0, 0)), "SocketAsyncExtension construction failed");
        m_bOwnerIOCP = true;
#   endif
	}
public:
    IOCPExtensionInitlializer* m_socketAsynclibraryInitlializer;
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(SocketAsyncExtension);
    SocketAsyncExtension(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) :
        Socket(addressFamily, socketType, protocolType), m_socketAsynclibraryInitlializer(NULL), m_bOwnerIOCP(true)
    { 
        WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long) 0, 0)), "SocketAsyncExtension construction failed");
        WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort((HANDLE)m_socket, m_hCompPort, (u_long) 0, 0)), "SocketAsyncExtension construction failed");
        m_socketAsynclibraryInitlializer = new IOCPExtensionInitlializer(m_socket);
    }
    ~SocketAsyncExtension()
	{
		if (m_hCompPort && m_bOwnerIOCP)
		{
			CloseHandle(m_hCompPort);
			m_hCompPort = NULL;
		}
        SAVE_DELETE_PTR(m_socketAsynclibraryInitlializer);
		SAVE_DELETE_PTR(m_AcceptAsyncTask);
		SAVE_DELETE_PTR(m_IOAsyncTask);
    }
    bool AcceptAsync(SocketAsyncEventArgs &e)
	{
		ZeroMemory(&e.m_olOverlap, sizeof(OVERLAPPED));
        SocketAsyncExtension* p = new SocketAsyncExtension(m_addressFamily, m_sockType, m_protoType);//, m_hCompPort);
        DWORD dwbyteTransfered;
        if (AcceptEX(
            m_socket,
            p->m_socket,
            e.GetBuffer(),
            //e.GetCount(),
            0,/* AcceptAsync 禁用连接后立刻读数据*/
            IOCP_SOCKET_ADDR_BUF_SIZE, 
            IOCP_SOCKET_ADDR_BUF_SIZE, 
            &dwbyteTransfered,
            &e.m_olOverlap
         ))
        {
            e.SetAcceptSocket(p);
            return false;
        }
        SOCKETAPI_ASSERT(WSA_IO_PENDING == WSAGetLastError(), "https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex");
        /* 避开重复启动任务 Start(AsyncState) CreateAsyncState / DisposeAsyncState 开销 */
        AsyncState context = m_AcceptAsyncTask->GetAsyncState();
        context[1] = &e;
        context[2] = p;
        m_AcceptAsyncTask->Start();
        return true;
    }
    bool ReceiveAsync(SocketAsyncEventArgs& e)
	{
        DWORD byteTransfered;
        int nRet = WSARecv
		(
			m_socket, &e.m_buffer, 1, &byteTransfered,// NULL,
            (DWORD*)&e.m_SocketFlags, &e.m_olOverlap, NULL /* 这里不知道为什么只能为 NULL 太坑了*/
		);
		if (0 == nRet)
		{
            e.m_nbyteTransfered = WaitOne(e, 0);/* 将完成端口的包取走 */
            return false;
		}
		SOCKETAPI_ASSERT(WSA_IO_PENDING == WSAGetLastError(), "ReceiveAsync failed");
		AsyncState context = m_IOAsyncTask->GetAsyncState();
		context[1] = &e;
        context[2] = (Object)SocketAsyncOperation::Receive;
        m_IOAsyncTask->Start();
		return true;
    }
    bool SendAsync(SocketAsyncEventArgs& e)
	{
		DWORD byteTransfered;
		int nRet = WSASend
		(
			m_socket, &e.m_buffer, 1, &byteTransfered,// NULL,
			(DWORD)e.m_SocketFlags, &e.m_olOverlap, NULL
		);
		if (0 == nRet)
		{
			e.m_nbyteTransfered = WaitOne(e, 0);/* 将完成端口的包取走 */
			return false;
		}
		SOCKETAPI_ASSERT(WSA_IO_PENDING == WSAGetLastError(), "SendAsync failed");
		AsyncState context = m_IOAsyncTask->GetAsyncState();
		context[1] = &e;
		context[2] = (Object)SocketAsyncOperation::Send;
		m_IOAsyncTask->Start();
		return true;
    }
private:
    int WaitOne(SocketAsyncEventArgs &e, DWORD milliseconds =  INFINITE)
    {
        ULONG_PTR completionKey = 0;
        LPOVERLAPPED pOverlapped= 0;
        DWORD byteTransfered    = 0;
        if (GetQueuedCompletionStatus(m_hCompPort, &byteTransfered, &completionKey, &pOverlapped, milliseconds))
		{
            return byteTransfered;
        }
        //NULL == pOverlapped  WAIT_TIMEOUT
        int nErrorCode = GetLastError();
        /* 完成端口绑定的 socket 被释放 */
        WINAPI_ASSERT
        (
            ERROR_OPERATION_ABORTED == nErrorCode ||/* 完成端口拥有者已经释放资源 */
            ERROR_ABANDONED_WAIT_0  == nErrorCode ||/* 完成端口拥有者已经释放资源 */
            ERROR_NETNAME_DELETED   == nErrorCode   /* remote closed */,
            "Wait completion port failed"
        );
        //printf("Wait IOCP-%p failed , error code %d\n", m_hCompPort, nErrorCode);
        return SOCKET_ERROR;
    }
};
#endif