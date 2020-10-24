#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS	//����inet_addr()���������Ĵ���
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <vector>

//namespace Socket_Win32

//ws2_32.lib ����win32��װ���Socketͨ��
namespace Socket_Win32_Ult{
	#pragma comment(lib,"ws2_32.lib")

	#define SocketLib_Version(hight, low) MAKEWORD(hight,low)	//����Socket��汾����
	#define DefultSocketLib_Version SocketLib_Version(2,2)		//Ĭ��Ϊ2.2�汾�Ŀ�

	class SocketLib_Win32
	{
	protected:
		static WSADATA wsaData;
	public:
		static int StartUp(WORD version) { return WSAStartup(version, &wsaData); }	//����Socket��
		static int StartUp() { return StartUp(DefultSocketLib_Version); }			//��Ĭ�ϵ�2.2�汾������
		static int Close() { return WSACleanup(); }									//�ر�Socket��
		static int GetLastErro() { return WSAGetLastError(); }						//��ȡSocket��������ֵ�һ�δ�����
		//��ȡ�汾��
		static WORD LibVersion() { return SocketLib_Version(wsaData.wHighVersion, wsaData.wVersion); }			//��ȡ�汾��
		static float LibVersionA() { return (HIBYTE(wsaData.wHighVersion) + 0.1f * LOBYTE(wsaData.wVersion)); }	//��ȡ�汾����ֵ2.2
		//��ȡ���socket������
		static int MaxSocketConnectSize() { return wsaData.iMaxSockets; }
	};

	//Socket����Э�������
	enum class  SocketIpprotoType
	{
		TCP = IPPROTO_TCP,
		UDP = IPPROTO_UDP,
		ICMP,
		IGMP,
		RM
	};

	#define SocketPort_Min 1024	//0-1023:ϵͳ�����˿ں�;49252-65535:��̬�˿ں�
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

	#define Max_LAN_Packet_Size  1500;		//��·����̫��Э���MTU(������紫�䵥Ԫ)Ϊ1500�ֽڣ����������ܷ��͵�����ֽڣ�
	#define Max_TCP_LAN_Packet_Size	1460	//��������TCP��������ֽ�(20tcp��ͷ��20IP��ͷ)
	#define Max_UDP_LAN_Packet_Size	1472	//��������UDP��������ֽ�(20tcp��ͷ��20IP��ͷ)

	#define Max_Network_Packet_Size  576;	//����·�����ϵ�MTU���ֵ��576�ֽڣ����������ܷ��͵�����ֽڣ�
	#define Max_TCP_Network_Packet_Size 536	//��������TCP����������󳤶�
	#define Max_UDP_Network_Packet_Size 548	//��������UDP����������󳤶�

	#define Max_Suit_Packed_Size 536		//�����ݳ��ȵĺ���ֵ����������Э�����
	#define Packed_Flag 0					//0,MSG_PEEK,MSG_OOB::��ȡ���ݺ󲻻���.һֱ����.�������
	struct SocketPacket
	{
	public:
		char buf[Max_Suit_Packed_Size] = { 0 };//Ĭ��ΪTCP����䳤��
		unsigned int bufLen = Max_Suit_Packed_Size;
		SocketPacket() {}
		virtual ~SocketPacket() { }
		void PushDataToBuffer(const char* source,int size)
		{
			bufLen = size;
			memcpy(buf, source, bufLen);
		}

		//TCP�շ����ݵĽӿ�
		int Send(SOCKET server) { return send(server, buf, bufLen, Packed_Flag); }
		int Recive(SOCKET server) { return recv(server, buf, bufLen, Packed_Flag); }

		//UDP�շ����ݵĽӿ�

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
	
	//TCP���շ��ӿ�û�в���
	class TCP_Packet : protected SocketPacket
	{
	protected:
		SocketConfig client;
		int clientSize;
		SocketConfig* clientPtr;

	public:
		TCP_Packet(SocketConfig client);
	};

	//TCP������
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

	//TCP�ͻ���
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

//UDP C/S����
namespace UDP_Win32
{
	using namespace Socket_Win32_Ult;
	//TCP���շ��ӿ�û�в���
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

	//UDPЭ�������
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
				throw "��ʼ������������,�����Socket_Win32_Ult::SocketLib_Win32::StartUp();";
		}

	};

	//UDPЭ��ͻ���
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

