/******************************************************************************/
/** @file umsf.h
@brief
	����xmUMSF��Unified Model Schedule Framework���࣬�����Ƕ�UMSF���Manager�ṩ��
	���û�����ʹ�õĽӿڡ�
@author �˻� dehxya@163.com
@par ����ģ��
	umsf
@par ����ļ�
	umsf.cpp
@attention
	һ������ֻ�ܴ���һ��xUMSFʵ����
@see ssa::xmUMSF
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_UMSF_H
#define __SSA_UMSF_H
#include "../base/base.h"
#include "isystem.h"
#include "inode.h"

#ifdef xmOS_WINDOWS
#ifdef __SSA_UMSF_EXPORT
#define xmUMSF_EXPORT __declspec(dllexport)
#else
#define xmUMSF_EXPORT __declspec(dllimport)
#endif
#else  // xmOS_LINUX
#define xmUMSF_EXPORT
#endif


namespace ssa
{
	class xmUMSF;
	class xmManager;
	class xmMrm;
	class xmMrtc;
	class xmRuntimeData;
	class xmRuntimeGroup;
	class xmModel;
	class xmInstance;
	class xmGroup;

	class xmUMSF_EXPORT xmData
	{
		friend xmInstance;
		friend xmGroup;
	public:
		xmData(const xmData& data) : m_pMrtc(data.m_pMrtc), m_pRtData(data.m_pRtData) {};
		~xmData() {};

		//	���ݻ�����Ϣ
		const xmString& DataFlag() const;
		const xmString& DataName() const;
		xmEDataType DataType() const;
		xmEIOType IOType() const;
		xmEDataCheckType CheckType() const;
		xmEDataMatchTarget DataMatchTarget() const;
		const xmString& DataMatchName() const;
		const xmString& DataSystemPath() const;
		const xmString& DataSet() const;
		int DataQoS() const;
		xmString DataPrint() const;
		const xmValue& DataInitValue() const;
		//	��ֵ��Ϣ
		xmRet GetValue(xmValue& dataValue) const;
		xmRet GetValue(const xmString& strDataPath, xmValue& dataValue) const;
		xmRet SetValue(const xmValue& dataValue);
		xmRet SetValue(const xmString& strDataPath, const xmValue& dataValue);
		void DataSystemValue(xmValue& systemValue) const;
		//	��ǩ��Ϣ
		size_t GetDataTag(xmPtr<const char*[]>& vTagName) const;
		bool   IsDataTagged(const xmString& strTagName) const;
		//	��VDR�Զ���λ������
		unsigned int DataAutoResetClick() const;
		//	��ģ���Զ���λ������
		unsigned int DataModelResetCycle() const;


		//	����ģ������IO���Ƿ��¼��־
		void LogControl(bool bOpen);
		bool IsLogged() const;
		//	����ģ������IOʱ���Ƿ�ͳ��IOʱ��
		void ETSControl(bool bOpen);
		bool IsETS();
		double ExecuteTime() const;

		//	��ӡ���ݶ�̬��Ϣ
		xmString PrintData() const;
		//	��ӡ���ݾ�̬��Ϣ
		const xmString& PrintXML() const;

		//	��ȡ��ǰ������Ϣ
		xmRet GetErrorCode_Processor() const;
		const xmString& GetErrorMessage_Processor() const;
		xmRet GetErrorCode_DataIO() const;
		const xmString& GetErrorMessage_DataIO() const;
	private:
		xmData(xmMrtc* pMrtc, xmRuntimeData* pRtData);
		xmMrtc* const m_pMrtc;
		xmRuntimeData* const m_pRtData;
	};

	class xmUMSF_EXPORT xmGroup
	{
		friend xmInstance;
	public:
		xmGroup(const xmGroup& group) : m_pMrtc(group.m_pMrtc), m_pRtGroup(group.m_pRtGroup) {};
		~xmGroup() {};

		//	����Ϣ
		const xmString& GroupFlag() const;
		const xmString& GroupName() const;
		xmEIOType IOType() const;
		int GroupActiveState() const;
		bool GroupIsEnabled() const;
		void GroupSetEnabled(bool bIsEnabled);

		//	�����־����
		void LogControl(bool bOpen);
		bool IsLogged() const;
		//	��IOʱ��ͳ�ƿ���
		void ETSControl(bool bOpen);
		bool IsETS();
		double ExecuteTime() const;

		//	��������Ϣ
		//	���ش��������ͣ�����޴�����������TT_NO_TRIGGER
		xmETriggerType TriggerType() const;
		//	���ش�������ʶ���磺��TIME_TIGGER GNC->�������ݡ�
		const char* TriggerFlag() const;
		//	���ش��������ƣ��硰TIME_TRIGGET������DATA_TRIGGER��
		const char* TriggerName() const;
		//	���ش���������
		const char* TriggerParam() const;
		//	���ش������ű�
		const char* TriggerScript() const;
		//	��������־����
		void TriggerLogControl(bool bOpen);
		bool IsTriggerLogged() const;
		//	������ִ��ʱ���Ƿ�ͳ��ִ��ʱ��
		void TriggerETSControl(bool bOpen);
		bool IsTriggerETS();
		double TriggerExecuteTime() const;

		//	������
		size_t  DataCount() const;
		xmData* Data(size_t uIndex);

		//	��ӡ�鶯̬��Ϣ
		xmString PrintGroup() const;
		//	��ӡ�龲̬��Ϣ
		const xmString& PrintXML() const;

		//	��ȡ��ǰ������Ϣ
		xmRet GetErrorCode_Trigger() const;
		const xmString& GetErrorMessage_Trigger() const;
		xmRet GetErrorCode_GroupIO() const;
		const xmString& GetErrorMessage_GroupIO() const;
	private:
		xmGroup(xmMrtc* pMrtc, xmRuntimeGroup* pRtGroup);
		xmMrtc* const m_pMrtc;
		xmRuntimeGroup* const m_pRtGroup;
	};

	class xmUMSF_EXPORT xmInstance
	{
		friend xmModel;
	public:
		xmInstance(const xmInstance& instance) : m_pMrtc(instance.m_pMrtc) {};
		~xmInstance() {};

		//	���У�ָģ��ʵ�������л����������Ƿ���ؼ����ļ�������ģ��ʵ�������ģ�����е����̣߳���ʼ���ݴ���
		bool IsAutoRun() const;
		bool IsRunning() const;
		xmRet Start();
		void Stop();
		xmRet Restart();

		const xmString& SchedulerFlag() const;
		const xmString& InstanceFlag() const;
		const xmString& InstanceName() const;
		void LogControl(xmEModelInterface eMi, bool bOpen);
		bool IsLogged(xmEModelInterface eMi) const;
		xmRet Inspect(xmString& strError);
		size_t GetTagName(xmPtr<const char*[]>& vTagName) const;
		size_t GetTagData(const xmString& strTagName, xmPtr<const char*[]>& vDataName) const;

		//	ʵ����������Ϣ
		//	TriggerPathΪ����������ʶ+�������������硰TIME_TIGGER GNC->��������: 1000/100��
		size_t TimeTriggerCount() const;
		xmString TimeTriggerPath(size_t uIndex) const;
		size_t EventTriggerCount() const;
		xmString EventTriggerPath(size_t uIndex) const;
		size_t CommandTriggerCount() const;
		xmString CommandTriggerPath(size_t uIndex) const;
		size_t DataTriggerCount() const;
		xmString DataTriggerPath(size_t uIndex) const;

		//	Model Instance and Schedule
		void StartWLS(unsigned int uTime, xmEInstanceMsgHandle eHandle = IMH_INSTANCE);
		void StopWLS(xmEInstanceMsgHandle eHandle = IMH_INSTANCE);
		double WorkLoad(xmEInstanceMsgHandle eHandle = IMH_INSTANCE) const;
		
		//	Model Instance and Schedule
		void MsgPause(xmEInstanceMsgHandle eHandle = IMH_INSTANCE);
		void MsgResume(xmEInstanceMsgHandle eHandle = IMH_INSTANCE);
		unsigned int MsgCount(xmEInstanceMsgHandle eHandle = IMH_INSTANCE) const;

		//	Model Schedule
		void ETSControl(xmESystemMessageFunction uFuncID, bool bOpen);
		bool IsETS(xmESystemMessageFunction uFuncID);
		double ExecuteTime(xmESystemMessageFunction uFuncID) const;

		//	Model Instance
		void ETSControl(xmEInstanceFunction uFuncID, bool bOpen);
		bool IsETS(xmEInstanceFunction uFuncID);
		double ExecuteTime(xmEInstanceFunction uFuncID) const;


		size_t  DataCount() const;
		size_t  DataCount(xmEIOType ioType) const;
		xmData* Data(xmEIOType ioType, size_t uIndex);
		xmData* Data(const xmString& strDataName);
		xmData* Data(xmEIOType ioType, const xmString& strDataName);

		size_t   GroupCount() const;
		xmGroup* Group(size_t uIndex);
		xmGroup* Group(const xmString& strGroupName);

	private:
		xmInstance(xmMrtc* pMrtc);
		xmMrtc* const m_pMrtc;
		xmData* _MakeObject(xmRuntimeData* pRtData);
		xmGroup* _MakeObject(xmRuntimeGroup* pRtGroup);
	};

	class xmUMSF_EXPORT xmModel
	{
		friend xmUMSF;
	public:
		xmModel(const xmModel& model) : m_pMrm(model.m_pMrm) {};
		~xmModel() {};

		const xmString& ModelName() const;
		unsigned int DriveTime() const;
		unsigned int DriveClick() const;
		bool IsRespondStart() const;
		bool IsRespondFreeze() const;
		bool IsRespondSpeed() const;
		//	��ȡ�ʹ�ӡģ��XML�ļ�������Ϣ
		xmString PrintDriveCycle() const;
		xmString PrintClassDeclare(const xmString& strClassName = NULL) const;
		xmString PrintDataDeclare(const xmString& strDataName = NULL) const;
		xmString PrintGroupDeclare(const xmString& strGroupName = NULL) const;
		xmString PrintScheduleConfig() const;

		//	ʵ����Ϣ
		size_t  InstanceCount() const;
		xmInstance* Instance(size_t uIndex);
		xmInstance* Instance(const xmString& strInstanceName);

	private:
		xmModel(xmMrm* pMrm);
		xmMrm* const m_pMrm;
		xmInstance* _MakeObject(xmMrtc* pMrtc);
	};


#define xmE_CONNECT_VDR __EC_UMSF(5)
	class xmUMSF_EXPORT xmUMSF : public xmUncopyable
	{
	public:
		//	����UMSF�߱��Ĺ��ܣ�����xmManager����ʵ��
		//	xmUMSF�����װ���û���Ҫʹ�õĲ���
		//	CreateUMSF()������ֻ������һ��UMSFʵ��������ͼ�������ʵ����ʱ�򣬻᷵��NULL
		static xmUMSF* CreateUMSF();
		static void DestroyUMSF();
		static xmUMSF* UMSF();

		//	���нӿڲ�������

		//	������ֹͣ������
		xmRet Boot(const char* strUmsfConfigFile, const char* strModelConfigFile);
		xmRet Halt(unsigned int uTimeoutMillis = INFINITE);
		xmRet Reboot(unsigned int uTimeoutMillis = INFINITE, const char* strUmsfConfigFile = NULL, const char* strModelConfigFile = NULL);
		bool IsRunning() const;
		
		//	����ģʽ
		xmEUmsfMode GetModel();
		void SetMode(xmEUmsfMode eUmsfMode);


		//	����ϵͳ���ʽӿ�
		xmISystem* System() const;
		//	���Ƚڵ���ʽӿ�
		xmINode* Node();

		//	���������ʽӿ�
		xmMsgHandler* Manager() const;

		//	��Ϻ���������
		const xmOrderMap<xmString, xmPtr<xmClass>>& OutputData() const;

		//	ģ����Ϣ
		size_t ModelCount() const;
		xmModel* Model(size_t uIndex);
		xmModel* Model(const xmString& strModelName);

		//	�Ը��ؽ���ͳ��
		void StartWLS(unsigned int uTime);
		void StopWLS();
		double WorkLoad();
		//	����Ϣ���еĳ��Ƚ���ͳ��
		unsigned int MsgCount();
		void MsgPause();
		void MsgResume();
		//	����Ϣ��Ӧ��������ִ��ʱ��ͳ��
		void ETSControl(xmESystemMessageFunction uFuncID, bool bOpen);
		bool IsETS(xmESystemMessageFunction uFuncID);
		double ExecuteTime(xmESystemMessageFunction uFuncID);

		//	����UMSF��������Ϣ���н��в���
		size_t TotalMsgCount() const;
		void TotalMsgPause();
		void TotalMsgResume();

		//	�汾��Ϣ
		const xmString& NodeID() const;
		const xmString& NodeName() const;
		const xmString& Version() const;
		const xmString& ModelServerVersion() const;
		const xmString& ModelInterfaceVersion() const;

		//	ͳ����Ϣ
		static const char* GetSystemMessageFunctionName(xmESystemMessageFunction eFunc);
		static xmESystemMessageFunction GetSystemMessageFunctionFlag(const xmString& strFuncName);
		static const char* GetInstanceFunctionName(xmEInstanceFunction eFunc);
		static xmEInstanceFunction GetInstanceFunctionFlag(const xmString& strFuncName);

		//	��������ӿڵ���ʧ�ܣ���ͨ���˺�����ȡ���ʧ����Ϣ
		//	�˺����޷���ȡUMSF�����й����в����Ĵ���
		//xmString GetLastError() const;
	private:
		xmUMSF(void);
		~xmUMSF(void);
		static xmUMSF* m_pUMSF;

		xmManager* m_pManager;
		xmString m_strUmsfConfigFile;
		xmString m_strModelConfigFile;

		xmModel* _MakeObject(xmMrm* pMrm);
	};
}
#endif	//	__SSA_UMSF_H