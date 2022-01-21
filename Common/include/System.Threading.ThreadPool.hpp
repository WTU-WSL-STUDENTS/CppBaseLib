/*
 * @Description: �̳߳�
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-19 11:09:49
 * @LastEditors: like
 * @LastEditTime: 2022-01-21 17:04:47
 */
#ifndef SYSTEM_THREADING_THREADPOOL_HPP
#define SYSTEM_THREADING_THREADPOOL_HPP

#include <System.Threading.EventWaitHandle.hpp>
#include <functional>

namespace System::Threading
{
    typedef void (*WaitCallback)(Object);               /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.waitcallback?view=net-5.0 */
    typedef void (*WaitOrTimerCallback)(Object, bool);  /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.waitortimercallback?view=net-5.0 */
    class ThreadPool;           /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.threadpool?view=net-5.0 */
    class RegisteredWaitHandle; /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.registeredwaithandle?view=net-5.0 */ 
};
/**
 * @brief ��ʾ�ڵ��� RegisterWaitForSingleObject ʱ��ע��ľ���� ���಻�ܱ��̳С�
 */
class System::Threading::RegisteredWaitHandle
{
friend class ThreadPool;
private:
    HANDLE m_hWait;
    RegisteredWaitHandle(){}
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
class System::Threading::ThreadPool : public IDisposable
{
private:
    static long m_nCompletedWorkCount;
    static long m_nPedingWorkCount;
    static ThreadPool* m_pThreadPool;
    template<typename Func>
    struct _ThreadPoolCallbackArgs
    {
        Func cb;
        Object args;
    };
    template<typename Func>
    static void __stdcall _ThreadPoolCallback(PTP_CALLBACK_INSTANCE Instance,PVOID args, PTP_WORK sender)
    {
        UNREFERENCED_PARAMETER(Instance);
        UNREFERENCED_PARAMETER(sender);
        _ThreadPoolCallbackArgs<Func>* pArgs = (_ThreadPoolCallbackArgs<Func>*)args; 
        pArgs->cb(pArgs->args);
        m_nCompletedWorkCount++;
    }
    TP_CALLBACK_ENVIRON CallBackEnviron;
    PTP_POOL pool;
    PTP_CLEANUP_GROUP group;
    ThreadPool() 
    {
        InitializeThreadpoolEnvironment(&CallBackEnviron);
        if(NULL == (pool = CreateThreadpool(NULL)))
        {
            printf("CreateThreadpool Failed, Error Code %d\n", GetLastError());
            return;
        }
        if(NULL == (group =CreateThreadpoolCleanupGroup()))
        {
            printf("CreateThreadpoolCleanupGroup Failed, Error Code %d\n", GetLastError());
            return;
        }
        SetThreadpoolCallbackPool(&CallBackEnviron, pool);
        SetThreadpoolCallbackCleanupGroup(&CallBackEnviron, group, NULL);
    }
protected:
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
public:
    static ThreadPool Ref()
    {
        return m_pThreadPool ? *m_pThreadPool : *(m_pThreadPool = new ThreadPool());
    }
    /**
     * @brief ��ȡ����Ϊֹ�Ѵ���Ĺ�������
     * 
     * @return int 
     */
    inline long CompletedWorkItemCount()
    {
        return m_nCompletedWorkCount;
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
     * @brief ��ȡ��ǰ�Ѽ��봦����еĹ�������
     * 
     * @return long 
     */
    inline long PendingWorkItemCount()
    {
        return m_nPedingWorkCount;
    }
    /**
     * @brief ��ȡ��ǰ���ڵ��̳߳��߳���
     * 
     * @return int 
     */
    inline int ThreadCount()
    {
        throw "Not Support";
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
     * @return true 
     * @return false 
     */
    inline bool QueueUserWorkItem(WaitCallback cb)
    {
        return QueueUserWorkItem(cb, NULL);
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
        return false;
    } 
    inline bool TrySubmitThreadpoolCallback (PTP_SIMPLE_CALLBACK cb, Object args)
    {
        if(::TrySubmitThreadpoolCallback(cb, args, &CallBackEnviron))
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
long System::Threading::ThreadPool::m_nCompletedWorkCount = 0;
long System::Threading::ThreadPool::m_nPedingWorkCount = 0;
System::Threading::ThreadPool* System::Threading::ThreadPool::m_pThreadPool = NULL;
#endif
