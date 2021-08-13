/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 19:21:57
 * @LastEditors: like
 * @LastEditTime: 2021-08-13 17:31:39
 */
#include <System.LinkList.hpp>
#include <System.DateTime.hpp>
#include <stdio.h>
using namespace System;

int main()
{
    DWORD old;
    DWORD current;
    old = GetTickCount();
    LinkList<int> doubleLink;
    int count = 100000;
    /* 添加数据 */
    for(int i = 0; i < count; i++)
    {
        doubleLink.Add(i);
    }
    
    /* 1.在最坏的情况下删除全部数据 */
    while(--count > -1)
    {
        if(!doubleLink.Remove(0))
        {
            printf("Remove Error At%d", count);
            return 1;
        }    
    }
    current = GetTickCount();
    printf("%d\n%d\n%d",doubleLink.Count(), old, current);
    IEnumerator<int>* it = doubleLink.GetEnumerator();
    // it->Dispose();
    // delete it;
    return 0;
}