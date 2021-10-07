/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-26 20:37:27
 * @LastEditors: like
 * @LastEditTime: 2021-10-01 22:33:08
 */

#include <System.Data.Sqlite3.hpp>

using namespace System::Data;
bool hasTitlePrinted = false;
int PrintRow(void* param, int columnCount, char** args, char** title)
{
    char buffer[1024] = {0};
    if(!hasTitlePrinted)
    {
        sprintf(buffer, "%10s%10s%10s%10s\n", title[0], title[1], title[2], title[3]);
        printf("%s", buffer);
    }
    sprintf("%10s%10s%10s%10s\n", args[0], args[1], args[2], args[3]);
    printf("%s", buffer);
    return 0;
}
typedef struct _Information
{
    const char* name;
    int age;
    char address[50];
    double salary;
}Information;
#define TABLE_NAME "main.Information"
#define SQLITE3_TITLE \
"ID INT PRIMARY KEY     NOT NULL,   "   \
"NAME           TEXT    NOT NULL,   "   \
"AGE            INT     NOT NULL,   "   \
"ADDRESS        CHAR(50),           "   \
"SALARY         REAL                "
int main()
{
    Information info[] = 
    {
        {"name1", 15, "��ɽ��", 100.1},     /* ��c�ӿ��п���������Ӣ�ģ�������̨��֧��Ӣ�� */
        {"name2", 16, "�����", 100.01},
        {"name3", 17, "������", 100.001},
        {"name4", 18, "��ɽ��", 100.0001},
        {"name5", 19, "������", 100.00001},
        {"name6", 20, "qiaokou", 100.000001},
        {"name7", 21, "������", 1000}
    };
    const char* selectAll = "select * from Information;";

    Sqlite3DbConnection conn("hello.db");/* �����Զ�дȨ�޴�,������Ҫ�ɶ�Ȩ��,����ļ��������򴴽����ݿ� */
    conn.Open();
    if(ConnectionState::Open != conn.GetState())
    {
        printf("�����ݿ�ʧ��\n");
        return 1;
    }
    /* �������ݿ� , Ĭ����һ��main*/
    IDbCommand* cmd = conn.CreateCommand(SQLITE3_TRY_CREATE_TABLE(TABLE_NAME, SQLITE3_TITLE));
    printf("Create Table : %d\n" , cmd->ExecuteNonQuery());
    cmd->Dispose();
    
    /* ��ѯ���� */
    int rowCount = 0;
    cmd = conn.CreateCommand(selectAll);
    printf("Select Row : %d\n", (rowCount = cmd->ExecuteNonQuery()));
    cmd->Dispose();
    
    /* �������� */
    if(rowCount < sizeof(info)/sizeof(Information))
    {
        char buffer[1024] = {0};
        int i = rowCount;
        sprintf(buffer, "insert into " TABLE_NAME " values ( %d, '%s', %d, '%s', %lf);", ++rowCount, info[i].name, info[i].age, info[i].address, info[i].salary);
        cmd = conn.CreateCommand(buffer);
        printf("Insert : %d\n", cmd->ExecuteNonQuery());
        cmd->Dispose();
    }
    else
    {
        printf("####%d\n", sizeof(info)/sizeof(Information));
        /* ��Ч����or�޸� */
        cmd = conn.CreateCommand("insert into " TABLE_NAME " values (?,?,?,?,?);");
        if(!cmd->BeginAffairs())
        {
            printf("Begin Affairs Failed \n");
        }
        auto *reader = cmd->ExecuteReader();
        if(NULL == reader)
        {
            printf("Execute Reader Failed \n");
            return 1;
        }
        for(int i = 0; i < sizeof(info)/sizeof(Information); i++)
        {
            printf("%d,%d,%d,%d,%d\n", 
                reader->SetInt32(1, ++rowCount),
                reader->SetText (2, info[i].name),
                reader->SetInt32(3, info[i].age),
                reader->SetText (4, info[i].address),
                reader->SetDouble(5, info[i].salary)
            );
            reader->Read();
            reader->Write();
        }
        if(!cmd->CommitAffairs())
        {
            printf("Commit Affairs Failed \n");
        }
        reader->Close();
        delete reader;
        reader = NULL;
    }

    /* ��Чselect */
    cmd = conn.CreateCommand(selectAll);
    auto *reader = cmd->ExecuteReader();
    if(NULL == reader)
    {
        printf("Execute Reader Failed \n");
        return 1;
    }
    if(!reader->Read())
    {
        printf("Read Failed \n");
        return 1;
    }
    printf("%s, %s, %s, %s, %s\n",
        reader->GetName(0),
        reader->GetName(1),
        reader->GetName(2),
        reader->GetName(3),
        reader->GetName(4)
    );
    

    do
    {
        printf("%d, %s, %d, %s, %lf\n", reader->GetInt32(0), reader->GetText(1), reader->GetInt32(2), reader->GetText(3), reader->GetDouble(4));
    }while(reader->Read());
    reader->Close();
    delete reader;
    reader = NULL;
    conn.Close();
    return 0;
}