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

/*  �����ֽڶ��� ,Ĭ�ϳ���Ϊ1   #define  DISABLE_PACK_ALIGN
 *  �����ֽڶ��볤��            #define PACK_ALIGN_SIZE
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
    struct Tuple<T, TList...> : public Tuple<TList...>/* ���������͵����� */
    {
        typedef T               ValueType;
        typedef Tuple<TList...>    BaseType;
        typedef Tuple<T, TList...> ThisType;    
        T val;
        Tuple(const ValueType& _val, const TList&... rest) : BaseType(rest...), val(_val){}
    };
    
    template<typename T>
    struct Tuple<T> : EmptyTuple/* ���ⵥ�����͵�������������������ݹ���յ� */
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