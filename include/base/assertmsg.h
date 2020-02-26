/******************************************************************************/
/** @file assertmsg.h
@brief
	提供参数可变的断言。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	assertmsg.cpp
@par 修改记录
	@par
	2017.09.13：
		修改了断言有效的模式，既可以在DEBUG模式下使用，也可以通过xmASSERT_VALID宏
		定义在RELEASE模式下使用。
	@par
	2014.03.03
		-# 代码初次完成。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_ASSERTMSG_H
#define __SSA_BASE_ASSERTMSG_H
#include "define.h"

namespace ssa
{
	/*==============================================================================
	函数名：__AssertMsg
	说  明：断言的实现函数，用户不需要直接使用。
	==============================================================================*/
//#ifdef __cplusplus
//	extern "C"	
//	{
//#endif
	xmBASE_EXPORT void __AssertMsg(const char* pFileName, unsigned int uLineNo, bool bCondition, const char* pContent, ...);
//#ifdef __cplusplus
//	}
//#endif
}


	/** @def xmAssert(bCondition)
		定义了用于调试的断言接口。
	@param [in] bCondition
		判断条件，当条件非真时，触发断言
	@param [in] pContent
		触发断言时提示的信息
	@param [in] ...
		可变参数，用于格式化pContent。
	@note 
		默认情况下，断言只在debug模式下有效，如果希望在release模式同样使用断言，
		需要在ssa.h文件之前或者工程中定义xmASSERT_VALID宏。
	@par 示例
		@par
		@code
			  int a = 10, b = 20;
			  // 当a小于b时，触发断言，断言信息为“<NULL>”
			  xmAssert(a >= b);
			  // 当a小于b时，触发断言，断言信息为“变量a小于b，其中a=10，b=20。”
			  xmAssertMsg(a >= b, "变量a小于b，其中a=%d，b=%d。", a, b);
		@endcode
	*/
#	ifdef xmOS_WINDOWS
#		define xmAssertAlways(bCondition)     ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), NULL)
#		define xmAssertMsgAlways(bCondition, pContent, ...)     ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), (pContent), ## __VA_ARGS__)
#	else  //	Linux
#		define xmAssertAlways(bCondition) ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), NULL)
#		define xmAssertMsgAlways(bCondition, pContent, args...) ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), (pContent), ## args)
#	endif

#if (defined xmMODE_DEBUG) || (defined xmASSERT_VALID)
#	ifdef xmOS_WINDOWS
#		define xmAssert(bCondition)     ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), NULL)
#		define xmAssertMsg(bCondition, pContent, ...)     ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), (pContent), ## __VA_ARGS__)
#	else  //	Linux
#		define xmAssert(bCondition) ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), NULL)
#		define xmAssertMsg(bCondition, pContent, args...) ssa::__AssertMsg(__FILE__, __LINE__, (bCondition), (pContent), ## args)
#	endif
#else  // xmMODE_RELEASE
#	ifdef xmOS_WINDOWS
#		define xmAssert(bCondition)
#		define xmAssertMsg(bCondition, pContent, ...)
#	else  //	Linux
#		define xmAssert(bCondition)
#		define xmAssertMsg(bCondition, pContent, args...)
#	endif
#endif // xmMODE_DEBUG


#endif	//	__SSA_BASE_ASSERTMSG_H