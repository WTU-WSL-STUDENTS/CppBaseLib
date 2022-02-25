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
//     Mutex mtx;/* 进程退出, 操作系统释放 */
//     inline void ThreadPrintf(const char *cmd, ...)  
//     {  
//         mtx.WaitOne();
//         // printf("%s %s ", __DATE__, __TIME__);  
//         va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
//         va_start(args,cmd); //使args指向可变参数的第一个参数  
//         vprintf(cmd,args);  //必须用vprintf等带V的  
//         va_end(args);       //结束可变参数的获取
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
//  * 实验 1, 1 Log 记录
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
//     程序异常退出
    
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

//  * 结论 :
//     WT_EXECUTEDEFAULT : 管理全权托付给线程池
//     WT_EXECUTEINIOTHREAD : 新的 API 中被废弃, 设置结果和 WT_EXECUTEDEFAULT 相同
//     WT_EXECUTEINPERSISTENTTHREAD : 导致程序崩溃原因不明( 不是 //printf 争抢写屏幕资源导致 )
//     WT_EXECUTELONGFUNCTION : 长时间运行的任务 ,  yiled 后抢回 contex 没有 WT_EXECUTEDEFAULT 积极
//     WT_TRANSFER_IMPERSONATION : 相同任务多次执行, 执行顺序比 WT_EXECUTEDEFAULT 好

//  * 实验 10, 10 Log 记录
//     略
//  * 结论 :
//     任务整体执行完的时间更短, 还发生过 //printf 抢占屏幕资源导致的偶发崩溃
    
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
//  * Unregister 接口只能在回调函数没有触发过的时候可以成功 why???
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
//     Sleep(1500);/* 主线程让出时间片让线程池处理回调 */
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
    _tprintf(_T("MyWaitCallback: wait is over.\n"));
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
    _tprintf(_T("MyTimerCallback: timer has fired.\n"));

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
         _tprintf(_T("MyWorkCallback: Task performed.\n"));
     }

    return;
}

VOID
DemoCleanupPersistentWorkTimer()
{
    BOOL bRet = FALSE;
    PTP_WORK work = NULL;
    PTP_TIMER timer = NULL;
    PTP_POOL pool = NULL;
    PTP_WORK_CALLBACK workcallback = MyWorkCallback;
    PTP_TIMER_CALLBACK timercallback = MyTimerCallback;
    TP_CALLBACK_ENVIRON CallBackEnviron;
    PTP_CLEANUP_GROUP cleanupgroup = NULL;
    FILETIME FileDueTime;
    ULARGE_INTEGER ulDueTime;
    UINT rollback = 0;

    InitializeThreadpoolEnvironment(&CallBackEnviron);

    //
    // Create a custom, dedicated thread pool.
    //
    pool = CreateThreadpool(NULL);

    if (NULL == pool) {
        _tprintf(_T("CreateThreadpool failed. LastError: %u\n"),
                     GetLastError());
        goto main_cleanup;
    }

    rollback = 1; // pool creation succeeded

    //
    // The thread pool is made persistent simply by setting
    // both the minimum and maximum threads to 1.
    //
    SetThreadpoolThreadMaximum(pool, 1);

    bRet = SetThreadpoolThreadMinimum(pool, 1);

    if (FALSE == bRet) {
        _tprintf(_T("SetThreadpoolThreadMinimum failed. LastError: %u\n"),
                     GetLastError());
        goto main_cleanup;
    }

    //
    // Create a cleanup group for this thread pool.
    //
    cleanupgroup = CreateThreadpoolCleanupGroup();

    if (NULL == cleanupgroup) {
        _tprintf(_T("CreateThreadpoolCleanupGroup failed. LastError: %u\n"), 
                     GetLastError());
        goto main_cleanup; 
    }

    rollback = 2;  // Cleanup group creation succeeded

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
    // Create work with the callback environment.
    //
    work = CreateThreadpoolWork(workcallback,
                                NULL, 
                                &CallBackEnviron);

    if (NULL == work) {
        _tprintf(_T("CreateThreadpoolWork failed. LastError: %u\n"),
                     GetLastError());
        goto main_cleanup;
    }

    rollback = 3;  // Creation of work succeeded

    //
    // Submit the work to the pool. Because this was a pre-allocated
    // work item (using CreateThreadpoolWork), it is guaranteed to execute.
    //
    for(int i = 0; i < 10; i++)
    {
        SubmitThreadpoolWork(work);
    }


    //
    // Create a timer with the same callback environment.
    //
    timer = CreateThreadpoolTimer(timercallback,
                                  NULL,
                                  &CallBackEnviron);


    if (NULL == timer) {
        _tprintf(_T("CreateThreadpoolTimer failed. LastError: %u\n"),
                     GetLastError());
        goto main_cleanup;
    }

    rollback = 4;  // Timer creation succeeded

    //
    // Set the timer to fire in one second.
    //
    ulDueTime.QuadPart = (ULONGLONG) -(1 * 10 * 1000 * 1000);
    FileDueTime.dwHighDateTime = ulDueTime.HighPart;
    FileDueTime.dwLowDateTime  = ulDueTime.LowPart;

    SetThreadpoolTimer(timer,
                       &FileDueTime,
                       0,
                       0);

    //
    // Delay for the timer to be fired
    //
    Sleep(1500);

    //
    // Wait for all callbacks to finish.
    // CloseThreadpoolCleanupGroupMembers also releases objects
    // that are members of the cleanup group, so it is not necessary 
    // to call close functions on individual objects 
    // after calling CloseThreadpoolCleanupGroupMembers.
    //
    CloseThreadpoolCleanupGroupMembers(cleanupgroup,
                                       FALSE,
                                       NULL);

    //
    // Already cleaned up the work item with the
    // CloseThreadpoolCleanupGroupMembers, so set rollback to 2.
    //
    rollback = 2;
    goto main_cleanup;

main_cleanup:
    //
    // Clean up any individual pieces manually
    // Notice the fall-through structure of the switch.
    // Clean up in reverse order.
    //

    switch (rollback) {
        case 4:
        case 3:
            // Clean up the cleanup group members.
            CloseThreadpoolCleanupGroupMembers(cleanupgroup,
                FALSE, NULL);
        case 2:
            // Clean up the cleanup group.
            CloseThreadpoolCleanupGroup(cleanupgroup);

        case 1:
            // Clean up the pool.
            CloseThreadpool(pool);

        default:
            break;
    }

    return;
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
    DemoNewRegisterWait();
    DemoCleanupPersistentWorkTimer();
    return 0;
}