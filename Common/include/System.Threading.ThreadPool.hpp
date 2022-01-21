/*
 * @Description: 线程池
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
 * @brief 表示在调用 RegisterWaitForSingleObject 时已注册的句柄。 此类不能被继承。
 */
class System::Threading::RegisteredWaitHandle
{
friend class ThreadPool;
private:
    HANDLE m_hWait;
    RegisteredWaitHandle(){}
public:
    /**
     * @brief 取消由 RegisterWaitForSingleObject(WaitHandle, WaitOrTimerCallback, Object, UInt32, Boolean) 方法发出的已注册等待操作。
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
     * @brief 获取迄今为止已处理的工作项数
     * 
     * @return int 
     */
    inline long CompletedWorkItemCount()
    {
        return m_nCompletedWorkCount;
    }
    /**
     * @brief 该接口一般不用调用 , 线程池释放权交给操作系统 
     * 
     */
    inline void Dispose() override
    {
        Dispose(true);
    }
    /**
     * @brief 获取当前已加入处理队列的工作项数
     * 
     * @return long 
     */
    inline long PendingWorkItemCount()
    {
        return m_nPedingWorkCount;
    }
    /**
     * @brief 获取当前存在的线程池线程数
     * 
     * @return int 
     */
    inline int ThreadCount()
    {
        throw "Not Support";
    }
    /**
     * @brief 设置最大线程数
     * 
     * @param workerThreads 
     * @param completionPortThreads 无意义
     * @return true 
     * @return false 
     */
    bool SetMaxThreads(int workerThreads, int completionPortThreads)
    {
        SetThreadpoolThreadMaximum(pool, workerThreads);/* 500 */
        return true;
    }
    /**
     * @brief 设置最小线程数
     * 
     * @param workerThreads 
     * @param completionPortThreads 无意义
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
     * @brief 将方法排入队列以便执行。 此方法在有线程池线程变得可用时执行
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
     * @brief 将方法排入队列以便执行。 此方法在有线程池线程变得可用时执行
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
         * WT_EXECUTEINPERSISTENTTHREAD 该线程不会释放。SetMaxThreads(1, _) 会导致整个线程池阻塞。
         * WT_EXECUTELONGFUNCTION 长时间运行的任务
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
     * @brief 收到信号或到达指定时间间隔为止, 执行回调函数。 若未收到信号, 可重置超时时间多次执行任务
     *  https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-registerwaitforsingleobject
     * 注意 : 频繁的事件触发或超时会存在丢包的问题
     * @param signal 触发信号
     * @param cb 超时或 signal 触发时的回调函数
     * @param args 回调函数的参数
     * @param millisecondsTimeOutInterval 超时时间, -1 永久等待 Signal 触发, 0 立刻触发回调
     * @param executeOnlyOnce true : 若 signal 未触发, timeout 后不会再等待; false : 若 signal 未触发, timeout 后会重置等待时间重新等待
     * @return RegisteredWaitHandle* 注册任务的 WaitHandle
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
