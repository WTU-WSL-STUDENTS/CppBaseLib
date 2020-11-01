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
	enum  class  SocketIpprotoType
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
		static SOCKET GetTcpInstance() { return socket(AF_INET, SOCK_STREAM, (int)SocketIpprotoType::TCP); }
		static SOCKET GetUdpInstance() 
		{
			SOCKET res = socket(AF_INET, SOCK_DGRAM, (int)SocketIpprotoType::UDP); 
			return res;
		}
		static int CloseSocket(SOCKET s) { return closesocket(s); }	//AF_INET, SOCK_STREAM, 0
		//�ж�SOCKET�Ƿ��Ǵ����
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
		int Recive(SOCKET server) { 
			return recv(server, buf, bufLen, Packed_Flag); }

		////UDP�շ����ݵĽӿ�

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
		operator SocketConfig()  const     // ��ʽת������
		{
			return cfg;
		}

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
	
	class I_TCP_Prroto :protected TCP_Packet, public I_Prroto
	{
	protected:
		SOCKET  new_sock = NULL;
		I_TCP_Prroto(SocketConfig client) :TCP_Packet(client){ socketHandle = Socket_Win32::GetTcpInstance(); }
		virtual ~I_TCP_Prroto() { }
	public:
		//����Ϣ��һ����Ҫ����
		int Connect()
		{
			int code = connect(socketHandle, (struct sockaddr*) & client, sizeof(client));
			if (SOCKET_ERROR == code)
				throw "��ָ���ͻ��˽�������ʧ�ܣ�����SocketConfig";
			return code;
		}
		//����Ϣ��һ����Ҫ����
		int CreateListen()
		{
			int code = listen(socketHandle, 5);	//����ӳ�5
			if (SOCKET_ERROR == code)
				throw "�����������˿�ʧ�ܣ�����SocketConfig";
			int dummy;
			if (new_sock == NULL)
				new_sock = accept(socketHandle, NULL, &dummy);
			else
				throw "δ��ɵ�ģ��";
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
	//TCP������
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
				throw "��ʼ��������ʧ�ܣ�����SocketConfig";
		}
	};

	//TCP�ͻ���
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
		//�ڷ�����Ϣ֮ǰ��Ҫ��ʼ��
	};
}

//UDP C/S����
namespace UDP_Win32
{
	using namespace Socket_Win32_Ult;
	//TCP���շ��ӿ�û�в���
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
		//��ָ��������������Ϣ(�ͻ��˵Ľӿ�)
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

	//UDPЭ�������
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
				throw "��ʼ������������,�����Socket_Win32_Ult::SocketLib_Win32::StartUp();";
		}
		int LoopProcess();
	};

	//UDPЭ��ͻ���,���Է������ݣ��������ݣ�
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
