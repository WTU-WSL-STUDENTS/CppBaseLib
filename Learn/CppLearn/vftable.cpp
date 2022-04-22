#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
//for memory leak check
#define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#include <stdlib.h> 
#include <crtdbg.h>
#define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif
#include <map>
#include <vector>
#include <algorithm>
using namespace std;

class A
{
public:
	int a;
public:
	A()
	{
		a = 0;
	}
	virtual int Func()
	{
		return this->a;
	}
	virtual void Func1()
	{}
}test;
class B : public A
{
public:
	int b = 0;
public:
	B() {}
	int Func()override
	{
		return this->b;
	}
	void Func1() override
	{}
}test1;
using FunctionPtr = void*;
using VirtualFunctionTable = FunctionPtr*;

void MyOverrideFunc(void* thisPtr)
{
	printf("MyOverrideFunc");
}
FunctionPtr GetVftableAddr(A* p)
{
	VirtualFunctionTable vftableAddr = *((VirtualFunctionTable*)p);
	printf("%p, %p, %p", p, &(p->a), vftableAddr);
	//vftableAddr[0] = MyOverrideFunc;/* 写入访问权限冲突。 代码区只有读取权限 */
	FunctionPtr AFuncAddr = vftableAddr[0];
	FunctionPtr AFuncAddr1 = vftableAddr[1];
	FunctionPtr AFuncAddr2 = vftableAddr[3];

	return AFuncAddr; 
	//return ((long*)((long*)p)[0])[0];
}

int  main()
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	A* a = new A;
	printf("%p, %p", GetVftableAddr(a), MyOverrideFunc);
	a->Func1();
	return 0;
}