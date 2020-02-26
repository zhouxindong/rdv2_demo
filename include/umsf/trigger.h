#ifndef __SSA_UMSF_TRIGGER_H
#define __SSA_UMSF_TRIGGER_H
#include "isystem.h"
#include "tscript.h"
#include <map>

namespace ssa
{
	class xmRuntimeGroup;

	//	Trigger Message Code，描述Trigger被触发后，发送给xmMrtc的消息定义，自动进行组的IO
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
		xmRuntimeGroup* const m_pGroup;		//	哪个数据组进行IO计算
		xmDECLARE_MESSAGE_CLASS(tmC_TRIGGER);
	};
	xmDECLARE_MESSAGE_FUNC(tmTriggerFunc, tmTrigger);


	//	描述Trigger被触发后，将要给谁发送消息
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
		xmMsgHandler* const m_pMrtc;		//	哪个模型运行环境
		xmRuntimeGroup* const m_pGroup;		//	哪个模型数据组
	};

	//	触发器，由xmModelTrigger生成，附带了运行信息
	//	每一个数据组定义的触发器，都会生成一个触发器，
	//	多个触发器可以共同使用一个触发器容器
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


	//	触发器容器，每个触发器容器使用独立线程工作，监视触发条件
	//	触发器容器均由Manager创建和使用，创建触发器需要使用xmTrigger，
	//	一个触发器容器可以包含多个触发器
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

	//	模型驱动节拍Trigger Message Code
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
		//	当前节拍数
		const size_t m_uClickCount;
		//	当前系统时间
		const time_t m_uSystemTime;
		//	驱动类型
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

		//	添加一个时间触发，uClickCycle表示网络节拍的周期，单位：ms。
		void AddTrigger(xmTrigger* pTrigger, unsigned int uClickCycle);

	public:
		//	获取一个节拍触发器节拍触发的个数，单位：个
		size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}
		//	获取一个节拍触发器在模型中描述的触发周期，单位：ms
		unsigned int GetTriggerTime(size_t uPos) const;
		//	获取一个节拍触发器在系统中实际的触发周期，单位：节拍数
		unsigned int GetTriggerClick(size_t uPos) const
		{
			return m_vTrigger[uPos]->first;
		}
		//	获取一个节拍触发器描述
		const xmTrigger* GetTrigger(size_t uPos) const
		{
			return m_vTrigger[uPos]->second;
		}

		//	启动工作负载统计
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	消息响应函数执行时间
		void ExecuteTimeStatistics(size_t uPos, bool bDo)
		{
			m_bExecuteTime[uPos] = bDo;
		}
		double ExecuteTime(size_t uPos)
		{
			return m_fExecuteTime[uPos];
		}
	private:
		//	模型消息响应的处理时间，单位：毫秒
		std::vector<bool>   m_bExecuteTime;
		std::vector<double> m_fExecuteTime;

		//	m_mapDescribe中的key表示节拍数
		std::multimap<unsigned int, xmTrigger*> m_mapTrigger;
		std::vector<std::multimap<unsigned int, xmTrigger*>::iterator> m_vTrigger;

		//	记录消息响应函数执行耗时
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
		//	获取一个节拍触发器节拍触发的个数，单位：个
		size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}

		unsigned int GetTriggerEvent(size_t uPos) const
		{
			return m_vTrigger[uPos]->first;
		}
		//	获取触发器参数
		const xmValue& GetTriggerParam(size_t uPos) const;

		const xmTrigger* GetTrigger(size_t uPos) const
		{
			return m_vTrigger[uPos]->second.m_pTrigger;
		}

		//	启动工作负载统计
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	消息响应函数执行时间
		void ExecuteTimeStatistics(size_t uPos, bool bDo)
		{
			m_bExecuteTime[uPos] = bDo;
		}
		double ExecuteTime(size_t uPos)
		{
			return m_fExecuteTime[uPos];
		}
	private:
		//	模型消息响应的处理时间，单位：毫秒
		std::vector<bool>   m_bExecuteTime;
		std::vector<double> m_fExecuteTime;
		//	记录消息响应函数执行耗时
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
		//	敏感数据在模型中的名称
		const xmString& GetDataName() const;
		//	敏感数据在仿真系统中的匹配名称
		const xmString& GetMatchName() const;


		const xmTrigger* GetTrigger() const
		{
			return m_pTrigger;
		}
		//	获取触发器参数
		const xmValue& GetTriggerParam() const
		{
			return m_Value;
		}

		//	启动工作负载统计
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}

		//	消息响应函数执行时间
		void ExecuteTimeStatistics(bool bDo)
		{
			m_bExecuteTime = bDo;
		}
		double ExecuteTime()
		{
			return m_fExecuteTime;
		}
	private:
		//	模型消息响应的处理时间，单位：毫秒
		bool   m_bExecuteTime;
		double m_fExecuteTime;
		//	记录消息响应函数执行耗时
		void __log_ETS(const xmValue& aValue);
		static void __log_WLS(double fWLPrecent, void* pParam);
	private:
		xmTrigger* m_pTrigger;
		//const xmString m_Name;	//	数据在仿真系统中的名称
		const xmValue m_Value;	//	敏感数据的敏感值

		bool __OnDataUpdate(const xmValue& aValue);
		void OnDataUpdate(const smDataUpdate& aMsg);
		xmCREATE_MESSAGE_MAP(xmDataTriggerContainer);
	};
}

#endif