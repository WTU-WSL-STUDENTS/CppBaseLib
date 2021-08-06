#include <System.IO.MemoryMappedFiles.hpp>
using namespace System::IO::MemoryMappedFiles;

int main()
{
    MemoryMappedFile* mapFile = MemoryMappedFile::CreateOrOpen("test1", 100);
    printf("Create Share Memory:%s\n", NULL != mapFile ? "SUCESS":"FAILED");
    MemoryMappedViewAccessor* accessor = mapFile->CreateViewAccessor();
    printf("Create Share Memory Accessor:%s\n", NULL != accessor ? "SUCESS":"FAILED");
    printf("HeadPtr:%p\n", accessor->GetBuffer());
    accessor->Memset(123);
    char* src = (char*)accessor->GetBuffer();
    for(size_t i = 0; i < accessor->GetSize(); i++)
    {
        printf("%3d:%d\n", i, src[i]);
    }
    while (1)
    {
        /* code */
        Sleep(1000);
    }
    
    // char* scan0 = (char*)accessor->Buffer();
    // size_t size = accessor->Size();
    // memset(scan0, 123, size);

    // delete accessor;
    // accessor = NULL;
    // delete mapFile;
    // mapFile = NULL;
}