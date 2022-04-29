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
#include <Interface/IList.hpp>

namespace System::Collections::Generic
{
	template<typename T>
	struct LinkedListNode;
	template<class TDerived, template<class> class RALLPolicy>
	class LinkedList;
};

template<typename T>
struct System::Collections::Generic::LinkedListNode
{
	class LinkedListNodeIterator : public std::iterator<std::bidirectional_iterator_tag, T>
	{
		WEAK_PTR(T) p;
	public:
		LinkedListNodeIterator(WEAK_PTR(T) x) : p(x) {}
		LinkedListNodeIterator(const LinkedListNodeIterator& mit) : p(mit.p) {}
		LinkedListNodeIterator& operator++() { ++p; return *this; }
		LinkedListNodeIterator operator++(int) { LinkedListNodeIterator tmp(*this); operator++(); return tmp; }
		bool operator==(const LinkedListNodeIterator& rhs) const { return p == rhs.p; }
		bool operator!=(const LinkedListNodeIterator& rhs) const { return p != rhs.p; }
		T& operator*() { return *p; }
	};
	LinkedListNode<T>* Previous;
	LinkedListNode<T>* Next;
	T Value;
};

template<class TDerived, template<class> class RALLPolicy>
class System::Collections::Generic::LinkedList : public ICollection<LinkedList<TDerived, RALLPolicy>, TDerived>
{
	
};
#endif