/*
 * @Description:  http://www.cplusplus.com/reference/iterator/iterator/
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-24 17:01:55
 * @LastEditors: like
 * @LastEditTime: 2022-04-29 17:05:21
 */
#ifndef SYSTEM_IENUMERATOR_HPP
#define SYSTEM_IENUMERATOR_HPP
#include "IDisposable.hpp"
#include "IEquatable.hpp"
#include "IComparable.hpp"
#include "ISelfOffsetable.hpp"

namespace System::Interface
{
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
    template<typename TDerived, typename TElement, typename Category>
	class IEnumerator;
    /**
     * @brief 内存连续的迭代器(指针)
     * 
     * @tparam TElement 
     * @tparam Category 
     */
	/*template<typename TElement>
	class SequenceMemoryEnumerator;*/
};
/**
 * @brief 需要实现的接口
 * CompareTo / Equals / operator->() / operator*() / operator=() / Increase / Decrease
 */
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
	TDerived& operator=(TDerived& other)
	{
		return InputIterator<TDerived, TElement>::operator=(other);
	}
	TDerived& operator++()
	{
		return InputIterator<TDerived, TElement>::operator++();
	}
	TDerived operator++(int n)
	{
		return InputIterator<TDerived, TElement>::operator++(n);
	}
	const TElement* operator->()const
	{
		return InputIterator<TDerived, TElement>::operator->();
	}
	const TElement& operator*()const
	{
		return InputIterator<TDerived, TElement>::operator*();
	}
	TElement* operator->()
	{
		return OutputIterator<TDerived, TElement>::operator->();
	}
	TElement& operator*()
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
	public ISelfOffsetable<TDerived>, 
	public IComparable<TDerived>,
	public BidirectionalIterator<TDerived, TElement>, 
	public std::random_access_iterator_tag
{
};

template<typename TDerived, typename TElement, typename Category /*= System::Interface::RandomAccessIterator<TDerived, TElement>*/>
class System::Interface::IEnumerator : 
	public IEquatable<TDerived>, 
	public Category,
	public std::iterator<Category, TElement>
{
    DECLARE_CRTP_INTERFACE(IEnumerator, TDerived, TElement, Category)
protected:
	WEAK_PTR(TElement)  m_p;
public:
    IEnumerator(WEAK_PTR(TElement) x) : m_p(x) {}
    IEnumerator(const IEnumerator& mit) : m_p(mit.m_p) {}

	TElement& operator*() CRTP_OVERRIDE
	{
		return *m_p;
	}
	const TElement& operator*()const CRTP_OVERRIDE
	{
		return *p;
	}
	TElement* operator->() CRTP_OVERRIDE
	{
		return p;
	}
	const TElement* operator->()const CRTP_OVERRIDE
	{
		return p;
	}
	int CompareTo(const IEnumerator& other) const CRTP_OVERRIDE
	{
		return m_p > other.m_p ? 1 : m_p < other.m_p ? -1 : 0;
	}
	bool Equals(const IEnumerator& other) CRTP_OVERRIDE
	{
		return m_p == other.m_p;
	}
	TElement* operator()
	{
		return m_p;
	}
};

//template<typename TElement>
//class SequenceMemoryEnumerator : public IEnumerator<SequenceMemoryEnumerator<TElement>, TElement, RandomAccessIterator<SequenceMemoryEnumerator<TElement>, TElement>>
//{
//public:
//	SequenceMemoryEnumerator(WEAK_PTR(TElement) x) : _IEnumerator(x) {}
//	SequenceMemoryEnumerator(const SequenceMemoryEnumerator& mit) :_IEnumerator(mit){}
//	SequenceMemoryEnumerator operator= (const SequenceMemoryEnumerator& mit) { return SequenceMemoryEnumerator(mit); }
//	void Increase(int n)
//	{
//		m_p += n;
//	}
//	void Decrease(int n)
//	{
//		m_p -= n;
//	}
//};
#endif