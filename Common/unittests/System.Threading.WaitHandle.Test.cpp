/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-18 08:58:39
 * @LastEditors: like
 * @LastEditTime: 2022-01-18 12:16:21
 */
#include <System.Threading.EventWaitHandle.hpp>
#include <System.Threading.Mutex.hpp>
#include <System.Threading.Semaphore.hpp>
#include <stdio.h>
using namespace System::Threading;

#define THREAD_COUNT 4
HANDLE hThreads[THREAD_COUNT];

EventWaitHandle ewh[] = {AutoResetEvent(false), ManualResetEvent(true)};
Mutex mtx;
Semaphore sem(2, 2);/* 最高并发 2, 当前空闲 2 */

DWORD WINAPI WaitOneTestThreadProc(LPVOID);
DWORD WINAPI MutexTestThreadProc(LPVOID);
DWORD WINAPI SemaphoreTestThreadProc(LPVOID);

class WaitOneTest
{
public:
    WaitOneTest()
    {
        DWORD dwThreadID; 
        for(int i = 0; i < THREAD_COUNT; i++) 
        {
            hThreads[i] = CreateThread(
                NULL,         
                0,                
                WaitOneTestThreadProc,       
                ewh,             
                0,                
                &dwThreadID); 

            if (hThreads[i] == NULL) 
            {
                printf("CreateThread failed (%d)\n", GetLastError());
                return;
            }
        }
        char c1;
        while('q' != (c1 = getchar()))
        {
            if('1' == c1)       ewh[0].Set();  
            else if('2' == c1)  ewh[1].Set();
            else if('3' == c1)  ewh[1].Reset();
        }
    }
};
class MutexTest
{
public:  
    MutexTest()
    {
        DWORD dwThreadID; 
        for(int i = 0; i < THREAD_COUNT; i++) 
        {
            hThreads[i] = CreateThread(
                NULL,         
                0,                
                MutexTestThreadProc,       
                &mtx,             
                0,                
                &dwThreadID); 

            if (hThreads[i] == NULL) 
            {
                printf("CreateThread failed (%d)\n", GetLastError());
                return;
            }
        }
        WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, INFINITE);
    }
};
class SemaphoreTest
{
public:  
    SemaphoreTest()
    {
        DWORD dwThreadID; 
        for(int i = 0; i < THREAD_COUNT; i++) 
        {
            hThreads[i] = CreateThread(
                NULL,         
                0,                
                SemaphoreTestThreadProc,       
                &sem,             
                0,                
                &dwThreadID); 

            if (hThreads[i] == NULL) 
            {
                printf("CreateThread failed (%d)\n", GetLastError());
                return;
            }
        }
        WaitForMultipleObjects(THREAD_COUNT, hThreads, TRUE, INFINITE);
    }
};

int main()
{
    // WaitOneTest waitTest;
    // MutexTest mtxTest;
    SemaphoreTest semTest;
    for(int i = 0; i < THREAD_COUNT; i++) 
    {
        CloseHandle(hThreads[i]);
    }
    ewh[0].Dispose();
    ewh[1].Dispose();
    mtx.Dispose();
    return 0;
}

/**
 * # 多线程竞争屏幕打印资源模拟
 * true : 绿灯 / Set
 * false: 红灯 / Reset
 * 
 * ## true , false 
 * - 线程触发信号 : 
 *  绿->线程 A 通过->红-->
 *  绿(自动初始化)->线程 B 通过->红-->
 *  绿(自动初始化)->线程 C 通过->红-->
 *  绿(自动初始化)->线程 D 通过->红-->
 *  绿(自动初始化)
 * 
 *  按下 '2'
 *  红->线程 A 通过->红
 *  按下 '2'
 *  红->线程 B 通过->红
 *  按下 '2'
 *  红->线程 C 通过->红
 *  按下 '2'
 *  红->线程 D 通过->红
 * 
 * ## false , true 
 * - 线程触发信号 : 
 *  
 *  红->
 *  按下 '1'
 *  绿->线程 A 通过->红-->
 *  绿(自动初始化)->线程 B 通过->红-->
 *  绿(自动初始化)->线程 C 通过->红-->
 *  绿(自动初始化)->线程 D 通过->红-->
 *  绿(自动初始化)
 *  
 *  绿->线程 A 通过->红
 *  按下 '2'
 *  红->线程 B 通过->红
 *  按下 '2'
 *  红->线程 C 通过->红
 *  按下 '2'
 *  红->线程 D 通过->红
 */
DWORD WINAPI WaitOneTestThreadProc(LPVOID lpParam) 
{
    EventWaitHandle* ewh = (EventWaitHandle*)lpParam;

    printf("Thread %d waiting for event...\n", GetCurrentThreadId());
    ewh[0].WaitOne();
    printf("Thread %d ################\n", GetCurrentThreadId());
    ewh[1].WaitOne();
    printf("Thread %d exiting\n", GetCurrentThreadId());
    return 1;
}

/**
 * 互斥锁
 */
DWORD WINAPI MutexTestThreadProc(LPVOID lpParam)
{
    Mutex mtx = *(Mutex*)lpParam;
    mtx.WaitOne();
    printf("Thread %d Writting to Database...\n", GetCurrentThreadId());
    ::Sleep(300);
    printf("Thread %d exiting\n", GetCurrentThreadId());
    mtx.ReleaseMutex();
    return 1;
}
/**
 * 并发
 * 2 : 当前空闲 2
 * 2 : 最高并发 2
 * 现象:
 * 一次有 2 个线程运行自己的业务逻辑
 *
 * 1 : 当前空闲 1
 * 2 : 最高并发 2
 * 现象:
 * 一次有 1 个线程运行自己的业务逻辑, 有 1 个入口被一直占用除非手动 Release
 * 
 * 0 : 当前空闲 0
 * 2 : 最高并发 2
 * 现象:
 * 一次有 0 个线程运行自己的业务逻辑, 有 2 个入口被一直占用除非手动 Release(2)
 */
DWORD WINAPI SemaphoreTestThreadProc(LPVOID lpParam)
{
    Semaphore sem = *(Semaphore*)lpParam;
    printf("Thread %d begin and waits for the semaphore....\n", GetCurrentThreadId());
    sem.WaitOne();
    ::Sleep(1000);
    printf("Thread %d exit and release Semaphore , Current Semaphore Count : %d\n", GetCurrentThreadId(), sem.Release() + 1);
    return 1;
}
