/*
 * @Description: 各种编译器的编译期警告接口
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-12 15:35:51
 * @LastEditors: like
 * @LastEditTime: 2021-09-12 16:14:48
 */
#ifndef COMPLIED_ERROR
#define COMPLIED_ERROR

/* 自定义警告禁用4字节最高4位 */
#define COMPLIER_ERROR_BEGIN    0xe0000000
#define COMPLIER_WARNING_BEGIN  0xf0000000

#if defined(COMPLIER_MSVC)
    #define __AUX_STR_EXP(_exp_)                    #_exp_
    #define ___AUX_STR(_exp_)                       __AUX_STR_EXP(_exp_)
    #define __LOCATION_STR                          __FILE__ "("  ___AUX_STR(__LINE__) ")"
    // #define warning(_code_, _message_)        message(__LOCATION_STR ": warning C" ___AUX_STR(COMPLIER_WARNING_BEGIN | _code_) ": " _message_)
    // #define error(_level_, _code_, _message_) message(__LOCATION_STR ":" _level_ " error C" ___AUX_STR(COMPLIER_ERROR_BEGIN | _code_) ": " _message_)
    // test
    // #pragma warning(10000, "this is a compile time warning message.")
    // #pragma error("", 10001, "this is a compile time error message.")
    // #pragma error("fatal", 10002, "this is a compile time fatal error message.")
#elif defined(COMPLIER_GCC)
    /* gcc提供 #error #warning 来提供报错 */
#endif


#endif