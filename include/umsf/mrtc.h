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
		//	strInstanceName������ΪNULLʱ����ʾ��ģ�ͽ���һ��ʵ����ʵ�����ƴ洢��MRM��
		//	�����NULL����ģ���ж��ʵ�������ʵ����������strInstanceName������������Ĳ�������
		//	����װ�ڸ����Ƶ���������
		//	bCombined������ʾ�Ƿ��ģ�͵�����ʵ��������װ��һ�����������ڡ�
		xmMrtc(xmPublicResource* pPubRes, const xmMdi* pMdi, const xmMui* pMui, const char* strInstanceCalculationFile);
		~xmMrtc(void);

		//	��һ������MDI��������ģ�����л���
		void Create();
		void Destroy();
		//	ģ�͵�����
		inline xmModelScheduler* ModelSchedule()
		{
			return m_pModelSchedule;
		}
		//	ģ�����ݷ���
		inline xmIModelServer* ModelServer()
		{
			return m_pModelServer;
		}

		//	����������model.config�ļ��е�������Ϣ������ģ���������л���
		//	��1������ƥ����Ϣ
		//	��������ƥ��
		//	�������ݵ�matchName����������·��
		//	������ݵ�matchNameֻ��������
		//	�ڲ�����ֻ��ƥ��pInitValue
		void DataMatch();
		void DataMatch(xmRuntimeData* pData, const xmRuntimeData::SMatchInfo& matchInfo);

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
		//	bIsReadyToGo������ʾ��ʵ���������Ƿ�ʼ������Ϣ������UMSF��ʼ���ع�������Ҫ����Ϊfalse��
		//	�������й����е����д���ʵ�����̣���Ӧ�ô���ʵ����������ʼ������Ϣ��
		void CreateInstance(bool bIsReadyToGo = true);
		void DestroyInstance();
		void RecreateInstance();
		void ReadyToGo();
		bool IsActive() const;
		bool IsAutoRun() const
		{
			return m_pMui->IsAutoRun();
		}
		//	���Ե�ģ��ʵ���Ƿ�����������������������ģ�鷢��ָ����Ϣ
		bool TestActiveAndPostMessage(const xmMessage& aMessage);

		//	��ȡģ������
		inline const xmString& ModelName() const
		{
			return m_pMui->ModelName();
		}

		//	��ȡģ��ʵ������
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

		//	��ȡģ����������Ϣ
		size_t GetDataCount() const;
		xmRuntimeData* GetData(const xmString& strDataName);
		size_t GetDataCount(xmEIOType ioType) const;
		xmRuntimeData* GetData(xmEIOType ioType, size_t uIndex);
		xmRuntimeData* GetData(xmEIOType ioType, const xmString& strDataName);

		size_t GetGroupCount() const;
		xmRuntimeGroup* GetGroup(size_t uIndex);
		xmRuntimeGroup* GetGroup(const xmString& strGroupName);

		//	��λģ��ʵ����ļ���״̬������ACTIVATE��������Ϊauto������Ч��
		void ResetGroupEnable(bool bIsEnabled);

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
		bool IsExecuteTimeStatistics(xmEInstanceFunction eFunc)
		{
			return (eFunc == IF_ALL || eFunc == IF_ERROR) ? false : m_bExecuteTime[eFunc];
		}
		double ExecuteTime(xmEInstanceFunction eFunc)
		{
			return (eFunc == IF_ALL || eFunc == IF_ERROR) ? 0.0 : m_fExecuteTime[eFunc];
		}

		//	��־�ӿ�
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
		xmPtr<xmModelServer> m_pModelServer;
		//	���Ƚӿ�
		xmPtr<xmModelScheduler> m_pModelSchedule;

		//	ģ��������Ϣ
		const xmMdi* const m_pMdi;
		//	ģ��ʹ����Ϣ
		xmPtr<const xmMui> m_pMui;
		//	ģ������ʵ��������
		xmIModelLoader* m_pModelLoader;
		//	ʵ�������ļ�����
		const xmString m_strInstanceCalculationFile;

		//	��ǰ�������������ʵʱ�������������
		unsigned int m_uClickCount;
		//	ģ�������Ľ�����
		unsigned int m_uDriveClick;
		//	��ǰϵͳǽʱ
		xmATime m_tWallTime;
		//	��ǰ����ʱ��
		xmATime m_tSimuTime;
		//	������ɾ��ʵ��ʱ�ı���
		mutable xmMutex m_IntanceMutex;

		//	ʵ����ǣ�����������־�ı���
		const xmString m_strFlag;

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

		void _CreateInstance(bool bIsReadyToGo);
		void _DestroyInstance();

		size_t _InitData(const xmMdi::t_DataPtrMap& vInitData, t_RtDataMap& mapData, xmBuffer& cache, xmEIOType iot);
		void _InitGroup(const xmModelGroup* pModelGroup, t_RtDataMap& mapData, xmRuntimeGroup* pGroup);

		int _GroupIO(xmRuntimeGroup* pGroup, bool bLockData = true);
		xmRet _DataIO(xmRuntimeData* pData, bool bLockData = true);
		xmRet _PreIO(xmRuntimeData* pData, xmValue& aValue, bool bLockData = true);

		//	�������ģ�����ƣ���ȡ���������ʱ����
		static xmRuntimeGroup* _FindGroup(t_RtGroupMap& mapGroup, const xmString& strGroupName);
		//	��ָ�����������ݱ��У��������ݵ�ģ�����ƺ�����·������ȡ�����ݵ�����ʱ��ֵ
		static xmValue _FindValue(const t_RtDataMap& mapData, const xmString& strDataPath);
		xmValue _FindValue(const xmString& strDataPath, xmEIOType& ioType) const;


		//	��¼MRTC����ģ��ʵ���ӿں�������
		//	��������������֮���Զ���Ϊ�꣬�����Ǻ�������Ϊ�˱������жϽ������ǰ���������õ��ַ������졣
		//void __log_error_iModel(const char* strFunction, int nErrorID, const char* strErrorMsg)
#define __log_error_iModel(strFunction, nErrorID, strErrorMsg)	\
		do{														\
			if (nErrorID != xmE_SUCCESS)						\
				xmLog::error(m_strFlag, "Call model interface %s failed(%d): %s", strFunction, nErrorID, strErrorMsg);\
		} while (false)
		//	��¼�ص���־
		//void __log_iModel(xmEModelInterface eMi, const char* strFunction)
#define __log_iModel(eMi, strFunction)	\
		do{								\
			if (IsLogged(eMi))			\
				xmLog::info(m_strFlag, "Call model interface: %s", strFunction);	\
		} while (false)


		//	��¼MRTC��Ϣ��Ӧ����ִ�к�ʱ
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
		void  iModel_OnFinishGroupInput(const char* strGroupName, int nErrorCount);
		void  iModel_OnPrepareGroupOutput(const char* strGroupName);
		void  iModel_OnFinishGroupOutput(const char* strGroupName, int nErrorCount);
		void  iModel_OnApplyGroupIO(const char* strGroupName, int nErrorCount);
		void  iModel_OnApplyDataIO(const char* strDataName, xmRet nErrorCode);
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_MRTC_H

