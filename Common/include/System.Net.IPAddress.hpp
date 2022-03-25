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
 *  主机字节
 *  |
 *  |--->HostToNetworkOrder ]
 *  |<---NetworkToHostOrder ] 网络字节(大端)
 *  |       ?           |
 *  |       |           |
 *  |       |           ↓
 *  |       inet_pton   inet_ntop
 *  |--->inet_ntoa              ]
 *  |<---inet_aton / inet_addr  ]Ip 字符串
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
     * @brief 基于socket的框架上发展出一种IPC机制，就是UNIX Domain Socket。
     * 虽然网络socket也可用于同一台主机的进程间通讯（通过loopback地址127.0.0.1），但是UNIX Domain Socket用于IPC 更有效率 ：
     * 不需要经过网络协议栈
     * 不需要打包拆包、计算校验和、维护序号和应答等，可靠性更强
     * 
     * 只是将应用层数据从一个进程拷贝到另一个进程。
     * 这是因为，IPC机制本质上是可靠的通讯，而网络协议是为不可靠的通讯设计的。
     * UNIX Domain Socket也提供面向流和面向数据包两种API接口，
     * 类似于TCP和UDP，但是面向消息的UNIX Domain Socket也是可靠的，消息既不会丢失也不会顺序错乱。
     * 
     */
    Unix            = AF_UNIX,
    /**
     * @brief IP 版本 4 的地址
     * 
     */
    InterNetwork    = AF_INET,
    /**
     * @brief IP 版本 6 的地址
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
     * @param strIp InterNetwork / InterNetworkV6 字符表达形式
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
     * @param nNetworkIp InterNetwork 网络字节序 127 0 0 1 -> 0x0100007f
     */
    IPAddress(UInt32 nNetworkIp)
    {
        m_eFamily = AddressFamily::InterNetwork;
        ipv4Addr.S_un.S_addr = nNetworkIp;
    }
    /**
     * @brief IPAddress 转字符串
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
     * @brief 指示服务器必须侦听所有网络接口上的客户端活动 0.0.0.0
     * 
     */
    static readonly IPAddress Any;
    /**
     * @brief  IP 广播地址 255.255.255.255
     * 
     */
    static readonly IPAddress Broadcast;
    /**
     * @brief 环回地址 127.0.0.1
     * 
     */
    static readonly IPAddress Loopback;
    /**
     * @brief 提供指示不应使用任何网络接口的 IP 地址 255.255.255.255
     * 
     */
    static readonly IPAddress None;
    /**
     * 
     * @brief Bind(EndPoint) 方法使用 IPv6Any 字段指示 Socket 必须侦听所有网络接口上的客户端活动 0:0:0:0:0:0:0;在压缩表示法中，字段等效于 ::
     * 
     */
    static readonly IPAddress IPv6Any;
    /**
     * @brief  环回地址,0:0:0:0:0:0:0:1，或等于::1，采用精简表示法
     * 
     */
    static readonly IPAddress IPv6Loopback;
    /**
     * @brief 提供指示不应使用任何网络接口的 IP 地址 0:0:0:0:0:0:0:0 或 ::0采用精简表示法
     * 
     */
    static readonly IPAddress IPv6None;
    /**
     * @brief 判断字符串是 InterNetwork 活 InterNetworkV6 格式
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
     * @brief 将值由主机字节(大端 or 小端 由 cpu / 操作系统 决定)顺序转换为网络字节(大端)顺序。
     * 
     * @param host 
     * @return UInt16 
     */
    static inline UInt16 HostToNetworkOrder(UInt16 host)
    {
        return htons(host);
    }	
    /**
     * @brief 将值由主机字节顺序转换为网络字节顺序。
     * 
     * @param host 
     * @return UInt32 
     */
    static inline UInt32 HostToNetworkOrder(UInt32 host)
    {
        return htonl(host);
    }	
    /**
     * @brief 将值由主机字节顺序转换为网络字节顺序。
     * 
     * @param host 
     * @return UInt64 
     */
    static inline UInt64 HostToNetworkOrder(UInt64 host)
    {
        return htonll(host);
    }	
    /**
     * @brief 将值由网络字节顺序转换为主机字节顺序
     * 
     * @param network 
     * @return UInt16 
     */
    static inline UInt16 NetworkToHostOrder(UInt16 network)
    {
        return ntohs(network);
    }
    /**
     * @brief 将值由网络字节顺序转换为主机字节顺序
     * 
     * @param network 
     * @return UInt32 
     */
    static inline UInt32 NetworkToHostOrder(UInt32 network)
    {
        return ntohl(network);
    }
    /**
     * @brief 将值由网络字节顺序转换为主机字节顺序
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
     * @brief 获取 SocketAddress 的基础缓冲区大小
     * 
     */
    int Size;
    union
    {
        sockaddr_in     ipv4Addr;
        sockaddr_in6    ipv6Addr;
        /**
         * @brief SocketAddress 的基础缓冲区大小
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
     * @brief 获取 SocketAddress 对象的网络地址的字符串形式, 127.0.0.1:8080
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
 * @brief 标识网络地址。 这是一个抽象类
 * 
 */
class System::Net::EndPoint
{
protected:
    EndPoint(){}
public:
    virtual ~EndPoint(){}
    /**
     * @brief 获取终结点所属的地址族
     * 
     * @return AddressFamily 
     */
    virtual AddressFamily GetAddressFamily() = 0;
    /**
     * @brief 通过 SocketAddress 实例创建 EndPoint 实例
     * 
     * @param socketAddress 
     * @return EndPoint* 
     */
    virtual EndPoint* Create(const SocketAddress& socketAddress) = 0;
    /**
     * @brief 将终结点信息序列化为 SocketAddress 实例
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
     * @param networkAddress Internet 主机的 IP 地址。 例如，大端格式的值 0x2414188f 可能为 IP 地址“143.24.20.36”
     * @param networkPort 与 address 关联的端口号，或为 0 以指定任何可用端口。 port 以主机顺序排列
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