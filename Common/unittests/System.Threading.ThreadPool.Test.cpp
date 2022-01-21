/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-20 11:47:40
 * @LastEditors: like
 * @LastEditTime: 2022-01-21 17:20:09
 */
#include <System.DateTime.hpp>
#include <System.Threading.ThreadPool.hpp>
#include <System.Threading.EventWaitHandle.hpp>
#include <System.Threading.Mutex.hpp>
#include <System.Threading.Semaphore.hpp>
using namespace System;
using namespace System::Threading;

#define WORKCOUNT 30

#define ENABLE_THREAD_PRINTF
#ifdef ENABLE_THREAD_PRINTF
    // #define DBG_PRINT (printf("%s:%u %s:%s:\t", __FILE__, __LINE__, __DATE__, __TIME__), printf) 
    Mutex mtx;/* 进程退出, 操作系统释放 */
    inline void ThreadPrintf(const char *cmd, ...)  
    {  
        mtx.WaitOne();
        // printf("%s %s ", __DATE__, __TIME__);  
        va_list args;       //定义一个va_list类型的变量，用来储存单个参数  
        va_start(args,cmd); //使args指向可变参数的第一个参数  
        vprintf(cmd,args);  //必须用vprintf等带V的  
        va_end(args);       //结束可变参数的获取
        printf("\n");  
        mtx.ReleaseMutex();
    }
#else
    #define ThreadPrintf printf
#endif

struct TaskTestArgs
{
    Mutex* pMtx;
    AutoResetEvent* pAre;
    long* pCount;
    int nWorkId;
};
/**
 * 
 * 实验 1, 1 Log 记录
 * 1, 1, WT_EXECUTEDEFAULT 
    Work-0 : Thread-17356 Yiled
    Work-1 : Thread-20204 Yiled
    Work-2 : Thread-20744 Yiled
    Work-3 : Thread-20824 Yiled
    Work-3 : Thread-20824 Hello from the thread pool.
    Work-2 : Thread-20744 Hello from the thread pool.
    Work-1 : Thread-20204 Hello from the thread pool.
    Work-0 : Thread-17356 Hello from the thread pool.
    Work-4 : Thread-8300 Yiled
    Work-5 : Thread-10988 Yiled
    Work-6 : Thread-20864 Yiled
    Work-7 : Thread-4900 Yiled 
    Work-5 : Thread-10988 Hello from the thread pool.
    Work-4 : Thread-8300 Hello from the thread pool. 
    Work-7 : Thread-4900 Hello from the thread pool. 
    Work-6 : Thread-20864 Hello from the thread pool.
    Work-8 : Thread-19740 Yiled
    Work-8 : Thread-19740 Hello from the thread pool.
    Work-9 : Thread-19820 Yiled
    Work-9 : Thread-19820 Hello from the thread pool.

 * 1, 1, WT_EXECUTEINIOTHREAD
    Work-0 : Thread-20796 Yiled
    Work-1 : Thread-17280 Yiled
    Work-2 : Thread-9252 Yiled 
    Work-3 : Thread-16588 Yiled
    Work-3 : Thread-16588 Hello from the thread pool.
    Work-2 : Thread-9252 Hello from the thread pool. 
    Work-1 : Thread-17280 Hello from the thread pool.
    Work-0 : Thread-20796 Hello from the thread pool.
    Work-4 : Thread-11260 Yiled
    Work-5 : Thread-15748 Yiled
    Work-6 : Thread-9036 Yiled 
    Work-7 : Thread-21408 Yiled
    Work-5 : Thread-15748 Hello from the thread pool.
    Work-4 : Thread-11260 Hello from the thread pool.
    Work-7 : Thread-21408 Hello from the thread pool.
    Work-6 : Thread-9036 Hello from the thread pool. 
    Work-8 : Thread-19676 Yiled
    Work-8 : Thread-19676 Hello from the thread pool.
    Work-9 : Thread-19688 Yiled
    Work-9 : Thread-19688 Hello from the thread pool.

 * 1, 1, WT_EXECUTEINPERSISTENTTHREAD
    Work-0 : Thread-13496 Yiled
    Work-0 : Thread-13496 Hello from the thread pool.
    程序异常退出
    
 * 1, 1, WT_EXECUTELONGFUNCTION
    Work-0 : Thread-2784 Yiled 
    Work-1 : Thread-2600 Yiled 
    Work-2 : Thread-18840 Yiled
    Work-3 : Thread-4056 Yiled 
    Work-4 : Thread-16964 Yiled
    Work-5 : Thread-2408 Yiled 
    Work-6 : Thread-7496 Yiled 
    Work-7 : Thread-1312 Yiled 
    Work-8 : Thread-21024 Yiled
    Work-9 : Thread-19384 Yiled
    Work-5 : Thread-2408 Hello from the thread pool. 
    Work-9 : Thread-19384 Hello from the thread pool.
    Work-4 : Thread-16964 Hello from the thread pool.
    Work-3 : Thread-4056 Hello from the thread pool. 
    Work-8 : Thread-21024 Hello from the thread pool.
    Work-7 : Thread-1312 Hello from the thread pool. 
    Work-6 : Thread-7496 Hello from the thread pool. 
    Work-2 : Thread-18840 Hello from the thread pool.
    Work-1 : Thread-2600 Hello from the thread pool. 
    Work-0 : Thread-2784 Hello from the thread pool.
 
 * 1, 1, WT_TRANSFER_IMPERSONATION
    Work-0 : Thread-11236 Yiled
    Work-1 : Thread-13028 Yiled
    Work-2 : Thread-4708 Yiled 
    Work-3 : Thread-19152 Yiled
    Work-3 : Thread-19152 Hello from the thread pool.
    Work-2 : Thread-4708 Hello from the thread pool. 
    Work-1 : Thread-13028 Hello from the thread pool.
    Work-0 : Thread-11236 Hello from the thread pool.
    Work-4 : Thread-10324 Yiled
    Work-5 : Thread-15688 Yiled
    Work-6 : Thread-20580 Yiled
    Work-7 : Thread-19092 Yiled
    Work-4 : Thread-10324 Hello from the thread pool.
    Work-5 : Thread-15688 Hello from the thread pool.
    Work-6 : Thread-20580 Hello from the thread pool.
    Work-7 : Thread-19092 Hello from the thread pool.
    Work-8 : Thread-21428 Yiled
    Work-8 : Thread-21428 Hello from the thread pool.
    Work-9 : Thread-2588 Yiled
    Work-9 : Thread-2588 Hello from the thread pool.

 * 结论 :
    WT_EXECUTEDEFAULT : 管理全权托付给线程池
    WT_EXECUTEINIOTHREAD : 新的 API 中被废弃, 设置结果和 WT_EXECUTEDEFAULT 相同
    WT_EXECUTEINPERSISTENTTHREAD : 导致程序崩溃原因不明( 不是 //printf 争抢写屏幕资源导致 )
    WT_EXECUTELONGFUNCTION : 长时间运行的任务 ,  yiled 后抢回 contex 没有 WT_EXECUTEDEFAULT 积极
    WT_TRANSFER_IMPERSONATION : 相同任务多次执行, 执行顺序比 WT_EXECUTEDEFAULT 好

 * 实验 10, 10 Log 记录
    略
 * 结论 :
    任务整体执行完的时间更短, 还发生过 //printf 抢占屏幕资源导致的偶发崩溃
    
 */
void TaskTest()
{
    Mutex mtx;
    AutoResetEvent notifiEvent(false);
    long totalCount = 0;
    Object pWorkArgs[WORKCOUNT] = {0};
    for(int i = 0 ; i < WORKCOUNT; i++)
    {
        pWorkArgs[i] = new TaskTestArgs{&mtx, &notifiEvent, &totalCount, i};
        /*ThreadPool::Ref().QueueUserWorkItem
        (
            [](Object args)
            {
                TaskTestArgs* p = (TaskTestArgs*)args;
                size_t i = 0;
                do
                {
                    i++;
                }while(i < SIZE_MAX);
                
                p->pMtx->WaitOne();
                ++(*(p->pCount));
                printf("    Work-%d :  Hello from the thread pool.\n", p->nWorkId);
                p->pMtx->ReleaseMutex();
                if(WORKCOUNT == *(p->pCount))
                {
                    p->pAre->Set();
                }
            }, 
            pWorkArgs[i]
        );*/
        ThreadPool::Ref().TrySubmitThreadpoolCallback
        (
            [](PTP_CALLBACK_INSTANCE instance, Object args)
            {
                TaskTestArgs* p = (TaskTestArgs*)args;
                size_t i = 0;
                do
                {
                    i++;
                }while(i < SIZE_MAX);
                
                Interlocked<long>::Increment(*(p->pCount));
                char str[WORKCOUNT] = {0};
                memset(str, '*', *(p->pCount));
                printf("    %s\n", str); 
                if(WORKCOUNT == *(p->pCount)) /* work end notification */
                {
                    p->pAre->Set();
                }
            }, pWorkArgs[i]
        );
    }
    printf("Begin work int threadpool ...\n");
    notifiEvent.WaitOne();
    printf("All work finished in threadpool. total %d. \n", totalCount);
    
    // 对于简单重复的运算单线程真香
    printf("---------------------------------\n");
    totalCount = 0;
    printf("Begin work ...\n");
    for(int n = 0; n < WORKCOUNT; n++)
    {
        size_t i = 0;
        do
        {
            i++;
        }while(i < SIZE_MAX);
        ++totalCount;
        char str[WORKCOUNT] = {0};
        memset(str, '*', n);
        printf("    %s\n", str); 
    }
    printf("\nAll work finished. total %d. \n", totalCount);

    /* release memory */
    for(int i = 0; i < WORKCOUNT; i++)
    {
        delete pWorkArgs[i];
        pWorkArgs[i] = NULL;
    }
    notifiEvent.Dispose();
    mtx.Dispose();
}


/**
 * @brief 
 * Unregister 接口只能在回调函数没有触发过的时候可以成功 why???
 */
void RegisterTest()
{
    int nKey = 0;
    bool bExecOnlyOnce = true;
    int nTimerWait = bExecOnlyOnce ? 1000 : 500;
    AutoResetEvent eTimerWork(false);
    AutoResetEvent areWhenUnregist(false);

    RegisteredWaitHandle* rwh = ThreadPool::Ref().RegisterWaitForSingleObject
    (
        eTimerWork, 
        [](Object args, bool timeout)
        {
            if(timeout)
            {
                printf("    Monitor-%d Keyboard timeout\n", GetCurrentThreadId());
            }
            else
            {
                printf("    Monitor-%d Keyboard value %d\n", GetCurrentThreadId(), (int)args);
            }
        }, (Object)nKey, nTimerWait, bExecOnlyOnce
    );
    Sleep(1500);/* 主线程让出时间片让线程池处理回调 */
    eTimerWork.Reset();
    rwh->Unregister(areWhenUnregist);
    if(areWhenUnregist.WaitOne(1000))
    {
        printf("Unregister Sucess\n");
    }
    else
    {
        printf("Unregister Failed\n");
    }
    delete rwh;
    areWhenUnregist.Dispose();
    eTimerWork.Dispose();
}
int main()
{  
    /* Threadpool Set */
    if(!ThreadPool::Ref().SetMinThreads(6, 0))
    {
        printf("SetMinThreads Failed\n");
        return 1;
    }
    if(!ThreadPool::Ref().SetMaxThreads(64, 0))
    {
        printf("SetMinThreads Failed\n");
        return 1;
    }
    /* Test */
    TaskTest();
    RegisterTest();
    /* Dispose */
    ThreadPool::Ref().Dispose();
    printf("Destory Resources.\n");
    return 0;
}