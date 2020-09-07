#ifndef __SSA_UMSF_TRIGGER_H
#define __SSA_UMSF_TRIGGER_H
#include "isystem.h"
#include "mtrigger.h"
#include "tscript.h"
#include "rtobject.h"
#include <map>

namespace ssa
{
	class xmMrtc;
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


	//	触发器，由xmModelTrigger生成，附带了运行信息
	//	每一个数据组定义的触发器，都会生成一个触发器，
	//	多个触发器可以共同使用一个触发器容器
	class xmModelTrigger;
	class xmTrigger : public xmRuntimeObject
	{
	public:
		virtual ~xmTrigger();
		virtual const xmString& Name() const;
	protected:
		virtual void _MakeFlag() const;

	public:
		static xmTrigger* Create(xmMrtc* pMrtc, xmRuntimeGroup* pGroup, const xmModelTrigger* pModelTrigger);
		virtual xmETriggerType Type() const = 0;
		virtual const xmModelTrigger* ModelTrigger() const = 0;

		bool IsTargetEnabled() const;
		xmEIOType TargetIOType() const;
		bool IsScriptPass(const xmValue* pParam);
		void PostTriggerMessgae(const xmValue* pParam);

		const char* PrintType() const;
		const xmString& PrintTrigger() const;
		const xmString& PrintParam() const;
		const xmString& PrintScript() const;

		xmRet GetLastErrorCode_Trigger() const
		{
			return m_nLEC_Trigger;
		}
		const xmString& GetLastErrorMessage_Trigger() const
		{
			return m_strLEM_Trigger;
		}
	protected:
		xmTrigger(xmMrtc* pMrtc, xmRuntimeGroup* pGroup);
		xmRuntimeGroup* const m_pGroup;		//	哪个模型数据组
		xmTriggerScript* m_pScript;

		//	last error code
		xmRet m_nLEC_Trigger;
		xmString m_strLEM_Trigger;
	};
	class xmTimeTrigger : public xmTrigger
	{
	public:
		xmTimeTrigger(xmMrtc* pMrtc, xmRuntimeGroup* pGroup, const xmModelTrigger* pModelTrigger) :
			xmTrigger(pMrtc, pGroup),
			m_pModelTrigger(dynamic_cast<const xmTimeModelTrigger*>(pModelTrigger))
		{
		};
		virtual ~xmTimeTrigger()
		{
		};
		virtual const xmModelTrigger* ModelTrigger() const
		{
			return m_pModelTrigger;
		}
		virtual xmETriggerType Type() const
		{
			return m_pModelTrigger->Type();
		}
		//	单位：毫秒
		unsigned int Time() const
		{
			return m_pModelTrigger->m_uTime;
		}
		//	单位：节拍数
		unsigned int Click() const
		{
			return m_pModelTrigger->m_uClick;
		}
	protected:
		const xmTimeModelTrigger* const m_pModelTrigger;
	};
	class xmEventTrigger : public xmTrigger
	{
	public:
		xmEventTrigger(xmMrtc* pMrtc, xmRuntimeGroup* pGroup, const xmModelTrigger* pModelTrigger) :
			xmTrigger(pMrtc, pGroup),
			m_pModelTrigger(dynamic_cast<const xmEventModelTrigger*>(pModelTrigger))
		{
		};
		virtual ~xmEventTrigger()
		{
		};
		virtual const xmModelTrigger* ModelTrigger() const
		{
			return m_pModelTrigger;
		}
		virtual xmETriggerType Type() const
		{
			return m_pModelTrigger->Type();
		}

		smSystemEvent::ESEType Event() const
		{
			return m_pModelTrigger->m_eEvent;
		}
		const xmString& EventName() const
		{
			return m_pModelTrigger->m_Name;
		}
		const xmValue& EventParam() const
		{
			return m_pModelTrigger->m_Param;
		}
	protected:
		const xmEventModelTrigger* const m_pModelTrigger;
	};
	class xmCommandTrigger : public xmTrigger
	{
	public:
		xmCommandTrigger(xmMrtc* pMrtc, xmRuntimeGroup* pGroup, const xmModelTrigger* pModelTrigger) :
			xmTrigger(pMrtc, pGroup),
			m_pModelTrigger(dynamic_cast<const xmCommandModelTrigger*>(pModelTrigger))
		{
		};
		virtual ~xmCommandTrigger()
		{
		};
		virtual const xmModelTrigger* ModelTrigger() const
		{
			return m_pModelTrigger;
		}
		virtual xmETriggerType Type() const
		{
			return m_pModelTrigger->Type();
		}

		smCtrlCommand::ECCType Command() const
		{
			return m_pModelTrigger->m_eCommand;
		}
		const xmString& CommandName() const
		{
			return m_pModelTrigger->m_Name;
		}
		const xmValue& CommandParam() const
		{
			return m_pModelTrigger->m_Param;
		}
	protected:
		const xmCommandModelTrigger* const m_pModelTrigger;
	};
	class xmDataTrigger : public xmTrigger
	{
	public:
		xmDataTrigger(xmMrtc* pMrtc, xmRuntimeGroup* pGroup, const xmModelTrigger* pModelTrigger);
		virtual ~xmDataTrigger()
		{
		};
		virtual const xmModelTrigger* ModelTrigger() const
		{
			return m_pModelTrigger;
		}
		virtual xmETriggerType Type() const
		{
			return m_pModelTrigger->Type();
		}
		void MatchData(const xmString& strMatchName);

		const xmString& DataName() const
		{
			return m_pModelTrigger->m_Name;
		}
		const xmString& MatchName() const
		{
			return m_strMatchName;
		}
		//	如果数据被匹配到一个属性，则此处表示这个属性的名称
		const xmString& PropertyName() const
		{
			return m_strPropertyName;
		}
		const xmValue& ParamValue() const
		{
			return m_pModelTrigger->m_Value;
		}
		bool iProcessor_System2Model() const;
		xmRet iProcessor_System2Model(const xmValue& systemValue, xmValue& modelValue);
	protected:
		const xmDataModelTrigger* const m_pModelTrigger;
	private:
		xmString m_strMatchName;
		xmRuntimeData* const m_pRtData;
		xmString m_strPropertyName;
	};



	//	触发器容器，每个触发器容器使用独立线程工作，监视触发条件
	//	触发器容器均由Manager创建和使用，创建触发器需要使用xmTrigger，
	//	一个触发器容器可以包含多个触发器
	class xmTriggerContainer
	{
	public:
		xmTriggerContainer() {};
		virtual ~xmTriggerContainer() {};
		//	添加触发器
		virtual void AddTrigger(xmTrigger* pTrigger) = 0;
		//	获取触发器类型
		virtual xmETriggerType Type() const = 0;
		//	获取此容器中的触发器数量
		virtual size_t GetTriggerCount() const = 0;
		//	获取此容器中的某个触发器
		virtual const xmTrigger* GetTrigger(size_t uPos) const = 0;
		//	进行触发器计算
		virtual void OnTrigger(const xmMessage& aMsg) = 0;
	};

	
	//	模型驱动节拍Trigger Message Code
	//	由于时钟驱动的组IO要区分输入组和输出组，因此重新定义了tmDriveClick消息
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
	class xmTimeTriggerContainer : public xmTriggerContainer
	{
	public:
		xmTimeTriggerContainer() : xmTriggerContainer() {};
		virtual ~xmTimeTriggerContainer() {};
		//	添加一个时间触发，wParam表示网络节拍的周期，单位：ms。
		virtual void AddTrigger(xmTrigger* pTrigger);
		virtual xmETriggerType Type() const
		{
			return TT_TIME_TRIGGER;
		}
		virtual size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}
		virtual const xmTrigger* GetTrigger(size_t uPos) const;
		virtual void OnTrigger(const xmMessage& aMsg);

	public:
		//	获取一个节拍触发器在模型中描述的触发周期，单位：ms
		unsigned int GetTriggerTime(size_t uPos) const;
		//	获取一个节拍触发器在系统中实际的触发周期，单位：节拍数
		unsigned int GetTriggerClick(size_t uPos) const
		{
			return m_vTrigger[uPos]->first;
		}

	private:
		typedef std::multimap<unsigned int, xmTimeTrigger*>::iterator t_TargetMapIter;
		std::multimap<unsigned int, xmTimeTrigger*> m_mapTrigger;
		std::vector<t_TargetMapIter> m_vTrigger;
	};
	

	class xmEventTriggerContainer : public xmTriggerContainer
	{
	public:
		xmEventTriggerContainer() : xmTriggerContainer(){};
		virtual ~xmEventTriggerContainer() {};
		virtual void AddTrigger(xmTrigger* pTrigger);
		virtual xmETriggerType Type() const
		{
			return TT_EVENT_TRIGGER;
		}
		virtual size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}
		virtual const xmTrigger* GetTrigger(size_t uPos) const;
		virtual void OnTrigger(const xmMessage& aMsg);

	public:
		smSystemEvent::ESEType GetTriggerEvent(size_t uPos) const
		{
			return m_vTrigger[uPos]->first;
		}
		//	获取触发器参数
		const xmValue& GetTriggerParam(size_t uPos) const;

	private:
		typedef std::multimap<smSystemEvent::ESEType, xmEventTrigger*>::iterator t_TargetMapIter;
		std::multimap<smSystemEvent::ESEType, xmEventTrigger*> m_mapTrigger;
		std::vector<t_TargetMapIter> m_vTrigger;
		void __OnEvent(smSystemEvent::ESEType eEvent, const xmString& strName, const xmValue& vuParam);
	};


	class xmCommandTriggerContainer : public xmTriggerContainer
	{
	public:
		xmCommandTriggerContainer() : xmTriggerContainer() {};
		virtual ~xmCommandTriggerContainer() {};
		virtual void AddTrigger(xmTrigger* pTrigger);
		virtual xmETriggerType Type() const
		{
			return TT_COMMAND_TRIGGER;
		}
		virtual size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}
		virtual const xmTrigger* GetTrigger(size_t uPos) const;
		virtual void OnTrigger(const xmMessage& aMsg);

	public:
		smCtrlCommand::ECCType GetTriggerCommand(size_t uPos) const
		{
			return m_vTrigger[uPos]->first;
		}
		//	获取触发器参数
		const xmValue& GetTriggerParam(size_t uPos) const;

	private:
		typedef std::multimap<smCtrlCommand::ECCType, xmCommandTrigger*>::iterator t_TargetMapIter;
		std::multimap<smCtrlCommand::ECCType, xmCommandTrigger*> m_mapTrigger;
		std::vector<t_TargetMapIter> m_vTrigger;
		void __OnCommand(smCtrlCommand::ECCType eCommand, const xmString& strName, const xmValue& vuParam);
	};


	class xmDataTriggerContainer : public xmTriggerContainer
	{
	public:
		xmDataTriggerContainer() : xmTriggerContainer() {};
		virtual ~xmDataTriggerContainer() {};
		virtual void AddTrigger(xmTrigger* pTrigger);
		virtual xmETriggerType Type() const
		{
			return TT_DATA_TRIGGER;
		}
		virtual size_t GetTriggerCount() const
		{
			return m_vTrigger.size();
		}
		virtual const xmTrigger* GetTrigger(size_t uPos) const;
		virtual void OnTrigger(const xmMessage& aMsg);

	public:
		//	敏感数据在模型中的名称
		const xmString& GetDataName(size_t uPos) const;
		//	敏感数据在仿真系统中的匹配名称
		const xmString& GetMatchName(size_t uPos) const;
		//	获取触发器参数
		const xmValue& GetTriggerParam(size_t uPos) const;

	private:
		typedef std::multimap<xmString, xmDataTrigger*>::iterator t_TargetMapIter;
		std::multimap<xmString, xmDataTrigger*> m_mapTrigger;
		std::vector<t_TargetMapIter> m_vTrigger;
	};
}

#endif