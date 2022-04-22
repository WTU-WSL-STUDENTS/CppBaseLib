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

/* �����Զ��� lua ��չ�ӿ� */
#define USE_LUA_EXTENTION

extern "C" 
{
	#include <lua.h>
	#include <lualib.h> 
	#include <lauxlib.h>
};
#include <map>
#include <CompliedEntry.h>

/* lua �������� */
using number	= lua_Number;
using integer	= lua_Integer;
using string	= const char*; /* �� std::string ���ֿ� */
#ifdef USE_LUA_EXTENTION
using boolean	= int;
#endif

/* lua ��������ת�� */
#define tolua(type, val)	(lua_push##type(STACK_OBJECT, val))	// c -> stack -> lua
#define fromlua(type, idx)	(lua_to##type(STACK_OBJECT, idx))	// lua -> stack -> c

/* lua �������� */
template<typename TKey, typename TVal>
using table = std::map<TKey, TVal>;
#ifdef USE_LUA_EXTENTION
using userdata = System::Object;
#endif

/* �ж��Ƿ�Ϊָ������ */
#define istype(type, idx)(lua_is##type(STACK_OBJECT, idx))	

/* ��ȡ lua �����ķ���ֵ */
#define getluaret(type, idx) luaL_check##type(STACK_OBJECT, idx)


/* lua �ӿ���һЩ���� */
#ifdef USE_LUA_EXTENTION
#	define luaL_checkboolean		luaL_checkinteger
#	define luaL_checkuserdata	luaL_checkudata
#endif

/* lua c �ӿڵĶ��� */
#define DECLARE_LUA_INTERFACE(funcName) extern "C" int funcName(lua_State* STACK_OBJECT)

/* lua c �ӿڵ�ע�� */
#define DECLARE_LUA_DLL_ENTRY(packName, .../* function register */)			\
extern "C" __declspec(dllexport) int luaopen_##packName(lua_State* luaEnv)	\
{																			\
	lua_newtable(luaEnv);													\
	luaL_setfuncs(luaEnv, luaLibs, 0);										\
	return 1;																\
} 																			\
static luaL_Reg luaLibs[] = {								\
	DECLARE_LUA_INTERFACE(sizeof(__VA_ARGS__), __VA_ARGS__)	\
	{NULL, NULL}											\
};
/* DECLARE_LUA_DLL_ENTRY һ�������֧�� 16 ���ӿ� */
#define DECLARE_LUA_INTERFACE(count, ...) DECLARE_LUA_INTERFACE_##count(__VA_ARGS__)
#ifdef DECLARE_LUA_INTERFACE
#	define DECLARE_LUA_INTERFACE_1(arg1) {#arg1, arg1},
#	define DECLARE_LUA_INTERFACE_2(arg1, arg2)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_1(arg2)
#	define DECLARE_LUA_INTERFACE_3(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_2(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_4(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_3(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_5(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_4(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_6(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_5(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_7(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_6(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_8(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_7(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_9(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_8(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_10(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_9(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_11(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_10(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_12(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_11(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_13(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_12(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_14(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_13(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_15(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_14(__VA_ARGS__)	
#	define DECLARE_LUA_INTERFACE_16(arg1, ...)	DECLARE_LUA_INTERFACE_1(arg1)	DECLARE_LUA_INTERFACE_15(__VA_ARGS__)	
#endif


#endif
