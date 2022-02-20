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
     * @brief 判断指定路径是否为文件
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
     * @brief 判断指定路径是否为文件
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
     * @brief 指定操作系统应创建新文件。 这需要 Write 权限。 如果文件已存在，则将引发 IOException异常
     * 
     */
    CreateNew,
    /**
     * @brief 指定操作系统应创建新文件。 如果此文件已存在，则会将其覆盖。 这需要 Write 权限。 FileMode.Create 等效于这样的请求：如果文件不存在，则使用 CreateNew；否则使用 Truncate。 如果该文件已存在但为隐藏文件，则将引发 UnauthorizedAccessException异常
     * 
     */
    Create,    
    /**
     * @brief 指定操作系统应打开现有文件。 打开文件的能力取决于 FileAccess 枚举所指定的值。 如果文件不存在，引发一个 FileNotFoundException 异常。
     * 
     */
    Open,
    /**
     * @brief 指定操作系统应打开文件（如果文件存在）；否则，应创建新文件。 如果用 FileAccess.Read 打开文件，则需要 Read权限。 如果文件访问为 FileAccess.Write，则需要 Write权限。 如果用 FileAccess.ReadWrite 打开文件，则同时需要 Read 和 Write权限
     * 
     */
    OpenOrCreate,
    /**
     * @brief 指定操作系统应打开现有文件。 该文件被打开时，将被截断为零字节大小。 这需要 Write 权限。 尝试从使用 FileMode.Truncate 打开的文件中进行读取将导致 ArgumentException 异常。
     * 
     */
    Truncate,
    /**
     * @brief 若存在文件，则打开该文件并查找到文件尾，或者创建一个新文件。 这需要 Append 权限。 FileMode.Append 只能与 FileAccess.Write 一起使用。 试图查找文件尾之前的位置时会引发 IOException 异常，并且任何试图读取的操作都会失败并引发 NotSupportedException 异常。
     * 
     */
    Append
};
enum System::IO::FileShare
{
    /**
     * @brief 谢绝共享当前文件。 文件关闭前，打开该文件的任何请求（由此进程或另一进程发出的请求）都将失败。
     * 
     */
    None = 0,
    /**
     * @brief 允许随后打开文件读取。 如果未指定此标志，则文件关闭前，任何打开该文件以进行读取的请求（由此进程或另一进程发出的请求）都将失败。
     * 
     */
    Read = 1,
    /**
     * @brief 允许随后打开文件写入。 如果未指定此标志，则文件关闭前，任何打开该文件以进行写入的请求（由此进程或另一进过程发出的请求）都将失败。
     * 
     */
    Write,
    /**
     * @brief 允许随后打开文件读取或写入。 如果未指定此标志，则文件关闭前，任何打开该文件以进行读取或写入的请求（由此进程或另一进程发出）都将失败
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
         * @brief 创建只写文件，若文件存在则报错(需要 FileStream 支持)；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, Text | Binary  ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, Text           ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, Binary         ), "wb"},
        /**
         * @brief 创建可读/写文件，若文件存在则报错(需要 FileStream 支持)；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, Binary         ), "wb+"},
        /**
         * @brief 创建只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, Text | Binary  ), "w"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, Text           ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, Binary         ), "wb"}, 
        /**
         * @brief 创建读/写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, Binary         ), "wb+"},
        /**
         * @brief 以只读方式打开文件，该文件必须存在
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, Text | Binary   ), "r"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, Text            ), "rt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, Binary          ), "rb"},
        /**
         * @brief 打开只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, Text | Binary  ), "w"}, /* Truncate */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, Text           ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, Binary         ), "wb"}, 
        /**
         * @brief 以读/写方式打开文件，该文件必须存在
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, Text | Binary  ), "r+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, Text           ), "rt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, Binary         ), "rb+"},
        /**
         * @brief OpenOrCreate (需要 FileStream 支持)
         * 
         */
        /**
         * @brief 打开只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, Text | Binary  ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, Text           ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, Binary         ), "wb"},
        /**
         * @brief 打开可读/写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, Text | Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, Text           ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, Binary         ), "wb+"},

        /**
         * @brief 以附加的方式打开只写文件。若文件不存在，则会创建该文件；如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（EOF 符保留）
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, Text | Binary), "a"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, Text         ), "at"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, Binary       ), "ab"},
        /**
         * @brief 以附加方式打开可读/写的文件。若文件不存在，则会创建该文件，如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（EOF符不保留）
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
     * @brief 文件锁定
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
     * @brief 文件解锁
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
        if(NULL == (m_pf = fopen(path, accessModeMappingToLetter[nAuth]))) /* 不加锁的情况下*/
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
     * @brief 获取一个值，该值指示当前流是否支持读取
     * 
     * @return true 
     * @return false 
     */
    bool CanRead() override
    {
        return FileAccess::Read & m_nAccess;
    }
    /**
     * @brief 获取一个值，该值指示当前流是否支持写入
     * 
     * @return true 
     * @return false 
     */
    bool CanWrite() override
    {
        return FileAccess::Write & m_nAccess;
    }
    /**
     * @brief 获取一个值，该值指示当前流是否支持查找
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
     * @brief 获取流的长度（以字节为单位）
     * 
     * @return size_t 
     */
    size_t Length() override
    {
        long len = _filelength(fileno(m_pf));
        return -1 < len ? len : 0; 
    }
    /**
     * @brief 获取 FileStream 中已打开的文件的绝对路径
     * 
     * @return const char* 
     */
    const char* Name()
    {
        return m_strPath;
    }
    /**
     * @brief 获取此流的当前位置
     * 
     * @return long 
     */
    inline long GetPosition() override
    {
        return ftell(m_pf);
    }
    /**
     * @brief 设置此流的当前位置
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
    static inline int Scan(FileStream& fs, const char* format/* %s 空格或换行为止 */, T& val)
    {
        return fscanf(fs.m_pf, format, val);
    }
    static inline bool Get(FileStream& fs, char* dest, size_t count/* 最长到换行符截至 */)
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
//         enum PathType/* 文件类型 */
//         {
//             IgnoreType = 0,
//             FILE = S_IFREG,
//             DIRECOTRY = 0040000
//             /*
//                 S_IFMT      0170000     文件类型的位mask
//                 S_IFSOCK    0140000     socket
//                 S_IFLNK     0120000     符号链接(symbolic link)
//                 S_IFREG     0100000     一般文件
//                 S_IFBLK     0060000     区块装置(block device)
//                 S_IFDIR     0040000     目录
//                 S_IFCHR     0020000     字符装置(character device)
//                 S_IFIFO     0010000     先进先出(fifo)
//                 S_ISUID     0004000     文件的(set user-id on execution)位
//                 S_ISGID     0002000     文件的(set group-id on execution)位
//                 S_ISVTX     0001000     文件的sticky位
//                 S_IRWXU     00700       文件所有者的mask值(即所有权限值)
//                 S_IRUSR     00400       文件所有者具可读取权限
//                 S_IWUSR     00200       文件所有者具可写入权限
//                 S_IXUSR     00100       文件所有者具可执行权限
//                 S_IRWXG     00070       用户组的mask值(即所有权限值)
//                 S_IRGRP     00040       用户组具可读取权限
//                 S_IWGRP     00020       用户组具可写入权限
//                 S_IXGRP     00010       用户组具可执行权限
//                 S_IRWXO     00007       其他用户的mask值(即所有权限值)
//                 S_IROTH     00004       其他用户具可读取权限
//                 S_IWOTH     00002       其他用户具可写入权限
//                 S_IXOTH     00001       其他用户具可执行权限
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