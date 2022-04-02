/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-19 16:19:50
 * @LastEditors: like
 * @LastEditTime: 2022-04-02 11:47:34
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
    PTP_TIMER m_pTimer;
    TimerCallback m_tcb;
    Object m_args;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Timer)
    Timer(TimerCallback cb) : Timer(cb, NULL, -1, 0){}
    Timer(TimerCallback cb, Object args, DWORD dueTime, DWORD period) : m_pTimer(NULL), m_tcb(cb), m_args(args)
    {
        WINAPI_ASSERT
        (
            m_pTimer = ::CreateThreadpoolTimer
            (
                DECLARE_TPLAMDA_TIMER
                ({
                    Timer* pArgs = static_cast<Timer*>(TPLAMDA_CONTEXT); 
                    // (*pArgs->cb)(pArgs->args);
                }),
                this, 
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
        CloseThreadpoolTimer(m_pTimer);
    }
    /**
     * @brief �ͷ� Timer �ĵ�ǰʵ��ʹ�õ�������Դ�����ͷ����ʱ��ʱ�����ź�
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