/*
 * @Description: 互斥锁
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-17 21:05:10
 * @LastEditors: like
 * @LastEditTime: 2022-01-21 11:10:19
 */
#ifndef SYSTEM_THREADING_MUTEX_HPP
#define SYSTEM_THREADING_MUTEX_HPP

#include <System.Threading.WaitHandle.hpp>

namespace System::Threading
{
    class Mutex;        /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.mutex.releasemutex?view=net-5.0 */
    template<typename T>
    class Interlocked;  /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.interlocked?view=net-5.0 */
};

/**
 * @brief 互斥锁
 * Refrence
 * - CreateMutex / ReleaseMutex - https://docs.microsoft.com/en-us/windows/win32/sync/using-mutex-objects
 */
class System::Threading::Mutex : public WaitHandle
{
protected:
    const char* m_strName;
    Mutex(const char* strName, HANDLE h) : m_strName(strName), WaitHandle(h){}
public:
    Mutex(const char* strName = NULL) : m_strName(strName), WaitHandle(CreateMutex(NULL, false, strName))
    {
        if(m_bDisposed)
        {
            printf("Create Mutex Failed , Error Code : %d", GetLastError());
        }
    }
    /**
     * @brief 互斥体解除阻塞
     * 
     */
    inline void ReleaseMutex()
    {
        if(!::ReleaseMutex(m_hWaitHandle))
        {
            printf("ReleaseMutex Failed , Error Code : %d\n", GetLastError());
        }
    }
    /**
     * @brief 打开指定的已命名的互斥体
     * 
     * @param strName 
     * @return Mutex 
     */
    static Mutex OpenExisting(const char* strName)
    {
        HANDLE h = OpenMutex(EVENT_ALL_ACCESS, false, strName);
        if(NULL == h)
        {
            printf("Mutex::OpenExisting Failed , Error Code : %d\n", GetLastError());
        }  
        return Mutex(strName, h);
    }
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
};

#endif