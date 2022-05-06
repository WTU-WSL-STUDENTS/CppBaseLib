#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
//for memory leak check
#define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#include <stdlib.h> 
#include <crtdbg.h>
#define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif
#include <iostream>
#include <list>

using namespace std;
int main()
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	list<int> list;
	return 0;
}
