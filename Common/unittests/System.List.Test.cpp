/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 19:21:57
 * @LastEditors: like
 * @LastEditTime: 2022-02-11 09:41:01
 */
#include <System.List.hpp>
#include <System.DateTime.hpp>
#include <stdio.h>
using namespace System;

int count = 10000;

int StructerTest()
{
    List<int, StructerItemMemoryDisposePolicy> list(100);
    printf("%zd, %zd\n", list.Count(), list.Capacity());
    /* 添加数据 */
    for(int i = 0; i < count; i++)
    {
        list.Add(i);
    }
    printf("%zd, %zd\n", list.Count(), list.Capacity());
    for(int i = 0; i < count; i++)
	{
		ERROR_ASSERT(i == list[i], "Add Data Error");
    }
    printf("Check List<T>::Add Sucess...\n");
    /* IndexOf */
    for(int i = 0; i < count; i++)
	{
		ERROR_ASSERT(i == list.IndexOf(i), "IndexOf Error");
    }
    printf("Check List<T>::IndexOf Sucess...\n");
    /* Remove */
    list.RemoveAll([](const int& item)->bool
    {
        return item % 2;
	});
	printf("%zd (should be %d), %zd\n", list.Count(), count/2, list.Capacity());
	list.RemoveAll([](const int& item)->bool
	{
		return item % 2 + 1;
        return true;
	});
    printf("%zd ( should be 0 ), %zd\n", list.Count(), list.Capacity());
    /* Insert */
    for(int i = 0; i < 10; i++)
    {
        list.Insert(0, i);
    }
    //list.Reverse();
    /* Display all data */
    list.ForEach([](const int& item)->void
    {
        printf("%d\n", item);
    });
    list.TrimExcess();
    printf("%zd( should be 10 ), %zd( should be 10 )\n", list.Count(), list.Capacity());
    return 0;
}
int PtrTest()
{
    List<int*, NewItemMemoryDisposePolicy> list(10);
    printf("%zd, %zd\n", list.Count(), list.Capacity());
    /* 添加数据 */
    for(int i = 0; i < count; i++)
    {
        list.Add(new int(i));
    }
    printf("%zd, %zd\n", list.Count(), list.Capacity());
    for(int i = 0; i < count; i++)
	{
		ERROR_ASSERT(i == *list[i], "Add Data Error");
    }
    printf("Check List<T>::Add Sucess...\n");
    /* IndexOf */
    for(int i = 0; i < count; i++)
    {
		ERROR_ASSERT(i == list.IndexOf(list[i]), "IndexOf Error");
    }
    printf("Check List<T>::IndexOf Sucess...\n");
    /* Remove */
    list.RemoveAll([](int* const (&item))->bool
    {
        return true;//*item % 2;
    });
    printf("%zd( should be 0 ), %zd\n", list.Count(), list.Capacity());
    /* Insert */
    for(int i = 0; i < 10; i++)
    {
        list.Insert(0, new int(i));
    }
    //list.Reverse(0, list.Count());
    /* Display all data */
    list.ForEach([](int* const (&item))->void
    {
        printf("%d\n", *item);
    });
    list.TrimExcess();
    printf("%zd(should be 10), %zd(should be 10)\n", list.Count(), list.Capacity());
    return 0;
}

typedef List<const char*, StructerItemMemoryDisposePolicy> StaticStringList;
typedef List<int, StructerItemMemoryDisposePolicy> IntList;

int main()
{
#ifdef _DEBUG
	MEMORYLEAK_ASSERT;
#endif
    //PtrTest();
    StructerTest();

    StaticStringList strList;/* 地址指向常量区, 该指针不需要手动释放 */
    strList.Add("A");
    strList.Add("C");
    strList.Add("B");
    printf("%p, %p, %p\n", "A", "C", "B");
    StaticStringList::Enumerator* it = (StaticStringList::Enumerator*)strList.GetEnumerator();
    while(it->MoveNext())/* MoveNext /Current 会被内联, 详细见 Common\resources\优化虚函数表 */ 
    {
        printf("%p, %s\n", *(it->Current()), *(it->Current()));
    }
    delete it;

    IntList intList({555, 666, 777, 888, 999});
    printf("IntList %zd, %zd\n", intList.Capacity(), intList.Count());
    intList.ForEach([](const int &item)->void
    {
        printf("%d\n",item);
    });
    printf("Sucess , press any key to exit...\n");
    return 0;
}