/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-07 21:16:24
 * @LastEditors: like
 * @LastEditTime: 2022-02-24 15:39:48
 */
#ifndef SYSTEM_IO_FILE_HPP
#define SYSTEM_IO_FILE_HPP
#include <System.IO.FileStream.hpp>
#include <System.IEnumerator.h>
#include <System.DateTime.hpp>
#include <System.List.hpp>

namespace System::IO
{
    /**
     * @brief ���·��ת����·��
     * 
     * @param relativePath 
     * @return true 
     * @return false 
     */
    inline static bool GetFullPath(const char* relativePath, char (&absolutePath)[_MAX_PATH])
    {
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        return realpath(relativePath, absolutePath);
#else
        return _fullpath(absolutePath, relativePath, _MAX_PATH);
#endif
    }
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
    /**
     * @brief ��ָ�����ַ���׷�ӵ��ļ��У�����ļ����������򴴽����ļ�
     * 
     * @param filePath 
     * @param lines 
     */
    static void AppendAllLines(const char* filePath, const char** lines, size_t lineCount)
    {
        FileStream fs(filePath, FileMode::Append);
        for(size_t i = 0; i < lineCount; i++)
        {
            FileStreamEx::WriteLine(fs, lines[i]);
        }
    }
    static void AppendAllLines(const char* filePath, IEnumerable<const char*> &lines)
    {
        FileStream fs(filePath, FileMode::Append);
        IEnumerator<const char*>* it = lines.GetEnumerator();
        while(it->MoveNext())
        {
            FileStreamEx::WriteLine(fs, *it->Current());
        }
    }
    static Task* AppendAllLinesAsync(const char* filePath, const char** lines, size_t& lineCount)
    {
        Task* t = new Task([](AsyncState c)->void
        {
            AppendAllLines(static_cast<const char*>(c[0]), static_cast<const char**>(c[1]), *static_cast<size_t*>(c[2]));
        }, CreateAsyncState(const_cast<char*>(filePath), const_cast<char**>(lines), &lineCount));
        t->Start();
        return t;
    }
    static Task* AppendAllLinesAsync(const char* filePath, IEnumerable<const char*> &lines)
    {
        Task* t = new Task([](AsyncState c)->void
        {
            AppendAllLines(*static_cast<const char**>(c[0]), *static_cast<IEnumerable<const char*>*>(c[1]));
        }, CreateAsyncState(const_cast<char*>(filePath), &lines));
        t->Start();
        return t;
    }
    /**
     * @brief ��ָ�����ַ���׷�ӵ��ļ��У�����ļ����������򴴽����ļ�
     * 
     * @param filePath 
     * @param str 
     */
    static void AppendAllText(const char* filePath, const char* str)
    {
        FileStream fs(filePath, FileMode::Append);
        fs.Write(str, strlen(str));
    }
    static Task* AppendAllTextAsync(const char* filePath, const char* str)
    {
        Task* t = new Task([](AsyncState c)->void
        {
            AppendAllText(static_cast<const char*>(c[0]), static_cast<const char*>(c[1]));
        }, CreateAsyncState(const_cast<char*>(filePath), const_cast<char*>(str)));
        t->Start();
        return t;
    }
    /**
     * @brief �������ļ����Ƶ����ļ��� ������ͬ�����ļ�
     * 
     * @param srcPath 
     * @param destPath 
     * @param overwrite 
     */
    static void Copy(const char* srcPath, const char* destPath, bool overwrite = true)
    {
        size_t len = FileSize(srcPath);
        ERROR_ASSERT(-1 != len, IOException);

        FileStream* src = new FileStream(srcPath, FileMode::Open);
        FileStream* dest = overwrite ? new FileStream(destPath, FileMode::Create) : new FileStream(destPath, FileMode::CreateNew);

        char* buf = (char*)malloc(len);
        size_t readedLen = src->Read(buf, len);
        dest->Write(buf, readedLen);
        dest->Flush();
        free(buf);
        delete dest;
        delete src;
    }
    /**
     * @brief ��ָ��·���д����򸲸��ļ�
     * 
     * @param filePath 
     * @return FileStream* 
     */
    static FileStream* Create(const char* filePath)
    {
        return new FileStream(filePath, FileMode::Create);
    }
    /**
     * @brief ������ʹ�� Encrypt(String) ������ǰ�ʻ����ܵ��ļ�
     * 
     * @param path 
     */
    static void Decrypt(const char* path)
    {
        throw "Not support";
    }
    /**
     * @brief ɾ��ָ�����ļ�
     * 
     * @param filePath 
     * @return true 
     * @return false 
     */
    static bool Delete(const char* filePath)
    {
        return 0 == remove(filePath);
    }
    /**
     * @brief ��ĳ���ļ����ܣ�ʹ��ֻ�м��ܸ��ļ����ʻ����ܽ������
     * 
     * @param path 
     */
    static void Encrypt(const char* path)
    {
        throw "Not support";
    }
    /**
     * @brief ȷ��ָ�����ļ��Ƿ����
     * 
     * @param filePath 
     * @return true 
     * @return false 
     */
    static bool Exists(const char* filePath)
    {
        return IsFile(filePath) && 0 == _access(filePath, 0);
    } 
    /**
     * @brief �����ļ��Ĵ�С
     * 
     * @param strPath 
     * @return size_t 
     */
    static size_t FileSize(const char* strPath)
    {
        struct _stat buf;
        VALRET_ASSERT(0 == _stat(strPath, &buf), -1);
        VALRET_ASSERT(S_IFREG == (buf.st_mode & S_IFREG) && 0 == _access(strPath, 0), -1);
        return buf.st_size;
    }
    /**
     * @brief ��ȡָ���ļ���Ŀ¼����ʱ�ı���ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool GetCreationTime(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTime(tmpInfo.st_ctime, dt);
    }
    /**
     * @brief ��ȡָ���ļ���Ŀ¼����ʱ�� UTC ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool GetCreationTimeUTC(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTimeUTC(tmpInfo.st_ctime, dt);
    }
     /**
     * @brief ��ȡָ���ļ���Ŀ¼������ʵı���ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool GetLastAccessTime(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTime(tmpInfo.st_atime, dt);
    }
    /**
     * @brief ��ȡָ���ļ���Ŀ¼������ʵ� UTC ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool GetLastAccessTimeUTC(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTimeUTC(tmpInfo.st_atime, dt);
    }
     /**
     * @brief �����ϴ�д��ָ���ļ���Ŀ¼�����ں�ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool GetLastWriteTime(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTime(tmpInfo.st_mtime, dt);
    }
    /**
     * @brief �����ϴ�д��ָ���ļ���Ŀ¼�����ں� UTC ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool GetLastWriteTimeUTC(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTimeUTC(tmpInfo.st_mtime, dt);
    }
    /**
     * @brief ��ָ���ļ��ƶ�����λ�ã��ṩָ�����ļ����͸���Ŀ���ļ���������Ѵ��ڣ���ѡ��
     * 
     * @param srcPath 
     * @param destPath 
     * @param overwrite 
     * @return true 
     * @return false 
     */
    static bool Move(const char* srcPath, const char* destPath, bool overwrite = true)
    {
        size_t len = FileSize(srcPath);
        ERROR_ASSERT(-1 != len, IOException);
        FileStream src(srcPath);
        FileStream dest = overwrite ? FileStream(destPath, FileMode::Create): FileStream(destPath, FileMode::CreateNew);
        char* buf = (char*)malloc(len);
        size_t readedLen = src.Read(buf, len);
        dest.Write(buf, readedLen);
        dest.Flush();
        free(buf);
        return File::Delete(srcPath);
    }
    /**
     * @brief ��ָ��·���ϵ� FileStream�����д�����д���/д���ʵ�ָ��ģʽ��ָ���Ĺ���ѡ��
     * 
     * @param filePath 
     * @return FileStream* 
     */
    static FileStream* Open(const char* filePath, FileMode mode, FileAccess access, FileShare share = FileShare::None)
    {
        return new FileStream(filePath, mode, access, share);
    }
    static FileStream* Open(const char* filePath)
    {
        return new FileStream(filePath, FileMode::Open, FileAccess::ReadWrite, FileShare::None);
    }
    /**
     * @brief �������ļ��Խ��ж�ȡ
     * 
     * @param filePath 
     * @return FileStream* 
     */
    static FileStream* OpenRead(const char* filePath)
    {
        return new FileStream(filePath, FileMode::Open, FileAccess::Read, FileShare::None);
    }
    /**
     * @brief ��һ�������ļ��򴴽�һ�����ļ��Խ���д��
     * 
     * @param filePath 
     * @return FileStream* 
     */
    static FileStream* OpenWrite(const char* filePath)
    {
        return new FileStream(filePath, File::Exists(filePath) ? FileMode::Open : FileMode::CreateNew, FileAccess::Write, FileShare::None);
    }
    /**
     * @brief ��һ���������ļ������ļ������ݶ���һ���ֽ����飬Ȼ��رո��ļ�
     * 
     * @param filePath 
     * @param buffer 
     * @param bufLen 
     * @return size_t 
     */
    static size_t ReadAllBytes(const char* filePath, char* buffer, size_t bufLen)
    {
        FileStream fs(filePath, FileMode::Open, FileAccess::Read, FileShare::None, FileType::Binary);
        size_t bufStep = 1024 > bufLen ? bufLen : 1024;
        size_t total   = 0;
        size_t readedLen = 0;
        size_t currentPos = 0;
        while(total < bufLen && 0 < (readedLen =  fs.Read(buffer + currentPos, bufStep)))
        {
            currentPos += readedLen;
            total += readedLen;
        }
        return readedLen;
    }
    static Task* ReadAllBytesAsync(const char* filePath, char* buffer, size_t& bufLen)
    {
        Task* t = new Task([](AsyncState args)->void
        {
            void** c = (void**)args;
            ERROR_ASSERT(ReadAllBytes(static_cast<const char*>(c[0]), static_cast<char*>(c[1]), *static_cast<size_t*>(c[2])), Unknown);
        }, CreateAsyncState(const_cast<char*>(filePath), buffer, &bufLen));
        t->Start();
        return t;
    }
    /**
     * @brief ��һ���ı��ļ������ļ��������ж���һ���ַ������飬Ȼ��رո��ļ�
     * 
     * @param filePath 
     * @param lines 
     * @param maxLineWidth 
     */
    static void ReadAllLines(const char* filePath, MallocList<char*> &lines, size_t maxLineWidth = 1024)
    {
        FileStream fs(filePath, FileMode::Open, FileAccess::Read, FileShare::None, FileType::Text);
        for(;;)
        {
            char* p = (char*)malloc(maxLineWidth);
            if(FileStreamEx::Get(fs, p, maxLineWidth))
            {
                lines.Add(p);
                printf("%s", p);
            }
            else
            {
                free(p);
                return;
            }
        }
    }
    static Task* ReadAllLinesAsync(const char* filePath, MallocList<char*> &lines, size_t& maxLineWidth)
    {
        Task* t = new Task([](AsyncState c)->void
        {
            ReadAllLines(static_cast<const char*>(c[0]), *static_cast<MallocList<char*>*>(c[1]), *static_cast<size_t*>(c[2]));
        }, CreateAsyncState(const_cast<char*>(filePath), &lines, &maxLineWidth));
        t->Start();
        return t;
    }  
    /**
     * @brief ʹ�������ļ��������滻ָ���ļ������ݣ���һ���̽�ɾ��ԭʼ�ļ������������滻�ļ��ı���
     * 
     * @param srcPath 
     * @param destPath 
     * @param backupPath 
     */
    static void Replace(const char* srcPath, const char* destPath, const char* backupPath = "\0")
    {
        if(0 != backupPath[0])
        {
            Copy(destPath, backupPath);
        }
        if(!Move(srcPath, destPath) && 0 != backupPath[0])/* if move fail , recover backup file */
        {
            Copy(backupPath, destPath);
        }
    }
    /**
     * @brief ���ô������ļ������ں�ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool SetCreationTime(const char* strPath, const DateTime& dt)
    {
        struct _stat tmpInfo;
        VALRET_ASSERT(0 != _stat(strPath, &tmpInfo), false);
        DateTime::DateTimeToTime_t(dt, tmpInfo.st_ctime);
        return true;
    }
    /**
     * @brief �����ϴη���ָ���ļ������ں�ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool SetLastAccessTime(const char* strPath, const DateTime& dt)
    {
        struct _stat tmpInfo;
        VALRET_ASSERT(0 != _stat(strPath, &tmpInfo), false);
        DateTime::DateTimeToTime_t(dt, tmpInfo.st_atime);
        return true;
    }
    /**
     * @brief �����ϴ�д��ָ�����ļ������ں�ʱ��
     * 
     * @param strPath 
     * @param dt 
     * @return true 
     * @return false 
     */
    inline static bool SetLastWriteTime(const char* strPath, const DateTime& dt)
    {
        struct _stat tmpInfo;
        VALRET_ASSERT(0 != _stat(strPath, &tmpInfo), false);
        DateTime::DateTimeToTime_t(dt, tmpInfo.st_mtime);
        return true;
    }
    /**
     * @brief ����һ�����ļ���������д��ָ�����ֽ����飬Ȼ��رո��ļ��� ���Ŀ���ļ��Ѵ��ڣ��򸲸Ǹ��ļ�
     * 
     * @param filePath 
     * @param buffer 
     */
    static void WriteAllBytes(const char* filePath, char* buffer, size_t bufLen)
    {
        FileStream* fs = Open(filePath, FileMode::Create, FileAccess::Write);
        VOIDRET_ASSERT(NULL != fs);
        fs->Write(buffer, bufLen);
    }
    static Task* WriteAllBytesAsync(const char* filePath, char* buffer, size_t& bufLen)
    {
        Task* t = new Task([](AsyncState c)->void
        {
            WriteAllBytes(static_cast<const char*>(c[0]), static_cast<char*>(c[1]), *static_cast<size_t*>(c[2]));
        }, CreateAsyncState(const_cast<char*>(filePath), buffer, &bufLen));
        t->Start();
        return t;
    }
    /**
     * @brief ����һ�����ļ���������д��һ�������ַ�����Ȼ��رո��ļ�
     * 
     * @param filePath 
     * @param lines 
     */
    static void WriteAllLines(const char* filePath, IEnumerable<char*> &lines)
    {        
        FileStream* fs = Open(filePath, FileMode::Create, FileAccess::Write);
        IEnumerator<char*>* it = lines.GetEnumerator();
        while(it->MoveNext())
        {
            FileStreamEx::WriteLine(*fs, *it->Current());
        }
        delete fs;
    }
    static Task* WriteAllLinesAsync(const char* filePath, IEnumerable<char*> &lines)
    {
        Task* t = new Task([](AsyncState c)->void
        {
            WriteAllLines(static_cast<const char*>(c[0]), *static_cast<IEnumerable<char*>*>(c[1]));
        }, CreateAsyncState(const_cast<char*>(filePath), &lines));
        t->Start();
        return t;
    }  
    static void WriteAllLines(const char* filePath, char** lines, size_t count)
    {        
        FileStream* fs = Open(filePath, FileMode::Create, FileAccess::Write);
        for(size_t i = 0; i < count; i++)
        {
            FileStreamEx::WriteLine(*fs, lines[i]);
        }
        delete fs;
    }
    static Task* WriteAllLinesAsync(const char* filePath, char** lines, size_t &count)
    {
        Task* t = new Task([](AsyncState c)->void
        {
            WriteAllLines(static_cast<const char*>(c[0]), static_cast<char**>(c[1]), *static_cast<size_t*>(c[2]));
        }, CreateAsyncState(const_cast<char*>(filePath), lines, &count));
        t->Start();
        return t;
    } 
};

#endif