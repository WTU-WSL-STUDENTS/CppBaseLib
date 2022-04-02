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
 * @brief 指定可控制任务的创建和执行的可选行为的标志
 * 
 */
enum System::Threading::Tasks::TaskCreation::Options
{
    /**
     * @brief 指定应使用默认行为
     * 
     */
    None = 0,     
    /**
     * @brief 提示 TaskScheduler 以一种尽可能公平的方式安排任务，这意味着较早安排的任务将更可能较早运行，而较晚安排运行的任务将更可能较晚运行
     * 
     */
    PreferFairness = DECLARE_ENUM_FLAG(0),  
    /**
     * @brief 指定任务将是长时间运行的、粗粒度的操作，涉及比细化的系统更少、更大的组件
     * 
     */
    LongRunning = DECLARE_ENUM_FLAG(1),
    /**
     * @brief 指定将任务附加到任务层次结构中的某个父级。 默认情况下，子任务（即由外部任务创建的内部任务）将独立于其父任务执行。 可以使用 AttachedToParent 选项以便将父任务和子任务同步
     * 
     */
    AttachedToParent = DECLARE_ENUM_FLAG(2),
    /**
     * @brief 指定任何尝试作为附加的子任务执行的子任务都无法附加到父任务, 使子任务 AttachedToParent 配置失效 
     * 
     */
    DenyChildAttach = DECLARE_ENUM_FLAG(3),
    HideScheduler = DECLARE_ENUM_FLAG(4),
    RunContinuationsAsynchronously = DECLARE_ENUM_FLAG(5),
};
/**
 * @brief 表示 Task 的生命周期中的当前阶段
 * 
 */
enum class System::Threading::Tasks::TaskStatus : long
{
    /**
     * @brief 该任务已通过对其自身的 CancellationToken 引发 OperationCanceledException 对取消进行了确认，此时该标记处于已发送信号状态；或者在该任务开始执行之前，已向该任务的 CancellationToken 发出了信号。 有关详细信息，请参阅任务取消。
     * 
     */
    Created = 0, 	
    /**
     * @brief 该任务正在等待内部将其激活并进行计划
     * 
     */
    WaitingForActivation, 	
    /**
     * @brief 该任务已被计划执行，但尚未开始执行
     * 
     */
    WaitingToRun, 
    /**
     * @brief 该任务正在运行，但尚未完成
     * 
     */
    Running, 
    /**
     * @brief 该任务已完成执行，正在隐式等待附加的子任务完成
     * 
     */
    WaitingForChildrenToComplete, 
    /**
     * @brief 已成功完成执行的任务
     * 
     */
    RanToCompletion, 
    /**
     * @brief 由于取消的原因而完成的任务
     * 
     */
    Canceled, 
    /**
     * @brief 由于未处理异常的原因而完成的任务
     * 
     */
    Faulted 
};
/**
 * @brief 为 ContinueWith 方法创建的任务指定行为
 * https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.tasks.taskcontinuationoptions?view=net-5.0
 * 
 */
enum System::Threading::Tasks::TaskContinuation::Options
{
    /**
     * @brief  延续任务在前面的任务完成后以异步方式运行，与前面任务最终的 Status 属性值无关。 如果延续为子任务，则会将其创建为分离的嵌套任务
     * 
     */
    None = 0,
    /**
     * @brief 提示 TaskScheduler 按任务计划的顺序安排任务，因此较早安排的任务将更可能较早运行
     * 
     */
    PreferFairness = DECLARE_ENUM_FLAG(0),
    /**
     * @brief 延续任务将是长期运行的、粗粒度的操作
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
 * @brief Task 任务的 Context
 * 
 */
typedef System::Object* AsyncState;
/**
 * @brief 创建 AsyncState , Context 的构建只允许传入指针对象
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
 * @brief 释放 AsyncState
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
     * @brief 获取一个用户定义的对象，该对象限定或包含有关异步操作的信息
     * 
     * @return void* 
     */
    virtual AsyncState GetAsyncState() = 0;
    /**
     * @brief 获取用于等待异步操作完成的 WaitHandle
     * 
     * @return WaitHandle 
     */
    virtual System::Threading::WaitHandle GetAsyncWaitHandle() = 0;
    /**
     * @brief 获取一个值，该值指示异步操作是否已完成
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
    /* Task 作为独立 Task 的入口 */
    System::Action<AsyncState> m_action;
    /* Task 作为 Continue task 的入口 */
    System::Action<Task&, AsyncState> m_childAct;
    AsyncState m_pAsyncState;
    WEAK_PTR(Task) parrent;

    /**
     * @brief Continue task 构造函数
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
     * @brief 独立 Task 构造函数
     * 
     * @param action 
     * @param args 由 malloc 活 calloc 创建的指针集合
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
     * @brief 取消任务, 在非 Task 执行线程调用, 可取消处于列队中但未执行的任务. 调用该接口， 会递归将 Canceled 标志位传导至所有父任务
     * 
     */
    void Cancel()
    {
        if((long)TaskStatus::WaitingToRun > m_nStatus)
        {
            WaitForThreadpoolWorkCallbacks(work, true);/* true : 撤销在排队中的工作项 */
        }
        Interlocked<long>::Exchange(m_nStatus, (long)TaskStatus::Canceled);
        if(parrent)
        {
            parrent->Cancel();
        }
    }
    /**
     * @brief 获取在创建 Task 时提供的状态对象，如果未提供，则为 NULL
     * 
     * @return AsyncState 
     */
    inline AsyncState GetAsyncState() override
    {
        return m_pAsyncState;
    }
    /**
     * @brief 获取可用于等待任务完成的 WaitHandle
     * 
     * @return System::Threading::WaitHandle 
     */
    System::Threading::WaitHandle GetAsyncWaitHandle() override
    {
        return m_are;
    }
    /**
     * @brief 获取用于创建此任务的 TaskCreationOptions
     * 
     * @return CreationOptions 
     */
    inline CreationOptions GetCreationOptions()
    {
        return m_nCreationOptions;
    } 
    /**
     * @brief 获取此 Task 实例的 ID
     * 
     * @return int 
     */
    inline int GetId()
    {
        return m_nId;
    }
    /**
     * @brief 获取此任务的 TaskStatus
     * 
     * @return TaskStatus 
     */
    TaskStatus GetStatus()
    {
        return (TaskStatus)m_nStatus;
    }
    /**
     * @brief 获取此 Task 实例是否由于被取消的原因而完成（停止）执行
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCanceled()
    {
        return (long)TaskStatus::Canceled == m_nStatus;
    }
    /**
     * @brief 获取一个值，它表示是否已完成任务
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCompleted() override
    {
        return IsCanceled() || IsCompletedSuccessfully() || IsFaulted();
    }
    /**
     * @brief 任务是否运行到完成
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCompletedSuccessfully()
    {
        return (long)TaskStatus::RanToCompletion == m_nStatus;
    }
    /**
     * @brief 获取 Task 是否由于未经处理异常的原因而完成
     * 
     * @return true 
     * @return false 
     */
    inline bool IsFaulted()
    {
        return (long)TaskStatus::Faulted == m_nStatus;
    }
    /**
     * @brief 创建一个在目标 Task 完成时异步执行的延续任务
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
     * @brief 对当前的 Task 同步运行 TaskScheduler
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
                (long)TaskStatus::Running == m_nStatus/* 如果主线程先退出， 线程池还在执行， 那么此时 Task 堆栈就被破坏，此时需要立刻退出 Task */
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
     * @brief 启动 Task
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
     * @brief 等待 Task 完成执行过程
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
        WaitForThreadpoolWorkCallbacks(work, (int)TaskStatus::WaitingForActivation > (int)m_nStatus/* true : 撤销在排队中的工作项 */);
        return true;
    }
    /**
     * @brief 等待提供的所有 Task 对象完成执行过程
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
     * @brief 等待提供的任一 Task 对象完成执行过程
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
//     /* ansy 异步策略 */
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
//      * @brief 启动 Task
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
//      * @brief 获取与此 CancellationTokenSource 关联的 CancellationToken
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
//      * @brief 传达取消请求
//      * 
//      */
//     inline void Cancel()
//     {
//         SetEvent(m_hCts);
//     }
//     /**
//      * @brief 在指定的毫秒数后计划对此 CancellationTokenSource 的取消操作
//      * 
//      * @param millisecondsDelay 
//      */
//     inline void CancelAfter(int millisecondsDelay)
//     {
//         /* TODO : Delay */
//         SetEvent(m_hCts);
//     }
//     /**
//      * @brief 释放 CancellationTokenSource 类的当前实例所使用的所有资源
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
//      * @brief 获取是否已请求取消此标记
//      * 
//      * @return true 
//      * @return false 
//      */
//     bool IsCancellationRequested()
//     {
//         return m_bCancelRequest;
//     }
//     /**
//      * @brief 获取此标记是否能处于已取消状态
//      * 
//      * @return true 
//      * @return false 
//      */
//     bool CanBeCanceled()
//     {
//         return m_bCanceled;
//     }
//     /**
//      * @brief 获取在取消标记时收到信号的 WaitHandle , 不应直接关闭或释放该句柄.
//      *  最好只在必要时使用此接口，然后在最早的机会中释放关联的 CancellationTokenSource 实例
//      * @return WaitHandle 
//      */
//     WaitHandle GetWaitHandle()
//     {
//         return WaitHandle(m_hCts);
//     }
//     /**
//      * @brief 注册一个将在取消此 CancellationToken 时调用的委托
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