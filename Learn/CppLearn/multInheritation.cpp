#define _CRT_SECURE_NO_WARNINGS
#ifdef _DEBUG
//for memory leak check
#define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#include <stdlib.h> 
#include <crtdbg.h>
#define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif
#include <stdio.h>

int i = 0;
class Top
{
public:
	int a = i++;
	virtual void Func()
	{
		printf("Top\n");
	}
};
class Left : public Top
{
public:
	int l = i++;
	void Func() override
	{
		printf("Left\n");
	}

};
class Right : public Top
{
public:
	int r = i++;
	void Func() override
	{
		printf("Right\n");
	}
};
class Interface
{
public:
	virtual void Func() = 0;
};

/*
 *
 * 
 *	 				 Top
 *					/   \
 * Interface 	Left   Right 
 *		|			\	/
 *     	|--------- Bottom
 * 菱形继承只是多继承的一种特例而已！
 * 
 *  Bottom 内存分布( x64 )：
 *  |-Interface
 *	|	vfptr		8 byte 
 *	|
 *	|-Left
 *	|	Top
 *	|	|	vfptr	8 byte
 *	|	|	a		8 byte ( x64 8 字节对齐 )
 *	|	|------
 *	|	l			8 byte ( x64 8 字节对齐 )
 *	|-Right
 *	|	Top
 *	|	|	vfptr	8 byte
 *	|	|	a		8 byte ( x64 8 字节对齐 )
 *	|	|------
 *	|	r			8 byte ( x64 8 字节对齐 )
 *	|-------
 *	|- b			8 byte ( x64 8 字节对齐 )
 *
 */
class Bottom : public Interface, public Left, public Right
{
public:
	int b = i++;
	void Func() override
	{
		printf("Bottom\n");
	}
};

int  main()
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	printf("%zd, %zd, %zd, %zd\n", sizeof(Top), sizeof(Left), sizeof(Right), sizeof(Bottom));
	Bottom* b = new Bottom;
	static_cast<Right*>(b)->Func();
	static_cast<Left*>(b)->Func();
	/* 访问菱形根节点( Top )的对象， 需要显示指定对象定义域，因为Left 和 Right 在 Bottom 中偏移地址不同， 此例中分别为 ： [b] + 8 和 [b] + ( 8 + 24 ) */
	static_cast<Top*>(static_cast<Left*>(b))->Func();	
	static_cast<Top*>(static_cast<Right*>(b))->Func();
	static_cast<Interface*>(b)->Func();
	delete b;
	return 0;
}