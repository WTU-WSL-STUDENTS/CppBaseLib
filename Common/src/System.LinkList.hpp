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
         * @description: ��ȡList�����Enumrator
         * @param {*}
         * @return {����IEnumerator<T>�� ͨ���ýӿ��ͷ�List����Ķ��ڴ�}
         * @author: like
         */        
        virtual IEnumerator<T>* GetEnumerator() override
        {
            return new LinkListEnumrator<T>(head, count);
        }
        /**
         * @description: ��ǰ������
         * @param {*}
         * @return {���ص�ǰList�ڴ洢������}
         * @author: like
         */        
        virtual size_t Count() override{return count;}
        /**
         * @description: �Ƿ�Ϊֻ�����ýӿ���Զ����false��δʵ����ع���
         * @param {*}
         * @return {����true}
         * @author: like
         */        
        virtual bool IsReadOnly() override{return isReadOnly;}
        /**
         * @description: ĩβ��������. O(1)
         * @param {����ֵ} T
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
         * @description: ����������ݣ����������ͷ��ڴ� 
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
         * @description: �Ƿ����ָ������.  O(n)
         * @param {�����ҵ�ֵ} T
         * @return {ָ��Ԫ�ص�λ��}
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
         * @description: ��buffer��ָ��λ�ÿ�ʼ����List�������帴�Ƶ�buffer��
         * @param {����ָ��} T*
         * @param {����ָ��ƫ����} offset
         * @return {*}
         * @author: like
         */        
        virtual void CopyTo(T* container, int offset) override
        {
            // memcpy(container + offset, scan0, count * sizeof(T));
        }
        /**
         * @description: �ӵ�һ��Ԫ�ؿ�ʼ���Ҳ�ɾ��ָ��Ԫ��. O(n^2)
         * @param {Ŀ��ֵ} T
         * @return {�Ƿ�ɾ���ɹ������ɾ��ʧ�ܣ�˵����ǰ������ָ��ֵ}
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