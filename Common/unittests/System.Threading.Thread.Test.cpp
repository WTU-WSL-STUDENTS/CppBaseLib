/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-08 15:10:49
 * @LastEditors: like
 * @LastEditTime: 2022-03-11 15:24:40
 */
#include <System.Threading.Thread.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace System::Threading;

static void ThreadProc() 
{
    for (int i = 0; i < 10; i++) 
    {
        printf("ThreadProc: %d\n", i);
        Thread::Sleep(0);
    }
}
void Test1()
{ 
    printf("Main thread: Start a second thread.\n");
    // The constructor for the Thread class requires a ThreadStart
    // delegate that represents the method to be executed on the
    // thread.  C# simplifies the creation of this delegate.
    Thread t(ThreadStart(ThreadProc), NULL);
    printf("Main thread priorty %d, second thread priorty %d\n", GetThreadPriority(GetCurrentThread()), GetThreadPriority(t.Handle()));
    // Start ThreadProc.  Note that on a uniprocessor, the new
    // thread does not get any processor time until the main thread
    // is preempted or yields.  Uncomment the Thread.Sleep that
    // follows t.Start() to see the difference.
    t.Start();
    Thread::Sleep(0);

    for (int i = 0; i < 4; i++) 
    {
        printf("Main thread: Do some work.\n");
        Thread::Sleep(0);
    }
    printf("Main thread: Call Join(), to wait until ThreadProc ends.\n");
    t.WaitOne();
    printf("Main thread: ThreadProc.Join has returned.  Press Enter to end program.\n");
    getchar();
}

void Test2()
{
    Thread newThread([](void* args)->DWORD
    {
        int n = 0;
        while(true)
        {
            n++;
            printf("New thread running.\n");
            Thread::Sleep(200);
        }
        return n;
    });
    printf("IsAlive %s, %d\n", newThread.IsAlive() ? "true" : "false", newThread.GetExitCode());
    newThread.Start();
    printf("IsAlive %s, %d\n", newThread.IsAlive() ? "true" : "false", newThread.GetExitCode());
    newThread.WaitOne(1000);
    
    // Abort newThread.
    printf("Main aborting new thread.\n");
    newThread.Abort(0xff);

    // Wait for the thread to terminate.
    printf("IsAlive %s, %d\n", newThread.IsAlive() ? "true" : "false", newThread.GetExitCode());
    newThread.WaitOne();
    printf("IsAlive %s, %d\n", newThread.IsAlive() ? "true" : "false", newThread.GetExitCode());
    printf("New thread terminated - Main exiting.");
}

int main()
{
    Test1();
    Test2();
    return 0;
}