/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 10:08:02
 * @LastEditors: like
 * @LastEditTime: 2022-04-30 20:40:56
 */
#ifndef INTERFACE_DEF_H
#define INTERFACE_DEF_H

#include <iterator> 
#include <type_traits>
#include "../CompliedEntry.h"
#include "../CompliedIndexer.h"
#include "../Complied.FunctionTraits.hpp"

/* CRTP 接口 / 抽象类 通用定义 */
#define _DECLARE_CRTP_INTERFACE(interfaceName, derivedType, ...) \
    friend derivedType;				        \
    protected: interfaceName() = default;	\
    public:                                 \
    virtual ~interfaceName() = default;	    \
    using _##interfaceName= interfaceName< ##__VA_ARGS__ >;
#define DECLARE_CRTP_INTERFACE(interfaceName, derivedType, ...) \
    _DECLARE_CRTP_INTERFACE(interfaceName, derivedType, derivedType, __VA_ARGS__)


#define CRTP_DERIVED		static_cast<TDerived&>(*this)
#define CRTP_CONST_DERIVED	static_cast<const TDerived&>(*this)
/* CRTP 接口定义 */
#define CRTP_VIRTUAL
/* CRTP 接口实现 */
#define CRTP_OVERRIDE
/**
 * @brief CRTP 抽象类标记
 * 
 */
#define CRTP_ABSTRACT

/**
 * @brief 申明 CRTP 接口函数, 接口函数签名最多支持 8 个输入变量
 * 1. 如果函数名称和输入参数与设定签名不一致, 编译报错
 * 2. 如果返回值与设定签名不一致, 则内置 flag ( CRTP_OVERRIDED_CHECK_xx ) 标志为 false. 可通过 IS_CRTP_OVERRIDED( func ) 获取指定函数是否重载成功
 */
#define REGEIST_CRTP_INTERFACE(retType, func, ...)          _REGEIST_CRTP_INTERFACE(retType, func, MACRO_ARG_COUNT(__VA_ARGS__), ##__VA_ARGS__)
#define _REGEIST_CRTP_INTERFACE(retType, func, count, ...)  __REGEIST_CRTP_INTERFACE(retType, func, count, ##__VA_ARGS__)
#define __REGEIST_CRTP_INTERFACE(retType, func, count, ...) constexpr bool CRTP_OVERRIDED_FLAG_##func  = ___REGEIST_CRTP_INTERFACE(retType, func, ADD_RVALUE_REFERENCE_T_##count (__VA_ARGS__))
//#define __REGEIST_CRTP_INTERFACE(retType, func, count, ...) \
//        constexpr bool CRTP_OVERRIDED_FLAG_##func =  count ?                             \
//        ___REGEIST_CRTP_INTERFACE(retType, func, ADD_RVALUE_REFERENCE_T_##count (__VA_ARGS__)) :\
//        (std::is_void<std::void_t<decltype(std::declval<TDerived&>().##func ())>>::value && FUNCTION_RETURN_TYPE_IS_THAT(retType, TDerived::##func))
#define ___REGEIST_CRTP_INTERFACE(retType, func, ...)                                               \
    (std::is_void<std::void_t<decltype(std::declval<TDerived&>().##func ( __VA_ARGS__ ))>>::value && FUNCTION_RETURN_TYPE_IS_THAT(retType, TDerived::##func))
//#define ADD_RVALUE_REFERENCE_T_0 
#define ADD_RVALUE_REFERENCE_T_1(t1)	std::declval<t1>()
#define ADD_RVALUE_REFERENCE_T_2(t1, t2) ADD_RVALUE_REFERENCE_T_1(t1), ADD_RVALUE_REFERENCE_T_1(t2)
#define ADD_RVALUE_REFERENCE_T_3(t1, ...) ADD_RVALUE_REFERENCE_T_1(t1), ADD_RVALUE_REFERENCE_T_2(__VA_ARGS__)
#define ADD_RVALUE_REFERENCE_T_4(t1, ...) ADD_RVALUE_REFERENCE_T_1(t1), ADD_RVALUE_REFERENCE_T_3(__VA_ARGS__)
#define ADD_RVALUE_REFERENCE_T_5(t1, ...) ADD_RVALUE_REFERENCE_T_1(t1), ADD_RVALUE_REFERENCE_T_4(__VA_ARGS__)
#define ADD_RVALUE_REFERENCE_T_6(t1, ...) ADD_RVALUE_REFERENCE_T_1(t1), ADD_RVALUE_REFERENCE_T_5(__VA_ARGS__)
#define ADD_RVALUE_REFERENCE_T_7(t1, ...) ADD_RVALUE_REFERENCE_T_1(t1), ADD_RVALUE_REFERENCE_T_6(__VA_ARGS__)
#define ADD_RVALUE_REFERENCE_T_8(t1, ...) ADD_RVALUE_REFERENCE_T_1(t1), ADD_RVALUE_REFERENCE_T_7(__VA_ARGS__)
/**
 * @brief 如果 CRTP_Type::func 如果 CRTP 的接口已经重载, 宏定义的结果为 true 否则为 false.
 * 
 */
#define IS_CRTP_OVERRIDED(func) (CRTP_OVERRIDED_FLAG_##func)
/**
 * @brief CRTP 派生类是否重载接口函数的静态检查
 * 
 */
#define CRTP_OVERRIDED_ASSERT(func) static_assert(IS_CRTP_OVERRIDED(func), "CRTP interface not impliment")
#define CRTP_INTERFACE_OVERRIDED_ASSERT(retType, func, ...) REGEIST_CRTP_INTERFACE(retType, func, ##__VA_ARGS__); CRTP_OVERRIDED_ASSERT(func);
#endif