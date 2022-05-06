#include <stdio.h>
#include <algorithm>
#include <System.Collections.Generic.List.hpp>
#include <System.h>
using namespace Interface;
using namespace std;

#define MAX_COUNT 10000

class Data : public ICloneable<Data>, public IEquatable<Data>, public IComparable<Data>
{
public:
	int val;
	int* resource;
	Data(int i) : resource(new int[4]), val(i){};
	Data(const Data& d) : val(d.val), resource(new int[4]) {}
	Data(Data&& d) noexcept : val(d.val), resource(d.resource)
	{
		d.resource = nullptr;
	}
	SET_ICLONEABLE( Data )
	~Data() 
	{
		VOIDRET_ASSERT(resource);
		delete[] resource; 
	}
	void DepthCopy(const Data& d) CRTP_OVERRIDE
	{
		if (Equals(d))
		{
			return;
		}
		val = d.val;
		if (nullptr == resource)
		{
			resource = new int[4];
		}
		memcpy(resource, d.resource, 16);
	}
	void MoveCopy(Data&& d) CRTP_OVERRIDE
	{
		if (Equals(d))
		{
			return;
		}
		val = d.val;
		if (resource)
		{
			delete[] resource;
		}
		resource = d.resource;
		d.resource = nullptr;
	}
	bool Equals(const Data& other) const CRTP_OVERRIDE
	{
		return val == other.val;//&& resource == other.resource;
	}
	int CompareTo(const Data& other) const CRTP_OVERRIDE
	{
		return val - other.val;
	}
};
void StackListTest()
{
	printf("\n\nBegin stack list test....\n");
	StackList<Data> list(2);
	list.AddRange({ 3 , 4, 5 });
	printf("%d %p\n%d %p\n%d %p\n", list[0].val, list[0].resource, list[1].val, list[1].resource, list[2].val, list[2].resource);
	for (int i = 0; i < MAX_COUNT; i++)
	{
		list.Add(std::move(i));
	}
	for (int i = 0; i < MAX_COUNT; i++)
	{
		list.Remove(i);
	}
	printf("%d, %d\n", list.Count(), list.Capacity());
	list[0].val = 100;
	std::for_each
	(
		list.begin(), list.end(),
		[](const Data& item)->void
		{
			printf("%d %p\n", item.val, item.resource);
		}
	);
	printf("Begin Sort...\n");
	list.Sort();
	for (auto& a : list)
	{
		printf("%d %p\n", a.val, a.resource);
	}
	printf("Begin Reverse...\n");
	list.Reverse();
	for (auto& a : list)
	{
		printf("%d %p\n", a.val, a.resource);
	}
	/* 显示释放资源，非必要 */
	list.Dispose();
	printf("%d, %d ( should be 0, 0 )\n", list.Count(), list.Capacity());
}
void HeapListTest()
{
	printf("\n\nBegin heap list test....\n");
	HeapList<int*> list(2);
	list.AddRange({ new int(3) , new int(4), new int(5) });
	for (int i = 0; i < MAX_COUNT; i++)
	{
		list.Add(new int(i));
	}
	for (int i = 0; i < MAX_COUNT; i++)
	{
		list.RemoveIf([i](int* item)->bool { return i == *item; });
	}
	printf("%d, %d\n", list.Count(), list.Capacity());
	std::for_each
	(
		list.begin(), list.end(),
		[](int* item)->void
		{
			printf("%d, %p\n", *item, item);
		}
	);
	printf("Begin Sort...\n");
	list.Sort();
	for (auto& a : list)
	{
		printf("%d, %p\n", *a, a);
	}
	printf("Begin Reverse...\n");
	list.Reverse();
	for (auto& a : list)
	{
		printf("%d, %p\n", *a, a);
	}
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
	/*StackList<Data>::Iterator::iterator_category a;
	a++;*/
	return 0;
}