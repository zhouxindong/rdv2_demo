#ifndef __SSA_VDR_ICOMMPROXY_H
#define __SSA_VDR_ICOMMPROXY_H
#include "export.h"
#include "packet.h"
#include "vdrattr.h"
#include "iservice.h"
#include "iproxy.h"

namespace ssa
{
	/**
	* ͨ�ŽӿڵĶ��壬ͨ������ģʽ������ʵ�ֲ����ʽ��ͨ�Žӿڽ��й���
	*/
	class xmVDR_EXPORT xmICommProxy : public xmIProxy
	{
	public:
		xmICommProxy();
		virtual ~xmICommProxy();

		/**
		* ��ʼ���ӿڣ���Ҫ�̳�ʵ�֡�
		* @param void* ��ʼ��ʱ����Ĳ���ָ�룬��Ϊ��ִ�г�ʼ��ʱ��ͬ��ͨ�Žӿڣ������ǲ�һ���ģ�����ֻ��ʹ��void*���ͽ��б�ʾ��
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool                Initialize(void*) = 0;
		/**
		* ��ȡVDR��������Ϣ����Ҫ�̳�ʵ�֡�
		* @return ������Ϣָ�룬�ɸ��ݷ��ص���Ϣ����һ������Ϣ���廯Ϊĳһ�����͡�
		*/
		virtual xmVDRAttr*          GetVDRAttr() =0;
		/**
		* ��ȡͨ�Žӿڵ����ͣ���Ҫ�̳�ʵ�֡�
		* @return �ӿ���Ϣ���͡�
		*/
		virtual xmEVDRCommProxyType GetCommProxyType()= 0;
		/**
		* ��ȡ/����Ĭ��ͨ�ŷ�ʽ�������ɿ��Ͳ��ɿ�����ģʽ��
		* @return ͨ�ŷ�ʽ��
		*/		
		virtual int                 GetDefaultQos();
		virtual void                SetDefaultQos(int nQos);
		/**
		* ��ȡͨ�Žӿڵ�ID�ţ�Ŀǰʹ�ýӿ���������ΪID�ű�ʾ��ͬ��ͨ�Žӿڡ�
		* @return �ӿ�ID�š�
		*/
		virtual int                 GetProxyID() { return GetCommProxyType(); };
		/**
		* �رսӿڣ���Ҫ�̳�ʵ�֡�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool                Close() = 0;
		/**
		* ������������Ҫ�̳�ʵ�֡�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual int                 Start() = 0;
		/**
		* ���ͱ�����VDR���磬��Ҫ�̳�ʵ�֡�
		* @param pkt ��Ҫ���͵ı��ġ�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool                Send(const xmPacket& pkt)  = 0;
		/**
		* ָ���������ݵķ���ʹ�÷���ӿڱ�ʾ������ʹ�ò�ͬ�ķ������������ݡ�
		* @param pService ���ݴ������ӿڡ�
		* @return �ա�
		*/
		virtual void                SetService(xmIService* pService);
		/**
		* ��ȡָ���������ݵķ���
		* @return ���ݴ������ӿ�ָ�룬0��ʾδ��ȡ����
		*/
		virtual xmIService*         GetService();
		/**
		* �յ����ģ����зַ�����
		* @param buf ���Ļ������׵�ַ��
		* @param buf ���Ļ��������ȡ�
		* @return ��������0��ʾ�ɹ���-1��ʾ����ʧ�ܡ�
		*/
		virtual int                 Recv(char* buf, int nLen);

	private:
		xmIService*                 m_pService;
		int                         m_nQos;
	};
}

#endif //__SSA_VDR_ICOMMPROXY_H