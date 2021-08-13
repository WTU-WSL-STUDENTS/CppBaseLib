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
         * @description: ����㿪ʼƫ��count��Ԫ�أ��ݹ�д����û��count��Χ�ж�,����count��������ǰ����
         * @param {������ͷ��ʼ����ƫ����} int
         * @return {ָ��λ�õ�ָ������}
         * @author: like
         */        
        INode<TKey, TVal>*& ElementAtFindFromHead(int offset)/*  */
        {
            return offset > 0 ? ElementAtFindFromHead(--offset)->next : Head->next; 
        }
        /**
         * @description: ����㿪ʼƫ��count��Ԫ�أ��ݹ�д����û��count��Χ�ж�,����count��������ǰ����
         * @param {������β����ʼ��ǰ��ƫ����} int
         * @return {ָ��λ�õ�ָ������}
         * @author: like
         */         
        INode<TKey, TVal>*& ElementAtFindFromLast(int offset)/* �ݹ�д����û��count��Χ�ж�,����count��������ǰ����  */
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
        INode<TKey, TVal>*& operator[] (size_t index)    //�������ã������ſ��ԶԷ���ֵ��ֵ
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
        INode<TKey, TVal>*& FindIf(size_t begin, int& end/* ���ص� ���ҵ���Ԫ���±� */,_Lamda _lamda)
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