#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
//for memory leak check
#define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#include <stdlib.h> 
#include <crtdbg.h>
#define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif
#include <iostream>
#define RWL_READ_BLOCK(code, code1) printf("begin\n"); code code1 printf("end\n");

typedef void** AsyncState;
template<typename _Last>
static void _CreateAsyncState(AsyncState& p, const _Last& t)
{
    *p = t;
}
template<typename _First, typename ..._Rest>
static void _CreateAsyncState(AsyncState& p, const _First& t, const _Rest&... u)
{
    *p++ = t;
    _CreateAsyncState(p, u...);
}
template<typename _First, typename ..._Rest>
static AsyncState CreateAsyncState(_First t, _Rest... u)
{
	AsyncState as = new void* [sizeof...(u) + 1];
	AsyncState p = as;
	*p++ = t;
	_CreateAsyncState(p, u...);
	return as;
}

template<typename T, typename... ARG>
AsyncState Test(T x, ARG... arg) {
	AsyncState as = new void* [sizeof...(arg) + 1];
	AsyncState p = as;
	*p++ = x;
	(void)std::initializer_list<T>{
		([&arg, &p] {*p++ = arg; }(), x)...
	};
	return as;
}
void** FuncWithoutInline()
{
	for (size_t i = 0; i < 1000; i++)
	{
		i++;
	}
	return new void* [10];
}
void Test(int a, int b) 
{
	printf("gg");
}
int main()
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	RWL_READ_BLOCK(Test(1, 2);, Test(1, 2););
	const char* str = "Hello world";
	char* p = const_cast<char*>(str);
	//p[0] = 'h';
	printf("%s\n", p);

	int* a = new int;
	int** b = new int*[10];
	printf("%p, %p\n", a, b);
	AsyncState args = CreateAsyncState(a, b);
	printf("%p, %p\n", args[0], args[1]);
	AsyncState args1 = Test(a, b);
	printf("%p, %p\n", args1[0], args1[1]);
	delete args1;
	delete args;
	int* list = new int[1];
	delete[] list;
	delete[] b;
	delete a;
	void** params = FuncWithoutInline();
	delete[] params;
	//delete params;/* 此处泄露 */
	return 0;
}
