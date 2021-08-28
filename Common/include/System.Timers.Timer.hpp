#ifndef SYSTEM.TIMERS.TIMER.HPP
#define SYSTEM.TIMERS.TIMER.HPP

#include <System.Threading.Thread.hpp>
using namespace System::Threading;

#ifndef _WIN32
typedef int Millisecond;
#define SLEEP(dwMilliseconds) usleep(dwMilliseconds)
#else
typedef DWORD Millisecond;
#define SLEEP(Milliseconds) Sleep(Milliseconds)
#endif
#define STABLE_TIMER 0xffffffff

typedef struct tagElapsedEventArgs 
{
    int SignalTime;
}ElapsedEventArgs;
typedef void (*ElapsedEventHandler)(void* sender/* Timer* t = (Timer*)sender */, ElapsedEventArgs& e);/* 定时器设定的毫秒时间到达触发的事件 */

namespace System::Timers
{
    class Timer;
}
class System::Timers::Timer/* https://docs.microsoft.com/zh-cn/dotnet/api/system.timers.timer.start?view=net-5.0 */
{
private:
    Thread* thread;
    static unsigned int ProcThread(void* sender)
    {
        Timer* t = (Timer*)sender;
        size_t count = t->AutoReset;
        do
        {
            SLEEP(t->Interval);
            ElapsedEventArgs e;
            e.SignalTime = 1;
            t->Elapsed(sender, e);
            if(STABLE_TIMER != count)
            {
                count--;
            }
        }while(0 < count && Running == t->thread->tInfo.CurrentStatus);
        return count;
    }
public:
    ElapsedEventHandler Elapsed;
    Millisecond Interval;
    size_t AutoReset; /* 触发一次后自动重新开始 */
    Timer():AutoReset(0), thread(NULL){}
    Timer(Millisecond interval):Interval(interval), AutoReset(0), thread(NULL){}
    bool Start()
    {
        if( thread)
        {
            return false;
        }
        thread = new Thread(ProcThread, this);
        return thread->Start();
    }
    bool Stop()
    {
        if(!thread || !thread->IsRunning())
        {
            return false;
        }
        thread->tInfo.CurrentStatus = Finished;
        return thread->WaitThreadFinshed();
    }
};

#endif