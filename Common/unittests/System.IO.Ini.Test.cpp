/*
 * @Description: ¶ÁĞ´iniÎÄ¼ş²âÊÔÓÃÀı 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-08-11 10:34:07
 * @LastEditors: like
 * @LastEditTime: 2021-08-11 14:58:33
 */
#include <System.IO.Ini.hpp>
using namespace System::IO;

#define EXENAME "System.IO.Ini.Test.exe"
#define ININAME_IN "\\test.ini"
#define ININAME_OUT "\\test_out.ini"
int main(int argc, char *argv[])
{
    IniFile ini;
    char pathIn[260] = {0};
    char pathOut[260] = {0};
    int len = strlen(argv[0]) - sizeof(EXENAME);
    memcpy(pathIn, argv[0], len);
    memcpy(pathOut, argv[0], len);
    if(!ReadIni(strcat(pathIn, ININAME_IN), ini))
    {
        printf("ReadIni Failed\n");
        return 1;
    }
    if(!WriteIni(strcat(pathOut, ININAME_OUT), ini))
    {
        printf("WriteIni Failed\n");
        return 1;
    }
    for(IniFile::iterator it = ini.begin(); it != ini.end(); it++)
    {
        printf("[%s]\n", it->section);
        for(SectionData::iterator it1 = it->data.begin(); it1 != it->data.end(); it1++)
        {
            printf("%s=%s\n", it1->key, it1->val);
        }
    }
}