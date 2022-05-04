/*
 * @Description: 按照 http://www.cplusplus.com/reference/iterator/iterator/ 中的描述, 将迭代器的能力拆分为以下三个部分 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-29 15:34:08
 * @LastEditors: like
 * @LastEditTime: 2022-05-04 16:13:37
 */
#ifndef SYSTEM_ISELFOFFSETABLE_HPP
#define SYSTEM_ISELFOFFSETABLE_HPP
#include "InterfaceDef.h"
namespace System::Interface
{
    template<typename TDerived>
    class ISelfIncreaseable;
    template<typename TDerived>
    class ISelfDecreaseable;
    template<typename TDerived, typename TElement>
    class ISelfOffsetable;
};
/**
 * @brief 需要实现以下接口或其中一部分 
 */
#define CRTPInterfaceDefineWithCheck_Increase				CRTP_INTERFACE_OVERRIDED_ASSERT(void, Increase, const std::ptrdiff_t&);						
#define CRTPInterfaceDefineWithCheck_Decrease				CRTP_INTERFACE_OVERRIDED_ASSERT(void, Decrease, const std::ptrdiff_t&);
#define CRTPInterfaceDefineWithCheck_AccessNativeReference	CRTP_INTERFACE_OVERRIDED_ASSERT(TElement&, AccessNativeReference, int);
#define CRTPInterfaceDefineWithCheck_Distance				CRTP_INTERFACE_OVERRIDED_ASSERT(std::ptrdiff_t, Distance, const TDerived&);
/**
 * @brief 需要实现 CRTPInterfaceDefineWithCheck_Increase. 提供 obj++ 和 ++obj 运算符重载实现
 * 
 */
template<typename TDerived>
class System::Interface::ISelfIncreaseable
{
	DECLARE_CRTP_INTERFACE(ISelfIncreaseable, TDerived)
public:
    TDerived& operator++()
	{ 
		CRTPInterfaceDefineWithCheck_Increase;
		CRTP_DERIVED.Increase(1);
		return CRTP_DERIVED;
	}
	TDerived operator++(int)
	{
		CRTPInterfaceDefineWithCheck_Increase;
		TDerived temp(CRTP_CONST_DERIVED);
		CRTP_DERIVED.Increase(1);
		return temp;
	}
};
/**
 * @brief 需要实现 CRTPInterfaceDefineWithCheck_Decrease. 提供 obj-- 和 --obj 运算符重载实现
 * 
 */
template<typename TDerived>
class System::Interface::ISelfDecreaseable
{
	DECLARE_CRTP_INTERFACE(ISelfDecreaseable, TDerived)
public:
    TDerived& operator--()
	{ 
		CRTPInterfaceDefineWithCheck_Decrease;
		CRTP_DERIVED.Decrease(1);
		return CRTP_DERIVED;
	}
	TDerived operator--(int)
	{
		CRTPInterfaceDefineWithCheck_Decrease;
		TDerived temp(CRTP_CONST_DERIVED);
		CRTP_DERIVED.Decrease(1);
		return temp;
	}
};
/**
 * @brief 需要实现所有接口的实现, 提供 
 * obj + n  / obj - n 
 * obj += n / obj -= n
 * obj - obj 
 * obj[n]
 * 运算符重载实现
 * 
 */
template<typename TDerived, typename TElement>
class System::Interface::ISelfOffsetable
{
	DECLARE_CRTP_INTERFACE(ISelfOffsetable, TDerived, TElement);
public:
	TDerived operator+(const std::ptrdiff_t& n) const
	{
		CRTPInterfaceDefineWithCheck_Increase;
		TDerived temp(CRTP_CONST_DERIVED);
		temp.Increase(n);
		return temp;
	}
	TDerived operator-(const std::ptrdiff_t& n) const
	{
		CRTPInterfaceDefineWithCheck_Decrease;
		TDerived temp(CRTP_CONST_DERIVED);
		temp.Decrease(n);
		return temp;
	}
	TDerived& operator+= (const std::ptrdiff_t& n)
	{
		CRTPInterfaceDefineWithCheck_Increase;
        CRTP_DERIVED.Increase(n);
		return CRTP_DERIVED
	}
	TDerived& operator-= (const std::ptrdiff_t& n)
	{
		CRTPInterfaceDefineWithCheck_Decrease;
        CRTP_DERIVED.Decrease(n);
		return CRTP_DERIVED
	}
	TElement& operator[](const std::ptrdiff_t& n) const
	{
		CRTPInterfaceDefineWithCheck_AccessNativeReference;
		return CRTP_CONST_DERIVED.AccessNativeReference(n);
	}
	std::ptrdiff_t operator-(const TDerived& other) const
	{
		CRTPInterfaceDefineWithCheck_Distance;
		return CRTP_CONST_DERIVED.Distance(other);
	}
};
#endif