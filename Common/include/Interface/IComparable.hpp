/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 10:04:14
 * @LastEditors: like
 * @LastEditTime: 2022-04-22 15:09:15
 */
#ifndef SYSTEM_ICOMPARABLE_HPP
#define SYSTEM_ICOMPARABLE_HPP
#include "InterfaceDef.h"

namespace System
{
	template<typename T>
	class IComparable;
};
/*
 *  Interface : 
 *		int CompareTo(const T& other) const
 */
template<typename T>
class System::IComparable
{
	DECLARE_CRTP_INTERFACE(IComparable, T)
public:
	/**
	 * @brief 
	 * 
	 * @param other 
	 * @return int
	 *	0 equal 
	 * 	大于 0 : this bigger than other
	 * 	小于 0 : this smaller than other 
	 */
	inline int CompareTo(const IComparable<T>& other) const CRTP_VIRTUAL
	{
		return CRTP_CONST_DERIVED.CompareTo(static_cast<const T&>(other));
	}
	bool IComparable::operator > (IComparable<T>& other)
	{
		return 0 < CompareTo(other);
	}
	bool IComparable::operator >= (IComparable<T>& other)
	{
		return 0 <= CompareTo(other);
	}
	bool IComparable::operator == (IComparable<T>& other)
	{
		return 0 == CompareTo(other);
	}
	bool IComparable::operator < (IComparable<T>& other)
	{
		return 0 > CompareTo(other);
	}
	bool IComparable::operator <= (IComparable<T>& other)
	{
		return 0 >= CompareTo(other);
	}
};
#endif