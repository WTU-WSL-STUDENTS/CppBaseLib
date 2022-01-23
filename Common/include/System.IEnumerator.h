/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-23 20:31:41
 * @LastEditors: like
 * @LastEditTime: 2022-01-23 20:35:15
 */
#ifndef SYSTEM_IENUMERATOR_HPP
#define SYSTEM_IENUMERATOR_HPP
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