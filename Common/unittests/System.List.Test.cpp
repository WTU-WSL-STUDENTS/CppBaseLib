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

int count = 100000;

int StructerTest()
{
    List<int, StructerItemMemoryDisposePolicy> list(100);
    printf("%d, %d\n", list.Count(), list.Capacity());
    /* 添加数据 */
    for(int i = 0; i < count; i++)
    {
        list.Add(i);
    }
    printf("%d, %d\n", list.Count(), list.Capacity());
    for(int i = 0; i < count; i++)
    {
        if(i != list[i])
        {
            printf("Add Data Error list[%d] = %d\n", i, list[i]);
            return 1;
        }
    }
    printf("Check List<T>::Add Sucess...\n");
    /* IndexOf */
    for(int i = 0; i < count; i++)
    {
        if(i != list.IndexOf(i))
        {
            printf("IndexOf Error list[%d] = %d\n", i, list[i]);
            return 1;
        }
    }
    printf("Check List<T>::IndexOf Sucess...\n");
    /* Remove */
    list.RemoveAll([](const int& item)->bool
    {
        return item % 2;
    });
    printf("%d, %d\n", list.Count(), list.Capacity());
    for(int i = 0; i < count / 2; i++)
    {
        if(!list.Remove(i * 2))
        {
            printf("Remove Error list[%d] = %d\n", i, list[i]);
            return 1;
        }
    }
    printf("%d, %d\n", list.Count(), list.Capacity());
    /* Insert */
    for(int i = 0; i < 10; i++)
    {
        list.Insert(0, i);
    }
    list.Reverse();
    /* Display all data */
    list.ForEach([](const int& item)->void
    {
        printf("%d\n", item);
    });
    list.TrimExcess();
    printf("%d, %d\n", list.Count(), list.Capacity());
    return 0;
}
int PtrTest()
{
    List<int*, NewItemMemoryDisposePolicy> list(10);
    printf("%d, %d\n", list.Count(), list.Capacity());
    /* 添加数据 */
    for(int i = 0; i < count; i++)
    {
        list.Add(new int(i));
    }
    printf("%d, %d\n", list.Count(), list.Capacity());
    for(int i = 0; i < count; i++)
    {
        if(i != *list[i])
        {
            printf("Add Data Error list[%d] = %d\n", i, *list[i]);
            return 1;
        }
    }
    printf("Check List<T>::Add Sucess...\n");
    /* IndexOf */
    for(int i = 0; i < count; i++)
    {
        if(i != list.IndexOf(list[i]))
        {
            printf("IndexOf Error list[%d] = %d\n", i, *list[i]);
            return 1;
        }
    }
    printf("Check List<T>::IndexOf Sucess...\n");
    /* Remove */
    list.RemoveAll([](int* const (&item))->bool
    {
        return true;//*item % 2;
    });
    printf("%d, %d\n", list.Count(), list.Capacity());
    /* Insert */
    for(int i = 0; i < 10; i++)
    {
        list.Insert(0, new int(i));
    }
    list.Reverse(0, list.Count());
    /* Display all data */
    list.ForEach([](int* const (&item))->void
    {
        printf("%d\n", *item);
    });
    list.TrimExcess();
    printf("%d, %d\n", list.Count(), list.Capacity());
    return 0;
}

typedef List<const char*, StructerItemMemoryDisposePolicy> StaticStringList;
typedef List<int, StructerItemMemoryDisposePolicy> IntList;

int main()
{
    PtrTest();
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
    printf("IntList %d, %d\n", intList.Capacity(), intList.Count());
    intList.ForEach([](const int &item)->void
    {
        printf("%d\n",item);
    });
    
    printf("Sucess , press any key to exit...\n");
    getchar();
    return 0;
}