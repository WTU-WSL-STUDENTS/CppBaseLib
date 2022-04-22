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
using PluginController = PluginLoader<std::string/* PluginToken ����Ϊ PluginLoader::map �� key, Ҳ��Ϊ CreatePlugin �����ָ����� */>;


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
		CANARY_ASSERT(pc.LoadPlugin(it, buffer)); /* ������غ��Զ�����һ���̴߳�����ҵ�� */
	}

	/* ����ģ�����������̹߳��𼴿� */
	System::Console::ReadKey();
	pc.ClearPlugin();
	printf("Success exit\n");
	return 0;
}