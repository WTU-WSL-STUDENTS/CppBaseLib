/*
 * @Description: 所有插件需要继承该头文件， 声明 IPLUGIN_EXPORT_IMPL 并继承 IPluginBase
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-19 16:32:39
 * @LastEditors: like
 * @LastEditTime: 2022-04-19 18:57:32
 */
#ifndef IPLUGIN_FACTORY_H
#define IPLUGIN_FACTORY_H
#include <CompliedEntry.h>
#include <memory> 

#ifndef __WINDOWS
#   define IPLUGIN_EXPORT 
#   define IPLUGIN_IMPORT
#else
#   define IPLUGIN_EXPORT __declspec(dllexport)
#   define IPLUGIN_IMPORT __declspec(dllimport)
#endif

#define IPLUGIN_EXPORT_DECLARE  \
extern "C"                      \
{                               \
    IPLUGIN_EXPORT IPluginBase& CreatePlugin(WEAK_PTR(void) context);  \
}

#define IPLUGIN_EXPORT_IMPL(pluginType) \
IPluginBase& CreatePlugin(void* context)\
{                               \
    static pluginType plugin;   \
    return plugin;              \
}   

/**
 * @brief 所有插件对象的抽象接口
 * 
 */
class IPluginBase : public std::enable_shared_from_this<IPluginBase>
{
public:
    virtual ~IPluginBase() = default;
};

IPLUGIN_EXPORT_DECLARE
#endif