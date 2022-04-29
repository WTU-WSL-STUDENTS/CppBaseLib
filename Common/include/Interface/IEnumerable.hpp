/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-29 15:30:59
 * @LastEditors: like
 * @LastEditTime: 2022-04-29 16:03:12
 */
/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-24 17:01:55
 * @LastEditors: like
 * @LastEditTime: 2022-04-26 15:06:11
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
#ifdef DOT_NET_ITERATOR_COMPATIBLE
    auto GetEnumerator()
	{
		return begin();
	};
#endif
    auto begin() CRTP_VIRTUAL
	{
		return CRTP_DERIVED.begin();
	}
    auto end() CRTP_VIRTUAL
	{
		return CRTP_DERIVED.end();
	}
};

#endif