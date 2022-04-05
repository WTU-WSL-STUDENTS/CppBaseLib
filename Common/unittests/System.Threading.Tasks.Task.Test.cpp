/*
 * @Description: https://docs.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-cancelioex
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-21 16:37:08
 * @LastEditors: like
 * @LastEditTime: 2022-03-31 10:30:08
 */
#include <System.Threading.Tasks.Task.hpp>
#include <System.Threading.CancellationToken.hpp>
#include <System.DateTime.hpp>
#define PRINTF_TEST_BOOL(str) printf(#str" : %s\n", str ? "true" : "false")
#define PRINTF_TEST_CHARS(str) printf(#str" : %s\n", str)

using namespace System;
using namespace System::Threading;
using namespace System::Threading::Tasks;

void TaskLiveStatusTest()
{
    int i;
    Task t([](AsyncState args)->void
    {
        Sleep(100);
        printf("print in task-%d\n", *static_cast<int*>(args[0]));
        if(2 == *static_cast<int*>(args[0]))
        {
            throw std::exception("gg");
        }
    }, CreateAsyncState(&i));
    printf("CreationOptions : %d\n", t.GetCreationOptions());
    for(i = 0; i < 3; i++)
    {
        printf("-----------------------\n");
        t.Start();
        PRINTF_TEST_BOOL(t.IsCanceled());
        PRINTF_TEST_BOOL(t.IsFaulted());
        PRINTF_TEST_BOOL(t.IsCompletedSuccessfully());
        PRINTF_TEST_BOOL(t.IsCompleted());
        PRINTF_TEST_BOOL(t.WaitOne(1));
        t.WaitOne();
        PRINTF_TEST_BOOL(t.WaitOne(1));
        PRINTF_TEST_BOOL(t.IsCanceled());
        PRINTF_TEST_BOOL(t.IsFaulted());
        PRINTF_TEST_BOOL(t.IsCompletedSuccessfully());
        PRINTF_TEST_BOOL(t.IsCompleted());
    }
}

void ParalleTaskTest()
{
    CancellationTokenSource cancelTokenSource;
    int accumulate = -1;
    Task t([](AsyncState args)->void
        {
            printf("begin prepare environment work\n");
            Sleep(1000);
            *(static_cast<int*>(args[0])) = 0;
            printf("end prepare environment work\n");
        },
        CreateAsyncState(&accumulate)
    );
    System::Action<AsyncState> func = [](AsyncState args)->void
    {
        int begin = (int)args[0];
        int end = (int)args[1];
        int sum = 0;
        for (int i = begin; i <= end; i++)
        {
            sum += i;
        }
        (*static_cast<int*>(args[2])) += sum;
        MemoryBarrier();
        printf("%d from %d to %d sum = %d, accumulation %d\n", GetCurrentThreadId(), begin, end, sum, *static_cast<int*>(args[2]));
        WEAK_PTR(CancellationToken) token = static_cast<CancellationToken*>(args[3]);
        while(!token->GetCancellationRequested())
        {
        }
        printf("%d Cancel timeout work\n", GetCurrentThreadId());
    };
    /* 最多*/
#define SUM_TASK_COUNT 10
    Task* sumTasks[SUM_TASK_COUNT];
    for (int i = 0; i < SUM_TASK_COUNT; i++)
    {
        sumTasks[i] = t.ContinueWith(func, CreateAsyncState((System::Object)(1 + i * 10), (System::Object)((i + 1) * 10), &accumulate, cancelTokenSource.GetToken()));
        sumTasks[i]->SetCreationOptions(TaskCreationOptions::AttachedToParent);
    }
    t.Start();
    Sleep(5000);
    printf("Cancel work\n");
    cancelTokenSource.Cancel();
    /* ContinueWith 创建的 work 线程默认与 Parent task 互相独立, 当线程设置为 AttachedToParent 时, Parent task 会等到线程执行完才退出 */
    //if (Task::WaitAll(sumTasks, SUM_TASK_COUNT))
    if(t.WaitOne())
    {
        printf("Accumulate res : %d\n", accumulate);
    }
    for (int i = 0; i < SUM_TASK_COUNT; i++)
    {
        delete sumTasks[i];/* 会等待任务完成再 delete */
    }
}
void DelayTaskTest()
{
    CancellationTokenSource cancelTokenSource;
    Task* t = Task::Delay(1000, cancelTokenSource.GetToken());
    char buff[_MAX_PATH];
    DateTime::Now().ToString(buff, TimeFormatType::STAMP);
    printf("Begin :%s\n", buff);
    Sleep(500);
    cancelTokenSource.Cancel();
    t->WaitOne();
    //await(t);
    DateTime::Now().ToString(buff, TimeFormatType::STAMP);
    printf("End : %s\n", buff);
    delete t;
}
int main()
{
#ifdef _DEBUG
    MEMORYLEAK_ASSERT;
#endif
    TaskLiveStatusTest();
    ParalleTaskTest();
    DelayTaskTest();
    printf("Sucess exit\n");
    return 0;
}