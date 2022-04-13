/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2021-09-30 16:13:24
 * @LastEditors: like
 * @LastEditTime: 2021-09-30 17:22:32
 */
#include <System.IO.File.hpp>
#include <LuaEntries.hpp>

int main(int argc, char** argv, char** env)
{
#ifdef _DEBUG
	MEMORYLEAK_ASSERT;
#endif
    //ERROR_ASSERT(2 == argc, "format error , CallLua.exe [lua path]");
	char path[MAX_PATH] = "G:/Project/github/CppBaseLib/CVDevelop/unittests/vstest/out/build/x64-Debug/test.lua";
	//System::IO::GetFullPath(argv[1], path);

    LuaExecuter le;
    le.CreateLuaStack();
    if(!le.LoadLua(path))
    {
        printf("Load lua failed %s\n", path);
        return 1;
	}
    for (int i = 0, sum = -1; i < 3; i++)
	{
		printf("Get Global %d \n", le.GetGlobal("calledbycpp"));
		ERROR_ASSERT(le.DoLua(sum, i + 100, i + 200), "Call lua function failed");
		printf("%d + %d = %d\n", i + 100, i + 200, sum);
	}
	printf("globalLuaArgs %d\n", le.GetGlobal("globalLuaArgs"));
	printf("%lf\n", LuaParam<double>::FromLua(le.Handle(), -1));
	le.DoLuaString("print(os.date(\"%Y-%m-%d %H:%M:%S\"))");
	le.FreeStack();
    return 0;
}