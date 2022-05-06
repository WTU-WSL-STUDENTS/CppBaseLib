/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 10:04:14
 * @LastEditors: like
 * @LastEditTime: 2022-05-04 16:20:17
 */
#ifndef SYSTEM_ICOMPARABLE_HPP
#define SYSTEM_ICOMPARABLE_HPP
#include "InterfaceDef.h"

namespace System::Interface
{
	template<typename TDerived>
	class IComparable;
    template<typename TDerived>
    class IEquatable;
};
#define CRTPInterfaceDefineWithCheck_CompareTo CRTP_INTERFACE_OVERRIDED_ASSERT(int, CompareTo, const TDerived&);	
#define CRTPInterfaceDefineWithCheck_Equals CRTP_INTERFACE_OVERRIDED_ASSERT(bool, Equals, const TDerived&);	
/**
 * @brief 实现 CompareTo 接口, 提供所有比较运算符的重载实现
 * 
 */
template<typename TDerived>
class System::Interface::IComparable
{
	DECLARE_CRTP_INTERFACE(IComparable, TDerived)
public:
	/*bool IComparable::operator > (const TDerived& other) const
	{
		return 0 < CRTPCompareTo(other);
	}
	bool IComparable::operator >= (const TDerived& other) const
	{
		return 0 <= CRTPCompareTo(other);
	}
	bool IComparable::operator < (const TDerived& other) const
	{
		return 0 > CRTPCompareTo(other);
	}
	bool IComparable::operator <= (const TDerived& other) const
	{
		return 0 >= CRTPCompareTo(other);
	}*/
	bool IComparable::operator > (const _IComparable& other) const
	{
		return 0 < CRTPCompareTo(other);
	}
	bool IComparable::operator >= (const _IComparable& other) const
	{
		return 0 <= CRTPCompareTo(other);
	}
	bool IComparable::operator < (const _IComparable& other) const
	{
		return 0 > CRTPCompareTo(other);
	}
	bool IComparable::operator <= (const _IComparable& other) const
	{
		return 0 >= CRTPCompareTo(other);
	}
private:
	/**
	 * @brief 
	 * 
	 * @param other 
	 * @return int
	 *	0 : *this == other
	 * >0 : *this > other
	 * <0 : *this < other
	 */
	int CRTPCompareTo(const IComparable<TDerived>& other) const CRTP_VIRTUAL
	{
		CRTPInterfaceDefineWithCheck_CompareTo;
		return CRTP_CONST_DERIVED.CompareTo(static_cast<const TDerived&>(other));
	}
};
/**
 * @brief 实现 Equals 接口, 提供 == 和 != 运算符的重载实现
 * 
 */
template<typename TDerived>
class System::Interface::IEquatable 
{
	DECLARE_CRTP_INTERFACE(IEquatable, TDerived)
public:
	bool IEquatable::operator == (const TDerived& other) const
	{
		return CRTPEquals(other);
	}
	bool IEquatable::operator != (const TDerived& other) const
	{
		return !CRTPEquals(other);
	}
private:
	bool CRTPEquals(const TDerived& other) const CRTP_VIRTUAL
	{
		CRTPInterfaceDefineWithCheck_Equals;
		return CRTP_CONST_DERIVED.Equals(other);
	}
};
#endif