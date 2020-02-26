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
		return "VDR Communication V5.0";
	};
	virtual xmRet Login(const xmString& strNodeID, const xmString& strAttribute = NULL);
	virtual xmRet Logout();
	virtual bool  IsLogined(void);

	virtual xmRet RegisterMsgHandler(xmMsgHandler* pHandler);
	virtual xmRet RegisterData(const xmString& strDataName, const xmValue& aValue, const xmString& strAttribute = NULL);
	virtual xmRet RegisterSensDate(const xmString& strDataPath);

	virtual void  ReadyToGo();

	virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue);
	virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue);

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
	//	�ڵ�����
	xmNode m_Node;
	//	���ݼ�����
	xmDataSet m_DataSet;


	//	���ļ�����
	unsigned int m_uClickCycle;
	size_t m_uClickCount;
	//	ϵͳʱ��
	time_t m_uSystemTime;

	//	��¼ϵͳ״̬�ͷ��汶��
	unsigned int m_uSimuState;
	int m_nSimuSpeed;

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
	std::multimap<xmString, xmPtr<MsgObject> > m_mapMsgObject;
	xmRet __LoadVdr(const Json::Value& jvObject);
	xmRet __LoadCommand(const Json::Value& jvObject);
	MsgObject* __LoadObject(const Json::Value& jvObject);
	std::map<xmString, bool> m_mapUserSensitive;	//	���һ������ϵͳ���е��������ݣ��û��Ƿ�Ҳע��������
};
#endif  //  __UMSF_SYSTEM_LAN_H

