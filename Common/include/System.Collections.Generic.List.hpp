/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-27 11:25:40
 * @LastEditors: like
 * @LastEditTime: 2022-04-27 11:25:41
 */
#ifndef SYSTEM_COLLECTIONS_GENERIC_LIST_HPP
#define SYSTEM_COLLECTIONS_GENERIC_LIST_HPP
#include <Interface/IList.hpp>
namespace System::Collections::Generic
{
	template<class T, template<class> class RALLPolicy>
	class List;
};

template<class T, template<class> class RALLPolicy>
class System::Collections::Generic::List final : public System::Interface::IList<List<T, RALLPolicy>, T, RALLPolicy<T>>
{
public:
	explicit List(int capacity = 8) : _IList(capacity) {};
	List(std::initializer_list<T> list) : _IList(list.size()) 
	{
		for (const auto& item : list)
		{
			Add(item);
		}
	};
	~List() { Dispose(); }
	template<typename FPredicate>
	bool RemoveIf(FPredicate match)
	{
		static_assert(std::is_same<function_traits<decltype(match)>::return_type, bool>::value);
		Iterator target = std::find_if(begin(), end(), match);
		Iterator targetNext = target + 1;
		VALRET_ASSERT(end() > target, false);
		RALLPolicy<T>::destroy(*target);
		CANARY_ASSERT(memmove(&(*target), &(*targetNext), (UInt64)GET_ITERATOR_NATIVE_POINTER(end()) - (UInt64)GET_ITERATOR_NATIVE_POINTER(targetNext)));
		m_nCount--;
		return true;
	}
	template<typename FPredicate>
	bool Exists(FPredicate match)
	{
		return end() > std::find(begin(), end(), match);
	}
	void Sort()
	{
		std::sort(begin(), end());
	}
	void Reverse()
	{
		std::reverse(begin(), end());
	}
	template<typename FPredicate>
	auto Find(FPredicate match)
	{
		return std::find(begin(), end(), match);
	}
	template<typename FPredicate>
	int FindIndex(FPredicate match)
	{
		auto target = Find(match);
		end() > target ? target.Distance(begin()) : -1;
	}
	template<typename F>
	void ForEach(F action)
	{
		std::for_each(begin(), end(), action);
	}
};

template <typename T>
using HeapList = System::Collections::Generic::List<T, HeapItemMemoryDisposePolicy>;
template <typename T>
using MallocList = System::Collections::Generic::List<T, MallocHeapItemMemoryDisposePolicy>;
template <typename T>
using StackList = System::Collections::Generic::List<T, StackItemMemoryDisposePolicy>;
#endif