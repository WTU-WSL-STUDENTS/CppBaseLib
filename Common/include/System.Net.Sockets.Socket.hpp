/*
 * @Description: Tcp,Udp传输层接口
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-06 19:31:39
 * @LastEditors: like
 * @LastEditTime: 2022-03-31 09:54:28
 */
#ifndef SYSTEM_NET_SOCKETS_SOCKET_HPP
#define SYSTEM_NET_SOCKETS_SOCKET_HPP
#include <System.Net.Sockets.SocketDef.h>
#include <CompliedIndexer.h>
/**
 * @brief 默认情况下 Socket 库的生命周期与进程绑定。
 * 如果要禁用该功能, 采用手动初始化 & 释放指定版本的 Socket 库, 请在此之前定义 #define BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS 0
 * 
 */
#ifndef BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS
#   define BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS 1
#endif

namespace System::Net::Sockets
{
    class Socket;
    class SocketAsyncExtension;
};
class System::Net::Sockets::Socket
{
friend class SocketAsyncExtension;
#if BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS
private:
    class SocketlibraryInitlializer
    {
    public:
        SocketlibraryInitlializer()
        {
            WSADATA wsaData;
            ERROR_ASSERT(NO_ERROR == WSAStartup(MAKEWORD(2, 2), &wsaData), "SocketlibraryInitlializer init Socket 2.2 failed");
        }
        ~SocketlibraryInitlializer()
        {
            WSACleanup();
        }
    };
    /**
     * @brief Socket 库资源初始化对象, 生命周期与进程绑定。
     * 
     */
    static readonly SocketlibraryInitlializer m_socketlibraryInitlializer;
#endif
protected:
    AddressFamily   m_addressFamily;
    SocketType      m_sockType;
    ProtocolType    m_protoType;
    
    bool            m_bIsBlocking;

    SOCKET          m_socket;
    Socket() : m_addressFamily(AddressFamily::InterNetwork), m_sockType(SocketType::Stream), m_protoType(ProtocolType::IPPROTO_IPV4), m_bIsBlocking(true), m_socket(INVALID_SOCKET){}
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Socket);
    Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) : 
        m_addressFamily(addressFamily), m_sockType(socketType), m_protoType(protocolType), 
        m_bIsBlocking(true) /* 默认为阻塞模式 */ 
    {
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET != (m_socket = socket((int)addressFamily, (int)socketType, (int)protocolType)),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket"
        );
    }
    /**
     * @brief m_socket 调用 Close() 进行资源释放, 通过 Accept 创建的 Socket 对象不需要调用 Close()
     * 
     */
    virtual ~Socket(){}
    /**
     * @brief 获取 Socket 的操作系统句柄
     * 
     */
    DECLARE_CONST_GETTER(SOCKET, Handle, 
    {
        return m_socket;
    })
    /**
     * @brief 获取 Socket 的地址族
     * 
     */
    DECLARE_CONST_GETTER(AddressFamily, AddressFamily,
    {
        return m_addressFamily;
    })
    /**
     * @brief 获取 Socket 的类型
     * 
     */
    DECLARE_CONST_GETTER(SocketType, SocketType,
    {
        return m_sockType;
    })
    /**
     * @brief 获取已经从网络接收且可供读取的数据量
     * 
     */
    DECLARE_CONST_GETTER(DWORD, Available,
    {
        DWORD nLen;
        IOControl(IOControlCode::DataToRead, nLen);
        return nLen;
    })
    /**
     * @brief Socket 是否处于阻塞模式
     * 
     * @return true 
     * @return false 
     */
    DECLARE_INDEXER(bool, Blocking, 
    {
        return m_bIsBlocking;
    },
    {
        VOIDRET_ASSERT(SETTER_VALUE != m_bIsBlocking);
        DWORD nArg = m_bIsBlocking;
        if(0 == IOControl(IOControlCode::NonBlockingIO, nArg))
        {
            m_bIsBlocking = SETTER_VALUE;
            MemoryBarrier();
        }
    })
    /**
     * @brief 查询当前 socket 是否处于 TCP 连接状态
     * 判定当前仍然处于连接状态的方法 :
     *  以非阻塞模式发送 0 字节数据, 发送成功或发送失败的 ErrorCode 为 WAEWOULDBLOCK 
     * 
     */
    DECLARE_CONST_GETTER(bool, Connected,
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        if(!m_bIsBlocking)
        {                
            return SOCKET_ERROR != send(m_socket, "", 0, (int)SocketFlags::None) || WSAEWOULDBLOCK == WSAGetLastError();
        }
        DWORD nBlockMode = 1;
        ERROR_ASSERT(0 == IOControl(IOControlCode::NonBlockingIO, nBlockMode), "GetConnected set nonblockmode failed");
        bool bIsConnect = SOCKET_ERROR != send(m_socket, "", 0, (int)SocketFlags::None) || WSAEWOULDBLOCK == WSAGetLastError();
        nBlockMode = 0;
        ERROR_ASSERT(0 == IOControl(IOControlCode::NonBlockingIO, nBlockMode), "GetConnected recover blockmode failed");
        return bIsConnect;
    })
    /**
     * @brief 该值指定套接字能否在断开操作之后重用
     * 
     */
    DECLARE_INDEXER(bool, DisconnectReuseSocket, 
    {
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReuseAddress, bEnable);
        return bEnable;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReuseAddress, SETTER_VALUE);
    })
    /**
     * @brief UDP 是否禁用分包
     * 
     */
    DECLARE_INDEXER(bool, DontFragment, 
    {
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IP, SocketOptionName::DontFragment, bEnable);
        return bEnable; 
    },
    {
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::DontFragment, SETTER_VALUE);
    })
    /**
     * @brief Socket 是否用于 IPv4 和 IPv6 的双模式套接字
     * 
     */
    DECLARE_INDEXER(bool, DualMode,
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addressFamily, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, bEnable);
        return bEnable;
    },
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addressFamily, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, SETTER_VALUE);    
    })
    /**
     * @brief Socket 是否可以发送或接收广播数据包
     * 
     */
    DECLARE_INDEXER(bool, EnableBroadcast, 
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Broadcast, bEnable);
        return bEnable;
    },
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Broadcast, SETTER_VALUE);
    })
    /**
     * @brief Tcp Socket 是否仅允许一个进程绑定到端口
     * 
     */
    DECLARE_INDEXER(bool, ExclusiveAddressUse,
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, bEnable);
        return bEnable;
    },
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, SETTER_VALUE);
    })
    /**
     * @brief Socket 在尝试发送所有挂起数据时是否延迟关闭套接字
     * 
     */
    DECLARE_INDEXER(LingerOption, LingerState,
    {
        LingerOption state;
        int size = sizeof(LingerOption);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&state, size);
        return state;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&SETTER_VALUE, sizeof(LingerOption));
    })
    /**
     * @brief 获取 Tcp 连接对方的 SocketAddress
     * 在第一个 IO 操作后调用
     *  stream   : Connect
     *  datagram : sendto / receivefrom
     */
    DECLARE_CONST_GETTER(SocketAddress, LocalEndPoint,
    {
        SocketAddress addr;
        SOCKETAPI_ASSERT(0 == getpeername(m_socket, (struct sockaddr *)addr.Item, (socklen_t*)&addr.Size), "Get local end point failed");
        return addr;
    })
    /**
     * @brief 传出的多路广播数据包是否传递到发送应用程序
     * 
     */
    DECLARE_INDEXER(bool, MulticastLoopback,
    {
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IP, SocketOptionName::MulticastLoopback, bEnable);
        return bEnable; 
    },
    {
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::MulticastLoopback, SETTER_VALUE);
    })
    /**
     * @brief 指定 Tcp Socket 是否正在使用 Nagle 算法（默认开启）。
     * Small Packet Problem
     * 在使用一些协议通讯的时候，比如Telnet，会有一个字节字节的发送的情景，
     * 每次发送一个字节的有用数据，就会产生41个字节长的分组，
     * 20个字节的IP Header 和 20个字节的TCP Header，
     * 这就导致了1个字节的有用信息要浪费掉40个字节的头部信息，
     * 这是一笔巨大的字节开销，而且这种Small packet在广域网上会增加拥塞的出现。
     * 如何解决这种问题？ 
     * Nagle就提出了一种通过减少需要通过网络发送包的数量来提高TCP/IP传输的效率，这就是Nagle算法
     * 
     */
    DECLARE_INDEXER(bool, NoDelay,
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, bEnable);
        return bEnable;
    },
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, SETTER_VALUE);
    })
    /**
     * @brief Socket 接收缓冲区的大小。默认值为 8192
     * 
     */
    DECLARE_INDEXER(int, ReceiveBufferSize,
    {
        int nSize = 8192;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, nSize);
        return nSize;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, SETTER_VALUE);
    })
    /**
     * @brief 超时值（以毫秒为单位）。 默认值为 0，指示超时期限无限大。 指定 -1 还会指示超时期限无限大
     * 
     */
    DECLARE_INDEXER(int, ReceiveTimeout,
    {
        int nTimeout = -1;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, nTimeout);
        return nTimeout;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, SETTER_VALUE);
    })
    /**
     * @brief 获取或设置一个值，它指定 Socket 发送缓冲区的大小。默认值为 8192
     * 
     */
    DECLARE_INDEXER(int, SendBufferSize,
    {
        int nSize = 8192;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, nSize);
        return nSize;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, SETTER_VALUE);
    })
    /**
     * @brief 超时值（以毫秒为单位）。 默认值为 0，指示超时期限无限大。 指定 -1 还会指示超时期限无限大
     * 
     */
    DECLARE_INDEXER(int, SendTimeout,
    {
        int nTimeout = -1;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, nTimeout);
        return nTimeout;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, SETTER_VALUE);
    })
    /**
     * @brief 指定 Socket 发送的 Internet 协议 (IP) 数据包的生存时间 (TTL) 值
     * 
     */
    DECLARE_INDEXER(byte, Ttl,
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        byte val;
        int size = sizeof(byte);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::TTL, (char*)&val, size);
        return val;
    },
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::TTL, (char*)&SETTER_VALUE, sizeof(byte));
    })
    /**
     * @brief 为新建连接创建新的 Socket
     * 从侦听套接字的连接请求队列中同步提取第一个挂起的连接请求，然后创建并返回新的 Socket 。 
     * 不能使用返回的此 Socket 来接受来自连接队列的其他任何连接。 
     * 但是，可以调用方法标识远程主机 Socket 的网络地址和端口号
     * 
     * @param remoteEP 返回客户端的网络地址信息
     * @return Socket* 返回客户端的资源句柄
     */
    Socket* Accept(SocketAddress &remoteEP)
    {
        Socket* s = new Socket();
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET  != (s->m_socket = accept(m_socket, (SOCKADDR *)remoteEP.Item, &remoteEP.Size)),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept"
        );
        return s;
    }
    // bool AcceptAsync(SocketAsyncEventArgs  &e)
    // {
    //     Socket* s = new Socket();
    //     // e.SetAccecptSocket(s);
    //     SocketAddress addr;
    //     epoll
    //     SOCKETAPI_ASSERT
    //     (
    //         INVALID_SOCKET  != (s->m_socket = accept(m_socket, (SOCKADDR *)addr.Item, &addr.Size)),/* WSAEWOULDBLOCK */
    //         "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept"
    //     );
    //     // s->m_socket = WSAAccept(m_socket, (SOCKADDR *)addr.Item, &addr.Size,
    //     // [](
    //     //     LPWSABUF lpCallerId,
    //     //     LPWSABUF lpCallerData,
    //     //     LPQOS pQos,
    //     //     LPQOS lpGQOS,
    //     //     LPWSABUF lpCalleeId,
    //     //     LPWSABUF lpCalleeData,
    //     //     GROUP FAR * g,
    //     //     DWORD_PTR dwCallbackData
    //     // )->int
    //     // {
    //     //     SocketAsyncEventArgs* args = (SocketAsyncEventArgs*)dwCallbackData;
    //     //     return 0;
    //     // },(DWORD_PTR) &e);
    //     // if(INVALID_SOCKET != s->m_socket)
    //     // {
    //     //     return false;
    //     // }
    //     // printf("%d'n", WSAGetLastError());
    //     return true;
    //     /* "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-wsaaccept" */
    // }
    /**
     * @brief 使 Socket 与一个本地端口相关联
     * 
     * @param localEP 
     */
    void Bind(SocketAddress &localEP)
    { 
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != bind(m_socket, (SOCKADDR *)localEP.Item, localEP.Size),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-bind"
        );
    }
    /**
     * @brief 关闭 Socket 连接并释放所有关联的资源
     * 对于面向连接的协议，建议在 Shutdown 调用方法之前调用 Close 。 这可确保所有数据在连接的套接字关闭之前都已发送和接收。
     * 
     * 如果需要 Close 先调用而不调用 Shutdown ，
     * 可以通过将选项设置 DontLinger Socket 为 false 并指定非零超时间隔来确保将排队等待传出传输的数据发送。 
     * Close 会被阻塞，直到发送此数据或指定的超时过期
     * 
     */
    void Close()
    {
        VOIDRET_ASSERT(INVALID_SOCKET != m_socket);
        SOCKETAPI_ASSERT(SOCKET_ERROR != closesocket(m_socket), "Dispose Socket failed, create socket failed");
        m_socket = INVALID_SOCKET;
    }
    /**
     * @brief 与远程主机建立连接
     * 
     * @param reuseSocket 
     */
    void Connect(SocketAddress &remoteEP)
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != connect(m_socket, (SOCKADDR *)remoteEP.Item, remoteEP.Size),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect"
        );
    }
    /**
     * @brief 将 Socket 置于侦听状态
     * 
     * @param backlog 客户端最大连接量, 如果一个已经处于监听状态的 Socket 调用该接口, backlog 不会修改。
     */
    void Listen(int backlog = SOMAXCONN) 
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != listen(m_socket, backlog), 
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen"
        );
    }
    /**
     * @brief 确定 Socket 的状态
     * 
     * @param microSeconds 等待响应的时间（以微秒为单位）。 
     * @param mode 
     * SelectRead
     *  如果已调用 Listen(Int32) 并且有挂起的连接
     *  如果有数据可供读取
     *  如果连接已关闭、重置或终止
     * 
     * SelectWrite
     *  如果正在处理 Connect(EndPoint) 并且连接已成功
     *  如果可以发送数据
     * 
     * SelectError
     *  如果正在处理异步 Connect(EndPoint)，并且连接已失败
     *  如果 OutOfBandInline 未设置，并且带外数据可用
     * 
     * @return true 在指定 mode 条件下出现上述情况, 返回 true
     * @return false 
     */
    bool Poll(int microSeconds, SelectMode mode) 
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(m_socket, &fds);
        fd_set* pfds[(int)SelectMode::SelectError + 1] = {NULL};
        pfds[(int)mode] = &fds;
        return Select(pfds[(int)SelectMode::SelectRead], pfds[(int)SelectMode::SelectWrite], pfds[(int)SelectMode::SelectError], microSeconds);
    }
    /**
     * @brief 接收来自绑定的 Socket 的数据
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags 
     * @param errorcode 
     * @return int 实际接收的长度, 不会超过 ReceiveBuffer 的容量。 If the connection has been gracefully closed, the return value is zero.
     */
    int Receive(char* buffer, int size, SocketFlags socketFlags, SocketError &errorcode) 
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-recv */
        if(SOCKET_ERROR == (nRet = recv(m_socket, buffer, size, (int)socketFlags)))
        {
            errorcode = WSAGetLastError();
        }
        return nRet;   
    }
    /**
     * @brief 接收数据报并存储源终结点
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags OutOfBand / Peek 
     * @param remoteEP 
     * @return int 实际接收的长度, 不会超过 ReceiveBuffer 的容量。 If the connection has been gracefully closed, the return value is zero.
     */
    int ReceiveFrom(char* buffer, int size, SocketFlags socketFlags, SocketAddress &remoteEP) 
    {
        int nRet;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != (nRet = recvfrom(m_socket, buffer, size, (int)socketFlags, (SOCKADDR *)remoteEP.Item, &remoteEP.Size)),
            "ReceiveMessageFrom failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recvfrom"
        );
        return nRet;
    }
    /**
     * @brief 将数据发送到连接的 Socket
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags OutOfBand / DontRoute 
     * @param errorcode 
     * @return int 实际发送的长度, 如果数据长度超过了 SendBuffer 的剩余容量, 会进行截断 
     */
    int Send( char* buffer, int size, SocketFlags socketFlags, SocketError &errorcode) 
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send */
        if(SOCKET_ERROR == (nRet = send(m_socket, buffer, size, (int)socketFlags)))
        {
            errorcode = WSAGetLastError();
        }
        return nRet;
    }
    /**
     * @brief 使用指定的 SocketFlags，将指定字节数的数据发送到指定的终结点
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags OutOfBand / DontRoute 
     * @param remoteEP 
     * @return int 实际发送的长度, 如果数据长度超过了 SendBuffer 的剩余容量, 会进行截断 
     */
    int SendTo(const char* buffer, int size, SocketFlags socketFlags, const SocketAddress &remoteEP) 
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-sendto */
        SOCKETAPI_ASSERT(SOCKET_ERROR != (nRet = sendto(m_socket, buffer, size, (int)socketFlags, (SOCKADDR *)remoteEP.Item, remoteEP.Size)), "SendTo failed");
        return nRet;
    }
    /**
     * @brief 为 Socket 设置低级别操作模式
     * 
     * @param ioControlCode 
     * @param optionInOutValue 输入 / 输出参数
     * @return int 返回 0 代表成功
     */
    inline int IOControl(IOControlCode ioControlCode, DWORD &optionInOutValue) const
    {
        int nRet;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != (nRet = ioctlsocket(m_socket, (long)ioControlCode, &optionInOutValue)),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-ioctlsocket"
        );
        return nRet; 
    }
    /**
     * @brief 返回 Socket 选项的值
     * 
     * @param optionLevel 
     * @param optionName 
     * @param buffer 
     * @param optlen 
     */
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, char* buffer, int &optlen) const
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != getsockopt(m_socket, (int)optionLevel, (int)optionName, buffer, &optlen),
            "SetSocketOption failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-getsockopt"
        );
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, bool &optionValue) const
    {
        int optlen = sizeof(bool);
        GetSocketOption(optionLevel, optionName, (char *)&optionValue, optlen);
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, int &optionValue) const
    {
        int optlen = sizeof(int);
        GetSocketOption(optionLevel, optionName, (char *)&optionValue, optlen);
        ERROR_ASSERT(sizeof(int) == optlen, "GetSocketOption failed"); 
    }
    /**
     * @brief 将指定的 Socket 选项设置为指定值
     * 
     * @param optionLevel 
     * @param optionName 
     * @param optionValue 
     * @param optlen 
     */
    inline void SetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, const char* optionValue, int optlen)
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != setsockopt(m_socket, (int)optionLevel, (int)optionName, optionValue, optlen),
            "SetSocketOption failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt"
        );
    }
    inline void SetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, bool optionValue)
    {
        SetSocketOption(optionLevel, optionName, (char *)&optionValue, sizeof(bool));
    }
    inline void SetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, int optionValue)
    {
        SetSocketOption(optionLevel, optionName, (char *)&optionValue, sizeof(int));
    }
    /**
     * @brief 禁用 Socket 上的发送和接收， 如果存在未收发的数据, 会立刻放弃处理。 如果需要处理，请通过设置 Linger 来处理关闭动作
     * 
     * @param how 
     */
    inline void Shutdown(SocketShutdown how)
    {
        SOCKETAPI_ASSERT(SOCKET_ERROR != shutdown(m_socket, (int)how), "SocketShutdown failed");
    }
    /**
     * @brief 确定一个或多个套接字的状态, readfds / writefds / errorexceptfds 套接字集合的指针只能有一个非空
     * 
     * @param readfds 
     * If listen has been called and a connection is pending, accept will succeed.
     * Data is available for reading (includes OOB data if SO_OOBINLINE is enabled).
     * Connection has been closed/reset/terminated.
     * 
     * @param writefds 
     * If processing a connect call (nonblocking), connection has succeeded.
     * Data can be sent.
     * 
     * @param errorexceptfds 
     * If processing a connect call (nonblocking), connection attempt failed.
     * OOB data is available for reading (only if SO_OOBINLINE is disabled).
     * 
     * @param microSeconds 等待响应的时间（以微秒为单位）。 -1 永久阻塞
     * @return int Socket 响应个数
     */
    static int Select(fd_set* readfds, fd_set* writefds, fd_set* errorexceptfds, int microSeconds)
    {
        timeval timeout = {0, microSeconds};
        int nReadyCount;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != (nReadyCount = select(-1, readfds, writefds, errorexceptfds, -1 == microSeconds ? NULL : &timeout)), 
            "Select failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select"
        );
        return nReadyCount;
    }
    static inline bool IsSelectted(const Socket& socket, fd_set* fds)
    {
        return __WSAFDIsSet(socket.m_socket, fds);
    }
    static void InitFds(const Socket* socketList, UInt32 socketListLen, fd_set& rwefds)
    {
        ERROR_ASSERT((FD_SETSIZE + 1) > socketListLen, "Socket count out of range , please use #define FD_SETSIZE set a lager size");
        rwefds.fd_count = socketListLen;
        for(UInt32 i = 0; i < socketListLen; i++)
        {
            rwefds.fd_array[i] = socketList[i].m_socket;
        } 
    }
};
#if BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS
    readonly System::Net::Sockets::Socket::SocketlibraryInitlializer System::Net::Sockets::Socket::m_socketlibraryInitlializer;
#endif
#endif