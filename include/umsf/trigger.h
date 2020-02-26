#ifndef __SSA_UMSF_TRIGGER_H
#define __SSA_UMSF_TRIGGER_H
#include "isystem.h"
#include "tscript.h"
#include <map>

namespace ssa
{
	class xmRuntimeGroup;

	//	Trigger Message Code������Trigger�������󣬷��͸�xmMrtc����Ϣ���壬�Զ��������IO
	#define tmC_TRIGGER 0x2000
	class tmTrigger : public xmMessage
	{
	public:
		tmTrigger(xmRuntimeGroup* pGroup) : m_pGroup(pGroup)
		{
		};
		tmTrigger(const tmTrigger& msg) : m_pGroup(msg.m_pGroup)
		{
		};
		virtual ~tmTrigger(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new tmTrigger(*this);
		};
	public:
		xmRuntimeGroup* const m_pGroup;		//	�ĸ����������IO����
		xmDECLARE_MESSAGE_CLASS(tmC_TRIGGER);
	};
	xmDECLARE_MESSAGE_FUNC(tmTriggerFunc, tmTrigger);


	//	����Trigger�������󣬽�Ҫ��˭������Ϣ
	class xmTriggerTarget
	{
	public:
		xmTriggerTarget(xmMsgHandler* pMrtc, xmRuntimeGroup* pGroup) : m_pMrtc(pMrtc), m_pGroup(pGroup)
		{
			xmAssert(m_pMrtc != NULL);
			xmAssert(m_pGroup != NULL);
		};
		xmTriggerTarget(const xmTriggerTarget& target) : m_pMrtc(target.m_pMrtc), m_pGroup(target.m_pGroup)
		{
		};
		void Post()
		{
			m_pMrtc->PostMsg(tmTrigger(m_pGroup));
		}
		bool IsEnabled() const;
		xmEIOType IOType() const;
	private:
		xmMsgHandler* const m_pMrtc;		//	�ĸ�ģ�����л���
		xmRuntimeGroup* const m_pGroup;		//	�ĸ�ģ��������
	};

	//	����������xmModelTrigger���ɣ�������������Ϣ
	//	ÿһ�������鶨��Ĵ���������������һ����������
	//	������������Թ�ͬʹ��һ������������
	class xmModelTrigger;
	class xmTrigger
	{
	public:
		virtual ~xmTrigger();
		virtual xmEMsgHandleObject Type() const = 0;

		void PostTriggerMessgae(const xmValue* pParam)
		{
			try
			{
				if (IsScriptPass(pParam))
				{
					m_TriggerTarget.Post();
				}
			}
			catch (xmExLuaExecute& e)
			{
				xmString strTitle = xmStringUtil::Format("%s::%s::%s", 
					m_strModelName.String(), 
					m_strInstanceName.String(),
					m_strGroupName.String());
				xmLog::error(strTitle, "Trigger script execute failed(%d): %s",
					e.Code(),
					e.Message());
			}
		}
		bool IsTargetEnabled() const
		{
			return m_TriggerTarget.IsEnabled();
		}
		xmEIOType TargetIOType() const
		{
			return m_TriggerTarget.IOType();
		}

		bool IsScriptPass(const xmValue* pParam)
		{
			if (m_pScript == NULL)
			{
				return true;
			}
			return m_pScript->IsTriggered(pParam);
		}

		static xmTrigger* Create(const char* strModelName, const char* strInstanceName, const char* strGroupName, const xmModelTrigger* pModelTrigger, const xmTriggerTarget& target, xmMrtc* pMrtc);
		virtual void MatchData(const xmString& strMatchName) = 0;

		const xmString& PrintTrigger() const;
		const xmString& PrintParam() const;
		const xmString& PrintScript() const;

		const xmString m_strModelName;
		const xmString m_strInstanceName;
		const xmString m_strGroupName;
	protected:
		xmTrigger(const char* strModelName, const char* strInstanceName, const char* strGroupName, const xmModelTrigger* pModelTrigger, const xmTriggerTarget& target);

		const xmModelTrigger* const m_pModelTrigger;
		xmTriggerTarget m_TriggerTarget;
		xmTriggerScript* m_pScript;
	};


	//	������������ÿ������������ʹ�ö����̹߳��������Ӵ�������
	//	��������������Manager������ʹ�ã�������������Ҫʹ��xmTrigger��
	//	һ���������������԰������������
	class xmTriggerContainer
	{
	public:
		virtual xmEMsgHandleObject Type() const = 0;
	protected:
		xmTriggerContainer()
		{
			xmPREPARE_RUN(TRIGGER_ETS);
		};
		virtual ~xmTriggerContainer() {};
		xmCALC_RUNTIME(TRIGGER_ETS);
	};

	//	ģ����������Trigger Message Code
	#define tmC_DRIVE_CLICK 0x2100
	class tmDriveClick : public xmMessage
	{
	public:
		tmDriveClick(size_t uClickCount, time_t uSystemTime, xmEIOType eIOType) : 
			m_uClickCount(uClickCount), m_uSystemTime(uSystemTime), m_eIOType(eIOType)
		{
		};
		tmDriveClick(const tmDriveClick& msg) : 
			m_uClickCount(msg.m_uClickCount), m_uSystemTime(msg.m_uSystemTime), m_eIOType(msg.m_eIOType)
		{
		};
		virtual ~tmDriveClick(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new tmDriveClick(*this);
		};
	public:
		//	��ǰ������
		const size_t m_uClickCount;
		//	��ǰϵͳʱ��
		const time_t m_uSystemTime;
		//	��������
		const xmEIOType m_eIOType;
		xmDECLARE_MESSAGE_CLASS(tmC_DRIVE_CLICK);
	};
	xmDECLARE_MESSAGE_FUNC(tmDriveClickFunc, tmDriveClick);


	class xmTimeTriggerContainer : public xmMsgHandler, public xmTriggerContainer
	{
	public:
		xmTimeTriggerContainer() : xmTriggerContainer() {};
		virtual ~xmTimeTriggerContainer()
		{
			xmMH_DESTRUCT_IMMEDIATELY();	// or xmMH_DESTRUCT_WAIT_IDLE();
		};
		virtual xmEMsgHandleObject Type() const
		{
			return MHO_TIME_TRIGGER;
		}

		//	���һ��ʱ�䴥����uClickCycle��ʾ������ĵ����ڣ���λ��ms��
		void AddTrigger(xmTrigger* pTrigger, unsigned int uClickCycle);

	public:
		//	��ȡһ�����Ĵ��������Ĵ����ĸ�������λ����
		size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}
		//	��ȡһ�����Ĵ�������ģ���������Ĵ������ڣ���λ��ms
		unsigned int GetTriggerTime(size_t uPos) const;
		//	��ȡһ�����Ĵ�������ϵͳ��ʵ�ʵĴ������ڣ���λ��������
		unsigned int GetTriggerClick(size_t uPos) const
		{
			return m_vTrigger[uPos]->first;
		}
		//	��ȡһ�����Ĵ���������
		const xmTrigger* GetTrigger(size_t uPos) const
		{
			return m_vTrigger[uPos]->second;
		}

		//	������������ͳ��
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	��Ϣ��Ӧ����ִ��ʱ��
		void ExecuteTimeStatistics(size_t uPos, bool bDo)
		{
			m_bExecuteTime[uPos] = bDo;
		}
		double ExecuteTime(size_t uPos)
		{
			return m_fExecuteTime[uPos];
		}
	private:
		//	ģ����Ϣ��Ӧ�Ĵ���ʱ�䣬��λ������
		std::vector<bool>   m_bExecuteTime;
		std::vector<double> m_fExecuteTime;

		//	m_mapDescribe�е�key��ʾ������
		std::multimap<unsigned int, xmTrigger*> m_mapTrigger;
		std::vector<std::multimap<unsigned int, xmTrigger*>::iterator> m_vTrigger;

		//	��¼��Ϣ��Ӧ����ִ�к�ʱ
		void __log_ETS(size_t uPos, size_t uClickCount);
		static void __log_WLS(double fWLPrecent, void* pParam)
		{
			xmLog::info("TimeTrigger", "Work load %.2f%% pre second.", ((xmMsgHandler*)pParam)->WorkLoad());
		}
	private:
		void __OnDriveClick(const tmDriveClick& aMsg, std::multimap<unsigned int, xmTrigger*>::iterator& pos);
		void OnDriveClick(const tmDriveClick& aMsg);
		xmCREATE_MESSAGE_MAP(xmTimeTriggerContainer);
	};
	
	class xmEventTriggerContainer : public xmMsgHandler, public xmTriggerContainer
	{
	public:
		xmEventTriggerContainer() : xmTriggerContainer(){};
		virtual ~xmEventTriggerContainer() {};
		virtual xmEMsgHandleObject Type() const
		{
			return MHO_EVENT_TRIGGER;
		}

		void AddTrigger(xmTrigger* pTrigger);

	public:
		//	��ȡһ�����Ĵ��������Ĵ����ĸ�������λ����
		size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}

		unsigned int GetTriggerEvent(size_t uPos) const
		{
			return m_vTrigger[uPos]->first;
		}
		//	��ȡ����������
		const xmValue& GetTriggerParam(size_t uPos) const;

		const xmTrigger* GetTrigger(size_t uPos) const
		{
			return m_vTrigger[uPos]->second.m_pTrigger;
		}

		//	������������ͳ��
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	��Ϣ��Ӧ����ִ��ʱ��
		void ExecuteTimeStatistics(size_t uPos, bool bDo)
		{
			m_bExecuteTime[uPos] = bDo;
		}
		double ExecuteTime(size_t uPos)
		{
			return m_fExecuteTime[uPos];
		}
	private:
		//	ģ����Ϣ��Ӧ�Ĵ���ʱ�䣬��λ������
		std::vector<bool>   m_bExecuteTime;
		std::vector<double> m_fExecuteTime;
		//	��¼��Ϣ��Ӧ����ִ�к�ʱ
		void __log_ETS(size_t uPos, unsigned int uEvent);
		static void __log_WLS(double fWLPrecent, void* pParam)
		{
			xmLog::info("EventTrigger", "Work load %.2f%% pre second.", ((xmMsgHandler*)pParam)->WorkLoad());
		}
	private:
		typedef struct __tagTdInfo
		{
			__tagTdInfo(xmTrigger* pTrigger, size_t uPos) : m_pTrigger(pTrigger), m_uPos(uPos) {};
			xmTrigger* m_pTrigger;
			size_t m_uPos;
		}STdInfo;
		typedef std::multimap<unsigned int, STdInfo>::iterator t_TargetMapIter;
		std::multimap<unsigned int, STdInfo> m_mapTrigger;
		std::vector<t_TargetMapIter> m_vTrigger;

	private:
		void OnSystemEvent(const smSystemEvent& aMsg)
		{
			OnEvent(smC_SYSTEM_EVENT | aMsg.m_eType, aMsg.m_strName, aMsg.m_vuParam);
		}
		void OnCtrlCommand(const smCtrlCommand& aMsg)
		{
			OnEvent(smC_CTRL_COMMAND | aMsg.m_eType, aMsg.m_strName, aMsg.m_vuParam);
		}
		xmCREATE_MESSAGE_MAP(xmEventTriggerContainer);
	private:
		void OnEvent(unsigned int uEvent, const xmString& strName, const xmValue& vuParam);
	};

	class xmDataTriggerContainer : public xmMsgHandler, public xmTriggerContainer
	{
	public:
		xmDataTriggerContainer(xmTrigger* pTrigger);
		virtual ~xmDataTriggerContainer()
		{
			xmMH_DESTRUCT_IMMEDIATELY();	// or xmMH_DESTRUCT_WAIT_IDLE();
		};

		virtual xmEMsgHandleObject Type() const
		{
			return MHO_DATA_TRIGGER;
		}
		//	����������ģ���е�����
		const xmString& GetDataName() const;
		//	���������ڷ���ϵͳ�е�ƥ������
		const xmString& GetMatchName() const;


		const xmTrigger* GetTrigger() const
		{
			return m_pTrigger;
		}
		//	��ȡ����������
		const xmValue& GetTriggerParam() const
		{
			return m_Value;
		}

		//	������������ͳ��
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	��Ϣ��Ӧ����ִ��ʱ��
		void ExecuteTimeStatistics(bool bDo)
		{
			m_bExecuteTime = bDo;
		}
		double ExecuteTime()
		{
			return m_fExecuteTime;
		}
	private:
		//	ģ����Ϣ��Ӧ�Ĵ���ʱ�䣬��λ������
		bool   m_bExecuteTime;
		double m_fExecuteTime;
		//	��¼��Ϣ��Ӧ����ִ�к�ʱ
		void __log_ETS(const xmValue& aValue);
		static void __log_WLS(double fWLPrecent, void* pParam);
	private:
		xmTrigger* m_pTrigger;
		//const xmString m_Name;	//	�����ڷ���ϵͳ�е�����
		const xmValue m_Value;	//	�������ݵ�����ֵ

		bool __OnDataUpdate(const xmValue& aValue);
		void OnDataUpdate(const smDataUpdate& aMsg);
		xmCREATE_MESSAGE_MAP(xmDataTriggerContainer);
	};
}

#endif