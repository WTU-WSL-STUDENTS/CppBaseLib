/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-06 16:29:57
 * @LastEditors: like
 * @LastEditTime: 2022-02-21 19:58:33
 */
#ifndef SYSTEM_IO_DIRECTORY_HPP
#define SYSTEM_IO_DIRECTORY_HPP

#include <System.IO.File.hpp>
#include <System.List.hpp>
#include <System.DateTime.hpp>
#include <string>
#include <functional>

#if defined(COMPLIER_MSVC)
#include <ShlObj.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/io.h>
#include <dirent.h>
#endif

using namespace std;

namespace System::IO
{
    class Directory;
    enum  class SearchOption
    {
        /* 仅在搜索操作中包括当前目录 */
        TopDirectoryOnly,   

        /* 在搜索操作中包括当前目录和所有它的子目录 */
        AllDirectories      
    };
    /**
     * @brief 判断指定路径是否为目录
     * 
     * @param buf 
     * @return true 
     * @return false 
     */
    inline static bool IsDirectory(struct _stat buf)
    {
        return S_IFDIR == (buf.st_mode & S_IFDIR);
    }
    /**
     * @brief 判断指定路径是否为目录
     * 
     * @param strPath 
     * @return true 
     * @return false 
     */
    inline static bool IsDirectory(const char* strPath)
    {
        struct _stat buf;
        if (0 == _stat(strPath, &buf))
            return S_IFDIR == (buf.st_mode & S_IFDIR);
        return false;
    }
    /**
     * @brief 按照搜索条件, 将搜索结果按照 func 过滤后将 文件 / 目录 名称存入容器中
     * 
     */
    bool Search_If(const char* path, const char* searchPattern, StackList<std::string>& results, 
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
    std::function<bool(const dirent&)>
#else
    std::function<bool(const _finddata_t&)>
#endif
    func )
    {
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        char condition[_MAX_PATH] = {0};
        strcat
        (
            strcat
            (
                strcpy(condition, path), 
                "\\"
            ), 
            searchPattern
        );  

        struct dirent **namelist;
        int n;
        int i;
        n = scandir(condition, &namelist, NULL, alphasort);
        if (n < 0)
        {
            return false;
        }
        else
        {
            for (i = 0; i < n; i++)
            {
                if (strcmp(namelist[i]->d_name, ".") && strcmp(namelist[i]->d_name, "..") && func(*(namelist[i]))) 
                {
                    results.Add(tmp);
                }
                free(namelist[i]);
            }
            free(namelist);
        }
        return true;
#else
        char condition[_MAX_PATH] = {0};
        strcat
        (
            strcat
            (
                strcpy(condition, path), 
                "\\"
            ), 
            searchPattern
        );  

        struct _finddata_t file_data;
        intptr_t hFile;
        hFile = _findfirst(condition, &file_data);
        if (-1L == hFile)
        {
            return false;
        }
        do{
            string tmp = file_data.name;
            
            if (strcmp(file_data.name, ".") && strcmp(file_data.name, "..") && func(file_data)) 
            {
                results.Add(tmp);
            }
        }while (0 == _findnext(hFile, &file_data));
        _findclose(hFile);
        return results.Count();
#endif
    }
    /**
     * @brief 按照搜索条件, 将搜索结果按照 func 组包后将结果存入容器中
     * 
     */
    bool Search_With(const char* path, const char* searchPattern, StackList<std::string>& results, 
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
    std::function<void(const char*, const dirent&, StackList<std::string>&)>
#else
    std::function<void(const char*, const _finddata_t&, StackList<std::string>&)>
#endif
    func )
    {
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        char condition[_MAX_PATH] = {0};
        strcat
        (
            strcat
            (
                strcpy(condition, path), 
                "\\"
            ), 
            searchPattern
        );  

        struct dirent **namelist;
        int n;
        int i;
        n = scandir(condition, &namelist, NULL, alphasort);
        if (n < 0)
        {
            return false;
        }
        else
        {
            for (i = 0; i < n; i++)
            {
                if (strcmp(namelist[i]->d_name, ".") && strcmp(namelist[i]->d_name, "..")) 
                {
                    func(path, *(namelist[i]), results);
                }
                free(namelist[i]);
            }
            free(namelist);
        }
#else
        char condition[_MAX_PATH] = {0};
        strcat
        (
            strcat
            (
                strcpy(condition, path), 
                "\\"
            ), 
            searchPattern
        );   
        struct _finddata_t file_data;
        intptr_t hFile;
        hFile = _findfirst(condition, &file_data);
        if (-1L == hFile)
        {
            //没有匹配文件
            return false;
        }
        do{
            if (strcmp(file_data.name, ".") && strcmp(file_data.name, "..")) 
            {
                func(path, file_data, results);
            }
        }
        while (0 == _findnext(hFile, &file_data));
        _findclose(hFile);
#endif
        return results.Count();
    }
};

class System::IO::Directory
{
private:
    /**
     * @brief 从容器中指定的路径中，读取其子目录(广度优先)
     * 
     * @param container 
     * @param begin 
     * @param count 
     * @return true 
     * @return false 
     */
    static bool _GetCurrentSubDirectorys(StackList<std::string>& container, size_t& begin, size_t& count)
    {
        char path[_MAX_PATH] = {0};
        for(;begin < count; begin++) /* search all sub dir at same level */
        {
            memset(path, 0, sizeof(path));
            strcpy(path, container[begin].c_str());
            Search_With(path, "*", container,
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
            [](const char* path, const dirent& d, StackList<std::string>& container)->void 
            { 
                if(DT_DIR & d.d_tpye) 
                {
                    char buf[_MAX_PATH];
                    strcat
                    (
                        strcat
                        (
                            strcpy(buf, path),
                            "\\"
                        ),
                        d.d_name
                    );
                    container.Add(std::string(buf));
                }
            });
#else
            [](const char* path, const _finddata_t& d, StackList<std::string>& container)->void 
            { 
                if(_A_SUBDIR & d.attrib)
                {
                    char buf[_MAX_PATH];
                    strcat
                    (
                        strcat
                        (
                            strcpy(buf, path),
                            "\\"
                        ),
                        d.name
                    );
                    container.Add(std::string(buf));
                } 
            });
#endif  
        }
        if(count >= container.Count())
        {
            return false;
        }
        count = container.Count();
        return true;
    }
public:
    /**
     * @brief 在指定路径中创建所有目录
     * 
     * @param strDirPath 目录的绝对路径 , 分隔符用 \\ 不能用 /
     * @return true 
     * @return false 1. 路径格式错误; 3. 权限不足
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
                if (access(strDirPath, 0) != 0)
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
        if(Exists(strDirPath))
        {
            return true;
        }
        if(SHCreateDirectoryEx(NULL, strDirPath, NULL))
        {
            printf("Directory::CreateDirectory Failed , Error Code %d\n", GetLastError());
            return false;
        }
        return true;
#endif
    }
    /**
     * @brief 调用 shell 复制指定目录, 将 src 拷贝至 dest\\src
     * 
     * @param src 绝对或相对路径
     * @param dest 绝对或相对路径
     * @return true 
     * @return false 
     */
    static bool CopyDirectory(const char* src, const char* dest, bool overwrite)
    {
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        if (access(src, F_OK))
        {
            return false;  //源文件不存在
        }
        bool bRet = false;

        DIR *dirp = NULL;
        //1.open the dir
        if(NULL == (dirp = opendir(src)))
        {
            return false;
        }

        struct dirent *entp = NULL;
        //2.read the dir
        while(NULL != (entp = readdir(dirp)))
        {
            if( 0 == (strcmp(entp->d_name,"..")) || 0 == (strcmp(entp->d_name, ".")))
            {
                continue;
            }

            char src_buf[100] = "";
            char dest_buf[100] = "";

            sprintf(src_buf, "%s/%s", src, entp->d_name);
            sprintf(dest_buf, "%s/%s", dest, entp->d_name);

            struct stat src_stat;

            if( stat(src_buf,&src_stat) )
            {
                goto finish; //get stat failed
            }
            if( S_ISREG(src_stat.st_mode) )
            {
                int iRet , iRead , size , iWrite;
                FILE* fS = NULL , *fD = NULL;
                char* buffer = NULL;
                iRet = access(src_buf, F_OK);
                if (iRet)
                {
                    return false;
                }
                fS = fopen(src_buf, "r");
                if (fS == NULL)
                {
                    return false;
                }
                fD = fopen(dest_buf, "w+");
                if (fD == NULL)
                {
                    goto finish;
                }

                fseek(fS, 0, SEEK_END);
                size = ftell(fS);
                fseek(fS, 0, SEEK_SET);
                buffer = (char*) calloc(1, size + 1);
                if (!buffer)
                {
                    goto finish;
                }

                iRead = fread(buffer, 1, size, fS);
                if (iRead != size)
                {
                    goto finish;
                }

                iWrite = fwrite((void*)buffer, 1,  size, fD);
                if (iWrite != size)
                {
                    goto finish;
                }
                if (fS)
                fclose(fS);
                if (fD)
                fclose(fD);
                if (buffer)
                free(buffer);
            }
            else if( S_ISDIR(src_stat.st_mode) )
            {
                if( -1 == mkdir(dest_buf, src_stat.st_mode) )
                {
                    goto finish;
                }
                copyDirectory(src_buf, dest_buf);  //if subdir, recursive call itself
            }
        }
        bRet = true;

        finish:
        if (dirp)
            closedir(dirp);
        return bRet;
#else
	if(!Exists(src) || !Exists(dest) || 0 == strcmp(src, dest))
	{
		return false;
	}

	//SHFileOperation can deal with multi dirs, so that dir must finish with double '\0';
	char pFrom[_MAX_PATH] = {0}; 
	strcpy(pFrom, src); 
	char pTo[_MAX_PATH] = {0};   
	strcpy(pTo, dest);

	SHFILEOPSTRUCT sfo;
	ZeroMemory((void*)&sfo, sizeof(SHFILEOPSTRUCT));
	sfo.hwnd = NULL;       
	sfo.wFunc = FO_COPY;       
	sfo.pFrom = pFrom; 
	sfo.pTo = pTo;         
	sfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_RENAMEONCOLLISION;  
	if(overwrite)
	{
		sfo.fFlags = sfo.fFlags & (0xFFFF ^ FOF_RENAMEONCOLLISION);
	}
	return 0 == SHFileOperation(&sfo);
#endif
    }

    /**
     * @brief 删除空目录
     * 
     * @param strEmptyDirPath 目录的绝对或相对路径
     * @return true 
     * @return false 1. 目录非空; 2. 权限不足
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
     * @return false 1. 权限不足
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
    /**
     * @brief 判断目录是否存在
     * 
     * @param strPath 
     * @return true 如果路径指定的是有效的目录, 并且有权限能够访问该路径
     * @return false 
     */
    inline static bool Exists(const char* strPath)
    {
        return IsDirectory(strPath) && 0 == _access(strPath, 0);
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
    /**
     * @brief 获取程序的工作目录
     * 
     * @param buf 长度为 _MAX_PATH 的内存块
     */
    inline static void GetCurrentDirectory(char (&buf)[_MAX_PATH])
    {
        ERROR_ASSERT(_getcwd(buf, sizeof(buf)), "GetCurrentDirectory failed");
    }
    /**
     * @brief 获取指定目录下的第一级子目录列表
     * 
     * @param strPath 
     * @param strSearchPattern 要与 strPath 中的子目录的名称匹配的搜索字符串。 此参数可以包含有效文本和通配符的组合，但不支持正则表达式
     *  p* 以字母 p 开头的所有名称
     *  *s 以字母 s 结尾的所有名称
     *  ?est 忽略名称的第一个字符, 名字格式满足 est 的所有名称
     * @return StackList<std::string> 
     */
    static bool GetDirectorys(StackList<std::string> list, const char* strPath, const char* strSearchPattern = "*")
    {
        if(!Directory::Exists(strPath))
        {
            return false;
        }
        char path[_MAX_PATH] = {0};
        strcpy(path, strPath);
        Search_If(path, strSearchPattern, list, 
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        [](const dirent& d)->bool { return DT_DIR & d.d_tpye; });
#else
        [](const _finddata_t& d)->bool { return _A_SUBDIR & d.attrib; });
#endif  
        std::string prefix(strPath);
        prefix += "\\";
        for(size_t i = 0; i < list.Count(); i++)
        {
            list[i] = prefix + list[i]; 
        }
        return true;
    }
    /**
     * @brief 获取指定目录下的递归(可选)子目录列表
     * 
     * @param strPath 
     * @param strSearchPattern 要与 strPath 中的子目录的名称匹配的搜索字符串。 此参数可以包含有效文本和通配符的组合，但不支持正则表达式
     *  p* 以字母 p 开头的所有名称
     *  *s 以字母 s 结尾的所有名称
     *  ?est 忽略名称的第一个字符, 名字格式满足 est 的所有名称
     * @param option 
     * @return StackList<std::string> 
     */
    static bool GetDirectorys(StackList<std::string>& list, const char* strPath, const char* strSearchPattern, SearchOption option)
    {
        if(!GetDirectorys(list, strPath, strSearchPattern))
        {
            return false;
        }
        if(SearchOption::TopDirectoryOnly != option)
        {
            size_t begin = 0;
            size_t count = list.Count();
            while(_GetCurrentSubDirectorys(list, begin, count)){}
        }
        return true;
    }
    /**
     * @brief 返回指定目录中与指定的搜索模式匹配的文件的名称及后缀
     * 
     * @param strPath 
     * @param strSearchPattern 
     * @return StackList<std::string> 
     */
    static bool GetFiles(StackList<std::string>& list, const char* strPath, const char* strSearchPattern = "*.*")
    {
        if(!Directory::Exists(strPath))
        {
            return false;
        }
        Search_If(strPath, strSearchPattern, list, 
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        [](const dirent& d)->bool { return !(DT_DIR & d.d_tpye); });
#else
        [](const _finddata_t& d)->bool { return !(_A_SUBDIR & d.attrib); });
#endif   
        return true;
    }
    /**
     * @brief 返回满足指定条件的所有文件和子目录的名称
     * 
     * @param results 
     * @param strPath 
     * @param strSearchPattern 
     * @return true 
     * @return false 
     */
    static bool GetFileSystemEntries(StackList<std::string>& results, const char* strPath, const char* strSearchPattern = "*")
    {
        
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        char condition[_MAX_PATH] = {0};
        strcat
        (
            strcat
            (
                strcpy(condition, strPath), 
                "\\"
            ), 
            strSearchPattern
        );  

        struct dirent **namelist;
        int n;
        int i;
        n = scandir(condition, &namelist, NULL, alphasort);
        if (n < 0)
        {
            return false;
        }
        else
        {
            for (i = 0; i < n; i++)
            {
                if (strcmp(namelist[i]->d_name, ".") && strcmp(namelist[i]->d_name, "..")) 
                {
                    results.Add(tmp);
                }
                free(namelist[i]);
            }
            free(namelist);
        }
        return true;
#else
        char condition[_MAX_PATH] = {0};
        strcat
        (
            strcat
            (
                strcpy(condition, strPath), 
                "\\"
            ), 
            strSearchPattern
        );  

        struct _finddata_t file_data;
        intptr_t hFile;
        hFile = _findfirst(condition, &file_data);
        if (-1L == hFile)
        {
            return false;
        }
        do{
            string tmp = file_data.name;
            
            if (strcmp(file_data.name, ".") && strcmp(file_data.name, "..")) 
            {
                results.Add(tmp);
            }
        }while (0 == _findnext(hFile, &file_data));
        _findclose(hFile);
        return true;
#endif
    }
    /**
     * @brief 移动文件夹, 通过复制 & 删除 实现移动操作, 如果删除失败, 已经执行的复制操作不会撤回
     * 
     * @param src 
     * @param dest 
     * @return true 
     * @return false 
     */
    static bool Move(const char* src, const char* dest)
    {
        if(Exists(src) && CopyDirectory(src, dest, true))
        {
            if (Delete(src))
                return true;
        }
        return false;
    }
};

#endif