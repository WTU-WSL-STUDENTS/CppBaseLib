/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 19:21:57
 * @LastEditors: like
 * @LastEditTime: 2021-08-18 19:48:24
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
    LinkList<double> doubleLink;
    system( "PAUSE "); 
    int count = 1000000;
    /* 添加数据 */
    for(int i = 0; i < count; i++)
    {
        doubleLink.Add(i);
    }
    system( "PAUSE "); 
    // for(int i = 0, temp = 0; i < count; i++)
    // {
    //     if(doubleLink[i] != i)
    //     {
    //         printf("%d-%d\n", i, doubleLink[i]);
    //     }
    //     temp = doubleLink[i];
    // }
    
    /* 1.在最坏的情况下删除全部数据 */
    while(count-- > 0)
    {
        if(!doubleLink.Remove(count))
        {
            printf("Remove Error At%d", count);
            return 1;
        }    
    }
    system( "PAUSE "); 
    current = GetTickCount();
    printf("当前元素个数%d\nTT:%d\n",doubleLink.Count(), current - old);
    return 0;
}