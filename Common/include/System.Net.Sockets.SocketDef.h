/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-27 14:08:05
 * @LastEditors: like
 * @LastEditTime: 2022-03-27 20:37:19
 */
#ifndef SYSTEM_NET_SOCKETS_SOCKETDEF_H
#define SYSTEM_NET_SOCKETS_SOCKETDEF_H

#include <System.Net.IPAddress.hpp>
namespace System::Net::Sockets
{
    using LingerOption   = linger;
    using ProtocolType  = IPPROTO;
    using SocketError   = int;
    enum class IOControlCode : long;
    enum class SelectMode;
    enum class SocketAsyncOperation : long;
    enum class SocketFlags;
    enum class SocketOptionLevel;
    enum class SocketOptionName;
    enum class SocketShutdown;
    enum class SocketType;
};
enum class System::Net::Sockets::IOControlCode : long /* https://docs.microsoft.com/zh-cn/dotnet/api/system.net.sockets.iocontrolcode?view=net-5.0 */
{
    /**
     * @brief 当等待接收数据时启用通知
     * 
     */
    AsyncIO         =  (long)FIOASYNC,
    /**
     * @brief 控制套接字的阻塞( blocking mode )行为。 如果使用此控制代码指定的参数为 0，套接字置于阻塞模式下。 如果参数不为零，套接字置于非阻塞模式下
     * 
     */
    NonBlockingIO   = (long)FIONBIO,
    /**
     * @brief 返回可读取的字节数
     * 
     */
    DataToRead      = (long)FIONREAD,
};
/**
 * @brief 为 Poll(Int32, SelectMode) 方法定义轮询模式
 * 
 */
enum class System::Net::Sockets::SelectMode
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
 * @brief 最近使用此上下文对象执行的异步套接字操作的类型
 * 
 */
enum class System::Net::Sockets::SocketAsyncOperation : long
{
    /**
     * @brief 没有套接字操作。
     * 
     */
    None = 0,	
    /**
     * @brief 一个套接字 Accept 操作。 
     * 
     */
    Accept,
    /**
     * @brief 一个套接字 Connect 操作。
     * 
     */
    Connect,
    /**
     * @brief 一个套接字 Disconnect 操作。
     * 
     */
    Disconnect,
    /**
     * @brief 一个套接字 Receive 操作。
     * 
     */
    Receive,
    /**
     * @brief 一个套接字 ReceiveFrom 操作。
     * 
     */
    ReceiveFrom,
    /**
     * @brief 一个套接字 ReceiveMessageFrom 操作。
     * 
     */
    ReceiveMessageFrom,
    /**
     * @brief 一个套接字 Send 操作。
     * 
     */
    Send,
    /**
     * @brief 一个套接字 SendPackets 操作。
     * 
     */
    SendPackets,
    /**
     * @brief 一个套接字 SendTo 操作。
     * 
     */
    SendTo
};
/**
 * @brief 指定套接字发送和接收行为。此枚举支持其成员值的按位组合
 * 
 */
enum class System::Net::Sockets::SocketFlags
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
enum class System::Net::Sockets::SocketOptionName
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
enum class System::Net::Sockets::SocketShutdown
{
    Receive = 0,
    Send,
    Both
};
/**
 * @brief 指定 Socket 类的实例表示的套接字类型
 * 
 */
enum class System::Net::Sockets::SocketType
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
#endif