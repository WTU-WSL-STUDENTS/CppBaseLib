/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 14:10:53
 * @LastEditors: like
 * @LastEditTime: 2021-09-29 17:09:55
 */
#ifndef LUA_STACK_TO_CPP_H
#define LUA_STACK_TO_CPP_H

extern "C" 
{
	#include <lua.h>
	#include <lualib.h> 
	#include <lauxlib.h>
};
#define LUA_OK 1
#define LUA_NG 0

size_t _GET_STRING_TEMP_SIZE;
/* GET_STRING/SET_STRING传入参数为LuaString类型 */
typedef struct _LuaString
{
	const char* str;
	size_t length;
}LuaString, *PLuaString;

/*
** access functions (stack -> C)
*/
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

#endif
