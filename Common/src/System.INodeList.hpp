/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 19:54:41
 * @LastEditors: like
 * @LastEditTime: 2021-08-12 16:02:00
 */
#ifndef SYSTEM_STACK_HPP
#define SYSTEM_STACK_HPP

namespace System
{
    template<class TKey, class TVal>
    class INode
    {
    public :
        INode<TKey, TVal>* pre;
        TKey key;
        TVal val;
        INode<TKey, TVal>* next;
        INode() : pre(NULL), next(NULL){}
        INode(INode* (&pr), TKey k, TVal v, INode* (&nex)) : pre(pr), key(k), val(v), next(nex){}
        INode(INode* (&pr), TKey k, TVal v) : pre(pr), key(k), val(v), next(NULL){}
        INode(TKey k, TVal v, INode* (&nex)) : pre(NULL), key(k), val(v), next(nex){}
        INode& operator=(const INode& node)
        {
            if(this != *node)
            {
                next = node->next;
                val  = node->val;
            }
            return *this;
        }
        virtual bool Dispose(){return true;};
        virtual int ToString(char* buffer){return 0;};
    };
    /**
     * INodeList
     * |----|
     * | 0  | |HEAD|
     * |----|
     * | 1  |
     * |----|
     * | 3  | |LAST|
     * |----|
     * /
    */
    template<class TKey, class TVal>
    class INodeList
    {
    protected:  
        INode<TKey, TVal>* Head;
        INode<TKey, TVal>* Last;
        size_t count;
    public:
        INodeList() : Head(NULL), Last(NULL){}
        ~INodeList(){Dispose();}
        size_t Count(){return count;}
        bool Dispose()
        {
            INode<TKey, TVal>* crt;
            INode<TKey, TVal>* next = Head;
            while(count-- > 0)
            {
                crt  = next;
                next = crt->next;
                delete crt;
            }
            Head = NULL;
            Last = NULL;
            return true;
        }
        /**
         * @description: 从起点开始偏移count的元素，递归写法，没做count范围判断,输入count不超过当前容量
         * @param {从数据头开始向后的偏移量} int
         * @return {指定位置的指针数据}
         * @author: like
         */        
        INode<TKey, TVal>*& ElementAtFindFromHead(int offset)/*  */
        {
            return offset > 0 ? ElementAtFindFromHead(--offset)->next : Head->next; 
        }
        /**
         * @description: 从起点开始偏移count的元素，递归写法，没做count范围判断,输入count不超过当前容量
         * @param {从数据尾部开始向前的偏移量} int
         * @return {指定位置的指针数据}
         * @author: like
         */         
        INode<TKey, TVal>*& ElementAtFindFromLast(int offset)/* 递归写法，没做count范围判断,输入count不超过当前容量  */
        {
            return offset > 0 ? ElementAtFindFromLast(--offset)->next : Last->pre; 
        }
        INode<TKey, TVal>*& At(int index)
        {
            if(count > idx)
            {
                if(idx < (count >> 1))
                {
                    ElementAtFindFromHead(index);
                }
                else
                {
                    ElementAtFindFromLast(count - index - 1);
                }
            }
            return NULL;
        }
        INode<TKey, TVal>*& operator[] (size_t index)    //返回引用，这样才可以对返回值赋值
        {
            if(idx < (count >> 1))
            {
                return ElementAtFindFromHead(index);
            }
            else
            {
                return ElementAtFindFromLast(count - index - 1);
            }
        }
        INode<TKey, TVal>*& Append(TKey key, TVal val)
        {  
            if(0 < count)
            {
                INode<TKey, TVal>* node = new INode<TKey, TVal>(Last, key, val, NULL);
                Last->next = node;
                Last = node;
                count++;
                return Last;
            }
            Head = new INode<TKey, TVal>(NULL, key, val, NULL);
            Last = Head;
            count = 1;
            return Last;
        }
        bool ReplaceAt(TKey key, TVal val, int index)
        {
            INode<TKey, TVal>* res = At(index);
            if(NULL != res)
            {
                res->key = key;
                res->val = val;
                return true;
            }
            return false;
        }
        bool RemoveAt(int index)
        {
            INode<TKey, TVal>* res = At(index);
            if(NULL != res)
            {
                res->next->pre = res->pre;
                res->pre->next = res->next;
                delete res;
                count--;
                return true;
            }
            return false;
        }
        template<class _Lamda>
        INode<TKey, TVal>*& FindIf(size_t begin, int& end/* 返回的 查找到的元素下标 */,_Lamda _lamda)
        {
            if(begin < end && count > end)
            {
                int revLen = count - end; 
                if(begin < revLen)/* begin->end*/
                {
                    INode<T>* p = ElementAtFindFromHead(begin);
                    do
                    {
                        if(_lamda(p))
                        {
                            return p;
                        }
                        p = p->next;
                    } while (++begin < end);   
                }
                else/* end->begin */
                {
                    INode<T>* p = ElementAtFindFromHead(revLen);
                    do
                    {
                        if(_lamda(p))
                        {
                            return p;
                        }
                        p = p->pre;
                    } while (begin < --end);     
                }
            }
            return NULL;
        };

    };
};

#endif