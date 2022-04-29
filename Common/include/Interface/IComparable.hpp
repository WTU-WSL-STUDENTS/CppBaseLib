/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 10:04:14
 * @LastEditors: like
 * @LastEditTime: 2022-04-29 16:34:09
 */
#ifndef SYSTEM_ICOMPARABLE_HPP
#define SYSTEM_ICOMPARABLE_HPP
#include "InterfaceDef.h"

namespace System::Interface
{
	template<typename TDerived>
	class IComparable;
};
/*
 *  Interface : 
 *		int CompareTo(const TDerived& other) const
 */
template<typename TDerived>
class System::Interface::IComparable
{
	DECLARE_CRTP_INTERFACE(IComparable, TDerived)
public:
	bool IComparable::operator > (IComparable<TDerived>& other)
	{
		return 0 < CompareTo(other);
	}
	bool IComparable::operator >= (IComparable<TDerived>& other)
	{
		return 0 <= CompareTo(other);
	}
	bool IComparable::operator == (IComparable<TDerived>& other)
	{
		return 0 == CompareTo(other);
	}
	bool IComparable::operator < (IComparable<TDerived>& other)
	{
		return 0 > CompareTo(other);
	}
	bool IComparable::operator <= (IComparable<TDerived>& other)
	{
		return 0 >= CompareTo(other);
	}
private:
	/**
	 * @brief 
	 * 
	 * @param other 
	 * @return int
	 *	0 equal 
	 * 	大于 0 : this bigger than other
	 * 	小于 0 : this smaller than other 
	 */
	int CompareTo(const IComparable<TDerived>& other) const CRTP_VIRTUAL
	{
		return CRTP_CONST_DERIVED.CompareTo(static_cast<const TDerived&>(other));
	}
};
#endif