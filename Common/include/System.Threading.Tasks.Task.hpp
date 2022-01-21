// /*
//  * @Description: 
//  * @Version: 1.0
//  * @Autor: like
//  * @Date: 2022-01-17 15:12:58
//  * @LastEditors: like
//  * @LastEditTime: 2022-01-19 16:26:44
//  */
// #ifndef SYSTEM_THREADING_TASKS_TASK_HPP
// #define SYSTEM_THREADING_TASKS_TASK_HPP

// #include <System.Threading.WaitHandle.hpp>
// #include <System.Threading.Mutex.hpp>
// #include <System.Threading.EventWaitHandle.hpp>
// #include <functional>
// #include <future>
// #include <vector>

// namespace System
// {
//     class IAsyncResult;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.iasyncresult.asyncwaithandle?view=net-5.0 */
//     typedef std::function<void(void)>  Action;
//     typedef std::function<void(Object)> Action1;
//     // class Action
// };
// namespace System::Threading
// {
//     class CancellationToken;
//     class CancellationTokenSource;
//     class CancellationTokenRegistration;
//     enum TaskCreationOptions;
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
// enum System::Threading::TaskCreationOptions
// {
//     None = 0,       /* Ĭ���첽���� , Task �����첽���л����У���ȡ����ϵͳ�ĸ��أ��������޷������� */
//     LongRunning,    /* ָ�������ǳ�ʱ�����е� */
//     TaskCreationOptionsCount
// };
// namespace System::Threading::Tasks
// {
//     class Task;
//     struct ValueTask;   /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.tasks.valuetask?view=net-5.0 */
// };

// class System::IAsyncResult
// {
// public:
//     /**
//      * @brief ��ȡһ���û�����Ķ��󣬸ö����޶�������й��첽��������Ϣ
//      * 
//      * @return void* 
//      */
//     virtual void* GetAsyncState() = 0;
// };

// // template<typename T>
// // class Action
// // {

// // };
// //https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.tasks.task.-ctor?view=net-5.0
// class System::Threading::Tasks::Task : public IAsyncResult, public IDisposable
// {
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
// };

// #endif