/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-15 15:22:38
 * @LastEditors: like
 * @LastEditTime: 2022-02-25 15:08:30
 */ 
#include <System.Threading.ReaderWriterLockSlim.hpp>
#include <iostream> 
#include <process.h> 
using namespace std;
using namespace System::Threading;
unsigned int __stdcall ReadThread(PVOID ParameterData);
unsigned int __stdcall WriteThread(PVOID ParameterData);
 
#define COUNT 30
HANDLE ThreadRead[COUNT],  ThreadWrite[COUNT];
ReaderWriterLockSlim rwl;
ConditionVariable cv;

int main()
{
    int i = 0;
    for (; i<COUNT; i++) {
        ThreadRead[i] = (HANDLE)_beginthreadex(NULL, 0, ReadThread, NULL, 0, NULL);
        ThreadWrite[i] = (HANDLE)_beginthreadex(NULL, 0, WriteThread, NULL, 0, NULL);
    }
 
    WaitForMultipleObjects(COUNT, ThreadRead, TRUE, INFINITE);
    WaitForMultipleObjects(COUNT, ThreadWrite, TRUE, INFINITE);
 
    printf("Sucess exit\n");
    return 0;
}
 
unsigned int __stdcall ReadThread(PVOID ParameterData) {
    rwl.EnterReadLock();
    printf("ID%8d  Read Thread Begin\n", GetCurrentThreadId());
    Sleep(200);/* 在读锁被占用时, 挂起当前线程, 其他线程仍然可以进入读锁 */
    printf("ID%8d  Read Thread Terminate\n", GetCurrentThreadId());
    rwl.ExitReadLock();
    return 0;
}
 
unsigned int __stdcall WriteThread(PVOID ParameterData) {
    rwl.EnterWriteLock();
    printf("ID%8d  Write Thread Begin\n", GetCurrentThreadId());
    Sleep(200);/* 在写锁被占用时, 挂起当前线程, 其他线程无法进入写锁 */
    printf("ID%8d  Write Thread Terminate\n", GetCurrentThreadId());
    rwl.ExitWriteLock();
    return 0;
}