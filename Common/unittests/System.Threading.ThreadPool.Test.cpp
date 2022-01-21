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
    Mutex mtx;/* �����˳�, ����ϵͳ�ͷ� */
    inline void ThreadPrintf(const char *cmd, ...)  
    {  
        mtx.WaitOne();
        // printf("%s %s ", __DATE__, __TIME__);  
        va_list args;       //����һ��va_list���͵ı������������浥������  
        va_start(args,cmd); //ʹargsָ��ɱ�����ĵ�һ������  
        vprintf(cmd,args);  //������vprintf�ȴ�V��  
        va_end(args);       //�����ɱ�����Ļ�ȡ
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
 * ʵ�� 1, 1 Log ��¼
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
    �����쳣�˳�
    
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

 * ���� :
    WT_EXECUTEDEFAULT : ����ȫȨ�и����̳߳�
    WT_EXECUTEINIOTHREAD : �µ� API �б�����, ���ý���� WT_EXECUTEDEFAULT ��ͬ
    WT_EXECUTEINPERSISTENTTHREAD : ���³������ԭ����( ���� //printf ����д��Ļ��Դ���� )
    WT_EXECUTELONGFUNCTION : ��ʱ�����е����� ,  yiled ������ contex û�� WT_EXECUTEDEFAULT ����
    WT_TRANSFER_IMPERSONATION : ��ͬ������ִ��, ִ��˳��� WT_EXECUTEDEFAULT ��

 * ʵ�� 10, 10 Log ��¼
    ��
 * ���� :
    ��������ִ�����ʱ�����, �������� //printf ��ռ��Ļ��Դ���µ�ż������
    
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
    
    // ���ڼ��ظ������㵥�߳�����
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
 * Unregister �ӿ�ֻ���ڻص�����û�д�������ʱ����Գɹ� why???
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
    Sleep(1500);/* ���߳��ó�ʱ��Ƭ���̳߳ش���ص� */
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