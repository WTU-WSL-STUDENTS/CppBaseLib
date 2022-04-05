/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-07-23 10:21:28
 * @LastEditors: like
 * @LastEditTime: 2022-04-03 09:12:28
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
#   define COMPLIER_MSVC
#elif defined(GCC)
#   define COMPLIER_GCC
#endif
/* 2. 参数定义 */
namespace System
{
    typedef void* Object;
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
#   include <CompliedError.h>
#   ifndef _CRT_SECURE_NO_WARNINGS
#       define _CRT_SECURE_NO_WARNINGS
#   endif
#   include <windows.h>
#elif defined(COMPLIER_GCC)
#else/* error */
#endif

#define ASSERT_ENABLE
#ifdef ASSERT_ENABLE
#   include <assert.h>
#   include <stdio.h>
#   define VOIDRET_ASSERT(condition)    \
        do                              \
        {                               \
            if(!(condition)){return;}   \
        } while (0)
#   define VALRET_ASSERT(condition, retVal) \
        do                                  \
        {                                   \
            if(!(condition)){return retVal;}\
        }while(0)
#   define ERROR_ASSERT(condition, errorMsg)                        \
        do                                                          \
        {                                                           \
            bool cond = condition;                                  \
            if(!(cond))                                             \
            {                                                       \
                printf("%s ( %d ) %s , error assert " #condition" \n", __FILE__, __LINE__, errorMsg);   \
                assert(cond);                                       \
            }                                                       \
        }while(0)
#   define CANARY_ASSERT(condition) ERROR_ASSERT(condition, "canary detect failed")
/* error code : https://docs.microsoft.com/en-us/windows/win32/debug/system-error-codes */
#   define WINAPI_ASSERT(condition, errorMsg)               \
        do                                                  \
        {                                                   \
            bool cond = condition;                          \
            if(!(cond))                                     \
            {                                               \
                printf("%s ( %d ) %s( code : %d ) , winapi assert "#condition"\n", __FILE__, __LINE__, errorMsg, GetLastError());   \
                assert(cond);                               \
            }                                               \
        }while(0)

#   define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#   include <stdlib.h> 
#   include <crtdbg.h>
#   define MEMORYLEAK_ASSERT _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#else
#   define VOIDRET_ASSERT(condition)
#   define VALRET_ASSERT(condition, retVal)
#endif
/**
 * @brief 定义枚举的值可按位组合
 * 
 */
#define DECLARE_ENUM_FLAG(n) (1 << n)
/**
 * @brief 禁用拷贝构造函数和赋值构造
 * 
 */
#define DISALLOW_COPY_AND_ASSIGN_CONSTRUCTED_FUNCTION(TypeName) TypeName(const TypeName&) = delete; TypeName& operator=(const TypeName&) = delete;
/**
 * @brief readonly
 * 
 */
#define readonly const
/**
 * @brief 标记当前指针非资源拥有者
 * 
 */
#define WEAK_PTR(type) type*
#define SAVE_DELETE_PTR(p) if((p)){delete (p); (p) = NULL;}
/**
 * @brief 处理器个数
 * 
 */
#define PROCESSER_COUNT 4
#endif