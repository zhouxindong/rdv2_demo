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
	//	ʱ�ӽ��ģ�10ms��
	void OnMSGTimerClick(const xmMSGTimerClick& aMsg);
	//	����ͬ����40ms��
	void OnMSGValueSyncTimerClick(const xmMSGValueSyncTimerClick& aMsg);
	//	�ڴ��ͬ����������
	void OnMSGMemorySync(const xmMSGMemorySync& aMsg);

	//	ϵͳ״̬�л�
	void OnMSGSwitchState(const xmMSGSwitchState& aMsg);
	//	�ڵ��¼�
	void OnMSGNodeEvent(const xmMSGNodeEvent& aMsg);
	//	���ݼ��¼�
	void OnMSGDataSetEvent(const xmMSGDataSetEvent& aMsg);

	//	�����¼�
	void OnMSGDataEvent(const xmMSGDataEvent& aMsg);
	//	�����¼�
	void OnMSGBulletin(const xmMSGBulletin& aMsg);
	//	��Ϣ�¼�
	void OnMSGMessage(const xmMSGMCast& aMsg);

	//	��־
	void OnMSGLog(const xmMSGLog& aMsg);
	xmCREATE_MESSAGE_MAP(SystemLan);

private:
	xmVDR m_VDR;
	xmAccessPoint m_AP;
	std::vector<xmMsgHandler*> m_vHandler;
	//	VDR����
	xmMCastVDRAttr m_LanAttr;
	//	��¼ʱ��ȴ�ʱ��
	long m_nWaitTIme;
	//	�ڵ�����
	xmNode m_Node;
	//	���ݼ���keyΪ���ݼ�����������
	std::map<std::string, xmPtr<xmDataSet>> m_mapDataSet;
	//	����Ϊ���ݼ��������ƣ��ں����н�ת��Ϊ���ݼ�VDR����
	xmDataSet* __AddDataSet(const std::string& strName);

	bool m_bClear;

	//	���ļ�����
	unsigned int m_uClickCycle;
	size_t m_uClickCount;
	//	ϵͳʱ��
	time_t m_uSystemTime;

	//	��¼ϵͳ״̬�ͷ��汶��
	unsigned int m_uSimuState;
	int m_nSimuSpeed;
	//	ϵͳ�У���ʾ��������������������
	xmString m_strSimuState;
	xmString m_strSimuSpeed;


	int m_nLEC_System;


	//	ָ�����¼�ӳ���
	typedef struct _tagMsgObject
	{
		unsigned int m_uType;
		xmString m_strCommand;
		xmString m_strName;
		xmString m_strValue;	//	�ַ���������������ֵ
		xmString m_strParam;
		xmValue  m_ctrlValue;	//	xmValue������������ֵ�����ڴ���ʱ����֪���������ͣ�������Ҫ�����й����г�ʼ����
	}MsgObject;
	//	ѵ����ʼָ��
	xmString m_strStartMsgDataName;
	xmPtr<MsgObject> m_pStartMsgObject;
	std::multimap<xmString, xmPtr<MsgObject> > m_mapMsgObject;
	bool __IsCmdArrived(const xmValue& sensValue, MsgObject* pObject);

	xmRet __LoadVdr(const Json::Value& jvObject);
	xmRet __LoadCommand(const Json::Value& jvObject);
	xmRet __LoadState(const Json::Value& jvObject);
	MsgObject* __LoadObject(const Json::Value& jvObject);
	std::map<xmString, bool> m_mapUserSensitive;	//	���һ������ϵͳ���е��������ݣ��û��Ƿ�Ҳע��������

	void __UpdateSystemState();
};
#endif  //  __UMSF_SYSTEM_LAN_H

