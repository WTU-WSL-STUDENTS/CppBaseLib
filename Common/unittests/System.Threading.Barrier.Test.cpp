/*
 * @Description: BarrierDemo : https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.barrier?view=net-5.0
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-17 09:23:56
 * @LastEditors: like
 * @LastEditTime: 2022-04-12 13:53:29
 */
#include <System.Threading.ThreadPool.hpp>
#include <System.Threading.Barrier.hpp>
#include <System.Threading.EventWaitHandle.hpp>
#include <exception>
using namespace System;
using namespace System::Threading;

#define WORKCOUNT 4

Barrier barrier(WORKCOUNT);
AutoResetEvent are(false);
long count = 0;

void SignalAndWaitWithHook()
{
    VOIDRET_ASSERT(barrier.SignalAndWait());
    printf("Post-Phase action: count=%d, phase=%d\n", count, barrier.GetCurrentPhaseNumber());
    if(2 == barrier.GetCurrentPhaseNumber()) 
    {
        throw std::exception("Error Phase Number\n");
    }
    if(3 == barrier.GetCurrentPhaseNumber())
    {
        are.Set();
    }
}

int main()
{
    for(int i = 0 ; i < WORKCOUNT; i++)
    {
        ThreadPoolSingleton::Ref().TrySubmitThreadpoolCallback
        (
            [](PTP_CALLBACK_INSTANCE instance, Object args)
            {
                long* count = static_cast<long*>(args);
                Interlocked<long>::Increment(*count);
                SignalAndWaitWithHook(); // during the post-phase action, count should be 4 and phase should be 0
                Interlocked<long>::Increment(*count);
                SignalAndWaitWithHook(); // during the post-phase action, count should be 8 and phase should be 1
                // The third time, SignalAndWait() will throw an exception and all participants will see it
                Interlocked<long>::Increment(*count);
                try
                {
                    SignalAndWaitWithHook();
                }
                catch(std::exception& e)
                {
                    printf("%s", e.what());
                }
                Interlocked<long>::Increment(*count);
                SignalAndWaitWithHook();
            }, &count
        );
    }
    if(are.WaitOne())
    {
        printf("Sucess Exit\n");
    }
    return 0;
}