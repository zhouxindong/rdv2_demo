#ifndef __SSA_UMSF_MRTC_H
#define __SSA_UMSF_MRTC_H
#include "publicres.h"
#include "mdi.h"
#include "mui.h"
#include "rtdata.h"
#include "rtgroup.h"
#include "mloader.h"
#include "mserver.h"
#include "mscheduler.h"
#include "udefine.h"

namespace ssa
{
	//	模型手动组IO时，向MRTC发送的消息。
#define tmC_APPLY_GROUP_IO	0x2100
	class tmApplyGroupIO : public xmMessage
	{
	public:
		tmApplyGroupIO(xmRuntimeGroup* pGroup) : m_pGroup(pGroup)
		{
		};
		tmApplyGroupIO(const tmApplyGroupIO& msg) : m_pGroup(msg.m_pGroup)
		{
		};
		virtual ~tmApplyGroupIO(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new tmApplyGroupIO(*this);
		};
	public:
		xmRuntimeGroup* m_pGroup;
		xmDECLARE_MESSAGE_CLASS(tmC_APPLY_GROUP_IO);
	};
	xmDECLARE_MESSAGE_FUNC(tmApplyGroupIOFunc, tmApplyGroupIO);

	//	模型手动数据IO时，向MRTC发送的消息。
#define tmC_APPLY_DATA_IO	0x2200
	class tmApplyDataIO : public xmMessage
	{
	public:
		tmApplyDataIO(xmRuntimeData* pData) : m_pData(pData)
		{
		};
		tmApplyDataIO(const tmApplyDataIO& msg) : m_pData(msg.m_pData)
		{
		};
		virtual ~tmApplyDataIO(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new tmApplyDataIO(*this);
		};
	public:
		xmRuntimeData* m_pData;
		xmDECLARE_MESSAGE_CLASS(tmC_APPLY_DATA_IO);
	};
	xmDECLARE_MESSAGE_FUNC(tmApplyDataIOFunc, tmApplyDataIO);

	/*******************************************************************************
    类名称：xmMrtc
    功  能：定义Model Runtime Context
    说  明：MRTC定义模型在仿真过程中可以使用的资源。
		    每一个模型均包含一个MRTC，模型所有的交互，均通过MRTC。
    *******************************************************************************/
	class xmUMSF;
	class xmManager;
	class xmMrm;
	class xmModelScheduler;
	class xmModelAssistScript;
	class xmMrtc : public xmMsgHandler
    {
		friend xmUMSF;
		friend xmManager;
		friend xmMrm;
		friend xmModelServer;
		friend xmModelScheduler;
		friend xmModelAssistScript;
	public:
		//	strInstanceName参数，为NULL时，表示该模型仅有一个实例，实例名称存储在MRM中
		//	如果非NULL，则模型有多个实例，这个实例的名称是strInstanceName，所有他输出的参数都是
		//	被封装在该名称的数据类内
		//	bCombined参数表示是否该模型的所有实例都被封装在一个数据类以内。
		xmMrtc(xmPublicResource* pPubRes, const xmMdi* pMdi, const xmMui* pMui, const char* strInstanceCalculationFile);
		~xmMrtc(void);

		//	（一）根据MDI创建基本模型运行环境
		void Create();
		void Destroy();
		//	模型调度器
		inline xmModelScheduler* ModelSchedule()
		{
			return m_pModelSchedule;
		}
		//	模型数据服务
		inline xmIModelServer* ModelServer()
		{
			return m_pModelServer;
		}

		//	（二）根据model.config文件中的配置信息，创建模型最终运行环境
		//	（1）数据匹配信息
		//	设置数据匹配
		//	输入数据的matchName可以是数据路径
		//	输出数据的matchName只能是数据
		//	内部数据只能匹配pInitValue
		void DataMatch();
		void DataMatch(xmRuntimeData* pData, const xmRuntimeData::SMatchInfo& matchInfo);

		//	（2）设置输出数据的组合路径
		//	pData：由于目前只支持实例级别的数据路径设置，因此应设置为NULL。
		void SetDataPath(xmRuntimeData* pData, const char* strDataPath);
		//	组合指定的输出到系统的数据，将模型的输出数据按照顺序组合成一个新的xmClass结构
		//	调用SetDataMatch前与调用后的输出结果不同
		//	如果数据已经被匹配，则按照匹配后的数据进行组合
		//	否则使用原始数据进行组合
		//	vData使用模型自身名称
		xmClass* GetCombineClass(const std::vector<xmString>& vData, xmValue& combineValue);
		//	（3）注册触发器，只有数据匹配之后，才能注册触发器，因为作为触发器的模型输入数据
		//	可能在系统匹配中被改变为其他名称
		size_t RegisterTrigger();

		//	创建并设置使用该MRTC的模型实例
		//	bIsReadyToGo参数表示当实例创建后，是否开始接收消息，除了UMSF初始加载过程中需要设置为false，
		//	其他运行过程中的所有创建实例过程，都应该创建实例后立即开始接收消息。
		void CreateInstance(bool bIsReadyToGo = true);
		void DestroyInstance();
		void RecreateInstance();
		void ReadyToGo();
		bool IsActive() const;
		bool IsAutoRun() const
		{
			return m_pMui->IsAutoRun();
		}
		//	测试当模型实例是否启动，如果启动，则向调度模块发送指定消息
		bool TestActiveAndPostMessage(const xmMessage& aMessage);

		//	获取模型名称
		inline const xmString& ModelName() const
		{
			return m_pMui->ModelName();
		}

		//	获取模型实例名称
		inline const xmString& InstanceName() const
		{
			return m_pMui->InstanceName();
		}

		inline const xmString& Flag() const
		{
			return m_strFlag;
		}

		inline xmEDataMatchTarget DefaultDataMatchTarget() const
		{
			return m_pMui->DefaultDataMatchTarget();
		}
		inline const xmString& DefaultDataSet() const
		{
			return m_pMui->DefaultDataSet();
		}

		//	获取模型中数据信息
		size_t GetDataCount() const;
		xmRuntimeData* GetData(const xmString& strDataName);
		size_t GetDataCount(xmEIOType ioType) const;
		xmRuntimeData* GetData(xmEIOType ioType, size_t uIndex);
		xmRuntimeData* GetData(xmEIOType ioType, const xmString& strDataName);

		size_t GetGroupCount() const;
		xmRuntimeGroup* GetGroup(size_t uIndex);
		xmRuntimeGroup* GetGroup(const xmString& strGroupName);

		//	复位模型实例组的激活状态，仅对ACTIVATE属性配置为auto的组有效。
		void ResetGroupEnable(bool bIsEnabled);

		//	运行环境自检
		xmRet Inspect(xmString& strError);

		//	启动工作负载统计
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	消息响应函数执行时间
		xmRet ExecuteTimeStatistics(xmEInstanceFunction eFunc, bool bDo)
		{
			if (eFunc == IF_ALL)
			{
				for (size_t i = 0; i < IF_ALL; i++)
				{
					m_bExecuteTime[i] = bDo;
				}
			}
			else if (eFunc != IF_ERROR)
			{
				m_bExecuteTime[eFunc] = bDo;
			}
			else
			{
				return xmE_FAIL;
			}
			return xmE_SUCCESS;
		}
		bool IsExecuteTimeStatistics(xmEInstanceFunction eFunc)
		{
			return (eFunc == IF_ALL || eFunc == IF_ERROR) ? false : m_bExecuteTime[eFunc];
		}
		double ExecuteTime(xmEInstanceFunction eFunc)
		{
			return (eFunc == IF_ALL || eFunc == IF_ERROR) ? 0.0 : m_fExecuteTime[eFunc];
		}

		//	日志接口
		void LogControl(xmEModelInterface eMi, bool bOpen)
		{
			if (bOpen)
			{
				m_uLog |= eMi;
			}
			else
			{
				m_uLog &= (~eMi);
			}
		}
		bool IsLogged(xmEModelInterface eMi) const
		{
			return (m_uLog & eMi) == eMi;
		}

	protected:
		unsigned int m_uLog;

	private:
		/*******************************************************************************
		接收来至 Trigger 和 Manager 的消息
		*******************************************************************************/
		void __OnWorldClick(const smWorldClick& aMsg);		
		void __OnSystemTime(const smSystemTime& aMsg);
		void __OnCtrlCommand(const smCtrlCommand& aMsg);	
		void __OnTrigger(const tmTrigger& aMsg);			
		void __OnApplyGroupIO(const tmApplyGroupIO& aMsg);	
		void __OnApplyDataIO(const tmApplyDataIO& aMsg);	
		void OnWorldClick(const smWorldClick& aMsg);		
		void OnSystemTime(const smSystemTime& aMsg);
		void OnCtrlCommand(const smCtrlCommand& aMsg);		
		void OnTrigger(const tmTrigger& aMsg);				
		void OnApplyGroupIO(const tmApplyGroupIO& aMsg);	
		void OnApplyDataIO(const tmApplyDataIO& aMsg);		
		xmCREATE_MESSAGE_MAP(xmMrtc);

		//	模型消息响应的处理时间，单位：毫秒
		xmCALC_RUNTIME(MRTC_ETS);
		bool   m_bExecuteTime[IF_ALL];
		double m_fExecuteTime[IF_ALL];

	private:
		typedef xmOrderMap<xmString, xmPtr<xmRuntimeData> > t_RtDataMap;
		typedef xmOrderMap<xmString, xmRuntimeData*> t_RtDataPtrMap;
		typedef xmOrderMap<xmString, xmPtr<xmRuntimeGroup> > t_RtGroupMap;

		//	共用资源
		xmPublicResource* const m_pPublicRes;
		//	为模型提供的服务接口
		xmPtr<xmModelServer> m_pModelServer;
		//	调度接口
		xmPtr<xmModelScheduler> m_pModelSchedule;

		//	模型配置信息
		const xmMdi* const m_pMdi;
		//	模型使用信息
		xmPtr<const xmMui> m_pMui;
		//	模型运行实例加载器
		xmIModelLoader* m_pModelLoader;
		//	实例计算文件名称
		const xmString m_strInstanceCalculationFile;

		//	当前网络节拍数，非实时，仅驱动后更新
		unsigned int m_uClickCount;
		//	模型驱动的节拍数
		unsigned int m_uDriveClick;
		//	当前系统墙时
		xmATime m_tWallTime;
		//	当前仿真时间
		xmATime m_tSimuTime;
		//	创建、删除实例时的保护
		mutable xmMutex m_IntanceMutex;

		//	实例标记，可以用于日志的标题
		const xmString m_strFlag;

		//	分内部、输入、输出三类记录模型数据
		//	下列三个表中的key都是指模型使用的数据名称
		xmBuffer m_InnerCache;
		xmBuffer m_InputCache;
		xmBuffer m_OutputCache;
		t_RtDataMap m_mapInnerData;
		t_RtDataMap m_mapInputData;
		t_RtDataMap m_mapOutputData;
		//	记录模型使用的全部数据
		t_RtDataPtrMap m_mapModelData;
		//	记录标签下的全部数据，key为标签名称，value为以遍历序号为顺序的描述数据信息
		xmOrderMap<xmString, std::map<unsigned int, xmRuntimeData*>> m_mapTagDataInfo;
		
		//	m_mapGroup记录所有IO组信息，其中key为组名，value为组对象
		t_RtGroupMap m_mapGroup;
		//	m_mapDataTriggerGroup记录所有使用数据触发器的组信息，key为敏感数据ModelName，value为组对象
		std::multimap<xmString, xmRuntimeGroup*> m_mapDataTriggerGroup;

		//	数据缓存与使用缓存的读写锁，所有运行时数据的值均在此数据缓存中
		mutable xmRWLock m_InnerCacheLock;
		mutable xmRWLock m_InputCacheLock;
		mutable xmRWLock m_OutputCacheLock;

		void _CreateInstance(bool bIsReadyToGo);
		void _DestroyInstance();

		size_t _InitData(const xmMdi::t_DataPtrMap& vInitData, t_RtDataMap& mapData, xmBuffer& cache, xmEIOType iot);
		void _InitGroup(const xmModelGroup* pModelGroup, t_RtDataMap& mapData, xmRuntimeGroup* pGroup);

		int _GroupIO(xmRuntimeGroup* pGroup, bool bLockData = true);
		xmRet _DataIO(xmRuntimeData* pData, bool bLockData = true);
		xmRet _PreIO(xmRuntimeData* pData, xmValue& aValue, bool bLockData = true);

		//	根据组的模型名称，获取该组的运行时对象
		static xmRuntimeGroup* _FindGroup(t_RtGroupMap& mapGroup, const xmString& strGroupName);
		//	在指定或整个数据表中，根据数据的模型名称和属性路径，获取该数据的运行时数值
		static xmValue _FindValue(const t_RtDataMap& mapData, const xmString& strDataPath);
		xmValue _FindValue(const xmString& strDataPath, xmEIOType& ioType) const;


		//	记录MRTC调用模型实例接口函数错误
		//	下面两个函数，之所以定义为宏，而不是函数，是为了避免在判断结果产生前，进行无用的字符串构造。
		//void __log_error_iModel(const char* strFunction, int nErrorID, const char* strErrorMsg)
#define __log_error_iModel(strFunction, nErrorID, strErrorMsg)	\
		do{														\
			if (nErrorID != xmE_SUCCESS)						\
				xmLog::error(m_strFlag, "Call model interface %s failed(%d): %s", strFunction, nErrorID, strErrorMsg);\
		} while (false)
		//	记录回调日志
		//void __log_iModel(xmEModelInterface eMi, const char* strFunction)
#define __log_iModel(eMi, strFunction)	\
		do{								\
			if (IsLogged(eMi))			\
				xmLog::info(m_strFlag, "Call model interface: %s", strFunction);	\
		} while (false)


		//	记录MRTC消息响应函数执行耗时
		void __log_ETS(const xmString& strFunction)
		{
			xmLog::info(m_strFlag, "%s execute time %.3fms.",
				strFunction.String(),
				xmRUN_TIME(MRTC_ETS));
		}
		static void __log_WLS(double fWLPrecent, void* pParam)
		{
			xmMrtc* pMrtc = (xmMrtc*)pParam;
			xmLog::info(pMrtc->m_strFlag, "Work load %.2f%% pre second.", pMrtc->WorkLoad());
		}

	private:
		//	调用模型的回调函数，要进行缓存保护和异常捕获
		//	出现错误后：只被记录到日志中，不中断UMSF运行
		xmRet iModel_OnLoad(const char* strDataFile);
		void  iModel_OnUnload(void);
		bool  iModel_IsReady(void);
		void  iModel_OnDrive(unsigned long uClick);
		void  iModel_OnTime(const xmATime& tWallTime, const xmATime& tSimuTime);
		//	下列回调函数，三个缓存均被UMSF进行了写保护
		//	控制命令响应回调函数
		void  iModel_OnStart(void);
		void  iModel_OnFinish(void);
		void  iModel_OnFreeze(void);
		void  iModel_OnUnfreeze(void);
		void  iModel_OnSpeed(int nSpeed);
		void  iModel_OnCommand(const xmString& cmdName, const xmValue& cmdParam);
		void  iModel_OnPrepareGroupInput(const char* strGroupName);
		void  iModel_OnFinishGroupInput(const char* strGroupName, int nErrorCount);
		void  iModel_OnPrepareGroupOutput(const char* strGroupName);
		void  iModel_OnFinishGroupOutput(const char* strGroupName, int nErrorCount);
		void  iModel_OnApplyGroupIO(const char* strGroupName, int nErrorCount);
		void  iModel_OnApplyDataIO(const char* strDataName, xmRet nErrorCode);
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_MRTC_H

