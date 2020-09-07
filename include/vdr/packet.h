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
		sockaddr_in Sock;                 //使用网络地址标识
		int         No;//= 0;             //使用序列号标识
		char        Name[16]/* = { 0 }*/; //使用名字标识
	};

	struct xmHead
	{
	public:
		//VDR序列号
		unsigned short   m_uVDRID;//  = 0x6B42;
		//作用域
		unsigned short   m_uRegion;// = 0;
		//数据包类型
		unsigned short   m_uPackCode;// = xmEPackCodes::xmPACK_NOTHING;
		//回复代码
		unsigned short   m_eRspCode;// = 0;
		//是向服务器，客户端还是双向的
		unsigned char    m_ePackTarget;// = xmEEndpointType::xmEET_SERVER;
		//时间戳
		long long        m_lTimeStamp;// = 0;
		//是否需要使用ID，为了单点间的会话，不是多播
		//主要用于控制数据是否只有发起者处理：1表示只有发起者处理，0表示给所有终端处理
		unsigned char    m_bUseFlowSeq;// = 0;
		//用于在需要控制数据流向，或者需要控制等待时间的时候使用
		unsigned int     m_uFlowSeq;// = 0;
		//使用那个会话来处理
		unsigned short   m_uSessionID;      // = xmESessionType::xmEST_LOGIN;
		//是向外发送，还是向内消息处理
		unsigned char    m_uSendDir;// = xmESendDirection::xmESD_OUT;
		//数据包长度，含报文头的长度
		unsigned int     m_uLength;// = 0;
		//报文中数据类型,xmEBT_DATASET,xmEBT_DATA,xmEBT_MULTIDATA,xmEBT_VALUE,xmEBT_SESSION,xmEBT_BULLETIN,xmEBT_LOG,xmEBT_MAX
		unsigned char    m_btType;// = 0;
		//帧计数        
		long long        m_lFrameCount;// = 0;
		//来源,默认低4个字节为各发送源的序列号，高4个字节用于帧计数
		char             m_addrSrc[8];// = { 0 };
		//目的，在login会话中，用于客户端向服务器询问IP地址使用情况
		//使用 m_addrDst[0]表示IP地址是否有效，m_addrDst[2-3]2个字节表示端口号，高4字节表示服务器地址
		char             m_addrDst[8];// = { 0 };
		//保留8个字节
		//使用第0字节表示通过保障Qos的方式，目前包括了三类：0使用默认方法，1使用不可靠，2为可靠
		//默认情况报文是不可靠的速度快，数据类通常使用不可靠的方式，指令类的使用可靠方式
		//具体如何处理，由通信代理决定
		char             m_cRerv[8];// = { 0 };
		//是那个版本的消息，以用于区别不同软件版本
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

		//往数据包写入数据
		virtual void  WriteData(unsigned char* in, unsigned int nSize) {};
		//从将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out) = 0;
		//从数据报文中解析数据
		virtual void  Decode(unsigned char* in, unsigned int nSize)= 0;

	public:
		xmHead           m_pktHead;
		xmByteBuffer     m_dynBuf;
		int              m_nRepet;
	};

};

#endif