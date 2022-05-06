/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-24 17:01:55
 * @LastEditors: like
 * @LastEditTime: 2022-05-06 14:25:26
 */
#ifndef SYSTEM_IENUMERABLE_HPP
#define SYSTEM_IENUMERABLE_HPP
#include "IEnumerator.hpp"

namespace System::Interface
{
    template<typename TDerived, typename TElement>
    class IEnumerable;
};

template<typename TDerived, typename TElement>
class System::Interface::IEnumerable
{
	DECLARE_CRTP_INTERFACE(IEnumerable, TDerived, TElement)
public:
    auto begin() const CRTP_VIRTUAL
	{
		return CRTP_CONST_DERIVED.begin();
	}
    auto end() const CRTP_VIRTUAL
	{
		return CRTP_CONST_DERIVED.end();
	}
};

#endif