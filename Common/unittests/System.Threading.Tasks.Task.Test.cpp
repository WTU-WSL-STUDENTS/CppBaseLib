/*
 * @Description: https://docs.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-cancelioex
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-21 16:37:08
 * @LastEditors: like
 * @LastEditTime: 2022-03-31 10:30:08
 */
#include <System.Threading.Tasks.Task.hpp>

#define PRINTF_TEST_BOOL(str) printf(#str" : %s\n", str ? "true" : "false")
#define PRINTF_TEST_CHARS(str) printf(#str" : %s\n", str)

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
        PRINTF_TEST_BOOL(t.Wait(1));
        t.Wait(); 
        PRINTF_TEST_BOOL(t.Wait(1));
        PRINTF_TEST_BOOL(t.IsCanceled());
        PRINTF_TEST_BOOL(t.IsFaulted());
        PRINTF_TEST_BOOL(t.IsCompletedSuccessfully());
        PRINTF_TEST_BOOL(t.IsCompleted());
        
    }
}

void ReuseTaskObjectTest()
{
    Task t([](AsyncState args)->void
    {
        int begin   = (int)args[0];
        int end     = (int)args[1];
        int sum     = 0;
        for(int i = begin; i <= end; i++)
        {
            sum += i;
        }
        (*static_cast<int*>(args[2])) += sum;
        MemoryBarrier();
        printf("from %d to %d sum = %d, accumulation %d\n", begin, end, sum, *static_cast<int*>(args[2]));
    });
    int accumulate = 0;
    t.Start(CreateAsyncState((System::Object)1, (System::Object)10, &accumulate));
    t.Wait();
    t.Start(CreateAsyncState((System::Object)11, (System::Object)20, &accumulate));
    t.Wait();
}
int main()
{
    ReuseTaskObjectTest();
    printf("Sucess exit\n");
    return 0;
}