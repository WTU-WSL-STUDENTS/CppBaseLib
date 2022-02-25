/*
 * @Description: https://docs.microsoft.com/en-us/windows/win32/api/ioapiset/nf-ioapiset-cancelioex
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-21 16:37:08
 * @LastEditors: like
 * @LastEditTime: 2022-02-23 21:34:43
 */
#include <System.Threading.Tasks.Task.hpp>

#define PRINTF_TEST_BOOL(str) printf(#str" : %s\n", str ? "true" : "false")
#define PRINTF_TEST_CHARS(str) printf(#str" : %s\n", str)

using namespace System::Threading::Tasks;

int main()
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
    printf("Sucess exit\n");
    return 0;
}