/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-25 11:39:01
 * @LastEditors: like
 * @LastEditTime: 2021-10-01 22:33:24
 */
#ifndef SYSTEM_SQL_SQLITE3_DB_CONNECTION_HPP
#define SYSTEM_SQL_SQLITE3_DB_CONNECTION_HPP
#ifndef SYSTEM_DATA_SQLITE3_HPP
#error 若要包含System.Data.Sqlite3DbConnection.hpp , 用System.Data.Sqlite3.hpp替代
#endif
#include <System.Data.IDbConnection.hpp>
extern "C"
{
    #include <sqlite3.h> /* https://sqlite.org/cintro.html */
}
/*
    SQLITE_OPEN_READONLY
    The database is opened in read-only mode. If the database does not already exist, an error is returned.
    SQLITE_OPEN_READWRITE
    The database is opened for reading and writing if possible, or reading only if the file is write protected by the operating system. In either case the database must already exist, otherwise an error is returned.
    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE
    The database is opened for reading and writing, and is created if it does not already exist. This is the behavior that is always used for sqlite3_open() and sqlite3_open16().
    In addition to the required flags, the following optional flags are also supported:

    SQLITE_OPEN_URI
    The filename can be interpreted as a URI if this flag is set.
    SQLITE_OPEN_MEMORY
    The database will be opened as an in-memory database. The database is named by the "filename" argument for the purposes of cache-sharing, if shared cache mode is enabled, but the "filename" is otherwise ignored.
    SQLITE_OPEN_NOMUTEX
    The new database connection will use the "multi-thread" threading mode. This means that separate threads are allowed to use SQLite at the same time, as long as each thread is using a different database connection.
    SQLITE_OPEN_FULLMUTEX
    The new database connection will use the "serialized" threading mode. This means the multiple threads can safely attempt to use the same database connection at the same time. (Mutexes will block any actual concurrency, but in this mode there is no harm in trying.)
    SQLITE_OPEN_SHAREDCACHE
    The database is opened shared cache enabled, overriding the default shared cache setting provided by sqlite3_enable_shared_cache().
    SQLITE_OPEN_PRIVATECACHE
    The database is opened shared cache disabled, overriding the default shared cache setting provided by sqlite3_enable_shared_cache().

    SQLITE_OPEN_NOFOLLOW
    The database filename is not allowed to be a symbolic link
*/
#define sqlite3_open  sqlite3_open_v2
#define sqlite3_close sqlite3_close_v2
#define sqlite3_prepare sqlite3_prepare_v2

namespace System::Data
{
    class Sqlite3DbConnection : public IDbConnection
    {
    friend class Sqlite3DbCommand;
    private:
        sqlite3* p;
        int flag;
    public:
        Sqlite3DbConnection(const char* dbName, int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE) : IDbConnection(dbName), flag(flags){}
        void Open() override /* 目前只支持UTF8编码数据库 */
        {
            int retCode = sqlite3_open(dbName, &p, flag, NULL);
            if(SQLITE_OK == retCode)
            {
                state = ConnectionState::Open;
                return;
            }
            printf("Open Sqlite3 Database Failed , Error Code %d\n", retCode);
            sqlite3_close(p);
        }
        void Close() override
        {
            if(ConnectionState::Closed != state)
            {
                state = ConnectionState::Closed;
                sqlite3_close(p);
            }
        }
        IDbCommand* CreateCommand(const char* sql) override;/* 函数在System.Data.Sqlite3.hpp中实现 */
    };
};
#endif