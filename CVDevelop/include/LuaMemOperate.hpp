/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-29 16:30:26
 * @LastEditors: like
 * @LastEditTime: 2021-09-29 17:31:28
 */
#ifndef LUA_MEM_OPERATE_HPP
#define LUA_MEM_OPERATE_HPP

#include <CppInterface2Lua.h>
#include <map>

template<typename T>
class LuaParam
{
public:
	static void ToLua(lua_State *L, T t){throw "Not Override";}
	static int FromLua(lua_State *L, int index, T &val){throw "Not Override";}
	static int GetLuaRet(lua_State *L, int index, int& val){throw "Not Override";}
};

template<>
class LuaParam<int>
{
public:
	static void ToLua(lua_State *L, int t)
	{
		LUA_SET_PARAM_INTEGER(t);
	}
	static int FromLua(lua_State *L, int index, int &val)
	{
        if(LUA_PARAM_IS_INTEGER(index))
        {

        }
		val = LUA_GET_PARAM_INTEGER(index);
		return LUA_OK;
	}
	static int GetLuaRet(lua_State *L, int index, int& val)
	{
		if (LUA_RET_IS_INTEGER(index))
		{
			val = LUA_GET_RET_INTEGER(index);
            return LUA_OK;
        }
        return LUA_NG;
	}
};
template<>
class LuaParam<double>
{
public:
	static void ToLua(lua_State *L, double t){LUA_SET_PARAM_DOUBLE(t);}
	static int FromLua(lua_State *L, int index, double &val)
    {
        val = GET_DOUBLE(index);
        return LUA_OK;
    }
    static int GetLuaRet(lua_State *L, int index, double &val)
    {
        if(LUA_RET_IS_DOUBLE(index))
        {
            val = LUA_GET_RET_DOUBLE(index);
            return LUA_OK;
        }
        return LUA_NG;
    }
};
template<>
class LuaParam<LuaString>
{
public:
	static void ToLua(lua_State *L, LuaString t){LUA_SET_PARAM_STRING(t);}
	static int FromLua(lua_State *L, int index, LuaString &val)
    {
        val = LUA_GET_PARAM_STRING(index);
        return LUA_OK;
    }
    static int GetLuaRet(lua_State *L, int index, LuaString &val)
    {
        if(LUA_RET_IS_STRING(index))
        {
            val = LUA_GET_PARAM_RET_STRING(index);
            return LUA_OK;
        }
        return LUA_NG;
    }
};

/* table */
template<typename TKey, typename TVal>
class LuaParam<std::map<TKey, TVal>>
{
public:
	static void ToLua(lua_State *L, std::map<TKey, TVal> t)
    {
		lua_newtable(L);
        std::map<K, V>::const_iterator it = arg_.begin()
		for (; it != arg_.end(); ++it)
		{
			LuaParam<TKey>::ToLua(L, it->first);
			LuaParam<TVal>::ToLua(L, it->second);
			lua_settable(L, -3);
		}
    }
	static int FromLua(lua_State *L, int index, std::map<TKey, TVal> &val)
    {
        lua_type
		lua_pushnil(L);
        index--;
        TKey k;
        TVal v;
		while (lua_next(L, index) != 0)
		{
            if(LUA_OK == LuaParam<TKey>::FromLua(L, index, k) && LUA_OK == LuaParam<TVal>::FromLua(L, index, v))
            {
                val[k] = v;
            }
			lua_pop(L, 1);
		}
        return LUA_OK;
    }
    static int GetLuaRet(lua_State *L, int index, std::map<TKey, TVal> &val)
    {
        if(LUA_RET_IS_STRING(index))
        {
            val = LUA_GET_PARAM_RET_STRING(index);
            return LUA_OK;
        }
        return LUA_NG;
    }
};



class LuaExecuter
{
private:
    lua_State *L;
public:
    bool CreateLuaStack()
    {
        if(L = luaL_newstate())
        {
            luaL_openlibs(L);
            return true;
        }
        return false;
    }
    void FreeStack()
    {

    }

};

#endif