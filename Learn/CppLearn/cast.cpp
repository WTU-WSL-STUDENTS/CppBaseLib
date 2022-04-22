#ifndef _CRT_SECURE_NO_WARNINGS
#	define _CRT_SECURE_NO_WARNINGS
#endif
#ifdef _DEBUG
//for memory leak check
#	ifndef _CRTDBG_MAP_ALLOC
#		define _CRTDBG_MAP_ALLOC //使生成的内存dump包含内存块分配的具体代码为止
#	endif
#	include <stdlib.h> 
#	include <crtdbg.h>
#	define CheckMemoryLeak _CrtSetDbgFlag( _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG )| _CRTDBG_LEAK_CHECK_DF)
#endif
#include <stdio.h>
#include <vector>
#include <string>

class Interface1
{
public:
	virtual ~Interface1() = default;
};
class Interface2
{
public:
	virtual ~Interface2() = default;
};
class Object : public Interface1, public Interface2
{

};

using namespace std;
int main() 
{
#ifdef _DEBUG
	CheckMemoryLeak;
#endif
	Object* obj = new Object;

	/**
	 * Object <-> Interface1 / Interface2 上下行转换完全一致
	 * 
	 * static_cast<Interface1*>(obj) 等同于 (Interface1*)obj
	 * static_cast<Interface2*>(obj) 等同于 (Interface2*)(obj + offsetof(Interface1))
	 * pis ： 在多继承时, 用 static_cast 自动计算了偏移量比较省心
	 * 
	 * key word : static 
	 */
	printf("\nstatic_cast------------------\n");
	printf("%p, %p\n", static_cast<Interface1*>(obj), static_cast<Interface2*>(obj));
	printf("%p, %p\n", static_cast<Object*>(static_cast<Interface1*>(obj)), static_cast<Object*>(static_cast<Interface2*>(obj)));
	/**
	 * Object -> Interface1 向上转换与 static_cast 相同
	 * Interface1-> Object 向下转换：
	 * __dynamic_cast(address, typeinfo for Interface1,typeinfo for Object) 进行类型检查, 转换失败返回 0
	 * 如果转换失败, 会抛出 __cxa_bad_cast 异常
	 * 
	 * key word : dynamic
	 */
	printf("\ndynamic_cast------------------\n");
	printf("%p, %p\n", dynamic_cast<Interface1*>(obj), dynamic_cast<Interface2*>(obj));
	printf("%p, %p\n", dynamic_cast<Object*>(dynamic_cast<Interface1*>(obj)), dynamic_cast<Object*>(dynamic_cast<Interface2*>(obj)));
	//printf("%p\n", dynamic_cast<Object*>((Interface1*)(void*)123)); //__cxa_bad_cast

	/**
	 * const_cast 只能消除限定符,不能更改继承类型
	 * 
	 * key word : const
	 */
	printf("\nconst_cast------------------\n");
	const Object* obj1 = obj;
	printf("%p, %p\n", obj, const_cast<Object*>(obj1));
	/**
	 * reinterpret_cast 等效于 c style 指针转换 (Interface1*)((void*)1) 和 (Interface2*)((void*)2)
	 *
	 * key word : reinterpret (重新解释)
	 */
	printf("\nreinterpret_cast--------------\n");
	printf("%p, %p\n", reinterpret_cast<Interface1*>(1), reinterpret_cast<Interface2*>(2));
	printf("%p, %p\n", reinterpret_cast<Interface1*>(obj), reinterpret_cast<Interface2*>(obj));
	printf("%p, %p\n", reinterpret_cast<Object*>(static_cast<Interface1*>(obj)), reinterpret_cast<Object*>(static_cast<Interface2*>(obj)));

	delete obj;
	return 0;
}