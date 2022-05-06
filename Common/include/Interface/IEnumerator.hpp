/*
 * @Description:  http://www.cplusplus.com/reference/iterator/iterator/
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-24 17:01:55
 * @LastEditors: like
 * @LastEditTime: 2022-05-04 16:27:06
 */
#ifndef SYSTEM_IENUMERATOR_HPP
#define SYSTEM_IENUMERATOR_HPP
#include "IDisposable.hpp"
#include "IComparable.hpp"
#include "ISelfOffsetable.hpp"
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING
#define GET_ITERATOR_NATIVE_POINTER(iterator) (&(*(iterator)))

namespace System::Interface
{
	/**
	 * IEnumerator 中可选 Category
	 *   iterator tag				| Category of iterators
	 * input_iterator_tag			|	Input Iterator
	 * output_iterator_tag			|	Output Iterator
	 * forward_iterator_tag			|	Forward Iterator
	 * bidirectional_iterator_tag	|	Bidirectional Iterator
	 * random_access_iterator_tag	|	Random-access Iterator
	 * 
	 */
	template<typename TDerived, typename TElement>
	struct OutputIterator;
	template<typename TDerived, typename TElement>
	struct InputIterator;
	template<typename TDerived, typename TElement>
	struct ForwardIterator;
	template<typename TDerived, typename TElement>
	struct BidirectionalIterator;
	template<typename TDerived, typename TElement>
	struct RandomAccessIterator;
	/**
	 * @brief 抽象 iterator
	 * 
	 */
    template<typename TDerived, typename TElement, typename Category>
	class IEnumerator;
    /**
     * @brief 内存连续的迭代器(指针)
     * 
     */
	template<typename TElement>
	class SequenceMemoryEnumerator;
};
template<typename TDerived, typename TElement>
struct System::Interface::OutputIterator : 
	public ISelfIncreaseable<TDerived>, 
	public std::output_iterator_tag
{
	TDerived& operator=(TDerived& other) CRTP_VIRTUAL
	{
		return static_cast<TDerived&>(*this).operator=(other);
	}
	TElement* operator->() CRTP_VIRTUAL
	{
		return static_cast<const TDerived&>(*this).operator->();
	}
	TElement& operator*() CRTP_VIRTUAL
	{
		return *(static_cast<TDerived&>(*this));
	}
};
template<typename TDerived, typename TElement>
struct System::Interface::InputIterator : 
	public ISelfIncreaseable<TDerived>, 
	public IEquatable<TDerived>, 
	public std::input_iterator_tag
{
	TDerived& operator=(TDerived& other) CRTP_VIRTUAL
	{
		return static_cast<TDerived&>(*this).operator=(other);
	}
	const TElement* operator->()const CRTP_VIRTUAL
	{
		return static_cast<const TDerived&>(*this).operator->();
	}
	const TElement& operator*()const CRTP_VIRTUAL
	{
		return *(static_cast<const TDerived&>(*this));
	}
};
template<typename TDerived, typename TElement>
struct System::Interface::ForwardIterator : 
	public InputIterator<TDerived, TElement>, 
	public OutputIterator<TDerived, TElement>, 
	public std::forward_iterator_tag
{
	TDerived& operator=(TDerived& other) CRTP_VIRTUAL
	{
		return InputIterator<TDerived, TElement>::operator=(other);
	}

	TDerived& operator++() CRTP_VIRTUAL
	{
		CRTPInterfaceDefineWithCheck_Increase;
		CRTP_DERIVED.Increase(1);
		return CRTP_DERIVED;
	}
	TDerived operator++(int) CRTP_VIRTUAL
	{
		CRTPInterfaceDefineWithCheck_Increase;
		TDerived temp(CRTP_CONST_DERIVED);
		CRTP_DERIVED.Increase(1);
		return temp;
	}
	const TElement* operator->()const CRTP_VIRTUAL
	{
		return InputIterator<TDerived, TElement>::operator->(); CRTP_VIRTUAL
	}
	const TElement& operator*()const CRTP_VIRTUAL
	{
		return InputIterator<TDerived, TElement>::operator*();
	}
	TElement* operator->() CRTP_VIRTUAL
	{
		return OutputIterator<TDerived, TElement>::operator->();
	}
	TElement& operator*() CRTP_VIRTUAL
	{
		return OutputIterator<TDerived, TElement>::operator*();
	}
};
template<typename TDerived, typename TElement>
struct System::Interface::BidirectionalIterator : 
	public ISelfDecreaseable<TDerived>, 
	public ForwardIterator<TDerived, TElement>, 
	public std::bidirectional_iterator_tag
{
};
template<typename TDerived, typename TElement>
struct System::Interface::RandomAccessIterator : 
	public ISelfOffsetable<TDerived, TElement>,
	public IComparable<TDerived>,
	public BidirectionalIterator<TDerived, TElement>, 
	public std::random_access_iterator_tag
{
};
#define CRTPInterfaceDefineWithCheck_IsValid CRTP_OVERRIDED_ASSERT(IS_CRTP_INTERFACE_OVERRIDED_NO_PARAMS(bool, IsValid));	
template<typename TDerived, typename TElement, typename Category = System::Interface::RandomAccessIterator<TDerived, TElement>>
CRTP_ABSTRACT class System::Interface::IEnumerator :
	public Category,
	public std::iterator<Category, TElement>
{
    DECLARE_CRTP_INTERFACE(IEnumerator, TDerived, TElement, Category)
public:
	explicit operator bool() const
	{
		return IsValid();
	}
private:
	bool IsValid() const
	{
		CRTPInterfaceDefineWithCheck_IsValid;
		return CRTP_CONST_DERIVED.IsValid();
	}
};
template<typename TElement>
class System::Interface::SequenceMemoryEnumerator :
	public IEnumerator<SequenceMemoryEnumerator<TElement>, TElement, RandomAccessIterator<SequenceMemoryEnumerator<TElement>, TElement>>
{
private:
	WEAK_PTR(TElement)  m_p;
public:
	explicit SequenceMemoryEnumerator(WEAK_PTR(TElement) x) : m_p(x) {}
	SequenceMemoryEnumerator(const SequenceMemoryEnumerator& mit) : m_p(mit.m_p) {}
	SequenceMemoryEnumerator& operator= (const SequenceMemoryEnumerator& mit) CRTP_OVERRIDE 
	{
		m_p = mit.m_p; 
		return *this; 
	}
	TElement& operator*() CRTP_OVERRIDE
	{
		return *m_p;
	}
	TElement& operator*()const CRTP_OVERRIDE
	{
		return *m_p;
	}
	TElement* operator->() CRTP_OVERRIDE
	{
		return m_p;
	}
	const TElement* operator->()const CRTP_OVERRIDE
	{
		return m_p;
	}
	int CompareTo(const SequenceMemoryEnumerator<TElement>& other) const CRTP_OVERRIDE
	{
		return m_p > other.m_p ? 1 : m_p < other.m_p ? -1 : 0;
	}
	bool Equals(const SequenceMemoryEnumerator<TElement>& other) const CRTP_OVERRIDE
	{
		return m_p == other.m_p;
	}
	void Increase(std::ptrdiff_t n) CRTP_OVERRIDE
	{
		m_p += n;
	}
	void Decrease(std::ptrdiff_t n) CRTP_OVERRIDE
	{
		m_p -= n;
	}
	std::ptrdiff_t Distance(const SequenceMemoryEnumerator<TElement>& other) const CRTP_OVERRIDE
	{
		return m_p - other.m_p;
	}
	TElement& AccessNativeReference(std::ptrdiff_t index) const CRTP_OVERRIDE
	{
		return m_p[index];
	}
	bool IsValid() const CRTP_OVERRIDE
	{
		return m_p;
	}
};
#endif