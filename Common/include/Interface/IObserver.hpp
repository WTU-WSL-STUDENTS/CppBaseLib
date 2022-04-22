/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:38:06
 * @LastEditors: like
 * @LastEditTime: 2022-04-22 15:50:01
 */
/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:10:22
 * @LastEditors: like
 * @LastEditTime: 2022-04-22 15:37:35
 */
#ifndef SYSTEM_IOBSERVER_HPP
#define SYSTEM_IOBSERVER_HPP
#include "InterfaceDef.h"
#include "IDisposable.hpp"
#include <exception>
namespace System
{
    template<typename T, typename TValue>
    class IObserver;
};
/*
 *  Interface : 
 *		IDisposable<UnsubscriberType> Subscribe(const T& other)
 */
template<typename T, typename TValue>
class System::IObserver 
{
	//DECLARE_CRTP_INTERFACE(IObserver, T)
public:
	inline void OnCompleted() CRTP_VIRTUAL
	{
		CRTP_DERIVED.OnCompleted();
	}
	inline void OnError(const std::exception& e) CRTP_VIRTUAL
	{
		CRTP_DERIVED.OnError(e);
	}
	inline void OnNext(const TValue& value) CRTP_VIRTUAL
	{
		CRTP_DERIVED.OnNext(value);
	}
};
#endif