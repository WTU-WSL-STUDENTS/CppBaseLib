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
/** ָ�� Thread �ĵ������ȼ�
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
     * @brief �߳�����������δֹͣ
     * 
     */
    Running = 0,
    /**
     * @brief ���������߳�ֹͣ�� ��������ڲ�
     * 
     */
    StopRequested = DECLARE_ENUM_FLAG(0),
    /**
     * @brief ���������̹߳���
     * 
     */
    SuspendRequested = DECLARE_ENUM_FLAG(1),
    /**
     * @brief �߳�����Ϊ��̨�߳�ִ�У������ǰ̨�̶߳��ԣ��� ��״̬����ͨ������ IsBackground ����������
     * 
     */
    Background = DECLARE_ENUM_FLAG(2),
    /**
     * @brief ��δ���̵߳��� Start() ����
     * 
     */
    Unstarted = DECLARE_ENUM_FLAG(3),
    /**
     * @brief �߳���ֹͣ
     * 
     */
    Stopped = DECLARE_ENUM_FLAG(4),
    /**
     * @brief �̴߳��ڵȴ��ж� ������ǵ��� Sleep(Int32) �� Join()����������������ͨ������ Enter(Object) �� Wait(Object, Int32, Boolean)�������߳�ͬ�������ϣ����� ManualResetEvent���ȴ��Ľ��
     * 
     */
    WaitSleepJoin = DECLARE_ENUM_FLAG(5),
    /**
     * @brief �߳��ѹ���
     * 
     */
    Suspended = DECLARE_ENUM_FLAG(6),
    /**
     * @brief �Ѷ��̵߳����� Abort(Object) ���������߳���δ�յ���ͼ��ֹ���Ĺ���� ThreadAbortException
     * 
     */
    AbortRequested = DECLARE_ENUM_FLAG(7),
    /**
     * @brief �߳�״̬���� AbortRequested ���Ҹ��߳���������������״̬��δ����Ϊ Stopped
     * 
     */
    Aborted = DECLARE_ENUM_FLAG(8)
};
/**
 * @brief 
 * 1. Ĭ�������ÿ���̻߳���� 1M ��ջ�ڴ�, ��Ĭ���������ഴ�� 2048 ���߳�
 * 2. your application will have better performance if you create one thread per processor and build queues of requests for which the application maintains the context information. (ThreadPool ��Ӧ�ó���)
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
     * @brief ������߳�������������δ������ֹ����ֹ����Ϊ true������Ϊ false
     * @return true 
     * @return false 
     */
    bool IsAlive()
    {
        VALRET_ASSERT(Running == m_nStatus, false);
        return STILL_ACTIVE == GetExitCode();
    }
    /**
     * @brief ��ȡ�̵߳ķ���ֵ
     * 1. ����ִ�н���, ���غ����� return ֵ
     * 2. ���� Abort ǿ�ƽ���, ����ֵ�� Abort ����
     * 3. ����ִ��, ���� STILL_ACTIVE
     * @return DWORD 
     */
    DWORD GetExitCode()
    {
        DWORD nRet;
        WINAPI_ASSERT(GetExitCodeThread(m_hWaitHandle, &nRet), "GetExitCode failed");
        return nRet;
    }
    /**
     * @brief �����߳��� cpu ���Ǽ��� Kernel ������ 
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
     * @brief ��ȡ�߳����ȼ�
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
     * @brief �����߳����ȼ�
     * 
     * @param priority 
     */
    void SetPriority(ThreadPriority priority)
    {
        WINAPI_ASSERT(SetThreadPriority(m_hWaitHandle, (int)priority), "https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreadpriority");
    }
    /**
     * @brief ��ȡ�̵߳�����
     * 
     * @return WCHAR* ��Ҫ���� LocalFree �ͷŷ�����ڴ�
     */
    WCHAR* GetUnicodeName()
    {
        WCHAR* str = NULL;
        WINAPI_ASSERT(SUCCEEDED(GetThreadDescription(m_hWaitHandle, &str)), "GetUnicodeName failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getthreaddescription");
        return str;
    }
    /**
     * @brief �����̵߳�����
     * 
     * @param str 
     */
    void SetUnicodeName(const WCHAR* str)
    {
        WINAPI_ASSERT(SUCCEEDED(SetThreadDescription(m_hWaitHandle, str)), "SetUnicodeName failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-setthreaddescription");
    }
    /**
     * @brief ��ȡ�߳�Ψһ��ʶ��
     * 
     * @return DWORD 
     */
    DWORD GetThreadId()
    {
        return m_nThreadId;
    }
    /**
     * @brief ��ȡ��ǰ�̵߳ı�ʶ��
     * 
     * @return DWORD 
     */
    static inline DWORD GetCurrentThreadId()
    {
        return ::GetCurrentThreadId();
    }
    /**
     * @brief ��ȡ�̵߳ĸ��������ĵ���Ϣ, �̱߳��봦�ڷ�����״̬
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
     * @brief ���øýӿڸ��߳̽�����ִ���κ� user-mode code �������ͷ��̵߳� Stack ʵ��ǿ����ֹ�̵߳�ִ�еĹ���.
     * �ýӿڴ������·���  
     * 1. �߳���ռ��ͬ������ʱ�����ͷ�ʹ��Ȩ
     * 2. Stack ����ָ�� Heap ��ָ�����, Heap �ڴ治���ͷ�
     * 3. �߳���ִ�� kernel32 calls ʱ��ֹ, �ᵼ�� �߳������ں˵� kernel32 state ״̬��ʵ�ʲ�һ��. 
     * 4. �߳�������ڵ��� dll ��ȫ�ֱ���, �߳���ֹʱ���ͷ� dll
     */
    void Abort(DWORD nExitCode = -1)
    {
        WINAPI_ASSERT(TerminateThread(m_hWaitHandle, nExitCode), "Abort thread failed https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminatethread");
    }
    /**
     * @brief ʹ�̵߳��԰��ƻ�ִ��
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
     * @brief ֹͣ�û�̬���� user-mode (application) code ִ�У�ʵ���̵߳Ĺ���. ���ͨ��ͬ������(Monitor, Mutex, Event, Semaphore)ʵ���Զ���� Suspend
     *  1. �� API ������Ϊ debugger ���
     *  2. ����߳�ռ��ͬ�������ٵ��øýӿڿ��ܻᵼ������
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
     * @brief ��ȡ��ǰ�߳����ڵĴ��������
     * 
     * @return DWORD 
     */
    static inline DWORD CurrentProcessorNumber()
    {
        /* https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentprocessornumber */
        return ::GetCurrentProcessorNumber();
    }
    /**
     * @brief ��ȡ��ǰ�߳̾��, ���߳�ӵ�������� CloseHandle(CurrentThread())
     * 
     * @return HANDLE 
     */
    static inline HANDLE CurrentThread()
    {
        return GetCurrentThread();
    }
    /**
     * @brief �����߳�ִ��׼�����ڵ�ǰ�����������е���һ���̡߳� �ɲ���ϵͳѡ��Ҫִ�е��߳�
     * YieldProcessor : ����ʣ��ʱ��Ƭ, ���¿�ʼ����, ���ȼ��ϵ͵��߳̾���ʧ��.
     * SwitchToThread : ֻҪ�пɵ����̣߳��������ȼ��ϵͣ�Ҳ���������
     * @return true 
     * @return false 
     */
    static inline bool TryYield()
    {
        /* https://docs.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-switchtothread */
        return SwitchToThread();
    }
    /**
     * @brief ����ǰ�̷߳���ȴ�������, �ȴ� millisecondsTimeout �������ж��¼�, �ٷŻص������ж�.
     * sleep(0) : ֱ�ӻص������������²��� cpu ����
     * 
     * @param millisecondsTimeout 
     */
    static inline void Sleep(DWORD millisecondsTimeout)
    {
        ::Sleep(millisecondsTimeout);
    }
};

#endif