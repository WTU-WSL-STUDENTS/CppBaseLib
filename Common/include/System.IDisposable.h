/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-17 14:36:47
 * @LastEditors: like
 * @LastEditTime: 2022-02-12 14:06:37
 */
#ifndef SYSTEM_IDISPOSABLE_H
#define SYSTEM_IDISPOSABLE_H

namespace System
{
    class IDisposable;      /* https://docs.microsoft.com/zh-cn/dotnet/api/system.idisposable?view=net-5.0 */
    class IAsyncDisposable; /* https://docs.microsoft.com/zh-cn/dotnet/api/system.iasyncdisposable.disposeasync?view=net-5.0 */
};

class System::IDisposable
{
public:
    virtual void Dispose() = 0;
};

namespace System::Threading::Tasks
{
    /* TODO : */
    struct ValueTask{};
    class Task{};
}
class System::IAsyncDisposable
{
public:
    virtual System::Threading::Tasks::ValueTask DisposeAsync () {throw "Not Support";}
};

#endif