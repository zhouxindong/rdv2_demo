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
	*  �ڵ����͡�
	*/
	typedef enum __tagNodeType
	{
		xmENT_NOTHING = 0,    /**< δ���塣 */
		xmENT_OBSERV,		  /**< �۲�ڵ㡣 */
		xmENT_WORK,		      /**< �����ڵ㡣 */
		xmENT_CTRL,	          /**< ���ƽڵ㡣 */
		xmENT_MAX,
	} xmENodeType;

#pragma pack(1)
	/**
	*  �ڵ�����������Ϣ��
	*/
	struct xmVDR_EXPORT xmNodeAttr
	{
	public:
		//std::string     m_strName;   
		//���ݵ����ֲ��ܳ���16������
		//char            m_strName[xmENameLenth::xmENL_NODE];
		/**
		* �ڵ����͡�
		*/
		unsigned char   m_ndType;// = xmENodeType::xmENT_NOTHING;
		/**
		* ����������
		*/
		unsigned short  m_uRegion;// = 0;
		/**
		* ȫ��ID��
		*/
		unsigned int    m_uID;
		/**
		* ȫ��ID����Ϣ�Ƿ��Զ����䡣
		* ��Ҫ�ӷ�����������Դ���ǿͻ����ƶ���1��ʾ�Զ����䡣
		*/
		unsigned int    m_uAutoAlloc;// = 1;

	public:
		xmNodeAttr();
	};
#pragma pack()

	/**
	*  �ڵ�������Ϣ��
	*/
	class xmVDR_EXPORT xmNode
	{
	public:
		xmNode(void);
		xmNode(const char* name);
		xmNode(const xmNode& data);
		virtual ~xmNode(void);

		/**
		* �Ⱥ����ء�
		*/
		void operator=(const xmNode& b);
		/**
		* �������ԡ�
		* @param strName ���֡�
		* @param eType ���͡�
		* @param uRegion ������
		* @return Ϊ�ա�
		*/
		void SetAttr(const char* strName, xmENodeType eType, unsigned short uRegion);
		/**
		* �������ԡ�
		* @param strName ���֡�
		* @param nAttr ���������ṹ��Ϣ��
		* @return Ϊ�ա�
		*/
		void SetAttr(const char* strName, const xmNodeAttr& nAttr);
		/**
		* �������ԡ�
		* @param nAttr ���������ṹ��Ϣ��
		* @return Ϊ�ա�
		*/
		void SetAttr(const xmNodeAttr& nAttr);

	public:
		/**
		* �ڵ����֡�
		* ��������ΪӦ���ṩһ�������Ƴ��ȵ����֡�
		*/
		std::string                 m_strName;
		/**
		* �ڵ�������ԡ�
		*/
		xmNodeAttr                  m_Attr;
		/**
		* ������Ϣ��
		* �������������ڵ����ϸ��Ϣ��
		*/
		xmByteBuffer                m_dynDescribe;
		/**
		* ���������ݼ���
		* ���ڴ洢�ڵ������ӵ����ݼ���
		*/
		std::vector<std::string>    m_linkedDatasets;
	};
}

#endif //__SSA_VDR_NODE_H