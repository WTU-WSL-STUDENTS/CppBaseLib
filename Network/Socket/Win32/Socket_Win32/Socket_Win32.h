#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS	//忽略inet_addr()函数触发的错误
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <vector>

//namespace Socket_Win32

//ws2_32.lib 调用win32封装库的Socket通信
namespace Socket_Win32_Ult{
	#pragma comment(lib,"ws2_32.lib")

	#define SocketLib_Version(hight, low) MAKEWORD(hight,low)	//创建Socket库版本对象
	#define DefultSocketLib_Version SocketLib_Version(2,2)		//默认为2.2版本的库

	class SocketLib_Win32
	{
	protected:
		static WSADATA wsaData;
	public:
		static int StartUp(WORD version) { return WSAStartup(version, &wsaData); }	//启动Socket库
		static int StartUp() { return StartUp(DefultSocketLib_Version); }			//以默认的2.2版本启动库
		static int Close() { return WSACleanup(); }									//关闭Socket库
		static int GetLastErro() { return WSAGetLastError(); }						//获取Socket库最近出现的一次错误码
		//获取版本号
		static WORD LibVersion() { return SocketLib_Version(wsaData.wHighVersion, wsaData.wVersion); }			//获取版本号
		static float LibVersionA() { return (HIBYTE(wsaData.wHighVersion) + 0.1f * LOBYTE(wsaData.wVersion)); }	//获取版本的数值2.2
		//获取最大socket连接数
		static int MaxSocketConnectSize() { return wsaData.iMaxSockets; }
	};

	//Socket传输协议的类型
	enum class  SocketIpprotoType
	{
		TCP = IPPROTO_TCP,
		UDP = IPPROTO_UDP,
		ICMP,
		IGMP,
		RM
	};

	#define SocketPort_Min 1024	//0-1023:系统保留端口号;49252-65535:动态端口号
	#define SocketPort_Max 49151
	struct SocketConfig : SOCKADDR_IN
	{
	public:
		SocketConfig(const char* ip = "127.0.0.1",USHORT port = SocketPort_Min)
		{
			sin_family = AF_INET;
			sin_addr.S_un.S_addr = inet_addr(ip);
			sin_port = port;
		}
	};

	class Socket_Win32
	{
	public:
		static SOCKET GetSocketInstance(SocketIpprotoType IPPROTO_Type = SocketIpprotoType::UDP) { return socket(AF_INET, SOCK_DGRAM, (int)IPPROTO_Type); }
		static int CloseSocket(SOCKET s) { return closesocket(s); }
		static bool IsInvalidSocket(SOCKET s) { return s == INVALID_SOCKET; }
		static int Bind(SOCKET s, SocketConfig cfg) { return bind(s, (SOCKADDR*)&cfg, sizeof(cfg)); }
	};

	#define Max_LAN_Packet_Size  1500;		//链路层以太网协议的MTU(最大网络传输单元)为1500字节（局域网下能发送的最大字节）
	#define Max_TCP_LAN_Packet_Size	1460	//局域网下TCP最大数据字节(20tcp包头，20IP包头)
	#define Max_UDP_LAN_Packet_Size	1472	//局域网下UDP最大数据字节(20tcp包头，20IP包头)

	#define Max_Network_Packet_Size  576;	//各级路由器上的MTU最大值是576字节（广域网下能发送的最大字节）
	#define Max_TCP_Network_Packet_Size 536	//广域网下TCP传输数据最大长度
	#define Max_UDP_Network_Packet_Size 548	//广域网下UDP传输数据最大长度

	#define Max_Suit_Packed_Size 536		//包数据长度的合适值，适配所有协议包长
	#define Packed_Flag 0					//0,MSG_PEEK,MSG_OOB::读取数据后不缓存.一直缓存.不清楚？
	struct SocketPacket
	{
	public:
		char buf[Max_Suit_Packed_Size] = { 0 };//默认为TCP最大传输长度
		unsigned int bufLen = Max_Suit_Packed_Size;
		SocketPacket() {}
		virtual ~SocketPacket() { }
		void PushDataToBuffer(const char* source,int size)
		{
			bufLen = size;
			memcpy(buf, source, bufLen);
		}

		//TCP收发数据的接口
		int Send(SOCKET server) { return send(server, buf, bufLen, Packed_Flag); }
		int Recive(SOCKET server) { return recv(server, buf, bufLen, Packed_Flag); }

		//UDP收发数据的接口

		int SendTo(SOCKET server, SocketConfig* client, int cfgSize) { return sendto(server, buf, bufLen, Packed_Flag, (sockaddr*)client, cfgSize); }
		int ReciveFrom(SOCKET server, OUT SocketConfig* client, OUT int* clientDataLen) { return recvfrom(server, buf, bufLen, Packed_Flag, (sockaddr*)client, clientDataLen); }
	};
	
	class I_SocketServer
	{
	public:
		virtual int ReciveFromClient() = 0;
		virtual int SendToClient() = 0;
	};
	class I_SocketClient
	{
	public:
		virtual int ReciveFromServer() = 0;
		virtual int SendToServer() = 0;
	};
}

namespace TCP_Win32
{
	using namespace Socket_Win32_Ult;
	
	//TCP的收发接口没有测试
	class TCP_Packet : protected SocketPacket
	{
	protected:
		SocketConfig client;
		int clientSize;
		SocketConfig* clientPtr;

	public:
		TCP_Packet(SocketConfig client);
	};

	//TCP服务器
	class TCP_Service :private TCP_Packet, I_SocketServer
	{
	protected:
		SOCKET socketHandle;
		SocketConfig serverCfg;
	public:
		TCP_Service(SocketConfig server, SocketConfig client):TCP_Packet(client)
		{
			socketHandle = Socket_Win32::GetSocketInstance(SocketIpprotoType::TCP);
			serverCfg = server;
			Socket_Win32::Bind(socketHandle, serverCfg);
		}

		int ReciveFromClient() override
		{
			TCP_Packet::Recive(socketHandle);
			return 1;
		}
		int SendToClient() override
		{
			TCP_Packet::SendTo(socketHandle, TCP_Packet::clientPtr, TCP_Packet::clientSize);
			return 1;
		}
	};

	//TCP客户端
	class TCP_Client :private TCP_Packet, I_SocketClient
	{		
	protected:
		SOCKET socketHandle;
		SocketConfig clientCfg;;
	public:
		TCP_Client(SocketConfig client, SocketConfig server) :TCP_Packet(server)
		{
			socketHandle = Socket_Win32::GetSocketInstance(SocketIpprotoType::TCP);
			clientCfg = client;
		}
		int ReciveFromServer()override 
		{
			TCP_Packet::Recive(socketHandle);
			return 1;
		}
		int SendToServer()override
		{
			TCP_Packet::SendTo(socketHandle, TCP_Packet::clientPtr, TCP_Packet::clientSize);
			return 1;
		}
	};
}

//UDP C/S对象
namespace UDP_Win32
{
	using namespace Socket_Win32_Ult;
	//TCP的收发接口没有测试
	class UDP_Packet :protected SocketPacket {};

	class I_UDP_Prroto:UDP_Packet
	{
	protected:
		SOCKET socketHandle;
		I_UDP_Prroto() { socketHandle = Socket_Win32::GetSocketInstance(SocketIpprotoType::UDP); }
		virtual ~I_UDP_Prroto() { }
	public:
		virtual int CloseSocket() { return Socket_Win32::CloseSocket(socketHandle); }
		virtual int ReciveFrom(SocketConfig* client, int* size)
		{
			int code = UDP_Packet::ReciveFrom(socketHandle, client, size);
			return code;
		}
		virtual int Send(const char* source,int size, SocketConfig* clientOrServerCfg, int cfgSize)
		{
			UDP_Packet::PushDataToBuffer(source, size);
			int code = UDP_Packet::SendTo(socketHandle, clientOrServerCfg, cfgSize);
			return code;
		}
		virtual char* PopDataFromBuffer()
		{
		/*	char* res = new char;
			memcpy(res, buf, bufLen);*/
			return buf;
		}
	};

	//UDP协议服务器
	class UDP_Service : public I_UDP_Prroto
	{
	protected:
		SocketConfig cfg;
	public:
		UDP_Service(SocketConfig serverCfg) : I_UDP_Prroto()
		{
			cfg = serverCfg;
			int code = Socket_Win32::Bind(socketHandle, cfg);
			if (SOCKET_ERROR == code)
				throw "初始化服务器出错,请调用Socket_Win32_Ult::SocketLib_Win32::StartUp();";
		}

	};

	//UDP协议客户端
	class UDP_Client :public I_UDP_Prroto
	{
	protected:
		SOCKET socketHandle;
		SocketConfig cfg;
	public:
		UDP_Client(SocketConfig clientCfg) :I_UDP_Prroto()
		{
			int code = socketHandle = Socket_Win32::GetSocketInstance(SocketIpprotoType::UDP);
			cfg = clientCfg;
		}

	};
}










//class Socket_Win32_Factory
//{
//protected:
//	unsigned short size = 0;
//	SocketLib_Win32 lib;
//	std::vector<SOCKET>  sockets = std::vector<SOCKET>(lib.MaxSocketConnectSize());
//public:
//	Socket_Win32_Factory(){}
//	SOCKET GetInstance()
//	{
//		SOCKET res = Socket_Win32::GetSocketServerInstance();
//		sockets.push_back(res);
//		return res;
//	}
//	 
//	int SetSerivcceConfig(SOCKET s, SocketConfig cfg) { return Socket_Win32::Bind(s, cfg); }
//};
//

