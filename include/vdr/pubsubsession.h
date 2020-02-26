#pragma once

#ifndef __SSA_VDR_PUBSUBSESSION_H
#define __SSA_VDR_PUBSUBSESSION_H

#include "session.h"
#include "userpackets.h"


namespace ssa
{
	class xmPubSubSession : public xmSession
	{
	public:
		xmPubSubSession(int id);
		virtual ~xmPubSubSession();

		typedef xmObjectRegistry<xmPubSubSession::OpcodeHandler<xmPubSubSession>, unsigned short>  OpcodeHandlerRegistry;


		virtual void        HandlePacket(const xmPacket& pkt);
		virtual void        FillOpcodeHandlerHashTable();
		xmESessionType      GetSessionType() const { return xmESessionType::xmEST_PUBSUB; };

	private:
		virtual bool		HandleSubDataAsk(const xmPacket& pkt);
		virtual bool		HandleSubDataRsp(const xmPacket& pkt);
	private:
		OpcodeHandlerRegistry m_OpcodeHandlerFactory;
	};
};

#endif
