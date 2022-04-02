// /*
//  * @Description: 
//  * @Version: 1.0
//  * @Autor: like
//  * @Date: 2022-01-17 15:12:58
//  * @LastEditors: like
//  * @LastEditTime: 2022-01-19 16:26:44
//  */
#ifndef SYSTEM_THREADING_TASKS_TASK_HPP
#define SYSTEM_THREADING_TASKS_TASK_HPP
#include <System.Threading.ThreadPool.hpp>
#include <System.Action.hpp>
#include <System.List.hpp>
#include <System.Threading.EventWaitHandle.hpp>
#include <System.Threading.Interlocked.hpp>
#include <System.Threading.Task.Parallel.hpp>
#include <exception>

// #include <functional>
// #include <future>
#include <vector>

namespace System
{
    class IAsyncResult;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.iasyncresult.asyncwaithandle?view=net-5.0 */
};
namespace System::Threading::Tasks
{
    namespace TaskCreation
    {
        enum Options;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.tasks.taskcreationoptions?view=net-5.0 */
	}
	namespace TaskContinuation
	{
		enum Options;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.tasks.taskcontinuationoptions?view=net-5.0 */
	}
	using TaskCreationOptions = TaskCreation::Options;
	using TaskContinuationOptions = TaskContinuation::Options;
    enum class TaskStatus : long;
    class Task;
    // template<typename T>
    // class Task;
    struct ValueTask{};   /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.tasks.valuetask?view=net-5.0 */
};
/**
 * @brief ָ���ɿ�������Ĵ�����ִ�еĿ�ѡ��Ϊ�ı�־
 * 
 */
enum System::Threading::Tasks::TaskCreation::Options
{
    /**
     * @brief ָ��Ӧʹ��Ĭ����Ϊ
     * 
     */
    None = 0,     
    /**
     * @brief ��ʾ TaskScheduler ��һ�־����ܹ�ƽ�ķ�ʽ������������ζ�Ž��簲�ŵ����񽫸����ܽ������У������������е����񽫸����ܽ�������
     * 
     */
    PreferFairness = DECLARE_ENUM_FLAG(0),  
    /**
     * @brief ָ�������ǳ�ʱ�����еġ������ȵĲ������漰��ϸ����ϵͳ���١���������
     * 
     */
    LongRunning = DECLARE_ENUM_FLAG(1),
    /**
     * @brief ָ�������񸽼ӵ������νṹ�е�ĳ�������� Ĭ������£������񣨼����ⲿ���񴴽����ڲ����񣩽��������丸����ִ�С� ����ʹ�� AttachedToParent ѡ���Ա㽫�������������ͬ��
     * 
     */
    AttachedToParent = DECLARE_ENUM_FLAG(2),
    /**
     * @brief ָ���κγ�����Ϊ���ӵ�������ִ�е��������޷����ӵ�������, ʹ������ AttachedToParent ����ʧЧ 
     * 
     */
    DenyChildAttach = DECLARE_ENUM_FLAG(3),
    HideScheduler = DECLARE_ENUM_FLAG(4),
    RunContinuationsAsynchronously = DECLARE_ENUM_FLAG(5),
};
/**
 * @brief ��ʾ Task �����������еĵ�ǰ�׶�
 * 
 */
enum class System::Threading::Tasks::TaskStatus : long
{
    /**
     * @brief ��������ͨ����������� CancellationToken ���� OperationCanceledException ��ȡ��������ȷ�ϣ���ʱ�ñ�Ǵ����ѷ����ź�״̬�������ڸ�����ʼִ��֮ǰ������������ CancellationToken �������źš� �й���ϸ��Ϣ�����������ȡ����
     * 
     */
    Created = 0, 	
    /**
     * @brief ���������ڵȴ��ڲ����伤����мƻ�
     * 
     */
    WaitingForActivation, 	
    /**
     * @brief �������ѱ��ƻ�ִ�У�����δ��ʼִ��
     * 
     */
    WaitingToRun, 
    /**
     * @brief �������������У�����δ���
     * 
     */
    Running, 
    /**
     * @brief �����������ִ�У�������ʽ�ȴ����ӵ����������
     * 
     */
    WaitingForChildrenToComplete, 
    /**
     * @brief �ѳɹ����ִ�е�����
     * 
     */
    RanToCompletion, 
    /**
     * @brief ����ȡ����ԭ�����ɵ�����
     * 
     */
    Canceled, 
    /**
     * @brief ����δ�����쳣��ԭ�����ɵ�����
     * 
     */
    Faulted 
};
/**
 * @brief Ϊ ContinueWith ��������������ָ����Ϊ
 * https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.tasks.taskcontinuationoptions?view=net-5.0
 * 
 */
enum System::Threading::Tasks::TaskContinuation::Options
{
    /**
     * @brief  ����������ǰ���������ɺ����첽��ʽ���У���ǰ���������յ� Status ����ֵ�޹ء� �������Ϊ��������Ὣ�䴴��Ϊ�����Ƕ������
     * 
     */
    None = 0,
    /**
     * @brief ��ʾ TaskScheduler ������ƻ���˳����������˽��簲�ŵ����񽫸����ܽ�������
     * 
     */
    PreferFairness = DECLARE_ENUM_FLAG(0),
    /**
     * @brief ���������ǳ������еġ������ȵĲ���
     * 
     */
    LongRunning     = DECLARE_ENUM_FLAG(1),
    AttachedToParent= DECLARE_ENUM_FLAG(2),
    DenyChildAttach = DECLARE_ENUM_FLAG(3),
    HideScheduler   = DECLARE_ENUM_FLAG(4),
    LazyCancellation= DECLARE_ENUM_FLAG(5),
    RunContinuationsAsynchronously = DECLARE_ENUM_FLAG(6),
    NotOnRanToCompletion    = DECLARE_ENUM_FLAG(16),
    NotOnFaulted            = DECLARE_ENUM_FLAG(17),
    NotOnCanceled           = DECLARE_ENUM_FLAG(18),
    OnlyOnCanceled          = DECLARE_ENUM_FLAG(16) | DECLARE_ENUM_FLAG(17),
    OnlyOnFaulted           = DECLARE_ENUM_FLAG(16) | DECLARE_ENUM_FLAG(18),
    OnlyOnRanToCompletion   = DECLARE_ENUM_FLAG(17) | DECLARE_ENUM_FLAG(18),
    ExecuteSynchronously    = DECLARE_ENUM_FLAG(19)
};
/**
 * @brief Task ����� Context
 * 
 */
typedef System::Object* AsyncState;
/**
 * @brief ���� AsyncState , Context �Ĺ���ֻ������ָ�����
 * 
 * @tparam PFirst 
 * @tparam PRest 
 * @param first 
 * @param rest 
 * @return AsyncState 
 */
template<typename T, typename... ARG>
AsyncState CreateAsyncState(T x, ARG... arg) 
{
	AsyncState as = new void* [sizeof...(arg) + 1];
	AsyncState p = as;
	*p++ = x;
	(void)std::initializer_list<T>
    {
		([&arg, &p] {*p++ = arg; }(), x)...
	};
	return as;
}
/**
 * @brief �ͷ� AsyncState
 * 
 * @param as 
 */
inline void DisposeAsyncState(AsyncState& as)
{
    VOIDRET_ASSERT(as);
    delete[] as;
    as = NULL;
}
/**
 * @brief https://docs.microsoft.com/zh-cn/dotnet/api/system.iasyncresult.iscompleted?view=net-5.0
 * 
 */
class System::IAsyncResult
{
public:
    /**
     * @brief ��ȡһ���û�����Ķ��󣬸ö����޶�������й��첽��������Ϣ
     * 
     * @return void* 
     */
    virtual AsyncState GetAsyncState() = 0;
    /**
     * @brief ��ȡ���ڵȴ��첽������ɵ� WaitHandle
     * 
     * @return WaitHandle 
     */
    virtual System::Threading::WaitHandle GetAsyncWaitHandle() = 0;
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ�첽�����Ƿ������
     * 
     * @return true 
     * @return false 
     */
    virtual bool IsCompleted() = 0;
};

typedef int CreationOptions;
typedef long TaskCurrentStatus;
typedef StackList<WEAK_PTR(System::Threading::Tasks::Task)> AttachedTasks;
class System::Threading::Tasks::Task final : public IAsyncResult//, public IDisposable
{
private:
    PTP_WORK work;
    static void MyWorkCallback(PTP_CALLBACK_INSTANCE Instance, PVOID Parameter, PTP_WORK Work)
    {
        UNREFERENCED_PARAMETER(Instance);
        UNREFERENCED_PARAMETER(Work);
        Task* t = static_cast<Task*>(Parameter);
        long val = (long)TaskStatus::WaitingToRun;
        Interlocked<long>::Exchange(t->m_nStatus, val);
        t->m_nId = GetCurrentThreadId();
        t->RunSynchronously();
        return;
    }

    //AttachedTasks attachedTasks;
    CreationOptions m_nCreationOptions;
    TaskCurrentStatus m_nStatus;
    int m_nId;
    System::Threading::AutoResetEvent m_are;
    /* Task ��Ϊ���� Task ����� */
    System::Action<AsyncState> m_action;
    /* Task ��Ϊ Continue task ����� */
    System::Action<Task&, AsyncState> m_childAct;
    AsyncState m_pAsyncState;
    WEAK_PTR(Task) parrent;

    /**
     * @brief Continue task ���캯��
     * 
     * @param action 
     * @param parrent 
     * @param args 
     */
    Task(Action<Task&, AsyncState> action, Task* parrent, AsyncState args) : 
        m_nCreationOptions(TaskCreationOptions::AttachedToParent), m_nStatus((long)TaskStatus::Created), m_nId(-1), m_are(AutoResetEvent(false)), 
        m_action(NULL), m_childAct(action),
        m_pAsyncState(args), parrent(parrent)
    {
        work = CreateThreadpoolWork((PTP_WORK_CALLBACK)MyWorkCallback, this, &(ThreadPoolSingleton::Ref().environment));
    }
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Task)
    /**
     * @brief ���� Task ���캯��
     * 
     * @param action 
     * @param args �� malloc �� calloc ������ָ�뼯��
     */
    Task(Action<AsyncState> action, AsyncState args = NULL) : 
        m_nCreationOptions(TaskCreationOptions::None), m_nStatus((long)TaskStatus::Created), m_nId(-1), m_are(AutoResetEvent(false)), 
        m_action(action), m_childAct(NULL),
        m_pAsyncState(args), parrent(NULL)
    
    {
        WINAPI_ASSERT(work = CreateThreadpoolWork((PTP_WORK_CALLBACK)MyWorkCallback, this, &(ThreadPoolSingleton::Ref().environment)), "Task construct failed");
    }
    ~Task()
    {
        if (!IsCompleted())
        {
            Wait();
        }
        CloseThreadpoolWork(work);
        work = NULL;
        DisposeAsyncState(m_pAsyncState);
    }
    /**
     * @brief ȡ������, �ڷ� Task ִ���̵߳���, ��ȡ�������ж��е�δִ�е�����. ���øýӿڣ� ��ݹ齫 Canceled ��־λ���������и�����
     * 
     */
    void Cancel()
    {
        if((long)TaskStatus::WaitingToRun > m_nStatus)
        {
            WaitForThreadpoolWorkCallbacks(work, true);/* true : �������Ŷ��еĹ����� */
        }
        Interlocked<long>::Exchange(m_nStatus, (long)TaskStatus::Canceled);
        if(parrent)
        {
            parrent->Cancel();
        }
    }
    /**
     * @brief ��ȡ�ڴ��� Task ʱ�ṩ��״̬�������δ�ṩ����Ϊ NULL
     * 
     * @return AsyncState 
     */
    inline AsyncState GetAsyncState() override
    {
        return m_pAsyncState;
    }
    /**
     * @brief ��ȡ�����ڵȴ�������ɵ� WaitHandle
     * 
     * @return System::Threading::WaitHandle 
     */
    System::Threading::WaitHandle GetAsyncWaitHandle() override
    {
        return m_are;
    }
    /**
     * @brief ��ȡ���ڴ���������� TaskCreationOptions
     * 
     * @return CreationOptions 
     */
    inline CreationOptions GetCreationOptions()
    {
        return m_nCreationOptions;
    } 
    /**
     * @brief ��ȡ�� Task ʵ���� ID
     * 
     * @return int 
     */
    inline int GetId()
    {
        return m_nId;
    }
    /**
     * @brief ��ȡ������� TaskStatus
     * 
     * @return TaskStatus 
     */
    TaskStatus GetStatus()
    {
        return (TaskStatus)m_nStatus;
    }
    /**
     * @brief ��ȡ�� Task ʵ���Ƿ����ڱ�ȡ����ԭ�����ɣ�ֹͣ��ִ��
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCanceled()
    {
        return (long)TaskStatus::Canceled == m_nStatus;
    }
    /**
     * @brief ��ȡһ��ֵ������ʾ�Ƿ����������
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCompleted() override
    {
        return IsCanceled() || IsCompletedSuccessfully() || IsFaulted();
    }
    /**
     * @brief �����Ƿ����е����
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCompletedSuccessfully()
    {
        return (long)TaskStatus::RanToCompletion == m_nStatus;
    }
    /**
     * @brief ��ȡ Task �Ƿ�����δ�������쳣��ԭ������
     * 
     * @return true 
     * @return false 
     */
    inline bool IsFaulted()
    {
        return (long)TaskStatus::Faulted == m_nStatus;
    }
    /**
     * @brief ����һ����Ŀ�� Task ���ʱ�첽ִ�е���������
     * 
     * @param continuationAction 
     * @return Task 
     */
    Task* ContinueWith(Action<Task&/* Parrent task */, AsyncState/* NULL */> continuationAction)
    {
        bool canAttachToParrent = TaskCreationOptions::DenyChildAttach & m_nCreationOptions;
        Task* task;
        if(TaskCreationOptions::DenyChildAttach & m_nCreationOptions)
        {
            task = new Task(continuationAction, NULL, NULL);
            return task;
        }
        task = new Task(continuationAction, this, NULL);
        //attachedTasks.Add(task);
        return task;
    }
    /**
     * @brief �Ե�ǰ�� Task ͬ������ TaskScheduler
     * 
     */
    void RunSynchronously()
    {
        long status = (long)TaskStatus::Running;
        Interlocked<long>::Exchange(m_nStatus, status);
        try
        {
            if(NULL == parrent)
            {
                m_action(m_pAsyncState);
            }
            else
            {
                m_childAct(*parrent, m_pAsyncState);
			}
			VOIDRET_ASSERT
            (
                (long)TaskStatus::Canceled != m_nStatus && 
                (long)TaskStatus::Running == m_nStatus/* ������߳����˳��� �̳߳ػ���ִ�У� ��ô��ʱ Task ��ջ�ͱ��ƻ�����ʱ��Ҫ�����˳� Task */
            );
     
			long status = (long)TaskStatus::WaitingForChildrenToComplete;
            Interlocked<long>::Exchange(m_nStatus, status);
            /* TODO : wait children finished */
           /* for(int i = 0; i < (int)attachedTasks.Count(); i++)
            {
                attachedTasks[i]->Start();
            }
            for(int i = 0; i < (int)attachedTasks.Count(); i++)
            {
                attachedTasks[i]->Wait(-1);
            }*/
            status = (long)TaskStatus::RanToCompletion;
            Interlocked<long>::Exchange(m_nStatus, status);
        }
        catch(const std::exception& e)
        {
			printf("Task catched exception : %s\n", e.what());
			status = (long)TaskStatus::Faulted;
            Interlocked<long>::Exchange(m_nStatus, status);
        }
        m_are.Set();
    }
    /**
     * @brief ���� Task
     * 
     */
    void Start()
    {
        SubmitThreadpoolWork(work);
        long status = (long)TaskStatus::WaitingForActivation;
        Interlocked<long>::Exchange(m_nStatus, status);
    }
    void Start(AsyncState args)
    {
        DisposeAsyncState(m_pAsyncState);
        m_pAsyncState = args;
        Start();
    }
    /**
     * @brief �ȴ� Task ���ִ�й���
     * 
     * @param millisecondsTimeout 
     * @return true 
     * @return false 
     */
    inline bool Wait(DWORD millisecondsTimeout)
    {
        return m_are.WaitOne(millisecondsTimeout);
    }
    inline bool Wait()
    {
        WaitForThreadpoolWorkCallbacks(work, (int)TaskStatus::WaitingForActivation > (int)m_nStatus/* true : �������Ŷ��еĹ����� */);
        return true;
    }
    /**
     * @brief �ȴ��ṩ������ Task �������ִ�й���
     * 
     * @param tasks 
     * @param count 
     * @param millisecondsTimeout 
     * @return true 
     * @return false 
     */
    static bool WaitAll(Task** tasks, int count, DWORD millisecondsTimeout = -1)
    {
        WaitHandle** h = (WaitHandle**)malloc(sizeof(WaitHandle*) * count);
        for(int i = 0 ; i < count ; i++)
        {
            h[i] = &tasks[i]->m_are;
        }
        bool bRet = WaitHandle::WaitAll(h, count, millisecondsTimeout);
        free(h);
        return bRet;
    }
    static bool WaitAll(const std::initializer_list<Task*>& il, DWORD millisecondsTimeout = -1)
    {
        WaitHandle** h = (WaitHandle**)malloc(sizeof(WaitHandle*) * il.size());
        auto ptr = il.begin();
        for(size_t i = 0; i < il.size(); i++, ptr++)
        {
            h[i] = &((*ptr)->m_are);
        }
        bool bRet = WaitHandle::WaitAll(h, (DWORD)il.size(), millisecondsTimeout);
        free(h);
        return bRet;
    }
    /**
     * @brief �ȴ��ṩ����һ Task �������ִ�й���
     * 
     * @param tasks 
     * @param count 
     * @param millisecondsTimeout 
     * @return true 
     * @return false 
     */
    static int WaitAny(Task** tasks, int count, DWORD millisecondsTimeout = -1)
    {
        WaitHandle** h = (WaitHandle**)malloc(sizeof(WaitHandle*) * count);
        for(int i = 0 ; i < count ; i++)
        {
            h[i] = &tasks[i]->m_are;
        }
        int n = WaitHandle::WaitAny(h, count, millisecondsTimeout);
        free(h);
        return n;
    }
    static int WaitAny(const std::initializer_list<Task*>& il, DWORD millisecondsTimeout = -1)
    {
        WaitHandle** h = (WaitHandle**)malloc(sizeof(WaitHandle*) * il.size());
        auto ptr = il.begin();
        for(size_t i = 0; i < il.size(); i++, ptr++)
        {
            h[i] = &((*ptr)->m_are);
        }
        int n = WaitHandle::WaitAny(h, (DWORD)il.size(), millisecondsTimeout);
        free(h);
        return n;
    }
// protected:
//     Action* m_pAct;
//     Action* m_pAct1;
//     void*   m_pArgsOfAction1;
//     CancellationToken m_token;
//     TaskCreationOptions m_eOption;
//     /* ansy �첽���� */
//     int AsyncLunchPolicy[2] = 
//     {
//         std::launch::async | std::launch::deferred,
//         std::launch::async
//     };
//     std::future<void> result;
// public:
//     Task(const Action& action) : 
//         m_pAct(new Action(action)), m_pAct1(NULL), m_pArgsOfAction1(NULL),m_token(CancellationToken(false)), m_eOption(TaskCreationOptions::None){}
//     Task(const Action& action, const CancellationToken& token, TaskCreationOptions option = TaskCreationOptions::None) : 
//         m_pAct(action), m_pAct1(NULL),  m_pArgsOfAction1(NULL), m_token(token), m_eOption(option){}
//     Task(const Action1& action, void* args) : 
//         m_pAct(NULL), m_pAct1(new Action1(action)), m_pArgsOfAction1(args), m_token(CancellationToken(false)), m_eOption(TaskCreationOptions::None){}
//     Task(const Action1& action, const CancellationToken& token, TaskCreationOptions option = TaskCreationOptions::None) : 
//         m_pAct(NULL), m_pAct1(new Action1(action)), m_pArgsOfAction1(args), m_token(token), m_eOption(option){}

//     /**
//      * @brief ���� Task
//      * 
//      */
//     void Start()
//     {
//         std::launch::async | std::launch::deferred
//         if(m_pAct)
//         {
//             result = std::async(AsyncLunchPolicy[m_eOption], *m_pAct);
//         }
//         else
//         {
//             result = std::async(AsyncLunchPolicy[m_eOption], *m_pAct1, m_pArgsOfAction1);
//         }
//     }
// };

// struct System::Threading::Tasks::ValueTask
// {
//     std::async(std::launch::async, fetchDataFromDB, "Data");
};
// namespace System::Threading
// {
//     class CancellationToken;
//     class CancellationTokenSource;
//     class CancellationTokenRegistration;
//     
// };
// class System::Threading::CancellationTokenSource :  public IDisposable , private CancellationToken
// {
// protected:
//     virtual void Dispose(bool disposing)
//     {
//         if(disposing && m_hCts)
//         {
//             CloseHandle(m_hCts);
//         }
//     }
// public:
//     CancellationTokenSource() : m_hCts(CreateEvent(NULL, true, false, NULL)){}
//     /**
//      * @brief ��ȡ��� CancellationTokenSource ������ CancellationToken
//      * 
//      * @return CancellationToken 
//      */
//     inline CancellationToken Token()
//     {
//         return *this;
//     }
//     inline bool IsCancellationRequested()
//     {
//         return m_bCancelRequest;
//     }
//     /**
//      * @brief ����ȡ������
//      * 
//      */
//     inline void Cancel()
//     {
//         SetEvent(m_hCts);
//     }
//     /**
//      * @brief ��ָ���ĺ�������ƻ��Դ� CancellationTokenSource ��ȡ������
//      * 
//      * @param millisecondsDelay 
//      */
//     inline void CancelAfter(int millisecondsDelay)
//     {
//         /* TODO : Delay */
//         SetEvent(m_hCts);
//     }
//     /**
//      * @brief �ͷ� CancellationTokenSource ��ĵ�ǰʵ����ʹ�õ�������Դ
//      * 
//      */
//     void Dispose() override
//     {
//         Dispose(true);
//     }
// };
// class System::Threading::CancellationToken 
// {
// private:
//     HANDLE m_hCts;
//     bool m_bCancelRequest;
//     bool m_bCanceled;
//     std::vector<Action1> m_vecActionQueue;
//     Mutex mtx;
// public:
//     CancellationToken(bool canceled) : m_bCancelRequest(false), m_bCanceled(canceled){}
//     /**
//      * @brief ��ȡ�Ƿ�������ȡ���˱��
//      * 
//      * @return true 
//      * @return false 
//      */
//     bool IsCancellationRequested()
//     {
//         return m_bCancelRequest;
//     }
//     /**
//      * @brief ��ȡ�˱���Ƿ��ܴ�����ȡ��״̬
//      * 
//      * @return true 
//      * @return false 
//      */
//     bool CanBeCanceled()
//     {
//         return m_bCanceled;
//     }
//     /**
//      * @brief ��ȡ��ȡ�����ʱ�յ��źŵ� WaitHandle , ��Ӧֱ�ӹرջ��ͷŸþ��.
//      *  ���ֻ�ڱ�Ҫʱʹ�ô˽ӿڣ�Ȼ��������Ļ������ͷŹ����� CancellationTokenSource ʵ��
//      * @return WaitHandle 
//      */
//     WaitHandle GetWaitHandle()
//     {
//         return WaitHandle(m_hCts);
//     }
//     /**
//      * @brief ע��һ������ȡ���� CancellationToken ʱ���õ�ί��
//      * 
//      * @param callback 
//      */
//     CancellationTokenRegistration Register(Action1 callback)
//     {
//         mtx.WaitOne();
//         m_vecActionQueue.push_back(callback);
//         mtx.ReleaseMutex();
//     }
// };
// class System::Threading::CancellationTokenRegistration
// {
// };

#endif