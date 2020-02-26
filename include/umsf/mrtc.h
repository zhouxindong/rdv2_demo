#ifndef __SSA_UMSF_MRTC_H
#define __SSA_UMSF_MRTC_H
#include "publicres.h"
#include "mdi.h"
#include "rtdata.h"
#include "rtgroup.h"
#include "mloader.h"
#include "mserver.h"
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
    class xmMrtc : public xmMsgHandler
    {
		friend xmTriggerScript;
		friend xmModelServer;
	public:
		//	strInstanceName参数，为NULL时，表示该模型仅有一个实例，实例名称存储在MRM中
		//	如果非NULL，则模型有多个实例，这个实例的名称是strInstanceName，所有他输出的参数都是
		//	被封装在该名称的数据类内
		//	bCombined参数表示是否该模型的所有实例都被封装在一个数据类以内。
		xmMrtc(xmPublicResource* pPubRes, const char* strModelName, xmMdi* pMdi, const char* strInstanceName);
		~xmMrtc(void);

		//	（一）根据MDI创建基本模型运行环境
		void Create();
		void Destroy();

		//	（二）根据model.config文件中的配置信息，创建模型最终运行环境
		//	（1）数据匹配信息
		//	设置数据匹配
		//	输入数据的matchName可以是数据路径
		//	输出数据的matchName只能是数据
		//	内部数据只能匹配pInitValue
		void SetDataMatch(xmRuntimeData* pData, const xmRuntimeData::SMatchInfo& matchInfo);
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
		void CreateInstance(const xmString& strModelFile, const xmString& strModelData);

		//	获取模型名称
		inline const xmString& ModelName() const
		{
			return m_strModelName;
		}

		//	获取模型实例名称
		inline const xmString& InstanceName() const
		{
			return m_strInstanceName;
		}

		//	获取模型中数据信息
		size_t GetDataCount() const;
		size_t GetDataCount(xmEIOType ioType) const;
		xmRuntimeData* GetData(const xmString& strDataName);
		xmRuntimeData* GetData(xmEIOType ioType, size_t uIndex);
		xmRuntimeData* GetData(xmEIOType ioType, const xmString& strDataName);
		//	获取数据数值，下列两个函数调用时，会使用MRTC对原数据进行保护
		xmRet GetDataValue(xmRuntimeData* pData, xmValue& cacheValue);
		xmRet GetDataSystemValue(xmRuntimeData* pData, xmValue& systemValue);

		//	获取模型组信息
		size_t GetGroupCount() const;
		const xmRuntimeGroup* GetGroup(const xmString& strDataName) const;
		const xmRuntimeGroup* GetGroup(size_t uIndex) const;
		xmRuntimeGroup* GetGroup(const xmString& strDataName)
		{
			return const_cast<xmRuntimeGroup*>(static_cast<const xmMrtc&>(*this).GetGroup(strDataName));
		}
		xmRuntimeGroup* GetGroup(size_t uIndex)
		{
			return const_cast<xmRuntimeGroup*>(static_cast<const xmMrtc&>(*this).GetGroup(uIndex));
		}
		//	复位模型实例组的激活状态，仅对ACTIVATE属性配置为auto的组有效。
		void ResetGroupEnable(bool bIsEnabled);

		//	打印数据信息
		xmString PrintData(const xmRuntimeData* pData = NULL) const;
		xmString PrintGroup(const xmRuntimeGroup* pGroup = NULL) const;


		//	模型数据服务
		xmIModelServer& ModelServer()
		{
			return *m_pModelServer;
		}

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
		double ExecuteTime(xmEInstanceFunction eFunc)
		{
			return (eFunc == IF_ALL || eFunc == IF_ERROR) ? 0.0 : m_fExecuteTime[eFunc];
		}
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
		xmModelServer* m_pModelServer;

		//	模型名称
		const xmString m_strModelName;
		//	模型配置信息
		xmMdi* const m_pMdi;
		//	模型运行实例加载器
		xmIModelLoader* m_pModelLoader;

		//	实例名称
		const xmString m_strInstanceName;
		//	当前网络节拍数，非实时，仅驱动后更新
		unsigned int m_uClickCount;
		//	模型驱动的节拍数
		unsigned int m_uDriveClick;
		//	当前系统墙时
		xmATime m_tWallTime;
		//	当前仿真时间
		xmATime m_tSimuTime;

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

		size_t _InitData(const xmMdi::t_DataPtrMap& vInitData, t_RtDataMap& mapData, xmBuffer& cache, xmEIOType iot);
		void _InitGroup(const xmModelGroup* pModelGroup, t_RtDataMap& mapData, xmRuntimeGroup* pGroup);

		xmRet _GroupIO(xmRuntimeGroup* pGroup, bool bLockData = true);
		xmRet _DataIO(xmRuntimeData* pData, bool bLockData = true);

		//	根据组的模型名称，获取该组的运行时对象
		static xmRuntimeGroup* _FindGroup(t_RtGroupMap& mapGroup, const xmString& strGroupName);
		//	在指定或整个数据表中，根据数据的模型名称和属性路径，获取该数据的运行时数值
		static xmValue _FindValue(const t_RtDataMap& mapData, const xmString& strDataPath);
		xmValue _FindValue(const xmString& strDataPath, xmEIOType& ioType) const;

		//	记录MRTC调用模型实例接口函数错误
		void __log_error_iModel(const xmString& strFunction, int nErrorID, const char* strErrorMsg)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(),	m_strInstanceName.String());
			xmLog::error(strTitle, "Call model interface %s failed(%d): %s",
				strFunction.String(),
				nErrorID,
				strErrorMsg);
		}
		//	记录模型数据组IO错误
		void __log_error_GroupIO(xmRuntimeGroup* pGroup, int nErrorID, const char* strErrorMsg)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(), m_strInstanceName.String());
			xmLog::error(strTitle, "Group %s %s failed(%d): %s",
				pGroup->ModelGroup().Name().String(),
				pGroup->IOType() == IOT_INPUT ? "input" : "output",
				nErrorID,
				strErrorMsg);
		}
		//	记录模型数据IO错误
		void __log_error_DataIO(xmRuntimeData* pData, int nErrorID, const char* strErrorMsg)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(), m_strInstanceName.String());
			xmLog::error(strTitle, "Data %s/%s %s failed(%d): %s",
				pData->DataName().String(),
				pData->SystemPath().String(),
				pData->IOType() == IOT_INPUT ? "input" : "output",
				nErrorID,
				strErrorMsg);
		}
		//	记录MRTC消息响应函数执行耗时
		void __log_ETS(const xmString& strFunction)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(), m_strInstanceName.String());
			xmLog::info(strTitle, "%s execute time %.3fms.",
				strFunction.String(),
				xmRUN_TIME(MRTC_ETS));
		}
		static void __log_WLS(double fWLPrecent, void* pParam)
		{
			xmMrtc* pMrtc = (xmMrtc*)pParam;
			xmString strTitle = xmStringUtil::Format("%s::%s", pMrtc->m_strModelName.String(), pMrtc->m_strInstanceName.String());
			xmLog::info(strTitle, "Work load %.2f%% pre second.", pMrtc->WorkLoad());
		}



	private:
		/*******************************************************************************
		供模型使用的接口，由xmModelServer调用
		*******************************************************************************/
		//	记录系统日志
		inline void iServer_Log(xmLog::ELogLevel eLevel, const char* strContent)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(), m_strInstanceName.String());
			xmLog::log(eLevel, strTitle, strContent);
		}

		//	获取仿真状态，开始，结束，冻结，解冻、快飞、回放
		inline unsigned int iServer_GetSimulateState(void) const
		{
			return m_pPublicRes->System()->GetSimulateState();
		};
		//	获取当前仿真倍速
		inline int iServer_GetSimulateSpeed(void) const
		{
			return m_pPublicRes->System()->GetSimulateSpeed();
		};
		//	获取当前由服务器统一的系统时间，单位：取决于系统
		inline time_t iServer_GetSystemTime(void) const
		{
			return m_pPublicRes->System()->GetSystemTime();
		};
		//	获取当前网络节拍数，单位：节拍个数
		inline size_t iServer_GetClickCount(void) const
		{
			return m_pPublicRes->System()->GetClickCount();
		};
		//	获取每个网络节拍的时间，由仿真系统决定，单位：毫秒
		inline unsigned int iServer_GetClickCycle(void) const
		{
			return m_pPublicRes->System()->GetClickCycle();
		};
		//	获取模型配置的驱动周期，此时间，不一定是模型实际驱动时间。单位：毫秒
		inline unsigned int iServer_GetDriveTime(void) const
		{
			return m_pMdi->DriveTime();
		};
		//	获取每个驱动周期的实际节拍数，单位：节拍数
		//	GetClickCycle() × GetDriveClick() = 实际驱动时间
		inline unsigned int iServer_GetDriveClick(void) const
		{
			return m_uDriveClick;
		};
		//	获取世界墙时
		inline const xmATime& iServer_GetWallTime(void) const
		{
			return m_tWallTime;
		};
		//	获取仿真时间
		inline const xmATime& iServer_GetSimuTime(void) const
		{
			return m_tSimuTime;
		};

		//	数据按照输入输出类型遍历，参数uPos表示第几个数据
		size_t iServer_InnerDataCount(void) const
		{
			return m_mapInnerData.Size();
		};
		const xmString& iServer_InnerDataName(size_t uPos) const
		{
			return m_mapInnerData.IndexKey(uPos);
		};
		const char* iServer_InnerDataAttr(size_t uPos, const char* strAttrName) const
		{
			return m_mapInnerData.IndexValue(uPos)->DataAttr(strAttrName);
		}
		const char* iServer_InnerDataAttr(const xmString& strDataName, const char* strAttrName) const
		{
			t_RtDataMap::t_cPos pos = m_mapInnerData.Find(strDataName);
			return (pos == m_mapInnerData.INVALID_POS()) ? NULL : m_mapInnerData.PosValue(pos)->DataAttr(strAttrName);
		}

		size_t iServer_InputDataCount(void) const
		{
			return m_mapInputData.Size();
		};
		const xmString& iServer_InputDataName(size_t uPos) const
		{
			return m_mapInputData.IndexKey(uPos);
		};
		const char* iServer_InputDataAttr(size_t uPos, const char* strAttrName) const
		{
			return m_mapInputData.IndexValue(uPos)->DataAttr(strAttrName);
		}
		const char* iServer_InputDataAttr(const xmString& strDataName, const char* strAttrName) const
		{
			t_RtDataMap::t_cPos pos = m_mapInputData.Find(strDataName);
			return (pos == m_mapInputData.INVALID_POS()) ? NULL : m_mapInnerData.PosValue(pos)->DataAttr(strAttrName);
		}

		size_t iServer_OutputDataCount(void) const
		{
			return m_mapOutputData.Size();
		};
		const xmString& iServer_OutputDataName(size_t uPos) const
		{
			return m_mapOutputData.IndexKey(uPos);
		};
		const char* iServer_OutputDataAttr(size_t uPos, const char* strAttrName) const
		{
			return m_mapOutputData.IndexValue(uPos)->DataAttr(strAttrName);
		}
		const char* iServer_OutputDataAttr(const xmString& strDataName, const char* strAttrName) const
		{
			t_RtDataMap::t_cPos pos = m_mapOutputData.Find(strDataName);
			return (pos == m_mapOutputData.INVALID_POS()) ? NULL : m_mapOutputData.PosValue(pos)->DataAttr(strAttrName);
		}

		//	数据按照标签遍历
		//	获取模型中所有Tag的名称，返回Tag数量
		size_t iServer_TagName(xmPtr<const char*[]>& vTagName) const;
		//	获取某一个Tag下的所有数据名称，返回数据数量
		size_t iServer_TagData(const xmString& strTagName, xmPtr<const char*[]>& vDataName) const;
		//	获取某一个数据下的所有标签名称，返回标签数量
		size_t iServer_DataTag(const xmString& strDataName, xmPtr<const char*[]>& vTagName) const;
		//	判断一个数据是否打上某个标签，如果strTagName为空，则判断这个数据是否打上标签
		bool iServer_IsDataTagged(const xmString& strDataName, const xmString& strTagName) const;


		//	供模型使用的数据访问接口，strDataPath参数，为数据的模型名称，也可以用符号“.”直接访问
		//	结构化数据中的属性，如:"天宫状态.位置信息.X轴"
		//	对于输入数据，如果模型改变其数值，当进行Input更新时，会被系统数据改变
		//	参数uPos表示第几个数据
		inline xmValue iServer_InnerData(const xmString& strDataPath) const
		{
			return _FindValue(m_mapInnerData, strDataPath);
		}
		inline xmValue iServer_InnerData(size_t uPos) const
		{
			return m_mapInnerData.IndexValue(uPos)->DataValue().PropertyValue();
		}
		inline xmValue iServer_InputData(const xmString& strDataPath) const
		{
			return _FindValue(m_mapInputData, strDataPath);
		}
		inline xmValue iServer_InputData(size_t uPos) const
		{
			return m_mapInputData.IndexValue(uPos)->DataValue().PropertyValue();
		}
		inline xmValue iServer_OutputData(const xmString& strDataPath) const
		{
			return _FindValue(m_mapOutputData, strDataPath);
		}
		inline xmValue iServer_OutputData(size_t uPos) const
		{
			return m_mapOutputData.IndexValue(uPos)->DataValue().PropertyValue();
		}
		//	供模型使用的数据访问接口，不区分内部、输入、输出数据，参数同上
		inline xmValue iServer_ModelData(const xmString& strDataPath, xmEIOType& ioType) const
		{
			return _FindValue(strDataPath, ioType);
		}
		inline xmValue iServer_ModelData(const xmString& strDataPath) const
		{
			xmEIOType ioType;
			return iServer_ModelData(strDataPath, ioType);
		}

		//	组遍历
		//	参数uPos表示第几个组，参数uIndex表示组中第几个数据
		size_t iServer_GroupCount(void) const
		{
			return m_mapGroup.Size();
		};
		const xmString& iServer_GroupName(size_t uPos) const
		{
			return m_mapGroup.IndexKey(uPos);
		};
		xmEIOType iServer_GroupType(size_t uPos) const
		{
			return m_mapGroup.IndexValue(uPos)->IOType();
		};
		xmEIOType iServer_GroupType(const xmString& strGroupName) const
		{
			t_RtGroupMap::t_cPos pos = m_mapGroup.Find(strGroupName);
			return (pos == m_mapGroup.INVALID_POS()) ? IOT_INNER : m_mapGroup.PosValue(pos)->IOType();
		};
		const char* iServer_GroupAttr(size_t uPos, const char* strAttrName) const
		{
			return m_mapGroup.IndexValue(uPos)->Attribute(strAttrName);
		}
		const char* iServer_GroupAttr(const xmString& strGroupName, const char* strAttrName) const
		{
			t_RtGroupMap::t_cPos pos = m_mapGroup.Find(strGroupName);
			return (pos == m_mapGroup.INVALID_POS()) ? NULL : m_mapGroup.PosValue(pos)->Attribute(strAttrName);
		}
		size_t iServer_GroupData(size_t uPos, xmPtr<const char*[]>& vDataName) const;
		size_t iServer_GroupData(const xmString& strGroupName, xmPtr<const char*[]>& vDataName) const;
		//	获取和设置数据组的状态
		bool  iServer_GetGroupEnabled(size_t uPos) const
		{
			return m_mapGroup.IndexValue(uPos)->IsEnabled();
		}
		bool  iServer_GetGroupEnabled(const xmString& strGroupName) const
		{
			t_RtGroupMap::t_cPos pos = m_mapGroup.Find(strGroupName);
			if (pos == m_mapGroup.INVALID_POS())
			{
				throw xmExIllegalInput();
			}
			return m_mapGroup.PosValue(pos)->IsEnabled();
		}
		xmRet iServer_SetGroupEnabled(size_t uPos, bool bIsEnabled)
		{
			m_mapGroup.IndexValue(uPos)->SetEnabled(bIsEnabled);
			return xmE_SUCCESS;
		}
		xmRet iServer_SetGroupEnabled(const xmString& strGroupName, bool bIsEnabled)
		{
			xmRuntimeGroup* pGroup = _FindGroup(m_mapGroup, strGroupName);
			if (pGroup == NULL)
			{
				return xmE_FAIL;
			}
			pGroup->SetEnabled(bIsEnabled);
			return xmE_SUCCESS;
		}

		//	模型申请进行IO，不受任何限制。IO为异步执行
		//	申请IO时，只能指定数据组或者数据，不能指定到数据属性
		xmRet iServer_ApplyGroupIO(const xmString& strGroupName);
		xmRet iServer_ApplyDataInput(const xmString& strDataName);
		xmRet iServer_ApplyDataOutput(const xmString& strDataName);
		//	模型手动进行IO，不受任何限制。IO为同步执行
		//	手动IO时，只能指定数据组或者数据，不能指定到数据属性
		xmRet iServer_ManualGroupIO(const xmString& strGroupName, bool bLockData = false);
		xmRet iServer_ManualDataInput(const xmString& strDataName, bool bLockData = false);
		xmRet iServer_ManualDataOutput(const xmString& strDataName, bool bLockData = false);

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
		void  iModel_OnFinishGroupInput(const char* strGroupName, bool isDone);
		void  iModel_OnPrepareGroupOutput(const char* strGroupName);
		void  iModel_OnFinishGroupOutput(const char* strGroupName, bool isDone);
		void  iModel_OnApplyGroupIO(const char* strGroupName, bool isDone);
		void  iModel_OnApplyDataIO(const char* strDataName, bool isDone);
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_MRTC_H

