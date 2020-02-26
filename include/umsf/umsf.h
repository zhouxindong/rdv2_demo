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
	//class xmUMSF;
	//class xmUMSF_EXPORT xmData
	//{
	//	friend xmUMSF;
	//public:
	//	const xmString& DataName() const;
	//private:
	//	xmData();
	//	~xmData();
	//};
	//class xmUMSF_EXPORT xmGroup
	//{
	//	friend xmUMSF;
	//public:
	//	const xmString& GroupName() const;
	//private:
	//	xmGroup();
	//	~xmGroup();
	//};
	//class xmUMSF_EXPORT xmInstance
	//{
	//	friend xmUMSF;
	//public:
	//	const xmString& InstanceName() const;
	//private:
	//	xmInstance();
	//	~xmInstance();
	//};
	//class xmUMSF_EXPORT xmModel
	//{
	//	friend xmUMSF;
	//public:
	//	const xmString& ModelName() const;
	//	size_t InstanceCount() const;
	//	xmInstance* Instance(size_t uIndex) const;
	//	xmInstance* Instance(const xmString& strInstanceName) const;
	//private:
	//	xmModel();
	//	~xmModel();
	//};


	class xmManager;
	class xmMrm;
	class xmMrtc;
	class xmRuntimeData;
	class xmRuntimeGroup;
	class xmUMSF_EXPORT xmUMSF : public xmUncopyable
	{
	public:
		//	����UMSF�߱��Ĺ��ܣ�����xmManager����ʵ��
		//	xmUMSF�����װ���û���Ҫʹ�õĲ���
		xmUMSF(void);
		~xmUMSF(void);

		//	���нӿڲ�������

		//	������ֹͣ������
		xmRet Boot(const char* strUmsfConfigFile, const char* strModelConfigFile);
		xmRet Halt(unsigned int uTimeoutMillis = INFINITE);
		xmRet Reboot(unsigned int uTimeoutMillis = INFINITE, const char* strUmsfConfigFile = NULL, const char* strModelConfigFile = NULL);
		
		//	����ϵͳ���ʽӿ�
		xmISystem* System() const;
		//	���Ƚڵ���ʽӿ�
		xmINode* Node();

		//	���������ʽӿ�
		xmMsgHandler* Manager() const;

		//	ģ����Ϣ
		size_t ModelCount() const;
		xmMrm* Model(size_t uIndex);
		xmMrm* Model(const xmString& strModelName);
		const xmString& ModelName(xmMrm* pModel) const;
		//	��ȡ�ʹ�ӡģ��XML�ļ�������Ϣ
		xmString PrintDriveCycle(xmMrm* pModel) const;
		xmString PrintClassDeclare(xmMrm* pModel, const xmString& strClassName = NULL) const;
		xmString PrintDataDeclare(xmMrm* pModel, const xmString& strDataName = NULL) const;
		xmString PrintGroupDeclare(xmMrm* pModel, const xmString& strGroupName = NULL) const;
		xmString PrintScheduleConfig(xmMrm* pModel) const;

		//	ʵ����Ϣ
		size_t InstanceCount(xmMrm* pModel) const;
		xmMrtc* Instance(xmMrm* pModel, size_t uIndex);
		xmMrtc* Instance(xmMrm* pModel, const xmString& strInstanceName);
		const xmString& InstanceName(xmMrtc* pInstance) const;
		//	ʵ���Լ�
		xmRet Inspect(xmMrtc* pInstance, xmString& strError);

		//	������Ϣ
		size_t DataCount(xmMrtc* pInstance, xmEIOType ioType) const;
		xmRuntimeData* Data(xmMrtc* pInstance, const xmString& strDataName);
		xmRuntimeData* Data(xmMrtc* pInstance, xmEIOType ioType, size_t uIndex);
		xmRuntimeData* Data(xmMrtc* pInstance, xmEIOType ioType, const xmString& strDataName);
		const xmString& DataName(xmRuntimeData* pData) const;
		xmEDataMatchTarget DataMatchTarget(xmRuntimeData* pData) const;
		const xmString& DataMatchName(xmRuntimeData* pData) const;
		const xmString& DataSystemPath(xmRuntimeData* pData) const;
		xmString DataPrint(xmRuntimeData* pData) const;
		xmEIOType DataType(xmRuntimeData* pData) const;
		const xmValue& DataInitValue(xmRuntimeData* pData) const;
		//	���ģ����ֵ��������������������xmMrtcָ�룬��Ϊ���ڶ���ֵʱ���ж�������
		//	������ģ�͵�д�����������ͻ
		void DataValue(xmMrtc* pInstance, xmRuntimeData* pData, xmValue& dataValue) const;
		void DataSystemValue(xmMrtc* pInstance, xmRuntimeData* pData, xmValue& systemValue) const;
		//	��ӡ���ݶ�̬��Ϣ
		xmString PrintData(xmMrtc* pInstance, xmRuntimeData* pData = NULL) const;
		//	��ӡ���ݾ�̬��Ϣ
		const xmString& PrintData(xmRuntimeData* pData) const;

		//	����Ϣ
		size_t GroupCount(xmMrtc* pInstance) const;
		xmRuntimeGroup* Group(xmMrtc* pInstance, size_t uIndex);
		xmRuntimeGroup* Group(xmMrtc* pInstance, const xmString& strDataName);
		const xmString& GroupName(xmRuntimeGroup* pGroup) const;
		xmEIOType GroupType(xmRuntimeGroup* pGroup) const;
		//	��ӡ�鶯̬��Ϣ
		xmString PrintGroup(xmMrtc* pInstance, xmRuntimeGroup* pGroup = NULL) const;
		//	��ӡ�龲̬��Ϣ
		const xmString& PrintGroup(xmRuntimeGroup* pGroup) const;


		//	��������Ϣ
		size_t TimeTriggerCount();
		xmString TimeTriggerPath(size_t uTriggerID);
		size_t EventTriggerCount();
		xmString EventTriggerPath(size_t uTriggerID);
		size_t DataTriggerCount();
		xmString DataTriggerPath(size_t uTriggerID);


		//	ͳ����Ϣ
		static const char* GetManagerFunctionName(xmEManagerFunction eFunc);
		static xmEManagerFunction GetManagerFunctionFlag(const xmString& strFuncName);
		static const char* GetInstanceFunctionName(xmEInstanceFunction eFunc);
		static xmEInstanceFunction GetInstanceFunctionFlag(const xmString& strFuncName);


		//	����Ϣ���еĳ��Ⱥ͸���ͳ��
		//	Manager��TimeTrigger��EventTrigger
		void StartWLS(xmEMsgHandleObject eMsgHandleObject, unsigned int uTime);
		void StopWLS(xmEMsgHandleObject eMsgHandleObject);
		double WorkLoad(xmEMsgHandleObject eMsgHandleObject);
		unsigned int MsgCount(xmEMsgHandleObject eMsgHandleObject);
		void MsgPause(xmEMsgHandleObject eMsgHandleObject);
		void MsgResume(xmEMsgHandleObject eMsgHandleObject);
		//	DataTrigger
		void StartWLS(size_t uDataTriggerID, unsigned int uTime);
		void StopWLS(size_t uDataTriggerID);
		double WorkLoad(size_t uDataTriggerID);
		unsigned int MsgCount(size_t uDataTriggerID);
		void MsgPause(size_t uDataTriggerID);
		void MsgResume(size_t uDataTriggerID);
		//	Model Instance
		void StartWLS(xmMrtc* pInstance, unsigned int uTime);
		void StopWLS(xmMrtc* pInstance);
		double WorkLoad(xmMrtc* pInstance);
		unsigned int MsgCount(xmMrtc* pInstance);
		void MsgPause(xmMrtc* pInstance);
		void MsgResume(xmMrtc* pInstance);

		//	����Ϣ��Ӧ��������ִ��ʱ��ͳ��
		//	Manager��ָ��Manager���ĸ���Ϣ��Ӧ����
		void StartETS(xmEManagerFunction uFuncID);
		void StopETS(xmEManagerFunction uFuncID);
		double ExecuteTime(xmEManagerFunction uFuncID);
		//	Trigger��ÿ��������ֻ��һ����Ϣ��Ӧ��������eMsgHandleObjectָ���ĸ���������
		//	��uTriggerIDָ���ĸ���Ϣ���еĵڼ���������
		void StartETS(xmEMsgHandleObject eMsgHandleObject, size_t uTriggerID);
		void StopETS(xmEMsgHandleObject eMsgHandleObject, size_t uTriggerID);
		double ExecuteTime(xmEMsgHandleObject eMsgHandleObject, size_t uTriggerID);
		//	Model Instance��ָ���ĸ�ʵ�����Ǹ�����
		void StartETS(xmMrtc* pInstance, xmEInstanceFunction uFuncID);
		void StopETS(xmMrtc* pInstance, xmEInstanceFunction uFuncID);
		double ExecuteTime(xmMrtc* pInstance, xmEInstanceFunction uFuncID);

		//	����UMSF��������Ϣ���г���ͳ��
		size_t TotalMsgCount() const;

		//	��������ӿڵ���ʧ�ܣ���ͨ���˺�����ȡ���ʧ����Ϣ
		//	�˺����޷���ȡUMSF�����й����в����Ĵ���
		xmString GetLastError() const;
	private:
		xmManager* m_pManager;
		xmString m_strUmsfConfigFile;
		xmString m_strModelConfigFile;
	};
}
#endif	//	__SSA_UMSF_H