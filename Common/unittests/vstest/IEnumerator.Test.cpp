#include <stdio.h>
#include <algorithm>
#include <System.Collections.Generic.List.hpp>
#include <System.h>
using namespace Interface;
using namespace std;

class Data : public IEquatable<Data>
{
public:
	int val;
	int* resource;
	Data(int i) : resource(new int[4]), val(i){};
	Data(const Data& d) : val(d.val), resource(new int[4])
	{

	}
	Data(Data&& d) noexcept : val(d.val), resource(d.resource)
	{
		d.resource = nullptr;
	}
	~Data() 
	{
		VOIDRET_ASSERT(resource);
		delete[] resource; 
	}

	bool Equals(const Data& other) const CRTP_OVERRIDE
	{
		return val == other.val;//&& resource == other.resource;
	}
};
void StackListTest()
{
	StackList<Data> list(2);
	list.AddRange({ 3 , 4, 5 });
	printf("%d %p\n%d %p\n%d %p\n", list[0].val, list[0].resource, list[1].val, list[1].resource, list[2].val, list[2].resource);
	for (int i = 0; i < 100; i++)
	{
		list.Add(std::move(i));
	}
	for (int i = 0; i < 100; i++)
	{
		list.Remove(i);
	}
	StackList<Data>::_IEnumerator p = list.GetEnumerator();
	printf("%d, %d ( should be 3, 192 )\n", list.Count(), list.Capacity());
	for (auto& a : p)
	{
		printf("%d %p\n", a.val, a.resource);
	}
	/* 显示释放资源，非必要 */
	list.Dispose();
	printf("%d, %d ( should be 0, 0 )\n", list.Count(), list.Capacity());
}
void HeapListTest()
{
	HeapList<int*> list(2);
	list.AddRange({ new int(3) , new int(4), new int(5) });
	for (int i = 0; i < 100; i++)
	{
		list.Add(new int(i));
	}
	for (int i = 0; i < 100; i++)
	{
		list.RemoveIf([i](int* item)->bool { return i == *item; });
	}
	HeapList<int*>::_IEnumerator p = list.GetEnumerator();
	
	printf("%d, %d( should be 3, 192 )\n", list.Count(), list.Capacity());
	std::for_each
	(
		p.begin(), p.end(),
		[](int* item)
		{
			printf("%d\n", *item);
		}
	);
	list.Dispose();
	printf("%d, %d (should be 0, 0)\n", list.Count(), list.Capacity());
}

int main()
{
#ifdef _DEBUG
	MEMORYLEAK_ASSERT;
#endif
	StackListTest();
	HeapListTest();
	return 0;
}