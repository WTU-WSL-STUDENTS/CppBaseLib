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
     * @brief ָ�� TcpListener �Ƿ�ֻ����һ�������׽����������ض��˿�
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
     * @brief ��ȡ TcpListener �����ĵ�ַ��Ϣ
     * 
     */
    DECLARE_CONST_GETTER(SocketAddress, LocalEndPoint,
    {
        return Socket::GetLocalEndPoint();
    })
    /**
     * @brief ��ȡ���� Socket
     * 
     */
    DECLARE_GETTER(WEAK_PTR(Socket), Server,
    {
        return this;
    })
    /**
     * @brief ����ʽ���� Connect , ���ڶ��������µ� Socket ����
     * 
     * @return Socket* 
     */
    Socket* AcceptSocket()
    {
        SocketAddress remoteEP;
        return Accept(remoteEP);
    }
    /**
     * @brief ����ʽ���� Connect , ���ڶ��������µ� TcpClient ����
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
     * @brief ȷ���Ƿ��й������������
     * 
     * @return true 
     * @return false 
     */
    bool Pending()
    {
        return Poll(0, SelectMode::SelectRead)/* && Receive buffer empty && ����δ�ر� */;
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