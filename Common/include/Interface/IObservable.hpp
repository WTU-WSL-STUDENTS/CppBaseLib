/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:10:22
 * @LastEditors: like
 * @LastEditTime: 2022-04-22 15:52:35
 */
#ifndef SYSTEM_IOBSERVABLE_HPP
#define SYSTEM_IOBSERVABLE_HPP
#include "InterfaceDef.h"
#include "IDisposable.hpp"
#include "IObserver.hpp"
#include <memory>

namespace System
{
    template<typename T, typename TValue>
    class IObservable;/* https://docs.microsoft.com/zh-cn/dotnet/api/system.iobservable-1.subscribe?view=net-6.0 */
};
/*
 *  Interface : 
 *		std::unique_ptr<IDisposable<UnsubscriberType>> Subscribe(const T& other) T<TValue>
 */
template<typename T, typename TValue>
class System::IObservable 
{
	DECLARE_CRTP_INTERFACE(IObservable, T)
public:
    template<typename UnsubscriberType>
	inline std::shared_ptr<IDisposable<UnsubscriberType>> Subscribe(const IObserver<T, TValue>& observer) CRTP_VIRTUAL
	{
		return CRTP_DERIVED.Subscribe(static_cast<const T&>(observer));
	}
};
#endif