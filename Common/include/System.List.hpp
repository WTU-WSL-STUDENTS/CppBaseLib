/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-12 16:04:40
 * @LastEditors: like
 * @LastEditTime: 2021-08-20 18:03:59
 */
#ifndef SYSTEM_LIST_HPP
#define SYSTEM_LIST_HPP
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
namespace System
{
    class IDisposable
    {
    public:
        virtual void Dispose() = 0;
    };

    template<class T>
    class IEnumerator : public IDisposable
    {
    public:
        virtual T*  Current()   = 0;
        virtual bool MoveNext() = 0;
        virtual void Reset()    = 0;
    };

    template<class T>
    class IEnumerable
    {
    public:
        virtual IEnumerator<T>* GetEnumerator() = 0;
    };

    template<class T>
    class ICollection : public IEnumerable<T>
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
       
    template<class T>
    class IList : public ICollection<T>
    {
    public:
        virtual int  IndexOf(const T& item)             = 0;
        virtual void Insert(size_t index, const T& item)= 0;
        virtual bool RemoveAt(size_t index)             = 0;
    };
    
    template<class T>
    class ListEnumrator : public IEnumerator<T>
    {
    protected:
        T*  scan0;
        int position;
        int count;
    public:
        ListEnumrator(T* begin, size_t len):scan0(begin), position(-1), count(len){}
        virtual T*  Current()   override {return (scan0 + position);}
        virtual bool MoveNext() override {return ++position < count;}
        virtual void Reset()    override {position = -1;}
        virtual void Dispose()  override {}
    };

    template<class T>
    class List : IList<T>
    {
    private:
        size_t capacity; 
        T*      scan0;
        size_t  count;
        bool isReadOnly;
    public:
        List(int _capacity = 1024): capacity(_capacity), scan0((T*)malloc(sizeof(T) * capacity)), 
            count(0), isReadOnly(false){}
        List(IEnumerable<T> enumerable) : count(0), isReadOnly(false)
        {
            IEnumerator<T>* enumerator = enumerable.GetEnumerator();
            if(enumerator->MoveNext())
            {
                T* src = enumerator->Current();
                count = 1;
                while(enumerator->MoveNext())
                {
                    count++;
                }
                capacity = (1 + Count >> 10) << 10;
                int byteCount = sizeof(T) * count;
                scan0 = malloc(byteCount);
                memcpy(scan0, src, byteCount);
            }
            else
            {
                capacity = 1024;
                scan0 = malloc(sizeof(T) * capacity);
            }
        }
        ~List()
        {
            free(scan0);
        }
        /**
         * @description: 获取List对象的Enumrator
         * @param {*}
         * @return {返回IEnumerator<T>， 通过该接口释放List申请的堆内存}
         * @author: like
         */        
        virtual IEnumerator<T>* GetEnumerator() override
        {
            return new ListEnumrator<T>(scan0, count);
        }
        /**
         * @description: 当前数据量
         * @param {*}
         * @return {返回当前List内存储的数据}
         * @author: like
         */        
        virtual size_t Count() override{return count;}
        T& operator[](int index)
        {
            return *(scan0 + index);
        }
        /**
         * @description: 是否为只读，该接口永远返回false，未实现相关功能
         * @param {*}
         * @return {返回true}
         * @author: like
         */        
        virtual bool IsReadOnly() override{return isReadOnly;}
        /**
         * @description: 末尾增加数据. O(1)
         * @param {数据值} T
         * @return {*}
         * @author: like
         */        
        virtual void Add(const T& item) override
        {
            if(count < capacity)
            {
                memcpy(scan0 + count, &item, sizeof(T));
                count++;
                return;
            }
            capacity *= 2;
            scan0 = (T*)realloc(scan0, sizeof(T) * capacity);
            memcpy(scan0 + count, &item, sizeof(T));
            count++;
        }
        /**
         * @description: 清楚数据内容，但并不会释放内存 
         * @param {*}
         * @return {*}
         * @author: like
         */        
        virtual void Clear() override
        {
            count = 0;
        }
        /**
         * @description: 是否存在指定数据.  O(n)
         * @param {待查找的值} T
         * @return {指定元素的位置}
         * @author: like
         */        
        virtual int Contains(const T& item) override
        {
            int loopCount = (int)count;
            while(--loopCount > -1)
            {
                if(*(scan0 + loopCount) == item)
                {
                    return loopCount;
                }
            } 
            return -1;
        }
        /**
         * @description: 从buffer的指定位置开始，将List数据整体复制到buffer内
         * @param {容器指针} T*
         * @param {容器指针偏移量} offset
         * @return {*}
         * @author: like
         */        
        virtual void CopyTo(T* container, int offset) override
        {
            memcpy(container + offset, scan0, count * sizeof(T));
        }
        List<T>* Clone()
        {
            List<T>* dest = new List<T>(capacity);
            dest.count = count;
            memcpy(dest.scan0, scan0, count * sizeof(T));
            return 
        }
        /**
         * @description: 从第一个元素开始查找并删除指定元素. O(n^2)
         * @param {目标值} T
         * @return {是否删除成功，如果删除失败，说明当前不存在指定值}
         * @author: like
         */        
        virtual bool Remove(const T& item) override
        {
            size_t loopCount = 0;
            if(2 > count)
            {
                do
                {
                    if(0 == memcmp(scan0 + loopCount, &item, sizeof(T)))
                    {
                        count--;
                        return true;
                    }
                } while (loopCount++ < count);  
            }
            do
            {
                if(0 == memcmp(scan0 + loopCount, &item, sizeof(T)))
                {
                    memcpy(scan0 + loopCount , scan0 + loopCount + 1, (count - loopCount - 2) * sizeof(T));
                    count--;
                    return true;
                }
            } while (loopCount++ < count);
            return false;    
        }
        /**
         * @description: 查找指定元素的位置 O(n)
         * @param {目标值} T
         * @return {目标值的位置}
         * @author: like
         */        
        virtual int IndexOf(const T& item) override
        {
            size_t loopCount = 0;
            do
            {
                if(0 == memcmp(scan0 + loopCount, &item, sizeof(T)))
                {
                    return loopCount;
                }
            } while (loopCount++ < count);
            return -1; 
        }
        /**
         * @description: 在指定位置插入插入值，指定位置之后（包含指定位置）的元素位置整体向后偏移1个. O(n)
         * @param {插入的位置} index
         * @param {插入的值} T
         * @return {*}
         * @author: like
         */        
        virtual void Insert(size_t index, const T& item) override
        {
            if(count < capacity)
            {
                memcpy(scan0 + index + 1, scan0 + index, (count - index - 1) * sizeof(T));
                memcpy(scan0 + index, &item, sizeof(T));
                count++;
                return;
            }
            capacity *= 2;
            scan0 = (T*)realloc(scan0, sizeof(T) * capacity);
            memcpy(scan0 + count, &item, sizeof(T));
            count++;
        }
        /**
         * @description: 删除指定位置的值，后方的元素自动向前补齐. O(log(n))
         * @param {待删除的位置} index
         * @return {删除的位置不能超过当前数据范围}
         * @author: like
         */        
        virtual bool RemoveAt(size_t index) override
        {
            if(index < count)
            {
                if(1 < count)
                {
                    memcpy(scan0 + index, scan0 + index + 1, (count - index - 2) * sizeof(T));
                }
                count--;
                return true;
            }
            return false;    
        }
        /**
         * @description: 将指定数据增加到当前容器，List(IEnumerable<T>)构造函数就调用了该接口
         * @param {数据源} IEnumerator
         * @return {*}
         * @author: like
         */        
        void AddRange(const IEnumerator<T> (&enumrator))
        {
            if(enumrator.MoveNext())
            {       
                Add(*(enumrator.Current()))
                AddRange(enumrator);
            }
        }
    };
}


#endif