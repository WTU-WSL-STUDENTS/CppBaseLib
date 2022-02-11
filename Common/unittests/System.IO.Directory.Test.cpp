/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-06 16:49:36
 * @LastEditors: like
 * @LastEditTime: 2022-02-11 10:43:38
 */

#include <System.IO.Directory.hpp>

using namespace System;
using namespace System::IO;

#define PRINTF_TEST_BOOL(str) printf(#str" : %s\n", str ? "true" : "false")
#define PRINTF_TEST_CHARS(str) printf(#str" : %s\n", str)

int main()
{
    char buf[_MAX_PATH];

    PRINTF_TEST_BOOL(IsDirectory("./"));
    PRINTF_TEST_BOOL(Directory::CreateDirectory("D:\\CppBaseLib\\Common\\build\\bin\\x86\\111\\test_CreateDirectory"));
    PRINTF_TEST_BOOL(Directory::CreateDirectory("D:\\CppBaseLib\\Common\\build\\bin\\x86\\222\\"));
    PRINTF_TEST_BOOL(GetFullPath("./aa", buf));
    printf("%s\n", buf);
    PRINTF_TEST_BOOL(Directory::CreateDirectory(buf));
    PRINTF_TEST_BOOL(Directory::CopyDirectory("111", "222", true));
    PRINTF_TEST_BOOL(Directory::Exists("./"));
    
    DateTime dt;
    PRINTF_TEST_BOOL(Directory::GetCreationTime("./", dt));
    dt.ToString(buf);
    printf("%s\n", buf);
    PRINTF_TEST_BOOL(Directory::GetCreationTimeUTC("./", dt));
    dt.ToString(buf);
    printf("%s\n", buf);
    PRINTF_TEST_BOOL(Directory::GetLastAccessTime("./", dt));
    dt.ToString(buf);
    printf("%s\n", buf);
    PRINTF_TEST_BOOL(Directory::GetLastAccessTimeUTC("./", dt));
    dt.ToString(buf);
    printf("%s\n", buf);
    PRINTF_TEST_BOOL(Directory::GetLastWriteTime("./", dt));
    dt.ToString(buf);
    printf("%s\n", buf);
    PRINTF_TEST_BOOL(Directory::GetLastWriteTimeUTC("./", dt));
    dt.ToString(buf);
    printf("%s\n", buf);

    Directory::GetCurrentDirectory(buf);
    printf("%s\n", buf);
    PRINTF_TEST_BOOL(GetFullPath("./", buf));
    printf("%s\n", buf);

    StackList<std::string> list;
    size_t i = 0;
    PRINTF_TEST_BOOL(Directory::GetFiles(list, "./", "*.exe"));
    for(; i < list.Count(); i++)
    {
        printf("\"%s\"\n", list[i].c_str());
    }
    Search_With(buf, "*.pdb", list, [](const char* path, const _finddata_t& data, StackList<std::string>& container)->void
    {
        char buf[_MAX_PATH] = {0};
        strcat
        (
            strcpy(buf, path),
            data.name
        );
        printf("%s\n", data.name);
        container.Add(string(buf));
        // container.Add(data.name);
    });
    for(; i < list.Count(); i++)
    {
        printf("\"%s\"\n", list[i].c_str());
    }
    list.Clear();
    PRINTF_TEST_BOOL(Directory::Move("./aa", "./111"));
    PRINTF_TEST_BOOL(Directory::Delete("D:\\CppBaseLib\\Common\\build\\bin\\x86\\111\\test_CreateDirectory"));
    PRINTF_TEST_BOOL(Directory::Delete("D:/CppBaseLib/Common/build/bin/x86/111/aa"));
    PRINTF_TEST_BOOL(Directory::Delete("D:\\CppBaseLib\\Common\\build\\bin\\x86\\111\\"));
    PRINTF_TEST_BOOL(Directory::Delete("D:\\CppBaseLib\\Common\\build\\bin\\x86\\222"));/* false , direcotry not empty */
    PRINTF_TEST_BOOL(Directory::Delete("D:\\CppBaseLib\\Common\\build\\bin\\x86\\222", true));
    printf("Sucess Exit\n");
    return 0;
}