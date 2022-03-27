/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-25 14:06:23
 * @LastEditors: like
 * @LastEditTime: 2022-03-27 11:21:56
 */
#ifndef SYSTEM_NET_SOCKETS_UDPCLIENT_HPP
#define SYSTEM_NET_SOCKETS_UDPCLIENT_HPP
#include <System.Net.Sockets.Socket.hpp>

namespace System::Net::Sockets
{
    class UdpClient;
};

class System::Net::Sockets::UdpClient : protected Socket
{
public:
    //
    UdpClient() : Socket(AddressFamily::InterNetwork, SocketType::Dgram, ProtocolType::IPPROTO_UDP){}
    /**
     * @brief 初始化 UdpClient 类的 Ipv4 新实例，并将它绑定到所提供的本地端口号, 地址由系统分配
     * 
     * @param port 
     */
    UdpClient(UInt16 port) : UdpClient(IPAddress::Any, port){}
    /**
     * @brief 初始化 UdpClient 类的 Ipv4/Ipv6 新实例，并将它绑定到所提供的本地端口号, 地址由系统分配
     * 
     * @param port 
     * @param family 
     */
    UdpClient(UInt16 port, AddressFamily family) : UdpClient(AddressFamily::InterNetwork == family? IPAddress::Any : IPAddress::IPv6Any, port){}
    /**
     * @brief 初始化 UdpClient 类的 Ipv4/Ipv6 新实例，并将它绑定到所提供的本地地址和端口号
     * 
     * @param localaddr 
     * @param port 
     */
    UdpClient(const IPAddress& localaddr, UInt16 port) : Socket(localaddr.m_eFamily, SocketType::Dgram, ProtocolType::IPPROTO_UDP)
    {
        PORT_ASSERT(port);
        TCPIP_AF_ASSERT(localaddr.m_eFamily);
        SocketAddress addr(localaddr, port);
        Bind(addr);
    }
    /**
     * @brief 获取已经从网络接收且可供读取的数据量
     * 
     */
    DECLARE_CONST_GETTER(DWORD, Available,
    {
        return Socket::GetAvailable();
    })
    /**
     * @brief 获取基础 Socket
     * 
     */
    DECLARE_GETTER(WEAK_PTR(Socket), Client,
    {
        return this;
    })
    /**
     * @brief 指定 UdpClient 是否允许将 Internet 协议 (IP) 数据报分段
     * 
     */
    DECLARE_INDEXER(bool, DontFragment, 
    {
        return Socket::GetDontFragment();
    },
    {
        Socket::SetDontFragment(SETTER_VALUE);
    })
    /**
     * @brief 指定 UdpClient 是否可以发送或接收广播数据包
     * 
     */
    DECLARE_INDEXER(bool, EnableBroadcast, 
    {
        VALRET_ASSERT(AddressFamily::InterNetwork == GetAddressFamily(), false);
        return Socket::GetEnableBroadcast();
    },
    {
        ERROR_ASSERT(AddressFamily::InterNetwork == GetAddressFamily(), "Ipv6 not support broadcast");
        Socket::SetEnableBroadcast(SETTER_VALUE);
    })
    /**
     * @brief 指定 UdpClient 是否只允许一个客户端使用端口
     * 
     */
    DECLARE_INDEXER(bool, ExclusiveAddressUse, 
    {
        return Socket::GetExclusiveAddressUse();
    },
    {
        Socket::SetExclusiveAddressUse(SETTER_VALUE);
    })
    /**
     * @brief 指定 UdpClient 是否将输出多播数据包传递给发送应用程序本身
     * 
     */
    DECLARE_INDEXER(bool, MulticastLoopback, 
    {
        return Socket::GetMulticastLoopback();
    },
    {
        Socket::SetMulticastLoopback(SETTER_VALUE);
    })
    /**
     * @brief 指定 UdpClient 发送的 Internet 协议 (IP) 数据包的生存时间 (TTL)
     * 
     */
    DECLARE_INDEXER(byte, Ttl, 
    {
        return Socket::GetTtl();
    },
    {
        Socket::SetTtl(SETTER_VALUE);
    })
    /**
     * @brief 关闭 UDP 连接
     * 
     */
    void Close()
    {
        Socket::Close();
    }
    /**
     * @brief 使用指定的 IP 地址和端口号将客户端连接到 UDP 主机 (UDP 可多次调用该接口)
     * UDP Connect 好处
     *  1. 在普通情况下都是 建立连结->发送报文->断开连结->建立连结->发送报文->断开连结 这样循环，但是在用了connect 之后就会变成 建立连结->发送报文->发送报文
     *  2. 可以调用 recv / send 接口
     * 
     * @param remoteEP 
     * @param remotePort 
     */
    void Connect(const IPAddress& remoteEP, UInt16 remotePort)
    {
        TCPIP_AF_ASSERT(remoteEP.m_eFamily);
        PORT_ASSERT(remotePort);
        SocketAddress addr(remoteEP, remotePort);
        Socket::Connect(addr);
    }
    /**
     * @brief 退出组播
     * 
     * @param remoteEP 
     * @param remotePort 
     */
    void DropMulticastGroup(const IPAddress& remoteEP, UInt32 port = 0)
    {
        ERROR_ASSERT(IsMulicastAddress(remoteEP), "ArgumentException");
        /* https://docs.microsoft.com/en-us/windows/win32/api/ws2ipdef/ns-ws2ipdef-ip_mreq */
        /* https://www.cnblogs.com/IntelligencePointer/p/14186663.html */
        switch(GetAddressFamily())
        {
            case AddressFamily::InterNetwork:
            {
                ip_mreq mulitcast;
                mulitcast.imr_multiaddr             = remoteEP.ipv4Addr;
                mulitcast.imr_interface.S_un.S_addr = port;
                SetSocketOption(SocketOptionLevel::IP, SocketOptionName::DropMembership, (char*)&mulitcast, sizeof(ip_mreq));
            }
            break;
            case AddressFamily::InterNetworkV6:
            {
                ipv6_mreq mulitcast = {0};
                mulitcast.ipv6mr_multiaddr  = remoteEP.ipv6Addr;
                mulitcast.ipv6mr_interface  = port;
                SetSocketOption(SocketOptionLevel::IP, SocketOptionName::DropMembership, (char*)&mulitcast, sizeof(ipv6_mreq));
            }
            break;
        };
    }
    /**
     * @brief 加入组播
     * 
     * @param remoteEP 
     * @param remotePort 
     */
    void JoinMulticastGroup(const IPAddress& remoteEP, UInt32 port = 0)
    {
        ERROR_ASSERT(IsMulicastAddress(remoteEP), "ArgumentException");
        switch(GetAddressFamily())
        {
            case AddressFamily::InterNetwork:
            {
                ip_mreq mulitcast;
                mulitcast.imr_multiaddr             = remoteEP.ipv4Addr;
                mulitcast.imr_interface.S_un.S_addr = port;
                SetSocketOption(SocketOptionLevel::IP, SocketOptionName::AddMembership, (char*)&mulitcast, sizeof(ip_mreq));
            }
            break;
            case AddressFamily::InterNetworkV6:
            {
                ipv6_mreq mulitcast = {0};
                mulitcast.ipv6mr_multiaddr  = remoteEP.ipv6Addr;
                mulitcast.ipv6mr_interface  = port;
                SetSocketOption(SocketOptionLevel::IP, SocketOptionName::AddMembership, (char*)&mulitcast, sizeof(ipv6_mreq));
            }
            break;
        };
    }
    /**
     * @brief 返回由远程主机发送的 UDP 数据报
     * 
     * @param buffer 用户接收缓冲区
     * @param size 用户缓冲区大小
     * @param remoteEP 发送端 Ip 信息
     * @return int 实际接收长度
     */
    int Receive(char* buffer, int size, SocketAddress &remoteEP)
    {
        return Socket::ReceiveFrom(buffer, size, SocketFlags::None, remoteEP);
    }
    /**
     * @brief 将 UDP 数据报发送到远程主机。
     * 
     * @param buffer 
     * @param size 
     * @param remoteEP 
     * @return int 
     */
    int Send(const char* buffer, int size, const SocketAddress &remoteEP)
    {
        return Socket::SendTo(buffer, size, SocketFlags::None, remoteEP);
    }
private:
    /**
     * @brief 是否是组播地址
     * 
     * @param addr 
     * @return true 
     * @return false 
     */
    bool IsMulicastAddress(const IPAddress& addr)
    {
        const UInt32 MulticastMin = 0xE0;
        const UInt32 MulticastMax = 0xFFFFFFEF;
        TCPIP_AF_ASSERT(addr.m_eFamily);
        return AddressFamily::InterNetwork == addr.m_eFamily ? 
            MulticastMin < addr.ipv4Addr.S_un.S_addr && addr.ipv4Addr.S_un.S_addr < MulticastMax :
            0xff == addr.ipv6Addr.u.Byte[0] && !(0x01 & addr.ipv6Addr.u.Byte[1]); 
            /**
             * @brief IPv6 multicast 
             * https://cloud.tencent.com/developer/article/1659122
             * 
             * | 0xff | flags | scope |
             * 0      7(0RTP) 11      15
             */
    }
};

#endif