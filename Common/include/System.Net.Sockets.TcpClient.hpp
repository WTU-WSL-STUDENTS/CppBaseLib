/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-23 15:49:09
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 15:24:40
 */
#ifndef SYSTEM_NET_SOCKETS_TCPCLIENT_HPP
#define SYSTEM_NET_SOCKETS_TCPCLIENT_HPP
#include <System.Net.IPAddress.hpp>
#include <System.Net.Sockets.Socket.hpp>

namespace System::Net::Sockets
{
    class TcpListener;
    class TcpClient;
};
class System::Net::Sockets::TcpClient : protected Socket
{
friend class TcpListener;
protected:
    TcpClient(const Socket& client) : Socket(client){}
public:
    /**
     * @brief 初始化 Ipv4 的新实例
     * 
     */
    TcpClient() : Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IPPROTO_TCP){}
    /**
     * @brief 初始化 Ipv4/Ipv6 的新实例
     * 
     * @param family 
     */
    TcpClient(AddressFamily family) : Socket(family, SocketType::Stream, ProtocolType::IPPROTO_TCP){}
    /**
     * @brief 初始化的新实例并同步连接到指定主机上的指定端口, 如果构造函数成功, 不必重复调用 Connect
     * 
     * @param remoteEP 
     * @param remotePort 
     */
    TcpClient(const IPAddress& remoteEP, UInt16 remotePort) : TcpClient(remoteEP.m_eFamily)
    {
        Connect(remoteEP, remotePort);
    }
    inline int GetSendTimeout()
    {
        return Socket::GetSendTimeout();
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
     * @brief TcpClient 是否已连接到远程主机
     * 
     */
    DECLARE_CONST_GETTER(bool, Connected,
    {
        return Socket::GetConnected();
    })
    /**
     * @brief 指定 TcpClient 是否只允许一个客户端使用端口
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
     * @brief 获取或设置有关关联的套接字的延迟状态的信息
     * 
     */
    DECLARE_INDEXER(LingerOption, LingerState,
    {
        return Socket::GetLingerState();
    },
    {
        Socket::SetLingerState(SETTER_VALUE);
    })
    /**
     * @brief 发送或接收缓冲区未满时禁用延迟
     * 
     */
    DECLARE_INDEXER(bool, NoDelay,
    {
        return Socket::GetNoDelay();
    },
    {
        Socket::SetNoDelay(SETTER_VALUE);
    })
    /**
     * @brief 获取或设置接收缓冲区的大小
     * 
     */
    DECLARE_INDEXER(int, ReceiveBufferSize,
    {
        return Socket::GetReceiveBufferSize();
    },
    {
        Socket::SetReceiveBufferSize(SETTER_VALUE);
    })
    /**
     * @brief 获取或设置接收超时
     * 
     */
    DECLARE_INDEXER(int, ReceiveTimeout,
    {
        return Socket::GetReceiveTimeout();
    },
    {
        Socket::SetReceiveTimeout(SETTER_VALUE);
    })
    /**
     * @brief 获取或设置发送缓冲区的大小
     * 
     */
    DECLARE_INDEXER(int, SendBufferSize,
    {
        return Socket::GetSendBufferSize();
    },
    {
        Socket::SetSendBufferSize(SETTER_VALUE);
    })
    /**
     * @brief 获取或设置发送超时
     * 
     */
    DECLARE_INDEXER(int, SendTimeout,
    {
        return Socket::GetSendTimeout();
    },
    {
        Socket::SetSendTimeout(SETTER_VALUE);
    })
    /**
     * @brief 并请求关闭基础 TCP 连接
     * 
     */
    void Close()
    {
        Socket::Close();
    }
    /**
     * @brief 使用指定的 IP 地址和端口号将客户端连接到 TCP 主机
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
};
#endif