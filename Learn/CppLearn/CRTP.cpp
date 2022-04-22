#include<iostream>
using namespace std;

template<typename T>
class IComparable
{
public:
	IComparable(/*std::enable_if_t<true>::type* = nullptr*/)
	{
		bool b = std::is_base_of<IComparable<T>, T>::value;
	}
	inline bool less(const IComparable<T>& b) const {
		return this->lessImpl(b);
	}
	inline bool greater(const IComparable<T>& b) const {
		return b.less(*this);
	}
	inline bool notLess(const IComparable<T>& b) const {
		return !this->less(b);
	}
	inline bool notGreater(const IComparable<T>& b) const {
		return !this->greater(b);
	}
	inline bool equal(const IComparable<T>& b) const {
		return !this->less(b) && !this->greater(b);
	}
	inline bool notEqual(const IComparable<T>& b) const {
		return !this->equal(b);
	}
	/*
	 *  "virtual bool lessImpl(const T& b) const = 0" 优点:
	 *      编译期进行函数实现检查, 即 interface / abstract class
	 *  缺陷:
	 *      1. 会生成虚函数表
	 *      2. 调用虚函数是通过 call rcx 编译器不知道 rcx 内容, 无法进一步优化
	 */
	 // virtual bool lessImpl(const T& b) const = 0;
private:
	/*  CRTP
	 *  "static_cast<const T&>(*this).lessImpl(static_cast<const T&>(b))"
	 *      在编译期会检查 (bool) T::lessImpl (const T&) 函数签名是否存在
	 *  实现了类似 interface /abstract class 的功能。
	 *  优点:
	 *      1. 不会生成虚函数表(如果没有其他虚函数的话)
	 *      2. 通过 call A::lessImpl(A const&) const 编译器可以继续优化
	 *  缺点:
	 *      1. 编译期检查相比纯虚函数函数签名的检查更加宽松。
	 */
	inline bool lessImpl(const IComparable<T>& b) const
	{
		return static_cast<const T&>(*this).lessImpl(static_cast<const T&>(b));
	}

};
class A : public IComparable<A>
{
public:
	A(int num) : IComparable<A>(), N(num) {}
	bool lessImpl(const A& b)const {
		return N < b.N;
	}
private:
	int N;
};
class B : public IComparable<B>
{
public:
	B(int num1, int num2) : IComparable<B>(), N1(num1), N2(num2) {}
	bool lessImpl(const B& b)const {
		return N1 < b.N1 || N2 < b.N2;
	}

	/* 错误写法
	 * CRTP 缺点 ： 类型检查很宽松, 返回值 int 可隐式转为 bool , 即使 CRTP 接口返回 int 也能通过静态多态的检查
	 * 此时结果和预期可能不一致！ 
	 */
	/*int lessImpl(const B& b)const {
		return (N1 < b.N1 || N2 < b.N2) ? 0 : 1;
	}*/
private:
	int N1, N2;
};
int main()
{
	A a(5), b(10);
	cout << a.less(b) << endl; // OK
	B c(5, 10), d(5, 0);
	cout << c.greater(d) << endl; // OK
	return 0;
}