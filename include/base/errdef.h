/******************************************************************************/
/** @file errdef.h
@brief
	此文件仅供开发人员使用，提供对__SSA_ERR_DEFINE、xmDECLARE_EXCEPTION宏的支持。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	errors.cpp
@see errors.h
@par 修改记录
	@par
	2017.09.15
		-# 代码初次完成，由errors.h、exceptions.h文件中，将与用户无关的内容抽取出来。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_ERRDEF_H
#define __SSA_BASE_ERRDEF_H
#include "define.h"

namespace ssa
{
	/*******************************************************************************
	名  称：__SSA_Error
	功  能：辅助生成与定义错误信息。
	*******************************************************************************/
	class xmBASE_EXPORT __SSA_Error
	{
	public:
		__SSA_Error(int nErrCode, const char* const strErrMsg);
	};

	#define __SSA_ERR_DEFINE(ErrDec, ErrCode, ErrMsg)                \
		static const int		 ErrDec = ErrCode;					 \
		static const __SSA_Error __SSA_ERR_MAP_##ErrDec(ErrCode, ErrMsg)


	//	错误编号范围划分，供__EC_XXX使用
	#define __ERROR_START_COMM	(-0x10000)
	#define __ERROR_START_BASE	(-0x11000)
	#define __ERROR_START_VDR	(-0x12000)
	#define __ERROR_START_UMSF	(-0x13000)
	#define __ERROR_START_DB	(-0x14000)
	#define __ERROR_START_XML	(-0x15000)
	#define __ERROR_START_LOG	(-0x16000)
	#define __ERROR_START_CFG	(-0x17000)
	#define __ERROR_START_EXC	(-0x20000)
	#define __ERROR_START_USR	(-0x30000)

	//	定义Error Code生成方式，其中nCode取值为0～9999，供SSA开发人员在__SSA_ERR_DEFINE中定义ErrCode
	#define __EC_COMM(nCode)		(__ERROR_START_COMM - nCode)
	#define __EC_BASE(nCode)		(__ERROR_START_BASE - nCode)
	#define __EC_VDR(nCode)			(__ERROR_START_VDR  - nCode)
	#define __EC_UMSF(nCode)		(__ERROR_START_UMSF - nCode)
	#define __EC_DB(nCode)			(__ERROR_START_DB   - nCode)
	#define __EC_XML(nCode)			(__ERROR_START_XML  - nCode)
	#define __EC_LOG(nCode)			(__ERROR_START_LOG  - nCode)
	#define __EC_CFG(nCode)			(__ERROR_START_CFG  - nCode)
	#define __EC_EXC(nCode)			(__ERROR_START_EXC  - nCode)
	#define __EC_USR(nCode)			(__ERROR_START_USR  - nCode)


	/*******************************************************************************
	名  称：xmDECLARE_EXCEPTION
	功  能：帮助SSA开发者快速定义指定Code的异常。
	参  数：
			  EXC ：新定义的异常类
			  strClassName：该异常类的名称，为字符串
			  nCode：定义该异常的ErrCode
	*******************************************************************************/
#define xmDECLARE_EXCEPTION(EXC, strClassName, nCode)				\
	class EXC : public xmException									\
	{																\
	public:															\
		EXC(void) : xmException(nCode){};							\
		EXC(const char* msg, ...) : xmException(nCode)				\
		{															\
			va_list args;											\
			va_start(args, msg);									\
			FormatMsg(msg, args);									\
			va_end(args);											\
		};															\
		EXC(const EXC& exc) : xmException(exc){};					\
		virtual ~EXC(void){};										\
		virtual void Throw(void) const { throw *this; };			\
		virtual const char* What(void) const { return strClassName; };\
	}
}

#endif //__SSA_BASE_ERRDEF_H
