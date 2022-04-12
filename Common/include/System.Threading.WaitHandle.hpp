/*
 * @Description: Ŀǰ��֧�� windows ƽ̨
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-17 15:23:58
 * @LastEditors: like
 * @LastEditTime: 2022-04-03 23:23:14
 */
#ifndef SYSTEM_THREADING_WAITHANDLE_HPP
#define SYSTEM_THREADING_WAITHANDLE_HPP

#include <CompliedEntry.h>
#ifndef COMPLIER_MSVC
#pragma error(fatal, -1, "Windows Plantform Support Only : System.Threading.WaitHandle.hpp")
#endif
#include <System.IDisposable.h>
#include <Windows.h>
#include <stdio.h>

namespace System::Threading
{
    class WaitHandle;           /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.waithandle.waittimeout?view=net-5.0 */
}

/**
 * @brief ��װ�ȴ��Թ�����Դ���ж�ռ���ʵĲ���ϵͳ�ض��Ķ���
 * # Refrence
 * - WaitOne            : WaitForSingleObject       - https://docs.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
 * - WaitAll / WaitAny  : WaitForMultipleObjects    - https://docs.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-waitformultipleobjects
 * - SignalAndWait      : SignalObjectAndWait       - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-signalobjectandwait
 */
class System::Threading::WaitHandle : public IDisposable
{
protected:
    HANDLE  m_hWaitHandle;
    bool    m_bDisposed;
    WaitHandle() : m_hWaitHandle(NULL),m_bDisposed(true){}
    WaitHandle(HANDLE h) : m_hWaitHandle(h), m_bDisposed(NULL == m_hWaitHandle){}
    /**
     * @brief �ṩ������һ�������� WaitHandle �ͷŹ���
     * 
     * @param disposing ������������Ƿ���øýӿڵ��ͷ�ʵ��
     */
    virtual void Dispose(bool disposing)
    {
        if(disposing && !m_bDisposed)
        {
            if(!CloseHandle(m_hWaitHandle))
            {
                printf("System::Threading::WaitHandle Dispose Failed , Error Code : %d, %p\n", GetLastError(), m_hWaitHandle);
                return;
            }
            m_hWaitHandle = NULL;
            m_bDisposed = true;
        }
    }
public:
    virtual ~WaitHandle()
    {
        Dispose(true);
    }
    const int WaitTimeout = 258;
    /**
     * @brief ��ȡ�����ñ�������ϵͳ���
     * 
     * @return HANDLE& 
     */
    inline HANDLE& Handle()
    {
        return m_hWaitHandle;
    }
    /**
     * @brief �ͷ� WaitHandle ��ĵ�ǰʵ����ʹ�õ�������Դ
     * 
     */
    virtual void Close()
    {
        Dispose(true);
    }
    /**
     * @brief �ͷ� WaitHandle ��ĵ�ǰʵ����ʹ�õ�������Դ
     * 
     */
    void Dispose() override
    {
        Dispose(true);
    } 
    /**
     * @brief �����ǰʵ���յ��źţ���Ϊ true������Ϊ false��
     * 
     * @param millisecond �ȴ��ĺ�����, -1 ��ʾ�����ڵȴ�
     * @return true 
     * @return false 
     */
    virtual bool WaitOne(DWORD millisecond = INFINITE)
    {
        int status;
        if(WAIT_OBJECT_0 == (status = WaitForSingleObject(m_hWaitHandle, millisecond)))
        {
            return true;
        }
        WINAPI_ASSERT(WAIT_TIMEOUT == status, "WaitOne WaitForSingleObject Failed");
        return false;
    }
    /**
     * @brief �ȴ�ָ�������е�����Ԫ�ض��յ��ź�
     * 
     * @param waitHandles 
     * @param count 
     * @return true 
     */
    static inline bool WaitAll(WaitHandle** waitHandles, DWORD count)
    {
        return WaitAll(waitHandles, count, INFINITE);
    }
    /**
     * @brief �ȴ�ָ�������е�����Ԫ�ؽ����źŻ򵽴�ָ��ʱ����Ϊֹ
     * 
     * @param waitHandles 
     * @param count 
     * @param millisecond 
     * @return true 
     * @return false 1. ��ʱ
     */
    static bool WaitAll(WaitHandle** waitHandles, DWORD count, DWORD millisecond)
    {
        if(count > MAXIMUM_WAIT_OBJECTS)
        {
            printf("WaitAll  Support Max Handle Count : 64");
            return false;
        }
        HANDLE* handles = (HANDLE*)malloc(sizeof(HANDLE) * count);
        for(DWORD i = 0; i < count; i++)
        {
            handles[i] = waitHandles[i]->m_hWaitHandle;
        }
        DWORD status =  WaitForMultipleObjects(count, handles, true, millisecond);
        free(handles);
        if(status < WAIT_ABANDONED_0)
        {
            return true;
        }
        //else if(WAIT_TIMEOUT == status)
        //{
        //    printf("WaitAll WaitForMultipleObjects Timeout\n");
        //    return false;
        //}
        //else if(WAIT_FAILED == status)
        //{
        //    printf("WaitAll WaitForMultipleObjects Failed, Error Code : %d\n", GetLastError());
        //    return false;
        //}
        //else if (WAIT_ABANDONED_0 >= status && status <= (status - WAIT_ABANDONED_0))/* ��Ȼû��ͨ���ȴ�, �������߳��Ѿ����ͷ� */
        //{
        //    waitHandles[status - WAIT_ABANDONED_0]->Dispose(); 
        //}
        WINAPI_ASSERT(WAIT_TIMEOUT == status, "WaitAll WaitForSingleObject Failed");
        return false;
    }
    /**
     * @brief �ȴ�ָ�������е���һԪ���յ��ź�
     * 
     * @param waitHandles 
     * @param count 
     * @return int ��������
     */
    static int WaitAny(WaitHandle** waitHandles, DWORD count)
    {
        return WaitAny(waitHandles, count, INFINITE);
    }
    /**
     * @brief �ȴ�ָ�������е�����Ԫ�ؽ����źŻ򵽴�ָ��ʱ����, ���һ�ε� MAXIMUM_WAIT_OBJECTS ��
     * 
     * @param waitHandles 
     * @param count 
     * @param millisecond 
     * @return int 1. �ɹ��ȴ�������������; 2. ��ʱ���� millisecond; 3. �ȴ�ʧ�ܷ��� -1
     */
    static int WaitAny(WaitHandle** waitHandles, DWORD count, DWORD millisecond)
    {
        if(count > MAXIMUM_WAIT_OBJECTS)
        {
            printf("WaitAny  Support Max Handle Count : 64");
            return false;
        }
        HANDLE* handles = (HANDLE*)malloc(sizeof(HANDLE) * count);
        for(DWORD i = 0; i < count; i++)
        {
            handles[i] = waitHandles[i]->m_hWaitHandle;
        }
        DWORD status = WaitForMultipleObjects(count, handles, false, millisecond);
        free(handles);
        if(status < WAIT_ABANDONED_0)
        {
            return status;
        }
        WINAPI_ASSERT(WAIT_TIMEOUT == status, "WaitAll WaitForSingleObject Failed");
        return -1;
        //else if(WAIT_TIMEOUT == status)
        //{
        //    printf("WaitAny WaitForMultipleObjects Timeout\n");
        //    return millisecond;
        //}
        //else if(WAIT_FAILED == status)
        //{
        //    printf("WaitAny WaitForMultipleObjects Failed, Error Code : %d\n", GetLastError());
        //    return -1;
        //}
        //else if (WAIT_ABANDONED_0 >= status && status <= (status - WAIT_ABANDONED_0))/* ��Ȼû��ͨ���ȴ�, �������߳��Ѿ����ͷ� */
        //{
        //    printf("WaitHandle has abandoned\n");
        //    return status - WAIT_ABANDONED_0;/* abandoned handle index */
        //}
    }
    /**
     * @brief ����źź͵ȴ����ɹ���ɣ���Ϊ true������ȴ�û����ɣ���˷���һֱ����������
     * 
     * @return true 
     */
    static inline bool SignalAndWait(WaitHandle toSignal, WaitHandle toWaitOn)
    {
        return SignalAndWait(toSignal, toWaitOn, -1, false);
    }
    /**
     * @brief ��һ�� WaitHandle �����źŲ��ȴ���һ����ָ����ʱ���Ϊ 32 λ�з�����������ָ���ڽ���ȴ�ǰ�Ƿ��˳������ĵ�ͬ����
     * 
     * @param toSignal 
     * @param toWaitOn 
     * @param millisecond 
     * @param exitContext 
     * @return true 
     * @return false 1. ��ʱ
     */
    static bool SignalAndWait(WaitHandle toSignal, WaitHandle toWaitOn, DWORD millisecond, bool exitContext)
    {
        DWORD nRet = SignalObjectAndWait(toSignal.m_hWaitHandle, toWaitOn.m_hWaitHandle, millisecond, exitContext);
        if(WAIT_OBJECT_0 == nRet)
        {
            return true;
        }
        printf("SignalObjectAndWait Failed , Return Code : %d, Error Code : %d\n", nRet, GetLastError());
        return false;
    }
};
#endif