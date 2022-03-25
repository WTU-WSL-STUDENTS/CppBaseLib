/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-25 14:06:23
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 17:07:49
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
     * @brief ��ʼ�� UdpClient ��� Ipv4 ��ʵ�����������󶨵����ṩ�ı��ض˿ں�, ��ַ��ϵͳ����
     * 
     * @param port 
     */
    UdpClient(UInt16 port) : UdpClient(IPAddress::Any, port){}
    /**
     * @brief ��ʼ�� UdpClient ��� Ipv4/Ipv6 ��ʵ�����������󶨵����ṩ�ı��ض˿ں�, ��ַ��ϵͳ����
     * 
     * @param port 
     * @param family 
     */
    UdpClient(UInt16 port, AddressFamily family) : UdpClient(AddressFamily::InterNetwork == family? IPAddress::Any : IPAddress::IPv6Any, port){}
    /**
     * @brief ��ʼ�� UdpClient ��� Ipv4/Ipv6 ��ʵ�����������󶨵����ṩ�ı��ص�ַ�Ͷ˿ں�
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
     * @brief ָ�� UdpClient �Ƿ����� Internet Э�� (IP) ���ݱ��ֶ�
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
     * @brief ָ�� UdpClient �Ƿ���Է��ͻ���չ㲥���ݰ�
     * 
     */
    DECLARE_INDEXER(bool, EnableBroadcast, 
    {
        return Socket::GetEnableBroadcast();
    },
    {
        Socket::SetEnableBroadcast(SETTER_VALUE);
    })
    /**
     * @brief ָ�� UdpClient �Ƿ�ֻ����һ���ͻ���ʹ�ö˿�
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
     * @brief ָ�� UdpClient �Ƿ�����ಥ���ݰ����ݸ�����Ӧ�ó�����
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
     * @brief ָ�� UdpClient ���͵� Internet Э�� (IP) ���ݰ�������ʱ�� (TTL)
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
     * @brief �ر� UDP ����
     * 
     */
    void Close()
    {
        Socket::Close();
    }
    /**
     * @brief ʹ��ָ���� IP ��ַ�Ͷ˿ںŽ��ͻ������ӵ� UDP ���� (UDP �ɶ�ε��øýӿ�)
     * UDP Connect �ô�
     *  1. ����ͨ����¶��� ��������->���ͱ���->�Ͽ�����->��������->���ͱ���->�Ͽ����� ����ѭ��������������connect ֮��ͻ��� ��������->���ͱ���->���ͱ���
     *  2. ���Ե��� recv / send �ӿ�
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
     * @brief �˳��鲥
     * 
     * @param remoteEP 
     * @param remotePort 
     */
    void DropMulticastGroup(const IPAddress& remoteEP, UInt16 remotePort)
    {
        /* https://docs.microsoft.com/en-us/windows/win32/api/ws2ipdef/ns-ws2ipdef-ip_mreq */
        /* https://www.cnblogs.com/IntelligencePointer/p/14186663.html */
        switch(GetAddressFamily())
        {
            case AddressFamily::InterNetwork:
            {
                ip_mreq mulitcast;
                mulitcast.imr_multiaddr.S_un.S_addr = INADDR_ANY;
                mulitcast.imr_interface.S_un.S_addr = 
                SetSocketOption(SocketOptionLevel::IP, SocketOptionName::DropMembership, (char*)&SETTER_VALUE, sizeof(ip_mreq));

            }
            break;
            case AddressFamily::InterNetworkV6:
            {

            }
            break;
        };
    }
    /**
     * @brief �����鲥
     * 
     * @param remoteEP 
     * @param remotePort 
     */
    void JoinMulticastGroup(const IPAddress& remoteEP, UInt16 remotePort)
    {
        
    }
    /**
     * @brief ������Զ���������͵� UDP ���ݱ�
     * 
     * @param buffer �û����ջ�����
     * @param size �û���������С
     * @param remoteEP ���Ͷ� Ip ��Ϣ
     * @return int ʵ�ʽ��ճ���
     */
    int Receive(char* buffer, int size, SocketAddress &remoteEP)
    {
        return Socket::ReceiveFrom(buffer, size, SocketFlags::None, remoteEP);
    }
    /**
     * @brief �� UDP ���ݱ����͵�Զ��������
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
};

#endif