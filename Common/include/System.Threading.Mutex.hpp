/*
 * @Description: 互斥锁
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-17 21:05:10
 * @LastEditors: like
 * @LastEditTime: 2022-03-11 16:22:47
 */
#ifndef SYSTEM_THREADING_MUTEX_HPP
#define SYSTEM_THREADING_MUTEX_HPP
#include <System.Threading.WaitHandle.hpp>

namespace System::Threading
{
    class Mutex;        /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.mutex.releasemutex?view=net-5.0 */
};
/**
 * @brief 互斥锁
 * Refrence
 * - CreateMutex / ReleaseMutex - https://docs.microsoft.com/en-us/windows/win32/sync/using-mutex-objects
 */
class System::Threading::Mutex final : public WaitHandle
{
protected:
    const char* m_strName;
    Mutex(const char* strName, HANDLE h) : m_strName(strName), WaitHandle(h){}
public:
    DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(Mutex)
    Mutex(const char* strName = NULL) : m_strName(strName), WaitHandle(CreateMutex(NULL, false, strName))
    {
        if(m_bDisposed)
        {
            printf("Create Mutex Failed , Error Code : %d", GetLastError());
        }
    }
    ~Mutex()
    {
        Dispose();
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
#endif