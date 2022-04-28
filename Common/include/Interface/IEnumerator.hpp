/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-24 17:01:55
 * @LastEditors: like
 * @LastEditTime: 2022-04-26 15:06:11
 */
#ifndef SYSTEM_IENUMERATOR_HPP
#define SYSTEM_IENUMERATOR_HPP
#include "IDisposable.hpp"
#include <memory>

namespace System::Interface
{
    template<typename TDerived, typename TElement>
    class IEnumerator;
    template<typename TDerived, typename TElement, typename TCategory>
    class IEnumerable;
};

template<typename TDerived, typename TElement>
class System::Interface::IEnumerator
{
	DECLARE_CRTP_INTERFACE(IEnumerator, TDerived, TElement)
private: 
    WEAK_PTR(TDerived)  m_p;
    int m_nElementCount;
	int m_nCurrent;
    IEnumerator(WEAK_PTR(TDerived) ie, int elementCount) : m_p(ie), m_nElementCount(elementCount), m_nCurrent(0) {}
public:
    TElement&  Current()const
    {
        return m_p->begin()[m_nCurrent];
    }
    bool MoveNext() 
    {
        return ++m_nCurrent < m_nElementCount;
    }
    void Reset()
    {
        m_nCurrent = 0;
    }

    auto begin() const
	{
        return m_p->begin();
    }
    auto end() const
	{
		return m_p->end();
	}
    operator std::initializer_list<TElement>() const
    {
        return std::initializer_list<TElement>(begin(), end());
    }
};

template<typename TDerived, typename TElement , typename TCategory = std::random_access_iterator_tag>
class System::Interface::IEnumerable
{
	DECLARE_CRTP_INTERFACE(IEnumerable, TDerived, TElement)
public:
	using _IEnumerator = IEnumerator<_IEnumerable, TElement>;
    _IEnumerator GetEnumerator()
    {
        return _IEnumerator(this, CRTP_DERIVED.Count());
	};
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