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
        SqlCommandText = 0,     /* sql命令 */
        StoredProcedure,    /* 存储过程名字 */
        TableName           /* 表名 */
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
         * @description: 执行非查询sql指令并且返回影响的行数 
         * @param {*}
         * @return {返回的行数, -1代表失败} 
         * @author: like
         */        
        virtual int ExecuteNonQuery()           SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
        virtual IDataReadWriter* ExecuteReader()SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
        virtual void Dispose(){}
        /**
         * @description: 开始事务 
         * @param {*}
         * @return {*}
         * @author: like
         */        
        virtual bool BeginAffairs()             SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
        /**
         * @description: 合并数据库修改 
         * @param {*}
         * @return {*}
         * @author: like
         */        
        virtual bool CommitAffairs()             SYSTEM_DATA_IDBCOMMAND_OVERRIDE_ERRO
    };
    enum ConnectionState
    {
        Closed      = 0,    /* 连接已关闭 */
        Open        = 1,    /* 连接已打开 */
        Connecting  = 2,    /* 连接对象正在与数据源连接 */
        Executing   = 4,    /* 连接对象正在执行命令 , unused */
        Fetching    = 8     /* 连接对象正在检索数据 , unused */
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