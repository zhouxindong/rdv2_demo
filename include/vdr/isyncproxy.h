#pragma once
#ifndef __SSA_VDR_ISYNCPROXY_H
#define __SSA_VDR_ISYNCPROXY_H
#include "export.h"
#include "iproxy.h"
#include "sensmsgs.h"
#include "packet.h"
#include "iservice.h"

namespace ssa
{
	class xmVDR_EXPORT xmISyncProxy : public xmIProxy
	{
	public:
		xmISyncProxy();
		virtual ~xmISyncProxy() {};

		void SetService(xmIService* pService);

		virtual int  GetProxyID() = 0;

		virtual int InCommingTicketPacket(const xmPacket& pkt);
		virtual int InCommingValPacket(const xmPacket& pkt);

	protected:
		xmIService* m_pService;
	};
}

#endif //__SSA_VDR_ISYNCPROXY_H
