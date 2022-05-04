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
	TDerived& operator=(TDerived& other) CRTP_VIRTUAL
	{
		return InputIterator<TDerived, TElement>::operator=(other);
	}

	TDerived& operator++() CRTP_VIRTUAL
	{
		CRTP_DERIVED.Increase(1);
		return CRTP_DERIVED;
	}
	TDerived operator++(int) CRTP_VIRTUAL
	{
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

template<typename TDerived, typename TElement, typename Category = System::Interface::RandomAccessIterator<TDerived, TElement>>
CRTP_ABSTRACT class System::Interface::IEnumerator :
	public Category,
	public std::iterator<Category, TElement>
{
    DECLARE_CRTP_INTERFACE(IEnumerator, TDerived, TElement, Category)
private:
	WEAK_PTR(TElement)  m_p;
public:
	explicit IEnumerator(WEAK_PTR(TElement) x) : m_p(x) {}
    IEnumerator(const IEnumerator& mit) : m_p(mit.m_p) {}

	TElement& operator*() CRTP_OVERRIDE
	{
		return *m_p;
	}
	const TElement& operator*()const CRTP_OVERRIDE
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
	int CompareTo(const TDerived& other) const CRTP_OVERRIDE
	{
		return m_p > other.m_p ? 1 : m_p < other.m_p ? -1 : 0;
	}
	bool Equals(const TDerived& other) const CRTP_OVERRIDE
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
	/* if(it) */
	explicit operator bool() const
	{
		return nullptr != m_p;
	}
	std::ptrdiff_t Distance(const TDerived& other) const
	{
		return m_p - other.m_p;
	}
	TElement& AccessNativeReference(std::ptrdiff_t index) const
	{
		return m_p[index];
	}
	/* std::distance */
	/*std::ptrdiff_t operator-(const TDerived& other) const
	{
		return m_p - other.m_p;
	}
	TDerived operator-(const std::ptrdiff_t& n) const
	{
		return TDerived(m_p - n);
	}*/
};

template<typename TElement>
class System::Interface::SequenceMemoryEnumerator :
	public IEnumerator<SequenceMemoryEnumerator<TElement>, TElement, RandomAccessIterator<SequenceMemoryEnumerator<TElement>, TElement>>
{
public:
	explicit SequenceMemoryEnumerator(WEAK_PTR(TElement) x) : _IEnumerator(x) {}
	SequenceMemoryEnumerator(const SequenceMemoryEnumerator& mit) :_IEnumerator(mit){}
	SequenceMemoryEnumerator& operator= (const SequenceMemoryEnumerator& mit) CRTP_OVERRIDE 
	{
		m_p = mit.m_p; 
		return *this; 
	}
	/*SequenceMemoryEnumerator& operator=(const TElement* x)
	{
		m_p = x;
		return *this;
	}*/
};
#endif