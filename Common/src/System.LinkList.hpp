/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-13 14:07:19
 * @LastEditors: like
 * @LastEditTime: 2021-08-13 17:29:54
 */
#ifndef SYSTEM_STACK_HPP
#define SYSTEM_STACK_HPP

#include <System.List.hpp>

namespace System
{
    template<class T>
    class LinkListNode
    {
    protected:
    public:
        LinkListNode<T>* pre;
        LinkListNode<T>* next;
        T val;
        LinkListNode() : pre(NULL), next(NULL){}
        LinkListNode(LinkListNode<T>* p, T v, LinkListNode<T>* n) : pre(p), val(v), next(n){}       
        const LinkListNode<T>* Previous(){return pre;}
        const LinkListNode<T>* Next(){return next;}
        T Value    (){return val;}
        T& RefValue(){return val;}
    };

    template<class T>
    class LinkListEnumrator : public IEnumerator<T>
    {
    protected:
        LinkListNode<T>* scan0;
        LinkListNode<T>* current;
        int position;
        int count;
    public:
        LinkListEnumrator(LinkListNode<T>* begin, size_t len):scan0(begin), current(begin), position(-1), count(len){}
        virtual T&  Current()   override {return current->RefValue();}
        virtual bool MoveNext() override 
        {
            if(++position < count)
            {
                current = current->next;
                return true;  
            }  
            return false;
        }
        virtual void Reset()    override 
        {
            position = -1;
            current  = scan0;
        }
        virtual void Dispose()  override
        {
            if(NULL == scan0)
            {
                return;
            }
            LinkListNode<T>* p;
            do
            {         
                p = scan0->next;
                delete scan0;
                scan0 = p;
            } while (count-- > 0);  
        }
    };
    
    template<class T>
    class LinkList : ICollection<T> , LinkListNode<T>
    {
    private:
        LinkListNode<T>* head;
        LinkListNode<T>* last;
        size_t  count;
        bool isReadOnly;
    protected:
        LinkListNode<T>* FindFromHead(int offset, LinkListNode<T>* begin)
        {
            return offset > 0 ? FindFromHead(--offset, begin->next) : begin->next;
        }
        LinkListNode<T>* FindFromLast(int offset, LinkListNode<T>* begin)
        {
            return offset > 0 ? FindFromLast(--offset, begin->pre) : begin->pre;
        }
    public:
        LinkList() : head(NULL), last(NULL), count(0), isReadOnly(false){}
        T& operator[](int index)
        {
            return FindFromHead(index, head)->val;
        }
        LinkListNode<T>* First(){return head;}
        LinkListNode<T>* Last (){return last;}
         /**
         * @description: 获取List对象的Enumrator
         * @param {*}
         * @return {返回IEnumerator<T>， 通过该接口释放List申请的堆内存}
         * @author: like
         */        
        virtual IEnumerator<T>* GetEnumerator() override
        {
            return new LinkListEnumrator<T>(head, count);
        }
        /**
         * @description: 当前数据量
         * @param {*}
         * @return {返回当前List内存储的数据}
         * @author: like
         */        
        virtual size_t Count() override{return count;}
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
            LinkListNode<T>* node = new LinkListNode<T>(last, item, NULL); 
            count++;
            if(1 < count)
            {
                last->next = node;
                last       = node;
            }
            else
            {
                head = node;
                last = node;
            }
        }
        /**
         * @description: 清楚数据内容，但并不会释放内存 
         * @param {*}
         * @return {*}
         * @author: like
         */        
        virtual void Clear() override
        {        
            // LinkListNode<T>* p = head;
            // while(NULL != (head = p))
            // {
            //     p = head->next;
            //     delete head;
            // }
            // head  = NULL;
            // pre   = NULL;
            // count = 0;
        }
        /**
         * @description: 是否存在指定数据.  O(n)
         * @param {待查找的值} T
         * @return {指定元素的位置}
         * @author: like
         */        
        virtual int Contains(const T& item) override
        {
            // int loopCount = (int)count;
            // while(--loopCount > -1)
            // {
            //     if(*(scan0 + loopCount) == item)
            //     {
            //         return loopCount;
            //     }
            // } 
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
            // memcpy(container + offset, scan0, count * sizeof(T));
        }
        /**
         * @description: 从第一个元素开始查找并删除指定元素. O(n^2)
         * @param {目标值} T
         * @return {是否删除成功，如果删除失败，说明当前不存在指定值}
         * @author: like
         */        
        virtual bool Remove(const T& item) override
        {
            LinkListNode<T>* begin = head;
            while(NULL != begin)
            {
                if(item == begin->val)
                {
                    return true;
                }
                begin = begin->next;
            }    
            return false;    
        }
        
    };
}

#endif