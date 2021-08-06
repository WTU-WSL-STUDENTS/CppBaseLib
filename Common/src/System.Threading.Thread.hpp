#ifndef THREADING_H
#define THREADING_H

#include <complied_entry.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#else
#define NOT_WINDOWS_PLANTFORM
#include <pthread.h>
#endif

namespace System::Threading /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.thread?view=net-5.0 */
{
    #if (defined NOT_WINDOWS_PLANTFORM)
        typedef pthread_t HANDLE;
        typedef void* THREAD_EVENT(void*);
        typedef void* THREAD_EVENT_ARGS;
        typedef int ThreadPriorty;
        #define Normal 0
    #else /* if (defined _WIN32)*/
        typedef void* HANDLE;
        typedef unsigned (__stdcall* THREAD_EVENT)(void*);
        typedef void* THREAD_EVENT_ARGS;
        #define THREAD_RETURN_ERROR 0xFFFFFFFF
        typedef enum _ThreadPriorty
        {
            Idle        = THREAD_PRIORITY_IDLE,         //���У���ͣ�
            Lowest      = THREAD_PRIORITY_LOWEST,       //��ͣ���ʵ�ǡ��ε͡���
            BelowNormal = THREAD_PRIORITY_BELOW_NORMAL, //���ڱ�׼
            Normal      = THREAD_PRIORITY_NORMAL,       //��׼
            AboveNormal = THREAD_PRIORITY_ABOVE_NORMAL, //���ڱ�׼
            Highest     = THREAD_PRIORITY_HIGHEST,      //��ߣ���ʵ�ǡ��θߡ���
            TimeCritical= THREAD_PRIORITY_TIME_CRITICAL //�ؼ�ʱ�䣨��ߣ�
        }ThreadPriorty;
    #endif
    typedef enum _ThreadStatus
    {
        NotInit = 0,
        Created,
        WaitingForActivation,   /* �ȴ����� */
        WaitingToRun,           /* �ȴ���ʼ*/
        Running,
        Canceled,
        Finished
    }ThreadStatus;
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
#if (defined NOT_WINDOWS_PLANTFORM)
    static void* ThreadEventContainer(void* args)
    {
        ThreadInfo* tInfo = (ThreadInfo*)args;
        THREAD_EVENT event = tInfo->Event;
        return (*event)(tInfo->Args);
    }
#else
    static unsigned long WINAPI ThreadEventContainer(void* args)
    {
        ThreadInfo* tInfo    = (ThreadInfo*)args;
        THREAD_EVENT event   = tInfo->Event;
        unsigned long ret    = (*event)(tInfo->Args);
        tInfo->CurrentStatus = Finished;
        return ret;
    }
#endif
public:
    ThreadInfo tInfo;
    Thread(THREAD_EVENT func, THREAD_EVENT_ARGS args, ThreadPriorty priorty = Normal)
    {
        tInfo.Event     = func;
        tInfo.Args      = args;
        tInfo.Priorty   = priorty;
        #if defined(NOT_WINDOWS_PLANTFORM)
            int ret = pthread_create(&tInfo.Handle, NULL, (THREAD_EVENT)ThreadEventContainer, (void*)&tInfo) ? NotInit : Created;
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
            tInfo.Handle = (HANDLE)_beginthreadex(0, 0, (THREAD_EVENT)ThreadEventContainer, (void*)&tInfo, CREATE_SUSPENDED, &tInfo.ThreadId);          
            if(NULL != tInfo.Handle)
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
    static void Sleep(DWORD millscend)
    {
        #ifdef _WIN32
        ::Sleep(millscend);
        #else
        usleep(millscend * 1000);
        #endif
    }
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
    bool IsRunning()
    {
        // DWORD nExitCode;&& (TRUE == ::GetExitCodeThread(tInfo.Handle, &nExitCode)) && (STILL_ACTIVE == nExitCode)
        return Running == tInfo.CurrentStatus ;
    }
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
    bool WaitThreadFinshed(DWORD millisecond = INFINITE)/* ���ȴ�millisecond���������Ĭ�������һֱ�� */
    {
        if(Running == tInfo.CurrentStatus)
        {
            if(WAIT_OBJECT_0 == WaitForSingleObject(tInfo.Handle, millisecond))
            {
                tInfo.CurrentStatus = Finished;
                return true;
            }
        }
        return Finished == tInfo.CurrentStatus;
    }
    bool ForceExit()/* ����Σ�յ�API, ǿ���߳̽����������ͷ��߳�ʹ�õģ���õ�������߳���Ȼ���н��� */
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
    bool Disopse()/* �����������ͷ��߳̾���ķ��� */
    {
        if(NULL != tInfo.Handle)
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