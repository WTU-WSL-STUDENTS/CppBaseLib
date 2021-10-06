/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-25 11:39:01
 * @LastEditors: like
 * @LastEditTime: 2021-10-01 22:20:44
 */
#ifndef SYSTEM_SQL_SQLITE3_DB_COMMAND_HPP
#define SYSTEM_SQL_SQLITE3_DB_COMMAND_HPP
#include <System.Data.Sqlite3DbConnection.hpp>
#include <map>

#define SQLITE3_DB_COMMAND_NOT_SUPPORT {throw "Sqlite3 Not Support This";} 

extern "C"
{
    #include <sqlite3.h> /* https://sqlite.org/cintro.html */
}
namespace System::Data
{
    class Sqlite3DbCommand;
    class Sqlite3DataReadWriter;
    typedef void (*DisposeBlob)(void*);
    typedef std::map<int, int> RowSizeMap;
    typedef std::map<int, void (*)(void*)> BlobDisposeMap;
};
class System::Data::Sqlite3DataReadWriter : public System::Data::IDataReadWriter
{
    friend class Sqlite3DbCommand;
private:
    sqlite3_stmt* stmt;
    Sqlite3DataReadWriter(sqlite3_stmt* sqlite3_stmt) : stmt(sqlite3_stmt){}
public:
    RowSizeMap rowSizeMap;
    BlobDisposeMap blobDisposeMap;
    bool Read() override
    {
        return SQLITE_ROW == sqlite3_step(stmt);
    }
    bool Write() override
    {
       return SQLITE_ROW == sqlite3_reset(stmt);  
    }
    void Close() override
    {
        sqlite3_finalize(stmt);
    }
    /* https://www.cnblogs.com/earthtosky/p/10528756.html */
    // OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION
    // (Boolean,   bool);
    // OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION(Byte,      unsigned char);
    OVERRIDE_DECLARE_SQL_GET_FUNCTION
    (
        Name                                ,
        const char*                         ,
        return sqlite3_column_name(stmt, i) ;
    )
    OVERRIDE_DECLARE_SQL_GET_FUNCTION
    (
        Type                                ,
        int                                 ,
        return sqlite3_column_type(stmt, i) ;
    )
    OVERRIDE_DECLARE_SQL_SET_FUNCTION
    (
        Null                                            ,
        void*                                           ,
        return SQLITE_OK == sqlite3_bind_null(stmt, i)  ;
    )
    OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION
    (
        Bytes                                                                                   , 
        unsigned char*                                                                          , 
        return (unsigned char*)sqlite3_column_blob(stmt, i);                                    , 
        return SQLITE_OK == sqlite3_bind_blob(stmt, i, &in, rowSizeMap[i], blobDisposeMap[i])   ;
    )
    OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION
    (
        Double                                              ,  
        double                                              , 
        return sqlite3_column_double(stmt, i);              , 
        return SQLITE_OK == sqlite3_bind_double(stmt, i, in);
    )
    OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION
    (
        Int32                                               , 
        int                                                 , 
        return sqlite3_column_int(stmt, i);                 , 
        return SQLITE_OK == sqlite3_bind_int(stmt, i, in)   ;
    )
    OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION
    (
        Int64                                               , 
        long long                                           , 
        return sqlite3_column_int64(stmt, i);               , 
        return SQLITE_OK == sqlite3_bind_int64(stmt, i, in) ;
    )
    OVERRIDE_DECLARE_SQL_GET_SET_FUNCTION
    (
        Text                                                                            , 
        const char*                                                                , 
        return (const char*)sqlite3_column_text(stmt, i);                               , //sqlite3_column_name
        return SQLITE_OK == sqlite3_bind_text(stmt, i, in, strlen(in), SQLITE_STATIC)  ;
    )
    OVERRIDE_DECLARE_SQL_GET_FUNCTION
    (
        Length                              , 
        int                                 , 
        return sqlite3_column_bytes(stmt, i);                                                    
    )
};
class System::Data::Sqlite3DbCommand : public System::Data::IDbCommand
{
private:
    sqlite3* p;
    sqlite3_callback execNonQueryCallBack;
    int nonQueryCallbackCount;
    /**
     * @description: ExecuteNonQuery每捕获一行，会触发依次回调函数
     * @param {void*} param
     * @param {一行元素的个数} int
     * @param {元素首地址} char**
     * @param {标题} char**
     * @return {执行成功返回0}
     * @author: like
     */    
    static int ExecuteNonQueryCallback(void* param, int argc, char** args, char** title)
    {
        Sqlite3DbCommand* p = (Sqlite3DbCommand*)param;
        p->nonQueryCallbackCount++;
        if(NULL != p->execNonQueryCallBack)
        {
            return (*p->execNonQueryCallBack)(param, argc, args, title);
        }
        return SQLITE_OK;
    }
public:
    Sqlite3DbCommand(const char* command, Sqlite3DbConnection* connection) : 
        IDbCommand(command, (IDbConnection*)connection), 
        p(connection->p),
        execNonQueryCallBack(NULL){}
    ~Sqlite3DbCommand()
    {
        Dispose();
    }
    int ExecuteNonQuery() override 
    {
        char* strError;
        nonQueryCallbackCount = 0;
        int retCode = sqlite3_exec(p, commandText, ExecuteNonQueryCallback, this, &strError);
        if(SQLITE_OK == retCode)
        {
            return nonQueryCallbackCount;
        }
        printf("Execute Sqlite3 Sql Failed : %s, Error Code %d , %s\n", commandText, retCode, strError);
        return -1;
    }
    IDataReadWriter* ExecuteReader() override
    {
        sqlite3_stmt* stmt = NULL;
        int retCode = sqlite3_prepare_v2(p, commandText, strlen(commandText), &stmt, NULL);
        
        if(SQLITE_OK == retCode)
        {
            return new Sqlite3DataReadWriter(stmt);
        }
        printf("ExecuteReader Failed , Sql : %s, Error Code : %d\n", commandText, retCode);
        return NULL;
    }
    bool BeginAffairs() override
    {
        return SQLITE_OK == sqlite3_exec(p, "begin;",0,0,0);
    }
    bool CommitAffairs() override
    {
        return SQLITE_OK == sqlite3_exec(p, "commit;",0,0,0);
    }
    void SetExecuteNonQueryCallback(sqlite3_callback callBack)
    {
        this->execNonQueryCallBack = callBack;
    }
};
#endif