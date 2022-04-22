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

class A
{
public:
	char* resource{ nullptr };
	A() = default;
	A(A&& a) noexcept : resource(a.resource)
	{
		a.resource = nullptr;
	}
	~A()
	{
		if (resource)
		{
			printf("dispose resource %p, %p\n", this, resource);
			delete[] resource;
		}
	}
};
A MoveConstructTest()/* 移动构造函数测试 */
{
	A a;
	a.resource = new char[3];
	printf("create resource %p, %p\n", &a, a.resource);
	return a;
}

using std::move;

/* 可以接收 lvalue/rvalue , 但是参数类型会被限定为 const ! */
void IsRightValue(const int&)
{
	printf("const int&  lvalue or rvalue\n");
}
/* 为了消除 const , c++11 以前用 c++ 的 const_cast 强制取消指针的 const
 *
 * c++11 新增右值引用 &&
 * 将 void IsRightValue(const int&) 分别实现为
 * void IsRightValue(int&) / void IsRightValue(int&&)
 * 通过函数的重载, 消除了 const 又可以兼容 左值/右值的传入!
 *
 */
void IsRightValue(int&/* lvalue */)
{
	printf("int& lvalue\n");
}
void IsRightValue(int&&/* rvalue */)
{
	printf("int&& rvalue\n");
}
void RightValueTest(int&& i/* 右值 */)
{
	printf("RightValueTest---------------\n");
	IsRightValue(5);
	IsRightValue(i);/* i 在函数体内部退化成了左值 */
	IsRightValue(std::move(i));
}
/*
 * 同样的接口要实现两边，显然非常丑陋。我们可以考虑用 template 来替我们写重复代码
 * 前置知识 ： 模板的右值引用是"universal reference" (通过引用折叠实现)
 */
template<typename T>
void TRightValueTest(T&& i)
{
	IsRightValue(i);
}
void RightValueTest1(int&& i)
{
	printf("RightValueTest1---------------\n");
	TRightValueTest(5);
	TRightValueTest(i);
	TRightValueTest(std::move(i));
	/* 结果与预期不符!
	 *  5,i,std::move(i) 都被判定为是左值
	 * 原因：
	 *    通过 RightValueTest 中的 IsRightValue(i); 我们可以知道, 传入右值在函数体内会退化成左值。
	 * 这种现象成为了我们用 template 来简化重复代码的最大障碍。
	 * 
	 * 为了解决这个问题, 我们需要一个在函数体内能够识别 T 的 左右值的功能。
	 * 完美转发 (std::forward<T>)
	 *		std::forward 能够依据 T 是左值还是右值, 返回一个 左值/右值的对象。
	 *
	 * 利用完美转发, 将 TRightValueTest 升级成 PerfectForwardingRightValueTest
	 */
}
template<typename T>
void PerfectForwardingRightValueTest(T&& i)
{
	IsRightValue(std::forward<T>(i));
}
void RightValueTest2(int&& i)
{
	printf("RightValueTest2---------------\n");
	PerfectForwardingRightValueTest(5);
	PerfectForwardingRightValueTest(i);
	PerfectForwardingRightValueTest(std::move(i));
}
int main() 
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	MoveConstructTest();
	RightValueTest(123);
	RightValueTest1(123);
	RightValueTest2(123);
	return 0;
}