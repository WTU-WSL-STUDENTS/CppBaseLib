/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-19 17:04:43
 * @LastEditors: like
 * @LastEditTime: 2022-04-19 19:01:13
 */
#include <PluginLoader.hpp>
#include <string>
using PluginController = PluginLoader<std::string/* PluginToken 既作为 PluginLoader::map 的 key, 也作为 CreatePlugin 传入的指针参数 */>;


int main(int argc, char** args)
{
	PluginController pc;
	char buffer[MAX_PATH];
	for (size_t i = strlen(args[0]) - 1; i > 0; i--)
	{
		if ('\\' == args[0][i])
		{
			args[0][i] = 0;
			break;
		}
	}
	std::initializer_list list = { "Filesystem" };
	
	for (auto& it : list)
	{
		sprintf(buffer, "%s\\%s.dll", args[0], it);
		//pc.GetInstance<Filesystem>(it);
		CANARY_ASSERT(pc.LoadPlugin(it, buffer)); /* 插件加载后，自动开启一个线程处理插件业务 */
	}

	/* 所有模块启动，主线程挂起即可 */
	System::Console::ReadKey();
	pc.ClearPlugin();
	printf("Success exit\n");
	return 0;
}