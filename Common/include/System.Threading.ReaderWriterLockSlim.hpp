/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-15 09:40:37
 * @LastEditors: like
 * @LastEditTime: 2022-03-11 17:14:38
 */
#ifndef SYSTEM_THREADING_READERWRITERLOCKSLIM_HPP
#define SYSTEM_THREADING_READERWRITERLOCKSLIM_HPP
#include <System.Threading.EventWaitHandle.hpp>

namespace System::Threading
{
    class CriticalSection;
    class ReaderWriterLockSlim; /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.readerwriterlockslim?view=net-5.0 */
    class ReaderWriterLock;     /* ReaderWriterLockSlim 的封装 */
    class ConditionVariable;
};
/**
 * @brief 关键区域, 如果不进入等待的话效率很高（不用从用户态切换至内核态挂起等待）
 * reference:
 * https://docs.microsoft.com/en-us/windows/win32/sync/using-critical-section-objects
 * https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-initializecriticalsection
 * https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-initializecriticalsectionandspincount
 */
class System::Threading::CriticalSection final
{
    friend class ConditionVariable;
private:
    CRITICAL_SECTION sec;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(CriticalSection)
    CriticalSection(DWORD spinCount = 0)
    {
        if(spinCount)
            InitializeCriticalSectionAndSpinCount(&sec, spinCount);
        else
            InitializeCriticalSection(&sec);
    }
    ~CriticalSection()
    {
        DeleteCriticalSection(&sec);
    }
    inline void Enter()
    {
        EnterCriticalSection(&sec); 
    }
    inline void Leave()
    {
        LeaveCriticalSection(&sec);
    }
};
/**
 * @brief 表示用于管理资源访问的锁定状态，可实现多线程读取或进行独占式写入访问
 * ReaderWriterLockSlim 的性能较 CriticalSection 有很大的提升，这是因为 ReaderWriterLockSlim 是基于原子访问的，关键段是基于事件内核对象的，从用户模式到内核模式的切换占用了大量的时钟周期
 * reference:
 * https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-initializesrwlock
 * https://docs.microsoft.com/en-us/windows/win32/sync/slim-reader-writer--srw--locks
 */
class System::Threading::ReaderWriterLockSlim
{
    friend class ConditionVariable;
private:
    SRWLOCK rwl;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(ReaderWriterLockSlim)
    ReaderWriterLockSlim()
    {
        InitializeSRWLock(&rwl);
    }
    ~ReaderWriterLockSlim(){}
    /**
     * @brief 尝试进入读取模式锁定状态, 存在 WriteLock 时会阻塞当前线程
     * 
     */
    inline void EnterReadLock()
    {
        AcquireSRWLockShared(&rwl);
    }
    /**
     * @brief 尝试进入写入模式锁定状态, 存在 ReadLock 时会阻塞当前线程
     * 
     */
    inline void EnterWriteLock()
    {
        AcquireSRWLockExclusive(&rwl);
    }
    /**
     * @brief 减少读取模式的递归计数，并在生成的计数为 0（零）时退出读取模式
     * Enter 和 Exit 必须在同一个线程中成对出现
     */
    inline void ExitReadLock()
    {
        ReleaseSRWLockShared(&rwl);
    }
    /**
     * @brief 减少写入模式的递归计数，并在生成的计数为 0（零）时退出写入模式
     * Enter 和 Exit 必须在同一个线程中成对出现
     */
    inline void ExitWriteLock()
    {
        ReleaseSRWLockExclusive(&rwl);
    }
    /**
     * @brief 尝试进入读取模式锁定状态
     * 
     * @return true 
     * @return false 获取 Read Lock 失败
     */
    inline bool TryEnterReadLock()
    {
        return TryAcquireSRWLockShared(&rwl);
    }
    /**
     * @brief 尝试进入写入模式锁定状态
     * 
     * @return true 
     * @return false 获取 Write Lock 失败
     */
    inline bool TryEnterWriteLock()
    {
        return TryAcquireSRWLockExclusive(&rwl);
    }
};
/**
 * @brief ReaderWriterLockSlim 封装
 * 
 */
class System::Threading::ReaderWriterLock : private ReaderWriterLockSlim
{
    friend class ConditionVariable;
private:
    ManualResetEvent wmre;
    ManualResetEvent rmre;
    size_t m_nReadCount;
    size_t m_nWriteCount;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(ReaderWriterLock)
    ReaderWriterLock() : wmre(ManualResetEvent(false)), rmre(ManualResetEvent(false)), m_nReadCount(0), m_nWriteCount(0){}
    ~ReaderWriterLock(){}
    /**
     * @brief 使用一个超时值获取读线程锁
     * 
     * @param milliseconds 
     */
    void AcquireReaderLock(DWORD milliseconds = INFINITE)
    {
        if(!TryEnterReadLock())
        {
            if(!rmre.WaitOne(milliseconds))
            {
                throw "AcquireReaderLock Timeout";
            }
            if(!TryEnterReadLock())
            {
                throw "TryEnterReadLock Failed"; 
            }
        }
    }
    /**
     * @brief 使用一个超时值获取写线程锁
     * 
     * @param milliseconds 
     */
    void AcquireWriterLock(DWORD milliseconds = INFINITE)
    {
        if(!TryEnterWriteLock())
        {
            if(!wmre.WaitOne(milliseconds))
            {
                throw "AcquireWriterLock Timeout";
            }
            if(!TryEnterWriteLock())
            {
                throw "TryEnterWriteLock Failed"; 
            }
        }
    }
    /**
     * @brief 释放锁，不管线程获取锁的次数如何
     * 
     */
    inline void ReleaseLock()
    {
        ReleaseWriterLock();
        if(IsReaderLockHeld())
        {
            for(size_t i = 0; i < m_nReadCount; i++)
            {
                ReleaseReaderLock(); 
            }
        }
        ReleaseWriterLock();
    }
    /**
     * @brief 减少读锁计数
     * 
     */
    inline void ReleaseReaderLock()
    {
        if(IsReaderLockHeld())
        {
            ExitReadLock(); 
            m_nReadCount--;  
            if(0 == m_nReadCount)
            {
                wmre.Reset();
            }
        }
    }
    /**
     * @brief 减少写锁计数
     * 
     */
    inline void ReleaseWriterLock()
    {
        if(IsWriterLockHeld())
        {
            ExitWriteLock();
            m_nWriteCount--;
            rmre.Reset();
        }
    }
    /**
     * @brief 获取一个值，该值指示当前线程是否持有读线程锁
     * 
     * @return true 
     * @return false 
     */
    inline bool IsReaderLockHeld()
    {
        return m_nReadCount;
    }
    /**
     * @brief 获取一个值，该值指示当前线程是否持有写线程锁
     * 
     * @return true 
     * @return false 
     */
    inline bool IsWriterLockHeld()
    {
        return m_nWriteCount;
    }
};
/**
 * @brief 提供 CriticalSection, ReaderWriterLock, ReaderWriterLockSlim 同步锁挂起的接口
 * reference :
 * https://docs.microsoft.com/en-us/windows/win32/sync/using-condition-variables
 */
class System::Threading::ConditionVariable
{
private:
    CONDITION_VARIABLE cv;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(ConditionVariable)
    ConditionVariable()
    {
        InitializeConditionVariable(&cv);
    }
    ~ConditionVariable(){}
    /**
     * @brief 进入 CriticalSection 后主动挂起当前线程, 直到被 ConditionVariable 唤起或超时
     * 
     * @param cs 
     * @param milliseconds 
     * @return true 被 ConditionVariable 成功唤醒
     * @return false 超时或失败
     */
    inline bool SleepForCritcalSection(CriticalSection& cs, DWORD milliseconds = INFINITE)
    {
        return SleepConditionVariableCS(&cv, &cs.sec, milliseconds);
    }
    /**
     * @brief 进入 ReaderWriterLockSlim 读取锁后, 主动挂起当前线程, 直到被 ConditionVariable 唤起或超时
     * 
     * @param rwl 
     * @param milliseconds 
     * @return true 被 ConditionVariable 成功唤醒
     * @return false 超时或失败
     */
    inline bool SleepForRWLSReadLock(ReaderWriterLockSlim& rwl, DWORD milliseconds = INFINITE)
    {
        return SleepConditionVariableSRW(&cv, &(rwl.rwl), milliseconds, CONDITION_VARIABLE_LOCKMODE_SHARED);
    }
    inline bool SleepForRWLSReadLock(ReaderWriterLock& rwl, DWORD milliseconds = INFINITE)
    {
        return SleepConditionVariableSRW(&cv, &(rwl.rwl), milliseconds, CONDITION_VARIABLE_LOCKMODE_SHARED);
    }
    /**
     * @brief 进入 ReaderWriterLockSlim 写入锁后, 主动挂起当前线程, 直到被 ConditionVariable 唤起或超时
     * 
     * @param rwl 
     * @param milliseconds 
     * @return true 被 ConditionVariable 成功唤醒
     * @return false 超时或失败
     */
    inline bool SleepForRWLSWriteLock(ReaderWriterLockSlim& rwl, DWORD milliseconds = INFINITE)
    {
        return SleepConditionVariableSRW(&cv, &(rwl.rwl), milliseconds, 0);
    }
    inline bool SleepForRWLSWriteLock(ReaderWriterLock& rwl, DWORD milliseconds = INFINITE)
    {
        return SleepConditionVariableSRW(&cv, &(rwl.rwl), milliseconds, 0);
    }
    /**
     * @brief Waking one thread is similar to setting an auto-reset event
     * 
     */
    inline void WakeOne()
    {
        WakeConditionVariable(&cv);
    }
    /**
     * @brief waking all threads is similar to pulsing a manual reset event but more reliable
     * 
     */
    inline void WakeAll()
    {
        WakeAllConditionVariable(&cv);
    }
};

#endif