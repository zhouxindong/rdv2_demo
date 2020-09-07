#pragma once
#ifndef __SSA_VDR_DATA_H
#define __SSA_VDR_DATA_H

#include "export.h"
#include "../../include/base/base.h"
#include "bytebuffer.h"
#include "defs.h"

namespace ssa
{
#pragma pack(1)
	/**
	*  数据属性描述信息。
	*/
	struct xmVDR_EXPORT xmDataAttr
	{
	public:
		//数据的名字不能超过16个汉字
		//char            m_strName[xmENameLenth::xmENL_DATA];
		/**
		* 数据类型。
		*/
		unsigned char   m_dtType;// = xmEDataType::xmDT_UNKNOWN;
		/**
		* 所属作用域。
		*/
		unsigned short  m_uRegion;// = 0;
		/**
		* 所属数据集。
		*/
		unsigned int    m_uDataSetID;// = 0;
		/**
		* 长度，通常情况下双精度已经够用。
		*/
		unsigned int    m_uSize;// = 8;
		/**
		* 复位时间。
		*/
		unsigned int    m_uResetTime;// = 0xFFFFFFFF;
		/**
		* 是否可以共享。
		* 系统使用，用户使用时不用管。
		*/
		bool            m_bShared;// = false;
		/**
		* 数据全局ID。
		* 系统使用，用户使用时不用管。
		*/
		unsigned int    m_uGlobalID;// = 0;
		/**
		* 数据内部使用ID。
		* 系统使用，用户使用时不用管。
		*/
		unsigned int    m_uInnerID;// = 0;
		/**
		* 多个类型的表示方法，每个字节的含义如下：
		* 第0字节是否服务器自动分配资源，需要从服务器分配资源还是客户端制定，1表示自动分配。
		* 第1字节表示是否为指令还是数据，0表示数据，1表示指令信息。
		*/
		unsigned char   m_uAutoAlloc[4];// = { 1,0,0,0 };

	public:
		xmDataAttr();
	};

#pragma pack()

	class xmDataUtils
	{
	public:
		static unsigned int GetLength(xmEDataType et);
	};

	/**
	*  数据描述信息。
	*  由于UDP网络报文长度65535限制，需要用户保证每个data序列化后的大小应该在60k个字节以内。
	*  如果不能确定是否超过60k，可以使用函数EncodeSize进行判断。
	*  如果是Struct的类型，必须对数据进行描述，使用void SetAttr(const char* strName, const xmValue& eType, unsigned short uRegion, unsigned int dataSetID, unsigned int uResetTime = 0xFFFFFFFF);
	*  或者直接对m_dynDescribe进行赋值
	*  有初始化信息时，需要对m_dynInitValue进行赋值
	*/
	class xmVDR_EXPORT xmData
	{
	public:
		xmData(void);
		xmData(const char* name);
		xmData(const xmData& data);
		virtual ~xmData(void);

		/**
		* 等号重载。
		*/
		void operator=(const xmData& b);
		/**
		* 设置数据属性。
		* @param strName 数据名字。
		* @param eType 数据类型。
		* @param uRegion 所属域。
		* @param dataSetID 所属集合。
		* @param nSize 长度。
		* @param pInitBuf 初始值缓冲区地址。
		* @param uResetTime 复位时间。
		* @return 为空。
		*/
		void SetAttr(const char* strName, xmEDataType eType, unsigned short uRegion, unsigned int dataSetID, unsigned int nSize , char* pInitBuf = 0, unsigned int uResetTime = 0xFFFFFFFF);
		/**
		* 设置数据属性。
		* @param strName 数据名字。
		* @param eType 结构化Value数据类型。
		* @param uRegion 所属域。
		* @param dataSetID 所属集合。
		* @param uResetTime 复位时间。
		* @return 为空。
		*/
		void SetAttr(const char* strName, const xmValue& eType, unsigned short uRegion, unsigned int dataSetID, unsigned int uResetTime = 0xFFFFFFFF);
		/**
		* 设置数据属性。
		* @param strName 数据名字。
		* @param dAttr 数据属性描述结构信息。
		* @param pInitBuf 初始值缓冲区地址。
		* @return 为空。
		*/
		void SetAttr(const char* strName, const xmDataAttr& dAttr, char* pInitBuf = 0);
		/**
		* 设置数据属性。
		* @param strName 数据名字。
		* @param dAttr 数据属性描述结构信息。
		* @param eType 结构化Value数据类型。
		* @return 为空。
		*/
		void SetAttr(const char* strName, const xmDataAttr& dAttr, const xmValue& eType);
		/**
		* 设置数据属性。
		* @param dAttr 数据属性描述结构信息。
		* @param pInitBuf 初始值缓冲区地址。
		* @return 为空。
		*/
		void SetAttr(const xmDataAttr& dAttr, char* pInitBuf = 0);
		/**
		* 设置数据属性。
		* @param dAttr 数据属性描述结构信息。
		* @param eType 结构化Value数据类型。
		* @return 为空。
		*/
		void SetAttr(const xmDataAttr& dAttr, const xmValue& eType);
		/**
		* 设置数据属性。
		* @param strName 数据名字。
		* @return 为空。
		*/
		void SetName(const char* strName);
		/**
		* 获取数据描述信息编码后的长度。
		* @return 描述信息编码后的长度。
		*/
		int  EncodeSize();

	public:
		/**
		* 数据名字。
		* 经讨论认为应该提供一个不限制长度的名字。
		*/
		std::string     m_strName;
		/**
		* 数据基本属性。
		*/
		xmDataAttr      m_Attr;
		/**
		* 初始值。
		*/
		xmByteBuffer    m_dynInitValue;
		/**
		* 描述信息。
		* 可以用于描述数据的详细信息，目前主要用于数据结构化信息的描述。
		*/
		xmByteBuffer    m_dynDescribe;
		/**
		* 数据的版本。
		* 用于数据有更新的时候使用。
		*/
		unsigned char   m_nVersion;
	};
}

#endif