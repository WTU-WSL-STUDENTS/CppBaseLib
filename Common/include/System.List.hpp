/*
 * @Description: ���ڴ����ݵ� List ���� , ջ��������Ϊ 0 ���Զ��ͷ��ڴ�, ��Ӱ�� List �Ĺ���
 * �Ż����� : 
 *  1. �Ż��麯����Ĳ�ѯ, ���һ���÷�ȡ���麯����Ĳ�ѯ.
 *  2. �� List::Add ���� policy-based class design , ����ջ���ݵ� Add ���Բ�ֳ���, �� API caller ���� List �����ֲ���, ʵ��ջ���� List ������(Add ʱ�������)        
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-12 16:04:40
 * @LastEditors: like
 * @LastEditTime: 2022-01-23 20:44:07
 */
#ifndef SYSTEM_LIST_HPP
#define SYSTEM_LIST_HPP
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <System.IEnumerator.h>
namespace System
{   
    template<class T>    
    class IList;
    template<class T>
    class ListEnumrator;
    template<class T>
    class List;
}

template<class T>
class System::IList : public ICollection<T>
{
public:
    virtual int  IndexOf(const T& item)             = 0;
    virtual void Insert(size_t index, const T& item)= 0;
    virtual bool RemoveAt(size_t index)             = 0;
};

template<class T>
class System::ListEnumrator : public IEnumerator<T>
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
class System::List : IList<T>
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
     * @description: ��ȡList�����Enumrator
     * @param {*}
     * @return {����IEnumerator<T>�� ͨ���ýӿ��ͷ�List����Ķ��ڴ�}
     * @author: like
     */        
    virtual IEnumerator<T>* GetEnumerator() override
    {
        return new ListEnumrator<T>(scan0, count);
    }
    /**
     * @description: ��ǰ������
     * @param {*}
     * @return {���ص�ǰList�ڴ洢������}
     * @author: like
     */        
    virtual size_t Count() override{return count;}
    T& operator[](int index)
    {
        return *(scan0 + index);
    }
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
     * @description: ����������ݣ����������ͷ��ڴ� 
     * @param {*}
     * @return {*}
     * @author: like
     */        
    virtual void Clear() override
    {
        count = 0;
    }
    /**
     * @description: �Ƿ����ָ������.  O(n)
     * @param {�����ҵ�ֵ} T
     * @return {ָ��Ԫ�ص�λ��}
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
     * @description: ��buffer��ָ��λ�ÿ�ʼ����List�������帴�Ƶ�buffer��
     * @param {����ָ��} T*
     * @param {����ָ��ƫ����} offset
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
     * @description: �ӵ�һ��Ԫ�ؿ�ʼ���Ҳ�ɾ��ָ��Ԫ��. O(n^2)
     * @param {Ŀ��ֵ} T
     * @return {�Ƿ�ɾ���ɹ������ɾ��ʧ�ܣ�˵����ǰ������ָ��ֵ}
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
     * @description: ����ָ��Ԫ�ص�λ�� O(n)
     * @param {Ŀ��ֵ} T
     * @return {Ŀ��ֵ��λ��}
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
     * @description: ��ָ��λ�ò������ֵ��ָ��λ��֮�󣨰���ָ��λ�ã���Ԫ��λ���������ƫ��1��. O(n)
     * @param {�����λ��} index
     * @param {�����ֵ} T
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
     * @description: ɾ��ָ��λ�õ�ֵ���󷽵�Ԫ���Զ���ǰ����. O(log(n))
     * @param {��ɾ����λ��} index
     * @return {ɾ����λ�ò��ܳ�����ǰ���ݷ�Χ}
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
     * @description: ��ָ���������ӵ���ǰ������List(IEnumerable<T>)���캯���͵����˸ýӿ�
     * @param {����Դ} IEnumerator
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
#endif