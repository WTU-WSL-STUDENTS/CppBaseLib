/*
 * @Description: 
        https://docs.microsoft.com/zh-cn/dotnet/api/system.threading.monitor?view=net-5.0
    - WaitOnAddress link error:
        https://stackoverflow.com/questions/41430077/waitonaddress-unresolved
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-17 10:04:57
 * @LastEditors: like
 * @LastEditTime: 2022-02-25 14:45:34
 */
#include <System.Threading.ThreadPool.hpp>
#include <System.Threading.Watcher.hpp>
#include <System.Threading.Barrier.hpp>
#include <System.Threading.Interlocked.hpp>
#include <System.Threading.EventWaitHandle.hpp>
#include <exception>
using namespace System;
using namespace System::Threading;
#define WORKCOUNT 10

class Random
{
public:
    Random(){}
    ~Random(){}
    inline int Next(int begin, int end, int seed)
    {
        srand(seed);
        return rand() * end / RAND_MAX + begin;
    }
};
struct Context
{
    Random* rnd;
    long* total;
    long* n;
    long taskId;
    AutoResetEvent* are;
};

Barrier barrier(WORKCOUNT);
AutoResetEvent are(false);
void SignalAndWaitWithHook()
{
    printf("Wait other task\n");
    if(barrier.SignalAndWait())
        are.Set();
}
int main()
{
    long total = -1;
    long n = 0;
    Random rnd;
    Context* cache[WORKCOUNT];
    for (int taskCtr = 0; taskCtr < WORKCOUNT; taskCtr++)
    {
        ThreadPoolSingleton::Ref().TrySubmitThreadpoolCallback
        (
            [](PTP_CALLBACK_INSTANCE instance, Object args)
            {
                Context context = *(Context*)(args);
                int values[10000] = {0};
                long taskTotal = 0;
                long taskN = 0;
                long ctr = 0;
                Watcher<long>::Wait(context.total);
                // Generate 10,000 random integers
                for (ctr = 0; ctr < 10000; ctr++)
                    taskTotal += values[ctr] = context.rnd->Next(0, 1001, *context.total);
                taskN = ctr;
                printf("Mean for task %d: %f (N=%d)\n", context.taskId, (taskTotal * 1.0)/taskN, taskN);
                Interlocked<long>::Add(*context.n, taskN);
                Interlocked<long>::Add(*context.total, taskTotal);
                Watcher<long>::WakeOne(context.total);
                SignalAndWaitWithHook();
            },
            cache[taskCtr] = new Context{&rnd, &total, &n, taskCtr, &are}
        );
    }
    Sleep(1000);/* 等待线程池安排所有任务 */
    total = 0;
    printf("Wake %p\n", &total);
    Watcher<long>::WakeOne(&total);
    VALRET_ASSERT(are.WaitOne(), 1);
    printf("Mean for all tasks: %f (N=%d)\n", (total * 1.0)/n, n);
    for(int i = 0; i < WORKCOUNT; i++)
    {
        delete cache[i];
        cache[i] = NULL;
    }
    return 0;
}
