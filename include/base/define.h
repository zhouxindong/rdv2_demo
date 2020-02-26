/******************************************************************************/
/** @file define.h
@brief
	定义公用的常数、数据结构、数据类型等信息。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	无
@par 修改记录
	@par
	2014.03.03
		-# 代码初次完成。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_DEFINE_H
#define __SSA_BASE_DEFINE_H
#include "environment.h"


/******************************************************************************/
/** @namespace ssa
@brief
	所有SSA提供的接口，均在ssa命名空间下，使用“xm”开头。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
namespace ssa
{
	/** @def xmMAX_LINE
		定义一行文字的最大长度，超过该长度，将自动换行或截断。
	*/
	#define xmMAX_LINE		1024

	/** @def xmMAX_FILE_PATH
		定义文件路径最大长度。
	*/
	/** @def xmMAX_FILE_NAME
		定义文件名最大长度。
	*/
	#ifdef xmOS_WINDOWS
	#	define xmMAX_FILE_PATH	MAX_PATH
	#	define xmMAX_FILE_NAME	NAME_MAX
	#else
	#	define xmMAX_FILE_PATH	260
	#	define xmMAX_FILE_NAME	255
	#endif

	/** @def xmMAX_NAME
		定义数据集合、数据名称的最大长度
	*/
	#define xmMAX_NAME		64

	/** @def xmDEF_WAITTIME
		SSA中默认的最长等待时间
	*/
	#define xmDEF_WAITTIME	3000


	/** @def INFINITE
		定义无穷大正整数
	*/
	#ifndef INFINITE
	#	ifdef xmPLATFORM_X86
	#		define INFINITE 0xFFFFFFFF
	#	else
	#		define INFINITE 0xFFFFFFFFFFFFFFFF
	#	endif
	#endif

	/** @def NULL
		定义空指针
	*/
	#ifndef NULL
	#	define NULL ((void*)0)
	#endif


	/** @def xmBLANK_STRING
		空白字符串
	*/
	#define xmBLANK_STRING " \t\r\n"



	/** @typedef xmRet
		定义SSA函数的返回值类型，所有返回类型为xmRet的函数，其含义定义如下：
	@retval ==0
		函数操作成功；
	@retval >0
		函数操作失败，失败原因由系统调用错误造成，通过Windows系统的GetLastError()系统
		调用或者Linux系统的errno变量产生。可通过 xmPrintErrorMsg() 查询或者通过系统
		API查询；
	@retval <0
		函数操作失败，失败原因由SSA定义，可通过查询通过xmPrintErrorMsg查询或者在errors.h
		文件查看 __SSA_ERR_DEFINE 定义。
	*/
	typedef int xmRet;


	/*******************************************************************************
	名  称：xmEThreadPriority
	功  能：定义SSA中线程优先级
	说  明：
	*******************************************************************************/
	typedef enum
	{
		xmTP_IDLE = (-15),
		xmTP_LOWEST = (-2),
		xmTP_LOW = (-1),
		xmTP_NORMAL = 0,
		xmTP_HIGH = 1,
		xmTP_HIGHEST = 2,
		xmTP_REALTIME = 15
	}xmEThreadPriority;

	//	定义无效的线程句柄
	#if defined(xmOS_WINDOWS)
	#	define INVALID_THREAD_VALUE NULL
	#elif defined(xmOS_LINUX)
	#	define INVALID_THREAD_VALUE (-1)
	#endif

	//	定义无效的IPC句柄
	#if defined(xmOS_WINDOWS)
	#	define INVALID_IPC_HANDLE NULL
	#elif defined(xmOS_LINUX)
	#	define INVALID_IPC_HANDLE (-1)
	#endif
}

#endif //__SSA_BASE_DEFINE_H