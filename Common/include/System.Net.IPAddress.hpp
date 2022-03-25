/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-14 15:39:04
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 14:51:01
 */
#ifndef SYSTEM_NET_IP_ADDRESS_HPP
#define SYSTEM_NET_IP_ADDRESS_HPP

#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")
#include <string.h>
#include <CompliedEntry.h>

#ifdef __WINDOWS
#   pragma warning(disable:4996)
#endif

#ifdef ASSERT_ENABLE
#   define SOCKETAPI_ASSERT(condition, errorMsg)        \
    do                                                  \
    {                                                   \
        bool cond = condition;                          \
        if(!(cond))                                     \
        {                                               \
            printf("\nWinapi assert " #condition        \
            "\n%s , error code : %d\n", errorMsg, WSAGetLastError());    \
            WSASetLastError(0);                         \
            assert(cond);                               \
        }                                               \
    }while(0)
#   define PORT_ASSERT(port)        ERROR_ASSERT(IPEndPoint::MinPort < (port) && (port) < IPEndPoint::MaxPort, "ArgumentOutOfRangeException")
#   define TCPIP_AF_ASSERT(family)  ERROR_ASSERT(AddressFamily::InterNetwork == (AddressFamily)(family) || AddressFamily::InterNetworkV6 == (AddressFamily)(family), "AddressFamily not support")
  
#endif
#define MAX_InterNetwork_LENGTH (4 * 4)
#define MAX_InterNetworkV6_LENGTH (8 * 5)
#define MAX_IP_LENGTH MAX_InterNetworkV6_LENGTH

/* ------------------------------------ IPAddress convert function --------------------------------------------- */
/**
 *  �����ֽ�
 *  |
 *  |--->HostToNetworkOrder ]
 *  |<---NetworkToHostOrder ] �����ֽ�(���)
 *  |       ?           |
 *  |       |           |
 *  |       |           ��
 *  |       inet_pton   inet_ntop
 *  |--->inet_ntoa              ]
 *  |<---inet_aton / inet_addr  ]Ip �ַ���
 *  |
 */

namespace System::Net
{
    enum class AddressFamily : ADDRESS_FAMILY;
    class IPAddress;
    class SocketAddress;
    class EndPoint;
    class IPEndPoint;
};
enum class System::Net::AddressFamily : ADDRESS_FAMILY
{
    Unspecified     = AF_UNSPEC,
    /**
     * @brief ����socket�Ŀ���Ϸ�չ��һ��IPC���ƣ�����UNIX Domain Socket��
     * ��Ȼ����socketҲ������ͬһ̨�����Ľ��̼�ͨѶ��ͨ��loopback��ַ127.0.0.1��������UNIX Domain Socket����IPC ����Ч�� ��
     * ����Ҫ��������Э��ջ
     * ����Ҫ������������У��͡�ά����ź�Ӧ��ȣ��ɿ��Ը�ǿ
     * 
     * ֻ�ǽ�Ӧ�ò����ݴ�һ�����̿�������һ�����̡�
     * ������Ϊ��IPC���Ʊ������ǿɿ���ͨѶ��������Э����Ϊ���ɿ���ͨѶ��Ƶġ�
     * UNIX Domain SocketҲ�ṩ���������������ݰ�����API�ӿڣ�
     * ������TCP��UDP������������Ϣ��UNIX Domain SocketҲ�ǿɿ��ģ���Ϣ�Ȳ��ᶪʧҲ����˳����ҡ�
     * 
     */
    Unix            = AF_UNIX,
    /**
     * @brief IP �汾 4 �ĵ�ַ
     * 
     */
    InterNetwork    = AF_INET,
    /**
     * @brief IP �汾 6 �ĵ�ַ
     * 
     */
    InterNetworkV6  = AF_INET6
};
class System::Net::IPAddress
{
public:
    union
    {
        IN_ADDR ipv4Addr;
        IN6_ADDR ipv6Addr;
    };
    AddressFamily m_eFamily;
    IPAddress(){}
    /**
     * @brief Construct a new IPAddress object
     * 
     * @param strIp InterNetwork / InterNetworkV6 �ַ������ʽ
     */
    IPAddress(const char* strIp)
    {
        ERROR_ASSERT(AddressFamily::Unspecified < (m_eFamily = GetIpTypeFromStr(strIp)), "IP string format error");
        int nRet;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != (nRet = inet_pton((int)m_eFamily, strIp, &ipv6Addr)), 
            "Trans IP string to binary numeric IP address in network byte order failed."
            "https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_pton"
        );
        ERROR_ASSERT(1 == nRet, "IPAddress constructed failed, IP string format error");
    }
    /**
     * @brief Construct a new IPAddress object
     * 
     * @param nNetworkIp InterNetwork �����ֽ��� 127 0 0 1 -> 0x0100007f
     */
    IPAddress(UInt32 nNetworkIp)
    {
        m_eFamily = AddressFamily::InterNetwork;
        ipv4Addr.S_un.S_addr = nNetworkIp;
    }
    /**
     * @brief IPAddress ת�ַ���
     * 
     * @param buffer 
     */
    void ToString(char* buffer) const
    {
        SOCKETAPI_ASSERT
        (
            inet_ntop((int)m_eFamily, &ipv6Addr, buffer, MAX_IP_LENGTH), 
            "ToString failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_ntop"
        );
    }
    bool operator==(IPAddress& other)
    {
        VALRET_ASSERT(other.m_eFamily == m_eFamily, false);
        switch(m_eFamily)
        {
            case AddressFamily::InterNetwork:
            {
                return other.ipv4Addr.S_un.S_addr == ipv4Addr.S_un.S_addr;
            }
            break;
            case AddressFamily::InterNetworkV6:
            {
                return (other.ipv6Addr.u.Word[0] == ipv6Addr.u.Word[0]) &&
                    (other.ipv6Addr.u.Word[1] == ipv6Addr.u.Word[1]) &&
                    (other.ipv6Addr.u.Word[2] == ipv6Addr.u.Word[2]) &&
                    (other.ipv6Addr.u.Word[3] == ipv6Addr.u.Word[3]) &&
                    (other.ipv6Addr.u.Word[4] == ipv6Addr.u.Word[4]) &&
                    (other.ipv6Addr.u.Word[5] == ipv6Addr.u.Word[5]) &&
                    (other.ipv6Addr.u.Word[6] == ipv6Addr.u.Word[6]) &&
                    (other.ipv6Addr.u.Word[7] == ipv6Addr.u.Word[7]);
            }
            break;
        };
        return true;
    }
    /**
     * @brief ָʾ����������������������ӿ��ϵĿͻ��˻ 0.0.0.0
     * 
     */
    static readonly IPAddress Any;
    /**
     * @brief  IP �㲥��ַ 255.255.255.255
     * 
     */
    static readonly IPAddress Broadcast;
    /**
     * @brief ���ص�ַ 127.0.0.1
     * 
     */
    static readonly IPAddress Loopback;
    /**
     * @brief �ṩָʾ��Ӧʹ���κ�����ӿڵ� IP ��ַ 255.255.255.255
     * 
     */
    static readonly IPAddress None;
    /**
     * 
     * @brief Bind(EndPoint) ����ʹ�� IPv6Any �ֶ�ָʾ Socket ����������������ӿ��ϵĿͻ��˻ 0:0:0:0:0:0:0;��ѹ����ʾ���У��ֶε�Ч�� ::
     * 
     */
    static readonly IPAddress IPv6Any;
    /**
     * @brief  ���ص�ַ,0:0:0:0:0:0:0:1�������::1�����þ����ʾ��
     * 
     */
    static readonly IPAddress IPv6Loopback;
    /**
     * @brief �ṩָʾ��Ӧʹ���κ�����ӿڵ� IP ��ַ 0:0:0:0:0:0:0:0 �� ::0���þ����ʾ��
     * 
     */
    static readonly IPAddress IPv6None;
    /**
     * @brief �ж��ַ����� InterNetwork �� InterNetworkV6 ��ʽ
     * 
     * @param strIp 
     * @return AddressFamily 
     */
    static AddressFamily GetIpTypeFromStr(const char* strIp)
    {
        ERROR_ASSERT(MAX_IP_LENGTH > strlen(strIp), "IPAddress format error");
        return strstr(strIp, ":") ? AddressFamily::InterNetworkV6 : strstr(strIp, ".") ? AddressFamily::InterNetwork : AddressFamily::Unspecified; 
    }
    /**
     * @brief ��ֵ�������ֽ�(��� or С�� �� cpu / ����ϵͳ ����)˳��ת��Ϊ�����ֽ�(���)˳��
     * 
     * @param host 
     * @return UInt16 
     */
    static inline UInt16 HostToNetworkOrder(UInt16 host)
    {
        return htons(host);
    }	
    /**
     * @brief ��ֵ�������ֽ�˳��ת��Ϊ�����ֽ�˳��
     * 
     * @param host 
     * @return UInt32 
     */
    static inline UInt32 HostToNetworkOrder(UInt32 host)
    {
        return htonl(host);
    }	
    /**
     * @brief ��ֵ�������ֽ�˳��ת��Ϊ�����ֽ�˳��
     * 
     * @param host 
     * @return UInt64 
     */
    static inline UInt64 HostToNetworkOrder(UInt64 host)
    {
        return htonll(host);
    }	
    /**
     * @brief ��ֵ�������ֽ�˳��ת��Ϊ�����ֽ�˳��
     * 
     * @param network 
     * @return UInt16 
     */
    static inline UInt16 NetworkToHostOrder(UInt16 network)
    {
        return ntohs(network);
    }
    /**
     * @brief ��ֵ�������ֽ�˳��ת��Ϊ�����ֽ�˳��
     * 
     * @param network 
     * @return UInt32 
     */
    static inline UInt32 NetworkToHostOrder(UInt32 network)
    {
        return ntohl(network);
    }
    /**
     * @brief ��ֵ�������ֽ�˳��ת��Ϊ�����ֽ�˳��
     * 
     * @param network 
     * @return UInt64 
     */
    static inline UInt64 NetworkToHostOrder(UInt64 network)
    {
        return ntohll(network);
    }
};
readonly System::Net::IPAddress System::Net::IPAddress::Any         = System::Net::IPAddress((UInt32)0);
readonly System::Net::IPAddress System::Net::IPAddress::Broadcast   = System::Net::IPAddress((UInt32)-1/*INADDR_BROADCAST*/);
readonly System::Net::IPAddress System::Net::IPAddress::Loopback    = System::Net::IPAddress((UInt32)0x0100007f/*INADDR_LOOPBACK*/);
readonly System::Net::IPAddress System::Net::IPAddress::None        = System::Net::IPAddress((UInt32)-1/*INADDR_NONE*/);
readonly System::Net::IPAddress System::Net::IPAddress::IPv6Any     = System::Net::IPAddress("::");
readonly System::Net::IPAddress System::Net::IPAddress::IPv6Loopback= System::Net::IPAddress("::1");
readonly System::Net::IPAddress System::Net::IPAddress::IPv6None    = System::Net::IPAddress("::0");
class System::Net::SocketAddress
{
public:
    /**
     * @brief ��ȡ SocketAddress �Ļ�����������С
     * 
     */
    int Size;
    union
    {
        sockaddr_in     ipv4Addr;
        sockaddr_in6    ipv6Addr;
        /**
         * @brief SocketAddress �Ļ�����������С
         * 
         */
        unsigned char   Item[sizeof(sockaddr_in6)];
    };
    SocketAddress() : Size(sizeof(sockaddr_in6)){}
    SocketAddress(AddressFamily family) : Size(AddressFamily::InterNetworkV6 == family ? sizeof(sockaddr_in6) : sizeof(sockaddr_in))
    {
        ipv6Addr.sin6_family = static_cast<ADDRESS_FAMILY>(family);
    }
    SocketAddress(const IPAddress &address, UInt16 port) : SocketAddress(address.m_eFamily)
    {
        switch (address.m_eFamily)
        {
        case AddressFamily::InterNetwork:
            {
                ipv4Addr.sin_addr = address.ipv4Addr;
            }
            break;
        case AddressFamily::InterNetworkV6:
            {
                ipv6Addr.sin6_addr = address.ipv6Addr;
            }
            break;
        }
        ipv6Addr.sin6_port = IPAddress::HostToNetworkOrder(port);
    }
    inline AddressFamily GetFamily()
    {
        return (AddressFamily)ipv6Addr.sin6_family;
    }
    inline UInt16 GetNetworkPort()
    {
        return ipv6Addr.sin6_port;
    }
    /**
     * @brief ��ȡ SocketAddress ����������ַ���ַ�����ʽ, 127.0.0.1:8080
     * 
     * @param buffer 
     */
    void ToString(char* buffer)
    {
        TCPIP_AF_ASSERT(ipv4Addr.sin_family);
        VOIDRET_ASSERT(NULL != buffer);
        void* pAddr = NULL;
        switch (GetFamily())
        {
        case AddressFamily::InterNetwork :
            {
                pAddr = &ipv4Addr.sin_addr;
            }
            break;
        case  AddressFamily::InterNetworkV6 :
            {
                pAddr = &ipv6Addr.sin6_addr;
            }
            break;
        default:
            return;
        }
        char ip[MAX_IP_LENGTH];
        SOCKETAPI_ASSERT
        (
            inet_ntop(ipv4Addr.sin_family, pAddr, ip, MAX_IP_LENGTH), 
            "ToString failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_ntop"
        );
        sprintf(buffer, "%s:%d", ip, ntohs(ipv6Addr.sin6_port));
    }
};
/**
 * @brief ��ʶ�����ַ�� ����һ��������
 * 
 */
class System::Net::EndPoint
{
protected:
    EndPoint(){}
public:
    virtual ~EndPoint(){}
    /**
     * @brief ��ȡ�ս�������ĵ�ַ��
     * 
     * @return AddressFamily 
     */
    virtual AddressFamily GetAddressFamily() = 0;
    /**
     * @brief ͨ�� SocketAddress ʵ������ EndPoint ʵ��
     * 
     * @param socketAddress 
     * @return EndPoint* 
     */
    virtual EndPoint* Create(const SocketAddress& socketAddress) = 0;
    /**
     * @brief ���ս����Ϣ���л�Ϊ SocketAddress ʵ��
     * 
     * @return SocketAddress* 
     */
    virtual SocketAddress* Serialize() = 0;
};
class System::Net::IPEndPoint final : public EndPoint
{
private:
    IPAddress   m_networkIp;
    UInt16      m_nNetworkPort;
public:
    IPEndPoint(){}
    IPEndPoint(const IPAddress& networkAddress, UInt16 networkPort) : m_networkIp(networkAddress), m_nNetworkPort(networkPort){}
    /**
     * @brief
     * 
     * @param networkAddress Internet ������ IP ��ַ�� ���磬��˸�ʽ��ֵ 0x2414188f ����Ϊ IP ��ַ��143.24.20.36��
     * @param networkPort �� address �����Ķ˿ںţ���Ϊ 0 ��ָ���κο��ö˿ڡ� port ������˳������
     */
    IPEndPoint(UInt32 networkAddress, UInt16 networkPort) : m_networkIp(IPAddress(networkAddress)), m_nNetworkPort(networkPort){}
    IPAddress& GetAddress()
    {
        return m_networkIp;
    } 
    AddressFamily GetAddressFamily() override
    {
        return m_networkIp.m_eFamily;
    }
    UInt16 GetNetworkPort()
    {
        return m_nNetworkPort;
    }
    EndPoint* Create(const SocketAddress& socketAddress) override
    {
        m_networkIp.m_eFamily   = (AddressFamily)socketAddress.ipv4Addr.sin_family;
        m_nNetworkPort          = socketAddress.ipv4Addr.sin_port;
        switch (m_networkIp.m_eFamily)
        {
        case AddressFamily::InterNetwork:
            {
                m_networkIp.ipv4Addr = socketAddress.ipv4Addr.sin_addr;
            }
            break;
        case AddressFamily::InterNetworkV6:
            {
                m_networkIp.ipv6Addr = socketAddress.ipv6Addr.sin6_addr;
            }
            break;
        default:
            break;
        }
        return this;
    }
    SocketAddress* Serialize() override
    {
        SocketAddress* p = new SocketAddress(m_networkIp.m_eFamily);
        p->ipv6Addr.sin6_port = m_nNetworkPort;
        switch (m_networkIp.m_eFamily)
        {
        case AddressFamily::InterNetwork:
            {
                p->ipv4Addr.sin_addr = m_networkIp.ipv4Addr;
            }
            break;
        case AddressFamily::InterNetworkV6:
            {
                p->ipv6Addr.sin6_addr = m_networkIp.ipv6Addr;
            }
            break;
        default:
            break;
        }
        return p;
    }
    bool operator==(IPEndPoint& other)
    {
        return (other.m_nNetworkPort == m_nNetworkPort) && (other.m_networkIp == m_networkIp);
    }
    static readonly int MinPort;
    static readonly int MaxPort;
};
readonly int System::Net::IPEndPoint::MinPort = 0;
readonly int System::Net::IPEndPoint::MaxPort = 65535;

#ifdef __WINDOWS
#   pragma warning(default:4996)
#endif
#endif