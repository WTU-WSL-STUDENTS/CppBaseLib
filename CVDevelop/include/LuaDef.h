/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 14:10:53
 * @LastEditors: like
 * @LastEditTime: 2021-09-30 10:15:12
 */
#ifndef LUA_DEF_H
#define LUA_DEF_H

extern "C" 
{
	#include <lua.h>
	#include <lualib.h> 
	#include <lauxlib.h>
};
size_t _GET_STRING_TEMP_SIZE;
/* GET_STRING/SET_STRING传入参数为LuaString类型 */
typedef struct _LuaString
{
	const char* str;
	size_t length;
}LuaString, *PLuaString;

/* lua->c 判断获取返回值 */
#define LUA_RET_IS_INTEGER(index)       lua_isinteger(L, index)
#define LUA_GET_RET_INTEGER(index)  	lua_tointeger(L, index)
#define LUA_RET_IS_DOUBLE(index)        lua_isnumber(L, index)
#define LUA_GET_RET_DOUBLE(index) 		lua_tonumber(L, index)
#define LUA_RET_IS_STRING(index)        lua_isstring(L, index)
#define LUA_GET_RET_STRING(index)   	{lua_tolstring(L, index, &_GET_STRING_TEMP_SIZE), _GET_STRING_TEMP_SIZE}
#define LUA_RET_IS_FUNCTION_PTR(index) 	lua_isuserdata(L, index)
#define LUA_GET_RET_FUNCTION_PTR(index)	lua_tocfunction(L, index)
#define LUA_RET_IS_USER_DATA(index)  	lua_isuserdata(L, index)
#define LUA_GET_RET_USER_DATA(index)  	lua_touserdata(L, index)

/* c->lua 在c脚本接口中，读取脚本输入的参数和写入返回值 */
#define LUA_GET_PARAM_INTEGER(index)luaL_checkinteger(L, index)
#define LUA_SET_PARAM_INTEGER(val) 	lua_pushinteger(L, val)
#define LUA_GET_PARAM_DOUBLE(index) luaL_checknumber(L, index)
#define LUA_SET_PARAM_DOUBLE(val) 	lua_pushnumber(L, val)
#define LUA_GET_PARAM_STRING(index) {luaL_checklstring(L, index, &_GET_STRING_TEMP_SIZE), _GET_STRING_TEMP_SIZE}
#define LUA_SET_PARAM_STRING(val) 	lua_pushlstring(L, val.str, val.length) 

#define LUA_RET_IS_TABLE(index)			lua_istable(L, index)
#define LUA_GET_PARAM_TABLE_CHECK(index)luaL_checktype(L, index, LUA_TTABLE);

#endif
