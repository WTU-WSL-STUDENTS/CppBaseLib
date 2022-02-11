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
    virtual void Dispose(){}
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
    virtual int GetPosition()
    {
        VALRET_ASSERT(CanSeek(), -1);
        return nCurrentPos;
    }
    /**
     * @brief ���õ�ǰ���е�λ��
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