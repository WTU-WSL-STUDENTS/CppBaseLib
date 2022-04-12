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
     * @brief ָ������ϵͳӦ�������ļ��� ���ļ�����ʱ�������ض�Ϊ���ֽڴ�С�� ����Ҫ Write Ȩ�ޡ� ���Դ�ʹ�� FileMode.Truncate �򿪵��ļ��н��ж�ȡ������ "ArgumentException" �쳣��
     * 
     */
    Truncate,
    /**
     * @brief �������ļ�����򿪸��ļ������ҵ��ļ�β�����ߴ���һ�����ļ��� ����Ҫ Append Ȩ�ޡ� FileMode.Append ֻ���� FileAccess.Write һ��ʹ�á� ��ͼ�����ļ�β֮ǰ��λ��ʱ������ IOException �쳣�������κ���ͼ��ȡ�Ĳ�������ʧ�ܲ����� "NotSupportedException" �쳣��
     * 
     */
    Append
};
enum System::IO::FileShare_::Option
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
         * @brief ����ֻд�ļ������ļ������򱨴�(��Ҫ FileStream ֧��)�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, FileType::Text | FileType::Binary   ), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, FileType::Text                      ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::CreateNew, FileType::Binary                    ), "wb"},
        /**
         * @brief �����ɶ�/д�ļ������ļ������򱨴�(��Ҫ FileStream ֧��)�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, FileType::Text | FileType::Binary   ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, FileType::Text                      ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::CreateNew, FileType::Binary                    ), "wb+"},
        /**
         * @brief ����ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, FileType::Text | FileType::Binary  ), "w"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, FileType::Text                     ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Create, FileType::Binary                   ), "wb"}, 
        /**
         * @brief ������/д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, FileType::Text | FileType::Binary  ), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, FileType::Text                     ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Create, FileType::Binary                   ), "wb+"},
        /**
         * @brief ��ֻ����ʽ���ļ������ļ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, FileType::Text | FileType::Binary ), "r"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, FileType::Text                    ), "rt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Read, FileMode::Open, FileType::Binary                  ), "rb"},
        /**
         * @brief ��ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, FileType::Text | FileType::Binary), "w"}, /* Truncate */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, FileType::Text                   ), "wt"}, 
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Open, FileType::Binary                 ), "wb"}, 
        /**
         * @brief �Զ�/д��ʽ���ļ������ļ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, FileType::Text | FileType::Binary), "r+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, FileType::Text                   ), "rt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Open, FileType::Binary                 ), "rb+"},
        /**
         * @brief OpenOrCreate (��Ҫ FileStream ֧��)
         * 
         */
        /**
         * @brief ��ֻд�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ���
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, FileType::Text | FileType::Binary), "w"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, FileType::Text                   ), "wt"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Truncate, FileType::Binary                 ), "wb"},
        /**
         * @brief �򿪿ɶ�/д�ļ������ļ��������ļ�������Ϊ�㣬�����ļ����ݻ���ʧ�����ļ��������򴴽����ļ�
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, FileType::Text | FileType::Binary), "w+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, FileType::Text                   ), "wt+"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::ReadWrite, FileMode::Truncate, FileType::Binary                 ), "wb+"},

        /**
         * @brief �Ը��ӵķ�ʽ��ֻд�ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF ��������
         * 
         */
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, FileType::Text | FileType::Binary  ), "a"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, FileType::Text                     ), "at"},
        {GEN_FILE_ACCESS_MODE_INDEX(FileAccess::Write, FileMode::Append, FileType::Binary                   ), "ab"},
        /**
         * @brief �Ը��ӷ�ʽ�򿪿ɶ�/д���ļ������ļ������ڣ���ᴴ�����ļ�������ļ����ڣ���д������ݻᱻ�ӵ��ļ�β�󣬼��ļ�ԭ�ȵ����ݻᱻ������EOF����������
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
        RWL_READ_BLOCK
        (
            long len = _filelength(fileno(m_pf));
        )
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
        RWL_READ_BLOCK
        (
            long pos = ftell(m_pf);
        );
        return pos;
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
    /**
     * @brief �ӵ�ǰ�ļ������첽��ȡ�ֽڲ�����д�뵽��һ������
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
    static inline int Scan(FileStream& fs, const char* format/* %s �ո����Ϊֹ */, T& val)
    {
        // System::Threading::ReaderWriterLockSlim& rwl = &fs.rwl;
        RWL_READ_BLOCK
        (
            int len = fscanf(fs.m_pf, format, val);
        );
        return len;
    }
    static inline bool Get(FileStream& fs, char* dest, size_t count/* ������з����� */)
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