
#pragma once
#ifndef __SSA_VDR_DATASET_H
#define __SSA_VDR_DATASET_H

#include "export.h"
#include "data.h"

namespace ssa
{
	class xmVDR;

	/**
	*  数据集类型。
	*/
	typedef enum __tagDataSetType
	{
		xmEDST_NOTHING = 0, /**< 未定义。 */
		xmEDST_READ,		/**< 可读数据集合。 */	
		xmEDST_WRITE,		/**< 可写数据集合。 */	
		xmEDST_RW,	        /**< 可读写数据集合。 */	
		xmEDST_MAX,
	} xmEDataSetType;

#pragma pack(1)
	/**
	*  数据集属性描述信息。
	*/
	struct xmDataSetAttr
	{
	public:
		//数据集的名字不能超过8个汉字
		//char                   m_strName[xmENameLenth::xmENL_DATASET];
		/**
		* 数据集ID。
		* 每个数据集合有一个ID，内存等全部是由ID来区分，目前的设计，ID只能到0xFFFFFFFF/2-1，之后的ID用于镜像部分的内存管理，也就是最大不要超过0xFFFFFFFF/2-1。
		*/
		unsigned int           m_uID;
		/**
		* 所属作用域。
		*/
		unsigned short         m_uRegion = 0;
		/**
		* 数据集死亡的时间。
		*/
		unsigned int           m_uDeadTime = 0xFFFFFFFF;
		/**
		* 同步时间周期，默认为1个周期。
		*/
		unsigned int		   m_uSyncTime = 1;
		/**
		* 数据集类型。
		*/
		xmEDataSetType         m_eType = xmEDataSetType::xmEDST_RW;
		/**
		* 全局信息是否自动分配。
		* 需要从服务器分配资源还是客户端制定，1表示自动分配。
		*/
		unsigned int           m_uAutoAlloc = 1;
	};
#pragma pack()

	/**
	*  数据集描述信息。
	*/
	class xmVDR_EXPORT xmDataSet
	{
	public:
		xmDataSet(void);
		xmDataSet(const char* name);
		xmDataSet(const xmDataSet& ds);
		virtual ~xmDataSet(void);
		/**
		* 获取数据集类型。
		* @return 获取数据集类型。
		*/
		virtual xmEDataSetType   GetType(void);
		/**
		* 等号重载。
		*/
		void    operator=(const xmDataSet& b);
		/**
		* 设置属性。
		* @param strName 名字。
		* @return 为空。
		*/
		void    SetName(const char* name);
		/**
		* 设置属性。
		* @param dsAttr 数据集基本属性结构描述信息。
		* @return 为空。
		*/
		void    SetAttr(const xmDataSetAttr& dsAttr);
		/**
		* 设置属性。
		* @param name 数据集名字。
		* @param dsAttr 数据集基本属性结构描述信息。
		* @return 为空。
		*/
		void    SetAttr(const char* name, const xmDataSetAttr& dsAttr);
	public:
		/**
		* 数据集名字。
		* 经讨论认为应该提供一个不限制长度的名字。
		*/
		std::string              m_strName;
		/**
		* 数据集基本属性。
		*/
		xmDataSetAttr            m_Attr;
		/**
		* 内存版本信息。
		*/
		unsigned char            m_nMemVersion = 0;;
		/**
		* 数据集中数据排列索引版本信息。
		*/
		unsigned char            m_nIndexVersion = 0;
	};
}

#endif
