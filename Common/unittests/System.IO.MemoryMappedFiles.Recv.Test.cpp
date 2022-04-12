/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-10-08 15:10:49
 * @LastEditors: like
 * @LastEditTime: 2022-02-21 19:32:04
 */
#include <System.IO.MemoryMappedFiles.hpp>
using namespace System::IO::MemoryMappedFiles;

int main()
{
    MemoryMappedFile* mapFile = MemoryMappedFile::OpenExisting("test1", ReadWrite);
    printf("Create Share Memory:%s\n", NULL != mapFile ? "SUCESS":"FAILED");
    MemoryMappedViewAccessor* accessor = mapFile->CreateViewAccessor();
    printf("Create Share Memory Accessor:%s\n", NULL != accessor ? "SUCESS":"FAILED");
    printf("HeadPtr:%p\n", accessor->GetBuffer());

    char* src = (char*)accessor->GetBuffer();
    for(int i = 0; i < 100; i++)
    {
        printf("%3d:%d\n", i, src[i]);
    }
    accessor->Flush(100);
    delete accessor;
    accessor = NULL;
    delete mapFile;
    mapFile = NULL;
}