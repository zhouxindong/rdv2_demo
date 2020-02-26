
#pragma once
#ifndef __SSA_VDR_DATASET_H
#define __SSA_VDR_DATASET_H

#include "export.h"
#include "data.h"

namespace ssa
{
	class xmVDR;

	/**
	*  ���ݼ����͡�
	*/
	typedef enum __tagDataSetType
	{
		xmEDST_NOTHING = 0, /**< δ���塣 */
		xmEDST_READ,		/**< �ɶ����ݼ��ϡ� */	
		xmEDST_WRITE,		/**< ��д���ݼ��ϡ� */	
		xmEDST_RW,	        /**< �ɶ�д���ݼ��ϡ� */	
		xmEDST_MAX,
	} xmEDataSetType;

#pragma pack(1)
	/**
	*  ���ݼ�����������Ϣ��
	*/
	struct xmDataSetAttr
	{
	public:
		//���ݼ������ֲ��ܳ���8������
		//char                   m_strName[xmENameLenth::xmENL_DATASET];
		/**
		* ���ݼ�ID��
		* ÿ�����ݼ�����һ��ID���ڴ��ȫ������ID�����֣�Ŀǰ����ƣ�IDֻ�ܵ�0xFFFFFFFF/2-1��֮���ID���ھ��񲿷ֵ��ڴ����Ҳ�������Ҫ����0xFFFFFFFF/2-1��
		*/
		unsigned int           m_uID;
		/**
		* ����������
		*/
		unsigned short         m_uRegion = 0;
		/**
		* ���ݼ�������ʱ�䡣
		*/
		unsigned int           m_uDeadTime = 0xFFFFFFFF;
		/**
		* ͬ��ʱ�����ڣ�Ĭ��Ϊ1�����ڡ�
		*/
		unsigned int		   m_uSyncTime = 1;
		/**
		* ���ݼ����͡�
		*/
		xmEDataSetType         m_eType = xmEDataSetType::xmEDST_RW;
		/**
		* ȫ����Ϣ�Ƿ��Զ����䡣
		* ��Ҫ�ӷ�����������Դ���ǿͻ����ƶ���1��ʾ�Զ����䡣
		*/
		unsigned int           m_uAutoAlloc = 1;
	};
#pragma pack()

	/**
	*  ���ݼ�������Ϣ��
	*/
	class xmVDR_EXPORT xmDataSet
	{
	public:
		xmDataSet(void);
		xmDataSet(const char* name);
		xmDataSet(const xmDataSet& ds);
		virtual ~xmDataSet(void);
		/**
		* ��ȡ���ݼ����͡�
		* @return ��ȡ���ݼ����͡�
		*/
		virtual xmEDataSetType   GetType(void);
		/**
		* �Ⱥ����ء�
		*/
		void    operator=(const xmDataSet& b);
		/**
		* �������ԡ�
		* @param strName ���֡�
		* @return Ϊ�ա�
		*/
		void    SetName(const char* name);
		/**
		* �������ԡ�
		* @param dsAttr ���ݼ��������Խṹ������Ϣ��
		* @return Ϊ�ա�
		*/
		void    SetAttr(const xmDataSetAttr& dsAttr);
		/**
		* �������ԡ�
		* @param name ���ݼ����֡�
		* @param dsAttr ���ݼ��������Խṹ������Ϣ��
		* @return Ϊ�ա�
		*/
		void    SetAttr(const char* name, const xmDataSetAttr& dsAttr);
	public:
		/**
		* ���ݼ����֡�
		* ��������ΪӦ���ṩһ�������Ƴ��ȵ����֡�
		*/
		std::string              m_strName;
		/**
		* ���ݼ��������ԡ�
		*/
		xmDataSetAttr            m_Attr;
		/**
		* �ڴ�汾��Ϣ��
		*/
		unsigned char            m_nMemVersion = 0;;
		/**
		* ���ݼ����������������汾��Ϣ��
		*/
		unsigned char            m_nIndexVersion = 0;
	};
}

#endif
