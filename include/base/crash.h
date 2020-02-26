/******************************************************************************/
/** @file crash.h
@brief
	提供参数可变的断言。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	crash.cpp
@par 修改记录
	@par
	2019.11.27
		-# 代码初次完成。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_CRASH_H
#define __SSA_BASE_CRASH_H
#include "define.h"

namespace ssa
{
	/*==============================================================================
	函数名：xmCatchCrash
	说  明：捕获导致程序崩溃的异常。
	==============================================================================*/
//#ifdef __cplusplus
//	extern "C"	
//	{
//#endif
	xmBASE_EXPORT void xmCatchCrash(const char* pDumpFileName = NULL, bool bLogCallStack = true);
//#ifdef __cplusplus
//	}
//#endif
}
#endif	//	__SSA_BASE_CRASH_H