#pragma once
#ifndef __SSA_VDR_BULLETINSESSION_H
#define __SSA_VDR_BULLETINSESSION_H
#include "session.h"
#include "userpackets.h"

namespace ssa
{
	class xmBulletinSession : public xmSession
	{

	public:
		xmBulletinSession(int id);
		virtual ~xmBulletinSession();

		typedef xmObjectRegistry<xmBulletinSession::OpcodeHandler<xmBulletinSession>, unsigned short>  OpcodeHandlerRegistry;

		virtual void      HandlePacket(const xmPacket& pkt);
		virtual void      FillOpcodeHandlerHashTable();
		xmESessionType    GetSessionType() const { return xmESessionType::xmEST_BULLETIN; };

	private:
		virtual bool      HandleBulletinAsk(const xmPacket& pkt);
		virtual bool      HandleBulletinRsp(const xmPacket& pkt);
		virtual bool      HandleMCastMSGAsk(const xmPacket& pkt);
		virtual bool      HandleMCastMSGRsp(const xmPacket& pkt);

	private:
		OpcodeHandlerRegistry m_OpcodeHandlerFactory;

	};
}

#endif // !__SSA_VDR_BULLETINSESSION_H
