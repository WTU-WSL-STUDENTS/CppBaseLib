/*
 * @Description: Sqlite´ó¿Ó : Get* columnIndex´Ó0 - (n - 1), Set* columnIndex´Ó1 - n
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-27 20:09:57
 * @LastEditors: like
 * @LastEditTime: 2021-10-01 22:44:00
 */
#ifndef SYSTEM_DATA_SQLITE3_HPP
#define SYSTEM_DATA_SQLITE3_HPP

#include <System.Data.Sqlite3DbCommand.hpp>

#define SQLITE3_TRY_CREATE_TABLE(table, titledefs) "create table if not exists " ##table "(" ##titledefs ");"

System::Data::IDbCommand* System::Data::Sqlite3DbConnection::CreateCommand(const char* sql)
{
    return new System::Data::Sqlite3DbCommand(sql, this);
}

#endif
