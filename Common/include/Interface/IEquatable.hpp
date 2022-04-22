/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:06:39
 * @LastEditors: like
 * @LastEditTime: 2022-04-22 15:24:45
 */
#ifndef SYSTEM_IEQUATABLE_HPP
#define SYSTEM_IEQUATABLE_HPP
#include "InterfaceDef.h"
namespace System
{
    template<typename T>
    class IEquatable;
};
/*
 *  Interface : 
 *		int Equals(const T& other) const
 */
template<typename T>
class System::IEquatable 
{
	DECLARE_CRTP_INTERFACE(IEquatable, T)
public:
	inline bool Equals(const IEquatable<T>& other) const CRTP_VIRTUAL
	{
		return CRTP_CONST_DERIVED.Equals(static_cast<const T&>(other));
	}
};
#endif