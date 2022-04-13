/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-30 10:46:08
 * @LastEditors: like
 * @LastEditTime: 2021-09-30 17:22:01
 */
#ifndef LUA_ENTRIES_HPP
#define LUA_ENTRIES_HPP

#include <LuaStackIO.hpp>
#include <stdio.h>

class LuaExecuter
{
private:
    lua_State *L;
    /* �ݹ�������ȡ����ֵ�� �ݹ���� */
    template<typename First>
    bool GetFuncParams(First& p, int index)
    {
        if(LuaParam<First>::GetLuaRet(L, index))
        {
            index--;
        }
        return index;
    }
    /* �ݹ�������ȡ����ֵ */
    template<typename First, typename... Rest>
    int GetFuncParams(First& p, Rest&... rest, int index)
    {
        if(LuaParam<First>::GetLuaRet(L, index))
        {
            return GetFuncParams(rest, --index);
        }
        return index;
    }
    /* �ݹ��������ò����� �ݹ���� */
    template<typename First>
    bool SetFuncParams(First p)
    {
        LuaParam<First>::ToLua(L, p);
        return true;
    }
    /* �ݹ��������ò��� */
    template<typename First, typename... Rest>
    bool SetFuncParams(First p, Rest&... rest)
    {
        LuaParam<int>::ToLua(L, p);
        return SetFuncParams(rest...);
    }
public:
    LuaExecuter() : L(NULL){}
    lua_State* Handle() { return L; }
    bool CreateLuaStack()
    {
        if(L = luaL_newstate())
        {
            luaopen_base(L);
            luaL_openlibs(L);
            return true;
        }
        return false;
    }
    void FreeStack()
    {
        lua_close(L);
    }

    /**
     * @description: ���ز�ִ�нű� 
     * @param {�ļ���} fileName
     * @return {*}
     * @author: like
     */
    bool LoadLua(const char* fileName)
    {
        return LUA_OK == luaL_dofile(L, fileName);
    }
    /**
     * @description: ִ��һ��lua�ű��ַ��� 
     * @param {�ű�} command
     * @return {*}
     * @author: like
     */    
    bool DoLuaString(const char* command)
    {
        return LUA_OK == luaL_dostring(L, command);
    }
    /**
     * @description: ��ȡ�ű���ָ�����Ƶ�ȫ�ֱ���ֵ,����ֵ����ջ�� 
     * @param {����} name
     * @return {*}
     * @author: like
     */    
    int GetGlobal(const char* name)
    {
        return lua_getglobal(L, name);
    }
    template<typename T>
    int SetGlobal(const char* name, const T& val)
    {
        LuaParam<T>::ToLua(L, val);
        lua_setglobal(L, name);
    }
    /**
     * @description: ִ��ջ����lua���� 
     * @param {����Ĳ�������} inputParamsCount
     * @param {���صĲ�������} outputParamsCount
     * @return {*}
     * @author: like
     */    
    bool DoFunction(int inputParamsCount, int outputParamsCount)
    {
        if(LUA_OK == lua_pcall(L, inputParamsCount, outputParamsCount, 0/* f(0)*/))
        {
            return true;   
        }
        lua_pop(L, 1);
        return false;
    }
    /**
     * @description: ����ջ����lua����������m������������n�������� ÿ������Ĳ�����Ҫʵ�� template<> class LuaParam<T>::ToLua()
     *  ÿ�����صĲ�����Ҫʵ�� template<> class LuaParam<T>::FromLua() 
     * @param {...} in
     * @param {...} out
     * @return {*}
     * @author: like
     */    
    // template<typename... TParamIn, typename... TParamOut>
    // bool DoLua(const TParamIn&... in, TParamOut&... out)
    template<typename TParamOut, typename... TParamIn>
    bool DoLua(TParamOut& out, const TParamIn&... in)
    {
        SetFuncParams<const TParamIn...>(in...);
        if(LUA_OK == lua_pcall(L, sizeof...(in), 1, 0/* f(0)*/))
        {
            bool getsucess = LuaParam<TParamOut>::GetLuaRet(L, -1, out);
            lua_pop(L, 1);
            return getsucess;
        }
        lua_pop(L, 1);
        return false;
    }
};

#endif