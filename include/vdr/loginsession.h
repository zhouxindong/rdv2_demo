#ifndef __SSA_VDR_LOGINSESSION_H
#define __SSA_VDR_LOGINSESSION_H

#pragma once

#include "session.h"
#include "userpackets.h"
#include "innerobjs.h"

namespace ssa
{
	class xmLoginSession : public xmSession
	{
	public:
		xmLoginSession(int id);
		virtual ~xmLoginSession();

		typedef xmObjectRegistry<xmLoginSession::OpcodeHandler<xmLoginSession>, unsigned short>  OpcodeHandlerRegistry;


		virtual void        HandlePacket(const xmPacket& pkt);
		virtual void        FillOpcodeHandlerHashTable();
		xmESessionType      GetSessionType() const { return xmESessionType::xmEST_LOGIN; };

	private:
		virtual bool		HandleNewVDRAsk(const xmPacket& pkt);
		virtual bool		HandleNewVDRsp(const xmPacket& pkt);

		virtual bool		HandleAddNodeAsk(const xmPacket& pkt);
		virtual bool		HandleAddNodeRsp(const xmPacket& pkt);

		virtual bool		HandleRemoveNodeAsk(const xmPacket& pkt);
		virtual bool		HandleRemoveNodeRsp(const xmPacket& pkt);

		virtual bool		HandleLinkNode2DataSetAsk(const xmPacket& pkt);
		virtual bool		HandleLinkNode2DataSetRsp(const xmPacket& pkt);

		virtual bool		HandleUnLinkNode2DataSetAsk(const xmPacket& pkt);
		virtual bool		HandleUnLinkNode2DataSetRsp(const xmPacket& pkt);

		virtual bool		HandleStructSyncAsk(const xmPacket& pkt);
		virtual bool		HandleStructSyncRsp(const xmPacket& pkt);

		virtual bool		HandleFectchDataSetAsk(const xmPacket& pkt);
		virtual bool		HandleFectchDataSetRsp(const xmPacket& pkt);
		virtual bool		HandleAddDataSetAsk(const xmPacket& pkt);
		virtual bool		HandleAddDataSetRsp(const xmPacket& pkt);

		virtual bool		HandleAddDataAsk(const xmPacket& pkt);
		virtual bool		HandleAddDataRsp(const xmPacket& pkt);

		virtual bool		HandleSyncDataAsk(const xmPacket& pkt);
		//用于在数据传输前判断传输是否有效的，通常是告诉客户端有新的数据要来了，做好准备。
		virtual bool		HandleSyncDataReady(const xmPacket& pkt);
		virtual bool		HandleSyncDataRsp(const xmPacket& pkt);

		virtual bool		HandleTransDataSet(const xmPacket& pkt);
		virtual bool		HandleTransData(const xmPacket& pkt);
		virtual bool		HandleTransMultiData(const xmPacket& pkt);

		virtual bool		HandleBuildIndexAsk(const xmPacket& pkt);
		virtual bool		HandleBuildIndexRsp(const xmPacket& pkt);

		virtual bool		HandleBuildMemoryAsk(const xmPacket& pkt);
		virtual bool		HandleBuildMemoryRsp(const xmPacket& pkt);

		virtual bool		HandleRemoveDataSetAsk(const xmPacket& pkt);
		virtual bool		HandleRemoveDataSetRsp(const xmPacket& pkt);

		virtual bool		HandleRemoveDataAsk(const xmPacket& pkt);
		virtual bool		HandleRemoveDataRsp(const xmPacket& pkt);

		virtual bool		HandleSwitchSystemStateAsk(const xmPacket& pkt);
		virtual bool		HandleSwitchSystemStateRsp(const xmPacket& pkt);

		virtual bool		HandleCommIsOKAsk(const xmPacket& pkt);
		virtual bool		HandleCommIsOKRsp(const xmPacket& pkt);

		virtual bool		HandleServerStopRsp(const xmPacket& pkt);
		//仅仅是为了代码组织上的简洁而单独作为一个函数
		void                SendInfo(xmSyncDataPacket* lpkt, xmDataSetWithDetail* pds);
	private:
		OpcodeHandlerRegistry m_OpcodeHandlerFactory;
	};
};

#endif