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
     * @brief ָʾ�ڽ����źź����Զ����� EventWaitHandle �����ֶ�����
     * 
     */
    enum EventResetMode
    {
        AutoReset = 0,
        ManualReset
    };
};

/**
 * @brief ��ʾһ���߳�ͬ���¼�
 * # Refrence
 * - OpenEvent      - https://docs.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-openeventa
 * - CreateEvent    - https://docs.microsoft.com/zh-cn/windows/win32/api/synchapi/nf-synchapi-createeventa 
 */
class System::Threading::EventWaitHandle : public WaitHandle
{
protected:
    const char* m_strName;  /* ���Ҫ���������̹���ͬ��������Ϊ���ƣ�����Ϊ null ����ַ����� ���������ִ�Сд */
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
     * @brief ��ָ������Ϊͬ���¼�������Ѿ�����)
     * 
     * @param strName Ҫ�򿪲����������̹����ͬ����������ơ� ���������ִ�Сд
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
     * @brief ���¼�״̬����Ϊ��ֹ״̬�����������ȴ����̼߳���
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
     * @brief ���¼�״̬����Ϊ����ֹ״̬�������߳���ֹ
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
 * @brief ��ʾ�߳�ͬ���¼���һ���ȴ��߳��ͷź��յ��ź�ʱ�Զ�����
 * 
 */
class System::Threading::AutoResetEvent : public System::Threading::EventWaitHandle
{
public:
    AutoResetEvent(bool initialState) : EventWaitHandle(initialState, EventResetMode::AutoReset){}
};

/**
 * @brief ��ʾ�߳�ͬ���¼����յ��ź�ʱ�������ֶ����ø��¼�
 * 
 */
class System::Threading::ManualResetEvent : public System::Threading::EventWaitHandle
{
public:
    ManualResetEvent(bool initialState) : EventWaitHandle(initialState, EventResetMode::ManualReset){}
};

#endif