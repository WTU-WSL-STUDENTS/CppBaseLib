/*
 * @Description: ���в����Ҫ�̳и�ͷ�ļ��� ���� IPLUGIN_EXPORT_IMPL ���̳� IPluginBase
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
 * @brief ���в������ĳ���ӿ�
 * 
 */
class IPluginBase : public std::enable_shared_from_this<IPluginBase>
{
public:
    virtual ~IPluginBase() = default;
};

IPLUGIN_EXPORT_DECLARE
#endif