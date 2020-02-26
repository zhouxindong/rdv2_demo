#pragma once
#ifndef __SSA_VDR_USERMSGHANDLERS_H
#define __SSA_VDR_USERMSGHANDLERS_H

#include "../base/msghandler.h"
#include "datacenter.h"
#include "objectregistry.h"
#include "sensmsgs.h"
#include "memorypage.h"
#include "isensmsgproxy.h"

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
		//Ϊ�����Ч�ʣ�ΪLOG����ʹ��һ���̴߳���
		xmLogHandlers            m_LogHandlers;
		xmMCast2NodesHandlers    m_MCast2NodesHandlers;

		void SensTickCallBack(int nTickTime,int nSyncCycles, long long lAbsTime);
		void SensValueSyncTickCallBack(int nTickTime,int nSyncCycles, long long lAbsTime);
		//�������ݼ��ϵķ�Χ���лص�����Ϊֵ�ǰ������ݼ��Ͻ���ͬ���ġ�
		void SensValSyncMsgCallBack(const std::string& strDataSetName, xmByteBuffer bbValue, long long  lAbsTime);
		void SensValCallBack(const std::string& strDataSetName, xmByteBuffer bbValue, long long  lAbsTime);
		//��������Ϣ�б仯ʱ���ص�
		void SensDataCallBack(const std::string& strName, xmEDataEvent ee);
		void SensDataSetCallBack(const std::string& strName, xmEDataSetEvent ee);
		void SensNodeCallBack(const std::string& strName, xmENodeEvent ee);
		void SensMCastMSGCallBack(const std::string& strSender, const xmByteBuffer& dbContent, xmEMCastMSGEvent ee);
		void SensLogCallBack(const std::string& strLog);
		void SensSwitchSystemStateCallBack(xmESystemState es);

		void Clear();

		void SetDC(xmDataCenter* pDataCenter);
		void SetSensMsgProxy(xmISensMsgProxy* pSensMsgProxy);

	private:
		xmDataCenter*            m_pDataCenter;
		xmISensMsgProxy*         m_pSensMsgProxy;
	};
}

#endif //__SSA_VDR_USERMSGHANDLERS_H