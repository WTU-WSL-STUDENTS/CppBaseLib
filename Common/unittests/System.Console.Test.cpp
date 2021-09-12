/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-14 19:41:20
 * @LastEditors: like
 * @LastEditTime: 2021-09-12 15:14:27
 */
#include <System.Console.hpp>
#define DISABLE_PACK_ALIGN
#include <System.Tuple.hpp>

using namespace System;

int main()
{
    Console::WriteLine/*<int, const char*, double>*/( 1, "asd", 3.14);
    Console::WriteLineWidthSplite/*<int, const char*, double>*/(",", 1, "asd", 3.14);
    Tuple<int, int, int> args(1, 2, 3);
    printf("sizeof Tuple<int, int, int>:%d, int = %d * 3\n", sizeof(args), sizeof(int));
    Tuple<int, double, float> args1(1, 2, 3);
    printf("sizeof Tuple<int, double, float>:%d, (´æÔÚ×Ö½Ú¶ÔÆë)int = %d, double = %d, float = %d\n", sizeof(args1), sizeof(int), sizeof(double), sizeof(float));
    return 0;
}
