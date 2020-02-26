/******************************************************************************/
/** @file environment.h
@brief
	软件开发基本环境定义。
@details
	本文件定义了SSA及各个模块的版本号，并提供了一组宏，可以通过\#ifdef进行测试，
	以判断软件运行环境。如：
	@par
	@code
        #ifdef xmMODE_DEBUG
            You debug code ... ...
        #else
            You release code ... ...
        #endif
	@endcode
@note
	SSA处理的字符串不支持UNICODE编码，在UNICODE环境下编译会提示错误，导致编
	译中断。
@remarks
	本文件是SSA的基本文件，所有使用SSA的代码均应包含本文件。
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
#ifndef __SSA_BASE_ENVIRONMENT_H
#define __SSA_BASE_ENVIRONMENT_H


/*******************************************************************************
说  明：版本号中，如是Beta版，则在版本前添加字符“Beta”，Release版不添加任何字符，
		版本号由“主版本号.副版本号.开发号”组成，SSA版本后增加“Build NNNN”，NNNN
		表示该发布在svn中的版本号。
*******************************************************************************/
#define xmVERSION_SSA	"Beta 5.0.00 Build 0000"	/**< SSA主版本号 */
#define xmVERSION_VDR	"Beta 5.0.00"				/**< 数据通信模块版本号 */
#define xmVERSION_UMSF	"Beta 5.0.00"				/**< 模型管理模块版本号 */
#define xmVERSION_DRR	"Beta 5.0.00"				/**< 数据存储模块版本号 */
static const int xmSSA_BASE_MAJOR_VERSION = 5;
static const int xmSSA_BASE_MINOR_VERSION = 0;
static const int xmSSA_BASE_PATCH_VERSION = 0;
static const int xmSSA_BASE_BUILD_VERSION = 0;


/** 
@def xmPLATFORM_X86
	32位操作系统
@par xmPLATFORM_X64
	64位操作系统
*/
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define xmPLATFORM_X64
#else
#   define xmPLATFORM_X86
#endif

/** 
@def xmOS_WINDOWS
	Windows操作系统
@par xmOS_LINUX
	Linux操作系统
*/
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(__WIN64__)
#   define xmOS_WINDOWS
#elif defined(__linux__)
#   define xmOS_LINUX
#else
#	error "SSA Error: No Supported System"
#endif

/** 
@def xmENDIAN_LITTLE
	小端存储
@par xmENDIAN_BIG
	大端存储
*/
#define xmENDIAN_LITTLE
#undef xmENDIAN_BIG

/** 
@def xmMODE_DEBUG
	调试模式运行
@par xmMODE_RELEASE
	发布模式运行
*/
#if defined(_DEBUG) || defined(DEBUG) || defined(Debug) || defined(debug) 
#	define xmMODE_DEBUG
#else
#	define xmMODE_RELEASE
#endif




/*******************************************************************************
说  明：定义基本的包含文件
*******************************************************************************/
#ifdef xmOS_WINDOWS
#	ifndef _WINSOCKAPI_
#		define _WINSOCKAPI_
#		include <windows.h>
#		undef _WINSOCKAPI_
#	else
#		include <windows.h>
#	endif
#endif

/*******************************************************************************
说  明：定义Base包接口导出宏
*******************************************************************************/
#ifdef xmOS_WINDOWS
	#ifdef __SSA_BASE_EXPORT
		#define xmBASE_EXPORT __declspec(dllexport)
	#else
		#define xmBASE_EXPORT __declspec(dllimport)
	#endif
#else  // xmOS_LINUX
	#define xmBASE_EXPORT
#endif

/*******************************************************************************
说  明：不支持UNICODE编码字符串的处理
*******************************************************************************/
#if defined(UNICODE) || defined(_UNICODE)
//#	error "SSA Error: No Supported UNICODE"
#endif

/*******************************************************************************
说  明：发布版本关闭部分确认无风险的编译警告
*******************************************************************************/
#if defined(xmMODE_RELEASE) && defined(xmOS_WINDOWS)
	#pragma warning(disable:4996)
	#pragma warning(disable:4251)
	#pragma warning(disable:4290)
	#pragma warning(disable:4297)
	#pragma warning(disable:4101)
	#pragma warning(disable:4197)
#endif

#endif  //__SSA_BASE_ENVIRONMENT_H



/*******************************************************************************
Copyright (C), 2011-2022, ACC13. All rights reserved.
所属模块：
文 件 名：
相关文件：
代码维护：邓华，dehxya@163.com
功能说明：
使用备注：
	多线程安全：不适用
	可抛出异常：不适用
	其他情况说明：无
--------------------------------------------------------------------------------
修改记录：
	2014.03.03：
		代码初次完成。
*******************************************************************************/



/*******************************************************************************
类名称：
功  能：
说  明：
*******************************************************************************/

/*******************************************************************************
名  称：
功  能：
说  明：
*******************************************************************************/

/*******************************************************************************
功  能：
说  明：
*******************************************************************************/

/*==============================================================================
函数名：
说  明：
==============================================================================*/

/*******************************************************************************
说  明：
*******************************************************************************/


/*==============================================================================
函数名：
功  能：
参  数：
返回值：
异  常：
说  明：
备  注：
--------------------------------------------------------------------------------
示  例：
==============================================================================*/
