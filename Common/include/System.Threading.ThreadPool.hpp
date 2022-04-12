/*
 * @Description: �̳߳�
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-19 11:09:49
 * @LastEditors: like
 * @LastEditTime: 2022-04-12 13:54:06
 */
#ifndef SYSTEM_THREADING_THREADPOOL_HPP
#define SYSTEM_THREADING_THREADPOOL_HPP
#include <System.Threading.EventWaitHandle.hpp>
#include <System.Singleton.hpp>
#include <functional>

#define TPLAMDA_CONTEXT context
#define DECLARE_TPLAMDA_WORK(codeblock)[](PTP_CALLBACK_INSTANCE Instance,PVOID TPLAMDA_CONTEXT, PTP_WORK Work)->void\
    {                                       \
        UNREFERENCED_PARAMETER(Instance);   \
        UNREFERENCED_PARAMETER(Work);       \
        codeblock                           \
    }
#define DECLARE_TPLAMDA_TIMER(codeblock)[](PTP_CALLBACK_INSTANCE Instance,PVOID TPLAMDA_CONTEXT, PTP_TIMER Timer)->void\
    {                                       \
        UNREFERENCED_PARAMETER(Instance);   \
        UNREFERENCED_PARAMETER(Timer);      \
        codeblock                           \
    }
#define TPLAMDA_WAIT_RESULT waitResult
#define DECLARE_TPLAMDA_WAIT(codeblock)[](PTP_CALLBACK_INSTANCE Instance,PVOID TPLAMDA_CONTEXT, PTP_WAIT Wait, TP_WAIT_RESULT TPLAMDA_WAIT_RESULT)->void\
    {                                       \
        UNREFERENCED_PARAMETER(Instance);   \
        UNREFERENCED_PARAMETER(Wait);       \
        codeblock                           \
    }

namespace System::Threading
{
    typedef DWORD (*WaitCallback)(Object);               /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.waitcallback?view=net-5.0 */
    typedef void (*WaitOrTimerCallback)(Object, bool);  /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.waitortimercallback?view=net-5.0 */
    class ThreadPool;           /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.threadpool?view=net-5.0 */
    class RegisteredWaitHandle; /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.registeredwaithandle?view=net-5.0 */ 
};
/* class which relay on ThreadPool */
namespace System::Threading
{
    class Timer;
};
namespace System::Threading::Tasks
{
    class Task;
};
/**
 * @brief ��ʾ�ڵ��� RegisterWaitForSingleObject ʱ��ע��ľ���� ���಻�ܱ��̳С�
 */
class System::Threading::RegisteredWaitHandle
{
friend class ThreadPool;
private:
    HANDLE m_hWait;
    RegisteredWaitHandle() : m_hWait(NULL){}
public:
    /**
     * @brief ȡ���� RegisterWaitForSingleObject(WaitHandle, WaitOrTimerCallback, Object, UInt32, Boolean) ������������ע��ȴ�������
     * 
     * @param waitHandle 
     * @return true 
     * @return false 
     */
    bool Unregister(WaitHandle waitHandle)
    {
        if(UnregisterWaitEx(m_hWait, waitHandle.Handle()))
        {
            return true;
        }
        printf("UnregisterWait Failed , Error Code %d\n", GetLastError());
        return false;
    }
    bool Unregister()
    {
        if(UnregisterWaitEx(m_hWait, INVALID_HANDLE_VALUE))
        {
            return true;
        }
        printf("UnregisterWait Failed , Error Code %d\n", GetLastError());
        return false;
    }
};
class System::Threading::ThreadPool final : public IDisposable
{
    friend class System::Threading::Tasks::Task;
    friend class System::Threading::Timer;
    friend class SingletonHasDefultConstructPolicy<ThreadPool>;
private:
    TP_CALLBACK_ENVIRON environment;
    PTP_POOL pool;
	PTP_CLEANUP_GROUP group;
	ThreadPool()
	{
		InitializeThreadpoolEnvironment(&environment);
		if (NULL == (pool = CreateThreadpool(NULL)))
		{
			printf("CreateThreadpool Failed, Error Code %d\n", GetLastError());
			return;
		}
		if (NULL == (group = CreateThreadpoolCleanupGroup()))
		{
			printf("CreateThreadpoolCleanupGroup Failed, Error Code %d\n", GetLastError());
			return;
		}
		SetThreadpoolCallbackPool(&environment, pool);
		SetThreadpoolCallbackCleanupGroup(&environment, group, NULL);
	}
public:
    virtual void Dispose(bool disposing)
    {
        if(disposing)
        {
            if(group)
            {
                CloseThreadpoolCleanupGroupMembers(group, false, NULL);
                CloseThreadpoolCleanupGroup(group);
            }
            if(pool)
                CloseThreadpool(pool);
        }
	}
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(ThreadPool)
    ~ThreadPool()
    {
        Dispose();
    }
    /**
     * @brief �ýӿ�һ�㲻�õ��� , �̳߳��ͷ�Ȩ��������ϵͳ 
     * 
     */
    inline void Dispose() override
    {
        Dispose(true);
    }
    /**
     * @brief ��������߳���
     * 
     * @param workerThreads 
     * @param completionPortThreads ������
     * @return true 
     * @return false 
     */
    bool SetMaxThreads(int workerThreads, int completionPortThreads)
    {
        SetThreadpoolThreadMaximum(pool, workerThreads);/* 500 */
        return true;
    }
    /**
     * @brief ������С�߳���
     * 
     * @param workerThreads 
     * @param completionPortThreads ������
     * @return true 
     * @return false 
     */
    bool SetMinThreads(int workerThreads, int completionPortThreads)
    {
        if(SetThreadpoolThreadMinimum(pool, workerThreads))
        {
            return true;
        }
        printf("SetThreadpoolThreadMinimum Failed , Error Code %d\n", GetLastError());
        return false;
    }
    /**
     * @brief ��������������Ա�ִ�С� �˷��������̳߳��̱߳�ÿ���ʱִ��
     * 
     * @param cb 
     * @param args 
     * @return true 
     * @return false 
     */
    inline bool QueueUserWorkItem(WaitCallback cb, Object args)
    {
        /**
         * WT_EXECUTEDEFAULT            
         * WT_EXECUTEINIOTHREAD
         * WT_EXECUTEINPERSISTENTTHREAD ���̲߳����ͷš�SetMaxThreads(1, _) �ᵼ�������̳߳�������
         * WT_EXECUTELONGFUNCTION ��ʱ�����е�����
         * WT_TRANSFER_IMPERSONATION ???
         */
        if(::QueueUserWorkItem((LPTHREAD_START_ROUTINE)cb, args, WT_EXECUTEDEFAULT))
        {
            return true;
        }
        printf("QueueUserWorkItem Failed , Error Code %d\n", GetLastError());
        throw "NotSupportedException";
        return false;
    } 
    inline bool QueueUserWorkItem(WaitCallback cb)
    {
        return QueueUserWorkItem(cb, NULL);
    } 
    
    inline bool TrySubmitThreadpoolCallback(PTP_SIMPLE_CALLBACK cb, Object args)
    {
        if(::TrySubmitThreadpoolCallback(cb, args, &environment))
        {
            return true;
        }
        printf("TrySubmitThreadpoolCallback Failed , Error Code %d\n", GetLastError());
        return false;
    } 
    /**
     * @brief �յ��źŻ򵽴�ָ��ʱ����Ϊֹ, ִ�лص������� ��δ�յ��ź�, �����ó�ʱʱ����ִ������
     *  https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-registerwaitforsingleobject
     * ע�� : Ƶ�����¼�������ʱ����ڶ���������
     * @param signal �����ź�
     * @param cb ��ʱ�� signal ����ʱ�Ļص�����
     * @param args �ص������Ĳ���
     * @param millisecondsTimeOutInterval ��ʱʱ��, -1 ���õȴ� Signal ����, 0 ���̴����ص�
     * @param executeOnlyOnce true : �� signal δ����, timeout �󲻻��ٵȴ�; false : �� signal δ����, timeout ������õȴ�ʱ�����µȴ�
     * @return RegisteredWaitHandle* ע������� WaitHandle
     */
    RegisteredWaitHandle* RegisterWaitForSingleObject(WaitHandle signal, WaitOrTimerCallback cb, Object args, UInt32 millisecondsTimeOutInterval, bool executeOnlyOnce = true)
    {
        RegisteredWaitHandle* rwh = new RegisteredWaitHandle;
        if (false == ::RegisterWaitForSingleObject(&rwh->m_hWait, signal.Handle(), (WAITORTIMERCALLBACKFUNC)cb, args , millisecondsTimeOutInterval, (executeOnlyOnce ? WT_EXECUTEONLYONCE : WT_EXECUTEDEFAULT)))
        {
            printf("RegisterWaitForSingleObject Failed , Error Code %d\n", GetLastError());
        }
        return rwh;
    }
}; 
using ThreadPoolSingleton = Singleton<System::Threading::ThreadPool>;
#endif
