/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-19 16:19:50
 * @LastEditors: like
 * @LastEditTime: 2022-04-04 08:45:44
 */
#ifndef SYSTEM_THREADING_TIMER_HPP
#define SYSTEM_THREADING_TIMER_HPP

#include <System.Action.hpp>
#include <System.Threading.ThreadPool.hpp>

namespace System::Threading
{
    using TimerCallback = Action<Object>;
    class Timer;
};
class System::Threading::Timer final : public IDisposable//, public IAsyncDisposable
{
private:
    struct TimerContext
    {
        TimerCallback m_tcb;
        Object m_args;
    };
    PTP_TIMER m_pTimer;
    TimerContext m_context;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Timer)
    Timer(TimerCallback cb) : Timer(cb, NULL, -1, 0){}
    Timer(TimerCallback cb, Object args, DWORD dueTime, DWORD period) : m_pTimer(NULL), m_context(TimerContext{ cb, args })
    {
        WINAPI_ASSERT
        (
            m_pTimer = ::CreateThreadpoolTimer
            (
                DECLARE_TPLAMDA_TIMER
                ({
                    TimerContext* t = static_cast<TimerContext*>(TPLAMDA_CONTEXT);
                    t->m_tcb(t->m_args);
                }),
                &m_context,
                &ThreadPoolSingleton::Ref().environment
            ), 
            "Construct Timer failed"
        );
        Change(dueTime, period);
    }
    ~Timer()
    {
        Dispose();
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
        if (m_pTimer)
        {
            CloseThreadpoolTimer(m_pTimer);
            m_pTimer = NULL;
        }
    }
};

#endif