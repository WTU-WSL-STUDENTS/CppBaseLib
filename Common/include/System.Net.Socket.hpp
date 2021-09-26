/*
 * @Description: Tcp,Udp传输层接口
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-06 19:31:39
 * @LastEditors: like
 * @LastEditTime: 2021-09-13 07:48:30
 */
#ifndef SYSTEM_SOCKET_H
#define SYSTEM_SOCKET_H
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <CompliedEntry.h>

#define MAX_SEND_BUFFER_SIZE 1024 * 2
#define MAX_TCP_BUFFER_SIZE 1024
#define MAX_UDP_BUFFER_SIZE 1024
#define TCP_MAX_LISTEN_COUNT 10
#define IP_STR_SIZE 16
/* 自动通讯管道控制，设定后整个服务器系统的通讯管道Id不用人为设定，系统会自动处理 */
#define AUTO_PIPLINE_MANAGE 0xffffffff
#ifdef _WIN32
    #pragma comment(lib,"ws2_32.lib")
    #define _WINSOCK_DEPRECATED_NO_WARNINGS	                    //忽略inet_addr()函数触发的错误

    #define SocketLib_Version(hight, low) MAKEWORD(hight,low)	//创建Socket库版本对象
    #define DEFAULT_WSLIB_VERSION SocketLib_Version(2,2)		//默认为2.2版本的库
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
#endif
#include <System.Threading.Thread.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <map>

using namespace System::Threading;

/* https://blog.csdn.net/printf123scanf/article/details/89433001 */
namespace System::Net
{
    typedef enum _SocketConnectProxy
    {
		Tcp_Server = 0x11,  /* |1|,|{01}|   */
        Tcp_Client,         /* |1| |{10}|   */
        Tcp_Server_Client,  /* |1| |{11}|   */
		Udp_Server = 0x21,  /* |2| |{01}|   */
        Udp_Client,         /* |2| |{10}|   */  /* 单播 */ 
        // Udp_Server_Client,  /* |2| |{11}|   */   
        Udp_Gropcast = 0x26,/* |2| /{0110}/ */  /* 组播 */
        Udp_Broadcast = 0x2A/* |2| |{1010}| */  /* 广播 */
    }SocketConnectProxy;   
    typedef struct tagSocketInfo
    {
        int Proxy;
        SOCKET Socket;
        sockaddr_in ServerAddr;
    }SocketInfo;
#pragma region socket basic function
    /**
     * @description:创建Tcp Socket 
     * @param {socket配置} SocketInfo
     * @return {SOCKET是否创建成功}
     * @author: like
     */    
    bool CreateTcpSocket(SocketInfo& socket)
    {
        if(0 == (0x10 & socket.Proxy))
        {
            printf("Create Tcp Socket Failed, Proxy Not Tcp :%d\n", socket.Proxy);
            return false;
        } 
        if(INVALID_SOCKET == (socket.Socket = ::socket(AF_INET, SOCK_STREAM, 0)))
        {
            printf("Create Tcp Socket Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        return true;
    }
    /**
     * @description:创建Udp Socket 
     * @param {socket配置} SocketInfo
     * @return {SOCKET是否创建成功}
     * @author: like
     */ 
    bool CreateUdpSocket(SocketInfo& socket)
    {
        if(0 == (0x20 & socket.Proxy))
        {
            printf("Create Udp Socket Failed, Proxy Not Udp :%d\n", socket.Proxy);
            return false;
        }
        if(INVALID_SOCKET == (socket.Socket = ::socket(AF_INET, SOCK_DGRAM, 0)))
        {
            printf("Create Udp Socket Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        return true;
    }
    /**
     * @description:对Tcp/Udp单播/Udp组播，将主机字节顺序转为网络字节顺序
     * @param {Tcp/Udp单播/Udp组播配置} SocketInfo
     * @param {输入的的Ip地址主机字节顺序} ServerIp
     * @param {输入的的端口主机字节顺序} ServerPort
     * @author: like
     */  
    void SetTcpUdpServerAddr(SocketInfo& socket, const char* ServerIp, unsigned short ServerPort)
    {
        socket.ServerAddr.sin_family = AF_INET;
        inet_pton(AF_INET, ServerIp, &socket.ServerAddr.sin_addr); 
        socket.ServerAddr.sin_port = htons(ServerPort);  
    }
    /**
     * @description:对Udp广播，将主机字节顺序转为网络字节顺序 
     * @param {Udp广播的配置} SocketInfo
     * @param {Udp广播发送至服务器的端口号} ServerPort
     * @return Porxy是否包含Udp广播
     * @author: like
     */
    bool SetUdpClientBroadcastAddr(SocketInfo& socket, unsigned short ServerPort)
    {
        if(0 == (Udp_Client & socket.Proxy))
        {
            printf("Set Udp Client Broadcast Failed, Only Udp Client Can Call This%d\n", socket.Proxy);
            return false;
        }
        socket.ServerAddr.sin_family = AF_INET;
	    socket.ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_BROADCAST);
        socket.ServerAddr.sin_port = htons(ServerPort);  
        return true;
    }
    /**
     * @description:ServerAddr从网络字节顺序转换为主机字节顺序
     * @param {socket配置} SocketInfo
     * @param {将配置中的Ip地址从网络字节顺序转换为主机字节顺序} addrBuffer
     * @param {将配置的端口从网络字节顺序转换为主机字节顺序} port
     * @author: like
     */    
    void GetSocketAddress(const SocketInfo& socket, char (&addrBuffer)[IP_STR_SIZE], DWORD& port)
    {
        memset(addrBuffer, 0, sizeof(addrBuffer));
        inet_ntop(AF_INET,&socket.ServerAddr.sin_addr, addrBuffer, sizeof(sockaddr_in));   
        port = ntohs(socket.ServerAddr.sin_port);
    }
    /**
     * @description:Tcp/Udp服务器设定监听的本机的ip/端口 
     * @param {Tcp/Udp服务器配置} SocketInfo
     * @return {是否绑定成功 (服务器本机端口可以通过设置SendKernelToLockServerPort独占该端口)}
     * @author: like
     */    
    bool TcpUdpServerBind(const SocketInfo& socket)
    {
        if(0 == (0x01 & socket.Proxy))
        {
            printf("Server Bind Failed, Only Server Can Call \"ServerBind\"\n");
            return false;
        }
        if(SOCKET_ERROR == bind(socket.Socket, (sockaddr*)&socket.ServerAddr, sizeof(sockaddr)))
        {
            printf("Tcp Bind Error, code:%d\n", GetLastError());
            return false;
        }
        printf("Server Bind Sucess\n");
        return true;
    }
    /**
     * @description: 设置服务器独占当前Addr&Port
     * @param {服务器配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */    
    bool SendKernelToLockServerPort(const SocketInfo& socket)
    {      
        if(0 == (Tcp_Server & socket.Proxy))
        {
            printf("Lock Server Addr&Port Failed, Format Error\n");
            return false;
        } 
        int optVal = 1;
        if (SOCKET_ERROR == setsockopt(socket.Socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *)&optVal, sizeof(int)))
        {
            printf("Lock Server Addr&Port Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        printf("Lock Server Addr&Port Sucess\n");
        return true;
    }
    /**
     * @description: 设置服务器共享当前Addr&Port
     * @param {服务器配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */    
    bool SendKernelToUnlockServerPort(const SocketInfo& socket)
    {      
        if(0 == (Tcp_Server & socket.Proxy))
        {
            printf("Unlock Server Addr&Port Failed, Format Error\n");
            return false;
        } 
        int optVal = 1;
        if (SOCKET_ERROR == setsockopt(socket.Socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *)&optVal, sizeof(int)))
        {
            printf("Unlock Server Addr&Port Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        printf("Unlock Server Addr&Port Sucess\n");
        return true;
    }
    /**
     * @description: 向内核申明当前是广播
     * @param {服务器配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */    
    bool SendKernelToMakesureSocketIsBrodcast(const SocketInfo& socket)
    {
        if(0 == (Udp_Server & socket.Proxy))
        {
            printf("Markesure Brodcast Failed, Only Udp Server Can Call \"SendKernelToMakesureSocketIsBrodcast\"\n");
            return false;
        }
        int optval = 1;
        if(SOCKET_ERROR == setsockopt(socket.Socket, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(int)))
        {
            printf("Udp Markesure Brodcast Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        printf("Udp Markesure Brodcast Sucess\n");
        return true;
    }
    /**
     * @description: 设置Send数据buffer最大长度
     * @param {服务器配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */  
    bool SendKernelToSetSendBufferLen(const SocketInfo& socket)
    {
        int optval = MAX_SEND_BUFFER_SIZE;
        if(SOCKET_ERROR == setsockopt(socket.Socket, SOL_SOCKET, SO_SNDBUF, (char *)&optval, sizeof(int)))
        {
            printf("Set Send Buffer Size Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        printf("Set Send Buffer Size Sucess:%d\n", MAX_SEND_BUFFER_SIZE);
        return true;  
    }
    /**
     * @description: 设置Recv数据buffer最大长度
     * @param {服务器配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */
    bool SendKernelToSetRecvBufferLen(const SocketInfo& socket)
    {
        int optval = MAX_SEND_BUFFER_SIZE;
        if(SOCKET_ERROR == setsockopt(socket.Socket, SOL_SOCKET, SO_RCVBUF, (char *)&optval, sizeof(int)))
        {
            printf("Set Send Buffer Size Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        printf("Set Send Buffer Size Sucess:%d\n", MAX_SEND_BUFFER_SIZE);
        return true;  
    }
    /**
     * @description: 设置Tcp服务器连接客户端最大数量（TCP_MAX_LISTEN_COUNT）
     * @param {Tcp服务器配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */    
    bool TcpServerListen(const SocketInfo& socket)
    {
        if(0 == (Tcp_Server & socket.Proxy))
        {
            printf("Tcp Listen Failed, Only Tcp Server Can Call \"Listen\"\n");
            return false;
        }
        if(listen(socket.Socket, TCP_MAX_LISTEN_COUNT))
        {
            printf("Tcp listen Failed, Error Code:%d\n", GetLastError());
            return false;
        }
        printf("Tcp listen Sucess, Max Listen Count:%d \n", TCP_MAX_LISTEN_COUNT);
        return true;
    }
    /**
     * @description: Tcp客户端连接指定服务器
     * @param {Tcp客户端配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */ 
    bool TcpClientConnect(const SocketInfo& socket)
    {
        if(0 == (Tcp_Client & socket.Proxy))
        {
            printf("Tcp Connect Failed, Only Tcp Client Can Call \"Connect\"\n");
            return false;
        }
        if(SOCKET_ERROR == connect(socket.Socket, (sockaddr*)&socket.ServerAddr, sizeof(sockaddr)))
        {
            printf("Tcp Client Connect Failed, Error Code:%d\n",GetLastError());
            return false;
        }
        return true;
    }
    /**
     * @description: Tcp服务器等待客户端连接。
     * @param {Tcp客户端配置} SocketInfo
     * @param {Tcp客户端配置} SocketInfo
     * @return {设置是否成功}
     * @author: like
     */ 
    bool TcpServerAccecpt(const SocketInfo& socket, SocketInfo& accecptedInfo/* 返回对方的SocketInfo信息*/)
    {
        if(0 == (Tcp_Client & accecptedInfo.Proxy))
        {
            accecptedInfo.Proxy = Tcp_Client;         
        }    
        int len = (int)sizeof(sockaddr);
        if(INVALID_SOCKET == (accecptedInfo.Socket = accept(socket.Socket, (sockaddr*)&accecptedInfo.ServerAddr, &len)))
        {
            printf("Tcp Server Accept Failed, Error Code:%d\n",GetLastError());
            return false;
        }
        return true;
    }     
    /**
     * @description: Tcp服务器在指定时间内是否有消息等待接收
     * @param {Tcp服务器配置} SocketInfo
     * @param {通信管道ID} Pipline
     * @param {超时数据结构{秒，毫秒}} timeval
     * @param {select返回数据} fd_set
     * @return {设置是否成功}
     * @author: like
     */     
    bool TcpAcceptArrive(const SocketInfo& socket, int Pipline, const struct timeval& timeout, struct fd_set& rfds)
    {
        int len;
        FD_SET(socket.Socket, &rfds);
#ifdef _WIN32
        if(0 == (len = select(-1, &rfds, NULL,NULL, &timeout)))/* 1s内是否收到数据，若无则重头来 */
#else
        if(0 == (len = select(socket.Socket + 1,&rfds, NULL,NULL, &timeout)))/* 1s内是否收到数据，若无则重头来 */
#endif
        {
            FD_ZERO(&rfds);
            return false;
        }
        if(0 > len)
        {      
            printf("Tcp Ansyn Pipline_%d  Wait Accecpt, Select Message Failed, Error Code%d\n", Pipline, GetLastError());
            return false;
        }
        return true;
    }
    /**
     * @description: Socket客户端/服务器 是否有消息可以接收
     * @param {客户端/服务器配置} SocketInfo
     * @param {通信管道ID} Pipline
     * @param {超时数据结构{秒，毫秒}} timeval
     * @param {select返回数据} fd_set
     * @return {设置是否成功}
     * @author: like
     */    
    bool SocketPackArrive(const SocketInfo& socket, int Pipline,const struct timeval& timeout, struct fd_set& rfds)
    { 
        int len;
        FD_SET(socket.Socket, &rfds);
#ifdef _WIN32
        if(0 == (len = select(-1, &rfds, NULL,NULL, &timeout)))/* 1s内是否收到数据，若无则重头来 */
#else
        if(0 == (len = select(socket.Socket + 1,&rfds, NULL,NULL, &timeout)))/* 1s内是否收到数据，若无则重头来 */
#endif
        {
            FD_ZERO(&rfds);
            return false;
        }
        if(0 > len)
        {      
            printf("Pipline_%d Select Message Failed, Error Code%d\n", Pipline, GetLastError());
            return false;
        }
        return true;
    }
    /**
     * @description: Tcp连接，向对方发送数据
     * @param {客户端/服务器配置} SocketInfo
     * @param {发送缓冲区} buf
     * @param {缓冲区长度} len
     * @return {实际发送长度}
     * @author: like
     */    
    int SendTcp(const SocketInfo/* SocketClient/SocketServer */& socketSendTo, const char* buf, int len)
    {
        if(INVALID_SOCKET == socketSendTo.Socket){return 0;}
        int realSendLen = 0;
        int ret;
        do 
        {
            ret = send(socketSendTo.Socket, (buf + realSendLen), (len - realSendLen), 0);
            if (SOCKET_ERROR == ret)
            {
                char ip[IP_STR_SIZE];
                DWORD port;
                GetSocketAddress(socketSendTo, ip, port);
                printf("Send Tcp To %s:%u Failed, Error Code%d\n", ip, port, GetLastError());
                return 0;
            }
            realSendLen += ret;
        } while (realSendLen < len);
        return realSendLen;
    }
    /**
     * @description: Udp向指定端口&地址发送消息
     * @param {客户端/服务器配置} SocketInfo
     * @param {发送缓冲区} buf
     * @param {缓冲区长度} len
     * @return {实际发送长度}
     * @author: like
     */    
    int SendUdp(const SocketInfo/* SocketServer */& sockeSendToServer, const char* buf, int len)
    {
        if(INVALID_SOCKET == sockeSendToServer.Socket){return 0;}
        int realSendLen = 0;
        int ret;
        do 
        {
            ret = sendto(sockeSendToServer.Socket, (buf + realSendLen), (len - realSendLen), 0, (struct sockaddr*)&sockeSendToServer.ServerAddr, sizeof(struct sockaddr));
            if (SOCKET_ERROR == ret)
            {
                char ip[IP_STR_SIZE];
                DWORD port;
                GetSocketAddress(sockeSendToServer, ip, port);
                printf("Send Udp To %s:%u Failed, Error Code%d\n", ip, port, GetLastError());
                return 0;
            }
            realSendLen += ret;
        } while (realSendLen < len);
        return realSendLen;
    }  
    /**
     * @description: Tcp连接，接收对方发送的数据
     * @param {客户端/服务器配置} SocketInfo
     * @param {接收缓冲区} buf
     * @param {缓冲区长度} len
     * @return {实际发送长度}
     * @author: like
     */             
    int RecvTcp(const SocketInfo/* SocketClient */& socketRecvFromClient, char (&buf)[MAX_TCP_BUFFER_SIZE], int len)
    {
        if(INVALID_SOCKET == socketRecvFromClient.Socket){return 0;}
        return recv(socketRecvFromClient.Socket, buf, len, 0);
    } 
    /**
     * @description: Udp等待并接收指定端口消息
     * @param {接收人配置} SocketInfo
     * @param {返回发件人信息} SocketInfo
     * @param {接收缓冲区} buf
     * @param {缓冲区长度} len
     * @return {实际发送长度}
     * @author: like
     */
    int RecvUdp(const SocketInfo/* SocketServer */& socketRecvedServer, SocketInfo/* SocketClient */& socketSend, char (&buf)[MAX_UDP_BUFFER_SIZE], int len)
    {
        if(INVALID_SOCKET == socketRecvedServer.Socket){return 0;}
        static int addrStructSize = sizeof(sockaddr_in);
        return recvfrom(socketRecvedServer.Socket, buf, len, 0, (struct sockaddr *)&socketSend.ServerAddr, &addrStructSize);   
    }
#pragma endregion socket basic function
    typedef SocketInfo SocketClient;
    typedef bool (*RecvCallback)(const char* buf, int len, int pipline, const SocketClient& client);
    typedef struct _ServerReceiveMsgPipline
    {
        Thread* thread;
        SocketClient client;/* Tcp Accecpted Otherside*/
        RecvCallback funcRecvCallback;
    }ServerReceiveMsgPipline;
    typedef struct _SocketServer : SocketInfo
    {
        int PiplineCount;            /* 当前总共启用的通道数 */
        ServerReceiveMsgPipline Piplines[TCP_MAX_LISTEN_COUNT];/* 通道执行的具体参数 */
    }SocketServer;
    class Socket;
}

class System::Net::Socket/*创建一个Socket节点，能向1个通道发送数据，能接收 TCP_MAX_LISTEN_COUNT 个通道数据*/
{
private:
    SocketServer socket;
protected:
    bool AnsynAccepet;   
    int GetCurrentPipline()
    {
        for(int i = TCP_MAX_LISTEN_COUNT - 1; i > -1; i--)
        {
            if( socket.Piplines[i].thread)
            {
                return i;
            }
        }
        return -1;
    }
#ifdef _WIN32   
    bool InitSocketLibWin32()
    {         
        WSADATA wsaData;
        if(0 != WSAStartup(DEFAULT_WSLIB_VERSION, &wsaData))
        {
            printf("WSAStartup Failed\n");
            return false;
        }
        return true;
    }
    bool CloseSocketLibWin32()
    {
        if(0 != WSACleanup())
        {
            printf("WSACleanup Failed\n");
            return false;
        }
        return true;
    }
    static unsigned int WINAPI TcpBlockListen(void* args)
    {
        Socket* p = (Socket*) args;
        int Pipline;
        char buf[MAX_TCP_BUFFER_SIZE] = {0};
        int len;
        struct timeval timeout = {1, 0};
        struct fd_set rfds;
        FD_ZERO(&rfds);
        TcpUdpServerBind(p->socket);
        TcpServerListen(p->socket);
        if(p->AnsynAccepet)/* 异步TcpAccept */
        {
            Pipline = p->socket.PiplineCount;
            p->socket.PiplineCount++;
            for(int i = 0; i < p->socket.PiplineCount; i++, Pipline++)
            {
                while (NULL == p->socket.Piplines[Pipline].thread)
                {
                    Pipline++;
                }   
                if(Tcp_Client != (Tcp_Client & p->socket.Piplines[Pipline].client.Proxy))
                {
                    break;
                }
            }
            if(Pipline == p->socket.PiplineCount)
            {
                printf("Ansyn Pipline_%d Wait Failed, AnsynAccepet Need Support Start Tcp Server By \"AUTO_PIPLINE_MANAGE\"\n", Pipline);
                return 1;
            }
            printf("Begin Ansyn Pipline_%d Wait,  API Use Auth Sucess\n", Pipline);
            bool accecptedFlag = false;
            while(Running == p->socket.Piplines[Pipline].thread->tInfo.CurrentStatus)
            {
                if(!TcpAcceptArrive(p->socket, Pipline, timeout, rfds))
                {
                    continue;
                }
                printf("Piplne_%d Begin Wait Client\n", Pipline); 
                if(!TcpServerAccecpt(p->socket, p->socket.Piplines[Pipline].client))
                {
                    printf("Pipline_%d Wait Client Failed\n", Pipline);      
                    return 1;
                }
                accecptedFlag = true;
                char buf[IP_STR_SIZE];
                DWORD port;
                GetSocketAddress(p->socket.Piplines[Pipline].client, buf, port);
                printf("Piplne_%d Connect Client Sucess, Client Address:%s:%u\n", Pipline,buf, port); 
                break;
            }
            if(!accecptedFlag)
            {
                printf("Stop Server , Pipline_%d Wait Accecpt Canceled\n", Pipline);
                return 1;
            }
        }
        else/* 同步Tcp*/
        {
            Pipline = p->GetCurrentPipline();      
        }
        FD_ZERO(&rfds);
        while(Running == p->socket.Piplines[Pipline].thread->tInfo.CurrentStatus)
        {
            if(!SocketPackArrive(p->socket.Piplines[Pipline].client, Pipline, timeout, rfds))
            {
                continue;
            }
            memset(buf, 0, sizeof(buf));
            if(0 < (len = RecvTcp(p->socket.Piplines[Pipline].client, buf, MAX_TCP_BUFFER_SIZE)))
            {
                if(!p->socket.Piplines[Pipline].funcRecvCallback)
                {
                    continue;
                }
                if((*(p->socket.Piplines[Pipline].funcRecvCallback))(buf, len,  Pipline, p->socket.Piplines[Pipline].client))
                {
                    continue;
                }
                p->socket.Piplines[Pipline].thread->tInfo.CurrentStatus = Finished;
            }
            else
            {
                if(10054 == GetLastError())/* 远程客户端退出，当前Pipline结束 */
                {
                    printf("Piplne_%d Recv Tcp Failed, Remote Client Closed\n", Pipline); 
                    return 1;
                }
                printf("Piplne_%d Recv Tcp Failed, Error Code:%d\n", Pipline, GetLastError());  
                // return 1;
            }
        }   
        return 0;
    }
    static unsigned int WINAPI UdpBlockListen(void* args)
    {
        Socket* p = (Socket*) args;
        int Pipline = p->GetCurrentPipline(); 
        char buf[MAX_TCP_BUFFER_SIZE] = {0};
        int len;
        struct timeval timeout = {1, 0};
        struct fd_set rfds;  
        FD_ZERO(&rfds);
        TcpUdpServerBind(p->socket);
        while(Running == p->socket.Piplines[Pipline].thread->tInfo.CurrentStatus)
        {
            if(!SocketPackArrive(p->socket, Pipline, timeout, rfds))
            {
                continue;
            }
            memset(buf, 0, sizeof(buf));
            if(0 < (len = RecvUdp(p->socket, p->socket.Piplines[Pipline].client, buf, MAX_TCP_BUFFER_SIZE)))
            {
                if(!p->socket.Piplines[Pipline].funcRecvCallback)
                {
                    continue;
                }
                if((*(p->socket.Piplines[Pipline].funcRecvCallback))(buf, len,  Pipline, p->socket.Piplines[Pipline].client))
                {
                    continue;
                }
                p->socket.Piplines[Pipline].thread->tInfo.CurrentStatus = Finished;
            }
            else
            {           
                printf("Piplne_%d Recv Tcp Failed, Error Code:%d\n", Pipline, GetLastError());  
            }
        }   
        return 0;
    }

#endif
    Socket(){}
public:
    Socket(const char* ServerIp/* 输入的广播的ServerIp无任何影响*/, unsigned short ServerPort, int proxy = Tcp_Server) : socket({proxy, NULL, {0}, 0, {0}}), AnsynAccepet(false)
    {
#ifdef _WIN32
        InitSocketLibWin32();
#endif
        socket.Proxy = proxy;
        /* Tcp */
        if(Tcp_Client == (Tcp_Client & proxy)) /* TCP客户端 */
        {
            CreateTcpSocket(socket);
            SetTcpUdpServerAddr(socket, ServerIp, ServerPort);
        }
        if(Tcp_Server == (Tcp_Server & proxy)) /* Tcp服务器 */
        {
            CreateTcpSocket(socket);
            SetTcpUdpServerAddr(socket, ServerIp, ServerPort);
            // TcpUdpServerBind(socket);
            // TcpServerListen(socket);
        }
        /* Udp */
        if(Udp_Broadcast == (Udp_Broadcast & proxy))/* Udp广播 */
        {
            CreateUdpSocket(socket);
            SetUdpClientBroadcastAddr(socket, ServerPort);
            SendKernelToMakesureSocketIsBrodcast(socket);  
        }
        else if(Udp_Gropcast == (Udp_Gropcast & proxy) || Udp_Client == (Udp_Client & proxy))/* 组播/单播 */
        {
            CreateUdpSocket(socket);
            SetTcpUdpServerAddr(socket, ServerIp, ServerPort); 
        }
        else if(Udp_Server == (Udp_Server & proxy))/* Udp服务器 */
        {
            CreateUdpSocket(socket);  
            SetTcpUdpServerAddr(socket, ServerIp, ServerPort); 
            // TcpUdpServerBind(socket);
        }
    }
    ~Socket(){Dispose();}

    /**
     * @description: 释放Socket客户端/服务器调用的所有资源
     * @param {*}
     * @return {返回是否成功释放Socket客户端/服务器调用的资源}
     * @author: like
     */        
    bool Dispose()
    {
        if(0 != (0x01 & socket.Proxy))/* 释放服务器线程资源 */
        {
            for(int i = 0, j = 0; i < TCP_MAX_LISTEN_COUNT  && j < socket.PiplineCount; i++, j++)
            {
                while(NULL == socket.Piplines[i].thread && i < TCP_MAX_LISTEN_COUNT)
                {
                    i++;
                }
                if(!socket.Piplines[i].thread)
                {
                    continue;
                }
                if(Running == socket.Piplines[i].thread->tInfo.CurrentStatus)/*说如果服务器还在跑，直接干掉*/
                {
                    if(!StopServer())
                    {
                        printf("Server Is Running, Try Close Server Failed, Pipline:%d\n", i);
                        return false;       
                    }
                }
                delete socket.Piplines[i].thread;
                socket.Piplines[i].thread = NULL;
            }
        }
        if(INVALID_SOCKET != socket.Socket)/* 释放Socket资源 */
        {
            shutdown(socket.Socket, 0x01);	
            if(0 != closesocket(socket.Socket))
            {
                printf("closesocket Failed, Error Code:%d\n", GetLastError());
                return false;
            }
            socket.Socket = INVALID_SOCKET;
        }        
#ifdef _WIN32
        return CloseSocketLibWin32();
#else
        return true;
#endif
    }
    /**
     * @description: Tcp服务设置异步Accept
     * @param {是否为异步，如果不在Tcp服务器的Proxy设定下没有任何作用} isAnsyn
     * @return {*}
     * @author: like
     */    
    void SetAnsynAccepet(bool isAnsyn)
    {
        if(Tcp_Server == (Tcp_Server & socket.Proxy))
        {
            AnsynAccepet = isAnsyn;
        }
    }
    /**
     * @description: 客户端连接服务器
     * @param {*}
     * @return {客户端是否成功连接服务器}
     * @author: like
     */    
    bool ConnectToServer()
    {
        return TcpClientConnect(socket);
    }        
    /**
     * @description: 启动服务
     * @param {收到数据后的回调函数} _func
     * @param {服务启动时的回调函数} _funcStart
     * @param {服务结束时的回调函数} _funcFinished
     * @param {需要启动的PiplineId 可设置为AUTO_PIPLINE_MANAGE} Pipline
     * @return {返回实际Pipline Id}
     * @author: like
     */                    
    int StartServer(RecvCallback _func = NULL, OnThreadStart _funcStart = NULL, OnThreadFinished _funcFinished = NULL, int Pipline = AUTO_PIPLINE_MANAGE)
    {
        if(0 == (0x01 & socket.Proxy))/* Not Server */
        {
            printf("Start Server Failed, Format Error\n");
            return false;       
        }
        if(AUTO_PIPLINE_MANAGE == Pipline)
        {
            Pipline = GetCurrentPipline() + 1;     
        }
        if(Pipline >= TCP_MAX_LISTEN_COUNT)/*Server Full*/
        {
            printf("Start Server Failed, Pipline Is Full\n");
            return -1;
        }
        if( socket.Piplines[Pipline].thread)/* Pipline Exists */
        {
            printf("Start Server Failed, Pipline-%d Already Exists\n", Pipline);
            return false;
        }
        if(Tcp_Server == (Tcp_Server & socket.Proxy)) /* Tcp Server */
        {
            if(!AnsynAccepet)
            {
                printf("Piplne_%d Begin Wait Client\n", Pipline); 
                if(!TcpServerAccecpt(socket, socket.Piplines[Pipline].client))
                {
                    printf("Piplne_%d Wait Client Failed\n", Pipline); 
                    return -1;
                }  
                socket.PiplineCount++;
                char buf[IP_STR_SIZE];
                DWORD port;
                GetSocketAddress(socket.Piplines[Pipline].client, buf, port);
                printf("Piplne_%d Connect Client Sucess, Client Address:%s:%u\n", Pipline,buf, port); 
            }
            socket.Piplines[Pipline].thread = new Thread((THREAD_EVENT)TcpBlockListen, this);
        }
        else/* Udp Server */
        {
            socket.Piplines[Pipline].thread = new Thread((THREAD_EVENT)UdpBlockListen, this);    
        }
        if(Created != socket.Piplines[Pipline].thread->tInfo.CurrentStatus)
        {
            delete socket.Piplines[Pipline].thread;
            socket.Piplines[Pipline].thread = NULL;
            printf("Piplne_%d Thread Init Failed\n", Pipline); 
            return -1;
        } 
        socket.Piplines[Pipline].thread->OnStart    = _funcStart;
        socket.Piplines[Pipline].thread->OnFinished = _funcFinished;
        socket.Piplines[Pipline].funcRecvCallback = _func;
        if(!socket.Piplines[Pipline].thread->Start())
        {
            printf("Piplne_%d Thread Start Failed\n", Pipline); 
            StopServer(Pipline);
            return -1;
        }
        return Pipline;
    }
    /**
     * @description:关闭服务
     * @param {需要启动的PiplineId 可设置为AUTO_PIPLINE_MANAGE} Pipline
     * @return {如果服务器没有运行返回true;如果服务器在运行，向其发送停止信号，最多等待1s服务器返回是否成功关闭结果}
     * @author: like
     */        
    bool StopServer(int Pipline = AUTO_PIPLINE_MANAGE)
    {
        if(0 == (0x01 & socket.Proxy))/* Not Server */
        {
            printf("Stop Server Failed, Format Error\n");
            return false;       
        }
        if(AUTO_PIPLINE_MANAGE == Pipline)
        {
            Pipline = GetCurrentPipline();
        }
        if(!socket.Piplines[Pipline].thread)/* Pipline Not Exists Or Already Stopped*/
        {
            printf("Stop Server Failed, Pipline-%d Not Exists\n", Pipline);
            return false;
        }
        if(socket.Piplines[Pipline].thread->IsRunning())
        {     
            int status = socket.Piplines[Pipline].thread->tInfo.CurrentStatus;        
            socket.Piplines[Pipline].thread->tInfo.CurrentStatus = Canceled;
            if(!socket.Piplines[Pipline].thread->WaitThreadCanceled(5000))/* 最多等待5s */
            {
                socket.Piplines[Pipline].thread->tInfo.CurrentStatus = status;
                printf("Stop Server Timeout\n");
                return false;
            }
        }
        socket.PiplineCount--;
        socket.Piplines[Pipline].client = {0};
        delete socket.Piplines[Pipline].thread;
        socket.Piplines[Pipline].thread->OnStart = NULL;
        socket.Piplines[Pipline].thread->OnFinished = NULL;
        socket.Piplines[Pipline].thread = NULL;
        socket.Piplines[Pipline].funcRecvCallback = NULL;
        return true;
    }
    /**
     * @description:服务在Start后，调用该函数可以阻塞当前一直到服务关闭
     * @param {需要等待的PiplineId 可设置为AUTO_PIPLINE_MANAGE} Pipline
     * @return {等待流程是否正确}
     * @author: like
     */               
    bool WaitExitFlag(int Pipline = AUTO_PIPLINE_MANAGE)
    {
        if(0 == (0x01 & socket.Proxy))
        {
            printf("Wait Server Exit Failed, Proxy Format Error\n");
            return false;
        }
        if(AUTO_PIPLINE_MANAGE == Pipline)
        {
            Pipline = GetCurrentPipline();
        }
        if(0 > Pipline)
        {
            printf("Pipline_[%d] Is Empty ", Pipline);
            return false;
        }
        return socket.Piplines[Pipline].thread->WaitThreadFinshed();
    }
    /**
     * @description:Socket服务是否创建成功 
     * @param {*}
     * @return {返回指定的服务是否初始化完成} 
     * @author: like
     */        
    bool IsCreateSocketSucess(){return INVALID_SOCKET != socket.Socket;}
    /**
     * @description:Socket服务是正在运行
     * @param {指定PiplineId 可设置为AUTO_PIPLINE_MANAGE} Pipline
     * @return {返回指定的服务是否正在运行}
     * @author: like
     */        
    bool IsRunning(int Pipline){return Running == socket.Piplines[Pipline].thread->tInfo.CurrentStatus;}
    /**
     * @description: Socket服务是否已经结束
     * @param {指定PiplineId 可设置为AUTO_PIPLINE_MANAGE} Pipline
     * @return {返回指定的服务是否已经结束}
     * @author: like
     */        
    bool IsFinished(int Pipline){return Finished == socket.Piplines[Pipline].thread->tInfo.CurrentStatus;}
    /**
     * @description: Tcp服务器可调用该接口向Tcp客户端发送消息
     * @param {指定PiplineId 可设置为AUTO_PIPLINE_MANAGE} pipline
     * @param {要发送的buffer} buf
     * @param {Tcp服务器要发送buffer的预期长度，最大不超过MAX_TCP_BUFFER_SIZE} len
     * @return {Tcp服务器实际发送buffer的长度}
     * @author: like
     */    
    int SendTcpToClient(int pipline, const char* buf, int len)
    {
        if(Tcp_Server != (Tcp_Server & socket.Proxy))
        {
            printf("Send Tcp To Client Failed, Proxy Format Error\n");
            return 0;
        }
        return SendTcp(socket.Piplines[pipline].client, buf, len);
    }
    /**
     * @description: Tcp客户端可调用该接口向Tcp服务器发送消息
     * @param {指定PiplineId 可设置为AUTO_PIPLINE_MANAGE} pipline
     * @param {要发送的buffer} buf
     * @param {Tcp客户端要发送buffer的预期长度，最大不超过MAX_TCP_BUFFER_SIZE} len
     * @return {Tcp客户端实际发送buffer的长度}
     * @author: like
     */   
    int SendTcpToServer(const char* buf, int len)
    {
        if(Tcp_Client != (Tcp_Client & socket.Proxy))
        {
            printf("Send Tcp To Server Failed, Proxy Format Error\n");
            return 0;
        }
        return SendTcp(socket, buf, len);
    }   
    /**
     * @description: Udp客户端可调用该接口向Udp服务器发送消息
     * @param {要发送的buffer} buf
     * @param {Udp客户端要发送buffer的预期长度，最大不超过MAX_TCP_BUFFER_SIZE} len
     * @return {Udp客户端实际发送buffer的长度}
     * @author: like
     */         
    int SendUdpToServer(const char* buf, int len)
    {
        if(Udp_Client != (Udp_Client & socket.Proxy))
        {
            printf("Send Udp To Server Failed, Proxy Format Error\n");
            return 0;
        }
        return SendUdp(socket, buf, len);
    } 
    /**
     * @description: Tcp服务器可以调用该接口等待并接收Tcp客户端发送的消息 
     * @param {指定PiplineId 可设置为AUTO_PIPLINE_MANAGE} pipline
     * @param {Tcp服务器接收数据的buffer} buf
     * @param {Tcp服务器buffer预期长度，最大不超过MAX_TCP_BUFFER_SIZE} len
     * @return {Tcp服务器实际接收的长度}
     * @author: like
     */    
    int RecvTcpFromClient(int pipline, char (&buf)[MAX_TCP_BUFFER_SIZE], int len)
    {
        if(Tcp_Server != (Tcp_Server & socket.Proxy))
        {
            printf("Recv Tcp From Client Failed, Proxy Format Error\n");
            return 0;
        }
        return RecvTcp(socket.Piplines[pipline].client, buf, len);
    }  
    /**
     * @description:Tcp客户端可以调用该接口等待并接收Tcp服务器回复的消息
     * @param {Tcp客户端接收数据的buffer} buf
     * @param {Tcp客户端buffer预期长度，最大不超过MAX_TCP_BUFFER_SIZE} len
     * @return {Tcp客户端实际接收的长度}
     * @author: like
     */      
    int RecvTcpFromServer(char (&buf)[MAX_TCP_BUFFER_SIZE], int len)
    {
        if(Tcp_Client != (Tcp_Client & socket.Proxy))
        {
            printf("Recv Tcp From Server Failed, Proxy Format Error\n");
            return 0;
        }
        return RecvTcp(socket, buf, len);  
    }
    /**
     * @description: Udp服务器可调用该接口等待并接受Udp客户端发送的消息
     * @param {接收数据的buffer}} buf
     * @param {Udp服务器收到buferr的预期长度，最大不超过MAX_UDP_BUFFER_SIZE} len
     * @param {返回客户端的地址} client
     * @return {Udp服务器实际接收到的数据}
     * @author: like
     */
    int RecvUdpFromClient(char (&buf)[MAX_TCP_BUFFER_SIZE], int len, SocketClient& client)
    {
        if(Udp_Server != (Udp_Server & socket.Proxy))
        {
            printf("Recv Tcp From Server Failed, Proxy Format Error\n");
            return 0;
            
        }
        return RecvUdp(socket, client, buf, len);
    }
};

#endif 