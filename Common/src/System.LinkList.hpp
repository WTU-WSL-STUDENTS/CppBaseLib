/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-13 14:07:19
 * @LastEditors: like
 * @LastEditTime: 2021-08-20 18:10:07
 */
#ifndef SYSTEM_STACK_HPP
#define SYSTEM_STACK_HPP

#include <System.List.hpp>

namespace System
{
    template<class T>
    struct DoubleLinkNode
    {
        DoubleLinkNode<T>* pre;
        T*              val;
        DoubleLinkNode<T>* next;
        DoubleLinkNode(DoubleLinkNode<T>* p, const T& v,DoubleLinkNode<T>* n) : pre(p), val((T*)malloc(sizeof(T))), next(){memcpy(val, &v, sizeof(T));}
        ~DoubleLinkNode(){ free(val); val = NULL;}
        static DoubleLinkNode<T>* FindFromHead(size_t offset, DoubleLinkNode<T>* begin)
        {
            return offset > 0 ? FindFromHead(--offset, begin->next) : begin;   
        }
        static DoubleLinkNode<T>*  FindFromLast(size_t offset, DoubleLinkNode<T>* last)
        {
            return offset > 0 ? FindFromLast(--offset, last->pre) : last;
        }
    };
    template<class T>
    class LinkListEnumrator : public IEnumerator<T>
    {
    protected:
        T** scan0;
        int position;
        size_t* count;
    public:
        LinkListEnumrator(DoubleLinkNode<T>* begin, size_t (&size)) : scan0((T**)malloc(sizeof(long) * size)), position(-1), count(&size)
        {
            for(size_t i = 0; i < size; i++)
            {
                *(scan0 + i) = begin->val;
                begin = begin->next;
            }
        }
        virtual T*  Current()   override {return scan0[position];}
        virtual bool MoveNext() override {return ++position < (int)(*count);}
        virtual void Reset()    override {position = -1;}
        virtual void Dispose()  override
        {
            while(*count-- > 0)
            {
                if(scan0[*count])
                {             
                    free(scan0[*count]);
                    scan0[*count] = NULL;
                }
            }
            if(NULL != scan0)
            {
                free(scan0);
                scan0 = NULL;
            }
        }
    };
    
    template<class T>
    class LinkList : ICollection<T> 
    {
    private:
        DoubleLinkNode<T>* head;
        DoubleLinkNode<T>* last;
        size_t  count;
        bool isReadOnly;
    public:
        LinkList() : head(NULL), last(NULL), count(0), isReadOnly(false){}
        ~LinkList()
        {   
            DoubleLinkNode<T>* p(NULL);
            while(count-- > 0)
            {
                p = head->next;
                delete head;
                head = p;
            }
        }
        T& operator[](size_t index)
        {
            return index < (count >> 1) ? *(DoubleLinkNode<T>::FindFromHead(index, head)->val) : *(DoubleLinkNode<T>::FindFromLast(count - 1 - index, last)->val);
        }
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
            DoubleLinkNode<T>* node = new DoubleLinkNode<T>(last, item, NULL);
            count++;
            if(last)
            {
                last->next = node;
                last       = node;  
                return; 
            }
            last = node;
            head = node;
        }
        /**
         * @description: 清楚数据内容，但并不会释放内存 
         * @param {*}
         * @return {*}
         * @author: like
         */        
        virtual void Clear() override
        {   
            if(count)
            {
                IEnumerator<T>* it = GetEnumerator();
                it->Dispose();
                head = NULL;
                last = NULL;   
                count = 0;
            }
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
            DoubleLinkNode<T>* begin = head;
            if(count == 0)
            {
                return false;
            }
            if(memcmp(head->val, &item, sizeof(T)) && count == 1)
            {
                delete head;
                head = NULL;
                last = NULL;
                count = 0;
                return true;
            }
            do
            {
                if(memcmp(begin->val, &item, sizeof(T)))
                {
                    if(last == begin)
                    {
                        last = last->pre;
                    }
                    else if(head == begin)
                    {
                        head = head->next;
                    }
                    if(begin->pre)
                    {
                        begin->pre->next = begin->next;
                    }
                    if(begin->next)
                    {
                        begin->next->pre = begin->pre;
                    }
                    delete begin;
                    begin = NULL;
                    count--;
                    return true;
                }
            }while(NULL != (begin = begin->next));
            return false;  
        }
        
    };
}

#endif