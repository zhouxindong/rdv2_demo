/******************************************************************************/
/** @file errors.h
@brief
	错误处理与定义。
@details
	SSA处理的错误由错误助记符（ErrDec）、错误编号（ErrCode）和错误信息（ErrMsg）
	三部分组成，理论上三者之间严格一一对应，通过 __SSA_ERR_DEFINE 宏由开发者统一定义，
	在同一系统内不允许出现重复。SSA错误分为三类：
		-# 系统调用错误，该错误由调用操作系统API接口产生，通过Windows系统的
	GetLastError()系统调用或者Linux系统的errno变量产生。系统调用错误的ErrCode大
	于0；
		-# SSA接口执行失败，用户调用SSA提供的接口函数失败，返回的失败信息。通常
	情况下，SSA接口函数返回0（或者xmE_SUCCESS）为执行成功，小于0为SSA执行失败，
	大于0是系统调用错误；
		-# SSA抛出异常，当无法用返回值表示执行失败（例如，在构造函数中或者字符串
	转换为整数的接口函数）或者出现严重错误时，SSA接口会抛出异常，异常同样包括
	ErrCode和ErrMsg两种信息，关于异常，详见 exceptions.h。
	
	本文件定义SSA错误代码和错误信息，供函数返回、抛出异常使用。SSA所有函数的错误
	返回值、异常代码和其所代表的信息，必须在此处进行定义。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	errors.cpp
@par 多线程安全
	是
@see exceptions.h
@par 修改记录
	@par
	2017.10.23
		-# 将 xmPrintErrorMsg 返回值由const char*改为 xmString 。
	@par
	2014.03.03
		-# 代码初次完成。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_ERRORS_H
#define __SSA_BASE_ERRORS_H
#include "errdef.h"
#include "stringutil.h"

namespace ssa
{
	/** 返回最后一次系统调用失败产生的错误编码
	@return 系统调用错误码，返回值≥0。
	*/
	xmBASE_EXPORT int xmGetLastOSCError(void);


	/** 通过错误编码，获得错误信息描述。
	@param [in] nErrCode
		错误编码，可以是系统调用错误（>0）、SSA接口执行失败错误（<0）或者是SSA抛出异常的
		ErrCode。如果ErrCode未定义，则返回空字符串。
	@return 错误信息描述。
	*/
	xmBASE_EXPORT xmString xmPrintErrorMsg(int nErrCode);


	/** 表示size_t类型中无效的位置。
	*/
	extern xmBASE_EXPORT const size_t xmINVALID_POS;
	#define xmINVALID_INDEX xmINVALID_POS


	/******************************************************************************/
	/** @name 通过 __SSA_ERR_DEFINE 宏，进行错误信息定义
	@note 
		“xmE_”开头的常量为错误编码助记符，字符串为错误信息。
	@{
	*/

	/*==============================================================================
	名  称：所有模块都有可能产生的通用错误
	==============================================================================*/
	//	无错误
	__SSA_ERR_DEFINE(xmE_SUCCESS,		0,				 "No Error.");
	//	失败
	__SSA_ERR_DEFINE(xmE_FAIL,		    __EC_COMM(0),    "Failed.");
	//	永远不应该被出现的异常，如果出现本错误，则说明出现编码错误
	__SSA_ERR_DEFINE(xmE_NEVER,			__EC_COMM(1),    "You should never see this message.");
	//	系统调用错误，使用操作系统API或者第三方开发包出现错误
	__SSA_ERR_DEFINE(xmE_SYS_CALL,		__EC_COMM(2),    "System call failed.");
	//	未知错误
	__SSA_ERR_DEFINE(xmE_UNKNOWN,		__EC_COMM(3),    "Unknown error.");

	//	输入参数无效
	__SSA_ERR_DEFINE(xmE_ILLEGAL_INPUT,	__EC_COMM(10),    "Illegal input parameter.");
	//	使用非法指针
	__SSA_ERR_DEFINE(xmE_POINTER,		__EC_COMM(11),    "Illegal pointer.");
	//	内存/数组访问越限
	__SSA_ERR_DEFINE(xmE_OUTOFRANGE,	__EC_COMM(12),    "Memory or array access out of range.");
	//	分配内存失败
	__SSA_ERR_DEFINE(xmE_MALLOC,		__EC_COMM(13),    "Memory allocate failed.");
	//	模板类型不匹配
	__SSA_ERR_DEFINE(xmE_TYPE_UNMATCH,	__EC_COMM(14),    "Template type unmatch.");
	//	非法调用
	__SSA_ERR_DEFINE(xmE_ILLEGAL_CALL,	__EC_COMM(15),    "Illegal function call.");


	/*==============================================================================
	名  称：base模块错误
	==============================================================================*/
	//	xmDynamicBuffer
	__SSA_ERR_DEFINE(xmE_OPERATION_REFUSED,__EC_BASE(101),  "Operation is refused.");
	//	async.h
	__SSA_ERR_DEFINE(xmE_CREATE_IPC,	__EC_BASE(110),  "Create IPC object failed.");
	__SSA_ERR_DEFINE(xmE_DESTROY_IPC,	__EC_BASE(111),  "Destroy IPC object failed.");
	__SSA_ERR_DEFINE(xmE_TIMEOUT,	    __EC_BASE(112),  "Process Timeout.");
	//	xmMsgHandle
	__SSA_ERR_DEFINE(xmE_MQ_CLOSED,		__EC_BASE(120),  "Message queue is closed.");
	__SSA_ERR_DEFINE(xmE_MQ_FULL,		__EC_BASE(121),  "Message queue is full.");
	//	xmShareMemory
	__SSA_ERR_DEFINE(xmE_CREATE_SHAREMEMORY, __EC_BASE(130),  "Create share memory failed.");
	//	xmFIle
	__SSA_ERR_DEFINE(xmE_CREATE_FILE,	__EC_BASE(140),  "Create File Failed.");
	//	xmValue
	__SSA_ERR_DEFINE(xmE_JSON_FILE,		__EC_BASE(150),  "JSON format error.");
	__SSA_ERR_DEFINE(xmE_JSON_NAME,		__EC_BASE(151),  "Data name redefinition");
	__SSA_ERR_DEFINE(xmE_JSON_RANGE,	__EC_BASE(152),  "Data out of range");
	__SSA_ERR_DEFINE(xmE_JSON_VALUE,    __EC_BASE(153),  "Unknown Value Name.");
	//	xmLib
	__SSA_ERR_DEFINE(xmE_DLL_LOAD,	__EC_BASE(154), "Load dll failed.");
	__SSA_ERR_DEFINE(xmE_DLL_FUNC,	__EC_BASE(155), "Get dll function address failed.");
	/**	@} ************************************************************************/
}

#endif //__SSA_BASE_ERRORS_H