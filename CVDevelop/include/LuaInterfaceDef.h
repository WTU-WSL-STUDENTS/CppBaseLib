/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 14:07:40
 * @LastEditors: like
 * @LastEditTime: 2021-09-30 09:45:57
 */
#ifndef LUA_2_INTERFACE_H
#define LUA_2_INTERFACE_H

#define DECLARE_LUA_DLL_ENTRY(packName)\
extern "C" __declspec(dllexport) int luaopen_##packName(lua_State* luaEnv){lua_newtable(luaEnv); luaL_setfuncs(luaEnv, luaLibs, 0); return 1;} 		
#define BEGIN_DECLARE_LUA_INTERFACE static luaL_Reg luaLibs[] = {								
#define DECLARE_LUA_INTERFACE(name) {#name, name} ,
#define END_DECLARE_LUA_INTERFACE {NULL, NULL} };
#define GEN_LUA_FUNCTION_CPP(FunctionName) extern "C" int FunctionName(lua_State* L)

#endif