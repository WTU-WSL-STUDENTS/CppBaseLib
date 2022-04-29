/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-26 10:44:13
 * @LastEditors: like
 * @LastEditTime: 2022-04-29 15:32:01
 */
#ifndef SYSTEM_ICOLLECTION_HPP
#define SYSTEM_ICOLLECTION_HPP
#include "IEnumerable.hpp"

namespace System::Interface
{
    template<typename TDerived, typename TElement>
    class ICollection;
};

template<typename TDerived, typename TElement>
class System::Interface::ICollection : public IEnumerable<TDerived, TElement>
{
	DECLARE_CRTP_INTERFACE(ICollection, TDerived, TElement)
protected:
    int m_nCount{0};
public:
    int Count() const
    {
        return m_nCount;
    }
    virtual bool IsReadOnly() const
    {
        return false;
	}
    virtual bool IsSynchronized() const
    {
        return false;
	}

	TElement& operator[](int index) CRTP_VIRTUAL
	{
		return CRTP_DERIVED[index];
	}
    auto begin()CRTP_VIRTUAL
	{
		return CRTP_DERIVED.begin();
	}
    auto end()CRTP_VIRTUAL
	{
		return CRTP_DERIVED.end();
	}
    void Add(const TElement& item) CRTP_VIRTUAL
    {
        CRTP_DERIVED.Add(std::forward<TElement>(item));
    }
    void Clear() CRTP_VIRTUAL
    {
        CRTP_DERIVED.Clear();
    }
    bool Contains(const TElement& item) CRTP_VIRTUAL
    {
        return CRTP_DERIVED.Contains(item);
    }
    void CopyTo(TElement* buffer, int bufferOffset) CRTP_VIRTUAL
    {
        CRTP_DERIVED.CopyTo(buffer, bufferOffset);
    }
    bool Remove(const TElement& item) CRTP_VIRTUAL
    {
        return CRTP_DERIVED.Contains(item);
    }


};
#endif