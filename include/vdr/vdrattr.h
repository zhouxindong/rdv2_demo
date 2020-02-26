#pragma once
#ifndef __SSA_VDR_VDRATTR_H
#define __SSA_VDR_VDRATTR_H

#include "export.h"
#include "defs.h"
#include <string>

namespace ssa
{

	/**
	*  VDRϵͳ���Ի��࣬����ϵͳ���ʹ�ã������ϵͳ������Ϣͨ������Ķ���ʵ�֣�������VDR�Ļ�����Ϣ��
	*  1.VDR��ID�ţ�
	*  2.������
	*  3.ͨ�ŷ�ʽ��
	*  4.��ǰVDR�����ݵĽ�ɫ��
	*/
	class xmVDR_EXPORT xmVDRAttr
	{
	public:
		xmVDRAttr();
		/**
		* VDR��ID�š�
		* VDRͨ��ID��������һ̨������ʵ�ֶ�����ݲֿ⡣
		*/
		unsigned short          m_ID;
		/**
		* ������
		* һ��VDR������Դ��ڶ����
		*/
		unsigned short          m_uRegion;
		/**
		* ͨ�ŷ�ʽ��
		* ���õ�ͨ�ŷ�ʽ��Ŀǰʵ��������,Ҳ�������ж���ID����xmECM_MAX�ȿ�,�ڴ�����ʱ�����ʹ�á�
		*/
		xmEVDRCommProxyType     m_eCommType;
		/**
		* ��ɫ���塣
		* ���ֵ�ǰVDR�ӿ��ڽ����������ݵĽ�ɫ�������Ƿ������������ݼ����Լ�����ʱ����RTE��
		*/
		xmEEndpointType         m_eEndpointType;
	};

	/**
	*  �ಥ��ʽ��VDR���ԣ��̳���xmVDRAttr�������˶ಥͨ�ŷ�ʽ�Ļ�������
	*  �������漰�������Ե����ã�������ú�����ʽ��Ҳ����ֱ�Ӷ����Ա�����ֵ��
	*/
	class xmVDR_EXPORT xmMCastVDRAttr : public xmVDRAttr
	{
	public:
		xmMCastVDRAttr();
		xmMCastVDRAttr(const xmMCastVDRAttr& attr);
		virtual ~xmMCastVDRAttr(void);

		/**
		* �ж��������������Ƿ�һ�¡�
		* @param attr ���Ƚϵ�����ֵ��
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		bool operator==(const xmMCastVDRAttr &attr);
		/**
		* ���÷�������ַ�Ͷ˿ںţ����ڷ����������ⲿ�Ŀ��Ƶȵ����ķ����������
		* @param servIP ��������IP��ַ��
		* @param uPort �������Ķ˿ںš�
		* @return Ϊ�ա�
		*/
		void SetServerAddr(const char* servIP, unsigned short uPort);
		/**
		* ���ÿͻ��˵�ַ�Ͷ˿ںţ����ڶԷ�������ͻ���֮��ķ����������
		* @param userIP �ͻ��˵�IP��ַ��
		* @param uPort �ͻ��˵Ķ˿ںš�
		* @return Ϊ�ա�
		*/
		void SetUserAddr(const char* userIP, unsigned short uPort);
		/**
		* �������ڷ���ϵͳʵ�ʴ���ʹ�õ��鲥��ַ�Ͷ˿ںţ��������á�
		* @param mcastIP �鲥�ĵ�IP��ַ��
		* @param uMcastPort �ͻ��˵Ķ˿ںš�
		* @return Ϊ�ա�
		*/
		void SetGroupAddr(const char* mcastIP, unsigned short uMcastPort);
		/**
		* �������ڷ���ϵͳʵ�ʴ���ʹ�õ��û��������룬���ڶ����ݵļ��ܺ͵ķ���ϵͳ���û�Ȩ�ޣ���ʵ�֡�
		* @param userName �û����֡�
		* @param userPwd ���롣
		* @return Ϊ�ա�
		*/
		void SetUserPwd(const char* userName, const char*  userPwd);

	public:
		/**
		* ��������ַ��
		*/
		std::string         m_srvIP;
		/**
		* �������˿ڡ�
		*/
		unsigned short      m_srvPort;
		/**
		* �ͻ��˵�ַ��
		*/
		std::string         m_usrIP;
		/**
		* �ͻ��˶˿ڡ�
		*/
		unsigned short      m_usrPort;
		/**
		* �鲥��ַ��
		*/
		std::string         m_groupIP;
		/**
		* �鲥�˿ںš�
		*/
		unsigned short      m_groupPort;
		/**
		* �û����֡�
		*/
		std::string         m_usrName;
		/**
		* �û����롣
		*/
		std::string         m_usrPwd;
	};

	/**
	*  ����ͨ�ŷ�ʽ��VDR���ԣ��̳���xmVDRAttr�������˱���ͨ�ŷ�ʽ�Ļ�������
	*/
	class xmVDR_EXPORT xmLocalVDRAttr : public xmVDRAttr
	{
	public:
		xmLocalVDRAttr(void);
		xmLocalVDRAttr(const xmLocalVDRAttr& attr);
		virtual ~xmLocalVDRAttr(void) {}

		/**
		* ���ó�Ϊ�����ݼ��ķ�ʽ��
		*/
		typedef enum __tagToBeMajor
		{
			xmETBM_AUTO = 0,	/**< ��ʾ��VDR�Զ�ѡ�� */
			xmETBM_OTHER,		/**< ��ʾָ�������ݼ����������ݼ����Ǳ����ݼ��� */
			xmETBM_THIS,		/**< ��ʾָ�������ݼ��������ݼ��Ǳ����ݼ��� */
		}xmETBMType;
		/**
		* �����ݼ���ѡ��ʽ��
		*/
		xmETBMType   m_eToBeMajor;
		/**
		* VDRʱ�ӽ��ģ���λ��ms��
		*/
		unsigned int m_uTimer;
	};
}

#endif //__SSA_VDR_VDRATTR_H