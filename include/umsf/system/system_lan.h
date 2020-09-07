#ifndef __UMSF_SYSTEM_LAN_H
#define __UMSF_SYSTEM_LAN_H

#include "../isystem.h"
#include "../../../extlibs/jsoncpp/include/json/json.h"
#include "vdr/vdr.h"
#include "vdr/accesspoint.h"
using namespace ssa;

class SystemLan : public xmMsgHandler, public xmISystem
{
public:
	SystemLan();
	virtual ~SystemLan();
	xmRet Create(const char* pPropertyJson);


public:
	virtual xmString Version()
	{
		return "VDR V5.0";
	};
	virtual xmRet Login(const xmString& strNodeID, const xmString& strAttribute = NULL);
	virtual xmRet Logout();
	virtual bool  IsLogined(void);

	virtual xmRet RegisterMsgHandler(xmMsgHandler* pHandler);
	virtual xmRet RegisterData(const xmString& strDataName, const xmValue& aValue, const xmString& strAttribute = NULL);
	virtual xmRet RegisterSensDate(const xmString& strDataPath);

	virtual void  ReadyToGo();

	virtual void  SetClearFlag(bool bClear) { m_bClear = bClear; };
	virtual bool  GetClearFlag() const { return m_bClear; };

	virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO);
	virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO);

	virtual unsigned int GetSimulateState(void) const
	{
		return m_uSimuState;
	};
	virtual int GetSimulateSpeed(void) const
	{
		return m_nSimuSpeed;
	};
	virtual unsigned int GetClickCycle(void) const
	{
		return m_uClickCycle;
	};
	virtual size_t GetClickCount(void) const
	{
		return m_uClickCount;
	};
	virtual time_t GetSystemTime(void) const 
	{ 
		return m_uSystemTime; 
	};

	virtual xmRet SyncDate(const xmString& strDataName = xmString(), int nDirction = (IOT_INPUT | IOT_OUTPUT));

	virtual xmString PrintData(const xmString& strDataName = NULL);

private:
	//	时钟节拍（10ms）
	void OnMSGTimerClick(const xmMSGTimerClick& aMsg);
	//	数据同步（40ms）
	void OnMSGValueSyncTimerClick(const xmMSGValueSyncTimerClick& aMsg);
	//	内存块同步（不定）
	void OnMSGMemorySync(const xmMSGMemorySync& aMsg);

	//	系统状态切换
	void OnMSGSwitchState(const xmMSGSwitchState& aMsg);
	//	节点事件
	void OnMSGNodeEvent(const xmMSGNodeEvent& aMsg);
	//	数据集事件
	void OnMSGDataSetEvent(const xmMSGDataSetEvent& aMsg);

	//	数据事件
	void OnMSGDataEvent(const xmMSGDataEvent& aMsg);
	//	公告事件
	void OnMSGBulletin(const xmMSGBulletin& aMsg);
	//	消息事件
	void OnMSGMessage(const xmMSGMCast& aMsg);

	//	日志
	void OnMSGLog(const xmMSGLog& aMsg);
	xmCREATE_MESSAGE_MAP(SystemLan);

private:
	xmVDR m_VDR;
	xmAccessPoint m_AP;
	std::vector<xmMsgHandler*> m_vHandler;
	//	VDR属性
	xmMCastVDRAttr m_LanAttr;
	//	登录时最长等待时间
	long m_nWaitTIme;
	//	节点属性
	xmNode m_Node;
	//	数据集，key为数据集的配置名称
	std::map<std::string, xmPtr<xmDataSet>> m_mapDataSet;
	//	输入为数据集配置名称，在函数中将转换为数据集VDR名称
	xmDataSet* __AddDataSet(const std::string& strName);

	bool m_bClear;

	//	节拍计数器
	unsigned int m_uClickCycle;
	size_t m_uClickCount;
	//	系统时间
	time_t m_uSystemTime;

	//	记录系统状态和仿真倍速
	unsigned int m_uSimuState;
	int m_nSimuSpeed;
	//	系统中，表示这两个参数的数据名称
	xmString m_strSimuState;
	xmString m_strSimuSpeed;


	int m_nLEC_System;


	//	指令与事件映射表
	typedef struct _tagMsgObject
	{
		unsigned int m_uType;
		xmString m_strCommand;
		xmString m_strName;
		xmString m_strValue;	//	字符串描述的命令数值
		xmString m_strParam;
		xmValue  m_ctrlValue;	//	xmValue描述的命令数值，由于创建时，不知道数据类型，所以需要在运行过程中初始化。
	}MsgObject;
	//	训练开始指令
	xmString m_strStartMsgDataName;
	xmPtr<MsgObject> m_pStartMsgObject;
	std::multimap<xmString, xmPtr<MsgObject> > m_mapMsgObject;
	bool __IsCmdArrived(const xmValue& sensValue, MsgObject* pObject);

	xmRet __LoadVdr(const Json::Value& jvObject);
	xmRet __LoadCommand(const Json::Value& jvObject);
	xmRet __LoadState(const Json::Value& jvObject);
	MsgObject* __LoadObject(const Json::Value& jvObject);
	std::map<xmString, bool> m_mapUserSensitive;	//	标记一个仿真系统敏感的命令数据，用户是否也注册了敏感

	void __UpdateSystemState();
};
#endif  //  __UMSF_SYSTEM_LAN_H

