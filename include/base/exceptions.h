/******************************************************************************/
/** @file exceptions.h
@brief
	错误处理与定义。
@details
	实现SSA的异常定义。SSA中的异常包括异常编码（Code）和异常信息（Message）两部分
	信息。所有供用户使用的异常，其Code必须在 errors.h 文件中进行定义。默认情况下，
	异常的Message为Code在 errors.h 文件中定义时所指定的信息。开发者可以自行定义
	该Code对应的信息，以便为用户提供更加详细的异常内容描述。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	exceptions.cpp 
@par 多线程安全
	是
@see errors.h
@see ssa::xmException
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_EXCEPTIONS_H
#define __SSA_BASE_EXCEPTIONS_H
#include "errors.h"
#include <stdexcept>
#ifdef xmOS_LINUX
#include <stdio.h>
#include <stdarg.h>
#endif // xmOS_LINUX


namespace ssa
{
	/******************************************************************************/
	/** @class xmException
	@brief
		SSA中所有异常的基类。
	@details
		xmException可以直接使用，或者被其它异常继承。xmException仅可以在创建的时候初
		始化，一旦创建完成，则不提供修改Code和Message内容的接口。
	@par 多线程安全
		不适用
	@par 修改记录
		@par
		2017.10.23
			-# 将记录异常信息的数据类型由const char* 改为 xmString 。
		@par
		2014.03.03
			-# 代码初次完成。
	*******************************************************************************/
	class xmBASE_EXPORT xmException
	{
	public:
		/******************************************************************************/
		/** @name 构造函数与析构函数
		@{
		*/

		/** 构造函数
		@param [in] code
			异常的Code，该数值必须在errors.h中预先定义。
		@param [in] msg
			用于自定义异常信息。
		@param [in] ...
			可变参数，如果该msg需要参数格式化，则在此填充。
		@par 示例
			@par
			@code
				//	捕获到的xmException的Code为xmE_OUTOFRANGE，Message为“Memory overflow.”，在 errors.h 中定义
				if( condition failed )
				{
					throw xmException(xmE_OUTOFRANGE);
				}
		
				//	捕获到的xmException的Code为xmE_OUTOFRANGE，Message为“访问第34个数组越界”
		        if( condition failed )
		        {
				    throw xmException(xmE_OUTOFRANGE, "访问第%d个数组越界", 34);
				}
			@endcode
		*/
		xmException(int code);
		xmException(int code, const char* msg, ...);
		/** 拷贝构造函数
		*/
		xmException(const xmException& exc) :m_nCode(exc.Code()), m_strMsg(exc.Message()){};
		/** 析构函数
		*/
		virtual ~xmException(void){};

		/**	@} ************************************************************************/


		/******************************************************************************/
		/** @name 基本操作函数
		@{
		*/

		/** 重新抛出本异常
		*/
		virtual void Throw(void) const
		{
			throw *this;
		}
		
		/** 获得异常的名称，不同异常类可以有自己的名称
		*/
		virtual const char* What() const 
		{	
			return "xmException";
		}

		/** 获得异常的编码
		*/
		int Code() const
		{ 
			return m_nCode; 
		}

		/** 获得异常的信息
		*/
		const char* Message() const
		{ 
			return m_strMsg.String();
		}

		/**	@} ************************************************************************/

	protected:
		void FormatMsg(const char* msg, va_list& args);
		const int m_nCode;
		xmString m_strMsg;
	};



	/******************************************************************************/
	/** @name 使用xmDECLARE_EXCEPTION宏来声明SSA中抛出的异常。
	@note
		“xmEx”开头的异常类为 xmException 的子类，其名称由后面的字符串指定，异常编码为“xmE_”
		开头的常量，异常信息为异常编码在errors.h中对应的信息。
	@{
	*/

	/*******************************************************************************
	使用xmDECLARE_EXCEPTION宏来声明SSA中可能抛出的异常，xmDECLARE_EXCEPTION宏的参数：
		第一个：异常类
		第二个：异常名称
		第三个：异常助记符，即异常编码，在errors.h中定义
	*******************************************************************************/
	//	定义通用异常
	xmDECLARE_EXCEPTION(xmExIllegalInput,	"xmExIllegalInput",	xmE_ILLEGAL_INPUT);
	xmDECLARE_EXCEPTION(xmExIllegalCall,	"xmExIllegalCall",  xmE_ILLEGAL_CALL);
	xmDECLARE_EXCEPTION(xmExPointer,		"xmExPointer",		xmE_POINTER);
	xmDECLARE_EXCEPTION(xmExOutOfRange,		"xmExOutOfRange",	xmE_OUTOFRANGE);
	xmDECLARE_EXCEPTION(xmExMalloc,			"xmExMalloc",		xmE_MALLOC);
	xmDECLARE_EXCEPTION(xmExTypeUnmatch,	"xmExTypeUnmatch",  xmE_TYPE_UNMATCH);
	xmDECLARE_EXCEPTION(xmExNever,			"xmExNever",		xmE_NEVER);
	xmDECLARE_EXCEPTION(xmExFailed,			"xmExFailed",		xmE_FAIL);
	xmDECLARE_EXCEPTION(xmExSysCall,		"xmExSysCall",		xmE_SYS_CALL);
	xmDECLARE_EXCEPTION(xmExUnknown,		"xmExUnknown",      xmE_UNKNOWN);

	//	定义base包异常
	xmDECLARE_EXCEPTION(xmExOperRefused,    "xmExOperRefused", xmE_OPERATION_REFUSED);
	xmDECLARE_EXCEPTION(xmExCreateIPC,		"xmExCreateIPC",   xmE_CREATE_IPC);
	xmDECLARE_EXCEPTION(xmExDestroyIPC,		"xmExDestroyIPC",  xmE_DESTROY_IPC);
	xmDECLARE_EXCEPTION(xmExCreateFile,		"xmExCreateFile",  xmE_CREATE_FILE);
	xmDECLARE_EXCEPTION(xmExCreateShareMemory,	"xmExCreateShareMemory",  xmE_CREATE_SHAREMEMORY);
	xmDECLARE_EXCEPTION(xmExTimeout,		"xmExTimeout",		xmE_TIMEOUT);

	xmDECLARE_EXCEPTION(xmExDllLoad,		"xmExDllLoad", xmE_DLL_LOAD);
	xmDECLARE_EXCEPTION(xmExDllFunc,		"xmExDllFunc", xmE_DLL_FUNC);

	xmDECLARE_EXCEPTION(xmExJsonFile,		"xmExJsonFile",  xmE_JSON_FILE);
	xmDECLARE_EXCEPTION(xmExJsonName,		"xmExJsonName",  xmE_JSON_NAME);
	xmDECLARE_EXCEPTION(xmExJsonValue,      "xmExJsonValue", xmE_JSON_VALUE);

	/**	@} ************************************************************************/
}

#endif // __SSA_BASE_EXCEPTIONS_H
