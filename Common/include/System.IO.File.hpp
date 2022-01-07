/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-07 21:16:24
 * @LastEditors: like
 * @LastEditTime: 2022-01-07 21:42:48
 */
#ifndef SYSTEM_IO_FILE_HPP
#define SYSTEM_IO_FILE_HPP

#include <System.IO.FileStream.hpp>

namespace System::IO
{
    inline static bool IsFile(struct _stat buf)
    {
        return S_IFREG == (buf.st_mode & S_IFREG);
    }
    class File;
};
class System::IO::File
{
private:
    File(){}
public:
    inline static bool CanExe(const char* filePath)
    {
        return 0 == _access(filePath, 0x01);
    }
    inline static bool CanRead(const char* filePath)
    {
        return 0 == _access(filePath, 0x02);
    }
    inline static bool CanWrite(const char* filePath)
    {
        return 0 == _access(filePath, 0x04);
    }
    inline static bool CanReadWrite(const char* filePath)
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
            printf("ReadAllBytes Error Return\n");
            return -1;
        }
        if(NULL == dest)
        {
            dest = (byte*)malloc(size);
            memset(dest, 0, size);
        }
        size_t readedTotalLen   = 0;
        if(!(readedTotalLen = src.Read(dest, size)))
        {
            return -1;
        }
        src.Close();
        return readedTotalLen;
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
            if(!p)
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

#endif