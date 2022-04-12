/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-19 19:54:16
 * @LastEditors: like
 * @LastEditTime: 2022-03-11 17:16:17
 */
#include <System.Threading.Timer.hpp>
#include <System.DateTime.hpp>
using namespace System;
using namespace System::Threading;

int tickCount = 0;
void TimerCallbackFunc(Object args)
{
    char buff[_MAX_PATH];
    EventWaitHandle* ewh = (EventWaitHandle*)args;
    DateTime::Now().ToString(buff);
    printf("%s\n", buff);
    if(10 < ++tickCount)
    {
        tickCount = 0;
        ewh->Set();
    }
}

int main()
{
    AutoResetEvent are(false);

    char buff[_MAX_PATH];
    DateTime::Now().ToString(buff);
    printf("%s Creating timer.\n", buff);
    Timer t((TimerCallback)TimerCallbackFunc, &are, 1000, 200);

    are.WaitOne();
    printf("Changing period to .5 seconds.\n");
    t.Change(0, 500);
    are.WaitOne();
    printf("Change frequence\n");
    t.Change(0, 0);
    Sleep(1000);
    t.Dispose();
    are.Dispose();
    printf("Destroying timer.\n");
    return 0;
}