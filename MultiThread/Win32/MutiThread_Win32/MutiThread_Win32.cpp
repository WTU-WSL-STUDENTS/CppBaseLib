#include "pch.h"
#include "MutiThread_Win32.h"


MutiThread_Win32::MutiThread_Win32(std::string threadName)
{
	name = threadName;
	this->pEntry = pEntry;
	this->pParam = pParam;
	nStackSize = 0;
	nInitflag = CREATE_SUSPENDED;
	bAutoClear = true;
}
MutiThread_Win32::MutiThread_Win32(THREAD_ENTRY pEntry, void* pParam):MutiThread_Win32()
{
	if (!InitThread(pEntry, pParam))
		printf("c创建线程失败，可能超出了最大线程数，或实例化HANDLE的时候出错了");
}

bool MutiThread_Win32::InitThread(THREAD_ENTRY pEntry, void* pParam)
{
	this->pEntry = pEntry;
	this->pParam = pParam;
	threadID = ThreadFactory.size() - 1;
	if (threadID < MaxThreadCount)
	{
		thread = (HANDLE)_beginthreadex(0, nStackSize, pEntry, pParam, nInitflag, &threadID);
		return NULL== thread?false:true;
	}
	return false;
}

//MutiThread_Win32* MutiThread_Win32::GetInstance()
//{
//	MutiThread_Win32* res = new MutiThread_Win32();
//	ThreadFactory.push_back(res);
//	return res;
//}
//MutiThread_Win32* MutiThread_Win32::GetInstance(THREAD_ENTRY pEntry, void* pParam) 
//{
//	MutiThread_Win32* res = new MutiThread_Win32(pEntry, pParam);
//	ThreadFactory.push_back(res);
//	return res;
//}
//

MutiThread_Win32* MutiThread_Win32_Factory::GetInstance()
{
	MutiThread_Win32* res = new MutiThread_Win32();
	ThreadFactory.push_back(res);
	return res;
}
MutiThread_Win32* MutiThread_Win32_Factory::GetInstance(THREAD_ENTRY pEntry, void* pParam)
{
	MutiThread_Win32* res = new MutiThread_Win32(pEntry, pParam);
	ThreadFactory.push_back(res);
	return res;
}