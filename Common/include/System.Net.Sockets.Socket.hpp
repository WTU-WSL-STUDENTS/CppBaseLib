/*
 * @Description: Tcp,Udp�����ӿ�
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-06 19:31:39
 * @LastEditors: like
 * @LastEditTime: 2022-03-25 16:59:19
 */
#ifndef SYSTEM_NET_SOCKETS_SOCKET_HPP
#define SYSTEM_NET_SOCKETS_SOCKET_HPP
#include <System.Net.IPAddress.hpp>
#include <CompliedIndexer.h>

/**
 * @brief Ĭ������� Socket ���������������̰󶨡�
 * ���Ҫ���øù���, �����ֶ���ʼ�� & �ͷ�ָ���汾�� Socket ��, ���ڴ�֮ǰ���� #define BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS 0
 * 
 */
#ifndef BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS
#   define BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS 1
#endif

namespace System::Net::Sockets
{
    using LingerOption   = linger;
    using ProtocolType  = IPPROTO;
    using SocketError   = int;
    enum IOControlCode;
    enum SelectMode;
    enum SocketFlags; 
    enum class SocketOptionLevel;
    enum SocketOptionName;
    enum SocketShutdown;
    enum SocketType;
    class Socket;
    // class SocketAsyncEventArgs;
};
enum System::Net::Sockets::IOControlCode /* https://docs.microsoft.com/zh-cn/dotnet/api/system.net.sockets.iocontrolcode?view=net-5.0 */
{
    /**
     * @brief ���ȴ���������ʱ����֪ͨ
     * 
     */
    AsyncIO         =  FIOASYNC,
    /**
     * @brief �����׽��ֵ�����( blocking mode )��Ϊ�� ���ʹ�ô˿��ƴ���ָ���Ĳ���Ϊ 0���׽�����������ģʽ�¡� ���������Ϊ�㣬�׽������ڷ�����ģʽ��
     * 
     */
    NonBlockingIO   = FIONBIO,
    /**
     * @brief ���ؿɶ�ȡ���ֽ���
     * 
     */
    DataToRead      = FIONREAD,
};
/**
 * @brief Ϊ Poll(Int32, SelectMode) ����������ѯģʽ
 * 
 */
enum System::Net::Sockets::SelectMode
{
    /**
     * @brief ��״̬ģʽ
     * 
     */
    SelectRead = 0,
    /**
     * @brief д״̬ģʽ
     * 
     */
    SelectWrite,
    /**
     * @brief ����״̬ģʽ
     * 
     */
    SelectError
};
/**
 * @brief ָ���׽��ַ��ͺͽ�����Ϊ����ö��֧�����Աֵ�İ�λ���
 * 
 */
enum System::Net::Sockets::SocketFlags
{
    /**
     * @brief ���Դ˵���ʹ���κα�־
     * 
     */
    None                = 0,
    /**
     * @brief �����������
     * 
     */
    OutOfBand           = MSG_OOB,
    /**
     * @brief ���ٲ鿴������Ϣ
     * 
     */
    Peek                = MSG_PEEK,
    /**
     * @brief ��ʹ��·�ɱ���з���
     * 
     */
    MsgDontRoute           = MSG_DONTROUTE,
    /**
     * @brief not support
     * 
     */
    WaitAll             = MSG_WAITALL,
    /**
     * @brief ��Ϣ̫���޷�����ָ���Ļ������������ѱ��ض�
     * 
     */
    Truncated           = DECLARE_ENUM_FLAG(8),
    /**
     * @brief ָʾ���������޷����� 64 KB ���ڲ����������ѱ��ض�
     * 
     */
    ControlDataTruncated= DECLARE_ENUM_FLAG(9),
    /**
     * @brief ָʾ�㲥���ݰ�
     * 
     */
    MsgBroadcast           = DECLARE_ENUM_FLAG(10),
    /**
     * @brief ָʾ�ಥ���ݰ�
     * 
     */
    Multicast           = DECLARE_ENUM_FLAG(11),
    /**
     * @brief ���ַ��ͻ������Ϣ
     * 
     */
    Partial             = DECLARE_ENUM_FLAG(15)
};
/**
 * @brief Ϊ SetSocketOption / GetSocketOption ���������׽���ѡ���
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
 * @brief ��������ѡ������
 * 
 */
enum System::Net::Sockets::SocketOptionName
{
    /**
     * @brief ʹ�׽����ܹ�Ϊ��ռ���ʽ��а� 
     * https://docs.microsoft.com/en-us/windows/win32/winsock/so-exclusiveaddruse
     * A socket with SO_EXCLUSIVEADDRUSE set cannot always be reused immediately after socket closure. 
     * For example, if a listening socket with the exclusive flag set accepts a connection after which the listening socket is closed, 
     * another socket cannot bind to the same port as the first listening socket with the exclusive flag 
     * until the accepted connection is no longer active.
     */
    ExclusiveAddressUse = SO_EXCLUSIVEADDRUSE,
    /**
     * @brief ��¼������Ϣ
     * 
     */
    Debug               = SO_DEBUG,
    /**
     * @brief �׽�����������
     * 
     */
    AcceptConnection    = SO_ACCEPTCONN,
    /**
     * @brief �����׽��ְ󶨵�����ʹ���еĵ�ַ
     * 
     */
    ReuseAddress        = SO_REUSEADDR,
    /**
     * @brief ʹ�� keep-alive
     * keepaliveԭ��:TCP��Ƕ��������,�Է����Ϊ��,��server��⵽����һ��ʱ��(/proc/sys/net/ipv4/tcp_keepalive_time 7200 ��2Сʱ)û�����ݴ���,��ô����client�˷���һ��keepalive packet,��ʱclient�������ַ�Ӧ:
     * 1��client����������,����һ��ACK.server���յ�ACK�����ü�ʱ��,��2Сʱ���ڷ���̽��.���2Сʱ�������������ݴ���,��ô�ڸ�ʱ��Ļ������������2Сʱ����̽���;
     * 2���ͻ����쳣�ر�,������Ͽ���client����Ӧ,server�ղ���ACK,��һ��ʱ��(/proc/sys/net/ipv4/tcp_keepalive_intvl 75 ��75��)���ط�keepalive packet, �����ط�һ������(/proc/sys/net/ipv4/tcp_keepalive_probes 9 ��9��);
     * 3���ͻ�����������,���Ѿ�����.server�յ���̽����Ӧ��һ����λ,server����ֹ���ӡ�
     */
    KeepAlive           = SO_KEEPALIVE,
    /**
     * @brief ��·�ɣ������ݰ�ֱ�ӷ��͵��ӿڵ�ַ
     * 
     */
    DontRoute           = SO_DONTROUTE,
    /**
     * @brief �������׽����Ϸ��͹㲥��Ϣ
     * 
     */
    Broadcast           = SO_BROADCAST,
    /**
     * @brief ����ʱ�ܿ�Ӳ��
     * 
     */
    UseLoopback         = SO_USELOOPBACK,
    /**
     * @brief �������δ���͵����ݣ����ڹر�ʱ����
     * 
     */
    Linger              = SO_LINGER,
    /**
     * @brief ���������������еĴ�������
     * 
     */
    OutOfBandInline     = SO_OOBINLINE,
    /**
     * @brief ָ��Ϊ���ͱ�����ÿ���׽��ֻ������ռ�������� ���������Ϣ��С�� TCP ���ڵĴ�С�޹�
     * 
     */
    SendBuffer          = SO_SNDBUF,
    /**
     * @brief ָ��Ϊ���ձ�����ÿ���׽��ֻ������ռ�������� ���������Ϣ��С�� TCP ���ڵĴ�С�޹�
     * 
     */
    ReceiveBuffer       = SO_RCVBUF,
    /**
     * @brief �ں�ִ�� Send ���� Buffer ���ȵ���ֵ
     * 
     */
    SendLowWater        = SO_SNDLOWAT,   
    /**
     * @brief �ں�ִ�� Recv ���� Buffer ���ȵ���ֵ
     * 
     */
    ReceiveLowWater     = SO_RCVLOWAT,
    /**
     * @brief ���ͳ�ʱ����ѡ���������ͬ������;�����첽������������
     * 
     */
    SendTimeout         = SO_SNDTIMEO,
    /**
     * @brief ���ͳ�ʱ����ѡ���������ͬ������;�����첽������������
     * 
     */
    ReceiveTimeout      = SO_RCVTIMEO,
    /**
     * @brief ��ȡ����״̬�����
     * 
     */
    Error               = SO_ERROR,
    /**
     * @brief ��ȡ�׽�������
     * 
     */
    Type                = SO_TYPE,

    /************************************* 
     * IP Setting
     * https://docs.microsoft.com/en-us/windows/win32/winsock/ipproto-ip-socket-options
     **************************************/
   
    /**
     * @brief ��ȡ���������ݰ�������ʱ��(Time To Live)
     * 
     */
    TTL                 = IP_TTL,
    /**
     * @brief IP ��·�㲥���ء�
     * ��windowsƽ̨, IP_MULTICAST_LOOP Ӧ�õ����ն�.�ڽ��ն�����IP_MULTICAST_LOOP. loop����Ϊ1����ʾ���������ͳ�ȥ�����ݣ�����Ϊ0��ʾ������
     * ��Linuxƽ̨, IP_MULTICAST_LOOP Ӧ�õ����Ͷ�
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
     * @brief �����鲥
     * 
     */
    AddMembership       = IP_ADD_MEMBERSHIP,
    /**
     * @brief �˳��鲥
     * 
     */
    DropMembership      = IP_DROP_MEMBERSHIP,
    /**
     * @brief �����Ƿ�ְ�
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
     * @brief ��ֵָ���Ƿ� Socket ������ IPv4 �� IPv6 ��˫ģʽ�׽��֣� 0 ����˫ģʽ�׽���( Ĭ������ )
     * 
     */
    Ipv6Only            =  IPV6_V6ONLY
};
/**
 * @brief ���� Shutdown ����ʹ�õĳ���
 * 
 */
enum System::Net::Sockets::SocketShutdown
{
    Receive = 0,
    Send,
    Both
};
/**
 * @brief ָ�� Socket ���ʵ����ʾ���׽�������
 * 
 */
enum System::Net::Sockets::SocketType
{
    Unknown = -1,
    /**
     * @brief ֧�ֿɿ���˫�򡢻������ӵ��ֽ����������ظ����ݣ�Ҳ�������߽硣 
     * �����͵� Socket �뵥���Է�����ͨ�ţ�������ͨ�ſ�ʼ֮ǰ��Ҫ����Զ���������ӡ� 
     * Stream ʹ�ô������Э�� (ProtocolType.Tcp) �� AddressFamily.InterNetwork ��ַ��
     * 
     */
    Stream  = SOCK_STREAM,
    /**
     * @brief ֧�����ݱ�������󳤶ȹ̶���ͨ����С���������ӡ����ɿ���Ϣ�� 
     * ��Ϣ���ܻᶪʧ���ظ��������ڵ���ʱ����˳�����С� 
     * Socket ���͵� Dgram �ڷ��ͺͽ�������֮ǰ����Ҫ�κ����ӣ����ҿ��������Է���������ͨ�š� 
     * Dgram ʹ�����ݱ�Э�� (ProtocolType.Udp) �� AddressFamily.InterNetwork ��ַ��
     * 
     */
    Dgram   = SOCK_DGRAM,
    Raw,
    Rdm,
    Seqpacket
};
class System::Net::Sockets::Socket
{
#if BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS
    class SocketlibraryInitlializer
    {
    public:
        SocketlibraryInitlializer()
        {
            WSADATA wsaData;
            ERROR_ASSERT(NO_ERROR == WSAStartup(MAKEWORD(2, 2), &wsaData), "SocketlibraryInitlializer init Socket 2.2 failed");
        }
        ~SocketlibraryInitlializer()
        {
            WSACleanup();
        }
    };
    /**
     * @brief Socket ����Դ��ʼ������, ������������̰󶨡�
     * 
     */
    static readonly SocketlibraryInitlializer m_socketlibraryInitlializer;
#endif
protected:
    AddressFamily   m_addressFamily;
    SocketType      m_sockType;
    ProtocolType    m_protoType;
    
    bool            m_bIsBlocking;

    SOCKET          m_socket;
    Socket(){}
public:
    Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) : 
        m_addressFamily(addressFamily), m_sockType(socketType), m_protoType(protocolType), 
        m_bIsBlocking(true) /* Ĭ��Ϊ����ģʽ */ 
    {
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET != (m_socket = socket((int)addressFamily, socketType, protocolType)), 
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket"
        );
    }
    /**
     * @brief m_socket ���� Close() ������Դ�ͷ�, ͨ�� Accept ������ Socket ������Ҫ���� Close()
     * 
     */
    virtual ~Socket(){}
    /**
     * @brief ��ȡ Socket �Ĳ���ϵͳ���
     * 
     */
    DECLARE_CONST_GETTER(SOCKET, Handle, 
    {
        return m_socket;
    })
    /**
     * @brief ��ȡ Socket �ĵ�ַ��
     * 
     */
    DECLARE_CONST_GETTER(AddressFamily, AddressFamily,
    {
        return m_addressFamily;
    })
    /**
     * @brief ��ȡ Socket ������
     * 
     */
    DECLARE_CONST_GETTER(SocketType, SocketType,
    {
        return m_sockType;
    })
    /**
     * @brief ��ȡ�Ѿ�����������ҿɹ���ȡ��������
     * 
     */
    DECLARE_CONST_GETTER(DWORD, Available,
    {
        DWORD nLen;
        IOControl(IOControlCode::DataToRead, nLen);
        return nLen;
    })
    /**
     * @brief Socket �Ƿ�������ģʽ
     * 
     * @return true 
     * @return false 
     */
    DECLARE_INDEXER(bool, Blocking, 
    {
        return m_bIsBlocking;
    },
    {
        VOIDRET_ASSERT(SETTER_VALUE != m_bIsBlocking);
        DWORD nArg = m_bIsBlocking;
        if(0 == IOControl(IOControlCode::NonBlockingIO, nArg))
        {
            m_bIsBlocking = SETTER_VALUE;
        }
    })
    /**
     * @brief ��ѯ��ǰ socket �Ƿ��� TCP ����״̬
     * �ж���ǰ��Ȼ��������״̬�ķ��� :
     *  �Է�����ģʽ���� 0 �ֽ�����, ���ͳɹ�����ʧ�ܵ� ErrorCode Ϊ WAEWOULDBLOCK 
     * 
     */
    DECLARE_CONST_GETTER(bool, Connected,
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        if(!m_bIsBlocking)
        {                
            return SOCKET_ERROR != send(m_socket, "", 0, SocketFlags::None) || WSAEWOULDBLOCK == WSAGetLastError();
        }
        DWORD nBlockMode = 1;
        ERROR_ASSERT(0 == IOControl(IOControlCode::NonBlockingIO, nBlockMode), "GetConnected set nonblockmode failed");
        bool bIsConnect = SOCKET_ERROR != send(m_socket, "", 0, SocketFlags::None) || WSAEWOULDBLOCK == WSAGetLastError();
        nBlockMode = 0;
        ERROR_ASSERT(0 == IOControl(IOControlCode::NonBlockingIO, nBlockMode), "GetConnected recover blockmode failed");
        return bIsConnect;
    })
    /**
     * @brief UDP �Ƿ���÷ְ�
     * 
     */
    DECLARE_INDEXER(bool, DontFragment, 
    {
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IP, SocketOptionName::DontFragment, bEnable);
        return bEnable; 
    },
    {
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::DontFragment, SETTER_VALUE);
    })
    /**
     * @brief Socket �Ƿ����� IPv4 �� IPv6 ��˫ģʽ�׽���
     * 
     */
    DECLARE_INDEXER(bool, DualMode,
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addressFamily, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, bEnable);
        return bEnable;
    },
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addressFamily, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, SETTER_VALUE);    
    })
    /**
     * @brief Socket �Ƿ���Է��ͻ���չ㲥���ݰ�
     * 
     */
    DECLARE_INDEXER(bool, EnableBroadcast, 
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, bEnable);
        return bEnable;
    },
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, SETTER_VALUE);
    })
    /**
     * @brief Tcp Socket �Ƿ������һ�����̰󶨵��˿�
     * 
     */
    DECLARE_INDEXER(bool, ExclusiveAddressUse,
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, bEnable);
        return bEnable;
    },
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, SETTER_VALUE);
    })
    /**
     * @brief Socket �ڳ��Է������й�������ʱ�Ƿ��ӳٹر��׽���
     * 
     */
    DECLARE_INDEXER(LingerOption, LingerState,
    {
        LingerOption state;
        int size = sizeof(LingerOption);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&state, size);
        return state;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&SETTER_VALUE, sizeof(LingerOption));
    })
    /**
     * @brief ��ȡ Tcp ���ӶԷ��� SocketAddress
     * �ڵ�һ�� IO ���������
     *  stream   : Connect
     *  datagram : sendto / receivefrom
     */
    DECLARE_CONST_GETTER(SocketAddress, LocalEndPoint,
    {
        SocketAddress addr;
        SOCKETAPI_ASSERT(0 == getpeername(m_socket, (struct sockaddr *)addr.Item, (socklen_t*)&addr.Size), "Get local end point failed");
        return addr;
    })
    /**
     * @brief �����Ķ�·�㲥���ݰ��Ƿ񴫵ݵ�����Ӧ�ó���
     * 
     */
    DECLARE_INDEXER(bool, MulticastLoopback,
    {
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IP, SocketOptionName::MulticastLoopback, bEnable);
        return bEnable; 
    },
    {
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::MulticastLoopback, SETTER_VALUE);
    })
    /**
     * @brief ָ�� Tcp Socket �Ƿ�����ʹ�� Nagle �㷨��Ĭ�Ͽ�������
     * Small Packet Problem
     * ��ʹ��һЩЭ��ͨѶ��ʱ�򣬱���Telnet������һ���ֽ��ֽڵķ��͵��龰��
     * ÿ�η���һ���ֽڵ��������ݣ��ͻ����41���ֽڳ��ķ��飬
     * 20���ֽڵ�IP Header �� 20���ֽڵ�TCP Header��
     * ��͵�����1���ֽڵ�������ϢҪ�˷ѵ�40���ֽڵ�ͷ����Ϣ��
     * ����һ�ʾ޴���ֽڿ�������������Small packet�ڹ������ϻ�����ӵ���ĳ��֡�
     * ��ν���������⣿ 
     * Nagle�������һ��ͨ��������Ҫͨ�����緢�Ͱ������������TCP/IP�����Ч�ʣ������Nagle�㷨
     * 
     */
    DECLARE_INDEXER(bool, NoDelay,
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, bEnable);
        return bEnable;
    },
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, SETTER_VALUE);
    })
    /**
     * @brief Socket ���ջ������Ĵ�С��Ĭ��ֵΪ 8192
     * 
     */
    DECLARE_INDEXER(int, ReceiveBufferSize,
    {
        int nSize = 8192;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, nSize);
        return nSize;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, SETTER_VALUE);
    })
    /**
     * @brief ��ʱֵ���Ժ���Ϊ��λ���� Ĭ��ֵΪ 0��ָʾ��ʱ�������޴� ָ�� -1 ����ָʾ��ʱ�������޴�
     * 
     */
    DECLARE_INDEXER(int, ReceiveTimeout,
    {
        int nTimeout = -1;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, nTimeout);
        return nTimeout;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, SETTER_VALUE);
    })
    /**
     * @brief ��ȡ������һ��ֵ����ָ�� Socket ���ͻ������Ĵ�С��Ĭ��ֵΪ 8192
     * 
     */
    DECLARE_INDEXER(int, SendBufferSize,
    {
        int nSize = 8192;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, nSize);
        return nSize;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, SETTER_VALUE);
    })
    /**
     * @brief ��ʱֵ���Ժ���Ϊ��λ���� Ĭ��ֵΪ 0��ָʾ��ʱ�������޴� ָ�� -1 ����ָʾ��ʱ�������޴�
     * 
     */
    DECLARE_INDEXER(int, SendTimeout,
    {
        int nTimeout = -1;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, nTimeout);
        return nTimeout;
    },
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, SETTER_VALUE);
    })
    /**
     * @brief ָ�� Socket ���͵� Internet Э�� (IP) ���ݰ�������ʱ�� (TTL) ֵ
     * 
     */
    DECLARE_INDEXER(byte, Ttl,
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        byte val;
        int size = sizeof(byte);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::TTL, (char*)&val, size);
        return val;
    },
    {
        TCPIP_AF_ASSERT(m_addressFamily);
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::TTL, (char*)&SETTER_VALUE, sizeof(byte));
    })
    /**
     * @brief Ϊ�½����Ӵ����µ� Socket
     * �������׽��ֵ��������������ͬ����ȡ��һ���������������Ȼ�󴴽��������µ� Socket �� 
     * ����ʹ�÷��صĴ� Socket �������������Ӷ��е������κ����ӡ� 
     * ���ǣ����Ե��÷�����ʶԶ������ Socket �������ַ�Ͷ˿ں�
     * 
     * @param remoteEP ���ؿͻ��˵������ַ��Ϣ
     * @return Socket* ���ؿͻ��˵���Դ���
     */
    Socket* Accept(SocketAddress &remoteEP)
    {
        Socket* s = new Socket();
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET  != (s->m_socket = accept(m_socket, (SOCKADDR *)remoteEP.Item, &remoteEP.Size)),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept"
        );
        return s;
    }
    /**
     * @brief ʹ Socket ��һ�����ض˿������
     * 
     * @param localEP 
     */
    void Bind(SocketAddress &localEP)
    { 
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != bind(m_socket, (SOCKADDR *)localEP.Item, localEP.Size),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-bind"
        );
    }
    /**
     * @brief �ر� Socket ���Ӳ��ͷ����й�������Դ
     * �����������ӵ�Э�飬������ Shutdown ���÷���֮ǰ���� Close �� ���ȷ���������������ӵ��׽��ֹر�֮ǰ���ѷ��ͺͽ��ա�
     * 
     * �����Ҫ Close �ȵ��ö������� Shutdown ��
     * ����ͨ����ѡ������ DontLinger Socket Ϊ false ��ָ�����㳬ʱ�����ȷ�����Ŷӵȴ�������������ݷ��͡� 
     * Close �ᱻ������ֱ�����ʹ����ݻ�ָ���ĳ�ʱ����
     * 
     */
    void Close()
    {
        VOIDRET_ASSERT(INVALID_SOCKET != m_socket);
        SOCKETAPI_ASSERT(SOCKET_ERROR != closesocket(m_socket), "Dispose Socket failed, create socket failed");
        m_socket = INVALID_SOCKET;
    }
    /**
     * @brief ��Զ��������������
     * 
     * @param reuseSocket 
     */
    void Connect(SocketAddress &remoteEP)
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != connect(m_socket, (SOCKADDR *)remoteEP.Item, remoteEP.Size),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-connect"
        );
    }
    /**
     * @brief �� Socket ��������״̬
     * 
     * @param backlog �ͻ������������, ���һ���Ѿ����ڼ���״̬�� Socket ���øýӿ�, backlog �����޸ġ�
     */
    void Listen(int backlog = SOMAXCONN) 
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != listen(m_socket, backlog), 
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-listen"
        );
    }
    /**
     * @brief ȷ�� Socket ��״̬
     * 
     * @param microSeconds �ȴ���Ӧ��ʱ�䣨��΢��Ϊ��λ���� 
     * @param mode 
     * SelectRead
     *  ����ѵ��� Listen(Int32) �����й��������
     *  ��������ݿɹ���ȡ
     *  ��������ѹرա����û���ֹ
     * 
     * SelectWrite
     *  ������ڴ��� Connect(EndPoint) ���������ѳɹ�
     *  ������Է�������
     * 
     * SelectError
     *  ������ڴ����첽 Connect(EndPoint)������������ʧ��
     *  ��� OutOfBandInline δ���ã����Ҵ������ݿ���
     * 
     * @return true ��ָ�� mode �����³����������, ���� true
     * @return false 
     */
    bool Poll(int microSeconds, SelectMode mode) 
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(m_socket, &fds);
        fd_set* pfds[SelectMode::SelectError + 1] = {NULL};
        pfds[mode] = &fds;
        return Select(pfds[SelectMode::SelectRead], pfds[SelectMode::SelectWrite], pfds[SelectMode::SelectError], microSeconds);
    }
    /**
     * @brief �������԰󶨵� Socket ������
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags 
     * @param errorcode 
     * @return int ʵ�ʽ��յĳ���, ���ᳬ�� ReceiveBuffer �������� If the connection has been gracefully closed, the return value is zero.
     */
    int Receive(char* buffer, int size, SocketFlags socketFlags, SocketError &errorcode) 
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-recv */
        if(SOCKET_ERROR == (nRet = recv(m_socket, buffer, size, socketFlags)))
        {
            errorcode = WSAGetLastError();
        }
        return nRet;   
    }
    /**
     * @brief �������ݱ����洢Դ�ս��
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags OutOfBand / Peek 
     * @param remoteEP 
     * @return int ʵ�ʽ��յĳ���, ���ᳬ�� ReceiveBuffer �������� If the connection has been gracefully closed, the return value is zero.
     */
    int ReceiveFrom(char* buffer, int size, SocketFlags socketFlags, SocketAddress &remoteEP) 
    {
        int nRet;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != (nRet = recvfrom(m_socket, buffer, size, socketFlags, (SOCKADDR *)remoteEP.Item, &remoteEP.Size)), 
            "ReceiveMessageFrom failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-recvfrom"
        );
        return nRet;
    }
    /**
     * @brief �����ݷ��͵����ӵ� Socket
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags OutOfBand / DontRoute 
     * @param errorcode 
     * @return int ʵ�ʷ��͵ĳ���, ������ݳ��ȳ����� SendBuffer ��ʣ������, ����нض� 
     */
    int Send( char* buffer, int size, SocketFlags socketFlags, SocketError &errorcode) 
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send */
        if(SOCKET_ERROR == (nRet = send(m_socket, buffer, size, socketFlags)))
        {
            errorcode = WSAGetLastError();
        }
        return nRet;
    }
    /**
     * @brief ʹ��ָ���� SocketFlags����ָ���ֽ��������ݷ��͵�ָ�����ս��
     * 
     * @param buffer 
     * @param size 
     * @param socketFlags OutOfBand / DontRoute 
     * @param remoteEP 
     * @return int ʵ�ʷ��͵ĳ���, ������ݳ��ȳ����� SendBuffer ��ʣ������, ����нض� 
     */
    int SendTo(const char* buffer, int size, SocketFlags socketFlags, const SocketAddress &remoteEP) 
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-sendto */
        SOCKETAPI_ASSERT(SOCKET_ERROR != (nRet = sendto(m_socket, buffer, size, socketFlags, (SOCKADDR *)remoteEP.Item, remoteEP.Size)), "SendTo failed");
        return nRet;
    }
    /**
     * @brief Ϊ Socket ���õͼ������ģʽ
     * 
     * @param ioControlCode 
     * @param optionInOutValue ���� / �������
     * @return int ���� 0 ����ɹ�
     */
    inline int IOControl(IOControlCode ioControlCode, DWORD &optionInOutValue) const
    {
        int nRet;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != (nRet = ioctlsocket(m_socket, (long)ioControlCode, &optionInOutValue)),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-ioctlsocket"
        );
        return nRet; 
    }
    /**
     * @brief ���� Socket ѡ���ֵ
     * 
     * @param optionLevel 
     * @param optionName 
     * @param buffer 
     * @param optlen 
     */
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, char* buffer, int &optlen) const
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != getsockopt(m_socket, (int)optionLevel, optionName, buffer, &optlen), 
            "SetSocketOption failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-getsockopt"
        );
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, bool &optionValue) const
    {
        int optlen = sizeof(bool);
        GetSocketOption(optionLevel, optionName, (char *)&optionValue, optlen);
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, int &optionValue) const
    {
        int optlen = sizeof(int);
        GetSocketOption(optionLevel, optionName, (char *)&optionValue, optlen);
        ERROR_ASSERT(sizeof(int) == optlen, "GetSocketOption failed"); 
    }
    /**
     * @brief ��ָ���� Socket ѡ������Ϊָ��ֵ
     * 
     * @param optionLevel 
     * @param optionName 
     * @param optionValue 
     * @param optlen 
     */
    inline void SetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, const char* optionValue, int optlen)
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != setsockopt(m_socket, (int)optionLevel, optionName, optionValue, optlen), 
            "SetSocketOption failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-setsockopt"
        );
    }
    inline void SetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, bool optionValue)
    {
        SetSocketOption(optionLevel, optionName, (char *)&optionValue, sizeof(bool));
    }
    inline void SetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, int optionValue)
    {
        SetSocketOption(optionLevel, optionName, (char *)&optionValue, sizeof(int));
    }
    /**
     * @brief ���� Socket �ϵķ��ͺͽ��գ� �������δ�շ�������, �����̷������� �����Ҫ������ͨ������ Linger ������رն���
     * 
     * @param how 
     */
    inline void Shutdown(SocketShutdown how)
    {
        SOCKETAPI_ASSERT(SOCKET_ERROR != shutdown(m_socket, how), "SocketShutdown failed");
    }
    /**
     * @brief ȷ��һ�������׽��ֵ�״̬, readfds / writefds / errorexceptfds �׽��ּ��ϵ�ָ��ֻ����һ���ǿ�
     * 
     * @param readfds 
     * If listen has been called and a connection is pending, accept will succeed.
     * Data is available for reading (includes OOB data if SO_OOBINLINE is enabled).
     * Connection has been closed/reset/terminated.
     * 
     * @param writefds 
     * If processing a connect call (nonblocking), connection has succeeded.
     * Data can be sent.
     * 
     * @param errorexceptfds 
     * If processing a connect call (nonblocking), connection attempt failed.
     * OOB data is available for reading (only if SO_OOBINLINE is disabled).
     * 
     * @param microSeconds �ȴ���Ӧ��ʱ�䣨��΢��Ϊ��λ���� -1 ��������
     * @return int Socket ��Ӧ����
     */
    static int Select(fd_set* readfds, fd_set* writefds, fd_set* errorexceptfds, int microSeconds)
    {
        timeval timeout = {0, microSeconds};
        int nReadyCount;
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != (nReadyCount = select(-1, readfds, writefds, errorexceptfds, -1 == microSeconds ? NULL : &timeout)), 
            "Select failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-select"
        );
        return nReadyCount;
    }
    static inline bool IsSelectted(const Socket& socket, fd_set* fds)
    {
        return __WSAFDIsSet(socket.m_socket, fds);
    }
    static void InitFds(const Socket* socketList, UInt32 socketListLen, fd_set& rwefds)
    {
        ERROR_ASSERT((FD_SETSIZE + 1) > socketListLen, "Socket count out of range , please use #define FD_SETSIZE set a lager size");
        rwefds.fd_count = socketListLen;
        for(UInt32 i = 0; i < socketListLen; i++)
        {
            rwefds.fd_array[i] = socketList[i].m_socket;
        } 
    }
};
#if BIND_SOCKET_LIB_LIFETIME_WITH_PROCESS
    readonly System::Net::Sockets::Socket::SocketlibraryInitlializer System::Net::Sockets::Socket::m_socketlibraryInitlializer;
#endif

#endif