/*
 * @Description: ������
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
 * @brief ������
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
     * @brief ������������
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
     * @brief ��ָ�����������Ļ�����
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
 * @brief Ϊ����̹߳���ı����ṩԭ�Ӳ��� (֧�����޵�����)
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
     * @brief ����������Ͳ��ú��滻��һ������������������Ϊһ��ԭ�Ӳ������
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long
     */
    static inline T Add(T& location, const T& value)
    {
        return ::InterlockedAdd(&location, value);    
    }
    /**
     * @brief �����������а�λ���롱���㲢�ý���滻��һ������������������Ϊһ��ԭ�Ӳ������
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T And(T& location, const T& value)
    {
        return ::InterlockedAnd(&location, value);    
    }
    /**
     * @brief �Ƚ�����ֵ�Ƿ���ȣ������ȣ����滻��һ��ֵ
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T CompareExchange(T& location, const T& value)
    {
        return ::InterlockedCompareExchange(&location, value);    
    }
    /**
     * @brief ��ԭ�Ӳ�������ʽ�ݼ�ָ��������ֵ���洢���
     * 
     * @param location 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Decrement(T& location)
    {
        return ::InterlockedDecrement(&location);    
    }
    /**
     * @brief ��ԭ�Ӳ�������ʽ����������Ϊָ����ֵ
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Exchange(T& location, const T& value)
    {
        return ::InterlockedExchange(&location, value);    
    }
    /**
     * @brief ��ԭ�Ӳ�������ʽ����ָ��������ֵ���洢���
     * 
     * @param location 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Increment(T& location)
    {
        return ::InterlockedIncrement(&location);    
    }
    /**
     * @brief �����������а�λ�������㲢�ý���滻��һ������������������Ϊһ��ԭ�Ӳ������
     * 
     * @param location 
     * @param value 
     * @return T ֧�������ڴ����� long �� long long
     */
    static inline T Or(T& location, const T& value)
    {
        return ::InterlockedOr(&location, value);    
    }
};

#endif