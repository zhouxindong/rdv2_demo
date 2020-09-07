#pragma once
#ifndef __SSA_VDR_USERMSGHANDLERS_H
#define __SSA_VDR_USERMSGHANDLERS_H

#include "../base/msghandler.h"
#include "datacenter.h"
#include "objectregistry.h"
#include "sensmsgs.h"
#include "memorypage.h"
#include "isensmsgproxy.h"
#include "packet.h"

namespace ssa
{
	class xmDataHandlers
	{
	public:
		xmDataHandlers();
		std::string       m_dtName;
		xmMsgHandler*     m_pHandle[xmEDataEvent::xmEDE_MAX];
	};

	class xmDataSetHandlers
	{
	public:
		xmDataSetHandlers();
		std::string       m_dsName;
		xmMsgHandler*     m_pHandle[xmEDataSetEvent::xmEDSE_MAX];
	};

	class xmNodeHandlers
	{
	public:
		xmNodeHandlers();
		std::string       m_ndName;
		xmMsgHandler*     m_pHandle[xmENodeEvent::xmENE_MAX];
	};

	class xmBulletinHandlers
	{
	public:
		xmBulletinHandlers();
		std::string       m_strSender;
		std::string       m_strTopic;
		xmMsgHandler*     m_pHandle[xmEBulletinEvent::xmEBE_MAX];
	};

	class xmMCast2NodesHandlers
	{
	public:
		xmMCast2NodesHandlers();
		std::string       m_strSender;
		xmMsgHandler*     m_pHandle[xmEMCastMSGEvent::xmEMCE_MAX];
	};

	class xmLogHandlers
	{
	public:
		xmLogHandlers();
		xmMsgHandler*     m_pHandle;
	};

	class xmSensMSGHandler
	{
	public:
		xmSensMSGHandler();
		typedef xmObjectRegistry<xmNodeHandlers, std::string>        NodeHandlersFactory;
		typedef xmObjectRegistry<xmDataSetHandlers, std::string>     DataSetHandlersFactory;
		typedef xmObjectRegistry<xmDataHandlers, std::string>        DataHandlersFactory;
		typedef xmObjectRegistry<xmMsgHandler, unsigned short>       MsgHandlersFactory;
		typedef xmObjectRegistry<xmBulletinHandlers, std::string>    BulletinHandlersFactory;

		typedef xmObjectRegistry<DataHandlersFactory, std::string>   DataHandlersGroupByDataSetFactory;

		NodeHandlersFactory      m_NodeHandlersFactory;
		DataSetHandlersFactory   m_DataSetHandlersFactory;
		
		DataHandlersFactory      m_DataHandlersFactory;
		DataHandlersGroupByDataSetFactory m_DataHandlersGroupByDataSetFactory;

		MsgHandlersFactory       m_MsgHandlersFactory;
		BulletinHandlersFactory  m_BulletinHandlersFactory;
		//为了提高效率，为LOG单独使用一条线程处理。
		xmLogHandlers            m_LogHandlers;
		xmMCast2NodesHandlers    m_MCast2NodesHandlers;

		void SensTickCallBack(const xmPacket& pkt/*int nTickTime,int nSyncCycles, long long lAbsTime*/);
		void SensValueSyncTickCallBack(const xmPacket& pkt/*int nTickTime,int nSyncCycles, long long lAbsTime*/);
		//按照数据集合的范围进行回调，因为值是按照数据集合进行同步的,nEventCode表明此数据中含有用户需要的敏感事件信息
		void SensValSyncMsgCallBack(const std::string& strDataSetName, const xmByteBuffer& bbValue, const xmTimeStamp& ets,int nEventCode,unsigned char cQos);
		//通过返回值判断发生了什么事情，一个bit标识一种事件，bit0标识是否有更新，bit1标识是否有复位事件。
		int  SensDSValCallBack(const std::string& strDataSetName, const xmByteBuffer& bbValue, const xmTimeStamp& ets);
		void SensDataValCallBack(const std::string& strDataName, const xmByteBuffer& bbValue, const xmTimeStamp& ets);
		//当数据信息有变化时，回调
		void SensDataCallBack(const std::string& strName, xmEDataEvent ee, const xmTimeStamp& ets);
		void SensDataSetCallBack(const std::string& strName, xmEDataSetEvent ee, const xmTimeStamp& ets);
		void SensNodeCallBack(const std::string& strName, xmENodeEvent ee, const xmTimeStamp& ets);
		void SensMCastMSGCallBack(const std::string& strSender, const xmByteBuffer& dbContent, xmEMCastMSGEvent ee, const xmTimeStamp& ets);
		void SensLogCallBack(const std::string& strLog, const xmTimeStamp& ets);
		void SensSwitchSystemStateCallBack(xmESystemState es, const xmTimeStamp& ets);

		void Clear();

		void SetDC(xmDataCenter* pDataCenter);
		void SetSensMsgProxy(xmISensMsgProxy* pSensMsgProxy);

	private:
		xmDataCenter*            m_pDataCenter;
		xmISensMsgProxy*         m_pSensMsgProxy;
	};
}

#endif //__SSA_VDR_USERMSGHANDLERS_H