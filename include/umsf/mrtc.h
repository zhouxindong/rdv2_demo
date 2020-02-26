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
	//	ģ���ֶ���IOʱ����MRTC���͵���Ϣ��
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

	//	ģ���ֶ�����IOʱ����MRTC���͵���Ϣ��
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
    �����ƣ�xmMrtc
    ��  �ܣ�����Model Runtime Context
    ˵  ����MRTC����ģ���ڷ�������п���ʹ�õ���Դ��
		    ÿһ��ģ�;�����һ��MRTC��ģ�����еĽ�������ͨ��MRTC��
    *******************************************************************************/
    class xmMrtc : public xmMsgHandler
    {
		friend xmTriggerScript;
		friend xmModelServer;
	public:
		//	strInstanceName������ΪNULLʱ����ʾ��ģ�ͽ���һ��ʵ����ʵ�����ƴ洢��MRM��
		//	�����NULL����ģ���ж��ʵ�������ʵ����������strInstanceName������������Ĳ�������
		//	����װ�ڸ����Ƶ���������
		//	bCombined������ʾ�Ƿ��ģ�͵�����ʵ��������װ��һ�����������ڡ�
		xmMrtc(xmPublicResource* pPubRes, const char* strModelName, xmMdi* pMdi, const char* strInstanceName);
		~xmMrtc(void);

		//	��һ������MDI��������ģ�����л���
		void Create();
		void Destroy();

		//	����������model.config�ļ��е�������Ϣ������ģ���������л���
		//	��1������ƥ����Ϣ
		//	��������ƥ��
		//	�������ݵ�matchName����������·��
		//	������ݵ�matchNameֻ��������
		//	�ڲ�����ֻ��ƥ��pInitValue
		void SetDataMatch(xmRuntimeData* pData, const xmRuntimeData::SMatchInfo& matchInfo);
		//	��2������������ݵ����·��
		//	pData������Ŀǰֻ֧��ʵ�����������·�����ã����Ӧ����ΪNULL��
		void SetDataPath(xmRuntimeData* pData, const char* strDataPath);
		//	���ָ���������ϵͳ�����ݣ���ģ�͵�������ݰ���˳����ϳ�һ���µ�xmClass�ṹ
		//	����SetDataMatchǰ����ú����������ͬ
		//	��������Ѿ���ƥ�䣬����ƥ�������ݽ������
		//	����ʹ��ԭʼ���ݽ������
		//	vDataʹ��ģ����������
		xmClass* GetCombineClass(const std::vector<xmString>& vData, xmValue& combineValue);
		//	��3��ע�ᴥ������ֻ������ƥ��֮�󣬲���ע�ᴥ��������Ϊ��Ϊ��������ģ����������
		//	������ϵͳƥ���б��ı�Ϊ��������
		size_t RegisterTrigger();

		//	����������ʹ�ø�MRTC��ģ��ʵ��
		void CreateInstance(const xmString& strModelFile, const xmString& strModelData);

		//	��ȡģ������
		inline const xmString& ModelName() const
		{
			return m_strModelName;
		}

		//	��ȡģ��ʵ������
		inline const xmString& InstanceName() const
		{
			return m_strInstanceName;
		}

		//	��ȡģ����������Ϣ
		size_t GetDataCount() const;
		size_t GetDataCount(xmEIOType ioType) const;
		xmRuntimeData* GetData(const xmString& strDataName);
		xmRuntimeData* GetData(xmEIOType ioType, size_t uIndex);
		xmRuntimeData* GetData(xmEIOType ioType, const xmString& strDataName);
		//	��ȡ������ֵ������������������ʱ����ʹ��MRTC��ԭ���ݽ��б���
		xmRet GetDataValue(xmRuntimeData* pData, xmValue& cacheValue);
		xmRet GetDataSystemValue(xmRuntimeData* pData, xmValue& systemValue);

		//	��ȡģ������Ϣ
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
		//	��λģ��ʵ����ļ���״̬������ACTIVATE��������Ϊauto������Ч��
		void ResetGroupEnable(bool bIsEnabled);

		//	��ӡ������Ϣ
		xmString PrintData(const xmRuntimeData* pData = NULL) const;
		xmString PrintGroup(const xmRuntimeGroup* pGroup = NULL) const;


		//	ģ�����ݷ���
		xmIModelServer& ModelServer()
		{
			return *m_pModelServer;
		}

		//	���л����Լ�
		xmRet Inspect(xmString& strError);

		//	������������ͳ��
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	��Ϣ��Ӧ����ִ��ʱ��
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
		�������� Trigger �� Manager ����Ϣ
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

		//	ģ����Ϣ��Ӧ�Ĵ���ʱ�䣬��λ������
		xmCALC_RUNTIME(MRTC_ETS);
		bool   m_bExecuteTime[IF_ALL];
		double m_fExecuteTime[IF_ALL];

	private:
		typedef xmOrderMap<xmString, xmPtr<xmRuntimeData> > t_RtDataMap;
		typedef xmOrderMap<xmString, xmRuntimeData*> t_RtDataPtrMap;
		typedef xmOrderMap<xmString, xmPtr<xmRuntimeGroup> > t_RtGroupMap;

		//	������Դ
		xmPublicResource* const m_pPublicRes;
		//	Ϊģ���ṩ�ķ���ӿ�
		xmModelServer* m_pModelServer;

		//	ģ������
		const xmString m_strModelName;
		//	ģ��������Ϣ
		xmMdi* const m_pMdi;
		//	ģ������ʵ��������
		xmIModelLoader* m_pModelLoader;

		//	ʵ������
		const xmString m_strInstanceName;
		//	��ǰ�������������ʵʱ�������������
		unsigned int m_uClickCount;
		//	ģ�������Ľ�����
		unsigned int m_uDriveClick;
		//	��ǰϵͳǽʱ
		xmATime m_tWallTime;
		//	��ǰ����ʱ��
		xmATime m_tSimuTime;

		//	���ڲ������롢��������¼ģ������
		//	�����������е�key����ָģ��ʹ�õ���������
		xmBuffer m_InnerCache;
		xmBuffer m_InputCache;
		xmBuffer m_OutputCache;
		t_RtDataMap m_mapInnerData;
		t_RtDataMap m_mapInputData;
		t_RtDataMap m_mapOutputData;
		//	��¼ģ��ʹ�õ�ȫ������
		t_RtDataPtrMap m_mapModelData;
		//	��¼��ǩ�µ�ȫ�����ݣ�keyΪ��ǩ���ƣ�valueΪ�Ա������Ϊ˳�������������Ϣ
		xmOrderMap<xmString, std::map<unsigned int, xmRuntimeData*>> m_mapTagDataInfo;
		
		//	m_mapGroup��¼����IO����Ϣ������keyΪ������valueΪ�����
		t_RtGroupMap m_mapGroup;
		//	m_mapDataTriggerGroup��¼����ʹ�����ݴ�����������Ϣ��keyΪ��������ModelName��valueΪ�����
		std::multimap<xmString, xmRuntimeGroup*> m_mapDataTriggerGroup;

		//	���ݻ�����ʹ�û���Ķ�д������������ʱ���ݵ�ֵ���ڴ����ݻ�����
		mutable xmRWLock m_InnerCacheLock;
		mutable xmRWLock m_InputCacheLock;
		mutable xmRWLock m_OutputCacheLock;

		size_t _InitData(const xmMdi::t_DataPtrMap& vInitData, t_RtDataMap& mapData, xmBuffer& cache, xmEIOType iot);
		void _InitGroup(const xmModelGroup* pModelGroup, t_RtDataMap& mapData, xmRuntimeGroup* pGroup);

		xmRet _GroupIO(xmRuntimeGroup* pGroup, bool bLockData = true);
		xmRet _DataIO(xmRuntimeData* pData, bool bLockData = true);

		//	�������ģ�����ƣ���ȡ���������ʱ����
		static xmRuntimeGroup* _FindGroup(t_RtGroupMap& mapGroup, const xmString& strGroupName);
		//	��ָ�����������ݱ��У��������ݵ�ģ�����ƺ�����·������ȡ�����ݵ�����ʱ��ֵ
		static xmValue _FindValue(const t_RtDataMap& mapData, const xmString& strDataPath);
		xmValue _FindValue(const xmString& strDataPath, xmEIOType& ioType) const;

		//	��¼MRTC����ģ��ʵ���ӿں�������
		void __log_error_iModel(const xmString& strFunction, int nErrorID, const char* strErrorMsg)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(),	m_strInstanceName.String());
			xmLog::error(strTitle, "Call model interface %s failed(%d): %s",
				strFunction.String(),
				nErrorID,
				strErrorMsg);
		}
		//	��¼ģ��������IO����
		void __log_error_GroupIO(xmRuntimeGroup* pGroup, int nErrorID, const char* strErrorMsg)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(), m_strInstanceName.String());
			xmLog::error(strTitle, "Group %s %s failed(%d): %s",
				pGroup->ModelGroup().Name().String(),
				pGroup->IOType() == IOT_INPUT ? "input" : "output",
				nErrorID,
				strErrorMsg);
		}
		//	��¼ģ������IO����
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
		//	��¼MRTC��Ϣ��Ӧ����ִ�к�ʱ
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
		��ģ��ʹ�õĽӿڣ���xmModelServer����
		*******************************************************************************/
		//	��¼ϵͳ��־
		inline void iServer_Log(xmLog::ELogLevel eLevel, const char* strContent)
		{
			xmString strTitle = xmStringUtil::Format("%s::%s", m_strModelName.String(), m_strInstanceName.String());
			xmLog::log(eLevel, strTitle, strContent);
		}

		//	��ȡ����״̬����ʼ�����������ᣬ�ⶳ����ɡ��ط�
		inline unsigned int iServer_GetSimulateState(void) const
		{
			return m_pPublicRes->System()->GetSimulateState();
		};
		//	��ȡ��ǰ���汶��
		inline int iServer_GetSimulateSpeed(void) const
		{
			return m_pPublicRes->System()->GetSimulateSpeed();
		};
		//	��ȡ��ǰ�ɷ�����ͳһ��ϵͳʱ�䣬��λ��ȡ����ϵͳ
		inline time_t iServer_GetSystemTime(void) const
		{
			return m_pPublicRes->System()->GetSystemTime();
		};
		//	��ȡ��ǰ�������������λ�����ĸ���
		inline size_t iServer_GetClickCount(void) const
		{
			return m_pPublicRes->System()->GetClickCount();
		};
		//	��ȡÿ��������ĵ�ʱ�䣬�ɷ���ϵͳ��������λ������
		inline unsigned int iServer_GetClickCycle(void) const
		{
			return m_pPublicRes->System()->GetClickCycle();
		};
		//	��ȡģ�����õ��������ڣ���ʱ�䣬��һ����ģ��ʵ������ʱ�䡣��λ������
		inline unsigned int iServer_GetDriveTime(void) const
		{
			return m_pMdi->DriveTime();
		};
		//	��ȡÿ���������ڵ�ʵ�ʽ���������λ��������
		//	GetClickCycle() �� GetDriveClick() = ʵ������ʱ��
		inline unsigned int iServer_GetDriveClick(void) const
		{
			return m_uDriveClick;
		};
		//	��ȡ����ǽʱ
		inline const xmATime& iServer_GetWallTime(void) const
		{
			return m_tWallTime;
		};
		//	��ȡ����ʱ��
		inline const xmATime& iServer_GetSimuTime(void) const
		{
			return m_tSimuTime;
		};

		//	���ݰ�������������ͱ���������uPos��ʾ�ڼ�������
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

		//	���ݰ��ձ�ǩ����
		//	��ȡģ��������Tag�����ƣ�����Tag����
		size_t iServer_TagName(xmPtr<const char*[]>& vTagName) const;
		//	��ȡĳһ��Tag�µ������������ƣ�������������
		size_t iServer_TagData(const xmString& strTagName, xmPtr<const char*[]>& vDataName) const;
		//	��ȡĳһ�������µ����б�ǩ���ƣ����ر�ǩ����
		size_t iServer_DataTag(const xmString& strDataName, xmPtr<const char*[]>& vTagName) const;
		//	�ж�һ�������Ƿ����ĳ����ǩ�����strTagNameΪ�գ����ж���������Ƿ���ϱ�ǩ
		bool iServer_IsDataTagged(const xmString& strDataName, const xmString& strTagName) const;


		//	��ģ��ʹ�õ����ݷ��ʽӿڣ�strDataPath������Ϊ���ݵ�ģ�����ƣ�Ҳ�����÷��š�.��ֱ�ӷ���
		//	�ṹ�������е����ԣ���:"�칬״̬.λ����Ϣ.X��"
		//	�����������ݣ����ģ�͸ı�����ֵ��������Input����ʱ���ᱻϵͳ���ݸı�
		//	����uPos��ʾ�ڼ�������
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
		//	��ģ��ʹ�õ����ݷ��ʽӿڣ��������ڲ������롢������ݣ�����ͬ��
		inline xmValue iServer_ModelData(const xmString& strDataPath, xmEIOType& ioType) const
		{
			return _FindValue(strDataPath, ioType);
		}
		inline xmValue iServer_ModelData(const xmString& strDataPath) const
		{
			xmEIOType ioType;
			return iServer_ModelData(strDataPath, ioType);
		}

		//	�����
		//	����uPos��ʾ�ڼ����飬����uIndex��ʾ���еڼ�������
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
		//	��ȡ�������������״̬
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

		//	ģ���������IO�������κ����ơ�IOΪ�첽ִ��
		//	����IOʱ��ֻ��ָ��������������ݣ�����ָ������������
		xmRet iServer_ApplyGroupIO(const xmString& strGroupName);
		xmRet iServer_ApplyDataInput(const xmString& strDataName);
		xmRet iServer_ApplyDataOutput(const xmString& strDataName);
		//	ģ���ֶ�����IO�������κ����ơ�IOΪͬ��ִ��
		//	�ֶ�IOʱ��ֻ��ָ��������������ݣ�����ָ������������
		xmRet iServer_ManualGroupIO(const xmString& strGroupName, bool bLockData = false);
		xmRet iServer_ManualDataInput(const xmString& strDataName, bool bLockData = false);
		xmRet iServer_ManualDataOutput(const xmString& strDataName, bool bLockData = false);

	private:
		//	����ģ�͵Ļص�������Ҫ���л��汣�����쳣����
		//	���ִ����ֻ����¼����־�У����ж�UMSF����
		xmRet iModel_OnLoad(const char* strDataFile);
		void  iModel_OnUnload(void);
		bool  iModel_IsReady(void);
		void  iModel_OnDrive(unsigned long uClick);
		void  iModel_OnTime(const xmATime& tWallTime, const xmATime& tSimuTime);
		//	���лص������������������UMSF������д����
		//	����������Ӧ�ص�����
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

