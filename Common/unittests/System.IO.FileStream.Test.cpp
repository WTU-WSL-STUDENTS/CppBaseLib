/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-08 15:10:49
 * @LastEditors: like
 * @LastEditTime: 2022-01-07 21:22:49
 */
#include <System.IO.FileStream.hpp>
#include <System.IO.File.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace System::IO;
const char* strPath = "..\\..\\resource\\unittests\\output\\FileStream_WriteTest.txt";
const char* strCopyPath = "..\\..\\resource\\unittests\\output\\FileStream_WriteTest_Copy.txt";

void ReadTest()
{
    vector<char*> lines;
    File::ReadAllLines(strPath, lines);
    printf("%zd", lines.size());
    int idx = 1;
    for(vector<char*>::iterator it = lines.begin(); it != lines.end(); it++, idx++)
    {
        printf("%3d:%s", idx, *it);
        char* str = *it;
        delete(*it);
    }
}
void WriteTest()
{
    FileStream *stream(NULL);
    if(File::Exists(strPath))
    {
        // stream = File::OpenWrite(strPath); /*�Զ�д�ķ�ʽ���ļ� */
        stream = new FileStream(strPath, FileOperate::Append);/*�Կɶ���д׷�ӵķ�ʽ���ļ�������ʼλ��Ϊ���һ���ַ��ĺ��� */
    }
    else
    {
        stream = File::Create(strPath);/* �½�һ���ļ�����������򸲸� */
    }
    printf("FileSize:%zd\n", stream->Length());
    stream->WriteLine("Line111");
    stream->WriteLine("Line2");
    stream->WriteByte('a');
    stream->WriteByte('\n');
    stream->WriteByte('b');
    stream->Write((System::byte*)"\ncount=8\n", 9);
    printf("After Write FileSize:%zd\n", stream->Length());
    delete stream;
    stream = NULL;
}
void CopyTest()
{
    File::Copy(strPath, strCopyPath);
}

int main()
{
    // WriteTest();
    // ReadTest();
    // CopyTest();
    return 0;
}