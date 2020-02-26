#pragma once
#ifndef __SSA_VDR_ISENSMSGPROXY_H
#define __SSA_VDR_ISENSMSGPROXY_H
#include "export.h"
#include "iproxy.h"
#include "sensmsgs.h"

namespace ssa
{
	class xmVDR_EXPORT xmISensMsgProxy : public xmIProxy
	{
	public:
		xmISensMsgProxy(){};
		virtual ~xmISensMsgProxy() {};
		virtual int  GetProxyID() { return xmEPUID_LOGIC_BEGAIN; };

		virtual void SensTick(const xmMSGTimerClick& sensMsg, xmMsgHandler* p);
		virtual void SensValueSyncTick(const xmMSGValueSyncTimerClick& sensMsg, xmMsgHandler* p);
		virtual void SensVal(const xmMSGMemorySync& sensMsg, xmMsgHandler* p);
		virtual void SensData(const xmMSGDataEvent& sensMsg, xmMsgHandler* p);
		virtual void SensDataSet(const xmMSGDataSetEvent& sensMsg, xmMsgHandler* p);
		virtual void SensNode(const xmMSGNodeEvent& sensMsg, xmMsgHandler* p);
		virtual void SensMCastMSG(const xmMSGMCast& sensMsg, xmMsgHandler* p);
		virtual void SensSwitchSystemState(const xmMSGSwitchState& sensMsg, xmMsgHandler* p);
	};
}

#endif //__SSA_VDR_ISENSMSGPROXY_H