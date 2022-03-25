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
     * @brief ��ʼ�� Ipv4 ����ʵ��
     * 
     */
    TcpClient() : Socket(AddressFamily::InterNetwork, SocketType::Stream, ProtocolType::IPPROTO_TCP){}
    /**
     * @brief ��ʼ�� Ipv4/Ipv6 ����ʵ��
     * 
     * @param family 
     */
    TcpClient(AddressFamily family) : Socket(family, SocketType::Stream, ProtocolType::IPPROTO_TCP){}
    /**
     * @brief ��ʼ������ʵ����ͬ�����ӵ�ָ�������ϵ�ָ���˿�, ������캯���ɹ�, �����ظ����� Connect
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
     * @brief ��ȡ�Ѿ�����������ҿɹ���ȡ��������
     * 
     */
    DECLARE_CONST_GETTER(DWORD, Available,
    {
        return Socket::GetAvailable();
    })
    /**
     * @brief ��ȡ���� Socket
     * 
     */
    DECLARE_GETTER(WEAK_PTR(Socket), Client,
    {
        return this;
    })
    /**
     * @brief TcpClient �Ƿ������ӵ�Զ������
     * 
     */
    DECLARE_CONST_GETTER(bool, Connected,
    {
        return Socket::GetConnected();
    })
    /**
     * @brief ָ�� TcpClient �Ƿ�ֻ����һ���ͻ���ʹ�ö˿�
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
     * @brief ��ȡ�������йع������׽��ֵ��ӳ�״̬����Ϣ
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
     * @brief ���ͻ���ջ�����δ��ʱ�����ӳ�
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
     * @brief ��ȡ�����ý��ջ������Ĵ�С
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
     * @brief ��ȡ�����ý��ճ�ʱ
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
     * @brief ��ȡ�����÷��ͻ������Ĵ�С
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
     * @brief ��ȡ�����÷��ͳ�ʱ
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
     * @brief ������رջ��� TCP ����
     * 
     */
    void Close()
    {
        Socket::Close();
    }
    /**
     * @brief ʹ��ָ���� IP ��ַ�Ͷ˿ںŽ��ͻ������ӵ� TCP ����
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