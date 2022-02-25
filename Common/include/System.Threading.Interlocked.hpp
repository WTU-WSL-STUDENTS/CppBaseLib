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
 * @brief 为多个线程共享的变量提供原子操作 (支持有限的类型)
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
     * @brief 对两个数求和并用和替换第一个参数，上述操作作为一个原子操作完成
     * 
     * @param location 
     * @param value 
     * @return T 支持数据内存类型 long
     */
    static inline T Add(T& location, const T& value)
    {
        return ::InterlockedAdd(&location, value);    
    }
    /**
     * @brief 对两个数进行按位“与”运算并用结果替换第一个参数，上述操作作为一个原子操作完成
     * 
     * @param location 
     * @param value 
     * @return T 支持数据内存类型 long 和 long long
     */
    static inline T And(T& location, const T& value)
    {
        return ::InterlockedAnd(&location, value);    
    }
    /**
     * @brief 比较两个值是否相等，如果相等，则替换第一个值
     * 
     * @param location 
     * @param value 
     * @return T 支持数据内存类型 long 和 long long
     */
    static inline T CompareExchange(T& location, const T& value)
    {
        return ::InterlockedCompareExchange(&location, value);    
    }
    /**
     * @brief 以原子操作的形式递减指定变量的值并存储结果
     * 
     * @param location 
     * @return T 支持数据内存类型 long 和 long long
     */
    static inline T Decrement(T& location)
    {
        return ::InterlockedDecrement(&location);    
    }
    /**
     * @brief 以原子操作的形式将变量设置为指定的值
     * 
     * @param location 
     * @param value 
     * @return T 支持数据内存类型 long 和 long long
     */
    static inline T Exchange(T& location, const T& value)
    {
        return ::InterlockedExchange(&location, value);    
    }
    /**
     * @brief 以原子操作的形式递增指定变量的值并存储结果
     * 
     * @param location 
     * @return T 支持数据内存类型 long 和 long long
     */
    static inline T Increment(T& location)
    {
        return ::InterlockedIncrement(&location);    
    }
    /**
     * @brief 对两个数进行按位“或”运算并用结果替换第一个参数，上述操作作为一个原子操作完成
     * 
     * @param location 
     * @param value 
     * @return T 支持数据内存类型 long 和 long long
     */
    static inline T Or(T& location, const T& value)
    {
        return ::InterlockedOr(&location, value);    
    }
    /**
     * @brief 执行当前线程的处理器在对指令重排(re-ordering)时，不能采用先执行 MemoryBarrier() 调用之后的内存存取，再执行 MemoryBarrier() 调用之前的内存存取的方式
     * 
     * reference :
     * https://zhuanlan.zhihu.com/p/126284925
     * https://docs.microsoft.com/en-us/windows/win32/api/winnt/nf-winnt-memorybarrier
     * 
     * Tips:
     * 内存屏障用于解决内存乱序 (编写的代码顺序与执行顺序不一致)
     * ## 内存乱序的原因
     * 1. 编译器优化导致 指令重排
     * 2. 多核 CPU cache 乱序访问
     * ## 内存乱序解决方案
     * 1. 定义变量原子操作 atomic 或 volatile     https://zhuanlan.zhihu.com/p/126284925 
     * 2. mb , rmb, wmb                         http://www.360doc.com/content/21/1022/11/15690396_1000823792.shtml 
     */
    static inline void MemoryBarrier()
    {
#if defined(__linux)
        __sync_synchronize();
#elif defined(__APPLE__)
        OSMemoryBarrier();
#else
        /* 常见的x86架构的处理器采用强顺序内存模型 (Strong ordered) */
        ::MemoryBarrier();
#endif
    }
};

#endif