/******************************************************************************/
/** @file type.h
@brief
	定义SSA中对类型的描述。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	type.cpp
@par 多线程安全
	是
@par 修改记录
	@par
	2020.01.20：
		-# 去掉了xmDT_CLASS的定义。。
	@par
	2018.06.23：
		-# 扩展了xmDataType函数的功能，能够将一个“BUFFER”开头加上一个数字的字符串，识别为xmDT_BUFFER类型。
	@par
	2018.06.11：
		-# 增加了xmDT_STRING的定义。
	@par
	2017.10.31：
		-# 将xmEDataType中的xmDT_CLASS定义，改为xmDT_STRUCT，用于描述结构化数据。
	@par
	2014.09.19：
		-# 由define.h文件中提取出来。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_TYPE_H
#define __SSA_BASE_TYPE_H
#include "define.h"

namespace ssa
{
	/** @enum xmEDataType
	@brief
		定义SSA中处理数据的类型。
	*/
	typedef enum
	{
		xmDT_UNKNOWN = 0,	/**< 未知类型 */
//		xmDT_CHAR = 1,
		xmDT_INT8 = 1,	/**< 8位整数 */
		xmDT_UINT8,		/**< 无符号8为整数 */
		xmDT_INT16,		/**< 16位整数 */
		xmDT_UINT16,	/**< 无符号16位整数 */
		xmDT_INT32,		/**< 32位整数 */
		xmDT_UINT32,	/**< 无符号32位整数 */
		xmDT_INT64,		/**< 64位整数 */
		xmDT_UINT64,	/**< 无符号64位整数 */
		xmDT_FLOAT,		/**< 浮点数 */
		xmDT_DOUBLE,	/**< 双精度浮点数 */
		xmDT_STRING,	/**< 以0结尾的字符串 */
		xmDT_BUFFER,	/**< 内存块类型 */
//		xmDT_BOOL,	
		xmDT_STRUCT,	/**< 结构化数据类型 */
//		xmDT_CLASS,	    /**< 结构化数据类型 */
		xmDT_MAX		/**< 结构化数据类型 */
	}xmEDataType;

	
	/** 根据数据类型，获得数据长度
	@return
		数据长度，未知类型返回0。
	*/
	xmBASE_EXPORT size_t xmTypeSize(xmEDataType eType);


	/** 根据数据类型，获得数据名称
	@return
		数据名称。
	@note 
		xmEDataType 数据类型与名称对应关系：
		- xmDT_UNKNOWN ： UNKNOWN
		- xmDT_INT8    ： INT8
		- xmDT_UINT8   ： UINT8
		- xmDT_INT16   ： INT16
		- xmDT_UINT16  ： UINT16
		- xmDT_INT32   ： INT32
		- xmDT_UINT32  ： UINT32
		- xmDT_INT64   ： INT64
		- xmDT_UINT64  ： UINT64
		- xmDT_FLOAT   ： FLOAT
		- xmDT_DOUBLE  ： DOUBLE
		- xmDT_STRING  ： STRING
		- xmDT_BUFFER  ： BUFFER
		- xmDT_STRUCT  ： STRUCT
	*/
	xmBASE_EXPORT const char* xmTypeName(xmEDataType eType);

	/** 根据数据名称，获得数据类型
	@return
		数据名称，未知名称返回xmDT_UNKNOWN。
	*/
	xmBASE_EXPORT xmEDataType xmDataType(const char* strName);

	/** 判断一个类型是否为数值类型。数值类型指各类整数和浮点数
	@return
		是数值类型返回true
	*/
	xmBASE_EXPORT bool xmIsNumber(xmEDataType eType);

	/** 判断一个类型是否为整数类型
	@return
		是整数类型返回true
	*/
	xmBASE_EXPORT bool xmIsInteger(xmEDataType eType);

	/** 判断一个类型是否为浮点数类型
	@return
		是浮点数类型返回true
	*/
	xmBASE_EXPORT bool xmIsDecimals(xmEDataType eType);
}

#endif //__SSA_BASE_TYPE_H