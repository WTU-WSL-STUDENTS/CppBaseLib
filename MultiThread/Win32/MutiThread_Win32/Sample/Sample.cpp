// Sample.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "MutiThread_Win32.h"
#include <process.h>

class Config
{
public:
	int start=0, end=10, step =1;
}cfg;
void Loop()
{
	for (int i = cfg.start; i< cfg.end; i += cfg.step)
	{
		cfg.start = i;
		printf("当前线程内的值为%i\n", i);
		//std::cout << cfg.start << std::endl;
	}
}
static unsigned __stdcall ThreadEntry(void* pThis)
{
	Loop();

	return 1;
}
int main()
{
	//cfg.end = INT32_MAX;
	//MutiThread_Win32* res = MutiThread_Win32_Factory::GetInstance(ThreadEntry, &cfg);
	//res->GetReturnCode();
	//res->Run();
	//res->GetReturnCode();
	//Sleep(1000);
	//printf("returnVal: %i\n", cfg.start);
	//res->GetReturnCode(); 
	//if (CONTEXT_CONTROL == res->GetReturnCode())printf("返回值：CONTEXT_CONTROL");

	MutiThread_Win32* res = MutiThread_Win32_Factory::GetInstance(ThreadEntry, &cfg);
	res->GetReturnCode();
	cfg.start = cfg.end;
	cfg.end *= 2;
	MutiThread_Win32* res1 = MutiThread_Win32_Factory::GetInstance(ThreadEntry, &cfg);
	res1->GetReturnCode();
    std::cout << "Hello World!\n";
}



// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
