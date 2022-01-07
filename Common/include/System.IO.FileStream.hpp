/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-13 07:43:18
 * @LastEditors: like
 * @LastEditTime: 2022-01-07 21:21:27
 */
#ifdef USE_CPP_FILE_API
#ifndef SYSTEM_IO_FILESTREAM_HPP
#define SYSTEM_IO_FILESTREAM_HPP
#endif
#include <System.IO.FileStreamcpp.hpp>
#endif
#ifndef SYSTEM_IO_FILESTREAM_HPP
#define SYSTEM_IO_FILESTREAM_HPP

#if !defined(USE_CPP_FILE_API) && !defined(USE_C_FILE_API)
#define USE_C_FILE_API
#endif
#define MAX_LINE_LENGTH 1024

#define _CRT_SECURE_NO_DEPRECATE
#ifdef __WINDOWS
#pragma warning(disable:4996)
#endif
#include "CompliedEntry.h"
#include "System.Convert.hpp"
#include "System.IO.Stream.hpp"
#include <io.h>
#include <stdio.h>
#include <map>
#include <vector>
using namespace std;

/*https://www.runoob.com/linux/linux-comm-stat.html*/

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
        bool Exists(const char* filePath, PathType type)
        {
            struct _stat buf;
            return (0 == _stat(filePath, &buf)) &&  (type == (buf.st_mode & type));
        } 
        size_t FileSize(const char* filePath)
        {
            struct _stat buf;
            if(!((0 == _stat(filePath, &buf)) &&  (FILE == (buf.st_mode & FILE))))
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
        typedef std::map<int, char*> AccessModeMappingToLetter;
        
        AccessModeMappingToLetter accessModeMappingToLetter = 
        {
            {ReadOnly,      "r" },
            {ReadBinary,    "rb"},
            {ReadWrite,     "r+"},
            {Create,        "w+"},
            {Append,        "a+"}
        };
        class FileStream : public Stream /* https://docs.microsoft.com/zh-cn/dotnet/api/system.io.filestream.-ctor?view=net-5.0#System_IO_FileStream__ctor_System_String_System_IO_FileMode_ */
        {
            public:
                const char* filePath;
                std::FILE* fileHandle;
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
                    long len = _filelength(fileno(fileHandle));
                    return -1 < len ? len : 0; 
    
                }

                long Seek(long offset, int origin)
                {
                    return fseek(fileHandle, offset, origin);
                }
                template<typename T>
                int Scan(const char* format, T& val)
                {
                    return fscanf(fileHandle, format, val);
                }
                size_t Read(System::byte* dest, size_t count)
                {
                    return fread(dest, sizeof(System::byte), count, fileHandle);
                }
                bool Get(System::byte* dest, size_t count)
                {
                    return fgets((char*)dest, count, fileHandle);
                }
                template<typename T>
                size_t Read(T* dest, int offset, int count)/* ƫ��offset���ֽڣ���ȡcount���ֽڣ�����ʵ�ʶ�ȡ�ĳ���*/
                {
                    if(0 == fseek(fileHandle, offset, 0))
                    {
                        return fread(dest, sizeof(T), count, fileHandle);
                    }
                    return 0;
                }
                template<typename T>
                size_t Read(T* dest,  int count)/* ƫ��offset���ֽڣ���ȡcount���ֽڣ�����ʵ�ʶ�ȡ�ĳ���*/
                {
                    return fread(dest, sizeof(T), count, fileHandle);
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
                template<typename T>
                size_t Write(T* src, size_t count = 1)
                {
                    return fwrite(src, sizeof(T), count, fileHandle);
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
    }
}

#ifdef __WINDOWS
#pragma warning(default:4996)
#endif
#endif