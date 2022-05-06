#include <thread>
#include <atomic>
#include <windows.h>
using namespace std;

//#define TEST_USE_ATOMIC

const int MaxLoopCount = 1000000;
const int MaxTestCount = 3;
struct Data
{
	std::atomic<int> accumulate;
	std::atomic<int> buffer[15];
	Data(int n) : accumulate(n), buffer{ 0 } {}
	Data& operator++()
	{
		++accumulate;
		for (int i = 0; i < 15; i++)
		{
			++buffer[i];
		}
		return *this;
	}
};
/* 结论 : 即使 int 在 64 位平台下也需要用 atomic. (采用的 InterLock) */
void TestAtomicOperating()
{
#ifndef TEST_USE_ATOMIC
	using AtomicType = int;
#else
	using AtomicType = std::atomic<int>;
#endif
	AtomicType accumulate;
	auto func = [&]()->void
	{
		for (int i = 0; i < MaxLoopCount; i++)
		{
			++accumulate;
		}
	};
	for (int i = 0; i < MaxTestCount; i++)
	{
		accumulate = 0;
		thread t1(func);
		thread t2(func);
		t1.join();
		t2.join();
		printf("%d ++int operate thread accumulate %d ( should be %d ), offset %d \n", i, accumulate - 0, 2 * MaxLoopCount, 2 * MaxLoopCount - accumulate);
	}
	printf("\n\n");
	Data data = 0;
	auto func1 = [&]()->void
	{
		for (int i = 0; i < MaxLoopCount; i++)
		{
			++data;
		}
	};
	for (int i = 0; i < MaxTestCount; i++)
	{
		data.accumulate.store(0);
		thread t1(func1);
		thread t2(func1);
		t1.join();
		t2.join();
		printf("%d ++Data operate thread accumulate %d ( should be %d ), offset %d\n", i, data.accumulate.load(), 2 * MaxLoopCount, 2 * MaxLoopCount - data.accumulate.load());
	}
	printf("\n\n");
}

int main()
{
	TestAtomicOperating();
	return 0;
}