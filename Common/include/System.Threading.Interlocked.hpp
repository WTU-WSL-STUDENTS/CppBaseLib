/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-15 15:49:24
 * @LastEditors: like
 * @LastEditTime: 2022-02-15 15:49:25
 */
#ifndef SYSTEM_THREADING_INTERLOCKED_HPP
#define SYSTEM_THREADING_INTERLOCKED_HPP
#include <System.Threading.WaitHandle.hpp>

namespace System::Threading
{
    template<typename T>
    class Interlocked;  /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.interlocked?view=net-5.0 */
};

/**
 * @brief Ϊ����̹߳���ı����ṩԭ�Ӳ��� (֧�����޵�����)
 * 
 * @tparam T 
 */
template<typename T>
class System::Threading::Interlocked
{
private:
    Interlocked(){}
public:
    /**
     * @brief ����������Ͳ��ú��滻��һ������������������Ϊһ��ԭ�Ӳ������
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long
     */
    static inline T Add(T& location, const T& value)
    {
        return ::InterlockedAdd(&location, value);    
    }
    /**
     * @brief �����������а�λ���롱���㲢�ý���滻��һ������������������Ϊһ��ԭ�Ӳ������
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T And(T& location, const T& value)
    {
        return ::InterlockedAnd(&location, value);    
    }
    /**
     * @brief �Ƚ�����ֵ�Ƿ���ȣ������ȣ����滻��һ��ֵ
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T CompareExchange(T& location, const T& value)
    {
        return ::InterlockedCompareExchange(&location, value);    
    }
    /**
     * @brief ��ԭ�Ӳ�������ʽ�ݼ�ָ��������ֵ���洢���
     * 
     * @param location 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Decrement(T& location)
    {
        return ::InterlockedDecrement(&location);    
    }
    /**
     * @brief ��ԭ�Ӳ�������ʽ����������Ϊָ����ֵ
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Exchange(T& location, const T& value)
    {
        return ::InterlockedExchange(&location, value);    
    }
    /**
     * @brief ��ԭ�Ӳ�������ʽ����ָ��������ֵ���洢���
     * 
     * @param location 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Increment(T& location)
    {
        return ::InterlockedIncrement(&location);    
    }
    /**
     * @brief �����������а�λ�������㲢�ý���滻��һ������������������Ϊһ��ԭ�Ӳ������
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Or(T& location, const T& value)
    {
        return ::InterlockedOr(&location, value);    
    }
    /**
     * @brief ִ�е�ǰ�̵߳Ĵ������ڶ�ָ������(re-ordering)ʱ�����ܲ�����ִ�� MemoryBarrier() ����֮����ڴ��ȡ����ִ�� MemoryBarrier() ����֮ǰ���ڴ��ȡ�ķ�ʽ
     * 
     * reference :
     * https://zhuanlan.zhihu.com/p/126284925
     * https://docs.microsoft.com/en-us/windows/win32/api/winnt/nf-winnt-memorybarrier
     * 
     * Tips:
     * �ڴ��������ڽ���ڴ����� (��д�Ĵ���˳����ִ��˳��һ��)
     * ## �ڴ������ԭ��
     * 1. �������Ż����� ָ������
     * 2. ��� CPU cache �������
     * ## �ڴ�����������
     * 1. �������ԭ�Ӳ��� atomic �� volatile     https://zhuanlan.zhihu.com/p/126284925 
     * 2. mb , rmb, wmb                         http://www.360doc.com/content/21/1022/11/15690396_1000823792.shtml 
     */
    static inline void MemoryBarrier()
    {
#if defined(__linux)
        __sync_synchronize();
#elif defined(__APPLE__)
        OSMemoryBarrier();
#else
        /* ������x86�ܹ��Ĵ���������ǿ˳���ڴ�ģ�� (Strong ordered) */
        ::MemoryBarrier();
#endif
    }
};

#endif