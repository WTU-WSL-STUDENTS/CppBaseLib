/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-07-23 10:21:28
 * @LastEditors: like
 * @LastEditTime: 2021-08-11 14:53:03
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

#define C_METHORD
#define CPP_METHORD
  
#ifdef CPP_METHORD
#define C_CPP_METHORD
#endif

namespace System
{
    typedef unsigned char byte;
    typedef short Int16;
    typedef int Int32;
    typedef unsigned short UInt16;
    typedef unsigned int UInt32;
    typedef long long Int64;
    typedef unsigned long long UInt64;
}


#ifndef _WIN32
typedef long Int64;
typedef unsigned long UInt64;
#else
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#endif

#endif