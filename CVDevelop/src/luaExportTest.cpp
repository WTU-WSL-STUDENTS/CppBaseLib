/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 10:28:27
 * @LastEditors: like
 * @LastEditTime: 2021-09-29 14:09:30
 */
#include <CLuaWrapperDef.h>


GEN_LUA_FUNCTION_CPP(Add)
{
	double left 	= GET_DOUBLE(1);
	double right 	= GET_DOUBLE(2);
	double res 		= left + right; 
	SET_DOUBLE(res);
	return 1;
}

BEGIN_DECLARE_LUA_INTERFACE
	DECLARE_LUA_INTERFACE(Add)
END_DECLARE_LUA_INTERFACE

DECLARE_LUA_DLL_ENTRY(lua_test)