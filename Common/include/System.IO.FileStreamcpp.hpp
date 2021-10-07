/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-13 07:43:18
 * @LastEditors: like
 * @LastEditTime: 2021-10-06 21:50:54
 */
#ifndef SYSTEM_IO_FILESTREAMCPP_HPP
#define SYSTEM_IO_FILESTREAMCPP_HPP
#define MAX_LINE_LENGTH 1024

#define _CRT_SECURE_NO_DEPRECATE
#include "CompliedEntry.h"
#include "System.Convert.hpp"
#include "System.IO.Stream.hpp"
#include <io.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <string>
#include <iostream>  
#include <fstream> 
#include <sstream>
using namespace std;

#define FILE_SCAN_END EOF

namespace System
{
    namespace IO
    {
        enum PathType/* �ļ����� */
        {
            IgnoreType = 0,
            FILE = S_IFREG,
            DIRECOTRY = 0040000
            /*
                S_IFMT      0170000     �ļ����͵�λmask
                S_IFSOCK    0140000     socket
                S_IFLNK     0120000     ��������(symbolic link)
                S_IFREG     0100000     һ���ļ�
                S_IFBLK     0060000     ����װ��(block device)
                S_IFDIR     0040000     Ŀ¼
                S_IFCHR     0020000     �ַ�װ��(character device)
                S_IFIFO     0010000     �Ƚ��ȳ�(fifo)
                S_ISUID     0004000     �ļ���(set user-id on execution)λ
                S_ISGID     0002000     �ļ���(set group-id on execution)λ
                S_ISVTX     0001000     �ļ���stickyλ
                S_IRWXU     00700       �ļ������ߵ�maskֵ(������Ȩ��ֵ)
                S_IRUSR     00400       �ļ������߾߿ɶ�ȡȨ��
                S_IWUSR     00200       �ļ������߾߿�д��Ȩ��
                S_IXUSR     00100       �ļ������߾߿�ִ��Ȩ��
                S_IRWXG     00070       �û����maskֵ(������Ȩ��ֵ)
                S_IRGRP     00040       �û���߿ɶ�ȡȨ��
                S_IWGRP     00020       �û���߿�д��Ȩ��
                S_IXGRP     00010       �û���߿�ִ��Ȩ��
                S_IRWXO     00007       �����û���maskֵ(������Ȩ��ֵ)
                S_IROTH     00004       �����û��߿ɶ�ȡȨ��
                S_IWOTH     00002       �����û��߿�д��Ȩ��
                S_IXOTH     00001       �����û��߿�ִ��Ȩ��
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

        enum FileOperate/* �ļ������� */
        {
            ReadOnly    ,   /* ��ֻ���ı��ļ������ļ�������ڡ� */
            ReadBinary  ,   /* ��ֻ���������ı������ļ�������ڡ� */
            ReadWrite   ,   /* �򿪿ɶ�д���ļ������ļ�������� */
            Create      ,   /* �򿪿ɶ�д�ļ������ļ��������ļ�������Ϊ0�������ļ����ݻ���ʧ�����ļ��������������ļ� */
            Append          /* �򿪿ɶ�д�ļ������ļ��������ڹ��Ĭ���ƶ���ĩβ�����ļ��������������ļ� */
        };
        typedef std::map<int, int> AccessModeMappingToLetter;
        AccessModeMappingToLetter accessModeMappingToLetter = 
        {
            {ReadOnly,      ios::in                 },
            {ReadBinary,    ios::in | ios::binary   },
            {ReadWrite,     ios::in |ios::out       },
            {Create,        ios::in |ios::out | ios::trunc},
            {Append,        ios::in |ios::out | ios::app}
        };
        class FileStream : public Stream /* https://docs.microsoft.com/zh-cn/dotnet/api/system.io.filestream.-ctor?view=net-5.0#System_IO_FileStream__ctor_System_String_System_IO_FileMode_ */
        {
            public:
                string filePath;
                fstream fileHandle;
                FileStream(string strFilePath, FileOperate operate = Create) : filePath(strFilePath)
                {
                    fileHandle.open(strFilePath, accessModeMappingToLetter[operate]);
                }
                ~FileStream()
                {
                    Close();
                }
                bool Valid(){return fileHandle.is_open();}
                inline size_t Length()
                {
                    int temp=fileHandle.tellg(); 
                    fileHandle.seekg(0,ios_base::end); 
                    size_t len = fileHandle.tellg(); 
                    fileHandle.seekg(temp); 
                    return len;
                }

                inline long Seek(long offset, int origin)
                {
                    fileHandle.seekg(offset, ios::cur);
                    return 0;
                }
                template<typename T>
                inline int Scan(const char* format, T& val)
                {
                    throw "FileStream Cpp Api Not Support Scan";
                }
                inline size_t Read(System::byte* dest, size_t count)
                {
                    return fileHandle.readsome((char*)dest, count);
                }
                inline bool Get(System::byte* dest, size_t count)
                {
                    return fileHandle.readsome((char*)dest, count);
                }
                template<typename T>
                inline size_t Read(T* dest, int offset, int count)/* ƫ��offset���ֽڣ���ȡcount���ֽڣ�����ʵ�ʶ�ȡ�ĳ���*/
                {
                    if(fileHandle.seekg(offset))
                    {
                        return fileHandle.readsome((char*)dest, sizeof(T) * count);
                    }
                    return 0;
                }
                template<typename T>
                inline size_t Read(T* dest,  int count)/* ƫ��offset���ֽڣ���ȡcount���ֽڣ�����ʵ�ʶ�ȡ�ĳ���*/
                {
                    return fileHandle.readsome((char*)dest, sizeof(T) * count);
                }
                inline int ReadByte()/* EOF -1*/
                {
                    return fileHandle.get();
                }
                inline char* ReadLine()
                {
                    char* buffer = (char*)calloc(MAX_LINE_LENGTH, 1);
                    if(fileHandle.getline(buffer, MAX_LINE_LENGTH))
                    {
                        return buffer;
                    }
                    else
                    {
                        free(buffer);
                        return NULL;
                    }
                }
                inline size_t Write(System::byte* src, size_t count)
                {
                    if(fileHandle.write((const char*)src, count))
                    {
                        return count;
                    }
                    return 0;
                }
                size_t Write(System::byte* src, int offset, int count)
                {
                    if(fileHandle.seekg(offset))
                    {
                        fileHandle.write((const char*)src, count);
                        return count;
                    }
                    return 0;
                }
                template<typename T>
                size_t Write(T* src, size_t count = 1)
                {
                    count *= sizeof(T);
                    fileHandle.write((const char*)src, count);
                    return count;
                }
                int WriteByte(System::byte byte)
                {
                    fileHandle.put(byte);
                    return 1;
                }
                bool WriteLine(const char* str)
                {
                    fileHandle.write(str, strlen(str));
                    fileHandle<<"\n";
                    return true;
                }
                void Flush()
                {          
                    fileHandle.flush();  
                }
                int Close()
                {  
                    fileHandle.close();
                    return 0;
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
                    while(0 < (len = src.Read(buff, 10240)))/* 1.����Դ���������ֽ� */
                    {
                        dest.Write(buff, len);
                    }
                    src.Close();
                    if(!Delete(srcPath)) /*2. ɾ��Դ�ļ� */
                    {
                        dest.Close();
                        Delete(destPath);
                        return false;
                    }
                    dest.Flush();/* 3.�������� */
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
                static size_t ReadAllBytes(const char* filePath, byte* (&dest), size_t &size)/* dest��Ҫ�ֶ��ͷ� */
                {
                    ifstream in(filePath);
                    ostringstream tmp;
                    tmp << in.rdbuf();
                    string str = tmp.str();
                    size_t len = str.length();
                    if(NULL == dest)
                    {
                        dest = (byte*)calloc(len, 1);
                    }
                    memcpy(dest, str.c_str(), len);
                    in.close();
                    size = len;
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
                        if(!p)
                        {
                            break;
                        }
                        lines.push_back(p);
                    }while(true);
                    fs.Close();
                }
                static void Replace(const char* srcPath, const char* destPath, const char* backupPath = "\0") /* ʹ�������ļ��������滻ָ���ļ������ݣ���һ���̽�ɾ��ԭʼ�ļ������������滻�ļ��ı��ݡ� */
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