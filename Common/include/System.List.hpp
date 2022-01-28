/*
 * @Description: 堆内存数据的 List 集合 , 栈对象引用为 0 会自动释放内存, 会影响 List 的管理
 * 优化方向 : 
 *  1. 优化虚函数表的查询, 设计一种用法取消虚函数表的查询.
 *  2. 将 List::Add 按照 policy-based class design , 将堆栈数据的 Add 策略拆分出来, 由 API caller 告诉 List 用那种策略, 实现栈对象 List 的适配(Add 时进行深拷贝)        
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
 * 1. 指向堆内存的指针
 *  优点
 *  - List 容量大
 *  缺点
 *  - 如果 Add 的指针指向栈内存, 会存在访问错误
 * 2. 堆栈内存结构
 *  优点
 *  - 数据排列整齐, 方便实现 Span 有利于 Cache 命中
 *  缺点
 *  堆栈数据都会以副本的形式存储到 List 中, 因此最大容量受 T 数据结构大小影响
 */
template<class T, template<class>class ListItemMemPolicy>
class System::List : IList<T>
{
private:
    T* m_pScan0;    /* 连续内存块 */
    ListItemMemPolicy<T> m_policy;
    size_t m_nCapacity; 
    size_t m_nCount;
    bool isReadOnly;
    /**
     * @brief 内存扩容
     * 
     */
    inline void MemoryGrowth(int nMemoryGrouthRate = LIST_MEMORY_GROUTH_RATE)
    {   
        LIST_DEBUG(LIST_COUNT_MAX == m_nCapacity);
        /* 确保 sizeof(T) * capacity 不溢出 */
        m_nCapacity = (LIST_COUNT_MAX >> nMemoryGrouthRate) < m_nCapacity ? LIST_COUNT_MAX : m_nCapacity << nMemoryGrouthRate;
        m_pScan0 = (T*)realloc(m_pScan0, sizeof(T) * m_nCapacity);
    }
    List(){}
public:
    /**
     * @brief 表示一种方法，该方法定义一组条件并确定指定对象是否符合这些条件
     * 
     */
    typedef std::function<bool(const T&)> IPredicate;
    /**
     * @brief 定义类型为比较两个对象而实现的方法
     * 
     */
    typedef std::function<int(const T&, const T&)> ICompare;

    /**
     * @brief 初始化容器, 该实例为空并且具有指定的初始容量
     * 
     * @param capacity 初始容量
     */
    List(int capacity = 8): m_nCapacity(capacity), m_pScan0((T*)malloc(sizeof(T) * m_nCapacity)), m_nCount(0), isReadOnly(false){}
    List(IEnumerable<T> ie/* T 如果是指针, 那么释放所有权归当前实例所有 */) : isReadOnly(false)
    {
        ICollection<T>* ic = (ICollection<T>*)&ie;
        m_nCount = ic->Count();
        m_nCapacity = m_nCount;
        m_pScan0 = (T*)malloc(sizeof(T) * m_nCapacity);
        ic->CopyTo(m_pScan0, 0);
    }
    /**
     * @brief 完整的释放内存有两个步骤 : 1. 遍历 IEnumerator 进行元素内存释放; 2. 调用 IEnumerator::Dispose()
     * 
     */
    ~List()
    {
        this->Clear();
        free(m_pScan0);
    }
    /**
     * @brief 末尾增加数据. O(1)
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
     * @brief 将指定集合的元素添加到 List<T> 的末尾
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
            LIST_DEBUG(LIST_COUNT_MAX == m_nCapacity || nLackCount + m_nCapacity < m_nCapacity/* 确保不溢出 */);
            capacity += nLackCount;
            m_pScan0 = (T*)realloc(m_pScan0, sizeof(T) * m_nCapacity);
            ic->CopyTo(m_pScan0 , m_nCount);
        }
        m_nCount += c;
    }
    /**
     * @brief 从 List<T> 中移除所有元素, List 申请的内存不会释放, 也不会清除
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
     * @brief 确定某元素是否在 List<T> 中。 O(n)
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
     * @brief 将 List<T> 或它的一部分复制到一个数组中
     * 
     * @param index List<T> 中从零开始的索引开始
     * @param array 一维数组
     * @param arrayIndex array 中从零开始的索引，从此处开始复制
     * @param count 要复制的元素数
     * @return true 
     * @return false 1.array 为 NULL; 2. 从 List<T> index 开始剩余数据不足 count 
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
     * @brief 确定 List<T> 是否包含与指定谓词定义的条件匹配的元素
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
     * @brief 搜索与指定谓词所定义的条件相匹配的元素，并返回整个 List<T> 中的第一个匹配元素
     * 
     * @param match 
     * @return T* 失败返回 NULL; 成功返回浅拷贝指针
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
     * @brief 搜索与指定谓词所定义的条件相匹配的元素，返回 List<T> 或它的一部分中第一个匹配项的从零开始的索引。 如果未找到与条件匹配的项，则此方法返回 -1
     * 
     * @param match 
     * @return size_t 失败返回 SIZE_MAX, 成功返回 index
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
     * @brief 搜索与指定谓词所定义的条件相匹配的元素，并返回整个 List<T> 中的最后一个匹配元素
     * 
     * @param match 
     * @return T* 失败返回 NULL; 成功返回浅拷贝指针
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
     * @brief 搜索与指定谓词所定义的条件相匹配的元素，返回 List<T> 或它的一部分中最后一个匹配项的从零开始的索引。 如果未找到与条件匹配的项，则此方法返回 -1
     * 
     * @param match 
     * @return size_t 失败返回 SIZE_MAX, 成功返回 index
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
     * @brief 获取迭代器
     * 
     * @return * IEnumerator<T>* 迭代器抽象接口
     */
    virtual IEnumerator<T>* GetEnumerator() override
    {
        return new ListEnumrator<T>(scan0, count);
    }
    /**
     * @brief 查找指定元素的位置 O(n)
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
     * @brief 将集合中的元素插入 List<T> 的指定索引处 O(n)
     * 
     * @param index 插入越靠前性能越差, 尾插性能同 Add
     * @param item 
     * @return * void 
     */
    virtual void Insert(size_t index, const T& item) override
    {
        LIST_DEBUG(index < m_nCount);
        if(m_nCount < m_nCapacity)
        {
            /*1. 整体后移; 2. 插入*/
            for(size_t i = m_nCount; i > index;)
            {
                memcpy(m_pScan0[i--], m_pScan0[i], sizeof(T));
            }
            m_policy.Copy(m_pScan0 + index, item);
        }
        else/* 尾插 */
        {
            MemoryGrowth();
            m_policy.Copy(m_pScan0 + m_nCount, item);
        }
        m_nCount++;
    }
    /**
     * @brief 如果 T 为指针类型, 释放的所有权归属于父 List<T>
     * 
     * @param index 应在此处插入新元素的从零开始的索引
     * @param ie 应将其元素插入到 List<T> 中的集合。 集合自身不能为 null，但它可以包含为 null 的元素（如果类型 T 为引用类型）
     */
    void InsertRange(int index, IEnumerable<T> ie)
    {
        LIST_DEBUG(index < m_nCount);
        ICollection<T>* ic = (ICollection<T>*)&ie;
        size_t c = ic->Count();
        if(m_nCount + c < m_nCapacity)
        {
            /*1. 整体后移; 2. 插入*/
            for(size_t i = m_nCount; i > index;)
            {
                memcpy(m_pScan0 + c + i--, m_pScan0 + i, sizeof(T));
            }
            ic->CopyTo(m_pScan0, index);
            m_nCount += c;
        }
        else/* 尾插 */
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
     * @brief 从 List<T> 中移除特定对象的第一个匹配项 O(n)
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
     * @brief 移除与指定的谓词所定义的条件相匹配的所有元素
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
     * @brief 移除 List<T> 的指定索引处的元素
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
     * @brief 从 List<T> 中移除一系列元素
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
        memmove(m_pScan0 + index, m_pScan0 + index + count, sizeof(T) * (m_nCount - index - count));/* memcpy 循环和 memmove 效率还未验证 */
        m_nCount -= count;
    }
    /**
     * @brief 将整个 List<T> 中元素的顺序反转
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
     * @brief 将指定范围中元素的顺序反转
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
     * @brief 如果分区大小小于或等于16个元素，则它将使用插入排序算法。
     * 如果分区数超过2个 log n，其中 n 是输入数组的范围，则使用 Heapsort 算法。否则，它将使用快速排序算法。
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
     * @brief 将容量设置为 List<T> 中元素的实际数目（如果该数目小于某个阈值）
     * 如果没有新元素添加到集合中，则可以使用此方法最大程度地减少集合的内存开销。 
     * 但重新分配和复制较大的 List<T> 成本很大，因此， TrimExcess 如果列表的容量超过 90% ( TRIM_EXCESS_THRESHOLD )，此方法不会执行任何操作
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
     * @brief 确定 List<T> 中的每个元素是否都与指定谓词定义的条件匹配
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
    List<T>* Clone()
    {
        List<T>* dest = new List<T>(capacity);
        dest.count = count;
        memcpy(dest.scan0, scan0, count * sizeof(T));
        return 
    }
};

// T 为 new 出来的指针
template<typename T>
class NewPtrListPolocy final
{
public:
    NewPtrListPolocy(){}
    /**
     * @brief src 为 new 出来的指针对应 List 的拷贝策略
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
     * @brief src 为 new 出来的指针对应 List 的释放策略
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
// T 为 API 分配出来的指针
template<typename T>
class MallocPtrListPolocy final
{
public:
    MallocPtrListPolocy(){}
    /**
     * @brief src 为 API 分配出来的指针对应 List 的拷贝策略
     * 
     * @param dest 
     * @param src 
     */
    inline void Copy(T* dest, const T&  src)
    {
        *dest = src;
    }
    /**
     * @brief src 为 API 分配出来的指针对应 List 的释放策略
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
/* T 为元素的结构 */
template<typename T>
class StructerListPolocy final
{
public:
    StructerListPolocy(){}
    /**
     * @brief src 为 API 分配出来的指针对应 List 的拷贝策略
     * 
     * @param dest 
     * @param src 
     */
    inline void Copy(T* dest, const T&  src)
    {
        memcpy(dest, &src, sizeof(T));
    }
    /**
     * @brief src 为 API 分配出来的指针对应 List 的释放策略
     * 
     * @param src 
     */
    inline void Free(T* src)
    {
    }
};
#endif