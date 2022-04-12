/*
 * @Description: https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.monitor?view=net-5.0
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-17 10:04:57
 * @LastEditors: like
 * @LastEditTime: 2022-02-25 17:01:58
 */
#include <System.Threading.SpinLock.hpp>
#include <System.Threading.ThreadPool.hpp>
#include <System.Threading.Barrier.hpp>
#include <System.Threading.Interlocked.hpp>
#include <System.Threading.EventWaitHandle.hpp>
#include <exception>
using namespace System;
using namespace System::Threading;
#define WORKCOUNT 3

SpinLock sl;
char buffer[12580];
size_t len;
Barrier barrier(WORKCOUNT);
AutoResetEvent are(false);
void SignalAndWaitWithHook()
{
    if(barrier.SignalAndWait())
        are.Set();
    printf("Task finished\n");
}
int main()
{
    memset(buffer, 0, sizeof(buffer));
    for (int taskCtr = 0; taskCtr < WORKCOUNT; taskCtr++)
    {
        ThreadPoolSingleton::Ref().TrySubmitThreadpoolCallback
        (
            [](PTP_CALLBACK_INSTANCE instance, Object args)
            {
                for (int i = 0; i < 10; i++)
                {
                    sl.Enter();
                    sprintf(buffer + len, "%d", i % 10);
                    len++;
                    sl.Exit();
                    // YieldProcessor();
                }
                SignalAndWaitWithHook();
            },
            NULL
        );
    }
    if(are.WaitOne())
    {
        int count5 = 0;
        for(size_t i = 0; i < len; i++)
        {
            if('5' == buffer[i])
                count5++;
        }
        printf(
            "Length = %d (should be 30000)\n"
            "number of occurrences of '5' in sb: %d (should be 3000)\n", len, count5);
    }
    return 0;
}