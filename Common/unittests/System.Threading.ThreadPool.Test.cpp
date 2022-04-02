// /*
//  * @Description: 
//  * @Version: 1.0
//  * @Autor: like
//  * @Date: 2022-01-20 11:47:40
//  * @LastEditors: like
//  * @LastEditTime: 2022-02-18 18:24:32
//  */
// #include <System.DateTime.hpp>
// #include <System.Threading.ThreadPool.hpp>
// #include <System.Threading.EventWaitHandle.hpp>
// #include <System.Threading.Mutex.hpp>
// #include <System.Threading.Semaphore.hpp>
// #include <System.Threading.Interlocked.hpp>

// using namespace System;
// using namespace System::Threading;

// #define WORKCOUNT 30

// #define ENABLE_THREAD_PRINTF
// #ifdef ENABLE_THREAD_PRINTF
//     // #define DBG_PRINT (printf("%s:%u %s:%s:\t", __FILE__, __LINE__, __DATE__, __TIME__), printf) 
//     Mutex mtx;/* �����˳�, ����ϵͳ�ͷ� */
//     inline void ThreadPrintf(const char *cmd, ...)  
//     {  
//         mtx.WaitOne();
//         // printf("%s %s ", __DATE__, __TIME__);  
//         va_list args;       //����һ��va_list���͵ı������������浥������  
//         va_start(args,cmd); //ʹargsָ��ɱ�����ĵ�һ������  
//         vprintf(cmd,args);  //������vprintf�ȴ�V��  
//         va_end(args);       //�����ɱ�����Ļ�ȡ
//         printf("\n");  
//         mtx.ReleaseMutex();
//     }
// #else
//     #define ThreadPrintf printf
// #endif

// struct TaskTestArgs
// {
//     Mutex* pMtx;
//     AutoResetEvent* pAre;
//     long* pCount;
//     int nWorkId;
// };
// /**
//  * 
//  * ʵ�� 1, 1 Log ��¼
//  * 1, 1, WT_EXECUTEDEFAULT 
//     Work-0 : Thread-17356 Yiled
//     Work-1 : Thread-20204 Yiled
//     Work-2 : Thread-20744 Yiled
//     Work-3 : Thread-20824 Yiled
//     Work-3 : Thread-20824 Hello from the thread pool.
//     Work-2 : Thread-20744 Hello from the thread pool.
//     Work-1 : Thread-20204 Hello from the thread pool.
//     Work-0 : Thread-17356 Hello from the thread pool.
//     Work-4 : Thread-8300 Yiled
//     Work-5 : Thread-10988 Yiled
//     Work-6 : Thread-20864 Yiled
//     Work-7 : Thread-4900 Yiled 
//     Work-5 : Thread-10988 Hello from the thread pool.
//     Work-4 : Thread-8300 Hello from the thread pool. 
//     Work-7 : Thread-4900 Hello from the thread pool. 
//     Work-6 : Thread-20864 Hello from the thread pool.
//     Work-8 : Thread-19740 Yiled
//     Work-8 : Thread-19740 Hello from the thread pool.
//     Work-9 : Thread-19820 Yiled
//     Work-9 : Thread-19820 Hello from the thread pool.

//  * 1, 1, WT_EXECUTEINIOTHREAD
//     Work-0 : Thread-20796 Yiled
//     Work-1 : Thread-17280 Yiled
//     Work-2 : Thread-9252 Yiled 
//     Work-3 : Thread-16588 Yiled
//     Work-3 : Thread-16588 Hello from the thread pool.
//     Work-2 : Thread-9252 Hello from the thread pool. 
//     Work-1 : Thread-17280 Hello from the thread pool.
//     Work-0 : Thread-20796 Hello from the thread pool.
//     Work-4 : Thread-11260 Yiled
//     Work-5 : Thread-15748 Yiled
//     Work-6 : Thread-9036 Yiled 
//     Work-7 : Thread-21408 Yiled
//     Work-5 : Thread-15748 Hello from the thread pool.
//     Work-4 : Thread-11260 Hello from the thread pool.
//     Work-7 : Thread-21408 Hello from the thread pool.
//     Work-6 : Thread-9036 Hello from the thread pool. 
//     Work-8 : Thread-19676 Yiled
//     Work-8 : Thread-19676 Hello from the thread pool.
//     Work-9 : Thread-19688 Yiled
//     Work-9 : Thread-19688 Hello from the thread pool.

//  * 1, 1, WT_EXECUTEINPERSISTENTTHREAD
//     Work-0 : Thread-13496 Yiled
//     Work-0 : Thread-13496 Hello from the thread pool.
//     �����쳣�˳�
    
//  * 1, 1, WT_EXECUTELONGFUNCTION
//     Work-0 : Thread-2784 Yiled 
//     Work-1 : Thread-2600 Yiled 
//     Work-2 : Thread-18840 Yiled
//     Work-3 : Thread-4056 Yiled 
//     Work-4 : Thread-16964 Yiled
//     Work-5 : Thread-2408 Yiled 
//     Work-6 : Thread-7496 Yiled 
//     Work-7 : Thread-1312 Yiled 
//     Work-8 : Thread-21024 Yiled
//     Work-9 : Thread-19384 Yiled
//     Work-5 : Thread-2408 Hello from the thread pool. 
//     Work-9 : Thread-19384 Hello from the thread pool.
//     Work-4 : Thread-16964 Hello from the thread pool.
//     Work-3 : Thread-4056 Hello from the thread pool. 
//     Work-8 : Thread-21024 Hello from the thread pool.
//     Work-7 : Thread-1312 Hello from the thread pool. 
//     Work-6 : Thread-7496 Hello from the thread pool. 
//     Work-2 : Thread-18840 Hello from the thread pool.
//     Work-1 : Thread-2600 Hello from the thread pool. 
//     Work-0 : Thread-2784 Hello from the thread pool.
 
//  * 1, 1, WT_TRANSFER_IMPERSONATION
//     Work-0 : Thread-11236 Yiled
//     Work-1 : Thread-13028 Yiled
//     Work-2 : Thread-4708 Yiled 
//     Work-3 : Thread-19152 Yiled
//     Work-3 : Thread-19152 Hello from the thread pool.
//     Work-2 : Thread-4708 Hello from the thread pool. 
//     Work-1 : Thread-13028 Hello from the thread pool.
//     Work-0 : Thread-11236 Hello from the thread pool.
//     Work-4 : Thread-10324 Yiled
//     Work-5 : Thread-15688 Yiled
//     Work-6 : Thread-20580 Yiled
//     Work-7 : Thread-19092 Yiled
//     Work-4 : Thread-10324 Hello from the thread pool.
//     Work-5 : Thread-15688 Hello from the thread pool.
//     Work-6 : Thread-20580 Hello from the thread pool.
//     Work-7 : Thread-19092 Hello from the thread pool.
//     Work-8 : Thread-21428 Yiled
//     Work-8 : Thread-21428 Hello from the thread pool.
//     Work-9 : Thread-2588 Yiled
//     Work-9 : Thread-2588 Hello from the thread pool.

//  * ���� :
//     WT_EXECUTEDEFAULT : ����ȫȨ�и����̳߳�
//     WT_EXECUTEINIOTHREAD : �µ� API �б�����, ���ý���� WT_EXECUTEDEFAULT ��ͬ
//     WT_EXECUTEINPERSISTENTTHREAD : ���³������ԭ����( ���� //printf ����д��Ļ��Դ���� )
//     WT_EXECUTELONGFUNCTION : ��ʱ�����е����� ,  yiled ������ contex û�� WT_EXECUTEDEFAULT ����
//     WT_TRANSFER_IMPERSONATION : ��ͬ������ִ��, ִ��˳��� WT_EXECUTEDEFAULT ��

//  * ʵ�� 10, 10 Log ��¼
//     ��
//  * ���� :
//     ��������ִ�����ʱ�����, �������� //printf ��ռ��Ļ��Դ���µ�ż������
    
//  */
// void TaskTest()
// {
//     Mutex mtx;
//     AutoResetEvent notifiEvent(false);
//     long totalCount = 0;
//     Object pWorkArgs[WORKCOUNT] = {0};
//     for(int i = 0 ; i < WORKCOUNT; i++)
//     {
//         pWorkArgs[i] = new TaskTestArgs{&mtx, &notifiEvent, &totalCount, i};
//         ThreadPool::Ref().TrySubmitThreadpoolCallback
//         (
//             [](PTP_CALLBACK_INSTANCE instance, Object args)
//             {
//                 TaskTestArgs* p = (TaskTestArgs*)args;
//                 size_t i = 0;
//                 do
//                 {
//                     i++;
//                 }while(i < SIZE_MAX);
                
//                 Interlocked<long>::Increment(*(p->pCount));
//                 char str[WORKCOUNT] = {0};
//                 memset(str, '*', *(p->pCount));
//                 printf("    %s\n", str); 
//                 if(WORKCOUNT == *(p->pCount)) /* work end notification */
//                 {
//                     p->pAre->Set();
//                 }
//             }, pWorkArgs[i]
//         );
//     }
//     printf("Begin work int threadpool ...\n");
//     notifiEvent.WaitOne();
//     printf("All work finished in threadpool. total %d. \n", totalCount);
    
//     /* release memory */
//     for(int i = 0; i < WORKCOUNT; i++)
//     {
//         delete pWorkArgs[i];
//         pWorkArgs[i] = NULL;
//     }
//     notifiEvent.Dispose();
//     mtx.Dispose();
// }
// /**
//  * @brief 
//  * Unregister �ӿ�ֻ���ڻص�����û�д�������ʱ����Գɹ� why???
//  */
// void RegisterTest()
// {
//     int nKey = 0;
//     bool bExecOnlyOnce = true;
//     int nTimerWait = bExecOnlyOnce ? 1000 : 500;
//     AutoResetEvent eTimerWork(false);
//     AutoResetEvent areWhenUnregist(false);

//     RegisteredWaitHandle* rwh = ThreadPool::Ref().RegisterWaitForSingleObject
//     (
//         eTimerWork, 
//         [](Object args, bool timeout)
//         {
//             if(timeout)
//             {
//                 printf("    Monitor-%d Keyboard timeout\n", GetCurrentThreadId());
//             }
//             else
//             {
//                 printf("    Monitor-%d Keyboard value %d\n", GetCurrentThreadId(), (int)args);
//             }
//         }, (Object)nKey, nTimerWait, bExecOnlyOnce
//     );
//     Sleep(1500);/* ���߳��ó�ʱ��Ƭ���̳߳ش���ص� */
//     eTimerWork.Reset();
//     rwh->Unregister(areWhenUnregist);
//     if(areWhenUnregist.WaitOne(1000))
//     {
//         printf("Unregister Sucess\n");
//     }
//     else
//     {
//         printf("Unregister Failed\n");
//     }
//     delete rwh;
//     areWhenUnregist.Dispose();
//     eTimerWork.Dispose();
// }
// DWORD WINAPI TestThreadPool(PVOID pContext);  
  
// CRITICAL_SECTION g_cs;  
// void Test()
// {
//     InitializeCriticalSection(&g_cs);  
//     SECURITY_ATTRIBUTES sa;  
//     sa.bInheritHandle = TRUE;  
//     sa.nLength = sizeof(SECURITY_ATTRIBUTES);  
//     sa.lpSecurityDescriptor = NULL;  
  
//     HANDLE hSemaphore = CreateSemaphoreW(&sa, 0 ,1, L"jyytet");   
  
//     for (int i = 0; i < 10; i++)  
//     {  
//         ThreadPool::Ref().QueueUserWorkItem(TestThreadPool, (Object)i);  
//     }  
//     ThreadPool::Ref().QueueUserWorkItem(TestThreadPool, (Object)10);  
  
//     WaitForSingleObject(hSemaphore, -1);  
//     CloseHandle(hSemaphore);  
//     hSemaphore = NULL;   
//     DeleteCriticalSection(&g_cs);  
// }
// int main()
// {  
//     Test();
//     // /* Threadpool Set */
//     // if(!ThreadPool::Ref().SetMinThreads(6, 0))
//     // {
//     //     printf("SetMinThreads Failed\n");
//     //     return 1;
//     // }
//     // if(!ThreadPool::Ref().SetMaxThreads(64, 0))
//     // {
//     //     printf("SetMinThreads Failed\n");
//     //     return 1;
//     // }
//     // /* Test */
//     // TaskTest();
//     // RegisterTest();
//     // /* Dispose */
//     // ThreadPool::Ref().Dispose();
//     // printf("Destory Resources.\n");
//     return 0;
// }

// DWORD WINAPI TestThreadPool(Object pContext)  
// {  
//     printf("%d\n", pContext);
//     if (10 == (int)pContext)  
//     {  
//         HANDLE hSemaphore = OpenSemaphoreW(SEMAPHORE_ALL_ACCESS, FALSE,  L"jyytet");  
//         // assert(hSemaphore !=  NULL);  
//         ReleaseSemaphore(hSemaphore, 1, NULL);  
//         CloseHandle(hSemaphore);  
//     }  
  
//     return 1;  
// } 



#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <CompliedEntry.h>
#include <CompliedIndexer.h>
//
// Thread pool wait callback function template
//
VOID
CALLBACK
MyWaitCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_WAIT              Wait,
    TP_WAIT_RESULT        WaitResult
    )
{
    // Instance, Parameter, Wait, and WaitResult not used in this example.
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(Parameter);
    UNREFERENCED_PARAMETER(Wait);
    UNREFERENCED_PARAMETER(WaitResult);

    //
    // Do something when the wait is over.
    //
    Sleep(1000);
	printf(" %d MyWaitCallback: wait is over.\n", GetCurrentThreadId());
}


//
// Thread pool timer callback function template
//
VOID
CALLBACK
MyTimerCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_TIMER             Timer
    )
{
    // Instance, Parameter, and Timer not used in this example.
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(Parameter);
    UNREFERENCED_PARAMETER(Timer);

    //
    // Do something when the timer fires.
    //
	printf(" %d MyTimerCallback: timer has fired.\n", GetCurrentThreadId());

}


//
// This is the thread pool work callback function.
//
VOID
CALLBACK
MyWorkCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                 Parameter,
    PTP_WORK              Work
    )
{
    // Instance, Parameter, and Work not used in this example.
    UNREFERENCED_PARAMETER(Instance);
    UNREFERENCED_PARAMETER(Parameter);
    UNREFERENCED_PARAMETER(Work);

    BOOL bRet = FALSE;
 
    //
    // Do something when the work callback is invoked.
    //
     {
		printf(" %d MyWorkCallback: Task performed.\n", GetCurrentThreadId());
     }

    return;
}

TP_CALLBACK_ENVIRON CallBackEnviron;

#define TASK_SELF_PTR p
#define DECLARE_TASK_LAMDA(codeblock)[]     \
    (PTP_CALLBACK_INSTANCE Instance,PVOID args,PTP_WORK Work)->void\
    {                                       \
        UNREFERENCED_PARAMETER(Instance);   \
        UNREFERENCED_PARAMETER(Work);       \
        Task* TASK_SELF_PTR =               \
        static_cast<Task*>(args);           \
        codeblock                           \
    }
class CancellationTokenSource;
class CancelToken
{
	friend class CancellationTokenSource;
private:
	bool* pCancel;
public:

};
class CancellationTokenSource
{
private:
    bool m_bCancel;
    CancelToken* p;
public:
    CancellationTokenSource() : m_bCancel(false), p(new CancelToken()){}
    ~CancellationTokenSource()
    {
        p->pCancel = NULL;
        delete p;
        p = NULL;
    }
    DECLARE_CONST_GETTER(bool, CancellationRequested, { return m_bCancel; })
    DECLARE_CONST_GETTER(WEAK_PTR(CancelToken), Token, { return p; })
    void Cancel() 
    {
        m_bCancel = true;
    }
};
class Task
{
public:
	PTP_WORK work{};
	void* context{}; 
    WEAK_PTR(CancelToken) pCancelToken = NULL;
public:
	Task(PTP_WORK_CALLBACK action)
	{

		WINAPI_ASSERT(work = CreateThreadpoolWork((PTP_WORK_CALLBACK)action, this, &CallBackEnviron), "Task construct failed");
	}
	~Task()
	{
		VOIDRET_ASSERT(work);
		CloseThreadpoolWork(work);
	}
	void Start(WEAK_PTR(CancelToken)cancel = NULL, void* c = NULL)
	{
		pCancelToken = cancel;
		context = c;
		SubmitThreadpoolWork(work);
	}
	void WaitOne()
	{
		WaitForThreadpoolWorkCallbacks(work, NULL == pCancelToken ? false : pCancelToken->);
	}

};
VOID
DemoCleanupPersistentWorkTimer()
{
    BOOL bRet = FALSE;
    PTP_TIMER timer = NULL;
    PTP_POOL pool = NULL;
    PTP_WORK_CALLBACK workcallback = MyWorkCallback;
    PTP_TIMER_CALLBACK timercallback = MyTimerCallback;
    PTP_CLEANUP_GROUP cleanupgroup = NULL;
    FILETIME FileDueTime;
    ULARGE_INTEGER ulDueTime;
    UINT rollback = 0;

    InitializeThreadpoolEnvironment(&CallBackEnviron);

    //
    // Create a custom, dedicated thread pool.
    //
    pool = CreateThreadpool(NULL);



    //
    // The thread pool is made persistent simply by setting
    // both the minimum and maximum threads to 1.
    //
    SetThreadpoolThreadMaximum(pool, 4);

    bRet = SetThreadpoolThreadMinimum(pool, 4);


    //
    // Create a cleanup group for this thread pool.
    //
    cleanupgroup = CreateThreadpoolCleanupGroup();

 


    //
    // Associate the callback environment with our thread pool.
    //
    SetThreadpoolCallbackPool(&CallBackEnviron, pool);

    //
    // Associate the cleanup group with our thread pool.
    // Objects created with the same callback environment
    // as the cleanup group become members of the cleanup group.
    //
    SetThreadpoolCallbackCleanupGroup(&CallBackEnviron,
                                      cleanupgroup,
                                      NULL);



    //
    // Submit the work to the pool. Because this was a pre-allocated
    // work item (using CreateThreadpoolWork), it is guaranteed to execute.
    //
    Task t(DECLARE_TASK_LAMDA
    (
        {
            VOIDRET_ASSERT(TASK_SELF_PTR);
			printf(" %d Begin MyWorkCallback: Task performed.\n", GetCurrentThreadId());
			Sleep(1000);
			if (NULL != TASK_SELF_PTR->pCancelToken && *TASK_SELF_PTR->pCancelToken)
			{
				return;
			}
			printf(" %d End MyWorkCallback: Task performed.\n", GetCurrentThreadId());
		}
    ));
    bool bCancelToken;
    for(int i = 0; i < 10; i++)
    {
        printf("begin work in main\n");
		bCancelToken = false;
        t.Start(&bCancelToken);
        Sleep(100);
        bCancelToken = true;
        t.WaitOne();
		printf("wait work finished in main\n");
    }
}

VOID
DemoNewRegisterWait()
{
    PTP_WAIT Wait = NULL;
    PTP_WAIT_CALLBACK waitcallback = MyWaitCallback;
    HANDLE hEvent = NULL;
    UINT i = 0;
    UINT rollback = 0;

    //
    // Create an auto-reset event.
    //
    hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    if (NULL == hEvent) {
        // Error Handling
        return;
    }

    rollback = 1; // CreateEvent succeeded

    Wait = CreateThreadpoolWait(waitcallback,
                                NULL,
                                NULL);

    if(NULL == Wait) {
        _tprintf(_T("CreateThreadpoolWait failed. LastError: %u\n"),
                     GetLastError());
        goto new_wait_cleanup;
    }

    rollback = 2; // CreateThreadpoolWait succeeded

    //
    // Need to re-register the event with the wait object
    // each time before signaling the event to trigger the wait callback.
    //
    for (i = 0; i < 5; i ++) {
        SetThreadpoolWait(Wait,
                          hEvent,
                          NULL);
        printf("...\n");
        SetEvent(hEvent);

        //
        // Delay for the waiter thread to act if necessary.
        //
        Sleep(500);

        //
        // Block here until the callback function is done executing.
        //

        WaitForThreadpoolWaitCallbacks(Wait, FALSE);
    }

new_wait_cleanup:
    switch (rollback) {
        case 2:
            // Unregister the wait by setting the event to NULL.
            SetThreadpoolWait(Wait, NULL, NULL);

            // Close the wait.
            CloseThreadpoolWait(Wait);

        case 1:
            // Close the event.
            CloseHandle(hEvent);

        default:
            break;
    }
    return;
}


int main( void)
{
	DemoCleanupPersistentWorkTimer();
	//DemoNewRegisterWait();
    return 0;
}
