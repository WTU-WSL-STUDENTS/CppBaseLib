/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-28 16:17:17
 * @LastEditors: like
 * @LastEditTime: 2022-04-30 15:50:52
 */
#ifndef COMPLIED_FUNCTION_TRAITS_HPP
#define COMPLIED_FUNCTION_TRAITS_HPP
#include <cstddef>
#include <tuple>
#include <type_traits>
#include "Complied.Macro.h"
/**
 * @brief 函数类型萃取
 * 
 * @tparam F 
 */
template<class F>
struct function_traits
{
private:
	using call_type = function_traits < decltype(&F::operator()) >;
public:
	using return_type = typename call_type::return_type;

	static const std::size_t arity = call_type::arity - 1;

	template <std::size_t N>
	struct argument
	{
		static_assert(N < arity, "error: invalid parameter index.");
		using type = typename call_type::template argument<N + 1>::type;
	};
};
template<class R, class... Args>
struct function_traits<R(*)(Args...)> : public function_traits < R(Args...) >
{};
template<class R, class... Args>
struct function_traits < R(Args...) >
{
	using return_type = R;

	static const std::size_t arity = sizeof...(Args);

	template <std::size_t N>
	struct argument
	{
		//static_assert(N < arity, "error: invalid parameter index.");
		using type = typename std::tuple_element<N, std::tuple<Args...>>::type;
	};
};
// member function pointer
template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...)> : public function_traits < R(C&, Args...) >
{};
// const member function pointer
template<class C, class R, class... Args>
struct function_traits<R(C::*)(Args...) const> : public function_traits < R(C&, Args...) >
{};
// member object pointer
template<class C, class R>
struct function_traits<R(C::*)> : public function_traits < R(C&) >
{};
template<class F>
struct function_traits<F&> : public function_traits < F >
{};
template<class F>
struct function_traits<F&&> : public function_traits < F >
{};
/**
 * @brief 编译时检查 ClassType::FuncName 是否可访问. 返回值与 ReturnType 一致则宏定义结果为 true 
 * 
 */
#define FUNCTION_RETURN_TYPE_IS_THAT(ReturnType, FuncName)	\
	(std::is_same<ReturnType, function_traits<decltype(&( FuncName ))>::return_type>::value)
/**
 * @brief 
 * 借鉴 std::_Wrapped_seekable_v
 */
#endif