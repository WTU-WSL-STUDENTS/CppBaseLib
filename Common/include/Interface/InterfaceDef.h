/*
 * @Description: 
 * @Version: 1.0
 * @Autor: like
 * @Date: 2022-04-22 10:08:02
 * @LastEditors: like
 * @LastEditTime: 2022-04-22 10:44:08
 */
#ifndef INTERFACE_DEF_H
#define INTERFACE_DEF_H

#define DECLARE_CRTP_INTERFACE(type, T) private: type() = default; friend T;
#define CRTP_DERIVED		static_cast<T&>(*this)
#define CRTP_CONST_DERIVED	static_cast<const T&>(*this)
#define CRTP_VIRTUAL
#define CRTP_OVERRIDE
#endif