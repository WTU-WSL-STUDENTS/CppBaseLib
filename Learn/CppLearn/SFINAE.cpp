#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
//for memory leak check
#define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#include <stdlib.h> 
#include <crtdbg.h>
#define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif
#include <stdio.h>
#include <type_traits>

/* 
 * " 泛型约束 "
 * SFINAE : Substitution Failure Is Not An Error 
 * https://en.cppreference.com/w/cpp/language/sfinae
 * https://zhuanlan.zhihu.com/p/21314708
 */

class A
{
public:
	using TypeA = int;
};
class B
{
public:
	using TypeB = int;
};

template <class T>
void foo(typename T::TypeA val)
{
	printf("Foo0 , %d\n", val);
}
template <typename T>
void foo(typename T::TypeB val)
{
	printf("Foo1 , %d\n", val);
}
template <typename T>
void foo(T val)
{
	printf("Foo2 , %d\n", val);
}

void callFoo() {
	foo<A>(1);	// Foo0: Succeed, Foo1: Failed,  Foo2: Failed
	foo<B>(2);	// Foo0: Failed,  Foo1: Succeed, Foo2: Failed
	foo<int>(3);// Foo0: Failed,  Foo1: Failed,  Foo2: Succeed
}


struct ICounter {
	virtual void increase() = 0;
	virtual ~ICounter() = default;
};

struct Counter : public ICounter {
	void increase() override 
	{
	}
};

template <typename T>
void inc_counter(T& intTypeCounter, std::decay_t<decltype(++intTypeCounter)>* = nullptr) 
{
	++intTypeCounter;
	printf("1. std::decay_t<decltype(++intTypeCounter)>\n");
}
//template <typename T>
//void inc_counter(T& counterObj, std::decay_t<decltype(counterObj.increase())>* = nullptr)
//{
//	counterObj.increase();
//	printf("2. std::decay_t<decltype(counterObj.increase())>\n");
//}

template <typename T>
void inc_counter(T& counterObj, typename std::enable_if<std::is_base_of<ICounter, T>::value>::type* = nullptr)
{
	/*
		00007FF7A2DC28C0  mov         qword ptr [rsp+10h],rdx	// nullptr
		00007FF7A2DC28C5  mov         qword ptr [rsp+8],rcx		// T*
	*/
	counterObj.increase();
	printf("3. enable_if work\n");
}
void doSomething() 
{
	Counter cntObj;
	unsigned int cntUI32;

	inc_counter(cntObj);/* 同时满足 2 和 3 重定义( 函数签名相同 ： 返回值 & 名称 & 传参 均相同) */
	inc_counter(cntUI32);
}


int  main()
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	callFoo();
	doSomething();
	return 0;
}