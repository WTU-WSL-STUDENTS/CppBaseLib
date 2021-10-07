/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-28 19:53:57
 * @LastEditors: like
 * @LastEditTime: 2021-10-01 22:00:53
 */
#ifndef SYSTEM_DATA_IDATARECORD_HPP
#define SYSTEM_DATA_IDATARECORD_HPP

#include <CompliedEntry.h>
#include <System.DateTime.hpp>
#define SYSTEM_DATA_IDATARECORD_ERROR {throw "Function Not Override";}

#define VIRTUAL_DECLARE_SQL_GET_FUNCTION(name, type) virtual type Get##name(int i)SYSTEM_DATA_IDATARECORD_ERROR 
#define VIRTUAL_DECLARE_SQL_SET_FUNCTION(name, type) virtual bool Set##name(int i, type in)SYSTEM_DATA_IDATARECORD_ERROR 
#define VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(name, type)\
VIRTUAL_DECLARE_SQL_GET_FUNCTION(name, type)            \
VIRTUAL_DECLARE_SQL_SET_FUNCTION(name, type)
  
#define OVERRIDE_DECLARE_SQL_GET_FUNCTION(name, type , GetRealize) type Get##name(int i)override{GetRealize}  
#define OVERRIDE_DECLARE_SQL_SET_FUNCTION(name, type, SetRealize)  bool Set##name(int i, type in)override{SetRealize}
#define OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION(name, type , GetRealize, SetRealize)  \
type Get##name(int i)override{GetRealize}                                           \
bool Set##name(int i, type in)override{SetRealize}
namespace System::Data
{
    class IDataRecord;
    class IDataReadWriter;
}
using namespace System;

class System::Data::IDataRecord
{
private:
    template<typename T>
    int GetRow(int colIndex, T& t)
    {
        return Get<T>(colIndex, t) ? colIndex : --colIndex;
    }     
    template<typename T>
    int SetRow(int colIndex, const T& t)
    {
        return Set<T>(colIndex++, t) ? colIndex : --colIndex;
    }    
protected:
    IDataRecord(){}
public:
    /* �����޷���д�����ģ�庯��,��û�ҷ���ʵ�� */
    // /**
    //  * @description: ��ȡָ����ŵĲ���
    //  * @param {int} columnIndex
    //  * @param {const} T
    //  * @return {*}
    //  * @author: like
    //  */    
    // template<typename T>
    // bool Get(int columnIndex, T& out) SYSTEM_DATA_IDATARECORD_ERROR
    // /**
    //  * @description: ��ָ�����д�����
    //  * @param {int} columnIndex
    //  * @param {const} T
    //  * @return {*}
    //  * @author: like
    //  */    
    // template<typename T>
    // bool Set(int columnIndex, const T& in)SYSTEM_DATA_IDATARECORD_ERROR

    VIRTUAL_DECLARE_SQL_GET_FUNCTION(Name, const char*);
    VIRTUAL_DECLARE_SQL_GET_FUNCTION(Type, int);
    VIRTUAL_DECLARE_SQL_SET_FUNCTION(Null, void*);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Boolean,   bool);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Byte,      unsigned char);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Bytes,     unsigned char*);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Char,      char);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Chars,     char*);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(DateTime,  DateTime);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Double,    double);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Float,     float);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Int16,     Int16);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Int32,     int);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Int64,     long long);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Text,      const char*);
    VIRTUAL_DECLARE_SQL_GET_SET_FUNCTION(Length,    int);/* ָ�볤�� */

    
    /**
     * @description: ͨ���ݹ�ķ�ʽ��ȡһ������
     * @param {�ݹ���ʼ����} T
     * @param {�ݹ�ʣ�����} TRest
     * @return {���������,1.���ݶ�ȡ����;2.���ݶ�ȡʧ��}
     * @author: like
     */   
    template<typename T, typename... TRest>  
    int GetRow(int colBegIndex,T& t, TRest&...)
    {
        if(Get<T>(colBegIndex))
        {
            return GetRow<TRest>(++colBegIndex, ...);
        }
        return colBegIndex;
    }      
    /**
     * @description: ����ʼ��ſ�ʼ,д��һ������ 
     * @param {���}} colBegIndex
     * @param {�ݹ���ʼ����} T
     * @param {�ݹ�ʣ�����} TRest
     * @return {���������,1.����д������;2.����д��ʧ��}
     * @author: like
     */    
    template<typename T, typename... TRest>  
    int SetRow(int colBegIndex,const T& t, const TRest&...)
    {
        if(Set<T>(colBegIndex++, t))
        {
            return SetRow<TRest>(colBegIndex, ...);
        }
        return colBegIndex;
    }                                   
};

class System::Data::IDataReadWriter : public System::Data::IDataRecord
{
public:
    /**
     * @description: ��ȡ��һ������
     * @param {*}
     * @return {�Ƿ��ȡ�ɹ�}
     * @author: like
     */    
    virtual bool Read() SYSTEM_DATA_IDATARECORD_ERROR
    virtual bool Write() SYSTEM_DATA_IDATARECORD_ERROR
    /**
     * @description: �ͷŵ�ǰsql��� 
     * @param {*}
     * @return {*}
     * @author: like
     */    
    virtual void Close() SYSTEM_DATA_IDATARECORD_ERROR
};

class IDataParameter
{

};
class IDataParameterConnection
{

};
#endif