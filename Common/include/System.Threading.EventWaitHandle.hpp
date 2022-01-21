/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-17 15:31:33
 * @LastEditors: like
 * @LastEditTime: 2022-01-19 21:50:25
 */
//https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.waithandle?view=net-5.0
#ifndef SYSTEM_THREADING_EVENTWAITHANDLE
#define SYSTEM_THREADING_EVENTWAITHANDLE

#include <System.Threading.WaitHandle.hpp>

namespace System::Threading
{
    class EventWaitHandle;  /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.eventwaithandle?view=net-5.0  */
    class AutoResetEvent;   /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.autoresetevent?view=net-5.0 */
    class ManualResetEvent; /* https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.manualresetevent?view=net-5.0 */
    /**
     * @brief 指示在接收信号后是自动重置 EventWaitHandle 还是手动重置
     * 
     */
    enum EventResetMode
    {
        AutoReset = 0,
        ManualReset
    };
};

/**
 * @brief 表示一个线程同步事件
 * # Refrence
 * - OpenEvent      - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-openeventa
 * - CreateEvent    - https://docs.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-createeventa 
 */
class System::Threading::EventWaitHandle : public WaitHandle
{
protected:
    const char* m_strName;  /* 如果要与其他进程共享同步对象，则为名称；否则为 null 或空字符串。 该名称区分大小写 */
    EventWaitHandle(const char* strName) : m_strName(strName){}
public:
    EventWaitHandle(bool initialState/* is signaled */, EventResetMode mode, const char* strName = NULL) : m_strName(strName), 
        WaitHandle(CreateEvent(NULL, EventResetMode::AutoReset != mode, initialState, strName))
    {
        if(m_bDisposed)
        {
            printf("Create EventWaitHandle Failed , Error Code : %d", GetLastError());
        }
    }
    /**
     * @brief 打开指定名称为同步事件（如果已经存在)
     * 
     * @param strName 要打开并与其他进程共享的同步对象的名称。 该名称区分大小写
     * @return EventWaitHandle 
     */
    inline static EventWaitHandle OpenExisting(const char* strName)
    {
        EventWaitHandle event(strName);
        if(event.m_bDisposed = NULL == (event.m_hWaitHandle = OpenEvent(EVENT_ALL_ACCESS, false, strName)))
        {
            printf("EventWaitHandle::OpenExisting Failed , Error Code : %d\n", GetLastError());
        }     
        return event;
    }
    /**
     * @brief 将事件状态设置为终止状态，允许阻塞等待的线程继续
     * 
     * @return true 
     * @return false 
     */
    inline bool Set()
    {
        if(SetEvent(m_hWaitHandle))
        {
            return true;
        }
        printf("SetEvent Failed , Error Code : %d\n", GetLastError());
        return false;
    }
    /**
     * @brief 将事件状态设置为非终止状态，导致线程阻止
     * 
     * @return true 
     * @return false 
     */
    inline bool Reset()
    {
        if(ResetEvent(m_hWaitHandle))
        {
            return true;
        }
        printf("ResetEvent Failed , Error Code : %d\n", GetLastError());
        return false;
    }
};

/**
 * @brief 表示线程同步事件在一个等待线程释放后收到信号时自动重置
 * 
 */
class System::Threading::AutoResetEvent : public System::Threading::EventWaitHandle
{
public:
    AutoResetEvent(bool initialState) : EventWaitHandle(initialState, EventResetMode::AutoReset){}
};

/**
 * @brief 表示线程同步事件，收到信号时，必须手动重置该事件
 * 
 */
class System::Threading::ManualResetEvent : public System::Threading::EventWaitHandle
{
public:
    ManualResetEvent(bool initialState) : EventWaitHandle(initialState, EventResetMode::ManualReset){}
};

#endif