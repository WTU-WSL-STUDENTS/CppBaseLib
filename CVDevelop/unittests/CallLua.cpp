/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-30 16:13:24
 * @LastEditors: like
 * @LastEditTime: 2021-09-30 17:22:32
 */

#include <LuaEntries.hpp>
typedef struct _Params
{
    int left;
    int right;
    int sum;
}Params;
int main()
{
    LuaExecuter le;
    le.CreateLuaStack();
    if(!le.LoadLua("test.lua"))
    {
        printf("Load Lua Failed \n");
    }
    printf("Get Global %d \n", le.GetGlobal("calledbycpp"));
    
    Params p = {1, 2, -1};
    if(!le.DoLua(p.sum, p.left, p.right))
    {
        printf("Do Lua Failed\n");
    }
    printf("%d + %d = %d", p.left, p.right, p.sum);
    le.FreeStack();
}