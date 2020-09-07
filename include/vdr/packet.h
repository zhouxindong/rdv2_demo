#ifndef __SSA_VDR__PACKET_H
#define __SSA_VDR__PACKET_H
#pragma once

#include "export.h"
#include "../../include/base/base.h"
#include "../../include/base/socket.h"
#include "packetcodes.h"
#include "vdrattr.h"
#include "bytebuffer.h"

namespace ssa
{
	typedef enum __tagBuffType
	{
		xmEBT_NOTHING = 0,
		xmEBT_DATASET,		//	
		xmEBT_DATA,		//	
		xmEBT_MULTIDATA,    //	
		xmEBT_VALUE,	//
		xmEBT_SESSION,
		xmEBT_BULLETIN,
		xmEBT_LOG,
		xmEBT_MAX,
	} xmEBuffType;

	typedef enum __tagSendDirection
	{
		xmESD_OUT = 0,
		xmESD_IN,		 //	
	} xmESendDirection;

#pragma pack(1)
	//�������ʶ���ݵ�Դ��ַ��Ŀ���ַ
	union uAddr
	{
		sockaddr_in Sock;                 //ʹ�������ַ��ʶ
		int         No;//= 0;             //ʹ�����кű�ʶ
		char        Name[16]/* = { 0 }*/; //ʹ�����ֱ�ʶ
	};

	struct xmHead
	{
	public:
		//VDR���к�
		unsigned short   m_uVDRID;//  = 0x6B42;
		//������
		unsigned short   m_uRegion;// = 0;
		//���ݰ�����
		unsigned short   m_uPackCode;// = xmEPackCodes::xmPACK_NOTHING;
		//�ظ�����
		unsigned short   m_eRspCode;// = 0;
		//������������ͻ��˻���˫���
		unsigned char    m_ePackTarget;// = xmEEndpointType::xmEET_SERVER;
		//ʱ���
		long long        m_lTimeStamp;// = 0;
		//�Ƿ���Ҫʹ��ID��Ϊ�˵����ĻỰ�����Ƕಥ
		//��Ҫ���ڿ��������Ƿ�ֻ�з����ߴ���1��ʾֻ�з����ߴ���0��ʾ�������ն˴���
		unsigned char    m_bUseFlowSeq;// = 0;
		//��������Ҫ�����������򣬻�����Ҫ���Ƶȴ�ʱ���ʱ��ʹ��
		unsigned int     m_uFlowSeq;// = 0;
		//ʹ���Ǹ��Ự������
		unsigned short   m_uSessionID;      // = xmESessionType::xmEST_LOGIN;
		//�����ⷢ�ͣ�����������Ϣ����
		unsigned char    m_uSendDir;// = xmESendDirection::xmESD_OUT;
		//���ݰ����ȣ�������ͷ�ĳ���
		unsigned int     m_uLength;// = 0;
		//��������������,xmEBT_DATASET,xmEBT_DATA,xmEBT_MULTIDATA,xmEBT_VALUE,xmEBT_SESSION,xmEBT_BULLETIN,xmEBT_LOG,xmEBT_MAX
		unsigned char    m_btType;// = 0;
		//֡����        
		long long        m_lFrameCount;// = 0;
		//��Դ,Ĭ�ϵ�4���ֽ�Ϊ������Դ�����кţ���4���ֽ�����֡����
		char             m_addrSrc[8];// = { 0 };
		//Ŀ�ģ���login�Ự�У����ڿͻ����������ѯ��IP��ַʹ�����
		//ʹ�� m_addrDst[0]��ʾIP��ַ�Ƿ���Ч��m_addrDst[2-3]2���ֽڱ�ʾ�˿ںţ���4�ֽڱ�ʾ��������ַ
		char             m_addrDst[8];// = { 0 };
		//����8���ֽ�
		//ʹ�õ�0�ֽڱ�ʾͨ������Qos�ķ�ʽ��Ŀǰ���������ࣺ0ʹ��Ĭ�Ϸ�����1ʹ�ò��ɿ���2Ϊ�ɿ�
		//Ĭ����������ǲ��ɿ����ٶȿ죬������ͨ��ʹ�ò��ɿ��ķ�ʽ��ָ�����ʹ�ÿɿ���ʽ
		//������δ�����ͨ�Ŵ������
		char             m_cRerv[8];// = { 0 };
		//���Ǹ��汾����Ϣ������������ͬ����汾
		unsigned short   m_nVersion;// = 0;

		xmHead()
		{
			m_uVDRID  = 0x6B42;
			m_uRegion = 0;
			m_uPackCode = xmEPackCodes::xmPACK_NOTHING;
			m_eRspCode = 0;
			m_ePackTarget = xmEEndpointType::xmEET_SERVER;
			m_lTimeStamp = 0;
			m_bUseFlowSeq = 0;
			m_uFlowSeq = 0;
			m_uSendDir = xmESendDirection::xmESD_OUT;
			m_uLength = 0;
			m_btType = 0;
			m_lFrameCount = 0;
			memset(m_addrSrc,0,8);
			memset(m_addrDst,0,8);
			memset(m_cRerv,0,8);
			m_nVersion = 0;
		}

	};
#pragma pack ()  

	class xmPacket
	{
	public:
		xmPacket() 
		{
			m_nRepet = 1;
		};
		xmPacket(const xmPacket& pkt) 
		{
			m_pktHead = pkt.m_pktHead;
			m_dynBuf = pkt.m_dynBuf;
			m_nRepet = pkt.m_nRepet;
		};

		virtual ~xmPacket()
		{
			Destroy();
		};

		virtual void  Destroy()
		{
			m_dynBuf.Clear();
		};

		//�����ݰ�д������
		virtual void  WriteData(unsigned char* in, unsigned int nSize) {};
		//�ӽ����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out) = 0;
		//�����ݱ����н�������
		virtual void  Decode(unsigned char* in, unsigned int nSize)= 0;

	public:
		xmHead           m_pktHead;
		xmByteBuffer     m_dynBuf;
		int              m_nRepet;
	};

};

#endif