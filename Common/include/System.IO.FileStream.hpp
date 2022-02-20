/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-13 07:43:18
 * @LastEditors: like
 * @LastEditTime: 2022-02-20 21:56:37
 */
#ifndef SYSTEM_IO_FILESTREAM_HPP
#define SYSTEM_IO_FILESTREAM_HPP
#include <CompliedEntry.h>
#include <System.IO.Stream.hpp>
#include <sys/stat.h>
#include <sys/locking.h>
#include <stdio.h>
#include <io.h>
#include <map>
#include <functional>

#ifdef ASSERT_ENABLE
#include <assert.h>
#define FILE_DEBUG(condition) assert(condition)
#endif

#define GEN_FILE_ACCESS_MODE_INDEX(access, mode, type) ( ((type) << 24) | ((access) << 8) | (mode) )


namespace System::IO
{
    enum FileAccess;
    enum FileMode;
    enum FileShare;
    enum FileType;
    class FileStream;
    class FileStreamEx;
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
    friend class FileStreamEx;
private:
    typedef std::map<int, const char*> AccessModeMappingToLetter; 
    AccessModeMappingToLetter accessModeMappingToLetter = /* https://baike.baidu.com/item/c%E8%AF%AD%E8%A8%80fopen%E5%87%BD%E6%95%B0 */
    {
        /**
         * @brief ����ֻд�ļ������ļ������򱨴�(��Ҫ FileStream ֧��)�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, Text | Binary  ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, Text           ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, Binary         ), "wb"},
        /**
         * @brief �����ɶ�/д�ļ������ļ������򱨴�(��Ҫ FileStream ֧��)�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, Binary         ), "wb+"},
        /**
         * @brief ����ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, Text | Binary  ), "w"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, Text           ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, Binary         ), "wb"}, 
        /**
         * @brief ������/д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, Binary         ), "wb+"},
        /**
         * @brief ��ֻ����ʽ���ļ������ļ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, Text | Binary   ), "r"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, Text            ), "rt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, Binary          ), "rb"},
        /**
         * @brief ��ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, Text | Binary  ), "w"}, /* Truncate */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, Text           ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, Binary         ), "wb"}, 
        /**
         * @brief �Զ�/д��ʽ���ļ������ļ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, Text | Binary  ), "r+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, Text           ), "rt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, Binary         ), "rb+"},
        /**
         * @brief OpenOrCreate (��Ҫ FileStream ֧��)
         * 
         */
        /**
         * @brief ��ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, Text | Binary  ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, Text           ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, Binary         ), "wb"},
        /**
         * @brief �򿪿ɶ�/д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ�
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, Binary         ), "wb+"},

        /**
         * @brief �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, Text | Binary), "a"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, Text         ), "at"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, Binary       ), "ab"},
        /**
         * @brief �Ը��ӷ�ʽ�򿪿ɶ�/д���ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF����������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Append, Text | Binary), "a+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Append, Text         ), "at+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Append, Binary       ), "ab+"}
    };
    FILE* m_pf;
    const char* m_strPath;
    FileMode m_eMode;
    int m_nAccess;
    FileShare m_eShare;
    
    /**
     * @brief �ļ�����
     * 
     * @return int 
     */
    inline bool LockFile()
    {
        // int nRet;
        // if(0 == (nRet == flock(fileno(m_pf), LOCK_EX | LOCK_NB)))
        // {
        //     return true;
        // }  
        // printf("Lock file : %s failed, error code : %d\n", path, nRet);
        return false;
    }
    /**
     * @brief �ļ�����
     * 
     * @return int 
     */
    inline bool UnlockFile()
    {
        // int nRet;
        // if(0 == (nRet == flock(fileno(m_pf), LOCK_UN | LOCK_NB)))
        // {
        //     return true;
        // }  
        // printf("Unlock file : %s failed, error code : %d\n", path, nRet);
        return false;
    }
protected:
    void Dispose(bool disposing) override
    {
        if(disposing && m_pf)
        {
            int nRet;
            if(None == m_eShare && !UnlockFile())
            {
                char buf[255];
                sprintf(buf, "Not allow share access : %s, unlock failed, error code : %d\n", m_strPath, nRet);
                throw buf;
            }
            fclose(m_pf);
            m_pf = NULL;
        }
    }
public:
    FileStream(const char* path, FileMode mode =  FileMode::OpenOrCreate, FileAccess access = FileAccess::ReadWrite, FileShare share = FileShare::ReadWrite, int type = FileType::Text | FileType::Binary) : 
        m_strPath(path), m_eMode(mode), m_nAccess(access), m_eShare(share)
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
        Dispose();
    }
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ���Ƿ�֧�ֶ�ȡ
     * 
     * @return true 
     * @return false 
     */
    bool CanRead() override
    {
        return FileAccess::Read & m_nAccess;
    }
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ���Ƿ�֧��д��
     * 
     * @return true 
     * @return false 
     */
    bool CanWrite() override
    {
        return FileAccess::Write & m_nAccess;
    }
    /**
     * @brief ��ȡһ��ֵ����ֵָʾ��ǰ���Ƿ�֧�ֲ���
     * 
     * @return true 
     * @return false 
     */
    bool CanSeek() override
    {
        return FileAccess::Read & m_nAccess;
    }
    void Dispose() override
    {
        Dispose(true);
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
    /**
     * @brief ��ȡ�����ĵ�ǰλ��
     * 
     * @return long 
     */
    inline long GetPosition() override
    {
        return ftell(m_pf);
    }
    /**
     * @brief ���ô����ĵ�ǰλ��
     * 
     * @param pos 
     */
    inline bool SetPosition(long pos) override
    {
        return Seek(pos, SeekOrigin::Begin);
    }
    Task CopyToAsync(Stream &destination, size_t bufferSize) override
    {
        FILE_DEBUG(CanRead() && destination.CanWrite());
        std::function<void(void)> func = [&]()->void 
        {
            char* buf = (char*)malloc(bufferSize);
            Read(buf, bufferSize);
            destination.Write(buf, bufferSize);
            free(buf);
            destination.Flush();
        };
        return Task();
    }
    inline void Flush() override
    {
        if(CanWrite())
        {
            if(fflush(m_pf))
                printf("Flush Error\n");   
        }
    }
    Task FlushAsync() override
    {
        FILE_DEBUG(CanWrite());
        std::function<void(void)> func = [&]()->void 
        {
            Flush();
        };

        return Task();
    }
    size_t Read(char* buffer, size_t count) override
    {
        /* read lock */
        return fread(buffer, sizeof(char), count, m_pf);
    }
    size_t Read(char* buffer, long offset, size_t count) override
    {
        FILE_DEBUG(Seek(offset, SeekOrigin::Current));
        return Read(buffer, count);
    }
    Task ReadAsync(char* buffer, long offset, size_t count) override
    {
        FILE_DEBUG(CanRead());
        std::function<void(void)> func = [&]()->void 
        {
            Read(buffer, offset, count);
        };
        return Task();
    }
    int ReadByte() override
    {
        /* read lock */
        return fgetc(m_pf);
    }
    bool Seek(long offset, SeekOrigin origin) override
    {
        FILE_DEBUG(CanSeek());
        /* read lock */
        return 0 == fseek(m_pf, offset, origin);
    }
    void Write(const char* buffer, size_t count) override
    {
        FILE_DEBUG(CanWrite());
        /* write lock */
        if(count != fwrite(buffer, sizeof(char), count, m_pf))
        {
            throw "Write error";
        }
    }
    void Write(const char* buffer, long offset, size_t count)
    {
        FILE_DEBUG(Seek(offset, SeekOrigin::Current));
        Write(buffer, count);
    }
    Task WriteAsync(char* buffer, long offset, size_t count) override
    {
        FILE_DEBUG(CanWrite());
        std::function<void(void)> func = [&]()->void 
        {
            Write(buffer, offset, count);
            Flush();
        };
        return Task();
    }
    inline void WriteByte(char value) override
    {
        FILE_DEBUG(CanWrite());
        fputc(value, m_pf);
    }
};
class System::IO::FileStreamEx
{
public:
    template<typename T>
    static inline int Scan(FileStream& fs, const char* format/* %s �ո����Ϊֹ */, T& val)
    {
        return fscanf(fs.m_pf, format, val);
    }
    static inline bool Get(FileStream& fs, char* dest, size_t count/* ������з����� */)
    {
        return fgets(dest, count, fs.m_pf);
    }
    static inline int ReadLine(FileStream& fs, char* buffer)
    {
        return fscanf(fs.m_pf, "[%^\n]\n", buffer);
    }
    static inline int WriteLine(FileStream& fs,const char* str)
    {
        return fprintf(fs.m_pf, "%s\n", str);	
    }

};
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


// #ifdef __WINDOWS
// #pragma warning(default:4996)
// #endif
#endif