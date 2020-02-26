#ifndef __SSA_VDR_SERVICE_H
#define __SSA_VDR_SERVICE_H

#include "datacenter.h"
#include "sessioncenter.h"
#include "sensmsghandler.h"
#include "logiccontroller.h"
#include "iservice.h"
#include "isyncproxy.h"
#include "defs.h"
#include <shared_mutex>

namespace ssa
{
	class xmService : public xmIService
	{
	public:
		xmService();

		void               SetCommProxy(xmICommProxy* pCommProxy) { m_pCommProxy = pCommProxy; };
		xmICommProxy*      GetCommProxy() { return m_pCommProxy; };
		xmVDRAttr*         GetVDRAttr();
		//�����Ƿ��������ͨ������
		bool               IsCommOK();
		//�ж��Ƿ��ʼ�����
		bool               IsValid();
		bool               IsValid(std::string strDataSetName);
		//ֹͣ����
		void               StopSessions();
		//�رշ���
		void               CloseSessions(bool isDel = false);
		//�ر�������Դ
		void               Close();

		//Ѱ�����������µ�VDRID��
		int                AskNewestID();

		int                AddNode(const xmNode& nd);
		int                RemoveNode(const std::string& strNodeName);
		int                AddDataSet(const xmDataSet& ds);
		int                RemoveDataSet(const std::string& strDataSetName);
		//��Node�����ݼ���������
		int                Link(const std::string& strNodeName, const std::string& strDataSetName);
		//��Node�����ݼ��Ͽ�
		int                UnLink(const std::string& strNodeName, const std::string& strDataSetName);
		int                AddData(const xmData& data);
		int                RemoveData(const std::string& strDataName);

		//�ӷ�������ȡ�ж������ݼ�
		int                FetchDataSetInfo();

		//�������ݼ�����ͬ������
		int                FetchDataInfo(const std::string& strDataSetName = "");
		//ͬ�����ݽṹ��Ϣ
		int                FetchStructInfo();

		//�����Ҫ���������½��ڴ��ʱ��ʹuIsLocal=0�����ֻ�ǽ��������ڴ�ʹ��uIsLocal=1��
		int                BuildMemory(const std::string& strDataSetName, unsigned char uIsLocal = 1);
		//����ʱ���źţ��������Ϊͬ�����ڣ����ֱ�ӷ��͸������ͻ���isUseServerLoopΪfalse�������Ҫͨ������������isUseServerLoopΪtrue��
		int                TickSignal(int nTickTime, int nSyncCycles, bool isUseServerLoop = false);

		//��ϵͳ״̬���й���
		int                SwitchSystemState(xmESystemState es);
		//�ڲ������ڴ�֮��ͬ��
		bool               SyncMainMemory2Mirror(const char* strDataSetName);

		//�����Ƿ�������ⷢ������
		void               SetPublishAbility(const std::string& strName, bool havePublishAbility);
		//�����Ƿ��Զ����ⷢ������
		void               SetAutoPublishSign(const std::string& strName, bool autoPublishSign);
		//�ֶ�ͬ�������źţ����������һ������Ҫ�ϴ��µ����ݣ�ͬʱ��ȡ�µ��ڴ����ݣ�һ�����������ȡ�������˵����ݡ�
		int                SyncContent(const std::string& strDataSetName, xmESyncDirection am, bool transMsgUsePostMode, bool bUseRTE);
		int                SendEmptyPkt(const std::string& strDataSetName, xmESyncDirection am, bool transMsgUsePostMode);
		int                Paste2Bulletin(const char* strSender, const char* strTopic, const char* pBuf, unsigned int uLength);
		int                MCast2Nodes(const char* strFromNode, const std::vector<std::string>& vToNodeName, const char* pBuf, unsigned int uLength);
		//�����ݰ��ַ�����
		int                Dispatch(const char* pBuf, int nLen);
		//ֻ�ڸտ�ʼ�����Ựʱʹ�á�
		int                DispatchWithWaitTime(const xmPacket& pkt, xmByteBuffer& wParam, bool transMsgUsePostMode, int waitTime);
		int                DispatchWithoutWaitTime(const xmPacket& pkt, bool transMsgUsePostMode);

		void               CreateSessionGroup(std::string strName);
		void               CreateGlobalSingleSessions();
		xmSessionCenter*   GetSessionCenter() { return &m_SessionCenter; };
		xmSensMSGHandler*  GetSensMSGHandler() { return &m_SensMSGHandler; };
		xmDataCenter*      GetDataCenter() { return &m_DataCenter; };

		bool               IsAttached(const std::string& strDataSetName);
		void               AddAttached(const xmDataSet& ds);
		void               RemoveAttached(const std::string& strDataSetName);
		void               RenewAttachedInfo(const xmDataSet& ds);
		void               ClearAttached();
		std::map<std::string, xmDataSet>&      GetAttchedDataSet();


		bool               IsAuthorized(const std::string& ndName);
		void               AddAuthorized(const xmNode& nd);
		void               RemoveAuthorized(const std::string& ndName);
		void               ClearAuthorized();
		std::map<std::string, xmNode>&         GetAuthorizedNode();

		void               SetWaitTime(long nTime = xmMAX_WAITING_TIME);
		long               GetWaitTime();

		xmLogicController* GetLogicController() { return &m_LogicController; };
		void               SetSyncProxy(xmISyncProxy* pSyncProxy);
		xmISyncProxy*      GetSyncProxy() { return m_pSyncProxy; };

		void               BeginSync();
		void               DoSync();
		void               EndSync();

		long long          GetTimeStamp() {return m_lTimeStamp;};

	public:
		typedef enum __tagServiceState
		{
			xmESS_NOTHING = 0,
			xmESS_STARTING,		//	
			xmESS_MAX,
		} xmEServiceState;
		//�����жϷ��������Ƿ������µ�ID����
		bool               m_isGetNewestID;
		//ϵͳ��״̬
		xmESystemState     m_eSystemState;

		long long          m_lTimeStamp;

	private:
		void               CreateSingleSession(int nSessionID);
		xmSession *        GetSession(int nSessionID, std::string strName, bool bUseMultiSessions);
		int                Trans2MSGQ(const xmPacket& pkt, bool transMsgUsePostMode);

	private:
		//���ڱ�֤ds��ʼ�����̵�״̬�����ʹ��״̬����
		xmEServiceState    m_State;
		//�����ǰ��������ָ�����Ǹ����ݼ��Ͻ��в���
		std::map<std::string, xmDataSet>      m_mapAttchedDataSet;
		std::map<std::string, xmNode>         m_mapAuthorizedNode;
		//�ж��ڱ�ͨ�Žڵ��VDR��,���ݼ��Ƿ�������ⷢ���Լ���ֵ
		std::map<std::string, bool>           m_mapHavePublishValueAbility;
		//�����ж����ݼ���ʱ�������������ⲿ���ֶ�����
		std::map<std::string, bool>           m_mapAutoPublishSign;

		xmDataCenter       m_DataCenter;
		xmSessionCenter    m_SessionCenter;
		xmLogicController  m_LogicController;
		xmSensMSGHandler   m_SensMSGHandler;

		typedef std::map<int, xmSession*>  SessionRegistry;
		SessionRegistry    m_SessionRegistry;
		xmICommProxy*      m_pCommProxy;
		xmISyncProxy*      m_pSyncProxy;

		std::shared_mutex  m_LocalMutex;
		std::shared_mutex  m_GlobalMutex;
		std::shared_mutex  m_LogMutex;
		std::shared_mutex  m_SyncMutex;

		long               m_nWaitTime;
	};
}

#endif