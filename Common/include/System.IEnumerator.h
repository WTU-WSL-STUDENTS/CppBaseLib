/*
 * @Description: 
 * 优化虚函数表查询 :
 * refrence : http://t.zoukankan.com/stdpain-p-14590681.html
 * final 语法上功能与 c# 中密封类类似
 * 1. final 关键字 :  
 *      class SumExpressionV2 final: public Expression
 *      ((SumExpression*)(expression))->update(&context, block[i]);
 * 优点 : 优化后性能与非虚函数几乎相同, 远远大于虚函数查表。 final 关键字可以让编译器有勇气内联
 * 缺点 : 表达式有点复杂 final 和 (SumExpression*) 缺一不可
 * 2. CRTP
 * 
 * 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-23 20:31:41
 * @LastEditors: like
 * @LastEditTime: 2022-01-24 19:05:24
 */
#ifndef SYSTEM_IENUMERATOR_HPP
#define SYSTEM_IENUMERATOR_HPP

#define ENABLE_CRTP /* 静态分发使能 */

#include <System.IDisposable.h>

namespace System
{
    template<class T>
    class IEnumerator;
    template<class T>
    class IEnumerable;
    template<class T>
    class ICollection;
};

template<class T>
class System::IEnumerator : public IDisposable
{
public:
    virtual T*  Current()   = 0;
    virtual bool MoveNext() = 0;
    virtual void Reset()    = 0;
};
template<class T>
class System::IEnumerable
{
public:
    virtual IEnumerator<T>* GetEnumerator() = 0;
};
template<class T>
class System::ICollection : public IEnumerable<T>
{
public:
    virtual size_t Count()                          = 0;
    virtual bool IsReadOnly()                       = 0;
    virtual void Add(const T& item)                 = 0;
    virtual void Clear ()                           = 0;
    virtual int Contains(const T& item)             = 0;
    virtual void CopyTo(T* container, int offset)   = 0;
    virtual bool Remove (const T& item)             = 0;
};
#endif