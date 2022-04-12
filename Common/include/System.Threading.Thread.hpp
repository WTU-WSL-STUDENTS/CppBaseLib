#ifndef THREADING_H
#define THREADING_H

#include <System.Threading.WaitHandle.hpp>
#include <System.Action.hpp>
#include <vector>

using ThreadStart = PTHREAD_START_ROUTINE; 
namespace System::Threading
{
    enum ThreadPriority;
    enum ThreadState;
    class Thread;
};
/** 指定 Thread 的调度优先级
 * https://docs.microsoft.com/en-us/windows/win32/procthread/scheduling-priorities
*/
enum System::Threading::ThreadPriority
{
    Lowest = THREAD_PRIORITY_LOWEST,
    BelowNormal = THREAD_PRIORITY_BELOW_NORMAL,
    Normal = THREAD_PRIORITY_NORMAL,
    AboveNormal = THREAD_PRIORITY_ABOVE_NORMAL,
    Highest = THREAD_PRIORITY_HIGHEST,
};
enum System::Threading::ThreadState
{
    /**
     * @brief 线程已启动且尚未停止
     * 
     */
    Running = 0,
    /**
     * @brief 正在请求线程停止。 这仅用于内部
     * 
     */
    StopRequested = DECLARE_ENUM_FLAG(0),
    /**
     * @brief 正在请求线程挂起
     * 
     */
    SuspendRequested = DECLARE_ENUM_FLAG(1),
    /**
     * @brief 线程正作为后台线程执行（相对于前台线程而言）。 此状态可以通过设置 IsBackground 属性来控制
     * 
     */
    Background = DECLARE_ENUM_FLAG(2),
    /**
     * @brief 尚未对线程调用 Start() 方法
     * 
     */
    Unstarted = DECLARE_ENUM_FLAG(3),
    /**
     * @brief 线程已停止
     * 
     */
    Stopped = DECLARE_ENUM_FLAG(4),
    /**
     * @brief 线程处于等待列队 这可能是调用 Sleep(Int32) 或 Join()、请求锁定（例如通过调用 Enter(Object) 或 Wait(Object, Int32, Boolean)）或在线程同步对象上（例如 ManualResetEvent）等待的结果
     * 
     */
    WaitSleepJoin = DECLARE_ENUM_FLAG(5),
    /**
     * @brief 线程已挂起
     * 
     */
    Suspended = DECLARE_ENUM_FLAG(6),
    /**
     * @brief 已对线程调用了 Abort(Object) 方法，但线程尚未收到试图终止它的挂起的 ThreadAbortException
     * 
     */
    AbortRequested = DECLARE_ENUM_FLAG(7),
    /**
     * @brief 线程状态包括 AbortRequested 并且该线程现在已死，但其状态尚未更改为 Stopped
     * 
     */
    Aborted = DECLARE_ENUM_FLAG(8)
};
/**
 * @brief 
 * 1. 默认情况下每个线程会分配 1M 的栈内存, 即默认情况下最多创建 2048 个线程
 * 2. your application will have better performance if you create one thread per processor and build queues of requests for which the application maintains the context information. (ThreadPool 的应用场景)
 */
class System::Threading::Thread final: public System::Threading::WaitHandle
{
private:
    DWORD m_nThreadId;
    int m_nStatus;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Thread)
    Thread(ThreadStart func, void* args = NULL) : m_nStatus(Unstarted)
    {     
        WINAPI_ASSERT(m_hWaitHandle = CreateThread(NULL, 0, func, args, CREATE_SUSPENDED, &m_nThreadId), "https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread");
    }
    /**
     * @brief 如果此线程已启动并且尚未正常终止或中止，则为 true；否则为 false
     * @return true 
     * @return false 
     */
    bool IsAlive()
    {
        VALRET_ASSERT(Running == m_nStatus, false);
        return STILL_ACTIVE == GetExitCode();
    }
    /**
     * @brief 获取线程的返回值
     * 1. 正常执行结束, 返回函数的 return 值
     * 2. 调用 Abort 强制结束, 返回值由 Abort 定义
     * 3. 正在执行, 返回 STILL_ACTIVE
     * @return DWORD 
     */
    DWORD GetExitCode()
    {
        DWORD nRet;
        WINAPI_ASSERT(GetExitCodeThread(m_hWaitHandle, &nRet), "GetExitCode failed");
        return nRet;
    }
    /**
     * @brief 设置线程在 cpu 的那几个 Kernel 中运行 
     * 
     * @param cpuIndex 
     */
    void SetThreadAffinity(const std::initializer_list<unsigned char>& cpuIndex)
    {
        DWORD nMask = 0;
        for(auto ptr = cpuIndex.begin(); ptr != cpuIndex.end(); ++ptr)
        {
            nMask |= (1 << *ptr);
        }
        WINAPI_ASSERT(SetThreadAffinityMask(m_hWaitHandle, nMask), "https://docs.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setthreadaffinitymask");
    }
    /**
     * @brief 获取线程优先级
     * 
     * @return ThreadPriority 
     */
    ThreadPriority GetPriority()
    {
        int nRet;
        WINAPI_ASSERT(THREAD_PRIORITY_ERROR_RETURN != (nRet = GetThreadPriority(m_hWaitHandle)), "https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getthreadpriority");
        if(ThreadPriority::Lowest > nRet)
        {
            return ThreadPriority::Lowest;
        }
        else if(ThreadPriority::Highest < nRet)
        {
            return ThreadPriority::Highest;
        }
        return (ThreadPriority)nRet;
    }
    /**
     * @brief 设置线程优先级
     * 
     * @param priority 
     */
    void SetPriority(ThreadPriority priority)
    {
        WINAPI_ASSERT(SetThreadPriority(m_hWaitHandle, (int)priority), "https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadpriority");
    }
    /**
     * @brief 获取线程的名称
     * 
     * @return WCHAR* 需要调用 LocalFree 释放分配的内存
     */
    WCHAR* GetUnicodeName()
    {
        WCHAR* str = NULL;
        WINAPI_ASSERT(SUCCEEDED(GetThreadDescription(m_hWaitHandle, &str)), "GetUnicodeName failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getthreaddescription");
        return str;
    }
    /**
     * @brief 设置线程的名称
     * 
     * @param str 
     */
    void SetUnicodeName(const WCHAR* str)
    {
        WINAPI_ASSERT(SUCCEEDED(SetThreadDescription(m_hWaitHandle, str)), "SetUnicodeName failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreaddescription");
    }
    /**
     * @brief 获取线程唯一标识符
     * 
     * @return DWORD 
     */
    DWORD GetThreadId()
    {
        return m_nThreadId;
    }
    /**
     * @brief 获取当前线程的标识符
     * 
     * @return DWORD 
     */
    static inline DWORD GetCurrentThreadId()
    {
        return ::GetCurrentThreadId();
    }
    /**
     * @brief 获取线程的各种上下文的信息, 线程必须处于非运行状态
     * 
     * @param context 
     * @return true 
     * @return false 
     */
    bool TryGetExecutionContext(
#if defined(MSVC_64)
        WOW64_CONTEXT&context)
#elif defined(MSVC_32) 
        CONTEXT& context)
#endif
    {
        VALRET_ASSERT(m_hWaitHandle != GetCurrentThread() && !(Running & m_nStatus), false);
        bool bRet = false;
#if defined(MSVC_64)
        WINAPI_ASSERT(bRet = Wow64GetThreadContext(m_hWaitHandle, &context), "TryGetExecutionContext failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getthreadcontext");
#elif defined(MSVC_32)
        WINAPI_ASSERT(bRet = GetThreadContext(m_hWaitHandle, &context), "TryGetExecutionContext failed");
#endif
        return bRet;
    }
    /**
     * @brief 调用该接口该线程将不会执行任何 user-mode code 并立刻释放线程的 Stack 实现强制终止线程的执行的功能.
     * 该接口存在以下风险  
     * 1. 线程在占有同步对象时不会释放使用权
     * 2. Stack 存在指向 Heap 的指针对象, Heap 内存不会释放
     * 3. 线程在执行 kernel32 calls 时终止, 会导致 线程所在内核的 kernel32 state 状态与实际不一致. 
     * 4. 线程如果正在调用 dll 的全局变量, 线程终止时会释放 dll
     */
    void Abort(DWORD nExitCode = -1)
    {
        WINAPI_ASSERT(TerminateThread(m_hWaitHandle, nExitCode), "Abort thread failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminatethread");
    }
    /**
     * @brief 使线程得以按计划执行
     * 
     */
    void Start()
    {
        VOIDRET_ASSERT((Suspended | Unstarted) & m_nStatus);
        DWORD nRet;
        WINAPI_ASSERT(-1 != (nRet = ResumeThread(m_hWaitHandle)), "Start failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-resumethread");
        ERROR_ASSERT(1 == nRet, "suspend count != 1, call start error");
        m_nStatus = Running;
    }
    /**
     * @brief 停止用户态代码 user-mode (application) code 执行，实现线程的挂起. 最好通过同步对象(Monitor, Mutex, Event, Semaphore)实现自定义的 Suspend
     *  1. 该 API 本意是为 debugger 设计
     *  2. 如果线程占有同步对象再调用该接口可能会导致死锁
     */
    void Suspend()
    {
        VOIDRET_ASSERT(Running & m_nStatus);
        DWORD nRet;
        WINAPI_ASSERT(-1 != (nRet = SuspendThread(m_hWaitHandle)), "Suspend failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-suspendthread");
        ERROR_ASSERT(0 == nRet, "suspend count > 0, call suspend too many times !");
        m_nStatus = Suspended;
    }
    /**
     * @brief 获取当前线程所在的处理器编号
     * 
     * @return DWORD 
     */
    static inline DWORD CurrentProcessorNumber()
    {
        /* https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentprocessornumber */
        return ::GetCurrentProcessorNumber();
    }
    /**
     * @brief 获取当前线程句柄, 非线程拥有者请勿 CloseHandle(CurrentThread())
     * 
     * @return HANDLE 
     */
    static inline HANDLE CurrentThread()
    {
        return GetCurrentThread();
    }
    /**
     * @brief 调用线程执行准备好在当前处理器上运行的另一个线程。 由操作系统选择要执行的线程
     * YieldProcessor : 放弃剩余时间片, 重新开始竞争, 优先级较低的线程竞争失败.
     * SwitchToThread : 只要有可调度线程，即便优先级较低，也会让其调度
     * @return true 
     * @return false 
     */
    static inline bool TryYield()
    {
        /* https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-switchtothread */
        return SwitchToThread();
    }
    /**
     * @brief 将当前线程放入等待队列中, 等待 millisecondsTimeout 毫秒后的中断事件, 再放回到就绪列队.
     * sleep(0) : 直接回到就绪队列重新参与 cpu 竞争
     * 
     * @param millisecondsTimeout 
     */
    static inline void Sleep(DWORD millisecondsTimeout)
    {
        ::Sleep(millisecondsTimeout);
    }
};

#endif