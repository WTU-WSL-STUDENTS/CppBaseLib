#pragma once
#include <string>
#include <process.h>
#include <windows.h>
#include <map>
#include <vector>

#define DLLFLAG __declspec(dllexport)
//#define _CRT_SECURE_NO_WARNINGS	//解决：warning C4267: “return”: 从“size_t”转换到“int”，可能丢失数据
#define MaxThreadCount 255							//最大线程数
typedef unsigned(__stdcall* THREAD_ENTRY)(void*);	//线程内要执行的函数指针,(THREAD_ENTRY)RunListen
typedef void* HANDLE;
class DLLFLAG MutiThread_Win32
{
friend class MutiThread_Win32_Factory;
protected:
	std::string name;			//线程名称
	unsigned long nInitflag;	//初始化标志
	unsigned long nStackSize;	//分配的占内存
	bool bAutoClear;			//是否自动释放内存
	THREAD_ENTRY pEntry;		//入口函数
	void* pParam;				//参数
	unsigned int threadID;		//线程id

	HANDLE thread;		//线程对象
	MutiThread_Win32(std::string threadName = "");	//禁止开放构造函数，创建线程只能通过静态方法
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

std::vector<MutiThread_Win32*>  ThreadFactory(MaxThreadCount);		//线程集合

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