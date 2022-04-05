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
#define IOCP_SOCKET_ADDR_BUF_SIZE (sizeof(sockaddr_in6) + 16)
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
    WEAK_PTR(char)                  m_pBuffer;
    WEAK_PTR(SocketAsyncExtension)  m_Socket;

    long    m_nLastOperation; 
	DWORD   m_dwBytesTransferred;
	WSAOVERLAPPED m_olOverlap;
public:
    EventHandler<SocketAsyncEventArgs&> Completed;

    SocketAsyncEventArgs() : m_Socket(NULL), m_nLastOperation((long)SocketAsyncOperation::None), Completed(NULL), m_olOverlap({0}) {}
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
     * @brief 异步套接字方法的数据缓冲区, （TODO: 需要在合适的时候初始化）
     * 
     */
    DECLARE_GETTER(WEAK_PTR(char), Buffer, 
    {
        return m_pBuffer;
    })
    /**
     * @brief 获取在完成端口中传输的字节数
     * 
     */
    DECLARE_GETTER(DWORD, BytesTransferred,
    {
        return m_dwBytesTransferred;
    })
    /**
     * @brief 
     * 
     */
    DECLARE_DATAWRAPPER_INDEXER(int, Count)
    /**
     * @brief ConnectAsync 接口返回的 Socket 对象
     * 
     */
    DECLARE_GETTER(SocketAsyncExtension*, ConnectSocket,
    {
        return m_Socket;
    })
    DECLARE_INDEXER(SocketAsyncOperation, LastOperation,
    {
        return (SocketAsyncOperation)m_nLastOperation;
    },
    {
        long val = (long)SETTER_VALUE;
        System::Threading::Interlocked<long>::Exchange(m_nLastOperation, val);
    })
    DECLARE_DATAWRAPPER_INDEXER(int, Offset)
    /**
     * @brief 获取或设置异步套接字操作的结果
     * 
     */
    DECLARE_GETTER(int, SocketError, { return WSAGetLastError(); })
    DECLARE_SETTER_RIGHT_VAL_ENABLE(int, SocketError, { return WSASetLastError(SETTER_VALUE); })

    DECLARE_DATAWRAPPER_INDEXER(SocketFlags, SocketFlags)
    DECLARE_DATAWRAPPER_INDEXER_RIGHT_VAL_ENABLE(Object, UserToken)

    void SetBuffer(WEAK_PTR(char) buffer, int size)
    {
        m_pBuffer   = buffer;
        m_Count     = size;
        m_Offset    = 0;
    }
};  
/**
 * @brief 完成端口函数指针初始化
 * 
 */
class IOCPExtensionInitlializer
{
#define INIT_SOCKET_IOCP_FUNC(capitalFuncName) (GetSocketAsyncFunctionFromGuid<LPFN_##capitalFuncName>(GUID(WSAID_##capitalFuncName)))
private:
	SOCKET m_socket;
public:
    LPFN_ACCEPTEX               AcceptEX;      
    LPFN_CONNECTEX              ConnectEX;    
    LPFN_DISCONNECTEX           DisconnectEX; 
    LPFN_GETACCEPTEXSOCKADDRS   GetAcceptExSockAddrs;
    LPFN_TRANSMITFILE           TransmitFile;
    LPFN_TRANSMITPACKETS        TransmitPackets;
    LPFN_WSARECVMSG             WSARecvMsg;
    LPFN_WSASENDMSG             WSASendMsg;
    // LPFN_WSAPOLL                WSAPoll;
    IOCPExtensionInitlializer(SOCKET socket) : m_socket(socket)
    {  
        AcceptEX            = INIT_SOCKET_IOCP_FUNC(ACCEPTEX);
        ConnectEX           = INIT_SOCKET_IOCP_FUNC(CONNECTEX);
        DisconnectEX        = INIT_SOCKET_IOCP_FUNC(DISCONNECTEX);
        GetAcceptExSockAddrs= INIT_SOCKET_IOCP_FUNC(GETACCEPTEXSOCKADDRS);
        TransmitFile        = INIT_SOCKET_IOCP_FUNC(TRANSMITFILE);
        TransmitPackets     = INIT_SOCKET_IOCP_FUNC(TRANSMITPACKETS);
        WSARecvMsg          = INIT_SOCKET_IOCP_FUNC(WSARECVMSG);
        WSASendMsg          = INIT_SOCKET_IOCP_FUNC(WSASENDMSG);
        // WSAPoll             = INIT_SOCKET_IOCP_FUNC(WSAPOLL);
    }
private:
    template<typename TFuncPtr>
    TFuncPtr GetSocketAsyncFunctionFromGuid(const GUID& guid)
    {
        TFuncPtr lpfn = NULL;
        DWORD dwBytes;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != WSAIoctl
            (
                m_socket,
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
/**
 * @brief 基于完成端口的异步 Socket 扩展
 * 
 */
class System::Net::Sockets::SocketAsyncExtension final : public Socket
{
private:
    /**
     * @brief Socket IOCP 库资源初始化对象
     * 
     */
#   define AcceptEX             m_socketAsynclibraryInitlializer->AcceptEX
#   define ConnectEX            m_socketAsynclibraryInitlializer->ConnectEX
#   define DisconnectEX         m_socketAsynclibraryInitlializer->DisconnectEX
#   define GetAcceptExSockAddrs m_socketAsynclibraryInitlializer->GetAcceptExSockAddrs
#   define TransmitFile         m_socketAsynclibraryInitlializer->TransmitFile
#   define TransmitPackets      m_socketAsynclibraryInitlializer->TransmitPackets
#   define WSARecvMsg           m_socketAsynclibraryInitlializer->WSARecvMsg
#   define WSASendMsg           m_socketAsynclibraryInitlializer->WSASendMsg
#   define WSAPoll              m_socketAsynclibraryInitlializer->WSAPoll
private:
    HANDLE m_hCompPort;
    System::Threading::Tasks::Task* m_CompPortFinishedTask = new System::Threading::Tasks::Task([](AsyncState c)->void
    {
        /* 通过调用 m_CompPortFinishedTask.Start(args) 复用 Task */
        SocketAsyncExtension* p = static_cast<SocketAsyncExtension*>(c[0]);
        SocketAsyncEventArgs* e = static_cast<SocketAsyncEventArgs*>(c[1]);
        SOCKET                s = (SOCKET)c[2];
        if (!p->WaitOne(*e))
        {
            closesocket(s);
            throw std::exception("Accept task canceled");
        }

        SocketAsyncExtension* sae = new SocketAsyncExtension(p->GetAddressFamily(), p->GetSocketType(), p->GetProtocolType(), s);
        CANARY_ASSERT(NULL == e->GetAcceptSocket());
        e->SetAcceptSocket(sae);
        e->Completed(e->GetUserToken(), *e);
    });
public:
    IOCPExtensionInitlializer* m_socketAsynclibraryInitlializer;
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(SocketAsyncExtension);
    SocketAsyncExtension(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) : Socket(addressFamily, socketType, protocolType), m_socketAsynclibraryInitlializer(NULL)
    { 
        // Create a handle for the completion port
        WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long) 0, 0)), "SocketAsyncExtension construction failed");
        // Associate the listening socket with the completion port
        WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort((HANDLE)m_socket, m_hCompPort, (u_long) 0, 0)), "SocketAsyncExtension construction failed");
    }
    SocketAsyncExtension(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType, SOCKET s) : Socket(addressFamily, socketType, protocolType, s), m_socketAsynclibraryInitlializer(NULL)
    {
        // Create a handle for the completion port
        WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, (u_long)0, 0)), "SocketAsyncExtension construction failed");
        // Associate the listening socket with the completion port
        WINAPI_ASSERT(NULL != (m_hCompPort = CreateIoCompletionPort((HANDLE)s, m_hCompPort, (u_long)0, 0)), "SocketAsyncExtension construction failed");
    }
    ~SocketAsyncExtension()
    {
        SAVE_DELETE_PTR(m_socketAsynclibraryInitlializer);
        SAVE_DELETE_PTR(m_CompPortFinishedTask);
        if (m_hCompPort)
        {
            CloseHandle(m_hCompPort);
            m_hCompPort = NULL;
        }
    }
    bool AcceptAsync(SocketAsyncEventArgs &e)
    {
        SOCKET s = socket((int)m_addressFamily, (int)m_sockType, (int)m_protoType);
        memset(&e.m_olOverlap, 0, sizeof (e.m_olOverlap));
        if(AcceptEX(
            m_socket,
            s,
            e.GetBuffer(),
            e.GetCount(),
            IOCP_SOCKET_ADDR_BUF_SIZE, 
            IOCP_SOCKET_ADDR_BUF_SIZE, 
            &e.m_dwBytesTransferred, 
            &e.m_olOverlap
         ))
        {
            SocketAsyncExtension *p = new SocketAsyncExtension(m_addressFamily, m_sockType, m_protoType);
            e.SetAcceptSocket(p);
            return false;
        }
        SOCKETAPI_ASSERT(WSA_IO_PENDING == WSAGetLastError(), "https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex");
        m_CompPortFinishedTask->Start(CreateAsyncState(this, &e, (Object)s));
        //return !m_CompPortFinishedTask->Wait();
        return true;
    }
    bool ReceiveAsync(SocketAsyncEventArgs& e)
    {
        /* https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/ms741687(v=vs.85) */
        //WSARecvMsg(e.GetAcceptSocket()->GetHandle(),);
        return false;
    }
private:
    bool WaitOne(SocketAsyncEventArgs &e, DWORD milliseconds =  INFINITE)
    {
        ULONG_PTR completionKey         = 0;
        LPOVERLAPPED pOverlapped        = 0;

        if (GetQueuedCompletionStatus(m_hCompPort, &e.m_dwBytesTransferred, &completionKey, &pOverlapped, milliseconds))
        {
            e.SetSocketError(0);
            return true;
        }
        else if (ERROR_OPERATION_ABORTED == GetLastError())
        {
            return false;
        }
        /* 完成端口绑定的 socket 被释放 */
        WINAPI_ASSERT(ERROR_OPERATION_ABORTED == GetLastError(), "Wait completetion port failed");
        return false;
    }
};
#endif