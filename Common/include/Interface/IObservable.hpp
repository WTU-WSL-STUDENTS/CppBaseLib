/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:10:22
 * @LastEditors: like
 * @LastEditTime: 2022-04-25 17:13:34
 */
#ifndef SYSTEM_IOBSERVABLE_HPP
#define SYSTEM_IOBSERVABLE_HPP
#include "InterfaceDef.h"
#include "IDisposable.hpp"
#include "IObserver.hpp"
#include <memory>

namespace System::Interface
{
    template<typename TDerived, typename TValue>
    class IObservable;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.iobservable-1.subscribe?view=net-6.0 */
};
/*
 *  Interface : 
 *		auto Subscribe(const TDerived& other) TDerived<TValue>
 */
template<typename TDerived, typename TValue>
class System::Interface::IObservable 
{
	DECLARE_CRTP_INTERFACE(IObservable, TDerived, TValue)
public:
	template<typename ObserverType>
	inline auto Subscribe(const IObserver<ObserverType, TValue>& observer) CRTP_VIRTUAL
	{
		return CRTP_DERIVED.Subscribe(static_cast<const ObserverType&>(observer));
	}
};
#endif