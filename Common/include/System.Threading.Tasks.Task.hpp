 /*
  * @Description: 
  * @Version: 1.0
  * @Autor: like
  * @Date: 2022-01-17 15:12:58
 * @LastEditors: like
 * @LastEditTime: 2022-04-04 08:09:46
  */
#ifndef SYSTEM_THREADING_TASKS_TASK_HPP
#define SYSTEM_THREADING_TASKS_TASK_HPP
#include <System.Threading.ThreadPool.hpp>
#include <System.Action.hpp>
#include <System.List.hpp>
#include <System.Threading.EventWaitHandle.hpp>
#include <System.Threading.Interlocked.hpp>
#include <System.Threading.CancellationToken.hpp>
#include <System.Threading.Task.Parallel.hpp>
#include <exception>

#define TASK_LAMDA_CONTEXT TPLAMDA_CONTEXT
#define DECLARE_TASKLAMDA(codeblock)[](PTP_CALLBACK_INSTANCE Instance,PVOID TASK_LAMDA_CONTEXT, PTP_WORK Work)->void\
    {                                       \
        UNREFERENCED_PARAMETER(Instance);   \
        UNREFERENCED_PARAMETER(Work);       \
        codeblock                           \
    }
using PTP_TASK_CALLBACK = PTP_WORK_CALLBACK;

namespace System
{
    class IAsyncResult;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.iasyncresult.asyncwaithandle?view=net-5.0 */
	using AsyncCallback = Action<WEAK_PTR(IAsyncResult)>;
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
     * @brief 指定任务将是长时间运行的、粗粒度的操作，涉及比细化的系统更少、更大的组件 CallbackMayRunLong
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
using System::Threading::Tasks::TaskStatus;
template<>
class System::Threading::Interlocked<TaskStatus>
{
public:
    static inline TaskStatus Add(TaskStatus& location, const TaskStatus& value)
    {
        long v = (long)value;
        return (TaskStatus)::InterlockedAdd((long*)&location, v);
    }
    static inline TaskStatus And(TaskStatus& location, const TaskStatus& value)
    {
        long v = (long)value;
        return (TaskStatus)::InterlockedAnd((long*)&location, v);
    }
    static inline TaskStatus CompareExchange(TaskStatus& location, const TaskStatus& value)
    {
        DWORD v = (DWORD)value;
        return (TaskStatus)InterlockedCompareExchange((long*)&location, v, v);
    }
    static inline TaskStatus Decrement(TaskStatus& location)
    {
        return (TaskStatus)::InterlockedDecrement((long*)&location);
    }
    static inline TaskStatus Exchange(TaskStatus& location, const TaskStatus& value)
    {
        long v = (long)value;
        ::InterlockedExchange((long*)&location, v);
        return location;
    }
    static inline TaskStatus Increment(TaskStatus& location)
    {
        return (TaskStatus)::InterlockedIncrement((long*)&location);
    }
    static inline TaskStatus Or(TaskStatus& location, const TaskStatus& value)
    {
        long v = (long)value;
        return (TaskStatus)::InterlockedOr((long*)&location, v);
    }
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
    virtual System::Threading::WaitHandle& GetAsyncWaitHandle() = 0;
    /**
     * @brief 获取一个值，该值指示异步操作是否已完成
     * 
     * @return true 
     * @return false 
     */
    virtual bool IsCompleted() = 0;
};

#define await(pTask) ((pTask)->GetAsyncWaitHandle().WaitOne())
typedef StackList<WEAK_PTR(System::Threading::Tasks::Task)> AttachedTasks;
class System::Threading::Tasks::Task final : public IAsyncResult//, public IDisposable
{
    class IOCP final : public WaitHandle
    {
    public:
        DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(IOCP)
        IOCP(DWORD numberOfConcurrentThreads = PROCESSER_COUNT) : WaitHandle(CreateIoCompletionPort((HANDLE)INVALID_HANDLE_VALUE, NULL, (u_long)NULL, numberOfConcurrentThreads))
        {
            WINAPI_ASSERT(false == m_bDisposed, "https://docs.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-createiocompletionport");
        }
        ~IOCP()
        {
            Dispose();
        }

        /**
         * @brief 如果当前实例收到信号，则为 true；否则为 false。
         *
         * @param millisecond 等待的毫秒数, -1 表示无限期等待
         * @return true
         * @return false
         */
        bool WaitOne(DWORD millisecond = INFINITE) override
        {
            DWORD byteTransferred;
            ULONG_PTR completionKey = 0;
            LPOVERLAPPED pOverlapped = 0;
            if (GetQueuedCompletionStatus(m_hWaitHandle, &byteTransferred, &completionKey, &pOverlapped, millisecond))
            {
                return true;
            }
            DWORD nErrorCode = GetLastError();
            WINAPI_ASSERT(ERROR_ABANDONED_WAIT_0 == nErrorCode/* IOCP disposed */ || WAIT_TIMEOUT == nErrorCode/* IOCP timeout*/, "Wait IOCP failed");
            return false;        
        }
        void Release()
        {
            WINAPI_ASSERT(PostQueuedCompletionStatus(m_hWaitHandle, 0, 0, 0), "Publish work finish failed");                                                                  
        }
    };
private:
    WEAK_PTR(Task) m_pParentTask;
    PTP_WORK m_pWork;
    /**
     * @brief Awaiter, 通过 IOCP 控制并发, 默认为 PROCESSER_COUNT
     */
    IOCP m_hTaskCompletionPort;
    OVERLAPPED_ENTRY m_olEntry;
    StackList<WEAK_PTR(Task)> m_childTasks;
    /**
     * @brief Wait handle
     */
    ManualResetEvent m_mre;
    /**
     * @brief Task work processing
     */
    System::Action<AsyncState> m_action;
    AsyncState m_pAsyncState;
    TaskCreationOptions m_eCreationOptions;
    TaskStatus m_eStatus;
    WEAK_PTR(CancellationToken) m_pToken;

    Task(Task* pParentTask = NULL) : m_pParentTask(pParentTask), m_pWork(NULL), 
        m_hTaskCompletionPort(IOCP()), m_olEntry({0}), m_childTasks(StackList<WEAK_PTR(Task)>(PROCESSER_COUNT)),
        m_mre(ManualResetEvent(false)),
        m_action(NULL), m_pAsyncState(NULL), m_pToken(NULL),
        m_eCreationOptions(TaskCreationOptions::None), m_eStatus(TaskStatus::Created)
    {
    }
    void TaskWorkProcessing()
    {              
        Interlocked<TaskStatus>::Exchange(m_eStatus, TaskStatus::Running);   
        try                                                                     
        {                                                                       
            RunSynchronously();                                              
            Interlocked<TaskStatus>::Exchange                                   
            (                                                                   
                m_eStatus,                                                   
                m_pToken && m_pToken->GetCancellationRequested() ?        
                TaskStatus::Canceled : TaskStatus::RanToCompletion              
            );                                                                  
        }                                                                       
        catch (const std::exception& e)                                         
        {  
            Interlocked<TaskStatus>::Exchange(m_eStatus, TaskStatus::Faulted);
        }           
        /* 当前任务执行完毕后, 执行所有异步等待的任务( ContinueWith / Await ) 并将结果投递到 IOCP */
        for(int i = 0; i <(int) m_childTasks.Count(); i++)                     
        {                                                                       
            m_childTasks[i]->Start();                                        
            m_hTaskCompletionPort.Release();                                 
        }                 
        /* 如有附加到当前线程的子任务, 当前线程同步等待子线程完成后再退出 */
        if (0 < m_childTasks.Count() && TaskCreationOptions::DenyChildAttach == (m_eCreationOptions & TaskCreationOptions::DenyChildAttach))
        {
            return;
        }
        WaitHandle** handles = new WaitHandle*[m_childTasks.Count()];
        int count = 0;
        for(int i = 0,idx = 0; i < (int)m_childTasks.Count(); i++)
        {                                                                       
            if (0 == (m_childTasks[i]->m_eCreationOptions & TaskCreationOptions::AttachedToParent))
            {
                continue;
            }
            handles[count] = &m_childTasks[i]->m_mre;
            count++;
        }         
        if (count)
        {
            WaitHandle::WaitAll(handles, count);
        }
        delete[] handles;
    }
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Task)
    /**
     * @brief 独立 Task 构造函数
     * 
     * @param action 
     * @param args 由 malloc 活 calloc 创建的指针集合
     */
    Task(Action<AsyncState> action, AsyncState args = NULL, WEAK_PTR(CancellationToken) token = NULL) : Task()
    {
        m_action        = action;
        m_pAsyncState   = args;
        m_pToken        = token;
        PTP_TASK_CALLBACK callback = DECLARE_TASKLAMDA
        ({
            WEAK_PTR(Task) t = static_cast<WEAK_PTR(Task)>(TASK_LAMDA_CONTEXT);
            SetEventWhenCallbackReturns(Instance, t->m_mre.Handle());
            t->TaskWorkProcessing();
        });
        WINAPI_ASSERT
        (
            m_pWork = CreateThreadpoolWork(callback, this, &(ThreadPoolSingleton::Ref().environment)), 
            "Task construct failed"
        );
    }
    ~Task()
    {
        DisposeAsyncState(m_pAsyncState);
        if(m_pWork)
        {
            if (TaskStatus::Created < m_eStatus && !IsCompleted())
            {
                WaitForThreadpoolWorkCallbacks(m_pWork, true);;/* 撤销排队中的 work */
                Interlocked<TaskStatus>::Exchange(m_eStatus, TaskStatus::Canceled);
            }
            CloseThreadpoolWork(m_pWork);
            m_pWork = NULL;
        }
    }
    /**
     * @brief 获取在创建 Task 时提供的状态对象，如果未提供，则为 NULL
     * 
     * @return AsyncState 
     */
    DECLARE_GETTER(AsyncState, AsyncState, { return m_pAsyncState; })
    /**
     * @brief 获取可用于等待任务完成的 WaitHandle 基于完成端口
     * 
     * @return System::Threading::WaitHandle 
     */
    DECLARE_GETTER(System::Threading::WaitHandle&, AsyncWaitHandle, { return m_hTaskCompletionPort; })
    /**
     * @brief 获取用于创建此任务的 TaskCreationOptions
     * 
     * @return CreationOptions 
     */
    DECLARE_INDEXER(TaskCreationOptions, CreationOptions, { return m_eCreationOptions; }, { m_eCreationOptions = SETTER_VALUE; MemoryBarrier(); })
    /**
     * @brief 获取此 Task 实例的 ID
     * 
     * @return int 
     */
    DECLARE_GETTER(int, DWORD, { return (DWORD)m_pWork; })
    /**
     * @brief 获取此任务的 TaskStatus
     * 
     * @return TaskStatus 
     */
    DECLARE_INDEXER(TaskStatus, Status, { return m_eStatus; }, { m_eStatus = SETTER_VALUE; MemoryBarrier(); })
    /**
     * @brief 获取此 Task 实例是否由于被取消的原因而完成（停止）执行
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCanceled()
    {
        return TaskStatus::Canceled == m_eStatus;
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
        return TaskStatus::RanToCompletion == m_eStatus;
    }
    /**
     * @brief 获取 Task 是否由于未经处理异常的原因而完成
     * 
     * @return true 
     * @return false 
     */
    inline bool IsFaulted()
    {
        return TaskStatus::Faulted == m_eStatus;
    }
    /**
     * @brief 创建一个在目标 Task 完成时异步执行的延续任务
     * 
     * @param continuationAction 
     * @return Task 
     */
    Task* ContinueWith(Action<AsyncState> continuationAction, AsyncState args = NULL, WEAK_PTR(CancellationToken) token = NULL)
    {
        ERROR_ASSERT(NULL == m_pToken || m_pToken == token, "Child task cancellation token need same as parent task");
        Task* task = new Task(this);
        task->m_action = continuationAction;
        task->m_pAsyncState = args;
        task->m_pToken = token;
        PTP_TASK_CALLBACK callback = DECLARE_TASKLAMDA
        ({
            WEAK_PTR(Task) t = static_cast<WEAK_PTR(Task)>(TASK_LAMDA_CONTEXT);
            t->m_pParentTask->m_hTaskCompletionPort.WaitOne();

            SetEventWhenCallbackReturns(Instance, t->m_mre.Handle());
            t->TaskWorkProcessing();
        });
        WINAPI_ASSERT
        (
            task->m_pWork = CreateThreadpoolWork(callback, task, &(ThreadPoolSingleton::Ref().environment)),
            "Task construct failed"
        );

        m_childTasks.Add(task);
        return task;
    }
    /**
     * @brief 对当前的 Task 同步运行 TaskScheduler
     * 
     */
    inline void RunSynchronously()
    {
        m_action(m_pAsyncState);
    }
    /**
     * @brief 启动 Task
     * 
     */
    void Start()
    {
		m_mre.Reset();
        SubmitThreadpoolWork(m_pWork);
        Interlocked<TaskStatus>::Exchange(m_eStatus, TaskStatus::WaitingForActivation);
    }
    void Start(AsyncState args)
    {
        DisposeAsyncState(m_pAsyncState);
        m_pAsyncState = args;
        Start();
    }
    /**
     * @brief 同步等待 Task 完成执行过程
     * 
     * @param millisecondsTimeout 
     * @return true 
     * @return false 
     */
    inline bool WaitOne(DWORD millisecondsTimeout = INFINITE)
    {
        return m_mre.WaitOne(millisecondsTimeout);
    }
    static Task* Delay(int millsecond, WEAK_PTR(CancellationToken) token = NULL)
    {
        ERROR_ASSERT(-2 < millsecond, "millsecond error");
        Task* task = new Task
        (
            [](AsyncState args)->void 
            {
                int millsecond = (int)args[0];
                WEAK_PTR(Task) delayTask = static_cast<WEAK_PTR(Task)>(args[1]);
                WEAK_PTR(CancellationToken) token = static_cast<WEAK_PTR(CancellationToken)>(args[2]);
                Timer t
                (
                    [](Object tArgs)->void
                    {
                        AsyncState args = static_cast<AsyncState>(tArgs);
                        WEAK_PTR(Task) delayTask = static_cast<WEAK_PTR(Task)>(args[1]);
                        WEAK_PTR(CancellationToken) token = static_cast<WEAK_PTR(CancellationToken)>(args[2]);
                        if (token)
                        {
                            VOIDRET_ASSERT(!token->GetCancellationRequested());
                        }
                        static_cast<WEAK_PTR(IOCP)>(&delayTask->GetAsyncWaitHandle())->Release();/* Delay task normal finished */
                    }, 
                    args,
                    millsecond, 
                    0
                );
                if (token)
                {
                    while (!token->GetCancellationRequested())
                    {
                        delayTask->GetAsyncWaitHandle().WaitOne(0);
                    }
                    t.Change(0, 0);/* exit timer immediately */
                    t.Dispose();
                }
                else
                {
                    delayTask->GetAsyncWaitHandle().WaitOne();
                }
            },
            NULL,
            token
        );
        task->Start(CreateAsyncState((Object)millsecond, task, token));
        /*task->m_action = continuationAction;
        task->m_pAsyncState = args;
        task->m_pToken = token;*/
        PTP_TASK_CALLBACK callback = DECLARE_TASKLAMDA
        ({
            WEAK_PTR(Task) t = static_cast<WEAK_PTR(Task)>(TASK_LAMDA_CONTEXT);
            SetEventWhenCallbackReturns(Instance, t->m_mre.Handle());
            t->TaskWorkProcessing();
        });
        WINAPI_ASSERT
        (
            task->m_pWork = CreateThreadpoolWork(callback, task, &(ThreadPoolSingleton::Ref().environment)),
            "Task construct failed"
        );
        return task;
    }
    /**
     * @brief 同步等待提供的所有 Task 对象完成执行过程
     * 
     * @param tasks 
     * @param count 
     * @param millisecondsTimeout 
     * @return true 
     * @return false 
     */
    static bool WaitAll(Task** tasks, int count, DWORD millisecondsTimeout = INFINITE)
    {
        ERROR_ASSERT(tasks, "tasks is nullptr");
        WaitHandle** handles = new WaitHandle * [count];
        for (int i = 0; i < count; i++)
        {
            ERROR_ASSERT(tasks[i], "task is nullptr");
            handles[i] = &tasks[i]->m_mre;
        }
        bool bRet = WaitHandle::WaitAll(handles, count, millisecondsTimeout);
        delete[] handles;
        return bRet;
    }
    static bool WaitAll(const std::initializer_list<Task*>& il, DWORD millisecondsTimeout = INFINITE)
    {
        WaitHandle** handles = new WaitHandle * [il.size()];
        auto ptr = il.begin();
        for (size_t i = 0; i < il.size(); i++, ptr++)
        {
            ERROR_ASSERT(*ptr, "task is nullptr");
            handles[i] = &((*ptr)->m_mre);
        }
        bool bRet = WaitHandle::WaitAny(handles, (DWORD)il.size(), millisecondsTimeout);
        delete[] handles;
        return bRet;
    }
    /**
     * @brief同步 等待提供的任一 Task 对象完成执行过程
     * 
     * @param tasks 
     * @param count 
     * @param millisecondsTimeout 
     * @return true 
     * @return false 
     */
    static int WaitAny(Task** tasks, int count, DWORD millisecondsTimeout = INFINITE)
    {
        ERROR_ASSERT(tasks, "tasks is nullptr");
        WaitHandle** handles = new WaitHandle*[count];
        for(int i = 0 ; i < count ; i++)
        {
            ERROR_ASSERT(tasks[i], "task is nullptr");
            handles[i] = &tasks[i]->m_mre;
        }
        int n = WaitHandle::WaitAny(handles, count, millisecondsTimeout);
        delete[] handles;
        return n;
    }
    static int WaitAny(const std::initializer_list<Task*>& il, DWORD millisecondsTimeout = INFINITE)
    {
        WaitHandle** handles = new WaitHandle * [il.size()];
        auto ptr = il.begin();
        for(size_t i = 0; i < il.size(); i++, ptr++)
        {
            ERROR_ASSERT(*ptr, "task is nullptr");
            handles[i] = &((*ptr)->m_mre);
        }
        int n = WaitHandle::WaitAny(handles, (DWORD)il.size(), millisecondsTimeout);
        delete[] handles;
        return n;
    }
};
class TaskAwaiter
{

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