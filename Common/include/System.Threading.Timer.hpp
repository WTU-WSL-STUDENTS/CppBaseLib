/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-19 16:19:50
 * @LastEditors: like
 * @LastEditTime: 2022-03-11 16:20:07
 */
#ifndef SYSTEM_THREADING_TIMER_HPP
#define SYSTEM_THREADING_TIMER_HPP

#include <System.Threading.ThreadPool.hpp>
#include <functional>

namespace System::Threading
{
    typedef void (*TimerCallback)(Object);
    class Timer;
};
class System::Threading::Timer final : public IDisposable//, public IAsyncDisposable
{
private:
    struct _TimerCallbackArgs
    {
        TimerCallback cb;
        Object args;
    };
    static void __stdcall _TimerCallback(PTP_CALLBACK_INSTANCE Instance,PVOID args, PTP_TIMER sender)
    {
        UNREFERENCED_PARAMETER(Instance);
        UNREFERENCED_PARAMETER(sender);
        _TimerCallbackArgs* pArgs = (_TimerCallbackArgs*)args; 
        (*pArgs->cb)(pArgs->args);
    }

    PTP_TIMER m_pTimer;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Timer)
    Timer(TimerCallback cb) : Timer(cb, NULL, -1, -1){}
    Timer(TimerCallback cb, Object args, DWORD dueTime, DWORD period)
    {
        _TimerCallbackArgs tArgs = {cb, args};
        if(NULL == (m_pTimer = ::CreateThreadpoolTimer(_TimerCallback, &tArgs, NULL/* the callback executes in the default callback environment */)))
        {
            printf("System::Threading::Timer CreateThreadpoolTimer Failed , Error Code : %d", GetLastError());
            return;
        }
        Change(dueTime, period);
    }
    ~Timer()
    {
        Dispose();
    }
    /**
     * @brief 当前系统线程池中定时器的个数
     * 
     * @return long 
     */
    static inline long ActiveCount()
    {
        throw "Not Support";
    }
    /**
     * @brief 更改计时器的启动时间和方法调用之间的间隔
     * 
     * @param dueTime 启用定时器之前的延时时间
     * @param period 调用回调方法的时间间隔 , 设为 0 回调函数只会执行一次 , 设为 -1 永不执行
     * @return true 
     * @return false 
     */
    inline bool Change (DWORD dueTime, DWORD period)
    {
        ULARGE_INTEGER ulDueTime;
        ulDueTime.QuadPart = ((ULONGLONG) -(1 * 10 * 1000)) * ((ULONGLONG)dueTime);
        FILETIME fileDueTime;
        fileDueTime.dwHighDateTime = ulDueTime.HighPart;
        fileDueTime.dwLowDateTime  = ulDueTime.LowPart;
        SetThreadpoolTimer(m_pTimer, &fileDueTime, period, 0/* 误差 */);
        return true;
    }
    /**
     * @brief 释放由 Timer 的当前实例使用的所有资源
     * 
     */
    void Dispose() override
    {
        CloseThreadpoolTimer(m_pTimer);
    }
    /**
     * @brief 释放 Timer 的当前实例使用的所有资源并在释放完计时器时发出信号
     * 
     * @param waitHandle 
     */
    void Dispose(WaitHandle waitHandle) 
    {
        CloseThreadpoolTimer(m_pTimer);
        PTP_CALLBACK_INSTANCE instance;
        SetEventWhenCallbackReturns(instance, waitHandle.Handle());
    }
};

#endif