/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-02 11:00:01
 * @LastEditors: like
 * @LastEditTime: 2022-04-02 11:51:19
 */
#ifndef SYSTEM_THREADING_CANCELLATION_TOKEN_HPP
#define SYSTEM_THREADING_CANCELLATION_TOKEN_HPP
#include <CompliedEntry.h>
#include <CompliedIndexer.h>
#include <System.Threading.ThreadPool.hpp>
#include <System.Threading.Timer.hpp>

namespace System::Threading
{
    class CancellationToken;
    class CancellationTokenSource;
};
class System::Threading::CancellationToken
{
	friend class CancellationTokenSource;
private:
    WEAK_PTR(readonly bool) m_pCancel;
public:
    CancellationToken(const bool &canceled) : m_pCancel(&canceled){}
    ~CancellationToken() 
    {
        m_pCancel = NULL;
    }
    DECLARE_CONST_GETTER(bool, CancellationRequested, { return *m_pCancel; })
    void ThrowIfCancellationRequested()
    {
        if (GetCancellationRequested())
        {
            throw std::exception("Receive cancel request");
        }
    }
};
class System::Threading::CancellationTokenSource
{
private:
    bool m_bCancel;
    CancellationToken* m_token;
public:
    CancellationTokenSource() : m_bCancel(false), m_token(new CancellationToken(m_bCancel)){}
    ~CancellationTokenSource()
    {
        ERROR_ASSERT(m_token, "Token has disposed unexpected");
        delete m_token;
        m_token = NULL;
    }
    /**
     * @brief 获取是否已请求取消此 CancellationTokenSource
     * 
     */
    DECLARE_CONST_GETTER(bool, CancellationRequested, { return m_bCancel; })
    /**
     * @brief 获取与此 CancellationTokenSource 关联的 CancellationToken
     * 
     */
    DECLARE_CONST_GETTER(WEAK_PTR(CancellationToken), Token, { return m_token; })
    /**
     * @brief 传达取消请求
     * 
     */
    void Cancel() 
    {
        m_bCancel = true;
    }
    void CancelAfter()
    {
        // timer = CreateThreadpoolTimer(timercallback,
        //                           NULL,
        //                           &CallBackEnviron);
    }
};
#endif