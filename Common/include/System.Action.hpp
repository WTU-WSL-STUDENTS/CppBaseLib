/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-02-17 16:25:32
 * @LastEditors: like
 * @LastEditTime: 2022-03-27 14:00:37
 */
#ifndef SYSTEM_ACTION_HPP
#define SYSTEM_ACTION_HPP
#include <CompliedEntry.h>

namespace System
{
    /* Action : https://docs.microsoft.com/zh-cn/dotnet/api/system.action-2?view=net-5.0 */
    template<typename _First, typename ..._Rest>
    using Action = void (*)(_First t, _Rest... u);
    /**
     * @brief 表示当事件提供数据时将处理该事件的方法
     * 
     * @tparam TEventArgs 
     */
    template<typename TEventArgs>
    using EventHandler = void (*)(System::Object sender, TEventArgs e);
   
   
    // using Action = void (*)();
    // template<typename T>
    // using Action = void (*)(T& arg);
    // template<typename T1, typename T2>
    // using Action = void (*)(T& arg1, T& arg2);
    // template<typename T1, typename T2,typename T3>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3);
    // template<typename T1, typename T2,typename T3, typename T4>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9, T10& arg10);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9, T10& arg10, T11& arg11);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9, T10& arg10, T11& arg11, T12& arg12);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9, T10& arg10, T11& arg11, T12& arg12, T13& arg13);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9, T10& arg10, T11& arg11, T12& arg12, T13& arg13, T14& arg14);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9, T10& arg10, T11& arg11, T12& arg12, T13& arg13, T14& arg14, T15& arg15);
    // template<typename T1, typename T2,typename T3, typename T4, typename T5, typename T6, typename T7, typename T8, typename T9, typename T10, typename T11, typename T12, typename T13, typename T14, typename T15, typename T16>
    // using Action = void (*)(T1& arg1, T2& arg2, T3& arg3, T4& arg4, T5& arg5, T6& arg6, T7& arg7, T8& arg8, T9& arg9, T10& arg10, T11& arg11, T12& arg12, T13& arg13, T14& arg14, T15& arg15, T16& arg16);
};

#endif