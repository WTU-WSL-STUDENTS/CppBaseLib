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
        /* �������������а�����ǰĿ¼ */
        TopDirectoryOnly,   

        /* �����������а�����ǰĿ¼������������Ŀ¼ */
        AllDirectories      
    };
    /**
     * @brief �ж�ָ��·���Ƿ�ΪĿ¼
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
     * @brief �ж�ָ��·���Ƿ�ΪĿ¼
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
     * @brief ������������, ������������� func ���˺� �ļ� / Ŀ¼ ���ƴ���������
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
     * @brief ������������, ������������� func ����󽫽������������
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
            //û��ƥ���ļ�
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
     * @brief ��������ָ����·���У���ȡ����Ŀ¼(�������)
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
     * @brief ��ָ��·���д�������Ŀ¼
     * 
     * @param strDirPath Ŀ¼�ľ���·�� , �ָ����� \\ ������ /
     * @return true 
     * @return false 1. ·����ʽ����; 3. Ȩ�޲���
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
     * @brief ���� shell ����ָ��Ŀ¼, �� src ������ dest\\src
     * 
     * @param src ���Ի����·��
     * @param dest ���Ի����·��
     * @return true 
     * @return false 
     */
    static bool CopyDirectory(const char* src, const char* dest, bool overwrite)
    {
#if defined(__linux) || defined(__APPLE__) || defined(__CYGWIN__)
        if (access(src, F_OK))
        {
            return false;  //Դ�ļ�������
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
     * @brief ɾ����Ŀ¼
     * 
     * @param strEmptyDirPath Ŀ¼�ľ��Ի����·��
     * @return true 
     * @return false 1. Ŀ¼�ǿ�; 2. Ȩ�޲���
     */
    inline static bool Delete(const char* strEmptyDirPath)
    {
        return 0 == _rmdir(strEmptyDirPath);
    }
    /**
     * @brief ɾ����Ŀ¼���ߵ�ָ�����õݹ�ʱ�ݹ�ɾ��Ŀ¼����Ŀ¼�µ�����
     * 
     * @param strDirPath Ŀ¼�ľ��Ի����·��
     * @param recursive �Ƿ�ݹ�ɾ�����ļ�����
     * @return true 
     * @return false 1. Ȩ�޲���
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

        /* ɾ����ǰĿ¼ */
        return Delete(strDirPath);

        return true;
#endif
    }
    /**
     * @brief �ж�Ŀ¼�Ƿ����
     * 
     * @param strPath 
     * @return true ���·��ָ��������Ч��Ŀ¼, ������Ȩ���ܹ����ʸ�·��
     * @return false 
     */
    inline static bool Exists(const char* strPath)
    {
        return IsDirectory(strPath) && 0 == _access(strPath, 0);
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
     * @brief ��ȡ����Ĺ���Ŀ¼
     * 
     * @param buf ����Ϊ _MAX_PATH ���ڴ��
     */
    inline static void GetCurrentDirectory(char (&buf)[_MAX_PATH])
    {
        ERROR_ASSERT(_getcwd(buf, sizeof(buf)), "GetCurrentDirectory failed");
    }
    /**
     * @brief ��ȡָ��Ŀ¼�µĵ�һ����Ŀ¼�б�
     * 
     * @param strPath 
     * @param strSearchPattern Ҫ�� strPath �е���Ŀ¼������ƥ��������ַ����� �˲������԰�����Ч�ı���ͨ�������ϣ�����֧��������ʽ
     *  p* ����ĸ p ��ͷ����������
     *  *s ����ĸ s ��β����������
     *  ?est �������Ƶĵ�һ���ַ�, ���ָ�ʽ���� est ����������
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
     * @brief ��ȡָ��Ŀ¼�µĵݹ�(��ѡ)��Ŀ¼�б�
     * 
     * @param strPath 
     * @param strSearchPattern Ҫ�� strPath �е���Ŀ¼������ƥ��������ַ����� �˲������԰�����Ч�ı���ͨ�������ϣ�����֧��������ʽ
     *  p* ����ĸ p ��ͷ����������
     *  *s ����ĸ s ��β����������
     *  ?est �������Ƶĵ�һ���ַ�, ���ָ�ʽ���� est ����������
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
     * @brief ����ָ��Ŀ¼����ָ��������ģʽƥ����ļ������Ƽ���׺
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
     * @brief ��������ָ�������������ļ�����Ŀ¼������
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
     * @brief �ƶ��ļ���, ͨ������ & ɾ�� ʵ���ƶ�����, ���ɾ��ʧ��, �Ѿ�ִ�еĸ��Ʋ������᳷��
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