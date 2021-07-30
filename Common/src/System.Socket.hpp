#ifndef SYSTEM_SOCKET_H
#define SYSTEM_SOCKET_H

#include "System.Threading.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <map>

#define RECEIVE_BUFFER_SIZE 1024
#ifdef WIN32
    #pragma comment(lib,"ws2_32.lib")
    #define _WINSOCK_DEPRECATED_NO_WARNINGS	                    //忽略inet_addr()函数触发的错误

    #define SocketLib_Version(hight, low) MAKEWORD(hight,low)	//创建Socket库版本对象
    #define DEFAULT_WSLIB_VERSION SocketLib_Version(2,2)		//默认为2.2版本的库
#endif


namespace System
{
    using namespace System::Threading;
    #define TCP_MAX_CLIENT 0xff
    #ifdef WIN32 
    typedef struct tagSocketInfo
    {
        WSADATA WsaData;
        SOCKET Socket;
        sockaddr_in HostAddr;
        sockaddr_in ClientAddr[TCP_MAX_CLIENT];
        int CurrentClientCount;
        SocketConnectProxy Proxy;
    }SocketInfo;
    #endif

    typedef enum SocketConnectProxy
    {
		Tcp = 0,
		Udp,
        SerialPort
    };   
    typedef bool (*CreateSocketInstance)(SocketInfo &info, unsigned short SelfPort, char* SelfIp, unsigned short OtherSidePort, char* OtherSideIp);
    typedef std::map<SocketConnectProxy, CreateSocketInstance> CreateSocketMap;
    bool GetTcpInstance(SocketInfo &info, unsigned short SelfPort, char* SelfIp, unsigned short OtherSidePort, char* OtherSideIp) 
    { 
        info.Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        info.HostAddr.sin_family        = AF_INET;
        info.HostAddr.sin_addr.s_addr   = inet_addr(SelfIp);
        info.HostAddr.sin_port          = htons(SelfPort);
        info.ClientAddr[info.CurrentClientCount].sin_family      = AF_INET;
        info.ClientAddr[info.CurrentClientCount].sin_addr.s_addr = inet_addr(OtherSideIp);
        info.ClientAddr[info.CurrentClientCount].sin_port        = htons(OtherSidePort);
        info.CurrentClientCount++;
        if(SOCKET_ERROR == bind(info.Socket, (sockaddr *)&info.HostAddr, sizeof(info.HostAddr)))
        {
            return false;
        }
        #ifdef WIN32
        int optVal = 1;
        if (SOCKET_ERROR == setsockopt(info.Socket, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char *)&optVal, sizeof(int)))
        {
            return false;
        }
        #endif
        if(listen(info.Socket, 10))
        {
            return false;
        }
        return INVALID_SOCKET == info.Socket;
    }
    bool GetUdpInstance(SocketInfo &info, unsigned short SelfPort, char* SelfIp, unsigned short OtherSidePort, char* OtherSideIp = "") /* 单播 广播 多播 */
    { 
        info.Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
        info.HostAddr.sin_family        = AF_INET;
        info.HostAddr.sin_addr.s_addr   = inet_addr(SelfIp);
        info.HostAddr.sin_port          = htons(SelfPort);
        info.ClientAddr[info.CurrentClientCount].sin_family     = AF_INET;
        info.ClientAddr[info.CurrentClientCount].sin_addr.s_addr= 0 == OtherSideIp[0] ? INADDR_BROADCAST : inet_addr(OtherSideIp);
        info.ClientAddr[info.CurrentClientCount].sin_port       = htons(OtherSidePort);
        info.CurrentClientCount++;
        int optval = 1;
        /* 手动向内核确认是广播*/
        if(SOCKET_ERROR == setsockopt(info.Socket, SOL_SOCKET, SO_BROADCAST, (char *)&optval, sizeof(int)))
        {
            return false;
        }
        /* 设置缓冲区大小*/
        optval = 1024*1024*1;
        if(SOCKET_ERROR == setsockopt(info.Socket, SOL_SOCKET, SO_SNDBUF, (char *)&optval, sizeof(int)))
        {
            return false;
        }
        optval = 1024*1024*1;
        if(SOCKET_ERROR == setsockopt(info.Socket, SOL_SOCKET, SO_RCVBUF, (char *)&optval, sizeof(int)))
        {
            return false;
        }
        return INVALID_SOCKET == info.Socket;
    }
    CreateSocketMap createSocketMap = 
    {
        CreateSocketMap::value_type(Tcp, &GetTcpInstance),
        CreateSocketMap::value_type(Udp, &GetUdpInstance)
    };

    class Socket
    {
        typedef void (*RecvCallback)(const char* buf, int len);
        typedef int Send(const char* buf, int len, int clentId);
        typedef int Receive(char* buf, int len, char* (&srcIp));
        protected:
            SocketInfo info;
            Thread* thread;
            RecvCallback funcRecvCallback;
            int RecvTcp(char* buf, int len)
            {
                if(Running != thread->tInfo.CurrentStatus){return 0;}
                return recv(info.Socket, buf, len, 0);
            }
            int RecvUdp(char* buf, int len, char* (&srcIp))
            {
                if(Running != thread->tInfo.CurrentStatus){return 0;}
                struct sockaddr_in recvFromIP;
                int len = sizeof(recvFromIP);
                int ret = recvfrom(info.Socket, buf, len, 0, (struct sockaddr *)&recvFromIP, &len);
                srcIp = inet_ntoa(recvFromIP.sin_addr);
                return ret;
            }
            static unsigned int TcpBlockListen(void* args)
            {
	            char buf[RECEIVE_BUFFER_SIZE];
                System::Socket* p = (System::Socket*) args;
                int len;
                while(Running == p->thread->tInfo.CurrentStatus)
                {
                    len = p->RecvTcp(buf, RECEIVE_BUFFER_SIZE);
                    if(0 < len)
                    {
                        (*(p->funcRecvCallback))(buf, len);
                    }
                }
                return 0;
            }
            static unsigned int UdpBlockListen(void* args)
            {
	            char buf[RECEIVE_BUFFER_SIZE];
                System::Socket* p = (System::Socket*) args;
                char* ip;
                int len;
                while(Running == p->thread->tInfo.CurrentStatus)
                {
                    len= p->RecvUdp(buf, RECEIVE_BUFFER_SIZE, ip);
                    if(0 < len)
                    {
                        (*(p->funcRecvCallback))(buf, len);
                    }
                }
                return 0;
            }
        public:
            Socket(SocketConnectProxy proxy, unsigned short SelfPort, char* SelfIp, unsigned short OtherSidePort, char* OtherSideIp)
            {
                #ifdef WIN32
                WSAStartup(DEFAULT_WSLIB_VERSION, &info.WsaData);
                #endif
                bool creatSocketSucess = (*createSocketMap[proxy])(info, SelfPort, SelfIp, OtherSidePort, OtherSideIp); 
                if(creatSocketSucess)
                {
                    thread = Tcp == info.Proxy ? new Thread(TcpBlockListen, this) : Udp == info.Proxy ? new Thread(UdpBlockListen, this) : NULL;
                    info.Proxy = proxy;
                }
                else
                {      
                    thread = NULL;
                    thread->tInfo.CurrentStatus = NotInit;
                }
            }
            ~Socket(){Dispose();}
            bool Dispose()
            {
                int ret(0);
                if(NULL != thread)
                {
                    delete thread;
                    thread = NULL;
                }
                if(INVALID_SOCKET != info.Socket)
                {
                    shutdown(info.Socket, 0x01);	
                    ret = closesocket(info.Socket);
                    info.Socket = INVALID_SOCKET;
                }
                 
                #ifdef WIN32
                ret |= WSACleanup();
                #endif
                return 0 == ret;
            }
            int SendTcp(const char* buf, int len)
            {
                if(Running != thread->tInfo.CurrentStatus){return 0;}
                int realSendLen = 0;
                int ret;
                do 
                {
                    ret = send(info.Socket, (buf + realSendLen), (len - realSendLen), 0);
                    if (SOCKET_ERROR == ret)
                    {
                        return 0;
                    }
                    realSendLen += ret;
                } while (realSendLen < len);
            }
            int SendUdp(const char* buf, int len, int clentId = 0)
            {
                if(Running != thread->tInfo.CurrentStatus){return 0;}
                int realSendLen = 0;
                int ret;
                do 
                {
                    ret = sendto(info.Socket, (buf + realSendLen), (len - realSendLen), 0, (struct sockaddr *)&info.ClientAddr[clentId], sizeof(info.ClientAddr[clentId]));
                    if (SOCKET_ERROR == ret)
                    {
                        return 0;
                    }
                    realSendLen += ret;
                } while (realSendLen < len);
            }
            bool Start(RecvCallback _func)
            {
                if(Created != thread->tInfo.CurrentStatus)
                {
                    return false;
                }
                funcRecvCallback = _func;
                return thread->Start();
            }
            bool Stop()
            {
                if(thread->IsRunning())
                {
                     thread->tInfo.CurrentStatus = Canceled;
                    return Finished == thread->tInfo.CurrentStatus;
                }
                return false;
            }
    };
}

#endif 