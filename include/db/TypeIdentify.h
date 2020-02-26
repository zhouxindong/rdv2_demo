/**************************************************
文件名    :TypeIdentify.h
功能      :运行时类型识别
相关文件  :
作者      :侯全超
-----------------------------------------------------
备注      :参照邓华的dynbuffer.h实现，用于对通用接口
		   所支持的四种类型提供运行时类型识别，便于接
		   口统一
-----------------------------------------------------
修改记录 : 
日 期        版本   修改人         修改内容 
2009/08/10   1.0    <侯全超>       创建初稿
*****************************************************/

#ifndef _XMDBI_TYPE_IDENTIFY_H
#define	_XMDBI_TYPE_IDENTIFY_H


#include <stdlib.h>
#include <stdio.h>
#include <memory>
#include <string.h>
#include <algorithm>
#include <stdarg.h>
#include "DbiBase.h"

using namespace ssa;
/*******************************************************************************
 *  实现运行时类型识别
 ******************************************************************************/

//	访问数据库所支持的数据类型
typedef enum EXMDataType
{
	XMDT_UNKNOW = 0,
	XMDT_INT,
	XMDT_UINT,
	XMDT_FLOAT,
	XMDT_STRING,
	XMDT_TM,
	XMDT_CHAR
}xmDBDATA_TYPE;


template <typename T> class XMDB_VALUE_TYPE
{
public:
	enum
	{
		TYPE = XMDT_UNKNOW, 
	};
	static string GetStrFrom(const T& Value)
	{
		return 0;
	}
	static bool Set(T& Des,const string& Value)
	{
		return false;
	}
};

template <> class XMDB_VALUE_TYPE<int>
{
public:
	enum
	{
		TYPE = XMDT_INT, 
	};
	static string GetStrFrom(const int& Value)
	{
		return CDbiBase::GetStrFromInt(Value);
	}
	static bool Set(int& Des,const string& Value)
	{			
		return false;
	}
};

template <> class XMDB_VALUE_TYPE<uint32_t>
{
public:
	enum
	{
		TYPE = XMDT_UINT, 
	};
	static string GetStrFrom(const uint32_t& Value)
	{
		return CDbiBase::GetStrFromUInt(Value);
	}
	static bool Set(uint32_t& Des,const string& Value)
	{			
		return false;
	}
};

template <> class XMDB_VALUE_TYPE<float>
{
public:
	enum
	{
		TYPE = XMDT_FLOAT, 
	};
	static string GetStrFrom(const float& Value)
	{
		return CDbiBase::GetStrFromFloat(Value);
	}
	static bool Set(float& Des,const string& Value)
	{			
		return false;
	}
};

template <> class XMDB_VALUE_TYPE<string>
{
public:
	enum
	{
		TYPE = XMDT_STRING, 
	};
	static string GetStrFrom(const string& Value)
	{
		return CDbiBase::GetStrFromString(Value);
	}
	static bool Set(string& Des,const string& Value)
	{
		Des = Value;
		return true;
	}
};

template <> class XMDB_VALUE_TYPE<char const*>
{
public:
	enum
	{
		TYPE = XMDT_STRING, 
	};
	static string GetStrFrom(const char*& Value)
	{
		return CDbiBase::GetStrFromString(Value);
	}
	static bool Set(char*& Des,const string& Value)
	{
		return false;
	}
};

template <> class XMDB_VALUE_TYPE<char>
{
public:
	enum
	{
		TYPE = XMDT_STRING, 
	};
	static string GetStrFrom(const char& Value)
	{
		return CDbiBase::GetStrFromChar(Value);
	}
	static bool Set(char& Des,const string& Value)
	{
		Des = Value[0];
		return true;
	}
};


template <> class XMDB_VALUE_TYPE<tm>
{
public:
	enum
	{
		TYPE = XMDT_TM, 
	};
	static string GetStrFrom(const tm& Value)
	{
		return CDbiBase::GetStrFromDate(Value);
	}
	static bool Set(tm& Des,const string& Value)
	{
		return false;
	}
};




#endif	/* _XMDBI_TYPE_IDENTIFY_H */