/*
 * @Description: Tcp,Udp传输层接口
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-06 19:31:39
 * @LastEditors: like
 * @LastEditTime: 2022-03-18 18:25:43
 */
#ifndef SYSTEM_NET_SOCKETS_SOCKET_HPP
#define SYSTEM_NET_SOCKETS_SOCKET_HPP
#include <System.Net.IPAddress.hpp>
// #include <MSWSock.h>

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
    IP      = IPPROTO_IP,
    Tcp     = IPPROTO_TCP,
    Udp     = IPPROTO_UDP,
    IPv6    = IPPROTO_IPV6,
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

private:
    SocketType      m_sockType;
    ProtocolType    m_protoType;
    
    bool            m_bIsBlocking;
    bool            m_bUdpFragmentEnable;

    SOCKET          m_socket;
    SocketAddress   m_addr;
    Socket(){}
public:
    Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) : 
        m_sockType(socketType), m_protoType(protocolType), 
        m_bIsBlocking(true),        /* 默认为阻塞模式 */ 
        m_bUdpFragmentEnable(false) /* Udp 默认禁用分包 */
    {
        m_addr.ipv4Addr.sin_family = (UInt16)addressFamily;
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET != (m_socket = socket((int)addressFamily, socketType, protocolType)), 
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket"
        );
    }
    ~Socket()
    {
        SOCKETAPI_ASSERT(SOCKET_ERROR != closesocket(m_socket), "Dispose Socket failed, create socket failed");
    }
    /**
     * @brief 获取 Socket 的操作系统句柄
     * 
     * @return SOCKET 
     */
    SOCKET Handle()
    {
        return m_socket;
    }
    /**
     * @brief 获取 Socket 的地址族
     * 
     * @return AddressFamily 
     */
    inline AddressFamily AddressFamily()
    {
        return m_addr.GetFamily();
    }
    /**
     * @brief 获取已经从网络接收且可供读取的数据量
     * 
     * @return DWORD 
     */
    DWORD Available()
    {
        DWORD nLen;
        IOControl(IOControlCode::DataToRead, nLen);
        return nLen;
    }
    /**
     * @brief Socket 是否处于阻塞模式
     * 
     * @return true 
     * @return false 
     */
    bool GetBlockingEnable()
    {
        return m_bIsBlocking;
    }
    void SetBlockingEnable(bool blocking)
    {
        VOIDRET_ASSERT(blocking != m_bIsBlocking);
        DWORD nArg = !blocking;
        if(0 == IOControl(IOControlCode::NonBlockingIO, nArg))
        {
            m_bIsBlocking = blocking;
        }
    }
    /**
     * @brief 查询当前 socket 是否处于 TCP 连接状态
     * 
     * @return true 以非阻塞模式发送 0 字节数据, 发送成功或发送失败的 ErrorCode 为 WAEWOULDBLOCK, 判定当前仍然处于连接状态
     * @return false 
     */
    bool IsConnected()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bTemp = m_bIsBlocking;
        SetBlockingEnable(false);
        if(SOCKET_ERROR == send(m_socket, "", 0, SocketFlags::None) && WSAEWOULDBLOCK != WSAGetLastError())
        {
            SetBlockingEnable(bTemp);
            return false;
        }
        SetBlockingEnable(bTemp);
        return true;
    }
    /**
     * @brief  设置 UDP 是否允许分包
     * 
     * @return true 
     * @return false 
     */
    bool GetFragmentEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        return m_bUdpFragmentEnable;
    }
    void SetFragmentEnable(bool blocking)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        m_bUdpFragmentEnable = true;
    }
    /**
     * @brief 该值指定是否 Socket 是用于 IPv4 和 IPv6 的双模式套接字
     * 
     * @return true 
     * @return false 
     */
    bool GetDualModeEnable()
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addr.GetFamily(), "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, bEnable);
        return bEnable;
    }
    void SetDualModeEnable(bool bEnable)
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addr.GetFamily(), "NotSupportedException");
        SetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, bEnable);
    }
    /**
     * @brief 该值指定 Socket 是否可以发送或接收广播数据包
     * 
     * @return true 
     * @return false 
     */
    bool GetBroadcastEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, bEnable);
        return bEnable;
    }
    void SetBroadcastEnable(bool bEnable)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, bEnable);
    }
    /**
     * @brief 该值指定 Tcp Socket 是否仅允许一个进程绑定到端口
     * 
     * @return true 
     * @return false 
     */
    bool GetExclusiveAddressUseEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, bEnable);
        return bEnable;
    }
    void SetExclusiveAddressUseEnable(bool bEnable)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, bEnable);
    }
    LingerOption GetLingerState()
    {
        LingerOption state;
        int size = sizeof(LingerOption);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&state, size);
        return state;
    }
    void SetLingerState(LingerOption state)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&state, sizeof(LingerOption));
    }
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
     * @return true 
     * @return false 
     */
    bool GetNoDelayEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, bEnable);
        return bEnable;
    }
    void SetNoDelayEnable(bool bEnable)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, bEnable);
    }
    /**
     * @brief 获取或设置一个值，它指定 Socket 接收缓冲区的大小。默认值为 8192
     * 
     * @return int 
     */
    int GetReceiveBufferSize()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, size);
        return size;
    }
    void SetReceiveBufferSize(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, size);
    }
    /**
     * @brief 超时值（以毫秒为单位）。 默认值为 0，指示超时期限无限大。 指定 -1 还会指示超时期限无限大
     * 
     * @return int 
     */
    int GetReceiveTimeout()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, size);
        return size;
    }
    void SetReceiveTimeout(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, size);
    }
    /**
     * @brief 获取或设置一个值，它指定 Socket 发送缓冲区的大小。默认值为 8192
     * 
     * @return int 
     */
    int GetSendBufferSize()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, size);
        return size;
    }
    void SetSendBufferSize(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, size);
    }
    /**
     * @brief 超时值（以毫秒为单位）。 默认值为 0，指示超时期限无限大。 指定 -1 还会指示超时期限无限大
     * 
     * @return int 
     */
    int GetSendTimeout()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, size);
        return size;
    }
    void SetSendTimeout(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, size);
    }
    /**
     * @brief 指定 Socket 发送的 Internet 协议 (IP) 数据包的生存时间 (TTL) 值
     * 
     * @return byte 
     */
    byte GetTtl()
    {
        byte val;
        int size = sizeof(byte);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::TTL, (char*)&val, size);
        ERROR_ASSERT(sizeof(byte) == size, "GetTtl failed");
        return val;
    }
    void SetTtl(byte size)
    {
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::TTL, (char*)&size, 1);
    }
    /**
     * @brief 为新建连接创建新的 Socket
     * 从侦听套接字的连接请求队列中同步提取第一个挂起的连接请求，然后创建并返回新的 Socket 。 
     * 不能使用返回的此 Socket 来接受来自连接队列的其他任何连接。 
     * 但是，可以调用方法标识远程主机 Socket 的网络地址和端口号
     * 
     * @return Socket* 
     */
    Socket* Accept()
    {
        Socket* s = new Socket();
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET  != (s->m_socket = accept(m_socket, (SOCKADDR *)s->m_addr.Item, &s->m_addr.Size)),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept"
        );
        return s;
    }
    void Bind(SocketAddress &localEP)
    { 
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != bind(m_socket, (SOCKADDR *)localEP.Item, localEP.Size),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-bind"
        );
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
            /**
                WSANOTINITIALISED
                    A successful WSAStartup call must occur before using this function.
                WSAENETDOWN
                    The network subsystem has failed.
                WSAEADDRINUSE
                    The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs during execution of the bind function, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function.
                WSAEINPROGRESS
                    A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
                WSAEINVAL
                    The socket has not been bound with bind.
                WSAEISCONN
                    The socket is already connected.
                WSAEMFILE
                    No more socket descriptors are available.
                WSAENOBUFS
                    No buffer space is available.
                WSAENOTSOCK
                    The descriptor is not a socket.
                WSAEOPNOTSUPP
                    The referenced socket is not of a type that supports the listen operation. 
             */
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
    int Send(const char* buffer, int size, SocketFlags socketFlags, SocketError &errorcode)
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send */
        if(SOCKET_ERROR == (nRet = send(m_socket, buffer, size, socketFlags)))
        {
            /**
                WSANOTINITIALISED
                    A successful WSAStartup call must occur before using this function.
                WSAENETDOWN
                    The network subsystem has failed.
                WSAEACCES
                    The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST socket option to enable use of the broadcast address.
                WSAEINTR
                    A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.
                WSAEINPROGRESS
                    A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
                WSAEFAULT
                    The buf parameter is not completely contained in a valid part of the user address space.
                WSAENETRESET
                    The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress.
                WSAENOBUFS
                    No buffer space is available.
                WSAENOTCONN
                    The socket is not connected.
                WSAENOTSOCK
                    The descriptor is not a socket.
                WSAEOPNOTSUPP
                    MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.
                WSAESHUTDOWN
                    The socket has been shut down; it is not possible to send on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.
                WSAEWOULDBLOCK
                    The socket is marked as nonblocking and the requested operation would block.
                WSAEMSGSIZE
                    The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.
                WSAEHOSTUNREACH
                    The remote host cannot be reached from this host at this time.
                WSAEINVAL
                    The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.
                WSAECONNABORTED
                    The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
                WSAECONNRESET
                    The virtual circuit was reset by the remote side executing a hard or abortive close. For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a "Port Unreachable" ICMP packet. The application should close the socket as it is no longer usable.
                WSAETIMEDOUT
                    The connection has been dropped, because of a network failure or because the system on the other end went down without notice. 
             * 
             */
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
        /**
            WSANOTINITIALISED
                A successful WSAStartup call must occur before using this function.
            WSAENETDOWN
                The network subsystem has failed.
            WSAEACCES
                The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST parameter to allow the use of the broadcast address.
            WSAEINVAL
                An unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.
            WSAEINTR
                A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.
            WSAEINPROGRESS
                A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
            WSAEFAULT
                The buf or to parameters are not part of the user address space, or the tolen parameter is too small.
            WSAENETRESET
                The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
            WSAENOBUFS
                No buffer space is available.
            WSAENOTCONN
                The socket is not connected (connection-oriented sockets only).
            WSAENOTSOCK
                The descriptor is not a socket.
            WSAEOPNOTSUPP
                MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.
            WSAESHUTDOW
                The socket has been shut down; it is not possible to sendto on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.
            WSAEWOULDBLOCK
                The socket is marked as nonblocking and the requested operation would block.
            WSAEMSGSIZE
                The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.
            WSAEHOSTUNREACH
                The remote host cannot be reached from this host at this time.
            WSAECONNABORTED
                The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
            WSAECONNRESET
                The virtual circuit was reset by the remote side executing a hard or abortive close. For UPD sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a "Port Unreachable" ICMP packet. The application should close the socket as it is no longer usable.
            WSAEADDRNOTAVAIL
                The remote address is not a valid address, for example, ADDR_ANY.
            WSAEAFNOSUPPORT
                Addresses in the specified family cannot be used with this socket.
            WSAEDESTADDRREQ
                A destination address is required.
            WSAENETUNREACH
                The network cannot be reached from this host at this time.
            WSAEHOSTUNREACH
                A socket operation was attempted to an unreachable host.
            WSAETIMEDOUT
                The connection has been dropped, because of a network failure or because the system on the other end went down without notice.
            */
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
    inline int IOControl(IOControlCode ioControlCode, DWORD &optionInOutValue)
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
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, char* buffer, int &optlen)
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != getsockopt(m_socket, (int)optionLevel, optionName, buffer, &optlen), 
            "SetSocketOption failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-getsockopt"
        );
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, bool &optionValue)
    {
        int optlen = sizeof(bool);
        GetSocketOption(optionLevel, optionName, (char *)&optionValue, optlen);
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, int &optionValue)
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
     * @brief 禁用 Socket 上的发送和接收
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
readonly System::Net::Sockets::Socket::SocketlibraryInitlializer System::Net::Sockets::Socket::m_socketlibraryInitlializer;
#endif