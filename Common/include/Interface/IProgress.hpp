/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-24 16:31:23
 * @LastEditors: like
 * @LastEditTime: 2022-04-25 17:13:26
 */
#ifndef SYSTEM_IPROGRESS_HPP
#define SYSTEM_IPROGRESS_HPP
#include "InterfaceDef.h"
#include <utility>

namespace System::Interface
{
    template<typename T>
    class IProgress;
};
/*
 *  Interface : 
 *		int Equals(const T& other) const
 */
template<typename T>
class System::Interface::IProgress
{
	DECLARE_CRTP_INTERFACE(IProgress, T)
public:
    template<typename TValue>
	inline void Report(TValue&& value) CRTP_VIRTUAL
	{
		CRTP_DERIVED.Report(std::forward<TValue>(value));
	}
};
#endif