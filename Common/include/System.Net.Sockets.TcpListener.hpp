/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-15 10:12:39
 * @LastEditors: like
 * @LastEditTime: 2022-03-16 09:18:46
 */
#ifndef SYSTEM_NET_SOCKETS_TCP_LISTENER_HPP
#define SYSTEM_NET_SOCKETS_TCP_LISTENER_HPP
#include <System.Net.Sockets.Socket.hpp>

namespace System::Net::Sockets
{
    class TcpListener;
};
class System::Net::Sockets::TcpListener
{
public:
private:
    SocketConfig m_sockCfg;
    SOCKET m_socket;
public:
    TcpListener(const IPAddress& localaddr, UInt16 localport) : 
    {
        m_sockCfg.ipv6Addr.sin6_family  = localaddr.m_eFamily;
        m_sockCfg.ipv6Addr.sin6_port    = IPAddress::HostToNetworkOrder(localport);
        switch (localaddr.m_eFamily)
        {
        case Ipv4:
            {
                m_sockCfg.m_nConfigLen = sizeof(sockaddr_in); 
                memcpy(&m_sockCfg.ipv4Addr.sin_addr, &localaddr.m_addr, sizeof(IN4_ADDR));
            }
            break;
        case Ipv6:
            {
                m_sockCfg.m_nConfigLen = sizeof(sockaddr_in6); 
                memcpy(&m_sockCfg.ipv6Addr.sin6_addr, &localaddr.m_addr, sizeof(IN6_ADDR));
            }
            break;
        default:
            break;
        }
        SOCKETAPI_ASSERT(INVALID_SOCKET != (m_socket = socket(localaddr.m_eFamily, SocketType::Stream, ProtocolType::IPPROTO_TCP)), "Construct TcpListener failed, create socket failed");
        SOCKETAPI_ASSERT(SOCKET_ERROR   != bind(m_socket, (sockaddr*)&m_sockCfg.ipv6Addr, m_sockCfg.m_nConfigLen)), "Construct TcpListener failed, bind error");
    }
};

#endif