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
	enum  class  SocketIpprotoType
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
		static SOCKET GetTcpInstance() { return socket(AF_INET, SOCK_STREAM, (int)SocketIpprotoType::TCP); }
		static SOCKET GetUdpInstance() 
		{
			SOCKET res = socket(AF_INET, SOCK_DGRAM, (int)SocketIpprotoType::UDP); 
			return res;
		}
		static int CloseSocket(SOCKET s) { return closesocket(s); }	//AF_INET, SOCK_STREAM, 0
		//判断SOCKET是否是错误的
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
		int Recive(SOCKET server) { 
			return recv(server, buf, bufLen, Packed_Flag); }

		////UDP收发数据的接口

		//int SendTo(SOCKET server, SocketConfig* client, int cfgSize) { return sendto(server, buf, bufLen, Packed_Flag, (sockaddr*)client, cfgSize); }
		//int ReciveFrom(SOCKET msgFrom, OUT SocketConfig* msgToCfg, OUT int* clientDataLen) { return recvfrom(msgFrom, buf, bufLen, Packed_Flag, (sockaddr*)msgToCfg, clientDataLen); }
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

	class I_Prroto
	{
	protected:
		SocketConfig cfg;
		unsigned int structSize = 0;
		SOCKET socketHandle = NULL;
		I_Prroto() {}
	public:
		I_Prroto(SocketConfig cfg, SOCKET socketHandle = NULL)
		{
			this->cfg = cfg;
			structSize = sizeof(cfg);
			this->socketHandle = socketHandle;
		}
		virtual int CloseSocket() { return Socket_Win32::CloseSocket(socketHandle); }
		virtual SOCKET GetSocketHandle() { return socketHandle; }
		//virtual SocketConfig GetSocketConfig() { return cfg; }
		virtual unsigned int ConfigStructSize() { return structSize; }
		operator SocketConfig()  const     // 隐式转换函数
		{
			return cfg;
		}

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
	
	class I_TCP_Prroto :protected TCP_Packet, public I_Prroto
	{
	protected:
		SOCKET  new_sock = NULL;
		I_TCP_Prroto(SocketConfig client) :TCP_Packet(client){ socketHandle = Socket_Win32::GetTcpInstance(); }
		virtual ~I_TCP_Prroto() { }
	public:
		//收消息的一方需要连接
		int Connect()
		{
			int code = connect(socketHandle, (struct sockaddr*) & client, sizeof(client));
			if (SOCKET_ERROR == code)
				throw "与指定客户端建立连接失败，请检查SocketConfig";
			return code;
		}
		//发消息的一方需要监听
		int CreateListen()
		{
			int code = listen(socketHandle, 5);	//最大延迟5
			if (SOCKET_ERROR == code)
				throw "监听服务器端口失败，请检查SocketConfig";
			int dummy;
			if (new_sock == NULL)
				new_sock = accept(socketHandle, NULL, &dummy);
			else
				throw "未完成的模块";
			return code;
		}

		int CloseSocket() override { return I_Prroto::CloseSocket() && Socket_Win32::CloseSocket(new_sock); }
		virtual int Recive()
		{
			int code = TCP_Packet::Recive(socketHandle);
			return code;
		}
		virtual int Send(const char* source, int size)
		{
			PushDataToBuffer(source, size);
			int code = TCP_Packet::Send(socketHandle);
			return code;
		}
		virtual char* PopDataFromBuffer()
		{
			/*	char* res = new char;
				memcpy(res, buf, bufLen);*/
			return buf;
		}
	};
	//TCP服务器
	class TCP_Service : public I_TCP_Prroto
	{
	protected:
		SocketConfig serverCfg;
	public:
		TCP_Service(SocketConfig server, SocketConfig client):I_TCP_Prroto(client)
		{
			serverCfg = server;
			int code = Socket_Win32::Bind(socketHandle, serverCfg);
			if (SOCKET_ERROR == code)
				throw "初始化服务器失败，请检查SocketConfig";
		}
	};

	//TCP客户端
	class TCP_Client :public I_TCP_Prroto
	{		
	protected:
		SocketConfig clientCfg;;
	public:
		TCP_Client(SocketConfig client, SocketConfig server) :I_TCP_Prroto(server)
		{
			clientCfg = client;
			socketHandle = Socket_Win32::GetTcpInstance();
			
		}
		//在发送消息之前需要初始化
	};
}

//UDP C/S对象
namespace UDP_Win32
{
	using namespace Socket_Win32_Ult;
	//TCP的收发接口没有测试
	class UDP_Packet :protected SocketPacket {};

	class I_UDP_Prroto :protected UDP_Packet, public I_Prroto
	{
	protected:
		I_UDP_Prroto() :UDP_Packet()
		{
			socketHandle = Socket_Win32::GetUdpInstance();
			if (Socket_Win32::IsInvalidSocket(socketHandle)) throw "";
		}
		I_UDP_Prroto(SocketConfig socketDef) :I_UDP_Prroto() { cfg = socketDef; structSize = sizeof(cfg); }
		virtual ~I_UDP_Prroto() { }
	public:
		//向指定服务器发送消息(客户端的接口)
		virtual int SendTo(const char* source,int size, I_Prroto ser)
		{
			//UDP_Packet::PushDataToBuffer(source, size);
			return sendto(socketHandle, source, size, Packed_Flag, (sockaddr*)&ser, ser.ConfigStructSize());
		}
<<<<<<< Updated upstream
		virtual int SendTo(const char* source,int size, SocketConfig* clientOrServerCfg, int cfgSize)
=======
		virtual int RecvFrom(SocketConfig client, int size)
>>>>>>> Stashed changes
		{
			return recvfrom(socketHandle, buf, bufLen, Packed_Flag, (struct sockaddr*) & client, (int*)size);
		}
		virtual char* PopDataFromBuffer()
		{
			/*	char* res = new char;
				memcpy(res, buf, bufLen);*/
			return buf;
		}
	};

	//UDP协议服务器
	typedef int (*Proc_Fun)(Socket_Win32_Ult::SocketPacket* pack);
	typedef struct LoopParam { I_Prroto client; Proc_Fun func; }Params;
	class UDP_Service : public I_UDP_Prroto
	{
	protected:
		bool isRunning;
	public:
		UDP_Service(SocketConfig serverCfg) : I_UDP_Prroto(serverCfg)
		{
			isRunning = true;
			int code = Socket_Win32::Bind(socketHandle, cfg);
			if (SOCKET_ERROR == code)
				throw "初始化服务器出错,请调用Socket_Win32_Ult::SocketLib_Win32::StartUp();";
		}
		int LoopProcess();
	};

	//UDP协议客户端,可以发送数据，接收数据？
	class UDP_Client :public I_UDP_Prroto
	{
<<<<<<< Updated upstream
	protected:
		SocketConfig cfg;
	public:
		UDP_Client(SocketConfig clientCfg) :I_UDP_Prroto() {cfg = clientCfg;}
=======
	public:
		UDP_Client() :I_UDP_Prroto() {}
>>>>>>> Stashed changes
	};
}
