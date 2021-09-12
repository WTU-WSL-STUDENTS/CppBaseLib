/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-14 12:37:09
 * @LastEditors: like
 * @LastEditTime: 2021-09-12 16:05:00
 */
#ifndef SYSTEM_TUPLE_HPP
#define SYSTEM_TUPLE_HPP

/*  禁用字节对齐 ,默认长度为1   #define  DISABLE_PACK_ALIGN
 *  设置字节对齐长度            #define PACK_ALIGN_SIZE
 */
#ifndef PACK_ALIGN_SIZE
#define PACK_ALIGN_SIZE 1
#endif

#if defined(MSVC)
warning(1,"Enable Tuple DISABLE_PACK_ALIGN ")
#else defined(GCC)
// #warning "Enable Tuple DISABLE_PACK_ALIGN "
#endif
#include <stdio.h>

namespace System
{
    template<typename... TList> struct Tuple;
    template<> struct Tuple<> {};
    typedef Tuple<> EmptyTuple;
    
    #ifdef DISABLE_PACK_ALIGN
    #pragma pack(PACK_ALIGN_SIZE)
    #endif 
    template<typename T, typename... TList>
    struct Tuple<T, TList...> : public Tuple<TList...>/* 任意多个类型的容器 */
    {
        typedef T               ValueType;
        typedef Tuple<TList...>    BaseType;
        typedef Tuple<T, TList...> ThisType;    
        T val;
        Tuple(const ValueType& _val, const TList&... rest) : BaseType(rest...), val(_val){}
    };
    
    template<typename T>
    struct Tuple<T> : EmptyTuple/* 任意单个类型的容器，多个类型容器递归的终点 */
    {
        typedef T           ValueType;
        typedef EmptyTuple  BaseType;
        typedef Tuple<T>    ThisType;
        ValueType value;
        Tuple(const ValueType& val) : value(value){}
        ThisType* operator=(const ThisType& other)
        {
            if(this == &other)
            {
                return *this;
            }
            value = other.value;
            return *this;
        }
    };
    #ifdef DISABLE_PACK_ALIGN
    #pragma pack()
    #endif 
};

#endif