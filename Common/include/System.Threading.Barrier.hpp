/*
 * @Description: 
    �ڴ��������ڽ���ڴ����� (��д�Ĵ���˳����ִ��˳��һ��)
    ## �ڴ������ԭ��
    1. �������Ż����� ָ������
    2. ��� CPU cache �������
    ## �ڴ�����������
    1. �������ԭ�Ӳ��� atomic �� volatile     https://zhuanlan.zhihu.com/p/126284925 
    2. mb , rmb, wmb                         http://www.360doc.com/content/21/1022/11/15690396_1000823792.shtml 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-14 16:23:30
 * @LastEditors: like
 * @LastEditTime: 2022-03-11 16:23:21
 */
#ifndef SYSTEM_THREADING_BARRIER_HPP
#define SYSTEM_THREADING_BARRIER_HPP
#include <System.Threading.EventWaitHandle.hpp>
#include <System.Threading.Interlocked.hpp>

namespace System::Threading
{
    class Barrier;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.barrier.signalandwait?view=net-5.0#System_Threading_Barrier_SignalAndWait */
};
/**
 * @brief ʹ��������ܹ����ò��з�ʽ����ĳ���㷨�ڶ���׶���Эͬ����
 * https://docs.microsoft.com/en-us/windows/win32/sync/synchronization-barriers
 * 
 */
class System::Threading::Barrier final
{
private:
    SYNCHRONIZATION_BARRIER barrier;
    long m_nCurrentPhaseNumber;
    long m_nParticipantCount;
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Barrier)
    /**
     * @brief ʹ��������ܹ����ò��з�ʽ����ĳ���㷨�ڶ���׶���Эͬ����
     * 
     * @param participantCount �����̵߳�����
     * @param spinCount �߳�ִ�����л� context ֮ǰ��������, -1 Ĭ��ֵ���� 2000 ��
     */
    Barrier(long participantCount, long spinCount = -1) : m_nParticipantCount(participantCount), m_nCurrentPhaseNumber(-1)
    {
        WINAPI_ASSERT(InitializeSynchronizationBarrier(&barrier, participantCount, spinCount), Barrier());
    }
    ~Barrier()
    {
        DeleteSynchronizationBarrier(&barrier);
    }
    /**
     * @brief ��ȡ���ϵĵ�ǰ�׶εı��
     * 
     * @return long 
     */
    inline long GetCurrentPhaseNumber()
    {
        return m_nCurrentPhaseNumber;
    }
    /**
     * @brief ��ȡ�����в����ߵ�����
     * 
     * @return long 
     */
    inline long GetParticipantCount()
    {
        return m_nParticipantCount;
    }
    /**
     * @brief �����������Ѵﵽ���ϲ��л� context �ȴ���������������Ҳ�ﵽ����
     * 
     * @return true ���һ����������
     * @return false 
     */
    inline bool SignalAndWait()
    {
        if(EnterSynchronizationBarrier(&barrier, 0 /* SYNCHRONIZATION_BARRIER_FLAGS_BLOCK_ONLY */))
        {
            Interlocked<long>::Increment(m_nCurrentPhaseNumber);
            return true;
        }
        return false;
    }
    /**
     * @brief �����������Ѵﵽ���ϲ������̵߳ȴ���������������Ҳ�ﵽ����, �ȴ�ʱ�����л� context
     * 
     * @return true ���һ����������
     * @return false 
     */
    inline bool SignalAndHoldOn()
    {
        if(EnterSynchronizationBarrier(&barrier, SYNCHRONIZATION_BARRIER_FLAGS_SPIN_ONLY))
        {
            Interlocked<long>::Increment(m_nCurrentPhaseNumber);
            return true;
        }
        return false;
    }
};
#endif