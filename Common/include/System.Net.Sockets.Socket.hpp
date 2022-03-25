/*
 * @Description: Tcp,Udp传输层接口
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-06 19:31:39
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 16:59:19
 */
#ifndef SYSTEM_NET_SOCKETS_SOCKET_HPP
#define SYSTEM_NET_SOCKETS_SOCKET_HPP
#include <System.Net.IPAddress.hpp>
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
    using LingerOption   = linger;
    using ProtocolType  = IPPROTO;
    using SocketError   = int;
    enum IOControlCode;
    enum SelectMode;
    enum SocketFlags; 
    enum class SocketOptionLevel;
    enum SocketOptionName;
    enum SocketShutdown;
    enum SocketType;
    class Socket;
    // class SocketAsyncEventArgs;
};
enum System::Net::Sockets::IOControlCode /* https://docs.microsoft.com/zh-cn/dotnet/api/system.net.sockets.iocontrolcode?view=net-5.0 */
{
    /**
     * @brief 当等待接收数据时启用通知
     * 
     */
    AsyncIO         =  FIOASYNC,
    /**
     * @brief 控制套接字的阻塞( blocking mode )行为。 如果使用此控制代码指定的参数为 0，套接字置于阻塞模式下。 如果参数不为零，套接字置于非阻塞模式下
     * 
     */
    NonBlockingIO   = FIONBIO,
    /**
     * @brief 返回可读取的字节数
     * 
     */
    DataToRead      = FIONREAD,
};
/**
 * @brief 为 Poll(Int32, SelectMode) 方法定义轮询模式
 * 
 */
enum System::Net::Sockets::SelectMode
{
    /**
     * @brief 读状态模式
     * 
     */
    SelectRead = 0,
    /**
     * @brief 写状态模式
     * 
     */
    SelectWrite,
    /**
     * @brief 错误状态模式
     * 
     */
    SelectError
};
/**
 * @brief 指定套接字发送和接收行为。此枚举支持其成员值的按位组合
 * 
 */
enum System::Net::Sockets::SocketFlags
{
    /**
     * @brief 不对此调用使用任何标志
     * 
     */
    None                = 0,
    /**
     * @brief 处理带外数据
     * 
     */
    OutOfBand           = MSG_OOB,
    /**
     * @brief 快速查看传入消息
     * 
     */
    Peek                = MSG_PEEK,
    /**
     * @brief 不使用路由表进行发送
     * 
     */
    MsgDontRoute           = MSG_DONTROUTE,
    /**
     * @brief not support
     * 
     */
    WaitAll             = MSG_WAITALL,
    /**
     * @brief 消息太大，无法放入指定的缓冲区，并且已被截断
     * 
     */
    Truncated           = DECLARE_ENUM_FLAG(8),
    /**
     * @brief 指示控制数据无法放入 64 KB 的内部缓冲区且已被截断
     * 
     */
    ControlDataTruncated= DECLARE_ENUM_FLAG(9),
    /**
     * @brief 指示广播数据包
     * 
     */
    MsgBroadcast           = DECLARE_ENUM_FLAG(10),
    /**
     * @brief 指示多播数据包
     * 
     */
    Multicast           = DECLARE_ENUM_FLAG(11),
    /**
     * @brief 部分发送或接收消息
     * 
     */
    Partial             = DECLARE_ENUM_FLAG(15)
};
/**
 * @brief 为 SetSocketOption / GetSocketOption 方法定义套接字选项级别
 * 
 */
enum class System::Net::Sockets::SocketOptionLevel
{
    /**
     * @brief https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-ip-socket-options
     * 
     */
    IP      = IPPROTO_IP,
    /**
     * @brief https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-tcp-socket-options
     * 
     */
    Tcp     = IPPROTO_TCP,
    /**
     * @brief https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-udp-socket-options
     * 
     */
    Udp     = IPPROTO_UDP,
    /**
     * @brief https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-ipv6-socket-options
     * 
     */
    IPv6    = IPPROTO_IPV6,
    /**
     * @brief https://docs.microsoft.com/en-us/windows/win32/winsock/sol-socket-socket-options
     * 
     */
    Socket  = SOL_SOCKET
};
/**
 * @brief 定义配置选项名称
 * 
 */
enum System::Net::Sockets::SocketOptionName
{
    /**
     * @brief 使套接字能够为独占访问进行绑定 
     * https://docs.microsoft.com/en-us/windows/win32/winsock/so-exclusiveaddruse
     * A socket with SO_EXCLUSIVEADDRUSE set cannot always be reused immediately after socket closure. 
     * For example, if a listening socket with the exclusive flag set accepts a connection after which the listening socket is closed, 
     * another socket cannot bind to the same port as the first listening socket with the exclusive flag 
     * until the accepted connection is no longer active.
     */
    ExclusiveAddressUse = SO_EXCLUSIVEADDRUSE,
    /**
     * @brief 记录调试信息
     * 
     */
    Debug               = SO_DEBUG,
    /**
     * @brief 套接字正在侦听
     * 
     */
    AcceptConnection    = SO_ACCEPTCONN,
    /**
     * @brief 允许将套接字绑定到已在使用中的地址
     * 
     */
    ReuseAddress        = SO_REUSEADDR,
    /**
     * @brief 使用 keep-alive
     * keepalive原理:TCP内嵌有心跳包,以服务端为例,当server检测到超过一定时间(/proc/sys/net/ipv4/tcp_keepalive_time 7200 即2小时)没有数据传输,那么会向client端发送一个keepalive packet,此时client端有三种反应:
     * 1、client端连接正常,返回一个ACK.server端收到ACK后重置计时器,在2小时后在发送探测.如果2小时内连接上有数据传输,那么在该时间的基础上向后推延2小时发送探测包;
     * 2、客户端异常关闭,或网络断开。client无响应,server收不到ACK,在一定时间(/proc/sys/net/ipv4/tcp_keepalive_intvl 75 即75秒)后重发keepalive packet, 并且重发一定次数(/proc/sys/net/ipv4/tcp_keepalive_probes 9 即9次);
     * 3、客户端曾经崩溃,但已经重启.server收到的探测响应是一个复位,server端终止连接。
     */
    KeepAlive           = SO_KEEPALIVE,
    /**
     * @brief 不路由，将数据包直接发送到接口地址
     * 
     */
    DontRoute           = SO_DONTROUTE,
    /**
     * @brief 允许在套接字上发送广播消息
     * 
     */
    Broadcast           = SO_BROADCAST,
    /**
     * @brief 可能时避开硬件
     * 
     */
    UseLoopback         = SO_USELOOPBACK,
    /**
     * @brief 如果存在未发送的数据，则在关闭时逗留
     * 
     */
    Linger              = SO_LINGER,
    /**
     * @brief 接收正常数据流中的带外数据
     * 
     */
    OutOfBandInline     = SO_OOBINLINE,
    /**
     * @brief 指定为发送保留的每个套接字缓冲区空间的总量。 这与最大消息大小或 TCP 窗口的大小无关
     * 
     */
    SendBuffer          = SO_SNDBUF,
    /**
     * @brief 指定为接收保留的每个套接字缓冲区空间的总量。 这与最大消息大小或 TCP 窗口的大小无关
     * 
     */
    ReceiveBuffer       = SO_RCVBUF,
    /**
     * @brief 内核执行 Send 操作 Buffer 长度的阈值
     * 
     */
    SendLowWater        = SO_SNDLOWAT,   
    /**
     * @brief 内核执行 Recv 操作 Buffer 长度的阈值
     * 
     */
    ReceiveLowWater     = SO_RCVLOWAT,
    /**
     * @brief 发送超时。此选项仅适用于同步方法;它对异步方法不起作用
     * 
     */
    SendTimeout         = SO_SNDTIMEO,
    /**
     * @brief 发送超时。此选项仅适用于同步方法;它对异步方法不起作用
     * 
     */
    ReceiveTimeout      = SO_RCVTIMEO,
    /**
     * @brief 获取错误状态并清除
     * 
     */
    Error               = SO_ERROR,
    /**
     * @brief 获取套接字类型
     * 
     */
    Type                = SO_TYPE,

    /************************************* 
     * IP Setting
     * https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-ip-socket-options
     **************************************/
   
    /**
     * @brief 获取或设置数据包的生存时间(Time To Live)
     * 
     */
    TTL                 = IP_TTL,
    /**
     * @brief IP 多路广播环回。
     * 在windows平台, IP_MULTICAST_LOOP 应用到接收端.在接收端启用IP_MULTICAST_LOOP. loop设置为1，表示接收自身发送出去的数据，设置为0表示不接收
     * 在Linux平台, IP_MULTICAST_LOOP 应用到发送端
     * 
     * Note  The Winsock version of the IP_MULTICAST_LOOP option is semantically different than the UNIX version of the IP_MULTICAST_LOOP option:
     *
     *  In Winsock, the IP_MULTICAST_LOOP option applies only to the receive path.
     *  In the UNIX version, the IP_MULTICAST_LOOP option applies to the send path.
     * 
     * For example, applications ON and OFF (which are easier to track than X and Y) join the same group on the same interface; 
     * application ON sets the IP_MULTICAST_LOOP option on, 
     * application OFF sets the IP_MULTICAST_LOOP option off. 
     * If ON and OFF are Winsock applications, OFF can send to ON, but ON cannot sent to OFF. 
     * In contrast, if ON and OFF are UNIX applications, ON can send to OFF, but OFF cannot send to ON.
     * 
     */
    MulticastLoopback   = IP_MULTICAST_LOOP,
    /**
     * @brief 加入组播
     * 
     */
    AddMembership       = IP_ADD_MEMBERSHIP,
    /**
     * @brief 退出组播
     * 
     */
    DropMembership      = IP_DROP_MEMBERSHIP,
    /**
     * @brief 设置是否分包
     * 
     */
    DontFragment        = IP_DONTFRAGMENT,
    /************************************* 
     * IPV4 Setting
     * https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-ipv4-socket-options
     **************************************/
    
    /**
     * @brief To be continune...
     * 
     */

    /************************************* 
     * IPV6 Setting
     * https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-ipv6-socket-options
     **************************************/
    
    /**
     * @brief 该值指定是否 Socket 是用于 IPv4 和 IPv6 的双模式套接字， 0 启用双模式套接字( 默认启用 )
     * 
     */
    Ipv6Only            =  IPV6_V6ONLY
};
/**
 * @brief 定义 Shutdown 方法使用的常量
 * 
 */
enum System::Net::Sockets::SocketShutdown
{
    Receive = 0,
    Send,
    Both
};
/**
 * @brief 指定 Socket 类的实例表示的套接字类型
 * 
 */
enum System::Net::Sockets::SocketType
{
    Unknown = -1,
    /**
     * @brief 支持可靠、双向、基于连接的字节流，而不重复数据，也不保留边界。 
     * 此类型的 Socket 与单个对方主机通信，并且在通信开始之前需要建立远程主机连接。 
     * Stream 使用传输控制协议 (ProtocolType.Tcp) 和 AddressFamily.InterNetwork 地址族
     * 
     */
    Stream  = SOCK_STREAM,
    /**
     * @brief 支持数据报，即最大长度固定（通常很小）的无连接、不可靠消息。 
     * 消息可能会丢失或重复并可能在到达时不按顺序排列。 
     * Socket 类型的 Dgram 在发送和接收数据之前不需要任何连接，并且可以与多个对方主机进行通信。 
     * Dgram 使用数据报协议 (ProtocolType.Udp) 和 AddressFamily.InterNetwork 地址族
     * 
     */
    Dgram   = SOCK_DGRAM,
    Raw,
    Rdm,
    Seqpacket
};
class System::Net::Sockets::Socket
{
#if BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS
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
    Socket(){}
public:
    Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) : 
        m_addressFamily(addressFamily), m_sockType(socketType), m_protoType(protocolType), 
        m_bIsBlocking(true) /* 默认为阻塞模式 */ 
    {
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET != (m_socket = socket((int)addressFamily, socketType, protocolType)), 
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
            return SOCKET_ERROR != send(m_socket, "", 0, SocketFlags::None) || WSAEWOULDBLOCK == WSAGetLastError();
        }
        DWORD nBlockMode = 1;
        ERROR_ASSERT(0 == IOControl(IOControlCode::NonBlockingIO, nBlockMode), "GetConnected set nonblockmode failed");
        bool bIsConnect = SOCKET_ERROR != send(m_socket, "", 0, SocketFlags::None) || WSAEWOULDBLOCK == WSAGetLastError();
        nBlockMode = 0;
        ERROR_ASSERT(0 == IOControl(IOControlCode::NonBlockingIO, nBlockMode), "GetConnected recover blockmode failed");
        return bIsConnect;
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
        GetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, bEnable);
        return bEnable;
    },
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, SETTER_VALUE);
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
        fd_set* pfds[SelectMode::SelectError + 1] = {NULL};
        pfds[mode] = &fds;
        return Select(pfds[SelectMode::SelectRead], pfds[SelectMode::SelectWrite], pfds[SelectMode::SelectError], microSeconds);
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
        if(SOCKET_ERROR == (nRet = recv(m_socket, buffer, size, socketFlags)))
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
            SOCKET_ERROR != (nRet = recvfrom(m_socket, buffer, size, socketFlags, (SOCKADDR *)remoteEP.Item, &remoteEP.Size)), 
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
        if(SOCKET_ERROR == (nRet = send(m_socket, buffer, size, socketFlags)))
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
        SOCKETAPI_ASSERT(SOCKET_ERROR != (nRet = sendto(m_socket, buffer, size, socketFlags, (SOCKADDR *)remoteEP.Item, remoteEP.Size)), "SendTo failed");
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
            SOCKET_ERROR != getsockopt(m_socket, (int)optionLevel, optionName, buffer, &optlen), 
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
            SOCKET_ERROR != setsockopt(m_socket, (int)optionLevel, optionName, optionValue, optlen), 
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
        SOCKETAPI_ASSERT(SOCKET_ERROR != shutdown(m_socket, how), "SocketShutdown failed");
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