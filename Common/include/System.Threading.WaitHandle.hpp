/*
 * @Description: 目前仅支持 windows 平台
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
 * @brief 封装等待对共享资源进行独占访问的操作系统特定的对象
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
     * @brief 提供派生类一个完整的 WaitHandle 释放过程
     * 
     * @param disposing 由派生类决定是否调用该接口的释放实现
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
     * @brief 获取或设置本机操作系统句柄
     * 
     * @return HANDLE& 
     */
    inline HANDLE& Handle()
    {
        return m_hWaitHandle;
    }
    /**
     * @brief 释放 WaitHandle 类的当前实例所使用的所有资源
     * 
     */
    virtual void Close()
    {
        Dispose(true);
    }
    /**
     * @brief 释放 WaitHandle 类的当前实例所使用的所有资源
     * 
     */
    void Dispose() override
    {
        Dispose(true);
    } 
    /**
     * @brief 如果当前实例收到信号，则为 true；否则为 false。
     * 
     * @param millisecond 等待的毫秒数, -1 表示无限期等待
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
     * @brief 等待指定数组中的所有元素都收到信号
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
     * @brief 等待指定数组中的所有元素接收信号或到达指定时间间隔为止
     * 
     * @param waitHandles 
     * @param count 
     * @param millisecond 
     * @return true 
     * @return false 1. 超时
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
        //else if (WAIT_ABANDONED_0 >= status && status <= (status - WAIT_ABANDONED_0))/* 虽然没有通过等待, 但所在线程已经被释放 */
        //{
        //    waitHandles[status - WAIT_ABANDONED_0]->Dispose(); 
        //}
        WINAPI_ASSERT(WAIT_TIMEOUT == status, "WaitAll WaitForSingleObject Failed");
        return false;
    }
    /**
     * @brief 等待指定数组中的任一元素收到信号
     * 
     * @param waitHandles 
     * @param count 
     * @return int 数组索引
     */
    static int WaitAny(WaitHandle** waitHandles, DWORD count)
    {
        return WaitAny(waitHandles, count, INFINITE);
    }
    /**
     * @brief 等待指定数组中的任意元素接收信号或到达指定时间间隔, 最多一次等 MAXIMUM_WAIT_OBJECTS 个
     * 
     * @param waitHandles 
     * @param count 
     * @param millisecond 
     * @return int 1. 成功等待返回数组索引; 2. 超时返回 millisecond; 3. 等待失败返回 -1
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
        //else if (WAIT_ABANDONED_0 >= status && status <= (status - WAIT_ABANDONED_0))/* 虽然没有通过等待, 但所在线程已经被释放 */
        //{
        //    printf("WaitHandle has abandoned\n");
        //    return status - WAIT_ABANDONED_0;/* abandoned handle index */
        //}
    }
    /**
     * @brief 如果信号和等待都成功完成，则为 true；如果等待没有完成，则此方法一直阻塞不返回
     * 
     * @return true 
     */
    static inline bool SignalAndWait(WaitHandle toSignal, WaitHandle toWaitOn)
    {
        return SignalAndWait(toSignal, toWaitOn, -1, false);
    }
    /**
     * @brief 向一个 WaitHandle 发出信号并等待另一个，指定超时间隔为 32 位有符号整数，并指定在进入等待前是否退出上下文的同步域
     * 
     * @param toSignal 
     * @param toWaitOn 
     * @param millisecond 
     * @param exitContext 
     * @return true 
     * @return false 1. 超时
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