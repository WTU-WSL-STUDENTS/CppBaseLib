/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-13 07:43:18
 * @LastEditors: like
 * @LastEditTime: 2022-02-11 20:37:59
 */
#ifndef SYSTEM_IO_FILESTREAM_HPP
#define SYSTEM_IO_FILESTREAM_HPP

#include <CompliedEntry.h>
#include <System.IO.Stream.hpp>
#include <sys/stat.h>
#include <stdio.h>
#include <io.h>
#include <map>

#ifdef ASSERT_ENABLE
#include <assert.h>
#define FILE_DEBUG(condition) assert(condition)
#endif

#define GEN_FILE_ACCESS_MODE_INDEX(access, mode, type) (((type) << 24) | ((access) << 8) | (mode))

namespace System::IO
{
    enum FileAccess;
    enum FileMode;
    enum FileShare;
    enum FileType;
    class FileStream;
    /**
     * @brief �ж�ָ��·���Ƿ�Ϊ�ļ�
     * 
     * @param buf 
     * @return true 
     * @return false 
     */
    inline static bool IsFile(struct _stat buf)
    {
        return S_IFREG == (buf.st_mode & S_IFREG);
    }
    /**
     * @brief �ж�ָ��·���Ƿ�Ϊ�ļ�
     * 
     * @param strPath 
     * @return true 
     * @return false 
     */
    inline static bool IsFile(const char* strPath)
    {
        struct _stat buf;
        if (0 == _stat(strPath, &buf))
            return S_IFREG == (buf.st_mode & S_IFREG);
        return false;
    }
};
enum System::IO::FileAccess
{
    Read = 1,
    Write,
    ReadWrite
};
enum System::IO::FileMode
{
    /**
     * @brief ָ������ϵͳӦ�������ļ��� ����Ҫ Write Ȩ�ޡ� ����ļ��Ѵ��ڣ������� IOException�쳣
     * 
     */
    CreateNew,
    /**
     * @brief ָ������ϵͳӦ�������ļ��� ������ļ��Ѵ��ڣ���Ὣ�串�ǡ� ����Ҫ Write Ȩ�ޡ� FileMode.Create ��Ч����������������ļ������ڣ���ʹ�� CreateNew������ʹ�� Truncate�� ������ļ��Ѵ��ڵ�Ϊ�����ļ��������� UnauthorizedAccessException�쳣
     * 
     */
    Create,    
    /**
     * @brief ָ������ϵͳӦ�������ļ��� ���ļ�������ȡ���� FileAccess ö����ָ����ֵ�� ����ļ������ڣ�����һ�� FileNotFoundException �쳣��
     * 
     */
    Open,
    /**
     * @brief ָ������ϵͳӦ���ļ�������ļ����ڣ�������Ӧ�������ļ��� ����� FileAccess.Read ���ļ�������Ҫ ReadȨ�ޡ� ����ļ�����Ϊ FileAccess.Write������Ҫ WriteȨ�ޡ� ����� FileAccess.ReadWrite ���ļ�����ͬʱ��Ҫ Read �� WriteȨ��
     * 
     */
    OpenOrCreate,
    /**
     * @brief ָ������ϵͳӦ�������ļ��� ���ļ�����ʱ�������ض�Ϊ���ֽڴ�С�� ����Ҫ Write Ȩ�ޡ� ���Դ�ʹ�� FileMode.Truncate �򿪵��ļ��н��ж�ȡ������ ArgumentException �쳣��
     * 
     */
    Truncate,
    /**
     * @brief �������ļ�����򿪸��ļ������ҵ��ļ�β�����ߴ���һ�����ļ��� ����Ҫ Append Ȩ�ޡ� FileMode.Append ֻ���� FileAccess.Write һ��ʹ�á� ��ͼ�����ļ�β֮ǰ��λ��ʱ������ IOException �쳣�������κ���ͼ��ȡ�Ĳ�������ʧ�ܲ����� NotSupportedException �쳣��
     * 
     */
    Append
};
enum System::IO::FileShare
{
    /**
     * @brief л������ǰ�ļ��� �ļ��ر�ǰ���򿪸��ļ����κ������ɴ˽��̻���һ���̷��������󣩶���ʧ�ܡ�
     * 
     */
    None = 0,
    /**
     * @brief ���������ļ���ȡ�� ���δָ���˱�־�����ļ��ر�ǰ���κδ򿪸��ļ��Խ��ж�ȡ�������ɴ˽��̻���һ���̷��������󣩶���ʧ�ܡ�
     * 
     */
    Read = 1,
    /**
     * @brief ���������ļ�д�롣 ���δָ���˱�־�����ļ��ر�ǰ���κδ򿪸��ļ��Խ���д��������ɴ˽��̻���һ�����̷��������󣩶���ʧ�ܡ�
     * 
     */
    Write,
    /**
     * @brief ���������ļ���ȡ��д�롣 ���δָ���˱�־�����ļ��ر�ǰ���κδ򿪸��ļ��Խ��ж�ȡ��д��������ɴ˽��̻���һ���̷���������ʧ��
     * 
     */
    ReadWrite
};
enum System::IO::FileType
{
    Text = 1,
    Binary
};
class System::IO::FileStream : public Stream
{
private:
    typedef std::map<int, const char*> AccessModeMappingToLetter; 
    AccessModeMappingToLetter accessModeMappingToLetter = /* https://baike.baidu.com/item/c%E8%AF%AD%E8%A8%80fopen%E5%87%BD%E6%95%B0 */
    {
        /**
         * @brief ����ֻд�ļ������ļ������򱨴�(��Ҫ FileStream ֧��)�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(Write, CreateNew, Text | Binary  ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(Write, CreateNew, Text           ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(Write, CreateNew, Binary         ), "wb"},
        /**
         * @brief �����ɶ�/д�ļ������ļ������򱨴�(��Ҫ FileStream ֧��)�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, CreateNew, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, CreateNew, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, CreateNew, Binary         ), "wb+"},
        /**
         * @brief ����ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Create, Text | Binary  ), "w"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Create, Text           ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Create, Binary         ), "wb"}, 
        /**
         * @brief ������/д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Create, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Create, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Create, Binary         ), "wb+"},
        /**
         * @brief ��ֻ����ʽ���ļ������ļ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(Read, Open, Text | Binary   ), "r"},
        {GEN_FILE_ACCESS_MODE_INDEX(Read, Open, Text            ), "rt"},
        {GEN_FILE_ACCESS_MODE_INDEX(Read, Open, Binary          ), "rb"},
        /**
         * @brief ��ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Open, Text | Binary  ), "w"}, /* Truncate */
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Open, Text           ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Open, Binary         ), "wb"}, 
        /**
         * @brief �Զ�/д��ʽ���ļ������ļ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Open, Text | Binary  ), "r+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Open, Text           ), "rt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Open, Binary         ), "rb+"},
        /**
         * @brief OpenOrCreate (��Ҫ FileStream ֧��)
         * 
         */
        /**
         * @brief ��ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Truncate, Text | Binary  ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Truncate, Text           ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Truncate, Binary         ), "wb"},
        /**
         * @brief �򿪿ɶ�/д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ�
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Truncate, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Truncate, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Truncate, Binary         ), "wb+"},

        /**
         * @brief �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Append, Text | Binary), "a"},
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Append, Text         ), "at"},
        {GEN_FILE_ACCESS_MODE_INDEX(Write, Append, Binary       ), "ab"},
        /**
         * @brief �Ը��ӷ�ʽ�򿪿ɶ�/д���ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF����������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Append, Text | Binary), "a+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Append, Text         ), "at+"},
        {GEN_FILE_ACCESS_MODE_INDEX(ReadWrite, Append, Binary       ), "ab+"}
    };
    FILE* m_pf;
    const char* m_strPath;
    FileMode m_eMode;
    FileAccess m_eAccess;
    FileShare m_eShare;
    
    /**
     * @brief �ļ�����
     * 
     * @return int 
     */
    inline bool LockFile()
    {
        int nRet;
        if(0 == (nRet == flock(fileno(m_pf), LOCK_EX | LOCK_NB)))
        {
            return true;
        }  
        printf("Lock file : %s failed, error code : %d\n", path, nRet);
        return false;
    }
    /**
     * @brief �ļ�����
     * 
     * @return int 
     */
    inline bool UnlockFile()
    {
        int nRet;
        if(0 == (nRet == flock(fileno(m_pf), LOCK_UN | LOCK_NB)))
        {
            return true;
        }  
        printf("Unlock file : %s failed, error code : %d\n", path, nRet);
        return false;
    }
public:
    FileStream(const char* path, FileMode mode =  OpenOrCreate, FileAccess access = ReadWrite, FileShare share = ReadWrite, FileType type = Text | Binary) : 
        m_strPath(path), m_eMode(mode), m_eAccess(access), m_eShare(access & share)
    {
        if(OpenOrCreate == mode)
        {
            mode = _access(path, 0) ? Open : Create;
        }
        else if(CreateNew == mode && _access(path, 0))
        {
            throw "IOException";
        }
        int nAuth = GEN_FILE_ACCESS_MODE_INDEX(mode, access, type);
        if(accessModeMappingToLetter.end() != accessModeMappingToLetter.find(nAuth))
        {
            throw "SecurityException";
        }
        if(NULL == (m_pf = fopen(path, accessModeMappingToLetter[nAuth]))) /* �������������*/
        {
            char buf[1024];
            sprintf(buf, "IOException , Error Code : %d\n", GetLastError());
            throw buf;
        }
        int nRet;
        if(None == share && !LockFile())
        {
            fclose(m_pf);
            m_pf = NULL;
            char buf[255];
            sprintf(buf, "Not allow share access : %s, lock failed, error code : %d\n", path, nRet);
            throw buf;
        }
    }
    ~FileStream()
    { 
        int nRet;
        if(None == m_eShare && !UnlockFile())
        {
            char buf[255];
            sprintf(buf, "Not allow share access : %s, unlock failed, error code : %d\n", path, nRet);
            throw buf;
        }
        fclose(m_pf);
        m_pf = NULL;
    }
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ���Ƿ�֧�ֶ�ȡ
     * 
     * @return true 
     * @return false 
     */
    bool CanRead() override
    {
        return Read & m_eAccess;
    }
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ���Ƿ�֧��д��
     * 
     * @return true 
     * @return false 
     */
    bool CanWrite() override
    {
        return Write & m_eAccess;
    }
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ���Ƿ�֧�ֲ���
     * 
     * @return true 
     * @return false 
     */
    bool CanSeek() override
    {
        return Read & m_eAccess;
    }
    /**
     * @brief ��ȡ���ĳ��ȣ����ֽ�Ϊ��λ��
     * 
     * @return size_t 
     */
    size_t Length() override
    {
        long len = _filelength(fileno(m_pf));
        return -1 < len ? len : 0; 
    }
    /**
     * @brief ��ȡ FileStream ���Ѵ򿪵��ļ��ľ���·��
     * 
     * @return const char* 
     */
    const char* Name()
    {
        return m_strPath;
    }
};
// namespace System
// {
//     namespace IO
//     {
//         enum PathType/* �ļ����� */
//         {
//             IgnoreType = 0,
//             FILE = S_IFREG,
//             DIRECOTRY = 0040000
//             /*
//                 S_IFMT      0170000     �ļ����͵�λmask
//                 S_IFSOCK    0140000     socket
//                 S_IFLNK     0120000     ��������(symbolic link)
//                 S_IFREG     0100000     һ���ļ�
//                 S_IFBLK     0060000     ����װ��(block device)
//                 S_IFDIR     0040000     Ŀ¼
//                 S_IFCHR     0020000     �ַ�װ��(character device)
//                 S_IFIFO     0010000     �Ƚ��ȳ�(fifo)
//                 S_ISUID     0004000     �ļ���(set user-id on execution)λ
//                 S_ISGID     0002000     �ļ���(set group-id on execution)λ
//                 S_ISVTX     0001000     �ļ���stickyλ
//                 S_IRWXU     00700       �ļ������ߵ�maskֵ(������Ȩ��ֵ)
//                 S_IRUSR     00400       �ļ������߾߿ɶ�ȡȨ��
//                 S_IWUSR     00200       �ļ������߾߿�д��Ȩ��
//                 S_IXUSR     00100       �ļ������߾߿�ִ��Ȩ��
//                 S_IRWXG     00070       �û����maskֵ(������Ȩ��ֵ)
//                 S_IRGRP     00040       �û���߿ɶ�ȡȨ��
//                 S_IWGRP     00020       �û���߿�д��Ȩ��
//                 S_IXGRP     00010       �û���߿�ִ��Ȩ��
//                 S_IRWXO     00007       �����û���maskֵ(������Ȩ��ֵ)
//                 S_IROTH     00004       �����û��߿ɶ�ȡȨ��
//                 S_IWOTH     00002       �����û��߿�д��Ȩ��
//                 S_IXOTH     00001       �����û��߿�ִ��Ȩ��
//             */
//         };
//         bool Exists(const char* filePath, PathType type)
//         {
//             struct _stat buf;
//             return (0 == _stat(filePath, &buf)) &&  (type == (buf.st_mode & type));
//         } 
//         size_t FileSize(const char* filePath)
//         {
//             struct _stat buf;
//             if(!((0 == _stat(filePath, &buf)) &&  (FILE == (buf.st_mode & FILE))))
//             {
//                 return 0;
//             }
//             return buf.st_size;
//         }

//         class FileStream : public Stream /* https://docs.microsoft.com/zh-cn/dotnet/api/system.io.filestream.-ctor?view=net-5.0#System_IO_FileStream__ctor_System_String_System_IO_FileMode_ */
//         {
//             protected:
//                 virtual void Dispose(bool disposing)
//                 {
//                     if(disposing)
//                     {
//                         /* TODO : Dispose*/
//                     }
//                 }
//             public:
//                 const char* filePath;
//                 std::FILE* fileHandle;
//                 FileStream(const char* strFilePath, FileOperate operate = Create) : filePath(strFilePath)
//                 {
//                     fileHandle = fopen(strFilePath, accessModeMappingToLetter[operate]);
//                 }
//                 ~FileStream()
//                 {
//                     Close();
//                 }
//                 bool Valid(){return NULL != fileHandle;}
//                 std::FILE* GetFileHandle(){return fileHandle;}
//                 size_t Length()
//                 {
//                     long len = _filelength(fileno(fileHandle));
//                     return -1 < len ? len : 0; 
    
//                 }
//                 void Dispose() override
//                 {
//                     Dispose(true);
//                 }
//                 long Seek(long offset, int origin)
//                 {
//                     return fseek(fileHandle, offset, origin);
//                 }
//                 template<typename T>
//                 int Scan(const char* format, T& val)
//                 {
//                     return fscanf(fileHandle, format, val);
//                 }
//                 size_t Read(System::byte* dest, size_t count)
//                 {
//                     return fread(dest, sizeof(System::byte), count, fileHandle);
//                 }
//                 bool Get(System::byte* dest, size_t count)
//                 {
//                     return fgets((char*)dest, count, fileHandle);
//                 }
//                 template<typename T>
//                 size_t Read(T* dest, int offset, int count)/* ƫ��offset���ֽڣ���ȡcount���ֽڣ�����ʵ�ʶ�ȡ�ĳ���*/
//                 {
//                     if(0 == fseek(fileHandle, offset, 0))
//                     {
//                         return fread(dest, sizeof(T), count, fileHandle);
//                     }
//                     return 0;
//                 }
//                 template<typename T>
//                 size_t Read(T* dest,  int count)/* ƫ��offset���ֽڣ���ȡcount���ֽڣ�����ʵ�ʶ�ȡ�ĳ���*/
//                 {
//                     return fread(dest, sizeof(T), count, fileHandle);
//                 }
//                 int ReadByte()/* EOF -1*/
//                 {
//                     return fgetc(fileHandle);
//                 }
//                 char* ReadLine()
//                 {
//                     char* buffer = (char*)calloc(MAX_LINE_LENGTH, 1);
//                     fgets(buffer, MAX_LINE_LENGTH,fileHandle);   
//                     return 0 != buffer[0] ? buffer : NULL;
//                 }
//                 size_t Write(System::byte* src, size_t count)
//                 {
//                     return fwrite(src, sizeof(System::byte), count, fileHandle);
//                 }
//                 size_t Write(System::byte* src, int offset, int count)
//                 {
//                     if(0 == fseek(fileHandle, offset, 0))
//                     {
//                         return fwrite(src, sizeof(System::byte), count, fileHandle);
//                     }
//                     return 0;
//                 }
//                 template<typename T>
//                 size_t Write(T* src, size_t count = 1)
//                 {
//                     return fwrite(src, sizeof(T), count, fileHandle);
//                 }
//                 int WriteByte(System::byte byte)
//                 {
//                     return fputc(byte, fileHandle);
//                 }
//                 bool WriteLine(const char* str)
//                 {
//                     return 0 < fprintf(fileHandle, "%s\n", str);	
//                 }
//                 void Flush()
//                 {            
//                     fflush(fileHandle);
//                 }
//                 int Close()
//                 {  
//                     return NULL != fileHandle ?  fclose(fileHandle) : 0;
//                 }
//         };
//     }
// }

// #ifdef __WINDOWS
// #pragma warning(default:4996)
// #endif
#endif