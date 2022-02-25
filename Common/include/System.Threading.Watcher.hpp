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
 * @brief �ṩ�����ڵ�ͬ�����ʶ���Ļ���
 * reference :
 * - Wait               : WaitOnAddress         - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitonaddress
 * - WakeOne / WakeAll  : WakeByAddressSingle   - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-wakebyaddresssingle
 */
template<typename T>
class System::Threading::Watcher
{
public:
    /**
     * @brief ��ز�����ֵ, �ڲ���δ����ʱһֱ����ǰ�߳�
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
        }while(0 == memcmp(copy, obj, sizeof(T)));/* obj �յ������ź�, ��Ҫ��� obj ���ݷ����˸ı�������˳� */
        free(copy);
    }
    /**
     * @brief ���ѵȴ�
     * 
     * @param obj 
     */
    static inline void WakeOne(T* obj)
    {
        WakeByAddressSingle(obj);
    }
    /**
     * @brief �������еȴ�
     * 
     * @param obj 
     */
    static inline void WakeAll(T* obj)
    {
        WakeByAddressAll(obj);
    }
};
#endif