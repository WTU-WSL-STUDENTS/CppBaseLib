/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-05-07 10:26:44
 * @LastEditors: like
 * @LastEditTime: 2022-05-07 10:27:58
 */
#include <System.Collections.Generic.LinkedList.hpp>
#include <System.Collections.Generic.List.hpp>
#include <list>
#include <algorithm>
using namespace System::Collections::Generic;

int main()
{
    std::list<int> l1 = { 5, 4, 3, 2, 1, 0, -1, -2, -3 , -4 };
    l1.sort();
    //std::sort(l1.begin(), l1.end());
	printf("%p, %p\n", GET_ITERATOR_NATIVE_POINTER(l1.begin()), GET_ITERATOR_NATIVE_POINTER(++l1.begin()));
	printf("%p, %p\n", &l1.begin(), &(++l1.begin()));
    StackList<int> list = {1, 2, 3, 4, 5};
    LinkedList<int, StackItemMemoryDisposePolicy> n(list);
    for (const auto& item : n)
    {
        printf("%d\n", item);
    }
    return 0;
}