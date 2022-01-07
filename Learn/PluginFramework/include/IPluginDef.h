/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-01-06 15:01:12
 * @LastEditors: like
 * @LastEditTime: 2022-01-06 16:18:31
 */
#ifndef IPLUGIN_DEF_H
#define IPLUGIN_DEF_H

#include <string>

using namespace std;

#ifndef __WINDOWS
#define IPLUGIN_EXPORT 
#define IPLUGIN_IMPORT
#else
#define IPLUGIN_EXPORT __declspec(dllexport)
#define IPLUGIN_IMPORT __declspec(dllimport)
#endif

#define IPLUGIN_EXPORT_DECLARE  \
extern "C"                      \
{                               \
    IPLUGIN_EXPORT IPlugin* CreatePlugin(string strPluginDir, string strPluginFileName);  \
    IPLUGIN_EXPORT void DistroyPlugin(IPlugin* pPluginObj);                                         \
}

/* ovrridedIPluginTypeName 是重载 IPlugin 接口的派生类 */
#define IPLUGIN_EXPORT_IMPL(ovrridedIPluginTypeName)    \
ovrridedIPluginTypeName* g_pPlugObj;                    \
IPlugin* CreatePlugin(std::string strPluginDir, std::string strPluginFileName)  \
{                                                                               \
    return g_pPlugObj ? g_pPlugObj : g_pPlugObj =                               \
        new ovrridedIPluginTypeName(strPluginDir, strPluginFileName);           \
}                                                                               \
void DistroyPlugin(IPlugin* pPluginObj) \
{                                       \
    if(g_pPlugObj)                      \
    {                                   \
        delete pPluginObj;              \
        pPluginObj = NULL;              \
    }                                   \
}

/* 插件文件加载操作 */
typedef void (*OnPluginBeginLoad)(void* sender, void* args);
typedef void (*OnPluginEndLoad)(void* sender, void* args);
/* 插件调用 Init 操作 */
typedef void (*OnPluginBeginInit)(void* sender, void* args);
typedef void (*OnPluginEndInit)(void* sender, void* args);
/* 插件调用 Stop 操作 */
typedef void (*OnPluginBeginStop)(void* sender, void* args);
typedef void (*OnPluginEndStop)(void* sender, void* args);

class PluginController;

class IPLUGIN_EXPORT IPlugin
{
    friend class PluginController;
public:
    IPlugin(string strPluginDir, string strPluginFileName) : 
        m_strPluginDir(strPluginDir), 
        m_strPluginFileName(strPluginFileName),
        m_strPluginConfigPath(strPluginDir + "\\Config" + strPluginFileName + ".json"),
        m_strPluginFilePath(strPluginDir + "\\" + strPluginFileName),
        m_pOnPluginBeginLoad(NULL),
        m_pOnPluginEndLoad(NULL),
        m_pOnPluginBeginInit(NULL),
        m_pOnPluginEndInit(NULL),
        m_pOnPluginBeginStop(NULL),
        m_pOnPluginEndStop(NULL)     
    {
    }
    virtual ~IPlugin()  {}
    virtual bool Init() {}
    virtual bool Stop() {}
    
    string m_strPluginConfigPath;
    string m_strPluginFilePath;
protected:
    string m_strPluginDir;
    string m_strPluginFileName;
private:
    /* for Plugin Controller */
    OnPluginBeginLoad   m_pOnPluginBeginLoad;
    OnPluginEndLoad     m_pOnPluginEndLoad;
    OnPluginBeginInit   m_pOnPluginBeginInit;
    OnPluginEndInit     m_pOnPluginEndInit;
    OnPluginBeginStop   m_pOnPluginBeginStop;
    OnPluginEndStop     m_pOnPluginEndStop;
    inline bool InitWithEvent()
    {
        if(m_pOnPluginBeginInit)
            (*m_pOnPluginBeginInit)(this, NULL);
        bool bRet = Init();
        if(m_pOnPluginEndInit)
            (*m_pOnPluginEndInit)(this, NULL);
        return bRet;
    }
    inline bool StopWithEvent()
    {
        if(m_pOnPluginBeginStop)
            (*m_pOnPluginBeginStop)(this, NULL);
        bool bRet = Stop();
        if(m_pOnPluginEndStop)
            (*m_pOnPluginEndStop)(this, NULL);
        return bRet;
    }
};

#endif