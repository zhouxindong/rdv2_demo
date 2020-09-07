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
@attention
	一个进程只能创建一个xUMSF实例。
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

		//	数据基本信息
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
		//	数值信息
		xmRet GetValue(xmValue& dataValue) const;
		xmRet GetValue(const xmString& strDataPath, xmValue& dataValue) const;
		xmRet SetValue(const xmValue& dataValue);
		xmRet SetValue(const xmString& strDataPath, const xmValue& dataValue);
		void DataSystemValue(xmValue& systemValue) const;
		//	标签信息
		size_t GetDataTag(xmPtr<const char*[]>& vTagName) const;
		bool   IsDataTagged(const xmString& strTagName) const;
		//	由VDR自动复位的周期
		unsigned int DataAutoResetClick() const;
		//	由模型自动复位的周期
		unsigned int DataModelResetCycle() const;


		//	控制模型数据IO后，是否记录日志
		void LogControl(bool bOpen);
		bool IsLogged() const;
		//	控制模型数据IO时，是否统计IO时间
		void ETSControl(bool bOpen);
		bool IsETS();
		double ExecuteTime() const;

		//	打印数据动态信息
		xmString PrintData() const;
		//	打印数据静态信息
		const xmString& PrintXML() const;

		//	获取当前错误信息
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

		//	组信息
		const xmString& GroupFlag() const;
		const xmString& GroupName() const;
		xmEIOType IOType() const;
		int GroupActiveState() const;
		bool GroupIsEnabled() const;
		void GroupSetEnabled(bool bIsEnabled);

		//	组的日志控制
		void LogControl(bool bOpen);
		bool IsLogged() const;
		//	组IO时间统计控制
		void ETSControl(bool bOpen);
		bool IsETS();
		double ExecuteTime() const;

		//	触发器信息
		//	返回触发器类型，如果无触发器，返回TT_NO_TRIGGER
		xmETriggerType TriggerType() const;
		//	返回触发器标识，如：“TIME_TIGGER GNC->仿真数据”
		const char* TriggerFlag() const;
		//	返回触发器名称，如“TIME_TRIGGET”、“DATA_TRIGGER”
		const char* TriggerName() const;
		//	返回触发器参数
		const char* TriggerParam() const;
		//	返回触发器脚本
		const char* TriggerScript() const;
		//	触发器日志控制
		void TriggerLogControl(bool bOpen);
		bool IsTriggerLogged() const;
		//	触发器执行时，是否统计执行时间
		void TriggerETSControl(bool bOpen);
		bool IsTriggerETS();
		double TriggerExecuteTime() const;

		//	组数据
		size_t  DataCount() const;
		xmData* Data(size_t uIndex);

		//	打印组动态信息
		xmString PrintGroup() const;
		//	打印组静态信息
		const xmString& PrintXML() const;

		//	获取当前错误信息
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

		//	运行，指模型实例的运行环境创建后，是否加载计算文件、创建模型实例服务和模型运行调度线程，开始数据处理
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

		//	实例触发器信息
		//	TriggerPath为“触发器标识+触发条件”，如“TIME_TIGGER GNC->仿真数据: 1000/100”
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
		//	获取和打印模型XML文件配置信息
		xmString PrintDriveCycle() const;
		xmString PrintClassDeclare(const xmString& strClassName = NULL) const;
		xmString PrintDataDeclare(const xmString& strDataName = NULL) const;
		xmString PrintGroupDeclare(const xmString& strGroupName = NULL) const;
		xmString PrintScheduleConfig() const;

		//	实例信息
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
		//	所有UMSF具备的功能，均由xmManager具体实现
		//	xmUMSF类仅封装了用户需要使用的部分
		//	CreateUMSF()函数，只允许创建一个UMSF实例，当试图创建多个实例的时候，会返回NULL
		static xmUMSF* CreateUMSF();
		static void DestroyUMSF();
		static xmUMSF* UMSF();

		//	下列接口不可重入

		//	启动、停止或重启
		xmRet Boot(const char* strUmsfConfigFile, const char* strModelConfigFile);
		xmRet Halt(unsigned int uTimeoutMillis = INFINITE);
		xmRet Reboot(unsigned int uTimeoutMillis = INFINITE, const char* strUmsfConfigFile = NULL, const char* strModelConfigFile = NULL);
		bool IsRunning() const;
		
		//	运行模式
		xmEUmsfMode GetModel();
		void SetMode(xmEUmsfMode eUmsfMode);


		//	仿真系统访问接口
		xmISystem* System() const;
		//	调度节点访问接口
		xmINode* Node();

		//	管理器访问接口
		xmMsgHandler* Manager() const;

		//	组合后的输出数据
		const xmOrderMap<xmString, xmPtr<xmClass>>& OutputData() const;

		//	模型信息
		size_t ModelCount() const;
		xmModel* Model(size_t uIndex);
		xmModel* Model(const xmString& strModelName);

		//	对负载进行统计
		void StartWLS(unsigned int uTime);
		void StopWLS();
		double WorkLoad();
		//	对消息队列的长度进行统计
		unsigned int MsgCount();
		void MsgPause();
		void MsgResume();
		//	对消息响应函数进行执行时间统计
		void ETSControl(xmESystemMessageFunction uFuncID, bool bOpen);
		bool IsETS(xmESystemMessageFunction uFuncID);
		double ExecuteTime(xmESystemMessageFunction uFuncID);

		//	整个UMSF中所有消息队列进行操作
		size_t TotalMsgCount() const;
		void TotalMsgPause();
		void TotalMsgResume();

		//	版本信息
		const xmString& NodeID() const;
		const xmString& NodeName() const;
		const xmString& Version() const;
		const xmString& ModelServerVersion() const;
		const xmString& ModelInterfaceVersion() const;

		//	统计信息
		static const char* GetSystemMessageFunctionName(xmESystemMessageFunction eFunc);
		static xmESystemMessageFunction GetSystemMessageFunctionFlag(const xmString& strFuncName);
		static const char* GetInstanceFunctionName(xmEInstanceFunction eFunc);
		static xmEInstanceFunction GetInstanceFunctionFlag(const xmString& strFuncName);

		//	如果上述接口调用失败，则通过此函数获取最后失败信息
		//	此函数无法获取UMSF在运行过程中产生的错误
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