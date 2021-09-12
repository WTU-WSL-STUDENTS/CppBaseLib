/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-07-23 10:21:28
 * @LastEditors: like
 * @LastEditTime: 2021-09-12 16:32:44
 */
#ifndef COMPLIED_ENTRY_H
#define COMPLIED_ENTRY_H

#if defined __WINDOWS
    #if defined __MSVC_64
    #else if defined __MSVC_32
    #endif 
#elif defined __LINUX
    #define LINUX
#elif defined __FREE_BSD
    #define FREE_BSD
#endif

/* 1. 编译器版本 */
#if defined(MSVC_32) || defined(MSVC_64) 
#define COMPLIER_MSVC
#elif defined(GCC)
#define COMPLIER_GCC
#endif
/* 2. 参数定义 */
namespace System
{
    typedef unsigned char byte;
    typedef short Int16;
    typedef int Int32;
    typedef unsigned short UInt16;
    typedef unsigned int UInt32;
    #if defined(MSVC_32) 
    typedef long long Int64;
    typedef unsigned long long UInt64;
    #elif defined(MSVC_64) 
    typedef long Int64;
    typedef unsigned long UInt64;
    #elif !defined(__WINDOWS) /* error */
    #endif
}

#if defined(COMPLIER_MSVC)
#include <CompliedError.h>
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>
#elif defined(COMPLIER_GCC)
#else/* error */
#endif

#endif