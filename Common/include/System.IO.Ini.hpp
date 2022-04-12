/*
 * @Description: Ini文件的读写接口
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 08:47:54
 * @LastEditors: like
 * @LastEditTime: 2022-02-21 19:12:38
 */
#ifndef SYSTEM_INI_H
#define SYSTEM_INI_H

#define SECTION_TITLE_SIZE (0xfd)
#define SECTION_TITLE_FILE_SIZE (SECTION_TITLE_SIZE + 2)
#define INI_KEY_SIZE       (0xff)
#define INI_VAL_SIZE       (0xff)
#define INI_KEYVAL_FILE_SIZE (INI_KEY_SIZE + INI_VAL_SIZE + 1)

// #ifdef _WIN32
// #include <errhandlingapi.h>
// #endif

#include <System.IO.File.hpp>
#include <vector>

using namespace std;

namespace System::IO
{
    typedef struct _IniPair
    {
        char key[INI_KEY_SIZE];
        char val[INI_VAL_SIZE];
    }IniPair;
    typedef std::vector<IniPair> SectionData;
    typedef struct _IniSection
    {
        char section[SECTION_TITLE_SIZE];
        SectionData data;
    }IniSection;
    typedef std::vector<IniSection> IniFile;
    /**
     * @description: 支持注释;//单独成行 
     * @param {ini文件路径}} const char*
     * @param {返回解析的ini文件数据} IniFile&
     * @return {路径存在且解析成功}
     * @author: like
     */    
    bool ReadIni(const char* iniPath, IniFile& readedIni)
    {
        FileStream* fs = File::OpenRead(iniPath);
        std::FILE* scan0 = FileStreamEx::FileHandle(*fs);
        IniSection sec;
        memset(sec.section, 0, sizeof(sec.section));
        char buffer[INI_KEYVAL_FILE_SIZE] = {0};
        char* splitIndex;
        while(NULL != fgets(buffer, INI_KEYVAL_FILE_SIZE, scan0))
        {
            if('[' == buffer[0])/* section */
            {
                if(0 != sec.section[0])
                {
                    readedIni.push_back(sec);
                    memset(sec.section, 0, sizeof(sec.section));
                    sec.data.clear();
                }
                char* p = buffer;
                memcpy(sec.section, (p + 1), strlen(buffer) - 3);
            }
            else if/* data */
            (
                ';' != buffer[0]                            &&  /* 当前行不是注释    */
                ('/' != buffer[0] || '/' != buffer[0])      &&  /* 当前行不是注释    */
                0 != sec.section[0]                         &&  /* 当前行属于Section */
                buffer < (splitIndex = strchr(buffer, '='))     /* 存在key=val格式   */
            )
            {
                int keyLen = splitIndex - buffer;
                int valLen = strlen(buffer) - keyLen - 2; 
                IniPair pair = {0};
                memcpy(pair.key, buffer, keyLen);
                memcpy(pair.val, (splitIndex + 1), valLen);
                sec.data.push_back(pair);
            }
            memset(buffer, 0, sizeof(buffer));
        }
        if(0 != sec.section[0])
        {
            readedIni.push_back(sec);
        }
        delete fs;
        return true;
    }
    bool WriteIni(const char* iniPath, IniFile readedIni)
    {
        FileStream * fs = File::OpenWrite(iniPath);
        std::FILE* p = FileStreamEx::FileHandle(*fs);
        for(IniFile::iterator it = readedIni.begin(); it != readedIni.end(); it++)
        {
            fprintf(p, "[%s]\n", it->section);
            for(SectionData::iterator it1 = it->data.begin(); it1 != it->data.end(); it1++)
            {
                fprintf(p, "%s=%s\n", it1->key, it1->val);
            }
        }
        delete fs;
        return true;
    }
}
#endif