/*
 * @Description: https://blog.csdn.net/weixin_40378837/article/details/113143449synchronized 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-17 21:47:52
 * @LastEditors: like
 * @LastEditTime: 2022-03-11 16:08:51
 */
#ifndef SYSTEM_THREADING_SPINLOKC_HPP
#define SYSTEM_THREADING_SPINLOKC_HPP
#include <System.Threading.WaitHandle.hpp>
#include <System.Threading.Interlocked.hpp>
#include <functional>

#define YIELD_THRESHOLD 2000
#define SLEEP_0_EVERY_HOW_MANY_TIMES 5
#define SLEEP_1_EVERY_HOW_MANY_TIMES 20

typedef std::function<bool(const size_t&)> NeedEnterSleep1;
typedef std::function<bool(const size_t&)> NeedEnterSleep0;

namespace System::Threading
{
    class SpinWait;
    class SpinLock;
};
/**
 * @brief ÿ���߳�ռ��һ��������
 * 
 */
class System::Threading::SpinWait
{
private:
    size_t m_nCount;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(SpinWait)
    SpinWait() : m_nCount(0){}
    ~SpinWait(){}
    /**
     * @brief ��ȡ�ѶԴ�ʵ������ SpinOnce() �Ĵ���
     * 
     * @return size_t 
     */
    inline size_t Count()
    {
        return m_nCount;
    }
    /**
     * @brief ��ȡ�� SpinOnce() ����һ�ε����Ƿ񽫲�����������ͬʱ����ǿ���������л�
     * 
     * @return true 
     * @return false 
     */
    inline bool WillNextSpinYield()
    {
#ifdef IS_SINGLE_PROCESSOR
        return true;
#else
        return m_nCount > YIELD_THRESHOLD;
#endif
    }
    inline void SpinOnce()
    {
        if(WillNextSpinYield())
        { 
#ifdef IS_SINGLE_PROCESSOR
            size_t count = m_nCount >= YIELD_THRESHOLD ? m_nCount - YIELD_THRESHOLD : m_nCount;
#else
            size_t count = m_nCount - YIELD_THRESHOLD;
#endif
            if(SLEEP_1_EVERY_HOW_MANY_TIMES - 1 == count % SLEEP_1_EVERY_HOW_MANY_TIMES) /* ��ǰ�߳���ӵ��ȴ��ж�, ����ʣ��ʱ��Ƭ, ���������ڲ��μ�ʱ��Ƭ���� */
            {
                Sleep(1);
            }
            else if(SLEEP_0_EVERY_HOW_MANY_TIMES - 1 == count % SLEEP_0_EVERY_HOW_MANY_TIMES) /* ��ǰ�߳���ӵ������ж�, ����ʣ��ʱ��Ƭ, �������½���һ��CPU���� */
            {
                Sleep(0);
            }
            else
            {
                YieldProcessor();/* ��ǰ�߳���ӵ������ж� */
            }
        }
        m_nCount = (m_nCount == SIZE_MAX ? YIELD_THRESHOLD : m_nCount + 1);
    }
    /**
     * @brief �ṩ�Զ��������㷨�ӿ�
     * ���� CPU ������������Ϊ���¼��������ʱ��Ƭռ�� :  
     * hold on -> yield -> sleep(0) -> sleep(1)
     * @param needSleep1 ִ�� Sleep1 ������
     * @param needSleep0 ִ�� Sleep0 ������
     * @param yieldThreshold ִ�� yield ����ֵ
     */
    inline void SpinOnce(NeedEnterSleep1 needSleep1, NeedEnterSleep0 needSleep0, size_t yieldThreshold)
    {
#ifdef IS_SINGLE_PROCESSOR
        size_t count = m_nCount >= YIELD_THRESHOLD ? m_nCount - YIELD_THRESHOLD : m_nCount;
#else
        if(m_nCount > yieldThreshold)
        { 
            size_t count = m_nCount - YIELD_THRESHOLD;
#endif
            if(needSleep1(count))
            {
                Sleep(1);
            }
            else if(needSleep0(count))
            {
                Sleep(0);
            }
            else
            {
                YieldProcessor();
            }
#ifndef IS_SINGLE_PROCESSOR 
        }
#endif
        m_nCount = (m_nCount == SIZE_MAX ? YIELD_THRESHOLD : m_nCount + 1);
    }
    void Reset()
    {
        m_nCount = 0;
    }
};
class System::Threading::SpinLock
{
private:
    long m_nStatus;
    DWORD m_nThreadId;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(SpinLock)
    SpinLock() : m_nStatus(0), m_nThreadId(-1){}
    ~SpinLock(){}
    /**
     * @brief ��ȡ����ǰ�Ƿ������κ��߳�ռ��
     * 
     * @return true 
     * @return false 
     */
    inline bool IsHeld()
    {
        return m_nStatus;
    }
    inline bool IsHeldByCurrentThread()
    {
        return IsHeld() && m_nThreadId == GetCurrentThreadId();
    }
    /**
     * @brief ��ȡ��, �������ռ�ÿ�ʼ�������� cpu ����
     * SpinLock �Ƿ�������������ζ�ţ�����̳߳��������������ٴν������
     * Enter �� Exit Ӧ�óɶԳ�����һ���߳���
     */
    void Enter()
    {
        if(!IsHeld())
        {
            Interlocked<long>::Increment(m_nStatus);
            Interlocked<DWORD>::Exchange(m_nThreadId, GetCurrentThreadId());
            return;
        }
        // ERROR_ASSERT(m_nThreadId == GetCurrentThreadId(), "Can't enter spin lock twice");
        SpinWait sw;
        do
        {
            sw.SpinOnce();
        }while(IsHeld());
    }
    /**
     * @brief �ͷ���
     * 
     */
    void Exit()
    {
        // ERROR_ASSERT(IsHeldByCurrentThread(), "Current thread has no access");
        Interlocked<long>::Decrement(m_nStatus);
    }
    /**
     * @brief ������ʽ��ȡ��
     * 
     * @return true 
     * @return false 
     */
    bool TryEnter()
    {
        if(!IsHeld())
        {
            Interlocked<long>::Increment(m_nStatus);
            Interlocked<DWORD>::Exchange(m_nThreadId, GetCurrentThreadId());
            return true;
        }
        ERROR_ASSERT(m_nThreadId == GetCurrentThreadId(), "Can't enter spin lock twice");
        return false;
    }
};
#endif