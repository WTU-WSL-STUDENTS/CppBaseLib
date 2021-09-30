/*
 * @Description: lua dll≤‚ ‘
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 10:28:27
 * @LastEditors: like
 * @LastEditTime: 2021-09-30 15:02:50
 */
#include <LuaInterfaceDef.h>
#include <LuaStackIO.hpp>

GEN_LUA_FUNCTION_CPP(Add)
{
	double left 	= LuaParam<double>::FromLua(L, 1);
	double right 	= LuaParam<double>::FromLua(L,2);
	LuaParam<double>::ToLua(L, left + right);
	return 1;
}
typedef std::map<int, const char*> IntKeyStringValMap;
GEN_LUA_FUNCTION_CPP(DisplayMap)
{
	IntKeyStringValMap map 	= LuaParam<IntKeyStringValMap>::FromLua(L, -1);
	IntKeyStringValMap::iterator it = map.begin();
	int count = 0;
	for(;it != map.end(); it++, count++)
	{
		printf("Key:%d, Val:%s\n", it->first, it->second);
	}
	LuaParam<int>::ToLua(L, count);
	return 1;
}

BEGIN_DECLARE_LUA_INTERFACE
	DECLARE_LUA_INTERFACE(Add)
	DECLARE_LUA_INTERFACE(DisplayMap)
END_DECLARE_LUA_INTERFACE

DECLARE_LUA_DLL_ENTRY(lua_test)