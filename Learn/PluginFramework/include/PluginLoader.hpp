/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-19 17:32:27
 * @LastEditors: like
 * @LastEditTime: 2022-04-19 19:01:26
 */
#ifndef PLUGIN_LOADER_HPP
#define PLUGIN_LOADER_HPP
#include <map> 
#include <IPluginBase.h>
#include <System.IO.File.hpp>
#include <System.IO.Directory.hpp>
#include <System.Console.hpp>

using PFuncCreatePlugin = IPluginBase & (*)(void* context);

template<typename PluginToken>
class PluginLoader
{
    class PluginContext
    {
        friend class PluginLoader<PluginToken>;
        HINSTANCE dll;
        std::shared_ptr<IPluginBase> ptr;
        PluginContext(const PluginToken& token,const char* path) : dll(LoadLibrary(path)), ptr(nullptr)
		{
			CANARY_ASSERT(dll);
            PFuncCreatePlugin  func = (PFuncCreatePlugin)GetProcAddress(dll, "CreatePlugin");
            CANARY_ASSERT(func);
            ptr = std::make_shared<IPluginBase>(func((void*)&token));
        }
        ~PluginContext()
        {
            FreeLibrary(dll);
        }
    };
private:
    std::map<PluginToken, PluginContext*> map;
public:
    PluginLoader()  = default;
    ~PluginLoader() = default;
    bool LoadPlugin(const PluginToken& token, const char* path)
    {
        if (map.find(token) != map.end())
        {
            System::Console::WriteLine("token already exists : ", token);
            return false;
        }
        if (!System::IO::File::Exists(path))
        {
            System::Console::WriteLine("plugin not exists : ", token);
            return false;
        }
        PluginContext* plugin = new PluginContext(token, path);
        if (nullptr == plugin->ptr)
        {
            System::Console::WriteLine("create plugin failed");
            delete plugin;
            return false;
        }
        map[token] = plugin;
        return true;
    }
    void UnloadPlugin(const PluginToken& token)
	{
        auto it = map.find(token);
        VOIDRET_ASSERT(it != map.end());
       
        SAVE_DELETE_PTR(it->second)
        map.erase(it);
    }
    void ClearPlugin()
    {
        auto it = map.begin();

        while (it != map.end())
		{
			SAVE_DELETE_PTR(it->second)
			map.erase(it);
            it = map.begin();
        }
    }
    template<typename PlugType>
    std::shared_ptr<PlugType> GetInstance(const PluginToken &token)
    {
        return std::static_pointer_cast<PlugType>(map[token]->ptr);
    }
};

using PluginController = PluginLoader<std::string>;
void LoadDirecotry(PluginController& pc, const char* path)
{
    VOIDRET_ASSERT(System::IO::Directory::Exists(path));
    StackList<std::string> list;
    System::IO::Directory::GetFiles(list, path, "*.dll");
    char buffer[MAX_PATH];
    for (int i = 0; i < list.Count(); i++)
    {
        sprintf("%s\\%s", path, list[i].c_str());
        pc.LoadPlugin(list[i].substr(0, list[i].size() - 4), buffer);
    }
}

#endif