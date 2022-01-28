/*
 * @Description: ���ڴ����ݵ� List ���� , ջ��������Ϊ 0 ���Զ��ͷ��ڴ�, ��Ӱ�� List �Ĺ���
 * �Ż����� : 
 *  1. �Ż��麯����Ĳ�ѯ, ���һ���÷�ȡ���麯����Ĳ�ѯ.
 *  2. �� List::Add ���� policy-based class design , ����ջ���ݵ� Add ���Բ�ֳ���, �� API caller ���� List �����ֲ���, ʵ��ջ���� List ������(Add ʱ�������)        
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-12 16:04:40
 * @LastEditors: like
 * @LastEditTime: 2022-01-28 09:45:45
 */
#ifndef SYSTEM_LIST_HPP
#define SYSTEM_LIST_HPP
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include <System.IEnumerator.h>

#ifdef ASSERT_ENABLE
#define LIST_DEBUG(condition) assert(condition)
#endif
#define LIST_COUNT_MAX SIZE_MAX / sizeof(T)
#define LIST_MEMORY_GROUTH_RATE 1
#define TRIM_EXCESS_THRESHOLD 0.9

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
class System::ListEnumrator final: public IEnumerator<T>
{
friend class List<T>;
private:
    T**  m_pScan0;
    int m_nPosition;
    int m_nCount;
public:
    ListEnumrator(T** begin, size_t len):m_pScan0(begin), m_nPosition(-1), m_nCount(len){}
    T* Current() override 
    {
        return (*m_pScan0 + m_nPosition);
    }
    bool MoveNext() override 
    {
        return ++m_nPosition < m_nCount;
    }
    void Reset() override 
    {
        m_nPosition = -1;
    }
    void Dispose() override{}
};
/**
 * @brief 
 * 
 * @tparam T : 
 * 1. ָ����ڴ��ָ��
 *  �ŵ�
 *  - List ������
 *  ȱ��
 *  - ��� Add ��ָ��ָ��ջ�ڴ�, ����ڷ��ʴ���
 * 2. ��ջ�ڴ�ṹ
 *  �ŵ�
 *  - ������������, ����ʵ�� Span ������ Cache ����
 *  ȱ��
 *  ��ջ���ݶ����Ը�������ʽ�洢�� List ��, ������������ T ���ݽṹ��СӰ��
 */
template<class T, template<class>class ListItemMemPolicy>
class System::List : IList<T>
{
private:
    T* m_pScan0;    /* �����ڴ�� */
    ListItemMemPolicy<T> m_policy;
    size_t m_nCapacity; 
    size_t m_nCount;
    bool isReadOnly;
    /**
     * @brief �ڴ�����
     * 
     */
    inline void MemoryGrowth(int nMemoryGrouthRate = LIST_MEMORY_GROUTH_RATE)
    {   
        LIST_DEBUG(LIST_COUNT_MAX == m_nCapacity);
        /* ȷ�� sizeof(T) * capacity ����� */
        m_nCapacity = (LIST_COUNT_MAX >> nMemoryGrouthRate) < m_nCapacity ? LIST_COUNT_MAX : m_nCapacity << nMemoryGrouthRate;
        m_pScan0 = (T*)realloc(m_pScan0, sizeof(T) * m_nCapacity);
    }
    List(){}
public:
    /**
     * @brief ��ʾһ�ַ������÷�������һ��������ȷ��ָ�������Ƿ������Щ����
     * 
     */
    typedef std::function<bool(const T&)> IPredicate;
    /**
     * @brief ��������Ϊ�Ƚ����������ʵ�ֵķ���
     * 
     */
    typedef std::function<int(const T&, const T&)> ICompare;

    /**
     * @brief ��ʼ������, ��ʵ��Ϊ�ղ��Ҿ���ָ���ĳ�ʼ����
     * 
     * @param capacity ��ʼ����
     */
    List(int capacity = 8): m_nCapacity(capacity), m_pScan0((T*)malloc(sizeof(T) * m_nCapacity)), m_nCount(0), isReadOnly(false){}
    List(IEnumerable<T> ie/* T �����ָ��, ��ô�ͷ�����Ȩ�鵱ǰʵ������ */) : isReadOnly(false)
    {
        ICollection<T>* ic = (ICollection<T>*)&ie;
        m_nCount = ic->Count();
        m_nCapacity = m_nCount;
        m_pScan0 = (T*)malloc(sizeof(T) * m_nCapacity);
        ic->CopyTo(m_pScan0, 0);
    }
    /**
     * @brief �������ͷ��ڴ����������� : 1. ���� IEnumerator ����Ԫ���ڴ��ͷ�; 2. ���� IEnumerator::Dispose()
     * 
     */
    ~List()
    {
        this->Clear();
        free(m_pScan0);
    }
    /**
     * @brief ĩβ��������. O(1)
     * @param item 
     * @return * void 
     */
    virtual void Add(const T& item) override
    {
        if(m_nCount == m_nCapacity)
        {
            MemoryGrowth();
        }
        m_policy.Copy(m_pScan0 + m_nCount, item);
        m_nCount++;
    }
    /**
     * @brief ��ָ�����ϵ�Ԫ����ӵ� List<T> ��ĩβ
     * 
     * @param ie 
     */
    void AddRange(IEnumerable<T> ie)
    {
        ICollection<T>* ic = (ICollection<T>*)&ie;
        size_t c = ic->Count();
        if(c + m_nCount <= m_nCapacity)
        {
            ic->CopyTo(m_pScan0 , m_nCount);
        }
        else
        {
            size_t nLackCount = m_nCapacity - c;
            LIST_DEBUG(LIST_COUNT_MAX == m_nCapacity || nLackCount + m_nCapacity < m_nCapacity/* ȷ������� */);
            capacity += nLackCount;
            m_pScan0 = (T*)realloc(m_pScan0, sizeof(T) * m_nCapacity);
            ic->CopyTo(m_pScan0 , m_nCount);
        }
        m_nCount += c;
    }
    /**
     * @brief �� List<T> ���Ƴ�����Ԫ��, List ������ڴ治���ͷ�, Ҳ�������
     * 
     * @return * void 
     */
    virtual void Clear() override
    {
        while(m_nCount)
        {
            m_policy.Free(m_pScan0[--m_nCount]);
        }
    }
    /**
     * @brief ȷ��ĳԪ���Ƿ��� List<T> �С� O(n)
     * 
     * @param item 
     * @return true 
     * @return false 
     */
    virtual bool Contains(const T& item) override
    {
        for(size_t i = 0; i < m_nCount; i++)
        {
            if(m_policy.Equals(m_pScan0 + i, &item))
            {
                return true;
            }
        }
        return false;
    }
    /**
     * @brief �� List<T> ������һ���ָ��Ƶ�һ��������
     * 
     * @param index List<T> �д��㿪ʼ��������ʼ
     * @param array һά����
     * @param arrayIndex array �д��㿪ʼ���������Ӵ˴���ʼ����
     * @param count Ҫ���Ƶ�Ԫ����
     * @return true 
     * @return false 1.array Ϊ NULL; 2. �� List<T> index ��ʼʣ�����ݲ��� count 
     */
    bool CopyTo(int index, T* array, int arrayIndex, int count)
    {
        VALRET_ASSERT(array && (index + count) <= m_nCount, false);
        memcpy(array + arrayIndex, m_pScan0 + index, count * sizeof(T));
        return true;
    }
    bool CopyTo(T* array)
    {
        VALRET_ASSERT(array, false);
        memcpy(array, m_pScan0, m_nCount * sizeof(T));
        return true;
    }
    virtual void CopyTo (T* array, int arrayIndex) override
    {
        VOIDRET_ASSERT(array);
        memcpy(array + arrayIndex, m_pScan0, m_nCount * sizeof(T));
    }
    /**
     * @brief ȷ�� List<T> �Ƿ������ָ��ν�ʶ��������ƥ���Ԫ��
     * 
     * @param match 
     * @return true 
     * @return false 
     */
    bool Exists(IPredicate match)
    {
        for(size_t i = 0; i < m_nCount; i++)
        {
            if(match(m_pScan0[i]))
            {
                return true;
            }
        }
        return false;
    }
    /**
     * @brief ������ָ��ν���������������ƥ���Ԫ�أ����������� List<T> �еĵ�һ��ƥ��Ԫ��
     * 
     * @param match 
     * @return T* ʧ�ܷ��� NULL; �ɹ�����ǳ����ָ��
     */
    T* Find(IPredicate match)
    {
        for(size_t i = 0; i < m_nCount; i++)
        {
            if(match(m_pScan0[i]))
            {
                return m_pScan0 + i;
            }
        }
        return NULL;
    }
    /**
     * @brief ������ָ��ν���������������ƥ���Ԫ�أ����� List<T> ������һ�����е�һ��ƥ����Ĵ��㿪ʼ�������� ���δ�ҵ�������ƥ������˷������� -1
     * 
     * @param match 
     * @return size_t ʧ�ܷ��� SIZE_MAX, �ɹ����� index
     */
    size_t FindIndex(IPredicate match)
    {
        for(size_t i = 0; i < m_nCount; i++)
        {
            if(match(m_pScan0[i]))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    /**
     * @brief ������ָ��ν���������������ƥ���Ԫ�أ����������� List<T> �е����һ��ƥ��Ԫ��
     * 
     * @param match 
     * @return T* ʧ�ܷ��� NULL; �ɹ�����ǳ����ָ��
     */
    T* FindLast(IPredicate match)
    {
        for(size_t i = m_nCount; i > 0;)
        {
            if(match(m_pScan0[--i]))
            {
                return m_pScan0 + i;
            }
        }
        return NULL;
    }
    /**
     * @brief ������ָ��ν���������������ƥ���Ԫ�أ����� List<T> ������һ���������һ��ƥ����Ĵ��㿪ʼ�������� ���δ�ҵ�������ƥ������˷������� -1
     * 
     * @param match 
     * @return size_t ʧ�ܷ��� SIZE_MAX, �ɹ����� index
     */
    T* FindLastIndex(IPredicate match)
    {
        for(size_t i = m_nCount; i > 0;)
        {
            if(match(m_pScan0[--i]))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    void ForEach(function<void(const T&)> action)
    {
        for(size_t i = 0; i < m_nCount; i++)
        {
            action(m_pScan0[i]);
        }
    }
    /**
     * @brief ��ȡ������
     * 
     * @return * IEnumerator<T>* ����������ӿ�
     */
    virtual IEnumerator<T>* GetEnumerator() override
    {
        return new ListEnumrator<T>(scan0, count);
    }
    /**
     * @brief ����ָ��Ԫ�ص�λ�� O(n)
     * 
     * @param item 
     * @return * int 
     */
    virtual size_t IndexOf(const T& item) override
    {
        for(size_t i = 0; i < m_nCount; i++)
        {
            if(m_policy.Equals(m_pScan0 + i, &item))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    size_t IndexOf(const T& item, int index, int count)
    {
        VALRET_ASSERT(index + count <= m_nCount, SIZE_MAX);
        for(size_t i = index; i < m_nCount - count; i++)
        {
            if(m_policy.Equals(m_pScan0 + i, &item))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    size_t IndexOf(const T& item, int index)
    {
        VALRET_ASSERT(index < m_nCount, SIZE_MAX);
        for(size_t i = index; i < m_nCount; i++)
        {
            if(m_policy.Equals(m_pScan0 + i, &item))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    /**
     * @brief �������е�Ԫ�ز��� List<T> ��ָ�������� O(n)
     * 
     * @param index ����Խ��ǰ����Խ��, β������ͬ Add
     * @param item 
     * @return * void 
     */
    virtual void Insert(size_t index, const T& item) override
    {
        LIST_DEBUG(index < m_nCount);
        if(m_nCount < m_nCapacity)
        {
            /*1. �������; 2. ����*/
            for(size_t i = m_nCount; i > index;)
            {
                memcpy(m_pScan0[i--], m_pScan0[i], sizeof(T));
            }
            m_policy.Copy(m_pScan0 + index, item);
        }
        else/* β�� */
        {
            MemoryGrowth();
            m_policy.Copy(m_pScan0 + m_nCount, item);
        }
        m_nCount++;
    }
    /**
     * @brief ��� T Ϊָ������, �ͷŵ�����Ȩ�����ڸ� List<T>
     * 
     * @param index Ӧ�ڴ˴�������Ԫ�صĴ��㿪ʼ������
     * @param ie Ӧ����Ԫ�ز��뵽 List<T> �еļ��ϡ� ����������Ϊ null���������԰���Ϊ null ��Ԫ�أ�������� T Ϊ�������ͣ�
     */
    void InsertRange(int index, IEnumerable<T> ie)
    {
        LIST_DEBUG(index < m_nCount);
        ICollection<T>* ic = (ICollection<T>*)&ie;
        size_t c = ic->Count();
        if(m_nCount + c < m_nCapacity)
        {
            /*1. �������; 2. ����*/
            for(size_t i = m_nCount; i > index;)
            {
                memcpy(m_pScan0 + c + i--, m_pScan0 + i, sizeof(T));
            }
            ic->CopyTo(m_pScan0, index);
            m_nCount += c;
        }
        else/* β�� */
        {
            AddRange(ie);
        }
    }
    size_t LastIndexOf(const T& item)
    {
        for(size_t i = m_nCount; i >0;)
        {
            if(m_policy.Equals(m_pScan0 + --i, &item))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    size_t LastIndexOf(const T& item, size_t index)
    {
        VALRET_ASSERT(index < m_nCount, SIZE_MAX);
        for(size_t i = index; i < SIZE_MAX; i--)
        {
            if(m_policy.Equals(m_pScan0 + i, &item))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    size_t LastIndexOf(const T& item, size_t index, size_t count)
    {
        VALRET_ASSERT(index < m_nCount && count <= index + 1, SIZE_MAX);      
        for(size_t i = 0; i < count; i++)
        {
            if(m_policy.Equals(m_pScan0 + index - i, &item))
            {
                return i;
            }
        }
        return SIZE_MAX;
    }
    /**
     * @brief �� List<T> ���Ƴ��ض�����ĵ�һ��ƥ���� O(n)
     * 
     * @param item 
     * @return true 
     * @return false 
     */
    virtual bool Remove(const T& item) override
    {
        size_t idx = this->IndexOf(item);
        VALRET_ASSERT(SIZE_MAX != idx, false);
        RemoveAt(idx);
        return true;  
    }
    /**
     * @brief �Ƴ���ָ����ν���������������ƥ�������Ԫ��
     * 
     * @param match 
     */
    void RemoveAll(IPredicate match)
    {
        for(size_t i = m_nCount; i > 0;)
        {
            if(match(m_pScan0 + --i))
            {
                RemoveAt(i);
            }
        }
    }
    /**
     * @brief �Ƴ� List<T> ��ָ����������Ԫ��
     * 
     * @param index 
     */
    void RemoveAt(int index)
    {
        LIST_DEBUG(index < m_nCount);
        m_policy.Free(m_pScan0 + idx);
        for(size_t i = idx; i < m_nCount; i++)
        {
            memcpy(m_pScan0 + idx++, m_pScan0 + idx, sizeof(T));
        }
        m_nCount--;
    }
    /**
     * @brief �� List<T> ���Ƴ�һϵ��Ԫ��
     * 
     * @param index 
     * @param count 
     */
    void RemoveRange(size_t index, size_t count)
    {
        LIST_DEBUG(index + count <= m_nCount);
        for(size_t i = 0; i < count; i++)
        {
            m_policy.Free(m_pScan0 + index + i);  
        }
        memmove(m_pScan0 + index, m_pScan0 + index + count, sizeof(T) * (m_nCount - index - count));/* memcpy ѭ���� memmove Ч�ʻ�δ��֤ */
        m_nCount -= count;
    }
    /**
     * @brief ������ List<T> ��Ԫ�ص�˳��ת
     * 
     */
    void Reverse()
    {
        VOIDRET_ASSERT(1 < m_nCount);
        T* begin = m_pScan0;
        T* end = m_pScan0 + m_nCount - 1;
        for(size_t i = 0; i < (m_nCount >> 1); i++, begin++, end--)
        {
            *begin ^= *end ^= *begin ^= *end;
        }
    }
    /**
     * @brief ��ָ����Χ��Ԫ�ص�˳��ת
     * 
     * @param index 
     * @param count 
     */
    void Reverse(size_t index, size_t count)
    {
        LIST_DEBUG(index + count <= m_nCount);
        VOIDRET_ASSERT(1 < m_nCount);
        T* begin = m_pScan0 + index;
        T* end = m_pScan0 + index + count;
        for(size_t i = index; i < (count >> 1); i++, begin++, end--)
        {
            *begin ^= *end ^= *begin ^= *end;
        }
    }
    /**
     * @brief ���������СС�ڻ����16��Ԫ�أ�������ʹ�ò��������㷨��
     * �������������2�� log n������ n ����������ķ�Χ����ʹ�� Heapsort �㷨����������ʹ�ÿ��������㷨��
     * 
     */
    void Sort(ICompare compare)
    {
        VOIDRET_ASSERT(1 < m_nCount);
        if(17 > m_nCount)/* https://www.cnblogs.com/nicaicai/p/12596565.html */
        {
            T* p = m_pScan0;
            T* val = *T*)malloc(sizeof(T));    
            for(int i = 1, index; i < (int)m_nCount; i++)
            {
                memcpy(val, p + i, sizeof(T));
                index = i - 1;
                while(0 <= index && 0 < compare(p + index, val))
                {
                    p[index + 1] = p[index];
                    memcpy(p + index + 1, p + index, sizeof(T));
                    index--;
                }    
                memcpy(p + index + 1, val, sizeof(T));
            }
            free(val);
            return;
        }
    }
    /**
     * @brief ����������Ϊ List<T> ��Ԫ�ص�ʵ����Ŀ���������ĿС��ĳ����ֵ��
     * ���û����Ԫ����ӵ������У������ʹ�ô˷������̶ȵؼ��ټ��ϵ��ڴ濪���� 
     * �����·���͸��ƽϴ�� List<T> �ɱ��ܴ���ˣ� TrimExcess ����б���������� 90% ( TRIM_EXCESS_THRESHOLD )���˷�������ִ���κβ���
     */
    void TrimExcess()
    {
        VOIDRET_ASSERT(m_nCount < m_nCapacity * TRIM_EXCESS_THRESHOLD);  
        void* p = malloc(sizeof(T) * m_nCount);
        memcpy(p, m_pScan0, sizeof(T) * m_nCount);
        free(m_pScan0);
        m_pScan0 = p;
        m_nCapacity = m_nCount;
    }
    /**
     * @brief ȷ�� List<T> �е�ÿ��Ԫ���Ƿ���ָ��ν�ʶ��������ƥ��
     * 
     * @param match 
     * @return true 
     * @return false 
     */
    bool TrueForAll(IPredicate match)
    {
        size_t i = 0;
        while(i < m_nCount && match(m_pScan0[i])
        {
            i++;
        }
        return i == m_nCount;
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
    List<T>* Clone()
    {
        List<T>* dest = new List<T>(capacity);
        dest.count = count;
        memcpy(dest.scan0, scan0, count * sizeof(T));
        return 
    }
};

// T Ϊ new ������ָ��
template<typename T>
class NewPtrListPolocy final
{
public:
    NewPtrListPolocy(){}
    /**
     * @brief src Ϊ new ������ָ���Ӧ List �Ŀ�������
     * 
     * @param dest 
     * @param src 
     */
    inline void Copy(T* dest, const T&  src)
    {
        *dest = src;
    }
    inline bool Equals(const T* cmp1, const T* cmp2)
    {
        return cmp1 == cmp2;
    }
    inline int Compare(const T* cmp1, const T* cmp2)
    {
        return cmp1 == cmp2;
    }
    /**
     * @brief src Ϊ new ������ָ���Ӧ List ���ͷŲ���
     * 
     * @param src 
     */
    inline void Free(T* src)
    {
        if(*src)
        {
            delete *src;
            *src = NULL;
        }
    }
    
};
// T Ϊ API ���������ָ��
template<typename T>
class MallocPtrListPolocy final
{
public:
    MallocPtrListPolocy(){}
    /**
     * @brief src Ϊ API ���������ָ���Ӧ List �Ŀ�������
     * 
     * @param dest 
     * @param src 
     */
    inline void Copy(T* dest, const T&  src)
    {
        *dest = src;
    }
    /**
     * @brief src Ϊ API ���������ָ���Ӧ List ���ͷŲ���
     * 
     * @param src 
     */
    inline void Free(T* src)
    {
        if(*src)
        {
            free(*src);
            *src = NULL;
        }
    }
};
/* T ΪԪ�صĽṹ */
template<typename T>
class StructerListPolocy final
{
public:
    StructerListPolocy(){}
    /**
     * @brief src Ϊ API ���������ָ���Ӧ List �Ŀ�������
     * 
     * @param dest 
     * @param src 
     */
    inline void Copy(T* dest, const T&  src)
    {
        memcpy(dest, &src, sizeof(T));
    }
    /**
     * @brief src Ϊ API ���������ָ���Ӧ List ���ͷŲ���
     * 
     * @param src 
     */
    inline void Free(T* src)
    {
    }
};
#endif