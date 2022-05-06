/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-28 11:41:56
 * @LastEditors: like
 * @LastEditTime: 2022-04-28 11:41:56
 */
#ifndef SYSTEM_COLLECTIONS_GENERIC_LINKED_LIST_HPP
#define SYSTEM_COLLECTIONS_GENERIC_LINKED_LIST_HPP
#include <Interface/ICollection.hpp>
using namespace System::Interface;
namespace System::Collections::Generic
{
	template<class T>
	class LinkedListNode;
	template<class T>
	class LinkedListNodeIterator;
	template<class T, template<class> class RALLPolicy>
	class LinkedList;
};
template<class T>
class System::Collections::Generic::LinkedListNode
{
	friend LinkedListNodeIterator;
	friend LinkedList;
protected:
	LinkedListNode<T>* previous{ nullptr };
	LinkedListNode<T>* next{ nullptr };
	T value;
public:
	LinkedListNode<T>& Next()
	{
		return *next;
	}
	LinkedListNode<T>& Previous()
	{
		return *previous;
	}
	T& ValueRef()
	{
		return value;
	}
};

template<class T>
class System::Collections::Generic::LinkedListNodeIterator : public IEnumerator<LinkedListNodeIterator<T>, T, BidirectionalIterator<LinkedListNodeIterator<T>, T>>
{
private:
	WEAK_PTR(LinkedListNode<T>) m_p;
public:
	explicit LinkedListNodeIterator(WEAK_PTR(LinkedListNode<T>) p) : m_p(p) {}
	LinkedListNodeIterator(const LinkedListNodeIterator<T>& mit) : m_p(mit.m_p) {}
	LinkedListNodeIterator& operator= (const LinkedListNodeIterator<T>& mit) CRTP_OVERRIDE
	{
		m_p = mit.m_p;
		return *this;
	}
	T& operator*() CRTP_OVERRIDE
	{
		return m_p->value;
	}
	T& operator*()const CRTP_OVERRIDE
	{
		return m_p->value;
	}
	T* operator->() CRTP_OVERRIDE
	{
		return &(m_p->value);
	}
	const T* operator->()const CRTP_OVERRIDE
	{
		return &(m_p->value);
	}
	int CompareTo(const LinkedListNodeIterator<T>& other) const CRTP_OVERRIDE
	{
		if (Equals(other))
		{
			return 0;
		}
		Node* current = this;
		while (nullptr != current->previous)
		{
			if (current->previous == &other)
			{
				return 1;
			}
		}
		return -1; /* 默认 this 和 other 处在一条链路上 */
	}
	bool Equals(const LinkedListNodeIterator<T>& other) const CRTP_OVERRIDE
	{
		return m_p->previous == other.m_p->previous && m_p->next == other.m_p->next;
	}
	void Increase(std::ptrdiff_t n) CRTP_OVERRIDE
	{
		while (0 < n-- && m_p)
		{
			m_p = m_p->next;
		}
	}
	void Decrease(std::ptrdiff_t n) CRTP_OVERRIDE
	{
		while (0 < n-- && m_p)
		{
			m_p = m_p->previous;
		}
	}

};
template<class T, template<class> class RALLPolicy>
class System::Collections::Generic::LinkedList : public Interface::ICollection<LinkedList<T, RALLPolicy>, T>
{
	using Node = LinkedListNode<T>;
private:
	Node* head;
public:
	LinkedList() : head(new Node){}
	template<class IEnumrable>
	LinkedList(const IEnumrable& ie) : LinkedList()
	{
		Node* current = head;
		for (const auto& val : ie)
		{
			current->value = val;
			current->next = new LinkedListNode<T>;
			current->next->previous = current;
			current = current->next;
			m_nCount++;
		}
	}
	LinkedListNodeIterator<T> begin() const
	{
		return LinkedListNodeIterator<T>(head);
	}

	LinkedListNodeIterator<T> end() const
	{
		LinkedListNode<T>* current = head;
		for(int i = 0; i < m_nCount; i++)
		{
			current = current->next;
		}
		return LinkedListNodeIterator<T>(current);
	}
	//n.begin();

};
#endif