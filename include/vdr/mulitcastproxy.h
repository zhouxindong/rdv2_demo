#ifndef __SSA_VDR_MULTICASTPROXY_H
#define __SSA_VDR_MULTICASTPROXY_H

#include "icommproxy.h"
#include "service.h"
#include "../base/thread.h"
#include "../base/socket.h"
#include "vdrattr.h"
#include "commproxyfactory.h"

namespace ssa
{
	class xmVDR_EXPORT xmMulitcastProxy : public xmICommProxy,public xmThread
	{
	public:
		xmMulitcastProxy():m_pService(0){};
		virtual ~xmMulitcastProxy(){};

		bool  Initialize(xmVDRAttr*);
		bool  Close();
		void  SetService(void* pService = 0);
		void* GetService();

		int   Run(void* lpVoid);
		int   Start();
		bool  Send(const xmPacket& pkt);
		int   Recv(char* buf, int nLen);

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
		xmService*       m_pService;
		xmMCastVDRAttr   m_VDRAttr;
		xmIPAddress      m_GroupAddr;
		//xmMutex          m_Mutex;
	};
}

#endif //