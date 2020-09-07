#ifndef __SSA_VDR_MULTICASTPROXY_H
#define __SSA_VDR_MULTICASTPROXY_H

#include "icommproxy.h"
#include "service.h"
#include "../base/thread.h"
#include "../base/socket.h"
#include "vdrattr.h"
#include "proxyfactory.h"

namespace ssa
{
	class xmMultiCastSocket : public xmUDPSocket
	{
	public:
		int GetSocket()
		{
			return this->m_Socket;
		}
	};

	class xmMulticastProxy : public xmICommProxy,public xmThread
	{
	public:
		xmMulticastProxy();
		virtual ~xmMulticastProxy();

		bool  Initialize(void*);
		bool  Close();

		int   Run(void* lpVoid);
		int   Start();
		bool  Send(const xmPacket& pkt);

		xmVDRAttr*          GetVDRAttr();
		xmEVDRCommProxyType GetCommProxyType() { return xmEVDRCommProxyType::xmECM_LAN; };

	public:
		//负责接收多播信息
		xmMultiCastSocket*  m_pMultiUdpSocket;
		//主要负责发送
		xmMultiCastSocket*  m_pSendSocket;

		SOCKET              m_socketSend;
		SOCKADDR_IN         m_socketAddr;
		xmEvent             m_evtSign;
		xmEvent             m_evtQiutSign;
		//如果为0,使用非阻塞模式,为1使用阻塞模式，目前默认使用1
		int                 m_nMode;

	private:
		xmMCastVDRAttr      m_VDRAttr;
		xmIPAddress         m_GroupAddr;

		//用于记录每个发送数据集的发送帧数
		unsigned int        m_nFrameCountSend;
		std::unordered_map<unsigned int, unsigned int>  m_mapFrameCountRecv;
	};
}

#endif //