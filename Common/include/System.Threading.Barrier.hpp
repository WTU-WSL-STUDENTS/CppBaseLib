/*
 * @Description: 
    内存屏障用于解决内存乱序 (编写的代码顺序与执行顺序不一致)
    ## 内存乱序的原因
    1. 编译器优化导致 指令重排
    2. 多核 CPU cache 乱序访问
    ## 内存乱序解决方案
    1. 定义变量原子操作 atomic 或 volatile     https://zhuanlan.zhihu.com/p/126284925 
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
 * @brief 使多个任务能够采用并行方式依据某种算法在多个阶段中协同工作
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
     * @brief 使多个任务能够采用并行方式依据某种算法在多个阶段中协同工作
     * 
     * @param participantCount 参与线程的数量
     * @param spinCount 线程执行完切换 context 之前的自旋数, -1 默认值自旋 2000 次
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
     * @brief 获取屏障的当前阶段的编号
     * 
     * @return long 
     */
    inline long GetCurrentPhaseNumber()
    {
        return m_nCurrentPhaseNumber;
    }
    /**
     * @brief 获取屏障中参与者的总数
     * 
     * @return long 
     */
    inline long GetParticipantCount()
    {
        return m_nParticipantCount;
    }
    /**
     * @brief 发出参与者已达到屏障并切换 context 等待所有其他参与者也达到屏障
     * 
     * @return true 最后一个到达屏障
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
     * @brief 发出参与者已达到屏障并阻塞线程等待所有其他参与者也达到屏障, 等待时不会切换 context
     * 
     * @return true 最后一个到达屏障
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