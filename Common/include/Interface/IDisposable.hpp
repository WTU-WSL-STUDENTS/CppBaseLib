/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 15:26:30
 * @LastEditors: like
 * @LastEditTime: 2022-04-22 15:28:11
 */
#ifndef SYSTEM_IDISPOSABLE_HPP
#define SYSTEM_IDISPOSABLE_HPP
namespace System
{
    template<typename T>
    class IDisposable;
};
/*
 *  Interface : 
 *		int Equals(const T& other) const
 */
template<typename T>
class System::IDisposable 
{
	DECLARE_CRTP_INTERFACE(IDisposable, T)
public:
	inline void Dispose() CRTP_VIRTUAL
	{
		return CRTP_DERIVED.Dispose();
	}
};
#endif