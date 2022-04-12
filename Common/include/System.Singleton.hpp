/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-03-31 17:12:32
 * @LastEditors: like
 * @LastEditTime: 2022-03-31 18:06:20
 */
#ifndef SYSTEM_SINGLETON_HPP
#include <CompliedEntry.h>
#include <System.Threading.ReaderWriterLockSlim.hpp>
namespace System
{
    template<typename T, template<class> class SingletonConstructPolicy>
    class SingletonWithAnyPolicy;
};
template<typename T, template<class> class SingletonConstructPolicy>
class System::SingletonWithAnyPolicy : private SingletonConstructPolicy<T>
{
private:
    T* m_instance;
    SingletonWithAnyPolicy() : m_instance(NULL)
    {
    }
public:
	~SingletonWithAnyPolicy()
	{
		if (m_instance)
		{
			SingletonConstructPolicy<T>::Free(m_instance);
		}
	}
	static SingletonWithAnyPolicy<T, SingletonConstructPolicy> Singleton;
	static System::Threading::CriticalSection SingletonCrticalSec;
    static T& Ref()
    {
        if(Singleton.m_instance)
        {
            return *Singleton.m_instance;
        }
		SingletonCrticalSec.Enter();
		if (!Singleton.m_instance)
        {
			SingletonConstructPolicy<T>::Construct(Singleton.m_instance);
        }
		SingletonCrticalSec.Leave();
		return *Singleton.m_instance;
    }
};
template<typename T, template<class> class SingletonConstructPolicy>
System::SingletonWithAnyPolicy<T, SingletonConstructPolicy> System::SingletonWithAnyPolicy<T, SingletonConstructPolicy>::Singleton;
template<typename T, template<class> class SingletonConstructPolicy>
System::Threading::CriticalSection System::SingletonWithAnyPolicy<T, SingletonConstructPolicy>::SingletonCrticalSec;

template<typename T>
class SingletonHasDefultConstructPolicy
{
public:
    static inline void Construct(T* (&p))
    {
        p = new T();
    }
    static inline void Free(T* (&p))
    {
        delete p;
        p = NULL;
    }
};
template<typename T>
using Singleton = System::SingletonWithAnyPolicy<T, SingletonHasDefultConstructPolicy>;
#endif