#ifndef __SSA_VDR_MULTICASTPROXY_H
#define __SSA_VDR_MULTICASTPROXY_H

#include "icommproxy.h"
#include "service.h"
#include "../base/thread.h"
#include "../base/socket.h"
#include "vdrattr.h"
#include "proxyfactory.h"

//#ifdef xmOS_WINDOWS
//#pragma comment(lib, "wsock32")
//#endif

namespace ssa
{
	class xmMulticastProxy : public xmICommProxy,public xmThread
	{
	public:
		xmMulticastProxy(){};
		virtual ~xmMulticastProxy(){};

		bool  Initialize(void*);
		bool  Close();

		int   Run(void* lpVoid);
		int   Start();
		bool  Send(const xmPacket& pkt);

		xmVDRAttr*          GetVDRAttr();
		xmEVDRCommProxyType GetCommProxyType() { return xmEVDRCommProxyType::xmECM_LAN; };

	public:
		//负责接收多播信息
		xmUDPSocket*     m_pMultiUdpSocket;
		//主要负责发送
		xmUDPSocket*     m_pUdpSocket;
		xmEvent          m_evtSign;
		xmEvent          m_evtQiutSign;

	private:
		xmMCastVDRAttr   m_VDRAttr;
		xmIPAddress      m_GroupAddr;
		//xmService*       m_pService;
	};
}

#endif //