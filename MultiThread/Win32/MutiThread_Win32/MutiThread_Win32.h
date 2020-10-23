#pragma once
#include <string>
#include <process.h>
#include <windows.h>
#include <map>
#include <vector>

#define DLLFLAG __declspec(dllexport)
//#define _CRT_SECURE_NO_WARNINGS	//�����warning C4267: ��return��: �ӡ�size_t��ת������int�������ܶ�ʧ����
#define MaxThreadCount 255							//����߳���
typedef unsigned(__stdcall* THREAD_ENTRY)(void*);	//�߳���Ҫִ�еĺ���ָ��,(THREAD_ENTRY)RunListen
typedef void* HANDLE;
class DLLFLAG MutiThread_Win32
{
friend class MutiThread_Win32_Factory;
protected:
	std::string name;			//�߳�����
	unsigned long nInitflag;	//��ʼ����־
	unsigned long nStackSize;	//�����ռ�ڴ�
	bool bAutoClear;			//�Ƿ��Զ��ͷ��ڴ�
	THREAD_ENTRY pEntry;		//��ں���
	void* pParam;				//����
	unsigned int threadID;		//�߳�id

	HANDLE thread;		//�̶߳���
	MutiThread_Win32(std::string threadName = "");	//��ֹ���Ź��캯���������߳�ֻ��ͨ����̬����
	MutiThread_Win32(THREAD_ENTRY pEntry, void* pParam);	
	bool InitThread(THREAD_ENTRY pEntry, void* pParam);
public:
	//static MutiThread_Win32* GetInstance();
	//static MutiThread_Win32* GetInstance(THREAD_ENTRY pEntry, void* pParam);
	std::string GetThradName() { return name; }
	WORD Run(){ResumeThread(thread);}
	WORD WaitOtherThread() { return WaitForSingleObject(thread, INFINITE); }
	WORD GetReturnCode()
	{
		DWORD dwExitCode;
		GetExitCodeThread(thread, &dwExitCode);
		return dwExitCode;
	}
};

std::vector<MutiThread_Win32*>  ThreadFactory(MaxThreadCount);		//�̼߳���

class DLLFLAG MutiThread_Win32_Factory
{
friend class MutiThread_Win32;

protected:
	MutiThread_Win32_Factory() {};
public:
	static MutiThread_Win32* GetInstance();
	static MutiThread_Win32* GetInstance(THREAD_ENTRY pEntry, void* pParam);
	static void PrintThreadStatus(MutiThread_Win32* t) { printf(StatusCode2String(t->GetReturnCode()).c_str()); }
	static std::string StatusCode2String(WORD code)
	{
		switch (code)
		{
		case STILL_ACTIVE:
			return "STILL_ACTIVE";
		default:
			return "NULL";
		}
	}
private:

};