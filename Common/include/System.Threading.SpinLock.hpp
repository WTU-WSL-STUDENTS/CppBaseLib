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
 * @brief 每个线程占用一个自旋锁
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
     * @brief 获取已对此实例调用 SpinOnce() 的次数
     * 
     * @return size_t 
     */
    inline size_t Count()
    {
        return m_nCount;
    }
    /**
     * @brief 获取对 SpinOnce() 的下一次调用是否将产生处理器，同时触发强制上下文切换
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
            if(SLEEP_1_EVERY_HOW_MANY_TIMES - 1 == count % SLEEP_1_EVERY_HOW_MANY_TIMES) /* 当前线程添加到等待列队, 放弃剩余时间片, 几个周期内不参加时间片竞争 */
            {
                Sleep(1);
            }
            else if(SLEEP_0_EVERY_HOW_MANY_TIMES - 1 == count % SLEEP_0_EVERY_HOW_MANY_TIMES) /* 当前线程添加到就绪列队, 放弃剩余时间片, 立刻重新进行一次CPU竞争 */
            {
                Sleep(0);
            }
            else
            {
                YieldProcessor();/* 当前线程添加到就绪列队 */
            }
        }
        m_nCount = (m_nCount == SIZE_MAX ? YIELD_THRESHOLD : m_nCount + 1);
    }
    /**
     * @brief 提供自定义自旋算法接口
     * 对于 CPU 依据自旋数分为以下几个级别的时间片占用 :  
     * hold on -> yield -> sleep(0) -> sleep(1)
     * @param needSleep1 执行 Sleep1 的条件
     * @param needSleep0 执行 Sleep0 的条件
     * @param yieldThreshold 执行 yield 的阈值
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
     * @brief 获取锁当前是否已由任何线程占用
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
     * @brief 获取锁, 如果锁被占用开始自旋争抢 cpu 调度
     * SpinLock 是非重入锁，这意味着，如果线程持有锁，则不允许再次进入该锁
     * Enter 和 Exit 应该成对出现在一个线程中
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
     * @brief 释放锁
     * 
     */
    void Exit()
    {
        // ERROR_ASSERT(IsHeldByCurrentThread(), "Current thread has no access");
        Interlocked<long>::Decrement(m_nStatus);
    }
    /**
     * @brief 非阻塞式获取锁
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