#pragma once
#ifndef __SSA_VDR_TICKSESSION_H
#define __SSA_VDR_TICKSESSION_H
#include "session.h"
#include "userpackets.h"

namespace ssa
{
	class xmTickSession : public xmSession
	{

	public:
		xmTickSession(int id);
		virtual ~xmTickSession();

		typedef xmObjectRegistry<xmTickSession::OpcodeHandler<xmTickSession>, unsigned short>  OpcodeHandlerRegistry;

		virtual void      HandlePacket(const xmPacket& pkt);
		virtual void      FillOpcodeHandlerHashTable();
		xmESessionType    GetSessionType() const { return xmESessionType::xmEST_TICK; };

	private:
		virtual bool      HandleTickAsk(const xmPacket& pkt);
		virtual bool      HandleTickRsp(const xmPacket& pkt);

	private:
		OpcodeHandlerRegistry m_OpcodeHandlerFactory;
		int               m_nSyncCycles;

	};
}

#endif // !1
