#pragma once
#ifndef __SSA_VDR_NODE_H
#define __SSA_VDR_NODE_H

#include "export.h"
#include "../../include/base/base.h"
#include "dataset.h"
#include "defs.h"

namespace ssa
{
	/**
	*  节点类型。
	*/
	typedef enum __tagNodeType
	{
		xmENT_NOTHING = 0,    /**< 未定义。 */
		xmENT_OBSERV,		  /**< 观察节点。 */
		xmENT_WORK,		      /**< 工作节点。 */
		xmENT_CTRL,	          /**< 控制节点。 */
		xmENT_MAX,
	} xmENodeType;

#pragma pack(1)
	/**
	*  节点属性描述信息。
	*/
	struct xmVDR_EXPORT xmNodeAttr
	{
	public:
		//std::string     m_strName;   
		//数据的名字不能超过16个汉字
		//char            m_strName[xmENameLenth::xmENL_NODE];
		/**
		* 节点类型。
		*/
		unsigned char   m_ndType;// = xmENodeType::xmENT_NOTHING;
		/**
		* 所属作用域。
		*/
		unsigned short  m_uRegion;// = 0;
		/**
		* 全局ID。
		*/
		unsigned int    m_uID;
		/**
		* 全局ID等信息是否自动分配。
		* 需要从服务器分配资源还是客户端制定，1表示自动分配。
		*/
		unsigned int    m_uAutoAlloc;// = 1;

	public:
		xmNodeAttr();
	};
#pragma pack()

	/**
	*  节点描述信息。
	*/
	class xmVDR_EXPORT xmNode
	{
	public:
		xmNode(void);
		xmNode(const char* name);
		xmNode(const xmNode& data);
		virtual ~xmNode(void);

		/**
		* 等号重载。
		*/
		void operator=(const xmNode& b);
		/**
		* 设置属性。
		* @param strName 名字。
		* @param eType 类型。
		* @param uRegion 所属域。
		* @return 为空。
		*/
		void SetAttr(const char* strName, xmENodeType eType, unsigned short uRegion);
		/**
		* 设置属性。
		* @param strName 名字。
		* @param nAttr 属性描述结构信息。
		* @return 为空。
		*/
		void SetAttr(const char* strName, const xmNodeAttr& nAttr);
		/**
		* 设置属性。
		* @param nAttr 属性描述结构信息。
		* @return 为空。
		*/
		void SetAttr(const xmNodeAttr& nAttr);

	public:
		/**
		* 节点名字。
		* 经讨论认为应该提供一个不限制长度的名字。
		*/
		std::string                 m_strName;
		/**
		* 节点基本属性。
		*/
		xmNodeAttr                  m_Attr;
		/**
		* 描述信息。
		* 可以用于描述节点的详细信息。
		*/
		xmByteBuffer                m_dynDescribe;
		/**
		* 关联的数据集。
		* 用于存储节点所连接的数据集。
		*/
		std::vector<std::string>    m_linkedDatasets;
	};
}

#endif //__SSA_VDR_NODE_H