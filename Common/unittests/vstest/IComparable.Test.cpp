#include <System.h>
#include <stdio.h>
using namespace Interface;
class A : public IComparable<A>
{
public:
	A(int a) : val(a) {}
	int CompareTo(const A& other) const CRTP_OVERRIDE
	{
		return val > other.val ? 1 : 0;
	}
	int val;
};
int main()
{
#ifdef _DEBUG
	MEMORYLEAK_ASSERT;
#endif
	IComparable<A>& a = static_cast<IComparable<A>&>(A(3));
	IComparable<A>& b = static_cast<IComparable<A>&>(A(4));
	printf("3 > 4 : %s", a > b ? "true" : "false");
	return 0;
}