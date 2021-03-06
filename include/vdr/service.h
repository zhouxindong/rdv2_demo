#ifndef __SSA_VDR_SERVICE_H
#define __SSA_VDR_SERVICE_H

#include "defs.h"
#include "datacenter.h"
#include "sessioncenter.h"
#include "sensmsghandler.h"
#include "logiccontroller.h"
#include "iservice.h"
#include "isyncproxy.h"
#include "authoritymanage.h"
#include <unordered_map>
#include "mmtimer.h"
#if(_MSC_VER >= 1900) //vs2015及以上版本
#include <shared_mutex>
#endif

namespace ssa
{
	class xmService : public xmIService
	{
	public:
		xmService();

		void               SetCommProxy(xmICommProxy* pCommProxy);// { m_pCommProxy = pCommProxy; };
		xmICommProxy*      GetCommProxy() { return m_pCommProxy; };
		xmVDRAttr*         GetVDRAttr();
		//测试是否与服务器通信正常
		bool               IsCommOK();
		//通知所有客户端，服务器停止工作
		bool               ExcCmd(void* cmd);
		//判断是否初始化完毕
		bool               IsValid();
		bool               IsValid(std::string strDataSetName);
		//停止服务
		void               StopSessions();
		//关闭服务
		void               CloseSessions(bool isDel = false);
		//关闭所有资源
		void               Close();

		//寻找区域内最新的VDRID号
		int                AskNewestID();

		int                AddNode(const xmNode& nd);
		int                RemoveNode(const std::string& strNodeName);
		int                AddDataSet(const xmDataSet& ds);
		int                RemoveDataSet(const std::string& strDataSetName);
		//将Node和数据集连接起来
		int                Link(const std::string& strNodeName, const std::string& strDataSetName);
		//将Node和数据集断开
		int                UnLink(const std::string& strNodeName, const std::string& strDataSetName);
		int                AddData(const xmData& data);
		int                RemoveData(const std::string& strDataName);

		//从服务器获取有多少数据集
		int                FetchDataSetInfo();

		//按照数据集名字同步数据
		int                FetchDataInfo(const std::string& strDataSetName = "");
		//同步数据结构信息
		int                FetchStructInfo();

		//如果需要服务器端新建内存的时候使uIsLocal=0，如果只是建立本地内存使用uIsLocal=1；
		int                BuildMemory(const std::string& strDataSetName, unsigned char uIsLocal = 1);
		//发起时间信号，输入参数为同步周期，如果直接发送给各个客户端isUseServerLoop为false，如果需要通过服务器发送isUseServerLoop为true。
		int                TickSignal(int nTickTime, int nSyncCycles, bool isUseServerLoop = false, bool isUseLocalTimer = false);

		//对系统状态进行管理
		int                SwitchSystemState(xmESystemState es);
		//内部两个内存之间同步
		bool               SyncMainMemory2Mirror(const char* strDataSetName);

		//设置是否可以往外发送数据
		void               SetPublishAbility(const std::string& strName, bool havePublishAbility);
		//设置是否自动往外发送数据
		void               SetAutoPublishSign(const std::string& strName, bool autoPublishSign);
		//手动同步触发信号，两种情况，一种是需要上传新的数据，同时获取新的内存数据，一种情况仅仅获取服务器端的数据。
		int                SyncContent(const std::string& strDataSetName, xmESyncDirection syncDir, bool transMsgUsePostMode, bool bUseRTE);
		int                CombContent(xmDataSetWithDetail* p, char* pBuf, int nStartPos, int nLength, bool bUseLast, xmEQos eTansMode, xmESyncDirection syncDir, bool transMsgUsePostMode, bool bUseRTE);
		int                SendEmptyPkt(const std::string& strDataSetName, xmESyncDirection syncDir, bool transMsgUsePostMode);
		
		int                RushValue(const char* strDataName, const char* bValue, unsigned int uLength, const char* flag);
		int                Paste2Bulletin(const char* strSender, const char* strTopic, const char* pBuf, unsigned int uLength);
		int                MCast2Nodes(const char* strFromNode, const std::vector<std::string>& vToNodeName, const char* pBuf, unsigned int uLength);
		//将数据包分发处理
		int                Dispatch(const char* pBuf, int nLen);
		//只在刚开始建立会话时使用。
		int                DispatchWithWaitTime(const xmPacket& pkt, xmByteBuffer& wParam, bool transMsgUsePostMode, int waitTime);
		int                DispatchWithoutWaitTime(const xmPacket& pkt, bool transMsgUsePostMode);

		void               CreateSessionGroup(std::string strName);
		void               CreateGlobalSingleSessions();
		xmSessionCenter*   GetSessionCenter() { return &m_SessionCenter; };
		xmSensMSGHandler*  GetSensMSGHandler() { return &m_SensMSGHandler; };
		xmDataCenter*      GetDataCenter() { return &m_DataCenter; };

		void               SetWaitTime(long nTime = xmMAX_WAITING_TIME);
		long               GetWaitTime();

		xmLogicController* GetLogicController() { return &m_LogicController; };
		void               SetSyncProxy(xmISyncProxy* pSyncProxy);
		xmISyncProxy*      GetSyncProxy() { return m_pSyncProxy; };

		void               BeginSync();
		void               DoSync();
		void               EndSync();

		long long          GetTimeStamp() {return m_lCurrentTimeStamp;};
		long long          GetFrameCount() { return m_lCurrentFrameCount; };
		unsigned int       GetSNCode() { return m_nSNCode; };

		bool               StartLocalTimer(int nTickTime, int nSyncCycles);
		bool               StopLocalTimer();
		bool               BroadcastStop();
	public:
		typedef enum __tagServiceState
		{
			xmESS_NOTHING = 0,
			xmESS_STARTING,		//	
			xmESS_MAX,
		} xmEServiceState;
		//用于判断服务器端是否有最新的ID返回
		bool               m_isGetNewestID;
		//系统的状态
		xmESystemState     m_eSystemState;

		long long          m_lFrameCountTotal;// = 0;
		//当前的时间戳
		long long          m_lCurrentTimeStamp;
		//当前帧号
		long long          m_lCurrentFrameCount;
		//是否使用自己的时钟;
		bool               m_bIsUseLocalTimer;// = false;
		bool               m_bIsLocalTimerStart;// = false;
		MMTimer*           m_pLocalMMTimer;// = 0;
		//用于权限集中管理
		xmAuthorityManage  m_AuthorityManage;

	private:
		void               CreateSingleSession(int nSessionID);
		xmSession *        GetSession(int nSessionID, std::string strName, bool bUseMultiSessions);
		int                Trans2MSGQ(const xmPacket& pkt, bool transMsgUsePostMode);

	private:
		//用于保证ds初始化过程的状态，最好使用状态机。
		xmEServiceState    m_State;

		//判断在本通信节点的VDR中,数据集是否可以往外发送自己的值
		std::unordered_map<std::string, bool>           m_mapHavePublishValueAbility;
		//用于判断数据集是时钟驱动还是由外部的手动驱动
		std::unordered_map<std::string, bool>           m_mapAutoPublishSign;

		xmDataCenter       m_DataCenter;
		xmSessionCenter    m_SessionCenter;
		xmLogicController  m_LogicController;
		xmSensMSGHandler   m_SensMSGHandler;

		typedef std::unordered_map<int, xmSession*>  SessionRegistry;
		SessionRegistry    m_SessionRegistry;
		xmICommProxy*      m_pCommProxy;
		xmISyncProxy*      m_pSyncProxy;

#if(_MSC_VER >= 1900) //vs2015及以上版本
		std::shared_mutex  m_LocalMutex;
		std::shared_mutex  m_GlobalMutex;
		std::shared_mutex  m_LogMutex;
		std::shared_mutex  m_SyncMutex;
#else

		xmMutex            m_LocalMutex;
		xmMutex            m_GlobalMutex;
		xmMutex            m_LogMutex;
		xmMutex            m_SyncMutex;
#endif

		long               m_nWaitTime;
		unsigned int       m_nSNCode;
		xmEEndpointType    m_eEndpointType;
	};
}

#endif