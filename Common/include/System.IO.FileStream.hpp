/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-13 07:43:18
 * @LastEditors: like
 * @LastEditTime: 2022-03-24 17:02:57
 */
#ifndef SYSTEM_IO_FILESTREAM_HPP
#define SYSTEM_IO_FILESTREAM_HPP
#include <CompliedEntry.h>
#include <System.IO.Stream.hpp>
#include <System.Threading.ReaderWriterLockSlim.hpp>
#include <sys/stat.h>
#include <sys/locking.h>
#include <stdio.h>
#include <io.h>
#include <map>

#ifdef __WINDOWS
#pragma warning(disable:4996)
#endif

#define GEN_FILE_ACCESS_MODE_INDEX(access, mode, type) ( ((type) << 24) | ((access) << 8) | (mode) )
// #define RWL_LOCK
#ifndef RWL_LOCK
#define RWL_READ_BLOCK(codeBlock) codeBlock
#define RWL_WRITE_BLOCK(codeBlock) codeBlock
#else
#define RWL_READ_BLOCK(codeBlock) rwl.EnterReadLock(); codeBlock rwl.ExitReadLock()
#define RWL_WRITE_BLOCK(codeBlock) rwl.EnterWriteLock(); codeBlock rwl.ExitWriteLock()
#endif
namespace System::IO
{
    namespace FileAccess_
    {
        enum Option;
    }
    namespace FileMode_
    {
        enum Option;
    }
    namespace FileShare_
    {
        enum Option;
    }
    namespace FileType_
    {
        enum Option;
    };
    using FileAccess = FileAccess_::Option;
    using FileMode = FileMode_::Option;
    using FileShare = FileShare_::Option;
    using FileType = FileType_::Option;
    class FileStream;
    class FileStreamEx;
};
enum System::IO::FileAccess_::Option
{
    Read = 1,
    Write,
    ReadWrite
};
enum System::IO::FileMode_::Option
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
     * @brief 指定操作系统应打开现有文件。 该文件被打开时，将被截断为零字节大小。 这需要 Write 权限。 尝试从使用 FileMode.Truncate 打开的文件中进行读取将导致 "ArgumentException" 异常。
     * 
     */
    Truncate,
    /**
     * @brief 若存在文件，则打开该文件并查找到文件尾，或者创建一个新文件。 这需要 Append 权限。 FileMode.Append 只能与 FileAccess.Write 一起使用。 试图查找文件尾之前的位置时会引发 IOException 异常，并且任何试图读取的操作都会失败并引发 "NotSupportedException" 异常。
     * 
     */
    Append
};
enum System::IO::FileShare_::Option
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
enum System::IO::FileType_::Option
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
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, FileType::Text | FileType::Binary   ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, FileType::Text                      ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, FileType::Binary                    ), "wb"},
        /**
         * @brief 创建可读/写文件，若文件存在则报错(需要 FileStream 支持)；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, FileType::Text | FileType::Binary   ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, FileType::Text                      ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, FileType::Binary                    ), "wb+"},
        /**
         * @brief 创建只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, FileType::Text | FileType::Binary  ), "w"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, FileType::Text                     ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, FileType::Binary                   ), "wb"}, 
        /**
         * @brief 创建读/写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, FileType::Text | FileType::Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, FileType::Text                     ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, FileType::Binary                   ), "wb+"},
        /**
         * @brief 以只读方式打开文件，该文件必须存在
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, FileType::Text | FileType::Binary ), "r"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, FileType::Text                    ), "rt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, FileType::Binary                  ), "rb"},
        /**
         * @brief 打开只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, FileType::Text | FileType::Binary), "w"}, /* Truncate */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, FileType::Text                   ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, FileType::Binary                 ), "wb"}, 
        /**
         * @brief 以读/写方式打开文件，该文件必须存在
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, FileType::Text | FileType::Binary), "r+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, FileType::Text                   ), "rt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, FileType::Binary                 ), "rb+"},
        /**
         * @brief OpenOrCreate (需要 FileStream 支持)
         * 
         */
        /**
         * @brief 打开只写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件。
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, FileType::Text | FileType::Binary), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, FileType::Text                   ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, FileType::Binary                 ), "wb"},
        /**
         * @brief 打开可读/写文件，若文件存在则文件长度清为零，即该文件内容会消失；若文件不存在则创建该文件
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, FileType::Text | FileType::Binary), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, FileType::Text                   ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, FileType::Binary                 ), "wb+"},

        /**
         * @brief 以附加的方式打开只写文件。若文件不存在，则会创建该文件；如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（EOF 符保留）
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, FileType::Text | FileType::Binary  ), "a"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, FileType::Text                     ), "at"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, FileType::Binary                   ), "ab"},
        /**
         * @brief 以附加方式打开可读/写的文件。若文件不存在，则会创建该文件，如果文件存在，则写入的数据会被加到文件尾后，即文件原先的内容会被保留（EOF符不保留）
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Append, FileType::Text | FileType::Binary  ), "a+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Append, FileType::Text                     ), "at+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Append, FileType::Binary                   ), "ab+"}
    };
    std::FILE* m_pf;
    const char* m_strPath;
    FileMode m_eMode;
    int m_nAccess;
    FileShare m_eShare;
    System::Threading::ReaderWriterLockSlim rwl;
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

            // if(FileShare::None == m_eShare && !UnlockFile())
            // {
            //     char buf[1024];
            //     sprintf(buf, "Not allow share access : %s, unlock failed\n", m_strPath);
            //     throw buf;
            // }
            RWL_READ_BLOCK
            (
                fclose(m_pf);
                m_pf = NULL;
            );
        }
    }
public:
    FileStream(const char* path, FileMode mode =  FileMode::OpenOrCreate, FileAccess access = FileAccess::ReadWrite, FileShare share = FileShare::ReadWrite, int type = FileType::Text | FileType::Binary) : 
        m_strPath(path), m_eMode(mode), m_nAccess(access), m_eShare(share)
    {
        if(FileMode::OpenOrCreate == mode)
        {
            mode = _access(path, 0) ? FileMode::Open : FileMode::Create;
        }
        int nAuth = GEN_FILE_ACCESS_MODE_INDEX(access, mode, type);
        if(FileMode::CreateNew == mode)
            ERROR_ASSERT(_access(path, 0), "IOException");
        ERROR_ASSERT(accessModeMappingToLetter.end() != accessModeMappingToLetter.find(nAuth), "SecurityException");
        ERROR_ASSERT(NULL != (m_pf = fopen(path, accessModeMappingToLetter[nAuth])), "DirectoryNotFoundException");
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
        RWL_READ_BLOCK
        (
            long len = _filelength(fileno(m_pf));
        )
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
        RWL_READ_BLOCK
        (
            long pos = ftell(m_pf);
        );
        return pos;
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
    /**
     * @brief 从当前文件流中异步读取字节并将其写入到另一个流中
     * 
     * @param destination 
     * @param bufferSize 
     * @return Task* 
     */
    Task* CopyToAsync(Stream& destination, size_t& bufferSize) override
    {
        ERROR_ASSERT(CanRead() && destination.CanWrite(), "NotSupportedException");
        Task* t = new Task([](AsyncState c)->void
        {
            FileStream* fs      = (FileStream*)c[0];
            Stream* destination = (Stream*)c[1]; 
            size_t bufferSize   = *(size_t*)c[2];
            char* buf = (char*)malloc(bufferSize);
            size_t readedLen = fs->Read(buf, bufferSize); 
            destination->Write(buf, readedLen);
            free(buf);
            destination->Flush();
        }, CreateAsyncState(this, &destination, &bufferSize));
        t->Start();
        return t;
    }
    inline void Flush() override
    {
        if(CanWrite())
        {
            RWL_READ_BLOCK
            (
                int nRet = fflush(m_pf);
            );
            ERROR_ASSERT(0 == nRet, "Flush Error");
        }
    }
    Task* FlushAsync() override
    {
        ERROR_ASSERT(CanWrite(), "NotSupportedException");
        Task* t = new Task([](AsyncState args)->void
        {
            static_cast<FileStream*>(args[0])->Flush();
        }, CreateAsyncState(this));
        t->Start();
        return t;
    }
    size_t Read(char* buffer, size_t count) override
    {
        RWL_READ_BLOCK
        (
            size_t len = fread(buffer, sizeof(char), count, m_pf);
        );
        return len;
    }
    size_t Read(char* buffer, long offset, size_t count) override
    {
        ERROR_ASSERT(Seek(offset, SeekOrigin::Current), "ArgumentException");
        return Read(buffer, count);
    }
    Task* ReadAsync(char* buffer, long& offset, size_t& count) override
    {
        ERROR_ASSERT(CanRead(), "NotSupportedException");
        Task* t = new Task([](AsyncState c)->void
        {
            FileStream* fs  = static_cast<FileStream*>(c[0]);
            char* buffer    = static_cast<char*>(c[1]); 
            long offset     = *static_cast<long*>(c[2]);
            size_t count    = *static_cast<size_t*>(c[3]);

            fs->Read(buffer, offset, count);
        }, CreateAsyncState(this, buffer, &offset, &count));
        t->Start();
        return t;
    }
    int ReadByte() override
    {
        RWL_READ_BLOCK
        (
            int c = fgetc(m_pf);
        );
        return c;
    }
    bool Seek(long offset, SeekOrigin origin) override
    {
        ERROR_ASSERT(CanSeek(), "NotSupportedException");     
        RWL_READ_BLOCK
        (
            int n = fseek(m_pf, offset, origin);
        );
        return 0 == n;
    }
    size_t Write(const char* buffer, size_t count) override
    {
        ERROR_ASSERT(CanWrite(), "NotSupportedException");
        RWL_WRITE_BLOCK
        (
            size_t len = fwrite(buffer, sizeof(char), count, m_pf);
        );
        return len;
    }
    size_t Write(const char* buffer, long offset, size_t count)
    {
        ERROR_ASSERT(Seek(offset, SeekOrigin::Current), "ArgumentException");
        return Write(buffer, count);
    }
    Task* WriteAsync(char* buffer, long& offset, size_t& count) override
    {
        ERROR_ASSERT(CanWrite(), "NotSupportedException");
        Task* t = new Task([](AsyncState c)->void
        {
            FileStream* fs  = static_cast<FileStream*>(c[0]);
            char* buffer    = static_cast<char*>(c[1]); 
            long offset     = *static_cast<long*>(c[2]);
            size_t count    = *static_cast<size_t*>(c[3]);

            fs->Write(buffer, offset, count);
            fs->Flush();
        }, CreateAsyncState(this, buffer, &offset, &count));
        t->Start();
        return t;
    }
    inline void WriteByte(char value) override
    {
        ERROR_ASSERT(CanWrite(), "NotSupportedException");
        RWL_WRITE_BLOCK
        (
            fputc(value, m_pf);
        );
    }
};
class System::IO::FileStreamEx
{
public:
    template<typename T>
    static inline int Scan(FileStream& fs, const char* format/* %s 空格或换行为止 */, T& val)
    {
        // System::Threading::ReaderWriterLockSlim& rwl = &fs.rwl;
        RWL_READ_BLOCK
        (
            int len = fscanf(fs.m_pf, format, val);
        );
        return len;
    }
    static inline bool Get(FileStream& fs, char* dest, size_t count/* 最长到换行符截至 */)
    {
        // System::Threading::ReaderWriterLockSlim& rwl = fs.rwl;
        RWL_READ_BLOCK
        (
            char* p = fgets(dest, count, fs.m_pf);
        );
        return NULL != p;
    }
    static inline int ReadLine(FileStream& fs, char* buffer)
    {
        // System::Threading::ReaderWriterLockSlim& rwl = fs.rwl;
        RWL_READ_BLOCK
        (
            int n = fscanf(fs.m_pf, "%[^\n]\n", buffer);
        );
        return n;
    }
    static inline int WriteLine(FileStream& fs,const char* str)
    {
        // System::Threading::ReaderWriterLockSlim& rwl = fs.rwl;
        RWL_WRITE_BLOCK
        (
            int n = fprintf(fs.m_pf, "%s\n", str);	
        );
        return n;
    }
    static inline FILE* FileHandle(FileStream& fs)
    {
        return fs.m_pf;
    }
};
#ifdef __WINDOWS
#pragma warning(default:4996)
#endif
#endif