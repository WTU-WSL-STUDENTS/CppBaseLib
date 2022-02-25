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
     * @brief ��������������дʱ����ȡָʾ��ǰ���Ƿ�֧�ֶ�ȡ
     * 
     * @return true 
     * @return false 
     */
    virtual bool CanRead()
    {
        return true;
    }
    /**
     * @brief ��������������дʱ����ȡָʾ��ǰ���Ƿ�֧��д�빦��
     * 
     * @return true 
     * @return false 
     */
    virtual bool CanWrite()
    {
        return true;
    }
    /**
     * @brief ��������������дʱ����ȡָʾ��ǰ���Ƿ�֧�ֲ��ҹ���
     * 
     * @return true 
     * @return false 
     */
    virtual bool CanSeek()
    {
        return true;
    }
    /**
     * @brief ��������������дʱ����ȡָʾ��ǰ���Ƿ���Գ�ʱ
     * 
     * @return true ĳЩ��ʵ����Ҫ��ͬ����Ϊ������ NetworkStream ��������������жϻ�ʧ���ᳬʱ
     * @return false 
     */
    virtual bool CanTimeout()
    {
        return false;
    }
    /**
     * @brief ��ȡ���ֽڳ���
     * 
     * @return size_t 
     */
    virtual size_t Length() = 0;
    /**
     * @brief ��ȡ��ǰ���е�λ��
     * 
     * @return int 
     */
    virtual long GetPosition() = 0;
    /**
     * @brief ���õ�ǰ���е�λ��
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
     * @brief �첽������������л��������������л������ݶ�д������豸��
     * 
     * @return Task 
     */
    virtual Task* FlushAsync() = 0;
    /**
     * @brief ��������������дʱ���ӵ�ǰ����ȡ�ֽ����У����������е�λ��������ȡ���ֽ���
     * 
     * @param buffer 
     * @param count 
     */
    virtual size_t Read(char* buffer, size_t count) = 0;
    virtual size_t Read(char* buffer, long offset, size_t count) = 0;
    /**
     * @brief �ӵ�ǰ���첽��ȡ�ֽ����У��������е�λ��������ȡ���ֽ���
     * 
     * @param buffer 
     * @param offset 
     * @param count 
     * @return Task 
     */
    virtual Task* ReadAsync(char* buffer, long& offset, size_t& count) = 0;
    /**
     * @brief �����ж�ȡһ���ֽڣ��������ڵ�λ����ǰ����һ���ֽڣ���������ѵ�������β���򷵻� -1
     * 
     * @return int 
     */
    virtual int ReadByte() = 0;
    /**
     * @brief ��������������дʱ�����õ�ǰ���е�λ��
     * 
     * @param offset 
     * @param origin 
     * @return true 
     * @return false 
     */
    virtual bool Seek(long offset, SeekOrigin origin) = 0;
    /**
     * @brief ��������������дʱ����ǰ����д���ֽ����У����������еĵ�ǰλ������д����ֽ���
     * 
     * @param buffer 
     * @param count 
     */
    virtual size_t Write(const char* buffer, size_t count) = 0;
    virtual size_t Write(const char* buffer, long offset, size_t count) = 0;
    /**
     * @brief ��������������дʱ����ǰ����д���ֽ����У����������еĵ�ǰλ������д����ֽ���
     * 
     * @param buffer 
     * @param offset 
     * @param count 
     * @return Task 
     */
    virtual Task* WriteAsync(char* buffer, long& offset, size_t& count) = 0;
    /**
     * @brief ��������������дʱ����ǰ����д���ֽ����У����������еĵ�ǰλ������д����ֽ���
     * 
     * @param value 
     */
    virtual void WriteByte(char value) = 0;  
};

#endif