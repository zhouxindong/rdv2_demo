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
	*  ��������������Ϣ��
	*/
	struct xmVDR_EXPORT xmDataAttr
	{
	public:
		//���ݵ����ֲ��ܳ���16������
		//char            m_strName[xmENameLenth::xmENL_DATA];
		/**
		* �������͡�
		*/
		unsigned char   m_dtType;// = xmEDataType::xmDT_UNKNOWN;
		/**
		* ����������
		*/
		unsigned short  m_uRegion;// = 0;
		/**
		* �������ݼ���
		*/
		unsigned int    m_uDataSetID;// = 0;
		/**
		* ���ȣ�ͨ�������˫�����Ѿ����á�
		*/
		unsigned int    m_uSize;// = 8;
		/**
		* ��λʱ�䡣
		*/
		unsigned int    m_uResetTime;// = 0xFFFFFFFF;
		/**
		* �Ƿ���Թ���
		* ϵͳʹ�ã��û�ʹ��ʱ���ùܡ�
		*/
		bool            m_bShared;// = false;
		/**
		* ����ȫ��ID��
		* ϵͳʹ�ã��û�ʹ��ʱ���ùܡ�
		*/
		unsigned int    m_uGlobalID;// = 0;
		/**
		* �����ڲ�ʹ��ID��
		* ϵͳʹ�ã��û�ʹ��ʱ���ùܡ�
		*/
		unsigned int    m_uInnerID;// = 0;
		/**
		* ������͵ı�ʾ������ÿ���ֽڵĺ������£�
		* ��0�ֽ��Ƿ�������Զ�������Դ����Ҫ�ӷ�����������Դ���ǿͻ����ƶ���1��ʾ�Զ����䡣
		* ��1�ֽڱ�ʾ�Ƿ�Ϊָ������ݣ�0��ʾ���ݣ�1��ʾָ����Ϣ��
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
	*  ����������Ϣ��
	*  ����UDP���籨�ĳ���65535���ƣ���Ҫ�û���֤ÿ��data���л���Ĵ�СӦ����60k���ֽ����ڡ�
	*  �������ȷ���Ƿ񳬹�60k������ʹ�ú���EncodeSize�����жϡ�
	*  �����Struct�����ͣ���������ݽ���������ʹ��void SetAttr(const char* strName, const xmValue& eType, unsigned short uRegion, unsigned int dataSetID, unsigned int uResetTime = 0xFFFFFFFF);
	*  ����ֱ�Ӷ�m_dynDescribe���и�ֵ
	*  �г�ʼ����Ϣʱ����Ҫ��m_dynInitValue���и�ֵ
	*/
	class xmVDR_EXPORT xmData
	{
	public:
		xmData(void);
		xmData(const char* name);
		xmData(const xmData& data);
		virtual ~xmData(void);

		/**
		* �Ⱥ����ء�
		*/
		void operator=(const xmData& b);
		/**
		* �����������ԡ�
		* @param strName �������֡�
		* @param eType �������͡�
		* @param uRegion ������
		* @param dataSetID �������ϡ�
		* @param nSize ���ȡ�
		* @param pInitBuf ��ʼֵ��������ַ��
		* @param uResetTime ��λʱ�䡣
		* @return Ϊ�ա�
		*/
		void SetAttr(const char* strName, xmEDataType eType, unsigned short uRegion, unsigned int dataSetID, unsigned int nSize , char* pInitBuf = 0, unsigned int uResetTime = 0xFFFFFFFF);
		/**
		* �����������ԡ�
		* @param strName �������֡�
		* @param eType �ṹ��Value�������͡�
		* @param uRegion ������
		* @param dataSetID �������ϡ�
		* @param uResetTime ��λʱ�䡣
		* @return Ϊ�ա�
		*/
		void SetAttr(const char* strName, const xmValue& eType, unsigned short uRegion, unsigned int dataSetID, unsigned int uResetTime = 0xFFFFFFFF);
		/**
		* �����������ԡ�
		* @param strName �������֡�
		* @param dAttr �������������ṹ��Ϣ��
		* @param pInitBuf ��ʼֵ��������ַ��
		* @return Ϊ�ա�
		*/
		void SetAttr(const char* strName, const xmDataAttr& dAttr, char* pInitBuf = 0);
		/**
		* �����������ԡ�
		* @param strName �������֡�
		* @param dAttr �������������ṹ��Ϣ��
		* @param eType �ṹ��Value�������͡�
		* @return Ϊ�ա�
		*/
		void SetAttr(const char* strName, const xmDataAttr& dAttr, const xmValue& eType);
		/**
		* �����������ԡ�
		* @param dAttr �������������ṹ��Ϣ��
		* @param pInitBuf ��ʼֵ��������ַ��
		* @return Ϊ�ա�
		*/
		void SetAttr(const xmDataAttr& dAttr, char* pInitBuf = 0);
		/**
		* �����������ԡ�
		* @param dAttr �������������ṹ��Ϣ��
		* @param eType �ṹ��Value�������͡�
		* @return Ϊ�ա�
		*/
		void SetAttr(const xmDataAttr& dAttr, const xmValue& eType);
		/**
		* �����������ԡ�
		* @param strName �������֡�
		* @return Ϊ�ա�
		*/
		void SetName(const char* strName);
		/**
		* ��ȡ����������Ϣ�����ĳ��ȡ�
		* @return ������Ϣ�����ĳ��ȡ�
		*/
		int  EncodeSize();

	public:
		/**
		* �������֡�
		* ��������ΪӦ���ṩһ�������Ƴ��ȵ����֡�
		*/
		std::string     m_strName;
		/**
		* ���ݻ������ԡ�
		*/
		xmDataAttr      m_Attr;
		/**
		* ��ʼֵ��
		*/
		xmByteBuffer    m_dynInitValue;
		/**
		* ������Ϣ��
		* ���������������ݵ���ϸ��Ϣ��Ŀǰ��Ҫ�������ݽṹ����Ϣ��������
		*/
		xmByteBuffer    m_dynDescribe;
		/**
		* ���ݵİ汾��
		* ���������и��µ�ʱ��ʹ�á�
		*/
		unsigned char   m_nVersion;
	};
}

#endif