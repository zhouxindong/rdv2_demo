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
	//联合体标识数据的源地址和目标地址
	union uAddr
	{
		sockaddr_in Sock;             //使用网络地址标识
		int         No = 0;               //使用序列号标识
		char        Name[14]/* = { 0 }*/; //使用名字标识
	};

	struct xmHead
	{
	public:
		//VDR序列号
		unsigned short   m_uVDRID  = 0x6B42;
		//作用域
		unsigned short   m_uRegion = 0;
		//数据包类型
		unsigned short   m_uPackCode = xmEPackCodes::xmPACK_NOTHING;
		//回复代码
		unsigned short   m_eRspCode = 0;
		//是向服务器，客户端还是双向的
		unsigned char    m_ePackTarget = xmEEndpointType::xmEET_SERVER;
		//时间戳
		long long        m_lTimeStamp = 0;
		//是否需要使用ID，为了单点间的会话，不是多播。
		//主要用于控制数据是否只有发起者处理：1表示只有发起者处理，0表示给所有终端处理
		unsigned char    m_bUseFlowSeq = 0;
		//用于在需要控制数据流向，或者需要控制等待时间的时候使用
		unsigned int     m_uFlowSeq = 0;
		//使用那个会话来处理
		unsigned short   m_uSessionID;      // = xmESessionType::xmEST_LOGIN;
		//是向外发送，还是向内消息处理
		unsigned char    m_sendDir = xmESendDirection::xmESD_OUT;
		//数据包长度，含报文头的长度
		unsigned int     m_uLength = 0;
		//数据类型,xmEBT_DATASET,xmEBT_DATA,xmEBT_MULTIDATA,xmEBT_VALUE,xmEBT_SESSION,xmEBT_BULLETIN,xmEBT_LOG,xmEBT_MAX
		unsigned char    m_btType = 0;
		//来源
		uAddr            m_srcAddr;
		//目的
		uAddr            m_disAddr;
		//是那个版本的消息，以用于区别不同软件版本
		unsigned short   m_nVersion = 0;
	};
#pragma pack ()  

	class xmPacket
	{
	public:
		xmPacket() {};
		xmPacket(const xmPacket& pkt) 
		{
			m_pktHead = pkt.m_pktHead;
			m_dynBuf = pkt.m_dynBuf;
		};

		virtual ~xmPacket()
		{
			Destroy();
		};

		virtual void  Destroy()
		{
			m_dynBuf.Clear();
		};

		//往数据包写入数据
		virtual void  WriteData(unsigned char* in, unsigned int nSize) {};
		//从将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out) = 0;
		//从数据报文中解析数据
		virtual void  Decode(unsigned char* in, unsigned int nSize)= 0;

	public:
		xmHead           m_pktHead;
		xmByteBuffer     m_dynBuf;
	};

};

#endif