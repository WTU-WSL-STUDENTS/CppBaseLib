/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-08 15:10:47
 * @LastEditors: like
 * @LastEditTime: 2022-02-11 20:41:34
 */
#ifndef SYSTEM_IO_STREAM_H
#define SYSTEM_IO_STREAM_H

#include <System.IDisposable.h>

namespace System::IO
{
    class Stream;
}
class System::IO::Stream : public IDisposable
{
protected:
    int nCurrentPos;
    int nReadTimeout;
    int nWriteTimeout;
    Stream(){}
public:
    /**
     * @brief 当在派生类中重写时，获取指示当前流是否支持读取
     * 
     * @return true 
     * @return false 
     */
    virtual bool CanRead()
    {
        return true;
    }
    /**
     * @brief 当在派生类中重写时，获取指示当前流是否支持写入功能
     * 
     * @return true 
     * @return false 
     */
    virtual bool CanWrite()
    {
        return true;
    }
    /**
     * @brief 当在派生类中重写时，获取指示当前流是否支持查找功能
     * 
     * @return true 
     * @return false 
     */
    virtual bool CanSeek()
    {
        return true;
    }
    /**
     * @brief 当在派生类中重写时，获取指示当前流是否可以超时
     * 
     * @return true 某些流实现需要不同的行为，例如 NetworkStream ，如果网络连接中断或丢失，会超时
     * @return false 
     */
    virtual bool CanTimeout()
    {
        return false;
    }
    virtual void Dispose(){}
    /**
     * @brief 获取流字节长度
     * 
     * @return size_t 
     */
    virtual size_t Length() = 0;
    /**
     * @brief 获取当前流中的位置
     * 
     * @return int 
     */
    virtual int GetPosition()
    {
        VALRET_ASSERT(CanSeek(), -1);
        return nCurrentPos;
    }
    /**
     * @brief 设置当前流中的位置
     * 
     * @param pos 
     * @return true 
     * @return false 
     */
    virtual bool SetPosition(int pos)
    {
        VALRET_ASSERT(CanSeek(), false);
        nCurrentPos = pos;
        return true;
    }
    virtual int ReadTimeout()
    {
        VALRET_ASSERT(CanTimeout(), -1);
        return nReadTimeout;
    }
    virtual int WriteTimeout()
    {
        VALRET_ASSERT(CanTimeout(), -1);
        return nWriteTimeout;
    }
};

#endif