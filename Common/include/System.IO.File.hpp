/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-07 21:16:24
 * @LastEditors: like
 * @LastEditTime: 2022-02-20 22:12:38
 */
#ifndef SYSTEM_IO_FILE_HPP
#define SYSTEM_IO_FILE_HPP
#include <System.IO.FileStream.hpp>
#include <System.IEnumerator.h>
#include <System.DateTime.hpp>

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
    /**
     * @brief 将指定的字符串追加到文件中，如果文件还不存在则创建该文件
     * 
     * @param filePath 
     * @param lines 
     */
    static void AppendAllLines(const char* filePath, const char* lines[])
    {
        FileStream fs(filePath, Append);
        for(size_t i = 0; i < sizeof(lines) / sizeof(char*); i++)
        {
            FileStreamEx::WriteLine(fs, lines[i]);
        }
    }
    static void AppendAllLines(const char* filePath, IEnumerable<const char*> &lines)
    {
        FileStream fs(filePath, Append);
        IEnumerator<const char*>* it = lines.GetEnumerator();
        while(it->MoveNext())
        {
            FileStreamEx::WriteLine(fs, *it->Current());
        }
    }
    static Task AppendAllLinesAsync(const char* filePath, const char* lines[])
    {
        std::function func = [&]()->void
        {
            AppendAllLines(filePath, lines);
        };
        return Task();
    }
    static Task AppendAllLinesAsync(const char* filePath, IEnumerable<const char*> &lines)
    {
        std::function func = [&]()->void
        {
            AppendAllLines(filePath, lines);
        };
        return Task();
    }
    /**
     * @brief 将指定的字符串追加到文件中，如果文件还不存在则创建该文件
     * 
     * @param filePath 
     * @param str 
     */
    static void AppendAllText(const char* filePath, const char* str)
    {
        FileStream fs(filePath, Append);
        fs.Write(str, strlen(str));
    }
    static Task AppendAllTextAsync(const char* filePath, const char* str)
    {
        std::function func = [&]()->void
        {
            AppendAllText(filePath, str);
        };
        return Task();
    }
    /**
     * @brief 将现有文件复制到新文件。 允许覆盖同名的文件
     * 
     * @param srcPath 
     * @param destPath 
     * @param overwrite 
     */
    static void Copy(const char* srcPath, const char* destPath, bool overwrite = true)
    {
        size_t len = FileSize(srcPath);
        FILE_DEBUG(-1 != len);

        FileStream src(srcPath, FileMode::Open);
        FileStream dest = overwrite ? FileStream(destPath, FileMode::Create): FileStream(destPath, FileMode::CreateNew);
        Task task = src.CopyToAsync(dest, len);
        /* wait */
    }
    /**
     * @brief 在指定路径中创建或覆盖文件
     * 
     * @param filePath 
     * @return FileStream* 
     */
    static FileStream* Create(const char* filePath)
    {
        return new FileStream(filePath, FileMode::Create);
    }
    /**
     * @brief 解密由使用 Encrypt(String) 方法当前帐户加密的文件
     * 
     * @param path 
     */
    static void Decrypt (string path)
    {
        throw "Not support";
    }
    /**
     * @brief 删除指定的文件
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
     * @brief 将某个文件加密，使得只有加密该文件的帐户才能将其解密
     * 
     * @param path 
     */
    static void Decrypt (string path)
    {
        throw "Not support";
    }
    /**
     * @brief 确定指定的文件是否存在
     * 
     * @param filePath 
     * @return true 
     * @return false 
     */
    static bool Exists(const char* filePath)
    {
        return IsFile(filePath) && 0 == _access(strPath, 0);
    } 
    /**
     * @brief 获取指定文件或目录创建时的本地时间
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
     * @brief 获取指定文件或目录创建时的 UTC 时间
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
     * @brief 获取指定文件或目录最近访问的本地时间
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
     * @brief 获取指定文件或目录最近访问的 UTC 时间
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
     * @brief 返回上次写入指定文件或目录的日期和时间
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
     * @brief 返回上次写入指定文件或目录的日期和 UTC 时间
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
    static bool Move(const char* srcPath, const char* destPath, bool overwrite = true)
    {
        FileStream src(srcPath);
        FileStream dest = overwrite ? FileStream(destPath, FileMode::Create): FileStream(destPath, FileMode::CreateNew);
        src.CopyToAsync(dest, Fil)/* https://docs.microsoft.com/zh-cn/dotnet/api/system.io.file.move?view=net-5.0 */
        return true;
    }
    static size_t FileSize(const char* filePath)
    {
        struct _stat buf;
        VALRET_ASSERT(0 == _stat(strPath, &buf), -1);
        VALRET_ASSERT(S_IFREG == (buf.st_mode & S_IFREG) && 0 == _access(strPath, 0), -1);
        return buf.st_size;
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