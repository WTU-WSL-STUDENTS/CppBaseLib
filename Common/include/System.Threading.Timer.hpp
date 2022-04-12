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
     * @brief ���ļ�ʱ��������ʱ��ͷ�������֮��ļ��
     * 
     * @param dueTime ���ö�ʱ��֮ǰ����ʱʱ��
     * @param period ���ûص�������ʱ���� , ��Ϊ 0 �ص�����ֻ��ִ��һ�� , ��Ϊ -1 ����ִ��
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
        SetThreadpoolTimer(m_pTimer, &fileDueTime, period, 0/* ��� */);
        return true;
    }
    /**
     * @brief �ͷ��� Timer �ĵ�ǰʵ��ʹ�õ�������Դ
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