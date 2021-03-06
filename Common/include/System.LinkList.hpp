/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-13 14:07:19
 * @LastEditors: like
 * @LastEditTime: 2022-02-11 10:46:01
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
         * @description: ????List??????Enumrator
         * @param {*}
         * @return {????IEnumerator<T>?? ??????????????List????????????}
         * @author: like
         */        
        virtual IEnumerator<T>* GetEnumerator() override
        {
            return new LinkListEnumrator<T>(head, count);
        }
        /**
         * @description: ??????????
         * @param {*}
         * @return {????????List????????????}
         * @author: like
         */        
        virtual size_t Count() override{return count;}
        /**
         * @description: ??????????????????????????false????????????????
         * @param {*}
         * @return {????true}
         * @author: like
         */        
        virtual bool IsReadOnly() override{return isReadOnly;}
        /**
         * @description: ????????????. O(1)
         * @param {??????} T
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
         * @description: ?????????????????????????????? 
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
         * @description: ????????????????.  O(n)
         * @param {??????????} T
         * @return {??????????????}
         * @author: like
         */        
        virtual bool Contains(const T& item) override
        {
            // int loopCount = (int)count;
            // while(--loopCount > -1)
            // {
            //     if(*(scan0 + loopCount) == item)
            //     {
            //         return loopCount;
            //     }
            // } 
            return false;
        }
        /**
         * @description: ??buffer??????????????????List??????????????buffer??
         * @param {????????} T*
         * @param {??????????????} offset
         * @return {*}
         * @author: like
         */        
        virtual void CopyTo(T* container, int offset) override
        {
            // memcpy(container + offset, scan0, count * sizeof(T));
        }
        /**
         * @description: ??????????????????????????????????. O(n^2)
         * @param {??????} T
         * @return {????????????????????????????????????????????????}
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