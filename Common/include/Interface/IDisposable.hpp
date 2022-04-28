/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:26:30
 * @LastEditors: like
 * @LastEditTime: 2022-04-25 17:13:40
 */
#ifndef SYSTEM_IDISPOSABLE_HPP
#define SYSTEM_IDISPOSABLE_HPP
#include "InterfaceDef.h"

namespace System::Interface
{
    template<typename TDerived>
    class IDisposable;
};
/*
 *  Interface : 
 *		int Equals(const TDerived& other) const
 */
template<typename TDerived>
class System::Interface::IDisposable
{
	DECLARE_CRTP_INTERFACE(IDisposable, TDerived)
public:
	inline void Dispose() CRTP_VIRTUAL
	{
		return CRTP_DERIVED.Dispose();
	}
};
#endif