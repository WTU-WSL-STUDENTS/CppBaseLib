/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 10:08:02
 * @LastEditors: like
 * @LastEditTime: 2022-04-28 16:19:12
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
/* CRTP ������ */
#define CRTP_ABSTRACT

#endif