/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-06 16:29:57
 * @LastEditors: like
 * @LastEditTime: 2022-01-07 21:45:09
 */
#ifndef SYSTEM_IO_DIRECTORY_HPP
#define SYSTEM_IO_DIRECTORY_HPP

#include <System.IO.File.hpp>
#include <System.List.hpp>
#include <System.DateTime.hpp>
#include <string>

#if defined(COMPLIER_MSVC)
#include <ShlObj.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/io.h>
#endif

namespace System::IO
{
    inline static bool IsDirectory(struct _stat buf)
    {
        return S_IFDIR == (buf.st_mode & S_IFDIR);
    }
    class Directory;
};

class System::IO::Directory
{
    /**
     * @brief 在指定路径中创建所有目录
     * 
     * @param strDirPath 目录的绝对或相对路径
     * @return true 
     * @return false 
     */
    static bool CreateDirectory(const char* strDirPath)
    {
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        int i, len = strlen(strDirPath);
        if (strDirPath[len - 1] != '/')
            strcat(strDirPath, "/");
        len = strlen(strDirPath);

        for (i = 1; i < len; i++)
        {
            if (strDirPath[i] == '/')
            {
                strDirPath[i] = 0;
                if (access(strDirPath, NULL) != 0)
                {
#ifdef __APPLE__
                    if (_mkdir(path.c_str(), 0777) == -1)
#else
                    if (mkdir(strDirPath, _S_IREAD | _S_IWRITE) == -1)
#endif
                    {
                        return false;
                    }
                }
                strDirPath[i] = '/';
            }
        }

        return true;
#else
        return 0 == SHCreateDirectoryEx(NULL, strDirPath, NULL);
#endif
    }
    /**
     * @brief 删除空目录
     * 
     * @param strEmptyDirPath 目录的绝对或相对路径
     * @return true 
     * @return false 
     */
    inline static bool Delete(const char* strEmptyDirPath)
    {
        return 0 == _rmdir(strEmptyDirPath);
    }
    /**
     * @brief 删除空目录或者当指定启用递归时递归删除目录及其目录下的内容
     * 
     * @param strDirPath 目录的绝对或相对路径
     * @param recursive 是否递归删除子文件内容
     * @return true 
     * @return false 
     */
    inline static bool Delete(const char* strDirPath, bool recursive)
    {
        if(!recursive)
        {
            return Delete(strDirPath);
        }
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        char strCmd[MAX_PATH] =  "rm -rf ";
        strcat(strCmd, strEmptyDirPath);
        return (system(strCmd) >= 0 ? true : false);
#else
        __finddata64_t c_file;
        intptr_t hFile;
        char pBuf[_MAX_PATH] = {0};
        strcpy(pBuf, strDirPath);
        strcat(pBuf, "\\*.*");
        hFile = _findfirst64(pBuf, &c_file);
        if (hFile == -1)
        {
            return false;
        }
        do
        {
            if (strlen(c_file.name) == 1 && c_file.name[0] == '.'
                || strlen(c_file.name) == 2 && c_file.name[0] == '.' && c_file.name[1] == '.')
                continue;
            memset(pBuf, 0 ,sizeof(pBuf));
            strcat(pBuf, strDirPath);
            strcat(pBuf, "\\");
            strcat(pBuf, c_file.name);
            if (c_file.attrib & _A_SUBDIR)
            {
                Delete(pBuf, true);
            }
            else
            {
                File::Delete(pBuf);
            }
        } while (_findnext64(hFile, &c_file) == 0);

        _findclose(hFile);

        /* 删除当前目录 */
        return Delete(strDirPath);

        return true;
#endif
    }
    inline static bool Exists(const char* strPath)
    {
        return 0 == _access(strPath, 0);
    }
    inline static bool GetCreationTime(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTime(tmpInfo.st_ctime, dt);
    }
    inline static bool GetCreationTimeUTC(const char* strPath, DateTime& dt)
    {
        struct _stat tmpInfo;
        if (_stat(strPath, &tmpInfo) != 0)
        {
            return false;
        }
        return DateTime::FromFileTimeUTC(tmpInfo.st_ctime, dt);
    }
    static List<std::string> GetFiles()
    {

        List<std::string> files;
        return files;
    }
};

#endif