/*
 * @Description: �ź��� https://docs.microsoft.com/en-us/windows/win32/sync/using-semaphore-objects
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
     * @brief ��ָ�������ź���������Ѿ����ڣ�
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
     * @brief �˳��ź���������ǰһ������
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
     * @brief ��ָ���Ĵ����˳��ź���������ǰһ������
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