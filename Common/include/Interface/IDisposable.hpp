/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:26:30
 * @LastEditors: like
 * @LastEditTime: 2022-05-04 16:21:09
 */
#ifndef SYSTEM_IDISPOSABLE_HPP
#define SYSTEM_IDISPOSABLE_HPP
#include "InterfaceDef.h"

namespace System::Interface
{
    template<typename TDerived>
    class IDisposable;
};
template<typename TDerived>
class System::Interface::IDisposable
{
	DECLARE_CRTP_INTERFACE(IDisposable, TDerived)
public:
	void Dispose() CRTP_VIRTUAL
	{
		return CRTP_DERIVED.Dispose();
	}
};
#endif