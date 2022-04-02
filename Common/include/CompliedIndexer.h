/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-03 21:01:51
 * @LastEditors: like
 * @LastEditTime: 2022-03-28 16:11:51
 */
#ifndef COMPLIED_INDEXER_H
#define COMPLIED_INDEXER_H

#define ENABLE_GETTER_INLINE
#define ENABLE_SETTER_INLINE

/**
 * @brief get
 * 
 */
#ifdef ENABLE_GETTER_INLINE
#   define INLINE_GETTERT inline
#else
#   define INLINE_GETTERT
#endif
#define DECLARE_READONLY_REF_GETTER(type, name, codeblock)  INLINE_GETTERT const type&  Get##name() const   codeblock
#define DECLARE_CONST_GETTER(type, name, codeblock)         INLINE_GETTERT type         Get##name() const   codeblock
#define DECLARE_REF_GETTER(type, name, codeblock)           INLINE_GETTERT type&        Get##name()         codeblock
#define DECLARE_GETTER(type, name, codeblock)               INLINE_GETTERT type         Get##name()         codeblock
/**
 * @brief set
 * 
 */
#ifdef ENABLE_SETTER_INLINE
#   define INLINE_SETTER inline
#else
#   define INLINE_SETTER
#endif
#define SETTER_VALUE Value
#define DECLARE_SETTER(type, name, codeblock) INLINE_SETTER void Set##name(const type& SETTER_VALUE)         codeblock
/**
 * @brief 索引器定义 get & set
 * 
 */
#define DECLARE_INDEXER(type, name, getterCodeblock, setterCodeblock)   \
    DECLARE_CONST_GETTER(type, name, getterCodeblock)                   \
    DECLARE_SETTER(type, name, setterCodeblock)

/**
 * @brief 定义私有对象 m_##name, 并声明其索引器 
 * 
 */
#define DECLARE_DATAWRAPPER_INDEXER(type, name) \
private:                                        \
    type m_##name{};                            \
public:                                         \
    DECLARE_INDEXER(type, name, {return m_##name;}, {m_##name = SETTER_VALUE;})

#endif