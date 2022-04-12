/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-17 16:21:05
 * @LastEditors: like
 * @LastEditTime: 2022-02-18 08:41:07
 */
#ifndef SYSTEM_THREADING_TASK_PARALLEL_HPP
#define SYSTEM_THREADING_TASK_PARALLEL_HPP
#include <omp.h>
#include <System.Threading.Barrier.hpp>

#define POST_PARALLEL_TASK(arg)                     \
ThreadPoolSingleton::Ref().TrySubmitThreadpoolCallback     \
(                                                   \
    [](PTP_CALLBACK_INSTANCE instance, Object args) \
    {                                               \
        (*static_cast<_First*>(args))();            \
        if(barrier.SignalAndWait())                 \
        {                                           \
            are.Set();                              \
        }                                           \
    }, &arg                                         \
)

namespace System::Threading::Tasks
{
    class Parallel;
};
class System::Threading::Tasks::Parallel
{
private:
    Barrier barrier;
    AutoResetEvent are;
    Parallel(long actionCount) : barrier(actionCount), are(AutoResetEvent(false)){}
    template<typename _First>
    static inline void _Invoke(_First t)
    {
        POST_PARALLEL_TASK(t);
    }
    template<typename _First, typename ..._Rest>
    static void _Invoke(_First t, _Rest... u)
    {
        POST_PARALLEL_TASK(t);
        _Invoke(u);
    }
public:
    ~Parallel(){}
    /**
     * @brief 
     * 提示 : 对操作共享变量的代码段要做同步标识
     * @tparam T 
     * @tparam TFunc 
     * @param begin 
     * @param end 
     * @param func 
     */
    template<typename T, typename TFunc>
    static void For(T& begin, T& end, TFunc func)
    {
#pragma omp parallel  
        {  
#pragma omp for 
            for(T i = begin; i < end; i++)
            {
                func(i);
            }
        }
    }
    template<typename _First, typename ..._Rest>
    static void Invoke(_First t, _Rest... u)
    {
        POST_PARALLEL_TASK(t);
        _Invoke(u);
        are.WaitOne();
    }
};

#endif