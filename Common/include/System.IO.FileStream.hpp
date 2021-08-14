#ifndef FILE_H
#define FILE_H


#include "complied_entry.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_DEPRECATE
#endif
#define MAX_LINE_LENGTH 1024
#include "System.Convert.hpp"
#include "System.IO.Stream.hpp"
#include <io.h>
#include <stdio.h>
#include <map>
#include <vector>
using namespace std;

/*https://www.runoob.com/linux/linux-comm-stat.html*/

namespace System
{
    namespace IO
    {
        enum PathType/* 文件类型 */
        {
            IgnoreType = 0,
            FILE = S_IFREG,
            DIRECOTRY = 0040000
            /*
                S_IFMT      0170000     文件类型的位mask
                S_IFSOCK    0140000     socket
                S_IFLNK     0120000     符号链接(symbolic link)
                S_IFREG     0100000     一般文件
                S_IFBLK     0060000     区块装置(block device)
                S_IFDIR     0040000     目录
                S_IFCHR     0020000     字符装置(character device)
                S_IFIFO     0010000     先进先出(fifo)
                S_ISUID     0004000     文件的(set user-id on execution)位
                S_ISGID     0002000     文件的(set group-id on execution)位
                S_ISVTX     0001000     文件的sticky位
                S_IRWXU     00700       文件所有者的mask值(即所有权限值)
                S_IRUSR     00400       文件所有者具可读取权限
                S_IWUSR     00200       文件所有者具可写入权限
                S_IXUSR     00100       文件所有者具可执行权限
                S_IRWXG     00070       用户组的mask值(即所有权限值)
                S_IRGRP     00040       用户组具可读取权限
                S_IWGRP     00020       用户组具可写入权限
                S_IXGRP     00010       用户组具可执行权限
                S_IRWXO     00007       其他用户的mask值(即所有权限值)
                S_IROTH     00004       其他用户具可读取权限
                S_IWOTH     00002       其他用户具可写入权限
                S_IXOTH     00001       其他用户具可执行权限
            */
        };
#define DECLARE_STAT_BUF struct _stat buf
#define EXISTS(filePath, type) ((0 == _stat(filePath, &buf)) &&  (type == (buf.st_mode & type)))
        bool Exists(const char* filePath, PathType type)
        {
            DECLARE_STAT_BUF;
            return EXISTS(filePath, type);
        } 
        size_t FileSize(const char* filePath)
        {
            DECLARE_STAT_BUF;
            if(!EXISTS(filePath, FILE))
            {
                return 0;
            }
            return buf.st_size;
        }

        enum FileOperate/* 文件流操作 */
        {
            ReadOnly    ,   /* 打开只读文件，该文件必须存在。 */
            ReadWrite   ,   /* 打开可读写的文件，该文件必须存在 */
            Create      ,   /* 打开可读写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件 */
            Append          /* 打开可读写文件，若文件存在则在光标默认移动至末尾，若文件不存在则建立该文件 */
        };
        typedef std::map<int, char*> AccessModeMappingToLetter;
        
        AccessModeMappingToLetter accessModeMappingToLetter = 
        {
            {ReadOnly,      "r" },
            {ReadWrite,     "r+"},
            {Create,        "w+"},
            {Append,        "a+"}
        };
        class FileStream : public Stream /* https://docs.microsoft.com/zh-cn/dotnet/api/system.io.filestream.-ctor?view=net-5.0#System_IO_FileStream__ctor_System_String_System_IO_FileMode_ */
        {
            private:
                const char* filePath;
                std::FILE* fileHandle;
            public:
                FileStream(const char* strFilePath, FileOperate operate = Create) : filePath(strFilePath)
                {
                    fileHandle = fopen(strFilePath, accessModeMappingToLetter[operate]);
                }
                ~FileStream()
                {
                    Close();
                }
                bool Valid(){return NULL != fileHandle;}
                std::FILE* GetFileHandle(){return fileHandle;}
                size_t Length()
                {
                    fpos_t pos;
                    if(0 != fgetpos(fileHandle, &pos))
                    {
                        return 0;
                    }
                    fseek(fileHandle,0,SEEK_END);
                    size_t filesize = ftell(fileHandle);
                    fsetpos(fileHandle, &pos);
                    return filesize;
                }

                long Seek(long offset, int origin)
                {
                    return fseek(fileHandle, offset, origin);
                }

                size_t Read(System::byte* dest, size_t count)
                {
                    return fread(dest, sizeof(System::byte), count, fileHandle);
                }
                size_t Read(System::byte* dest, int offset, int count)/* 偏移offset个字节，读取count个字节，返回实际读取的长度*/
                {
                    if(0 == fseek(fileHandle, offset, 0))
                    {
                        return fread(dest, sizeof(System::byte), count, fileHandle);
                    }
                    return 0;
                }
                int ReadByte()/* EOF -1*/
                {
                    return fgetc(fileHandle);
                }
                char* ReadLine()
                {
                    char* buffer = (char*)calloc(MAX_LINE_LENGTH, 1);
                    fgets(buffer, MAX_LINE_LENGTH,fileHandle);   
                    return 0 != buffer[0] ? buffer : NULL;
                }
                size_t Write(System::byte* src, size_t count)
                {
                    return fwrite(src, sizeof(System::byte), count, fileHandle);
                }
                size_t Write(System::byte* src, int offset, int count)
                {
                    if(0 == fseek(fileHandle, offset, 0))
                    {
                        return fwrite(src, sizeof(System::byte), count, fileHandle);
                    }
                    return 0;
                }
                int WriteByte(System::byte byte)
                {
                    return fputc(byte, fileHandle);
                }
                bool WriteLine(const char* str)
                {
                    return 0 < fprintf(fileHandle, "%s\n", str);	
                }
                void Flush()
                {            
                    fflush(fileHandle);
                }
                int Close()
                {  
                    return NULL != fileHandle ?  fclose(fileHandle) : 0;
                }
        };

        class File
        {
            private:
                File(){}
            public:
                static bool CanExe(const char* filePath)
                {
                    return 0 == _access(filePath, 0x01);
                }
                static bool CanRead(const char* filePath)
                {
                    return 0 == _access(filePath, 0x02);
                }
                static bool CanWrite(const char* filePath)
                {
                    return 0 == _access(filePath, 0x04);
                }
                static bool CanReadWrite(const char* filePath)
                {
                    return 0 == _access(filePath, 0x06);
                }
                static void AppendAllLines(const char* filePath, char* lines[], int length)
                {
                    FileStream fs(filePath, Append);
                    for(int i = 0; i < length; i++)
                    {
                        fs.WriteLine(lines[i]);
                    }
                    fs.Close();
                }
                static void Copy(const char* srcPath, const char* destPath)
                {
                    FileStream src(srcPath, ReadOnly);
                    FileStream dest(destPath, ReadWrite);
                    size_t len;
                    System::byte buff[MAX_LINE_LENGTH];
                    while(0 < (len = src.Read(buff, MAX_LINE_LENGTH)))
                    {
                        dest.Write(buff, len);
                    }
                    dest.Close();
                    src.Close();
                }
                static FileStream* Create(const char* filePath)
                {
                    FileStream* fs = new FileStream(filePath, FileOperate::Create);
                    return fs->Valid() ? fs :NULL;
                }
                static bool Delete(const char* filePath){return 0 == remove(filePath);}
                static bool Exists(const char* filePath){return System::IO::Exists(filePath, PathType::FILE);}
                static bool Move(const char* srcPath, const char* destPath)
                {
                    FileStream src(srcPath, ReadOnly);
                    FileStream dest(destPath);
                    size_t len;
                    System::byte buff[10240];
                    while(0 < (len = src.Read(buff, 10240)))/* 1.复制源数据所有字节 */
                    {
                        dest.Write(buff, len);
                    }
                    src.Close();
                    if(!Delete(srcPath)) /*2. 删除源文件 */
                    {
                        dest.Close();
                        Delete(destPath);
                        return false;
                    }
                    dest.Flush();/* 3.保存数据 */
                    dest.Close();
                    return true;
                }
                static FileStream* Open(const char* filePath, FileOperate mode = FileOperate::Create)
                {
                    FileStream* fs = new FileStream(filePath, mode);
                    if(fs->Valid())
                    {
                        fs->Seek(0, SEEK_SET);
                        return fs;
                    }
                    return NULL;
                }
                static FileStream* OpenRead(const char* filePath)
                {
                    FileStream* fs = new FileStream(filePath, ReadOnly);
                    return fs->Valid() ? fs :NULL;
                }
                static FileStream* OpenWrite(const char* filePath)
                {
                    FileStream* fs = new FileStream(filePath, ReadWrite);
                    return fs->Valid() ? fs :NULL;
                }
                static size_t ReadAllBytes(const char* filePath, byte* (&dest), size_t &size)/* dest需要手动释放 */
                {
                    FileStream src(filePath, ReadOnly);
                    size = src.Length();
                    if(1 > size)
                    {
                        return 0;
                    }
                    dest = (byte*)malloc(size);
                    size_t len = src.Read(dest, size);
                    src.Close();
                    return len;
                }
                static void ReadAllLines(const char* filePath, vector<char*> &lines)
                {
                    FileStream fs(filePath, ReadWrite);
                    if(!fs.Valid())
                    {
                        return;
                    }
                    do
                    {
                        char* p = fs.ReadLine();
                        if(NULL == p)
                        {
                            break;
                        }
                        lines.push_back(p);
                    }while(true);
                    fs.Close();
                }
                static void Replace(const char* srcPath, const char* destPath, const char* backupPath = "\0") /* 使用其他文件的内容替换指定文件的内容，这一过程将删除原始文件，并创建被替换文件的备份。 */
                {
                    if(0 != backupPath[0])
                    {
                        Copy(destPath, backupPath);
                    }
                    Move(srcPath, destPath);
                }
        };
    }
}

#endif