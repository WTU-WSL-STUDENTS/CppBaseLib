/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:06:39
 * @LastEditors: like
 * @LastEditTime: 2022-04-29 16:27:21
 */
#ifndef SYSTEM_IEQUATABLE_HPP
#define SYSTEM_IEQUATABLE_HPP
#include "InterfaceDef.h"
namespace System::Interface
{
    template<typename TDerived>
    class IEquatable;
};
/*
 *  Interface : 
 *		int Equals(const TDerived& other) const
 */
template<typename TDerived>
class System::Interface::IEquatable 
{
	DECLARE_CRTP_INTERFACE(IEquatable, TDerived)
public:
	bool IEquatable::operator == (const IEquatable<TDerived>& other)
	{
		return Equals(other);
	}
	bool IEquatable::operator != (const IEquatable<TDerived>& other)
	{
		return !Equals(other);
	}
private:
	bool Equals(const IEquatable<TDerived>& other) const CRTP_VIRTUAL
	{
		return CRTP_CONST_DERIVED.Equals(static_cast<const TDerived&>(other));
	}
};
#endif