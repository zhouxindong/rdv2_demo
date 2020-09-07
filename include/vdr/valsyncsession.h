#ifndef __SSA_VDR_VALUESYNCSESSION_H
#define __SSA_VDR_VALUESYNCSESSION_H

#pragma once
#include "session.h"
#include "userpackets.h"

namespace ssa
{
	class xmValSyncSession : public xmSession
	{

	public:
		xmValSyncSession(int id);
		virtual ~xmValSyncSession();

		typedef xmObjectRegistry<xmValSyncSession::OpcodeHandler<xmValSyncSession>, unsigned short>  OpcodeHandlerRegistry;

		virtual void      HandlePacket(const xmPacket& pkt);
		virtual void      FillOpcodeHandlerHashTable();
		xmESessionType    GetSessionType() const { return xmESessionType::xmEST_VALSYNC; };

	private:
		virtual bool      HandleMemorySyncAsk(const xmPacket& pkt);
		//下面函数通常只有RTI进行调用，用于同步数据
		virtual bool      HandleMemorySyncRTEAsk(const xmPacket& pkt);
		virtual bool      HandleMemorySyncRsp(const xmPacket& pkt);

		virtual bool      HandleSystemSynAsk(const xmPacket& pkt);
		virtual bool      HandleSystemSynRsp(const xmPacket& pkt);

		virtual bool      HandleDataValSyncAsk(const xmPacket& pkt);
		virtual bool      HandleDataValSyncRsp(const xmPacket& pkt);

	private:
		OpcodeHandlerRegistry m_OpcodeHandlerFactory;

	};
};

#endif