#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
//for memory leak check
#define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#include <stdlib.h> 
#include <crtdbg.h>
#define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif
#include <thread>

using namespace std;
#define COUNT 3

thread* StackLifetime(size_t val);
thread* StackLifetime1(size_t& val);

/* 栈对象释放在本例中也称为 "析构" */
int  main()
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	/* test 1 */
	char** list = new char* [COUNT];
	for (size_t i = 0; i < COUNT; i++)
	{
		char p[_MAX_PATH];
		sprintf(p, "%zd\n", i);
		list[i] = p;
		printf("%p, %s", p, p);
		/* char p[_MAX_PATH] 被析构 */
	}
	for (size_t i = 0; i < COUNT; i++)
	{
		printf("%p, %s", list[i], list[i]);
	}
	delete[] list;
	/* test 2 */
	char* list1[COUNT];
	for (size_t i = 0; i < COUNT; i++)
	{
		char* p = new char[_MAX_PATH];
		sprintf(p, "%zd\n", i);
		printf("%p, %s", p, p);
		list1[i] = p;
	}
	for (size_t i = 0; i < COUNT; i++)
	{
		printf("%p, %s", list1[i], list1[i]);
		delete[] list1[i];
	}
	/* test 3 */
	/**
	* 1. 理解函数参数的生命周期
	**/
	thread* t;
	t = StackLifetime(COUNT);
	t->join();
	delete t;
	size_t count = COUNT;
	printf("StackLifetime1 input val : %zd, %p\n", count, &count);
	t = StackLifetime1(count);
	t->join();
	delete t;
	/* size_t count 被析构		*/
	/* thread* t 被析构			*/
	/* char* list1[COUNT] 被析构*/
	/* char** list 被析构		*/
}
thread* StackLifetime(size_t val/* 拷贝构造 size_t */)
{
	return new thread([](void* args)->void
		{
			printf("StackLifetime : %zd, %p\n", *static_cast<size_t*>(args), args);/*  指针指向 size_t 拷贝构造的栈地址, 线程执行时 size_t 已被析构, 野指针 */
		}, &val);
	/* size_t 被析构 */
}
thread* StackLifetime1(size_t& val/* 构造 size_t& */)
{
	return new thread([](void* args)->void
		{
			printf("StackLifetime1 : %zd, %p\n", *static_cast<size_t*>(args), args);/* 指针指向 StackLifetime 输入参数的地址 */
		}, &val);
	/* size_t& 被析构 */
	/* TODO : 不确定是否存在 size_t& / size_t* 的构造和析构动作, 此处为类比推断 */
}