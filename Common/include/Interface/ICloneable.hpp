/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-05-05 18:33:51
 * @LastEditors: like
 * @LastEditTime: 2022-05-06 14:47:44
 */
#ifndef SYSTEM_ICLONEABLE_HPP
#define SYSTEM_ICLONEABLE_HPP
#include "IEnumerable.hpp"

#define POSITIVE_ATTITUDE_TO_CRTP_ICLONEABLE
#ifndef POSITIVE_ATTITUDE_TO_CRTP_ICLONEABLE
#	define CRTP_INTERFACE_DEPRECATE_ICLONEABLE_CLASS						\
    [[deprecated(															\
        "warning ICRTP0001: "												\
        "ICloneable 旨在抽象类的 \"赋值\" / \"移动\" (copy-and-swap). "		\
        "目前编译器无法识别 CRTP 的 ICloneable 写法, "						\
		"用 SET_ICLONEABLE 宏定义可实现类似功能, "							\
		"且保留向后兼容的可能性 (如果 ICloneable 的问题能够解决的话)."  	\
        "如果对解决该问题持悲观态度, "										\
		"取消 POSITIVE_ATTITUDE_TO_CRTP_ICLONEABLE 宏定义可放弃依赖该接口, "\
		"接口的作用仅仅在于提示用户对象的\"赋值\" / \"移动\" 语义会被调用")]]
#else
#	define CRTP_INTERFACE_DEPRECATE_ICLONEABLE_CLASS
#endif
namespace System::Interface
{
    template<typename TDerived>
    class ICloneable;
};

#define CRTPInterfaceDefineWithCheck_DepthCopy CRTP_INTERFACE_OVERRIDED_ASSERT(void, DepthCopy, const TDerived&);	
#define CRTPInterfaceDefineWithCheck_MoveCopy CRTP_INTERFACE_OVERRIDED_ASSERT(void, MoveCopy, TDerived&&);	
template<typename TDerived>
class CRTP_INTERFACE_DEPRECATE_ICLONEABLE_CLASS System::Interface::ICloneable
{
#ifdef POSITIVE_ATTITUDE_TO_CRTP_ICLONEABLE
    DECLARE_CRTP_INTERFACE(ICloneable, TDerived)
public:
	ICloneable(const _ICloneable& d) = default;
	ICloneable(_ICloneable&& d) = default;
	_ICloneable& operator=(const _ICloneable& d)
	{
        CRTPInterfaceDefineWithCheck_DepthCopy;
        CRTP_DERIVED.DepthCopy(static_cast<const TDerived&>(d));
		return *this;
	}
	_ICloneable& operator=(_ICloneable&& d) noexcept
	{
        CRTPInterfaceDefineWithCheck_MoveCopy;
        CRTP_DERIVED.MoveCopy(static_cast<TDerived&&>(std::forward<TDerived>(d)));
		return *this;
	}
	/* 编译器不认这种写法， 无法将 拷贝 / 移动 语义抽象成接口 */
	TDerived& operator=(const TDerived& d)
	{
		CRTPInterfaceDefineWithCheck_DepthCopy;
		CRTP_DERIVED.DepthCopy(d);
		return *this;
	}
	TDerived& operator=(TDerived&& d) noexcept
	{
		CRTPInterfaceDefineWithCheck_MoveCopy;
		CRTP_DERIVED.MoveCopy(std::forward<TDerived>(d));
		return *this;
	}
#endif
};

#define SET_ICLONEABLE(typeName)		\
typeName& operator=(const typeName & d){\
	DepthCopy(d);						\
	return *this;						\
}										\
typeName& operator=(typeName && d) noexcept{\
	MoveCopy(std::forward<typeName>(d));	\
	return *this;							\
}
#endif