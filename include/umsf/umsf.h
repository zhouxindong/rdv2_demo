/******************************************************************************/
/** @file umsf.h
@brief
	定义xmUMSF（Unified Model Schedule Framework）类，该类是对UMSF框架Manager提供的
	供用户开发使用的接口。
@author 邓华 dehxya@163.com
@par 所属模块
	umsf
@par 相关文件
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
		//	所有UMSF具备的功能，均由xmManager具体实现
		//	xmUMSF类仅封装了用户需要使用的部分
		xmUMSF(void);
		~xmUMSF(void);

		//	下列接口不可重入

		//	启动、停止或重启
		xmRet Boot(const char* strUmsfConfigFile, const char* strModelConfigFile);
		xmRet Halt(unsigned int uTimeoutMillis = INFINITE);
		xmRet Reboot(unsigned int uTimeoutMillis = INFINITE, const char* strUmsfConfigFile = NULL, const char* strModelConfigFile = NULL);
		
		//	仿真系统访问接口
		xmISystem* System() const;
		//	调度节点访问接口
		xmINode* Node();

		//	管理器访问接口
		xmMsgHandler* Manager() const;

		//	模型信息
		size_t ModelCount() const;
		xmMrm* Model(size_t uIndex);
		xmMrm* Model(const xmString& strModelName);
		const xmString& ModelName(xmMrm* pModel) const;
		//	获取和打印模型XML文件配置信息
		xmString PrintDriveCycle(xmMrm* pModel) const;
		xmString PrintClassDeclare(xmMrm* pModel, const xmString& strClassName = NULL) const;
		xmString PrintDataDeclare(xmMrm* pModel, const xmString& strDataName = NULL) const;
		xmString PrintGroupDeclare(xmMrm* pModel, const xmString& strGroupName = NULL) const;
		xmString PrintScheduleConfig(xmMrm* pModel) const;

		//	实例信息
		size_t InstanceCount(xmMrm* pModel) const;
		xmMrtc* Instance(xmMrm* pModel, size_t uIndex);
		xmMrtc* Instance(xmMrm* pModel, const xmString& strInstanceName);
		const xmString& InstanceName(xmMrtc* pInstance) const;
		//	实例自检
		xmRet Inspect(xmMrtc* pInstance, xmString& strError);

		//	数据信息
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
		//	获得模型数值的下面两个函数，传入xmMrtc指针，是为了在读数值时进行读保护，
		//	避免与模型的写入操作发生冲突
		void DataValue(xmMrtc* pInstance, xmRuntimeData* pData, xmValue& dataValue) const;
		void DataSystemValue(xmMrtc* pInstance, xmRuntimeData* pData, xmValue& systemValue) const;
		//	打印数据动态信息
		xmString PrintData(xmMrtc* pInstance, xmRuntimeData* pData = NULL) const;
		//	打印数据静态信息
		const xmString& PrintData(xmRuntimeData* pData) const;

		//	组信息
		size_t GroupCount(xmMrtc* pInstance) const;
		xmRuntimeGroup* Group(xmMrtc* pInstance, size_t uIndex);
		xmRuntimeGroup* Group(xmMrtc* pInstance, const xmString& strDataName);
		const xmString& GroupName(xmRuntimeGroup* pGroup) const;
		xmEIOType GroupType(xmRuntimeGroup* pGroup) const;
		//	打印组动态信息
		xmString PrintGroup(xmMrtc* pInstance, xmRuntimeGroup* pGroup = NULL) const;
		//	打印组静态信息
		const xmString& PrintGroup(xmRuntimeGroup* pGroup) const;


		//	触发器信息
		size_t TimeTriggerCount();
		xmString TimeTriggerPath(size_t uTriggerID);
		size_t EventTriggerCount();
		xmString EventTriggerPath(size_t uTriggerID);
		size_t DataTriggerCount();
		xmString DataTriggerPath(size_t uTriggerID);


		//	统计信息
		static const char* GetManagerFunctionName(xmEManagerFunction eFunc);
		static xmEManagerFunction GetManagerFunctionFlag(const xmString& strFuncName);
		static const char* GetInstanceFunctionName(xmEInstanceFunction eFunc);
		static xmEInstanceFunction GetInstanceFunctionFlag(const xmString& strFuncName);


		//	对消息队列的长度和负载统计
		//	Manager、TimeTrigger、EventTrigger
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

		//	对消息响应函数进行执行时间统计
		//	Manager，指定Manager的哪个消息响应函数
		void StartETS(xmEManagerFunction uFuncID);
		void StopETS(xmEManagerFunction uFuncID);
		double ExecuteTime(xmEManagerFunction uFuncID);
		//	Trigger，每个触发器只有一个消息响应函数，由eMsgHandleObject指定哪个触发器，
		//	由uTriggerID指定哪个消息队列的第几个触发器
		void StartETS(xmEMsgHandleObject eMsgHandleObject, size_t uTriggerID);
		void StopETS(xmEMsgHandleObject eMsgHandleObject, size_t uTriggerID);
		double ExecuteTime(xmEMsgHandleObject eMsgHandleObject, size_t uTriggerID);
		//	Model Instance，指定哪个实例的那个函数
		void StartETS(xmMrtc* pInstance, xmEInstanceFunction uFuncID);
		void StopETS(xmMrtc* pInstance, xmEInstanceFunction uFuncID);
		double ExecuteTime(xmMrtc* pInstance, xmEInstanceFunction uFuncID);

		//	整个UMSF中所有消息队列长度统计
		size_t TotalMsgCount() const;

		//	如果上述接口调用失败，则通过此函数获取最后失败信息
		//	此函数无法获取UMSF在运行过程中产生的错误
		xmString GetLastError() const;
	private:
		xmManager* m_pManager;
		xmString m_strUmsfConfigFile;
		xmString m_strModelConfigFile;
	};
}
#endif	//	__SSA_UMSF_H