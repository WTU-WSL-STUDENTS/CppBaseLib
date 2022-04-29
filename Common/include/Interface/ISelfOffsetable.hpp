/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-29 15:34:08
 * @LastEditors: like
 * @LastEditTime: 2022-04-29 15:57:47
 */
#ifndef SYSTEM_ISELFOFFSETABLE_HPP
#define SYSTEM_ISELFOFFSETABLE_HPP
#include "InterfaceDef.h"
namespace System::Interface
{
    template<typename TDerived>
    class ISelfIncreaseable;
    template<typename TDerived>
    class ISelfDecreaseable;
    template<typename TDerived>
    class ISelfOffsetable;

};
/**
 * @brief 需要实现接口
 * void Increase(int n)
 * void Decrease(int n)
 * @tparam TDerived 
 */
template<typename TDerived>
class System::Interface::ISelfIncreaseable
{
	DECLARE_CRTP_INTERFACE(ISelfIncreaseable, TDerived)
public:
    TDerived& operator++()
	{ 
		CRTP_DERIVED.Increase();
		return CRTP_DERIVED;
	}
	TDerived operator++(int)
	{
		TDerived temp(CRTP_CONST_DERIVED);
		CRTP_DERIVED.Increase();
		return temp;
	}
private:
    void Increase()
    {
        CRTP_DERIVED.Increase(1);
    }
};
template<typename TDerived>
class System::Interface::ISelfDecreaseable
{
	DECLARE_CRTP_INTERFACE(ISelfDecreaseable, TDerived)
public:
    TDerived& operator--()
	{ 
		CRTP_DERIVED.Decrease();
		return CRTP_DERIVED;
	}
	TDerived operator--(int)
	{
		TDerived temp(CRTP_CONST_DERIVED);
		CRTP_DERIVED.Decrease();
		return temp;
	}
private:
    void Decrease()
    {
        CRTP_DERIVED.Decrease(1);
    }
};
template<typename TDerived>
class System::Interface::ISelfOffsetable : public ISelfIncreaseable<TDerived>, public ISelfDecreaseable<TDerived>
{
	DECLARE_CRTP_INTERFACE(ISelfOffsetable, TDerived)
public:
	TDerived operator+(int n)
	{
		TDerived temp(CRTP_CONST_DERIVED);
		temp.Increase(n);
		return temp;
	}
	TDerived operator-(int n)
	{
		TDerived temp(CRTP_CONST_DERIVED);
		temp.Decrease(n);
		return temp;
	}
	TDerived& operator+= (int n)
	{
        CRTP_DERIVED.Increase(n);
		return CRTP_DERIVED
	}
	TDerived& operator-= (int n)
	{
        CRTP_DERIVED.Decrease(n);
		return CRTP_DERIVED
	}
	TDerived operator[](int n)
	{
		return operator+(n);
	}
private:
    void Increase(int n)
    {
        CRTP_DERIVED.Increase(n);
    }
    void Decrease(int n)
    {
        CRTP_DERIVED.Decrease(n);
    }
};
#endif