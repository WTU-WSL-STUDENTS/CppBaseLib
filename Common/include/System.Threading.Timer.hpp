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
     * @brief ��ǰϵͳ�̳߳��ж�ʱ���ĸ���
     * 
     * @return long 
     */
    static inline long ActiveCount()
    {
        throw "Not Support";
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