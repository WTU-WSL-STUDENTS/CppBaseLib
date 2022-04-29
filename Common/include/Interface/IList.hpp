/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-26 10:46:07
 * @LastEditors: like
 * @LastEditTime: 2022-04-26 15:06:20
 */
#ifndef SYSTEM_ILIST_HPP
#define SYSTEM_ILIST_HPP
#include "ICollection.hpp"

namespace System::Interface
{
    template<typename TDerived, typename TElement, typename RALLPolicy>
    CRTP_ABSTRACT class IList;
};
/**
 * @brief ��̬���������
 * 
 * @tparam TDerived CRTP ������
 * @tparam TElement ��������
 * @tparam RALLPolicy allocator::destroy ����ָ��ָ��Ķ��ڴ治�����, IList ͨ�� RALLPolicy::destroy �滻 allocator::destroy
 */
template<typename TDerived, typename TElement, typename RALLPolicy>
CRTP_ABSTRACT class System::Interface::IList : public ICollection<TDerived, TElement>, public IDisposable<TDerived> , protected RALLPolicy
{
DECLARE_CRTP_INTERFACE(IList, TDerived, TElement, RALLPolicy)
private:
	std::allocator<TElement> allocPool;
	TElement* m_scan0;
	int m_nCapacity;
	IList(int capacity) : RALLPolicy(&allocPool),
        m_scan0(allocPool.allocate(capacity)), m_nCapacity(capacity){}
	void KeepMemoryEnough(int willAddElementCount = 1)
	{
		if (m_nCount + willAddElementCount < m_nCapacity) return;
		m_nCapacity = m_nCount + willAddElementCount;
		TElement* scan0 = nullptr;
		int nCapacity = 0;
		ERROR_ASSERT(0 < (nCapacity = m_nCapacity * 2), "out of range");
		ERROR_ASSERT(scan0 = allocPool.allocate(nCapacity), "out of memory");
		CANARY_ASSERT(memcpy(scan0, m_scan0, m_nCapacity * sizeof(TElement)));
		allocPool.deallocate(m_scan0, m_nCapacity);
		m_nCapacity = nCapacity;
		m_scan0 = scan0;
	}
public:
	int Capacity()
	{
		return m_nCapacity;
	}
	//using Iterator = SequenceMemoryEnumerator<TElement>;
	using Iterator = IEnumerator<TDerived, TElement, RandomAccessIterator<TDerived, TElement>>;
	TElement& operator[](int index) CRTP_OVERRIDE
	{
		return m_scan0[index];
	}
	Iterator  begin() CRTP_OVERRIDE
	{
		return m_scan0;
	}
	Iterator  end() CRTP_OVERRIDE
	{
		return m_scan0 + m_nCount;
	}
    /**
     * @brief ĩβ��������. O(1)
     * @param item  ���ƶ����캯�����ȵ����ƶ�����, û���ƶ��������ÿ�������
     * @return * void 
     */
	void Add(const TElement& item) CRTP_OVERRIDE
	{
		KeepMemoryEnough();
		allocPool.construct(m_scan0 + m_nCount, item);
		m_nCount++;
	}
	void Add(TElement&& item) CRTP_OVERRIDE
	{
		KeepMemoryEnough();
		allocPool.construct(m_scan0 + m_nCount, forward<TElement>(item));
		m_nCount++;
	}
	/**
	 * @brief �� List<T> ���Ƴ�����Ԫ��, List ������ڴ治���ͷ�, Ҳ�������
	 *
	 * @return * void
	 */
	void Clear() CRTP_OVERRIDE
	{
		for (int i = 0; i < m_nCount; i++)
		{
			RALLPolicy::destroy(m_scan0[i]);
		}
		m_nCount = 0;
	}
	bool Contains(const TElement& item) CRTP_OVERRIDE
	{
		return end() != std::find(m_scan0, end(), item);
	}
	void CopyTo(TElement* buffer/* buffer must be empty */, int bufferOffset) CRTP_OVERRIDE
	{
		uninitialized_copy_n(m_scan0, m_nCount, buffer + bufferOffset);
	}
    /**
     * @brief �ͷ� IList ӵ�е��ڴ���Դ
     * 
     */
	void Dispose() CRTP_OVERRIDE
	{
		VOIDRET_ASSERT(m_nCapacity);
		Clear();
		allocPool.deallocate(m_scan0, m_nCapacity);
		m_scan0 = nullptr;
		m_nCapacity = 0;
	}
	bool Remove(const TElement& item) CRTP_OVERRIDE
	{
		Iterator target = std::find(begin(), end(), item);
		Iterator targetNext = target + 1;
		VALRET_ASSERT(end() > target, false);
		RALLPolicy::destroy(*target);
		CANARY_ASSERT(memmove((TElement*)target, (TElement*)targetNext, (UInt64)(TElement*)end() - (UInt64)(TElement*)targetNext));
		m_nCount--;
		return true;
	}

	void AddRange(std::initializer_list<TElement> it)
	{
		KeepMemoryEnough((int)it.size());
		for (auto& item : it)
		{
			Add(std::move(const_cast<TElement&>(item)));
		}
	}
	int IndexOf(const TElement& item)
	{
		TElement* target;
		VALRET_ASSERT(end() != (target = std::find(m_scan0, end(), item)), -1);
		return (end() - m_scan0) / sizeof(TElement);
	}
	void Insert(size_t index, const TElement& item)
	{
		KeepMemoryEnough();
		if (m_nCount > index)
		{
			CANARY_ASSERT(memmove(m_scan0 + index, m_scan0 + index + 1, sizeof(TElement) * (m_nCount - index)));
			allocPool.construct(m_scan0 + index, item);
			return;
		}
		Add(item);
	}
	void Insert(size_t index, TElement&& item)
	{
		KeepMemoryEnough();
		if (m_nCount > index)
		{
			CANARY_ASSERT(memmove(m_scan0 + index, m_scan0 + index + 1, sizeof(TElement) * (m_nCount - index)));
			allocPool.construct(m_scan0 + index, std::forward<TElement>(item));
			return;
		}
		Add(std::forward<TElement>(item));
	}
	bool RemoveAt(size_t index)
	{
		ERROR_ASSERT(index < m_nCount, "out of range");
		auto* target = m_scan0 + index;
		RALLPolicy::destroy(*target);
		CANARY_ASSERT(memmove(target, target + 1, (UInt64)(m_scan0 + m_nCount) - (UInt64)(target + 1)));
		m_nCount--;
		return true;
	}
};


template<typename T>
class StackItemMemoryDisposePolicy
{
private:
	WEAK_PTR(std::allocator<T>) p;
public:
	StackItemMemoryDisposePolicy(WEAK_PTR(std::allocator<T>) alloc) : p(alloc) {}
	/**
	 * @brief ջ������ͷŲ���(������������)
	 *
	 * @param src
	 */
	inline void destroy(T& src)
	{
        p->destroy(&src);
	}
};

template<typename T>
class HeapItemMemoryDisposePolicy
{
public:
    HeapItemMemoryDisposePolicy(WEAK_PTR(std::allocator<T>) alloc/* IList ԭ���ڴ����ָ�� */){}
	/**
	 * @brief ���ڴ��ͷŲ���, delete ���������������ͷŶ��ڴ�
	 *
	 * @param src
	 */
	inline void destroy(typename std::enable_if<
		std::is_pointer<typename std::remove_reference<T>::type>::value, T>::type & src)
	{
		SAVE_DELETE_PTR(src);
	}
};

template<typename T>
class MallocHeapItemMemoryDisposePolicy
{
public:
	MallocHeapItemMemoryDisposePolicy(WEAK_PTR(std::allocator<T>) alloc) {}
	/**
	 * @brief malloc /calloc ����Ķ��ڴ��ͷŲ���, ������ڴ������ new(ptr)T ����, ��Ҫ�ֶ�������������, ��������ڴ�й©��Σ�� 
	 *
	 * @param src
	 */
	inline void destroy(typename std::enable_if<
		std::is_pointer<typename std::remove_reference<T>::type>::value, T>::type & src)
	{
		if (src)
		{
			free(src);
			src = NULL;
		}
	}
};
#endif