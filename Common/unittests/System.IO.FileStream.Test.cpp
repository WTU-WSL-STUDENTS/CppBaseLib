/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-08 15:10:49
 * @LastEditors: like
 * @LastEditTime: 2022-02-24 16:53:29
 */
#include <System.IO.FileStream.hpp>
#include <System.IO.File.hpp>
#include <iostream>
#include <string>

#define PRINTF_TEST_BOOL(str) printf(#str" : %s\n", str ? "true" : "false")
#define PRINTF_TEST_CHARS(str) printf(#str" : %s\n", str)

using namespace std;
using namespace System;
using namespace System::IO;
const char* strPath = "write.txt";
const char* strCopyPath = "write_copy.txt";
const char* strAsyncCopyPath = "write_async_copy.txt";

void ReadTest()
{
    MallocList<char*> lines;
    File::ReadAllLines(strPath, lines, 255);
    int idx = 1;
    IEnumerator<char*>* it = lines.GetEnumerator();
    while(it->MoveNext())
    {
        printf("%s", *it->Current());
    }
}
void WriteTest()
{
    FileStream *fs(NULL);
    char path[MAX_PATH];
    System::IO::GetFullPath(strPath, path);
    if(File::Exists(path))
    {
        fs = new FileStream(strPath, FileMode::Append);/*以可读可写追加的方式打开文件，光标初始位置为最后一个字符的后面 */
        printf("    Sucess append to file : %s\n", path);
    }
    else
    {
        fs = new FileStream(strPath, FileMode::CreateNew);
        printf("    Sucess create new file : %s\n", path);
        if(NULL == FileStreamEx::FileHandle(*fs))
        {
            printf("Error at create file : %d\n", GetLastError());
            return;
        }
    }
    PRINTF_TEST_BOOL(fs->CanRead());
    PRINTF_TEST_BOOL(fs->CanWrite());
    PRINTF_TEST_BOOL(fs->CanSeek());
    PRINTF_TEST_CHARS(fs->Name());
    
    printf("FileSize : %zd\n", /*fs->Length()*/ fs->GetPosition());
    FileStreamEx::WriteLine(*fs, "Line111");
    printf("FileSize : %zd\n", /*fs->Length()*/ fs->GetPosition());
    FileStreamEx::WriteLine(*fs, "Line2");
    printf("FileSize : %zd\n", /*fs->Length()*/ fs->GetPosition());
    fs->WriteByte('a');
    printf("FileSize : %zd\n", /*fs->Length()*/ fs->GetPosition());
    fs->WriteByte('\n');
    printf("FileSize : %zd\n", /*fs->Length()*/ fs->GetPosition());
    fs->WriteByte('b');
    printf("FileSize : %zd\n", /*fs->Length()*/ fs->GetPosition());
    fs->Write("\ncount=8\n", 9);
    printf("FileSize : %zd\n", /*fs->Length()*/ fs->GetPosition());
    fs->Flush();
    printf("After Write FileSize:%zd\n", fs->Length());
    delete fs;
}
void CopyTest()
{
    File::Copy(strPath, strCopyPath);
    FileStream* src = new FileStream(strPath, FileMode::Open);
    FileStream* dest = new FileStream(strAsyncCopyPath, FileMode::Create);
    size_t len = File::FileSize(strPath);
    Task* t = src->CopyToAsync(*dest, len);
    printf("Copy async %s , copy bytes : %zd\n", t->Wait(-1) ? "sucess" : "failed", len);
    delete t;
    delete dest;
    delete src;
}
#define TASK_COUNT 10//MAXIMUM_WAIT_OBJECTS
void AnsyncTest()
{
    StackList<char*> list({"Hello", "World"});
    char* lines[] = {"Hello", "World"};
    size_t count = 2;
    Task* tasks[TASK_COUNT];
    char* path[TASK_COUNT];

    for(int i = 0; i < TASK_COUNT; i++)
    {
        path[i] = new char[MAX_PATH];
        sprintf(path[i], "./AsyncWrite/%d.txt", i);
        tasks[i] = File::WriteAllLinesAsync(path[i], lines, count); 
        tasks[i]->ContinueWith([](Task &parent, AsyncState args)->void
        {
            printf("Print task-%d attached to write lines task-%d , status : %d\n", GetCurrentThreadId(), parent.GetId(), parent.GetStatus());
            AsyncState c = parent.GetAsyncState();
            printf("Write lines sucess to : %s\n", static_cast<const char*>(c[0]));
            // static_cast<StackList<char*>*>(c[1])->ForEach([](char* const& str)->void{printf("%s\n", str);});
            printf("%s %s\n", static_cast<char**>(c[1])[0], static_cast<char**>(c[1])[1]);
        });/* 在父任务析构时, Continue task 也随之释放 */
    }
    if(Task::WaitAll(tasks, TASK_COUNT, -1))
    {
        printf("finished ansyc test\n");
    }
    for (int i=0; i < TASK_COUNT; i++)
    {
        printf("Write lines task-%d , status : %d\n", tasks[i]->GetId(), tasks[i]->GetStatus());
        delete tasks[i];
        delete[] path[i];
    }
}
int main()
{
    printf("------------------------\nBegin write test\n");
    WriteTest();
    printf("------------------------\nBegin read test\n");
    ReadTest();
    printf("------------------------\nBegin copy test\n");
    CopyTest();
    printf("------------------------\nBegin ansync test\n");
    AnsyncTest();
    printf("Sucess exit\n");
    return 0;
}