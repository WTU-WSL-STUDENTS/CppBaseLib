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

#include <luadef.h>
#include <map>

template<typename T>
class LuaParam
{
public:
    /* 向脚本写入返回参数 */
	static void ToLua(lua_State *L, T t){throw "Not Override";}
    /* 脚本输入参数不对会直接报错 */
	static T FromLua(lua_State *L, int index){throw "Not Override";}
	/* 获取脚本返回值 */
    static bool GetLuaRet(lua_State *L, int index, int& val){throw "Not Override";}
};

template<>
class LuaParam<int>
{
public:
	static void ToLua(lua_State *L, int t){LUA_SET_PARAM_INTEGER(t);}
	static int FromLua(lua_State *L, int index){return (int)LUA_GET_PARAM_INTEGER(index);}
	static bool GetLuaRet(lua_State *L, int index, int& val)
	{
        if(LUA_RET_IS_INTEGER(index))
        {
            val = LUA_GET_RET_INTEGER(index);
            return true;
        }
        return false;
	}
};
template<>
class LuaParam<double>
{
public:
	static void ToLua(lua_State *L, double t){LUA_SET_PARAM_DOUBLE(t);}
	static double FromLua(lua_State *L, int index){return (double)LUA_GET_PARAM_DOUBLE(index);}
    static bool GetLuaRet(lua_State *L, int index, double &val)
    {
        if(LUA_RET_IS_DOUBLE(index))
        {
            val = LUA_GET_RET_DOUBLE(index);
            return true;
        }
        return false;
    }
};
template<>
class LuaParam<LuaString>
{
public:
	static void ToLua(lua_State *L, LuaString t){LUA_SET_PARAM_STRING(t);}
	static LuaString FromLua(lua_State *L, int index){return LUA_GET_PARAM_STRING(index);}
    static bool GetLuaRet(lua_State *L, int index, LuaString &val)
    {
        if(LUA_RET_IS_STRING(index))
        {
            val = LUA_GET_RET_STRING(index);
            return true;
        }
        return false;
    }
};
template<>
class LuaParam<const char*>
{
public:
	static void ToLua(lua_State *L, const char* t){lua_pushlstring(L, t, strlen(t));}
	static const char* FromLua(lua_State *L, int index){return luaL_checklstring(L, index, &_GET_STRING_TEMP_SIZE);}
    static bool GetLuaRet(lua_State *L, int index, const char* &val)
    {
        if(LUA_RET_IS_STRING(index))
        {
            size_t len;
            val = lua_tolstring(L, index, &len);
            return len;
        }
        return false;
    }
};
/* table */
template<typename TKey, typename TVal>
class LuaParam<std::map<TKey, TVal>>
{
public:
	static void ToLua(lua_State *L, std::map<TKey, TVal> table)
    {
		lua_newtable(L);
        std::map<TKey, TVal>::iterator it = table.begin();
		for (; it != table.end(); ++it)
		{
			LuaParam<TKey>::ToLua(L, it->first);
			LuaParam<TVal>::ToLua(L, it->second);
			lua_settable(L, -3);
		}
    }
	static std::map<TKey, TVal>  FromLua(lua_State *L, int index)
    {
        LUA_GET_PARAM_TABLE_CHECK(index);
		lua_pushnil(L);
        if(0 > index)
        {
            index--;
        }
        std::map<TKey, TVal> map;
		while (lua_next(L, index) != 0)
		{
            map[LuaParam<TKey>::FromLua(L, -2)] = LuaParam<TVal>::FromLua(L, -1);
			lua_pop(L, 1);
		}
        return map;
    }
    static bool GetLuaRet(lua_State *L, int index, std::map<TKey, TVal> &val)
    {
        if(LUA_RET_IS_TABLE(index))
        {
            lua_pushnil(L);
            if(0 > index)
            {
                index--;
            }
            TKey k;
            TVal v;
            while (lua_next(L, index) != 0)
            {
                if(LUA_OK == LuaParam<TKey>::GetLuaRet(L, index, k) && LUA_OK == LuaParam<TVal>::GetLuaRet(L, index, v))
                {
                    map[k] = v;
                }
                lua_pop(L, 1);
            }
            return true;
        }
        return false;
    }
};

#endif