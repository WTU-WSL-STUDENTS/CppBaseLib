/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-27 14:08:05
 * @LastEditors: like
 * @LastEditTime: 2022-03-27 20:37:19
 */
#ifndef SYSTEM_NET_SOCKETS_SOCKETDEF_H
#define SYSTEM_NET_SOCKETS_SOCKETDEF_H

#include <System.Net.IPAddress.hpp>
namespace System::Net::Sockets
{
    using LingerOption   = linger;
    using ProtocolType  = IPPROTO;
    using SocketError   = int;
    enum class IOControlCode : long;
    enum class SelectMode;
    enum class SocketAsyncOperation : long;
    enum class SocketFlags;
    enum class SocketOptionLevel;
    enum class SocketOptionName;
    enum class SocketShutdown;
    enum class SocketType;
};
enum class System::Net::Sockets::IOControlCode : long /* https://docs.microsoft.com/zh-cn/dotnet/api/system.net.sockets.iocontrolcode?view=net-5.0 */
{
    /**
     * @brief ���ȴ���������ʱ����֪ͨ
     * 
     */
    AsyncIO         =  (long)FIOASYNC,
    /**
     * @brief �����׽��ֵ�����( blocking mode )��Ϊ�� ���ʹ�ô˿��ƴ���ָ���Ĳ���Ϊ 0���׽�����������ģʽ�¡� ���������Ϊ�㣬�׽������ڷ�����ģʽ��
     * 
     */
    NonBlockingIO   = (long)FIONBIO,
    /**
     * @brief ���ؿɶ�ȡ���ֽ���
     * 
     */
    DataToRead      = (long)FIONREAD,
};
/**
 * @brief Ϊ Poll(Int32, SelectMode) ����������ѯģʽ
 * 
 */
enum class System::Net::Sockets::SelectMode
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
 * @brief ���ʹ�ô������Ķ���ִ�е��첽�׽��ֲ���������
 * 
 */
enum class System::Net::Sockets::SocketAsyncOperation : long
{
    /**
     * @brief û���׽��ֲ�����
     * 
     */
    None = 0,	
    /**
     * @brief һ���׽��� Accept ������ 
     * 
     */
    Accept,
    /**
     * @brief һ���׽��� Connect ������
     * 
     */
    Connect,
    /**
     * @brief һ���׽��� Disconnect ������
     * 
     */
    Disconnect,
    /**
     * @brief һ���׽��� Receive ������
     * 
     */
    Receive,
    /**
     * @brief һ���׽��� ReceiveFrom ������
     * 
     */
    ReceiveFrom,
    /**
     * @brief һ���׽��� ReceiveMessageFrom ������
     * 
     */
    ReceiveMessageFrom,
    /**
     * @brief һ���׽��� Send ������
     * 
     */
    Send,
    /**
     * @brief һ���׽��� SendPackets ������
     * 
     */
    SendPackets,
    /**
     * @brief һ���׽��� SendTo ������
     * 
     */
    SendTo
};
/**
 * @brief ָ���׽��ַ��ͺͽ�����Ϊ����ö��֧�����Աֵ�İ�λ���
 * 
 */
enum class System::Net::Sockets::SocketFlags
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
enum class System::Net::Sockets::SocketOptionName
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
enum class System::Net::Sockets::SocketShutdown
{
    Receive = 0,
    Send,
    Both
};
/**
 * @brief ָ�� Socket ���ʵ����ʾ���׽�������
 * 
 */
enum class System::Net::Sockets::SocketType
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
#endif