#ifndef THREADING_H
#define THREADING_H

#include <CompliedEntry.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#define NOT_WINDOWS_PLANTFORM
#include <pthread.h>
#endif

namespace System::Threading /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.thread?view=net-5.0 */
{
    #if (defined _WIN32)
        typedef void* HANDLE;
        typedef unsigned (__stdcall* THREAD_EVENT)(void*);
        typedef void* THREAD_EVENT_ARGS;
        #define THREAD_RETURN_ERROR 0xFFFFFFFF
        typedef enum _ThreadPriorty
        {
            Idle        = THREAD_PRIORITY_IDLE,         //空闲（最低）
            Lowest      = THREAD_PRIORITY_LOWEST,       //最低（其实是“次低”）
            BelowNormal = THREAD_PRIORITY_BELOW_NORMAL, //低于标准
            Normal      = THREAD_PRIORITY_NORMAL,       //标准
            AboveNormal = THREAD_PRIORITY_ABOVE_NORMAL, //高于标准
            Highest     = THREAD_PRIORITY_HIGHEST,      //最高（其实是“次高”）
            TimeCritical= THREAD_PRIORITY_TIME_CRITICAL //关键时间（最高）
        }ThreadPriorty;
    #else
        typedef pthread_t HANDLE;
        typedef void* THREAD_EVENT(void*);
        typedef void* THREAD_EVENT_ARGS;
        typedef int ThreadPriorty;
        #define Normal 0
    #endif
    typedef enum _ThreadStatus
    {
        NotInit = 0,
        Created,
        WaitingForActivation,   /* 等待安排 */
        WaitingToRun,           /* 等待开始*/
        Running,
        Canceled,
        Finished
    }ThreadStatus;
    typedef void (*OnThreadStart)   (void* sender, int /* ThreadStatus */ args);
    typedef void (*OnThreadFinished)(void* sender, int /* ThreadStatus */ args);
    typedef struct tagThreadInfo
    {
        HANDLE Handle;
        unsigned int ThreadId;
        THREAD_EVENT Event;
        THREAD_EVENT_ARGS Args;
        int CurrentStatus;
        ThreadPriorty Priorty;
        bool CanCancel;
    }ThreadInfo;

    class Thread;
}

class System::Threading::Thread
{
private:
#if (defined _WIN32)
    /**
     * @description: Windows线程函数的容器，通过该函数提供OnStart/OnFinished事件
     * @param 线程函数的参数
     * @return 返回线程函数执行结果
     * @author: like
     */    
    static unsigned long WINAPI ThreadEventContainer(void* args)
    {
        Thread* p = (Thread*)args;
        ThreadInfo* tInfo = &p->tInfo;
        if(p->OnStart)
        {
            p->OnStart(p, tInfo->CurrentStatus);
        }
        THREAD_EVENT event   = tInfo->Event;
        unsigned long ret    = (*event)(tInfo->Args);
        if(p->OnFinished)
        {
            p->OnFinished(p, tInfo->CurrentStatus);
        }
        tInfo->CurrentStatus = Finished;
        return ret;
    }
#else
    /**
     * @description: Linux线程函数的容器，通过该函数提供OnStart/OnFinished事件
     * @param 线程函数的参数
     * @return 返回线程函数执行结果
     * @author: like
     */    
    static void* ThreadEventContainer(void* args)
    {
        Thread* p = (Thread*)args;
        ThreadInfo* tInfo = &p->tInfo;
        if(p->OnStart)
        {
            p->OnStart(p, tInfo->CurrentStatus);
        }
        THREAD_EVENT event = tInfo->Event;
        void* ret = (*event)(tInfo->Args);
        if(p->OnFinished)
        {
            p->OnFinished(p, tInfo->CurrentStatus);
        }
        tInfo->CurrentStatus = Finished;
        return ret;
    }
#endif
public:
    ThreadInfo tInfo;
    OnThreadStart OnStart;
    OnThreadFinished OnFinished;
    Thread(THREAD_EVENT func, THREAD_EVENT_ARGS args, ThreadPriorty priorty = Normal):OnStart(NULL), OnFinished(NULL)
    {
        tInfo.Event     = func;
        tInfo.Args      = args;
        tInfo.Priorty   = priorty;
        #if defined(NOT_WINDOWS_PLANTFORM)
            int ret = pthread_create(&tInfo.Handle, NULL, (THREAD_EVENT)ThreadEventContainer, (void*)this) ? NotInit : Created;
            if(0 == ret)
            {
                if(Normal != priorty)
                {
                    tInfo.CurrentStatus = 0 != pthread_setschedprio(tInfo.Handle, tInfo.Priorty) ? NotInit : Created;
                }
                else
                {
                    tInfo.CurrentStatus = Created;
                }
            }
            else
            {
                tInfo.CurrentStatus = NotInit;
            }
        #else
            tInfo.Handle = (HANDLE)_beginthreadex(0, 0, (THREAD_EVENT)ThreadEventContainer, (void*)this, CREATE_SUSPENDED, &tInfo.ThreadId);          
            if( tInfo.Handle)
            {
                tInfo.CurrentStatus = Normal == priorty ? Created : TRUE == SetThreadPriority(tInfo.Handle, tInfo.Priorty) ? Created : NotInit;
            }
            else
            {
                tInfo.CurrentStatus = NotInit;
            }
        #endif
    }
    Thread(THREAD_EVENT func): Thread(func, NULL){}
    ~Thread(){Disopse();}
    /**
     * @description:阻塞调用该函数的线程 
     * @param 阻塞当前线程指定时间(毫秒)
     * @return 
     * @author: like
     */    
    static void Sleep(DWORD millscend)
    {
        #ifdef _WIN32
        ::Sleep(millscend);
        #else
        usleep(millscend * 1000);
        #endif
    }
    /**
     * @description:启动线程，Created是线程是否创建的标志，如果不处于“创建完毕”的状态会直接返回false。
     * @param 
     * @return 返回线程是否启动成功，如果启动成功会将线程状态切换至Running
     * @author: like
     */    
    bool Start()
    {
        if(Created != tInfo.CurrentStatus)
        {
            return false;
        }
        bool bRet = THREAD_RETURN_ERROR != ResumeThread(tInfo.Handle);
        if(bRet)
        {
            tInfo.CurrentStatus = Running;
        }
        return bRet;
    }
    /**
     * @description: 判断当前线程是否处于运行状态 
     * @param 
     * @return 返回当前线程是否启动
     * @author: like
     */    
    bool IsRunning()
    {
        // DWORD nExitCode;&& (TRUE == ::GetExitCodeThread(tInfo.Handle, &nExitCode)) && (STILL_ACTIVE == nExitCode)
        return Running == tInfo.CurrentStatus ;
    }
    /**
     * @description:暂停 
     * @param
     * @return 返回暂停结果，该函数并非线程安全，未测试最好不用
     * @author: like
     */    
    bool Pause()
    {     
        if(Running != tInfo.CurrentStatus)
        {
            return WaitingToRun == tInfo.CurrentStatus;
        }
        bool bRet = THREAD_RETURN_ERROR != SuspendThread(tInfo.Handle);      
        if(bRet)
        {
            tInfo.CurrentStatus = WaitingToRun;
        }
        return bRet;
    }
    /**
     * @description:此方法调用的线程会被阻塞，直到timeout或当前线程调用的函数执行完毕
     * @param 设置timeout单位毫秒，默认为一直阻塞直到线程退出
     * @return 返回当前线程执行的函数是否执行完毕。如果函数已经执行完毕会立刻返回true,
     * 如果函数还在执行最多等待millisecond，返回函数是否执行完毕的结果。如果线程处于其
     * 他情况，会立刻返回false
     * @author: like
     */ 
    bool WaitThreadFinshed(DWORD millisecond = INFINITE)/* 最多等待millisecond毫秒如果，默认情况是一直等 */
    {
        if(Finished == tInfo.CurrentStatus)
        {
            return true;
        }
        if(Running == tInfo.CurrentStatus)
        {
            if(WAIT_OBJECT_0 == WaitForSingleObject(tInfo.Handle, millisecond))
            {
                tInfo.CurrentStatus = Finished;
                return true;
            }
            else
            {
                printf("WaitForSingleObject Finished Timeout\n");
            }
        }
        return false;
    }
    /**
     * @description:此方法调用的线程会被阻塞，直到timeout或当前线程调用的函数执行完毕
     * @param 设置timeout单位毫秒，默认为一直阻塞直到线程退出
     * @return 返回当前线程执行的函数是否执行完毕。如果函数已经执行完毕会立刻返回true,
     * 如果函数还在执行最多等待millisecond，返回函数是否执行完毕的信号。如果线程处于其
     * 他情况，会立刻返回false
     * @author: like
     */    
    bool WaitThreadCanceled(DWORD millisecond = INFINITE)/* 最多等待millisecond毫秒如果，默认情况是一直等 */
    {
        if(Finished == tInfo.CurrentStatus)
        {
            return true;
        }
        if(Canceled == tInfo.CurrentStatus)
        {
            if(WAIT_OBJECT_0 == WaitForSingleObject(tInfo.Handle, millisecond))
            {
                tInfo.CurrentStatus = Finished;
                return true;
            }
            else
            {
                printf("WaitForSingleObject Cancel Failed, Error Code:%d\n", GetLastError());
            }
        }
        return false;
    }
    /**
     * @description:线程退出，该方法存在强制关闭线程可能导致线程内存无法释放
     * @param
     * @return 返回强制关闭线程的结果
     * @author: like
     */    
    bool ForceExit()/* 这是危险的API, 强制线程结束并不会释放线程使用的，最好的设计是线程自然运行结束 */
    {
        if(Running != tInfo.CurrentStatus)
        {
            return Finished == tInfo.CurrentStatus;
        }
        if(tInfo.ThreadId == GetCurrentThreadId())
        {
            _endthreadex(0);
        }
        else
        {
            if(!TerminateThread(tInfo.Handle, 0))
            {
                return false;
            }
        }
        tInfo.CurrentStatus = Finished;
        return true;
    }
    bool Disopse()/* 析构函数中释放线程句柄的方法 */
    {
        if( tInfo.Handle)
        {
            if(!CloseHandle(tInfo.Handle))
            {
                return false;
            }
            tInfo.CurrentStatus = NotInit;
        }
        return true;
    }
};

#endif