/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 16:30:26
 * @LastEditors: like
 * @LastEditTime: 2021-09-30 10:48:59
 */
#ifndef LUA_MEM_OPERATE_HPP
#define LUA_MEM_OPERATE_HPP

#include <CompliedEntry.h>
#include <luadef.h>
#include <map>

#define STACK_OBJECT L
#define IO_VAL value

#define DECLARE_LUA_STACK_IO(type, tolua, fromlua, getluaret)   \
template<>                                                      \
class LuaStack<type>                                            \
{                                                               \
public:                                                         \
	static inline void PushArg(lua_State* L, const type & IO_VAL) {tolua}               \
    static inline void GetArg(lua_State* L, int index, type & IO_VAL) { fromlua }       \
    static inline void GetLuaRet(lua_State* L, int index, type& IO_VAL) { getluaret }   \
};
#define DECLARE_LUA_STACK_IO_SIMPLE_TYPE(type)  \
    DECLARE_LUA_STACK_IO(type,                  \
    {                           \
        tolua(type, IO_VAL);    \
    },                          \
    {                                                                   \
        ERROR_ASSERT(istype(type, index), "GetArg simple type error");  \
        IO_VAL = fromlua(type, index);                                  \
    },                                                                  \
    {                                   \
        IO_VAL = getluaret(type, index);\
    })

/* 任意类型的入栈出栈的接口 */
template<typename T>
class LuaStack
{
public:
    /* 向脚本传参数 */
    static void PushArg(lua_State* L, const T& IO_VAL)          = 0;
    /* 从脚本中获取参数 */
	static void GetArg(lua_State *L, int index, T& IO_VAL)      = 0;
	/* 获取脚本返回值 */
    static void GetLuaRet(lua_State *L, int index, T& IO_VAL)   = 0;
};

/* lua 简单类型的入栈出栈定义 */
DECLARE_LUA_STACK_IO_SIMPLE_TYPE(number);
DECLARE_LUA_STACK_IO_SIMPLE_TYPE(integer);
DECLARE_LUA_STACK_IO_SIMPLE_TYPE(string);
#ifdef USE_LUA_EXTENTION
DECLARE_LUA_STACK_IO_SIMPLE_TYPE(boolean);
#endif

/* table */
template<typename TKey, typename TVal>
class LuaStack<table<TKey, TVal>>
{
public:
	static void PushArg(lua_State * STACK_OBJECT, const table<TKey, TVal>& IO_VAL)
    {
		lua_newtable(L);
        table<TKey, TVal>::iterator it = IO_VAL.begin();
		for (; it != IO_VAL.end(); ++it)
		{
            LuaStack<TKey>::PushArg(STACK_OBJECT, it->first);
            LuaStack<TVal>::PushArg(STACK_OBJECT, it->second);
			lua_settable(STACK_OBJECT, -3);
		}
	}
    static void GetArg(lua_State* STACK_OBJECT, int index, table<TKey, TVal>& IO_VAL)
	{
        ERROR_ASSERT(istype(table, index), "GetArg table type error");
		lua_pushnil(STACK_OBJECT);
		if (0 > index)
		{
			index--;
		}
		while (lua_next(STACK_OBJECT, index) != 0)/* reference : lua_next */
		{
            IO_VAL[LuaStack<TKey>::GetArg(STACK_OBJECT, -2)] = LuaStack<TVal>::GetArg(STACK_OBJECT, -1);
			lua_pop(STACK_OBJECT, 1);
		}
    }
    static void GetLuaRet(lua_State *STACK_OBJECT, int index, std::map<TKey, TVal> &val)
	{
		ERROR_ASSERT(istype(table, index), "GetLuaRet table type error");
		lua_pushnil(STACK_OBJECT);
		if (0 > index)
		{
			index--;
		}
		TKey k;
		TVal v;
		while (lua_next(STACK_OBJECT, index) != 0)
		{
			LuaParam<TKey>::GetLuaRet(STACK_OBJECT, index, k);
			LuaParam<TVal>::GetLuaRet(STACK_OBJECT, index, v)
			map[k] = v;
			lua_pop(STACK_OBJECT, 1);
		}
    }
};

#endif