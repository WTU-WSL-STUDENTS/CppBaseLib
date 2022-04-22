/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-19 17:03:22
 * @LastEditors: like
 * @LastEditTime: 2022-04-19 18:53:42
 */
#include "Filesystem.h"
#include <string>

IPluginBase& CreatePlugin(WEAK_PTR(void) context)
{                               
	static Filesystem plugin;
	printf("Construct with user token : %s\n", static_cast<std::string*>(context)->c_str());
	return plugin;              
}

Filesystem::Filesystem()
{
	printf("Filesystem constructed\n");
}
Filesystem::~Filesystem()
{
	printf("Filesystem disposed\n");
}