#include <System.Threading.Thread.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace System::Threading;
static unsigned int PrintSomething(void* args)
{
    char* str = (char*)args;
    Thread::Sleep(3000);
    printf("PrintFrom PrintSomething:%s\n", str);
    return 0;
}
class HowToDisposeLoop
{
public:
    Thread* t;
    HowToDisposeLoop()
    {
        t = new Thread(Loop, this);
        printf("Create Thread:%s", t->tInfo.CurrentStatus == Created ? "sucess" : "failed");
        printf("Start():%s\n", t->Start() ? "sucess" : "failed");
        printf("Do Other Things In main(), Do Not Delay By Sleep !\n");
    }
    ~HowToDisposeLoop()
    {
        printf("Disopse:%s\n", t->Disopse() ? "sucess" : "failed");
    }
    static unsigned int Loop(void* args)
    {
        HowToDisposeLoop* p = (HowToDisposeLoop*)args;
        while(Running == p->t->tInfo.CurrentStatus)
        {
            Thread::Sleep(100);
            printf("Loop...\n");
        }
        return 0;
    }
};
int main()
{
    Thread t(PrintSomething, "input args from main");
    printf("Create Thread:%s", t.tInfo.CurrentStatus == Created ? "sucess" : "failed");
    printf("Start():%s\n", t.Start() ? "sucess" : "failed");
    printf("Do Other Things In main(), Do Not Delay By Sleep !\n");
    printf("WaitThreadFinshed:%s\n", t.WaitThreadFinshed() ? "sucess" : "failed");
    printf("Disopse:%s\n", t.Disopse() ? "sucess" : "failed");

    /* test2 */
    HowToDisposeLoop test2;
    Thread::Sleep(1000);
    
    // test2.t->tInfo.CurrentStatus = Canceled;
    // Thread::Sleep(150);/* 如果将状态设置为Canceled的话，需要延时一下，延时时间建议为循环执行一次的时间，等线程函数执行结束，将结果更新到实际的info中 */
    test2.t->tInfo.CurrentStatus = Finished;/* 如果将线程状态设置为Finished,则不需要延时 */
    printf("WaitThreadFinshed:%s\n", test2.t->WaitThreadFinshed() ? "sucess" : "failed");
    return 0;
}