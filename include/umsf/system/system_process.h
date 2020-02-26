#ifndef __UMSF_SYSTEM_PROCESS_H
#define __UMSF_SYSTEM_PROCESS_H

#include "../isystem.h"
#include "../../../extlibs/jsoncpp/include/json/json.h"
using namespace ssa;

class SystemProcess : public xmISystem
{
public:
	SystemProcess();
	virtual ~SystemProcess();
	xmRet Create(const char* pPropertyJson);
private:
	//	�߾��ȶ�ʱ��
	UINT m_wAccuracy;
	MMRESULT m_TimerID;
	static void PASCAL PrecisionTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);


public:
	virtual xmString Version() { return "PROCESS V0.1"; };
	virtual xmRet Login(const xmString& strNodeID, const xmString& strAttribute = NULL) { return xmE_SUCCESS; };
	virtual xmRet Logout() { return xmE_SUCCESS; };
	virtual bool  IsLogined(void) { return true; };

	virtual xmRet RegisterMsgHandler(xmMsgHandler* pHandler);
	virtual xmRet RegisterData(const xmString& strDataName, const xmValue& aValue, const xmString& strAttribute = NULL);
	virtual xmRet RegisterSensDate(const xmString& strDataPath);
	
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
	virtual time_t GetSystemTime(void) const;

	virtual xmString PrintData(const xmString& strDataName = NULL);

private:
	mutable xmMutex m_HandlerMutex;
	mutable xmMutex m_DataMutex;

	//	��¼ϵͳ״̬�ͷ��汶��
	unsigned int m_uSimuState;
	int m_nSimuSpeed;

	//	���ļ�����
	bool         m_bClickActive;
	unsigned int m_uClickCycle;
	size_t m_uClickCount;

	//	��Ϣ����ӿ�
	std::vector<xmMsgHandler*> m_vHandler;

	//	���ݱ�
	std::map<xmString, xmPtr<xmValue>> m_mapData;
	std::map<xmString, xmStringVector> m_mapSensitivePath;
	//	�ж�����������һ���Ƿ�����һ�����Ӽ�
	bool _IsSubset(const xmStringVector& aVector, const xmStringVector& bVector);

	//	ָ�����¼�ӳ���
	typedef struct _tagMsgObject
	{
		unsigned int m_uType;
		xmString m_strCommand;
		xmString m_strName;
		xmString m_strValue;
		xmString m_strParam;
		xmValue  m_ctrlValue;
	}MsgObject;
	std::multimap<xmString, xmPtr<MsgObject> > m_mapMsgObject;
	MsgObject* __LoadObject(const Json::Value& jvObject);
};
#endif  //  __UMSF_SYSTEM_PROCESS_H

