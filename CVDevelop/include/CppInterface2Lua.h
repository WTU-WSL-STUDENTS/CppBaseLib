/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 14:07:40
 * @LastEditors: like
 * @LastEditTime: 2021-09-29 17:23:23
 */
#ifndef CPP_INTERFACE_TO_LUA_H
#define CPP_INTERFACE_TO_LUA_H

#include <LuaStack2Cpp.h>

extern "C" 
{
	#include <lua.h>
	#include <lualib.h> 
	#include <lauxlib.h>
};

#define DECLARE_LUA_DLL_ENTRY(packName)\
extern "C" __declspec(dllexport) int luaopen_##packName(lua_State* luaEnv){lua_newtable(luaEnv); luaL_setfuncs(luaEnv, luaLibs, 0); return 1;} 		
#define BEGIN_DECLARE_LUA_INTERFACE static luaL_Reg luaLibs[] = {								
#define DECLARE_LUA_INTERFACE(name) {#name, name} ,
#define END_DECLARE_LUA_INTERFACE {NULL, NULL} };
#define GEN_LUA_FUNCTION_CPP(FunctionName) extern "C" int FunctionName(lua_State* L)

#define LUA_PARAM_IS_INTEGER(index)	(LUA_TNUMBER == l_unlikely(lua_type(L, index))
#define LUA_GET_PARAM_INTEGER(index)luaL_checkinteger(L, index)
#define LUA_SET_PARAM_INTEGER(val) 	luaL_checkinteger(L, val)

#define LUA_PARAM_IS_DOUBLE(index)	(LUA_TNUMBER == l_unlikely(lua_type(L, index))
#define LUA_GET_PARAM_DOUBLE(index) luaL_checknumber(L, index)
#define LUA_SET_PARAM_DOUBLE(val) 	lua_pushnumber(L, val)

#define LUA_PARAM_IS_STRING(index)	(LUA_TSTRING == l_unlikely(lua_type(L, index))
#define LUA_GET_PARAM_STRING(index) {luaL_checklstring(L, index, &_GET_STRING_TEMP_SIZE), _GET_STRING_TEMP_SIZE}
#define LUA_SET_PARAM_STRING(val) 	lua_pushlstring(L, val.str, val.length) 


#define LUA_PARAM_IS_NIL(index)				(LUA_TNIL == l_unlikely(lua_type(L, index))
#define LUA_PARAM_IS_BOOL(index)			(LUA_TBOOLEAN == l_unlikely(lua_type(L, index))
#define LUA_PARAM_IS_LIGHTUSERDATA(index)	(LUA_TLIGHTUSERDATA== l_unlikely(lua_type(L, index))
#define LUA_PARAM_IS_TABLE(index)			(LUA_TTABLE == l_unlikely(lua_type(L, index))
#define LUA_PARAM_IS_FUNCTION(index)		(LUA_TFUNCTION == l_unlikely(lua_type(L, index))
#define LUA_PARAM_IS_USERDATA(index)		(LUA_TUSERDATA == l_unlikely(lua_type(L, index))

#endif