/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-25 16:55:11
 * @LastEditors: like
 * @LastEditTime: 2021-10-01 21:52:27
 */
#ifndef SYSTEM_DATA_IDBCONNECTION_HPP
#define SYSTEM_DATA_IDBCONNECTION_HPP

#include <System.Data.IDataRecord.hpp>
#include <stdio.h>
#include <string.h>

#define SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO {throw "Interface : IDbCommand::ExecuteNonQuery Not Overload";}

namespace System::Data
{
    enum CommandType 
    {
        SqlCommandText = 0,     /* sql���� */
        StoredProcedure,    /* �洢�������� */
        TableName           /* ���� */
    };
    class IDbConnection;
    class IDbCommand
    {
    protected:
        const char* commandText;
        CommandType type;
        IDbConnection* connection;
        IDbCommand(const char* command, IDbConnection* connect) : type(CommandType::SqlCommandText), commandText(command), connection(connect){}
    public:
        IDbCommand(){}
        ~IDbCommand(){Dispose();}
        const char* CommandText(){return commandText;}
        /**
         * @description: ִ�зǲ�ѯsqlָ��ҷ���Ӱ������� 
         * @param {*}
         * @return {���ص�����, -1����ʧ��} 
         * @author: like
         */        
        virtual int ExecuteNonQuery()           SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
        virtual IDataReadWriter* ExecuteReader()SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
        virtual void Dispose(){}
        /**
         * @description: ��ʼ���� 
         * @param {*}
         * @return {*}
         * @author: like
         */        
        virtual bool BeginAffairs()             SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
        /**
         * @description: �ϲ����ݿ��޸� 
         * @param {*}
         * @return {*}
         * @author: like
         */        
        virtual bool CommitAffairs()             SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
    };
    enum ConnectionState
    {
        Closed      = 0,    /* �����ѹر� */
        Open        = 1,    /* �����Ѵ� */
        Connecting  = 2,    /* ���Ӷ�������������Դ���� */
        Executing   = 4,    /* ���Ӷ�������ִ������ , unused */
        Fetching    = 8     /* ���Ӷ������ڼ������� , unused */
    };
    class IDbConnection
    {
    protected:
        ConnectionState state;
        const char* dbName;
        IDbConnection(const char* dataBaseName) : state(ConnectionState::Closed), dbName(dataBaseName){}
    public:
        ConnectionState GetState(){return state;}
        const char* GetDbName(){return dbName;}
        virtual void Open(){}
        virtual void Close(){}
        virtual IDbCommand* CreateCommand(const char* sql){return NULL;}
    };
}

#endif