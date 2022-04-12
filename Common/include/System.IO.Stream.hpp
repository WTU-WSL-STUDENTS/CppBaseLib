/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-08 15:10:47
 * @LastEditors: like
 * @LastEditTime: 2022-02-24 17:10:14
 */
#ifndef SYSTEM_IO_STREAM_H
#define SYSTEM_IO_STREAM_H

#include <System.Threading.WaitHandle.hpp>
#include <System.Threading.Tasks.Task.hpp>

using Task = System::Threading::Tasks::Task;
using ValueTask = System::Threading::Tasks::ValueTask;

namespace System::IO
{
    enum SeekOrigin;
    class Stream;
}
enum System::IO::SeekOrigin
{
    Begin = 0,
    Current,
    End
};
class System::IO::Stream : public IDisposable//, public IAsyncDisposable
{
protected:
    int m_nReadTimeout;
    int m_nWriteTimeout;
    Stream(){}
    virtual void Dispose(bool disposing) = 0;
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
    virtual long GetPosition() = 0;
    /**
     * @brief 设置当前流中的位置
     * 
     * @param pos 
     * @return true 
     * @return false 
     */
    virtual bool SetPosition(long pos) =  0;
    virtual int ReadTimeout()
    {
        VALRET_ASSERT(CanTimeout(), -1);
        return m_nReadTimeout;
    }
    virtual int WriteTimeout()
    {
        VALRET_ASSERT(CanTimeout(), -1);
        return m_nWriteTimeout;
    }
    virtual Task* CopyToAsync(Stream& destination, size_t& bufferSize) = 0;
    void Dispose() override
    {
        Dispose(true);
    }
    virtual void Flush() = 0;
    /**
     * @brief 异步清除此流的所有缓冲区并导致所有缓冲数据都写入基础设备中
     * 
     * @return Task 
     */
    virtual Task* FlushAsync() = 0;
    /**
     * @brief 当在派生类中重写时，从当前流读取字节序列，并将此流中的位置提升读取的字节数
     * 
     * @param buffer 
     * @param count 
     */
    virtual size_t Read(char* buffer, size_t count) = 0;
    virtual size_t Read(char* buffer, long offset, size_t count) = 0;
    /**
     * @brief 从当前流异步读取字节序列，并将流中的位置提升读取的字节数
     * 
     * @param buffer 
     * @param offset 
     * @param count 
     * @return Task 
     */
    virtual Task* ReadAsync(char* buffer, long& offset, size_t& count) = 0;
    /**
     * @brief 从流中读取一个字节，并将流内的位置向前提升一个字节，或者如果已到达流结尾，则返回 -1
     * 
     * @return int 
     */
    virtual int ReadByte() = 0;
    /**
     * @brief 当在派生类中重写时，设置当前流中的位置
     * 
     * @param offset 
     * @param origin 
     * @return true 
     * @return false 
     */
    virtual bool Seek(long offset, SeekOrigin origin) = 0;
    /**
     * @brief 当在派生类中重写时，向当前流中写入字节序列，并将此流中的当前位置提升写入的字节数
     * 
     * @param buffer 
     * @param count 
     */
    virtual size_t Write(const char* buffer, size_t count) = 0;
    virtual size_t Write(const char* buffer, long offset, size_t count) = 0;
    /**
     * @brief 当在派生类中重写时，向当前流中写入字节序列，并将此流中的当前位置提升写入的字节数
     * 
     * @param buffer 
     * @param offset 
     * @param count 
     * @return Task 
     */
    virtual Task* WriteAsync(char* buffer, long& offset, size_t& count) = 0;
    /**
     * @brief 当在派生类中重写时，向当前流中写入字节序列，并将此流中的当前位置提升写入的字节数
     * 
     * @param value 
     */
    virtual void WriteByte(char value) = 0;  
};

#endif