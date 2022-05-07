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
        "ICloneable ּ�ڳ������ \"��ֵ\" / \"�ƶ�\" (copy-and-swap). "		\
        "Ŀǰ�������޷�ʶ�� CRTP �� ICloneable д��, "						\
		"�� SET_ICLONEABLE �궨���ʵ�����ƹ���, "							\
		"�ұ��������ݵĿ����� (��� ICloneable �������ܹ�����Ļ�)."  	\
        "����Խ��������ֱ���̬��, "										\
		"ȡ�� POSITIVE_ATTITUDE_TO_CRTP_ICLONEABLE �궨��ɷ��������ýӿ�, "\
		"�ӿڵ����ý���������ʾ�û������\"��ֵ\" / \"�ƶ�\" ����ᱻ����")]]
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
	/* ��������������д���� �޷��� ���� / �ƶ� �������ɽӿ� */
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