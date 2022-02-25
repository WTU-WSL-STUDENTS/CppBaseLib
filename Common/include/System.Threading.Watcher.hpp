/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-16 15:13:02
 * @LastEditors: like
 * @LastEditTime: 2022-02-25 14:34:29
 */
#ifndef SYSTEM_THREADING_WATCHER_HPP
#define SYSTEM_THREADING_WATCHER_HPP
#include <System.Threading.ReaderWriterLockSlim.hpp>
#include <System.Threading.Interlocked.hpp>

namespace System::Threading
{
    template<typename T>
    class Watcher;
};
/**
 * @brief 提供进程内的同步访问对象的机制
 * reference :
 * - Wait               : WaitOnAddress         - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitonaddress
 * - WakeOne / WakeAll  : WakeByAddressSingle   - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-wakebyaddresssingle
 */
template<typename T>
class System::Threading::Watcher
{
public:
    /**
     * @brief 监控参数的值, 在参数未发生时一直挂起当前线程
     * 
     * @param obj 
     */
    static inline bool Wait(T* obj, DWORD millisecondsTimeout)
    {
        T* copy = (T*)malloc(sizeof(T));
        bool res = WaitOnAddress(obj, copy, sizeof(T), millisecondsTimeout);
        free(copy);
        return res;
    }
    static void Wait(T* obj)
    {
        T* copy = (T*)malloc(sizeof(T));
        memcpy(copy, obj, sizeof(T));
        do
        {
            if(!WaitOnAddress(obj, copy, sizeof(T), INFINITE))
            {
                WINAPI_ASSERT(false, Watcher::Wait() WaitOnAddress);
                break;
            }
        }while(0 == memcmp(copy, obj, sizeof(T)));/* obj 收到唤醒信号, 需要检查 obj 内容发生了改变才允许退出 */
        free(copy);
    }
    /**
     * @brief 唤醒等待
     * 
     * @param obj 
     */
    static inline void WakeOne(T* obj)
    {
        WakeByAddressSingle(obj);
    }
    /**
     * @brief 唤醒所有等待
     * 
     * @param obj 
     */
    static inline void WakeAll(T* obj)
    {
        WakeByAddressAll(obj);
    }
};
#endif