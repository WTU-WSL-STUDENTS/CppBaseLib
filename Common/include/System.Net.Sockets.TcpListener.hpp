/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-15 10:12:39
 * @LastEditors: like
 * @LastEditTime: 2022-03-27 16:47:26
 */
#ifndef SYSTEM_NET_SOCKETS_TCP_LISTENER_HPP
#define SYSTEM_NET_SOCKETS_TCP_LISTENER_HPP
#include <System.Net.Sockets.TcpClient.hpp>

namespace System::Net::Sockets
{
    class TcpListener;
};
class System::Net::Sockets::TcpListener : protected Socket
{
private:
    SocketAddress m_addr;
public:
    TcpListener(const IPAddress& localaddr, UInt16 localport) : 
        Socket(localaddr.m_eFamily, SocketType::Stream, ProtocolType::IPPROTO_TCP), 
        m_addr(SocketAddress(localaddr, localport))
    {
        TCPIP_AF_ASSERT(localaddr.m_eFamily);
        PORT_ASSERT(localport);
        Bind(m_addr);
    }
    virtual ~TcpListener()
    {
    }
    /**
     * @brief 指定 TcpListener 是否只允许一个基础套接字来侦听特定端口
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
     * @brief 获取 TcpListener 监听的地址信息
     * 
     */
    DECLARE_CONST_GETTER(SocketAddress, LocalEndPoint,
    {
        return Socket::GetLocalEndPoint();
    })
    /**
     * @brief 获取基础 Socket
     * 
     */
    DECLARE_GETTER(WEAK_PTR(Socket), Server,
    {
        return this;
    })
    /**
     * @brief 阻塞式捕获 Connect , 并在堆区创建新的 Socket 对象
     * 
     * @return Socket* 
     */
    Socket* AcceptSocket()
    {
        SocketAddress remoteEP;
        return Accept(remoteEP);
    }
    /**
     * @brief 阻塞式捕获 Connect , 并在堆区创建新的 TcpClient 对象
     * 
     * @return Socket* 
     */
    TcpClient* AcceptTcpClient()
    {
        SocketAddress remoteEP;
        ERROR_ASSERT(sizeof(TcpClient) == sizeof(Socket), "AcceptTcpClient error, Can't trans Socket* to TcpClient*");
        return (TcpClient*)Accept(remoteEP);
    }
    /**
     * @brief 确定是否有挂起的连接请求
     * 
     * @return true 
     * @return false 
     */
    bool Pending()
    {
        return Poll(0, SelectMode::SelectRead)/* && Receive buffer empty && 连接未关闭 */;
    }
    void Start(int backlog = SOMAXCONN)
    {
        Listen(backlog);
    }
    void Stop()
    {
        if(!(GetLingerState().l_onoff))
        {
            Shutdown(SocketShutdown::Both);
        }
        Close();
    }
    static TcpListener* Create(UInt16 port)
    {
        return new TcpListener(IPAddress::Any, port);
    }
};
#endif