/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-14 12:41:02
 * @LastEditors: like
 * @LastEditTime: 2021-08-14 20:10:51
 */
#ifndef SYSTEM_CONSOLE_H
#define SYSTEM_CONSOLE_H

#include <iostream>

namespace System
{
    class Console;
}
class System::Console
{
private:
    template<typename _Rest>
    static void WriteLine(_Rest t){std::cout << t << std::endl;}

    template<typename _Rest>
    static void WriteLineWidthSplite(const char* split, _Rest t){std::cout << t << std::endl;}
public:
    /**
     * @description: 输入的字符串依次输出
     * @param {N个可变长度参数，通过递归依次输出}
     * @return {*}
     * @author: like
     */    
    template<typename _First, typename ..._Rest>
    static void WriteLine(_First t, _Rest... u)
    {
        std::cout << t;
        WriteLine(u...);
    }
    /**
     * @description: 输入的字符串依次输出
     * @param {分隔符}
     * @param {N个可变长度参数，通过递归依次输出}
     * @return {*}
     * @author: like
     */  
    template<typename _First, typename ..._Rest>
    static void WriteLineWidthSplite(const char* split, _First t, _Rest... u)
    {
        std::cout << t << split;
        WriteLineWidthSplite(split, u...);
    }
};
#endif