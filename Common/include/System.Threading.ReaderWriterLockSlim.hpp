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
    class ReaderWriterLock;     /* ReaderWriterLockSlim �ķ�װ */
    class ConditionVariable;
};
/**
 * @brief �ؼ�����, ���������ȴ��Ļ�Ч�ʺܸߣ����ô��û�̬�л����ں�̬����ȴ���
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
 * @brief ��ʾ���ڹ�����Դ���ʵ�����״̬����ʵ�ֶ��̶߳�ȡ����ж�ռʽд�����
 * ReaderWriterLockSlim �����ܽ� CriticalSection �кܴ��������������Ϊ ReaderWriterLockSlim �ǻ���ԭ�ӷ��ʵģ��ؼ����ǻ����¼��ں˶���ģ����û�ģʽ���ں�ģʽ���л�ռ���˴�����ʱ������
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
     * @brief ���Խ����ȡģʽ����״̬, ���� WriteLock ʱ��������ǰ�߳�
     * 
     */
    inline void EnterReadLock()
    {
        AcquireSRWLockShared(&rwl);
    }
    /**
     * @brief ���Խ���д��ģʽ����״̬, ���� ReadLock ʱ��������ǰ�߳�
     * 
     */
    inline void EnterWriteLock()
    {
        AcquireSRWLockExclusive(&rwl);
    }
    /**
     * @brief ���ٶ�ȡģʽ�ĵݹ�������������ɵļ���Ϊ 0���㣩ʱ�˳���ȡģʽ
     * Enter �� Exit ������ͬһ���߳��гɶԳ���
     */
    inline void ExitReadLock()
    {
        ReleaseSRWLockShared(&rwl);
    }
    /**
     * @brief ����д��ģʽ�ĵݹ�������������ɵļ���Ϊ 0���㣩ʱ�˳�д��ģʽ
     * Enter �� Exit ������ͬһ���߳��гɶԳ���
     */
    inline void ExitWriteLock()
    {
        ReleaseSRWLockExclusive(&rwl);
    }
    /**
     * @brief ���Խ����ȡģʽ����״̬
     * 
     * @return true 
     * @return false ��ȡ Read Lock ʧ��
     */
    inline bool TryEnterReadLock()
    {
        return TryAcquireSRWLockShared(&rwl);
    }
    /**
     * @brief ���Խ���д��ģʽ����״̬
     * 
     * @return true 
     * @return false ��ȡ Write Lock ʧ��
     */
    inline bool TryEnterWriteLock()
    {
        return TryAcquireSRWLockExclusive(&rwl);
    }
};
/**
 * @brief ReaderWriterLockSlim ��װ
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
     * @brief ʹ��һ����ʱֵ��ȡ���߳���
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
     * @brief ʹ��һ����ʱֵ��ȡд�߳���
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
     * @brief �ͷ����������̻߳�ȡ���Ĵ������
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
     * @brief ���ٶ�������
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
     * @brief ����д������
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
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ�߳��Ƿ���ж��߳���
     * 
     * @return true 
     * @return false 
     */
    inline bool IsReaderLockHeld()
    {
        return m_nReadCount;
    }
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ�߳��Ƿ����д�߳���
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
 * @brief �ṩ CriticalSection, ReaderWriterLock, ReaderWriterLockSlim ͬ��������Ľӿ�
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
     * @brief ���� CriticalSection ����������ǰ�߳�, ֱ���� ConditionVariable �����ʱ
     * 
     * @param cs 
     * @param milliseconds 
     * @return true �� ConditionVariable �ɹ�����
     * @return false ��ʱ��ʧ��
     */
    inline bool SleepForCritcalSection(CriticalSection& cs, DWORD milliseconds = INFINITE)
    {
        return SleepConditionVariableCS(&cv, &cs.sec, milliseconds);
    }
    /**
     * @brief ���� ReaderWriterLockSlim ��ȡ����, ��������ǰ�߳�, ֱ���� ConditionVariable �����ʱ
     * 
     * @param rwl 
     * @param milliseconds 
     * @return true �� ConditionVariable �ɹ�����
     * @return false ��ʱ��ʧ��
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
     * @brief ���� ReaderWriterLockSlim д������, ��������ǰ�߳�, ֱ���� ConditionVariable �����ʱ
     * 
     * @param rwl 
     * @param milliseconds 
     * @return true �� ConditionVariable �ɹ�����
     * @return false ��ʱ��ʧ��
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