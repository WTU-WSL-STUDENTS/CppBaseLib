/*
 * @Description: 信号量 https://docs.microsoft.com/en-us/windows/win32/sync/using-semaphore-objects
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-17 21:47:52
 * @LastEditors: like
 * @LastEditTime: 2022-01-18 11:57:09
 */
#ifndef SYSTEM_THREADING_SEMAPHORE_HPP
#define SYSTEM_THREADING_SEMAPHORE_HPP

#include <System.Threading.WaitHandle.hpp>

namespace System::Threading
{
    class Semaphore;
};

class System::Threading::Semaphore : public WaitHandle
{
protected:
    const char* m_strName;
    Semaphore(const char* strName) : m_strName(strName){}
public:
    Semaphore(int initialCount, int maximumCount, const char* strName = NULL) : m_strName(strName),
        WaitHandle(CreateSemaphore(NULL, initialCount, maximumCount, strName))
    {
        if(m_bDisposed)
        {
            printf("Create Semaphore Failed , Error Code : %d", GetLastError());
        }
    }
    /**
     * @brief 打开指定名称信号量（如果已经存在）
     * 
     * @param strName 
     * @return Semaphore 
     */
    inline static Semaphore OpenExisting(const char* strName)
    {
        Semaphore semaphore(strName);
        if(semaphore.m_bDisposed = NULL == (semaphore.m_hWaitHandle = OpenSemaphore(EVENT_ALL_ACCESS, false, strName)))
        {
            printf("Semaphore::OpenExisting Failed , Error Code : %d\n", GetLastError());
        }     
        return semaphore;
    }
    /**
     * @brief 退出信号量并返回前一个计数
     * 
     * @return long 
     */
    long Release()
    {
        long lPreviousCount; 
        if(ReleaseSemaphore(m_hWaitHandle, 1, &lPreviousCount))
            return lPreviousCount;
        printf("ReleaseSemaphore Failed , Error Code : %d\n", GetLastError());
        return -1;
    }
    /**
     * @brief 以指定的次数退出信号量并返回前一个计数
     * 
     * @param releaseCount 
     * @return long 
     */
    long Release(long releaseCount)
    {
        long lPreviousCount; 
        if(ReleaseSemaphore(m_hWaitHandle, releaseCount, &lPreviousCount))
            return lPreviousCount;
        printf("ReleaseSemaphore Failed , Error Code : %d\n", GetLastError());
        return -1;
    }
};

#endif