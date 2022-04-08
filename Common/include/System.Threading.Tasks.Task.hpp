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
     * @brief ָ�������ǳ�ʱ�����еġ������ȵĲ������漰��ϸ����ϵͳ���١��������� CallbackMayRunLong
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
    virtual System::Threading::WaitHandle& GetAsyncWaitHandle() = 0;
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ�첽�����Ƿ������
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
         * @brief �����ǰʵ���յ��źţ���Ϊ true������Ϊ false��
         *
         * @param millisecond �ȴ��ĺ�����, -1 ��ʾ�����ڵȴ�
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
     * @brief Awaiter, ͨ�� IOCP ���Ʋ���, Ĭ��Ϊ PROCESSER_COUNT
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
        /* ��ǰ����ִ����Ϻ�, ִ�������첽�ȴ�������( ContinueWith / Await ) �������Ͷ�ݵ� IOCP */
        for(int i = 0; i <(int) m_childTasks.Count(); i++)                     
        {                                                                       
            m_childTasks[i]->Start();                                        
            m_hTaskCompletionPort.Release();                                 
        }                 
        /* ���и��ӵ���ǰ�̵߳�������, ��ǰ�߳�ͬ���ȴ����߳���ɺ����˳� */
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
     * @brief ���� Task ���캯��
     * 
     * @param action 
     * @param args �� malloc �� calloc ������ָ�뼯��
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
                WaitForThreadpoolWorkCallbacks(m_pWork, true);;/* �����Ŷ��е� work */
                Interlocked<TaskStatus>::Exchange(m_eStatus, TaskStatus::Canceled);
            }
            CloseThreadpoolWork(m_pWork);
            m_pWork = NULL;
        }
    }
    /**
     * @brief ��ȡ�ڴ��� Task ʱ�ṩ��״̬�������δ�ṩ����Ϊ NULL
     * 
     * @return AsyncState 
     */
    DECLARE_GETTER(AsyncState, AsyncState, { return m_pAsyncState; })
    /**
     * @brief ��ȡ�����ڵȴ�������ɵ� WaitHandle ������ɶ˿�
     * 
     * @return System::Threading::WaitHandle 
     */
    DECLARE_GETTER(System::Threading::WaitHandle&, AsyncWaitHandle, { return m_hTaskCompletionPort; })
    /**
     * @brief ��ȡ���ڴ���������� TaskCreationOptions
     * 
     * @return CreationOptions 
     */
    DECLARE_INDEXER(TaskCreationOptions, CreationOptions, { return m_eCreationOptions; }, { m_eCreationOptions = SETTER_VALUE; MemoryBarrier(); })
    /**
     * @brief ��ȡ�� Task ʵ���� ID
     * 
     * @return int 
     */
    DECLARE_GETTER(int, DWORD, { return (DWORD)m_pWork; })
    /**
     * @brief ��ȡ������� TaskStatus
     * 
     * @return TaskStatus 
     */
    DECLARE_INDEXER(TaskStatus, Status, { return m_eStatus; }, { m_eStatus = SETTER_VALUE; MemoryBarrier(); })
    /**
     * @brief ��ȡ�� Task ʵ���Ƿ����ڱ�ȡ����ԭ�����ɣ�ֹͣ��ִ��
     * 
     * @return true 
     * @return false 
     */
    inline bool IsCanceled()
    {
        return TaskStatus::Canceled == m_eStatus;
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
        return TaskStatus::RanToCompletion == m_eStatus;
    }
    /**
     * @brief ��ȡ Task �Ƿ�����δ�������쳣��ԭ������
     * 
     * @return true 
     * @return false 
     */
    inline bool IsFaulted()
    {
        return TaskStatus::Faulted == m_eStatus;
    }
    /**
     * @brief ����һ����Ŀ�� Task ���ʱ�첽ִ�е���������
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
     * @brief �Ե�ǰ�� Task ͬ������ TaskScheduler
     * 
     */
    inline void RunSynchronously()
    {
        m_action(m_pAsyncState);
    }
    /**
     * @brief ���� Task
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
     * @brief ͬ���ȴ� Task ���ִ�й���
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
     * @brief ͬ���ȴ��ṩ������ Task �������ִ�й���
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
     * @briefͬ�� �ȴ��ṩ����һ Task �������ִ�й���
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