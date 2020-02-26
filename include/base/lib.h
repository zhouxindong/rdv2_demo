/******************************************************************************/
/** @file lib.h
@brief
	定义 xmLib 类，实现不同操作系统的（动态/静态）链接库的使用。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	lib.cpp
@see ssa::xmLib
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_LIB_H
#define __SSA_BASE_LIB_H
#include "stringutil.h"
#include "singleton.h"


namespace ssa
{
	/** @def t_LibFunc
	定义不同操作系统的（动态）链接库的导出函数
	*/
#if defined(xmOS_WINDOWS)
#	define t_LibFunc FARPROC
#elif defined(xmOS_LINUX)
#	define t_LibFunc void*
#endif

	/******************************************************************************/
	/** @class xmLib
	@brief
		实现不同操作系统的（动态/静态）链接库的使用。
	@par 多线程安全
		否
	@par 修改记录
		@par
			2015.04.11
			-# 代码初次完成。
	*******************************************************************************/
	class xmBASE_EXPORT xmLib : private xmUncopyable
	{
	public:
		/** 创建Lib对象，加载指定Lib文件
		@param [in] strLibName
			要加载的文件
		*/
		xmLib() : m_LibHandle(NULL)
		{
		};
		xmLib(const char* strLibName) : m_LibHandle(NULL)
		{
			Load(strLibName);
		};
		/** 析构函数，自动释放库
		*/
		~xmLib()
		{
			if (IsLoaded())
			{
				Release();
			}
		};

		/** 判断是否加载成功
		@return 加载成功返回true
		*/
		inline bool IsLoaded() const
		{
			return (m_LibHandle != NULL);
		}

		/** 加载一个指定库
		@param [in] strLibName
			要加载的文件
		@retval xmE_SUCCESS
			加载成功
		@retval xmE_ILLEGAL_INPUT
			strLibName 参数为NULL
		@retval xmE_ILLEGAL_CALL
			当前已经加载其他库，需要先调用 Release() 函数
		@retval xmE_FAIL
			加载失败，可通过 PrintError() 函数打印失败信息
		*/
		xmRet Load(const char* strLibName);
		//	LoadEx加载一个动态链接库LibA，其依赖库的搜索路径由LibA的存储路径开始，参数strLibName必须是绝对路径
		//	Load由加载LibA的程序的存储路径开始
		xmRet LoadEx(const char* strLibName);


		/** 获取库中的一个接口函数
		@param [in] strFuncName
			函数名
		@return 
			函数指针，需要用户通过强制类型转换为自己需要的函数类型。获取失败或当前库未加载返回NULL
		*/
		t_LibFunc GetFunction(const char* strFuncName);

		/** 释放已经加载的库
		*/
		void Release(void);

		/** 打印错误信息
		@return
			错误信息
		*/
		xmString PrintError();

	private:
		xmRet _LoadLibrary(const char* strLibName, bool bUseLocalPath);

#ifdef xmOS_WINDOWS
		HMODULE m_LibHandle;
#else
		void*  m_LibHandle;
#endif
	};
}

#endif	//__SSA_BASE_LIB_H
