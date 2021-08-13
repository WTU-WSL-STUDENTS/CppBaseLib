/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 19:21:57
 * @LastEditors: like
 * @LastEditTime: 2021-08-13 17:30:25
 */
#include <System.List.hpp>
#include <System.DateTime.hpp>
#include <stdio.h>
using namespace System;

int main()
{
    DWORD old;
    DWORD current;
    old = GetTickCount();
    List<int> list;
    int count = 100000;
    /* 添加数据 */
    for(int i = 0; i < count; i++)
    {
        list.Add(i);
    }
    /* 1.在最坏的情况下删除全部数据 */
    for(int i = 0; i < count; i++)
    {
        if(!list.RemoveAt(0))
        {
            printf("Remove Error At%d", i);
            return 1;
        }
    }
    current = GetTickCount();
    printf("%d\n%d\n%d",list.Count(), old, current);
    IEnumerator<int>* it = list.GetEnumerator();
    it->Dispose();
    delete it;
    return 0;
}