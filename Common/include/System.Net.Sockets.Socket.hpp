/*
 * @Description: Tcp,Udp�����ӿ�
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-06 19:31:39
 * @LastEditors: like
 * @LastEditTime: 2022-03-18 18:25:43
 */
#ifndef SYSTEM_NET_SOCKETS_SOCKET_HPP
#define SYSTEM_NET_SOCKETS_SOCKET_HPP
#include <System.Net.IPAddress.hpp>
// #include <MSWSock.h>

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
    IP      = IPPROTO_IP,
    Tcp     = IPPROTO_TCP,
    Udp     = IPPROTO_UDP,
    IPv6    = IPPROTO_IPV6,
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
private:
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

private:
    SocketType      m_sockType;
    ProtocolType    m_protoType;
    
    bool            m_bIsBlocking;
    bool            m_bUdpFragmentEnable;

    SOCKET          m_socket;
    SocketAddress   m_addr;
    Socket(){}
public:
    Socket(AddressFamily addressFamily, SocketType socketType, ProtocolType protocolType) : 
        m_sockType(socketType), m_protoType(protocolType), 
        m_bIsBlocking(true),        /* Ĭ��Ϊ����ģʽ */ 
        m_bUdpFragmentEnable(false) /* Udp Ĭ�Ͻ��÷ְ� */
    {
        m_addr.ipv4Addr.sin_family = (UInt16)addressFamily;
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET != (m_socket = socket((int)addressFamily, socketType, protocolType)), 
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-socket"
        );
    }
    ~Socket()
    {
        SOCKETAPI_ASSERT(SOCKET_ERROR != closesocket(m_socket), "Dispose Socket failed, create socket failed");
    }
    /**
     * @brief ��ȡ Socket �Ĳ���ϵͳ���
     * 
     * @return SOCKET 
     */
    SOCKET Handle()
    {
        return m_socket;
    }
    /**
     * @brief ��ȡ Socket �ĵ�ַ��
     * 
     * @return AddressFamily 
     */
    inline AddressFamily AddressFamily()
    {
        return m_addr.GetFamily();
    }
    /**
     * @brief ��ȡ�Ѿ�����������ҿɹ���ȡ��������
     * 
     * @return DWORD 
     */
    DWORD Available()
    {
        DWORD nLen;
        IOControl(IOControlCode::DataToRead, nLen);
        return nLen;
    }
    /**
     * @brief Socket �Ƿ�������ģʽ
     * 
     * @return true 
     * @return false 
     */
    bool GetBlockingEnable()
    {
        return m_bIsBlocking;
    }
    void SetBlockingEnable(bool blocking)
    {
        VOIDRET_ASSERT(blocking != m_bIsBlocking);
        DWORD nArg = !blocking;
        if(0 == IOControl(IOControlCode::NonBlockingIO, nArg))
        {
            m_bIsBlocking = blocking;
        }
    }
    /**
     * @brief ��ѯ��ǰ socket �Ƿ��� TCP ����״̬
     * 
     * @return true �Է�����ģʽ���� 0 �ֽ�����, ���ͳɹ�����ʧ�ܵ� ErrorCode Ϊ WAEWOULDBLOCK, �ж���ǰ��Ȼ��������״̬
     * @return false 
     */
    bool IsConnected()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bTemp = m_bIsBlocking;
        SetBlockingEnable(false);
        if(SOCKET_ERROR == send(m_socket, "", 0, SocketFlags::None) && WSAEWOULDBLOCK != WSAGetLastError())
        {
            SetBlockingEnable(bTemp);
            return false;
        }
        SetBlockingEnable(bTemp);
        return true;
    }
    /**
     * @brief  ���� UDP �Ƿ�����ְ�
     * 
     * @return true 
     * @return false 
     */
    bool GetFragmentEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        return m_bUdpFragmentEnable;
    }
    void SetFragmentEnable(bool blocking)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        m_bUdpFragmentEnable = true;
    }
    /**
     * @brief ��ֵָ���Ƿ� Socket ������ IPv4 �� IPv6 ��˫ģʽ�׽���
     * 
     * @return true 
     * @return false 
     */
    bool GetDualModeEnable()
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addr.GetFamily(), "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, bEnable);
        return bEnable;
    }
    void SetDualModeEnable(bool bEnable)
    {
        ERROR_ASSERT(AddressFamily::InterNetworkV6 == m_addr.GetFamily(), "NotSupportedException");
        SetSocketOption(SocketOptionLevel::IPv6, SocketOptionName::Ipv6Only, bEnable);
    }
    /**
     * @brief ��ֵָ�� Socket �Ƿ���Է��ͻ���չ㲥���ݰ�
     * 
     * @return true 
     * @return false 
     */
    bool GetBroadcastEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, bEnable);
        return bEnable;
    }
    void SetBroadcastEnable(bool bEnable)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_UDP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Udp, SocketOptionName::Broadcast, bEnable);
    }
    /**
     * @brief ��ֵָ�� Tcp Socket �Ƿ������һ�����̰󶨵��˿�
     * 
     * @return true 
     * @return false 
     */
    bool GetExclusiveAddressUseEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, bEnable);
        return bEnable;
    }
    void SetExclusiveAddressUseEnable(bool bEnable)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ExclusiveAddressUse, bEnable);
    }
    LingerOption GetLingerState()
    {
        LingerOption state;
        int size = sizeof(LingerOption);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&state, size);
        return state;
    }
    void SetLingerState(LingerOption state)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::Linger, (char*)&state, sizeof(LingerOption));
    }
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
     * @return true 
     * @return false 
     */
    bool GetNoDelayEnable()
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        bool bEnable = false;
        GetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, bEnable);
        return bEnable;
    }
    void SetNoDelayEnable(bool bEnable)
    {
        ERROR_ASSERT(ProtocolType::IPPROTO_TCP == m_protoType, "NotSupportedException");
        SetSocketOption(SocketOptionLevel::Socket, (SocketOptionName)TCP_NODELAY, bEnable);
    }
    /**
     * @brief ��ȡ������һ��ֵ����ָ�� Socket ���ջ������Ĵ�С��Ĭ��ֵΪ 8192
     * 
     * @return int 
     */
    int GetReceiveBufferSize()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, size);
        return size;
    }
    void SetReceiveBufferSize(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveBuffer, size);
    }
    /**
     * @brief ��ʱֵ���Ժ���Ϊ��λ���� Ĭ��ֵΪ 0��ָʾ��ʱ�������޴� ָ�� -1 ����ָʾ��ʱ�������޴�
     * 
     * @return int 
     */
    int GetReceiveTimeout()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, size);
        return size;
    }
    void SetReceiveTimeout(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::ReceiveTimeout, size);
    }
    /**
     * @brief ��ȡ������һ��ֵ����ָ�� Socket ���ͻ������Ĵ�С��Ĭ��ֵΪ 8192
     * 
     * @return int 
     */
    int GetSendBufferSize()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, size);
        return size;
    }
    void SetSendBufferSize(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendBuffer, size);
    }
    /**
     * @brief ��ʱֵ���Ժ���Ϊ��λ���� Ĭ��ֵΪ 0��ָʾ��ʱ�������޴� ָ�� -1 ����ָʾ��ʱ�������޴�
     * 
     * @return int 
     */
    int GetSendTimeout()
    {
        int size;
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, size);
        return size;
    }
    void SetSendTimeout(int size)
    {
        SetSocketOption(SocketOptionLevel::Socket, SocketOptionName::SendTimeout, size);
    }
    /**
     * @brief ָ�� Socket ���͵� Internet Э�� (IP) ���ݰ�������ʱ�� (TTL) ֵ
     * 
     * @return byte 
     */
    byte GetTtl()
    {
        byte val;
        int size = sizeof(byte);
        GetSocketOption(SocketOptionLevel::Socket, SocketOptionName::TTL, (char*)&val, size);
        ERROR_ASSERT(sizeof(byte) == size, "GetTtl failed");
        return val;
    }
    void SetTtl(byte size)
    {
        SetSocketOption(SocketOptionLevel::IP, SocketOptionName::TTL, (char*)&size, 1);
    }
    /**
     * @brief Ϊ�½����Ӵ����µ� Socket
     * �������׽��ֵ��������������ͬ����ȡ��һ���������������Ȼ�󴴽��������µ� Socket �� 
     * ����ʹ�÷��صĴ� Socket �������������Ӷ��е������κ����ӡ� 
     * ���ǣ����Ե��÷�����ʶԶ������ Socket �������ַ�Ͷ˿ں�
     * 
     * @return Socket* 
     */
    Socket* Accept()
    {
        Socket* s = new Socket();
        SOCKETAPI_ASSERT
        (
            INVALID_SOCKET  != (s->m_socket = accept(m_socket, (SOCKADDR *)s->m_addr.Item, &s->m_addr.Size)),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-accept"
        );
        return s;
    }
    void Bind(SocketAddress &localEP)
    { 
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != bind(m_socket, (SOCKADDR *)localEP.Item, localEP.Size),
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-bind"
        );
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
            /**
                WSANOTINITIALISED
                    A successful WSAStartup call must occur before using this function.
                WSAENETDOWN
                    The network subsystem has failed.
                WSAEADDRINUSE
                    The socket's local address is already in use and the socket was not marked to allow address reuse with SO_REUSEADDR. This error usually occurs during execution of the bind function, but could be delayed until this function if the bind was to a partially wildcard address (involving ADDR_ANY) and if a specific address needs to be committed at the time of this function.
                WSAEINPROGRESS
                    A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
                WSAEINVAL
                    The socket has not been bound with bind.
                WSAEISCONN
                    The socket is already connected.
                WSAEMFILE
                    No more socket descriptors are available.
                WSAENOBUFS
                    No buffer space is available.
                WSAENOTSOCK
                    The descriptor is not a socket.
                WSAEOPNOTSUPP
                    The referenced socket is not of a type that supports the listen operation. 
             */
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
    int Send(const char* buffer, int size, SocketFlags socketFlags, SocketError &errorcode)
    {
        int nRet;
        /* https://docs.microsoft.com/en-us/windows/win32/api/winsock2/nf-winsock2-send */
        if(SOCKET_ERROR == (nRet = send(m_socket, buffer, size, socketFlags)))
        {
            /**
                WSANOTINITIALISED
                    A successful WSAStartup call must occur before using this function.
                WSAENETDOWN
                    The network subsystem has failed.
                WSAEACCES
                    The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST socket option to enable use of the broadcast address.
                WSAEINTR
                    A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.
                WSAEINPROGRESS
                    A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
                WSAEFAULT
                    The buf parameter is not completely contained in a valid part of the user address space.
                WSAENETRESET
                    The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress.
                WSAENOBUFS
                    No buffer space is available.
                WSAENOTCONN
                    The socket is not connected.
                WSAENOTSOCK
                    The descriptor is not a socket.
                WSAEOPNOTSUPP
                    MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.
                WSAESHUTDOWN
                    The socket has been shut down; it is not possible to send on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.
                WSAEWOULDBLOCK
                    The socket is marked as nonblocking and the requested operation would block.
                WSAEMSGSIZE
                    The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.
                WSAEHOSTUNREACH
                    The remote host cannot be reached from this host at this time.
                WSAEINVAL
                    The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.
                WSAECONNABORTED
                    The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
                WSAECONNRESET
                    The virtual circuit was reset by the remote side executing a hard or abortive close. For UDP sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a "Port Unreachable" ICMP packet. The application should close the socket as it is no longer usable.
                WSAETIMEDOUT
                    The connection has been dropped, because of a network failure or because the system on the other end went down without notice. 
             * 
             */
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
        /**
            WSANOTINITIALISED
                A successful WSAStartup call must occur before using this function.
            WSAENETDOWN
                The network subsystem has failed.
            WSAEACCES
                The requested address is a broadcast address, but the appropriate flag was not set. Call setsockopt with the SO_BROADCAST parameter to allow the use of the broadcast address.
            WSAEINVAL
                An unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled.
            WSAEINTR
                A blocking Windows Sockets 1.1 call was canceled through WSACancelBlockingCall.
            WSAEINPROGRESS
                A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function.
            WSAEFAULT
                The buf or to parameters are not part of the user address space, or the tolen parameter is too small.
            WSAENETRESET
                The connection has been broken due to keep-alive activity detecting a failure while the operation was in progress.
            WSAENOBUFS
                No buffer space is available.
            WSAENOTCONN
                The socket is not connected (connection-oriented sockets only).
            WSAENOTSOCK
                The descriptor is not a socket.
            WSAEOPNOTSUPP
                MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only receive operations.
            WSAESHUTDOW
                The socket has been shut down; it is not possible to sendto on a socket after shutdown has been invoked with how set to SD_SEND or SD_BOTH.
            WSAEWOULDBLOCK
                The socket is marked as nonblocking and the requested operation would block.
            WSAEMSGSIZE
                The socket is message oriented, and the message is larger than the maximum supported by the underlying transport.
            WSAEHOSTUNREACH
                The remote host cannot be reached from this host at this time.
            WSAECONNABORTED
                The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
            WSAECONNRESET
                The virtual circuit was reset by the remote side executing a hard or abortive close. For UPD sockets, the remote host was unable to deliver a previously sent UDP datagram and responded with a "Port Unreachable" ICMP packet. The application should close the socket as it is no longer usable.
            WSAEADDRNOTAVAIL
                The remote address is not a valid address, for example, ADDR_ANY.
            WSAEAFNOSUPPORT
                Addresses in the specified family cannot be used with this socket.
            WSAEDESTADDRREQ
                A destination address is required.
            WSAENETUNREACH
                The network cannot be reached from this host at this time.
            WSAEHOSTUNREACH
                A socket operation was attempted to an unreachable host.
            WSAETIMEDOUT
                The connection has been dropped, because of a network failure or because the system on the other end went down without notice.
            */
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
    inline int IOControl(IOControlCode ioControlCode, DWORD &optionInOutValue)
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
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, char* buffer, int &optlen)
    {
        SOCKETAPI_ASSERT
        (
            SOCKET_ERROR != getsockopt(m_socket, (int)optionLevel, optionName, buffer, &optlen), 
            "SetSocketOption failed"
            "https://docs.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-getsockopt"
        );
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, bool &optionValue)
    {
        int optlen = sizeof(bool);
        GetSocketOption(optionLevel, optionName, (char *)&optionValue, optlen);
    }
    inline void GetSocketOption(SocketOptionLevel optionLevel, SocketOptionName optionName, int &optionValue)
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
     * @brief ���� Socket �ϵķ��ͺͽ���
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
readonly System::Net::Sockets::Socket::SocketlibraryInitlializer System::Net::Sockets::Socket::m_socketlibraryInitlializer;
#endif