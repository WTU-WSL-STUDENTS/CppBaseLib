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

/* CRTP �ӿ� / ������ ͨ�ö��� */
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
/* CRTP �ӿڶ��� */
#define CRTP_VIRTUAL
/* CRTP �ӿ�ʵ�� */
#define CRTP_OVERRIDE
/**
 * @brief CRTP ��������
 * 
 */
#define CRTP_ABSTRACT

/**
 * @brief ���� CRTP �ӿں���, �ӿں���ǩ�����֧�� 8 ���������
 * 1. ����������ƺ�����������趨ǩ����һ��, ���뱨��
 * 2. �������ֵ���趨ǩ����һ��, ������ flag ( CRTP_OVERRIDED_CHECK_xx ) ��־Ϊ false. ��ͨ�� IS_CRTP_OVERRIDED( func ) ��ȡָ�������Ƿ����سɹ�
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
 * @brief ��� CRTP_Type::func ��� CRTP �Ľӿ��Ѿ�����, �궨��Ľ��Ϊ true ����Ϊ false.
 * 
 */
#define IS_CRTP_OVERRIDED(func) (CRTP_OVERRIDED_FLAG_##func)
/**
 * @brief CRTP �������Ƿ����ؽӿں����ľ�̬���
 * 
 */
#define CRTP_OVERRIDED_ASSERT(func) static_assert(IS_CRTP_OVERRIDED(func), "CRTP interface not impliment")
#define CRTP_INTERFACE_OVERRIDED_ASSERT(retType, func, ...) REGEIST_CRTP_INTERFACE(retType, func, ##__VA_ARGS__); CRTP_OVERRIDED_ASSERT(func);
#endif