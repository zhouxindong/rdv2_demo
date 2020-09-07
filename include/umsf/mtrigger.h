#ifndef __SSA_UMSF_MTRIGGER_H
#define __SSA_UMSF_MTRIGGER_H
#include "imessage.h"

namespace ssa
{
	//	定义触发器描述
	class xmUMSF;
	class xmModelTrigger
	{
		friend xmUMSF;
	public:
		xmModelTrigger() {};
		virtual ~xmModelTrigger() {};
		virtual xmETriggerType Type() const = 0;
		//	以XML格式打印整个触发器字段
		const xmString& PrintXML() const;
		//	仅打印触发器内容
		const xmString& PrintTrigger() const;
		//	仅打印触发器参数
		const xmString& PrintParam() const;

		//	触发器中附加的Lua脚本
		//	由于Js引擎太大了，因此触发器中只使用Lua脚本
		xmString m_strScript;
		//	脚本的绝对路径
		xmString m_strScriptAbsPath;

	protected:
		virtual xmString _PrintXML() const = 0;
		virtual xmString _PrintTrigger() const = 0;
		virtual xmString _PrintParam() const = 0;
	private:
		mutable xmString m_strPrintXML;
		mutable xmString m_strPrintTrigger;
		mutable xmString m_strPrintParam;
	};



	class xmTimeModelTrigger : public xmModelTrigger
	{
	public:
		xmTimeModelTrigger() : xmModelTrigger(), m_uTime(0), m_uClick(0)
		{
		};
		virtual ~xmTimeModelTrigger()
		{
		};
		virtual xmETriggerType Type() const
		{
			return TT_TIME_TRIGGER;
		}
	protected:
		virtual xmString _PrintXML() const;
		virtual xmString _PrintTrigger() const;
		virtual xmString _PrintParam() const;

	public:
		//	触发周期，来自于原始配置，单位：毫秒
		unsigned int m_uTime;
		//	触发周期，来至于计算结果，单位：节拍数
		unsigned int m_uClick;
	};

	class xmEventModelTrigger : public xmModelTrigger
	{
	public:
		xmEventModelTrigger() : xmModelTrigger()
		{
		};
		virtual ~xmEventModelTrigger()
		{
		};
		virtual xmETriggerType Type() const
		{
			return TT_EVENT_TRIGGER;
		}
	protected:
		virtual xmString _PrintXML() const;
		virtual xmString _PrintTrigger() const;
		virtual xmString _PrintParam() const;

	public:
		smSystemEvent::ESEType m_eEvent;
		//	表示触发事件的名称
		xmString m_Name;
		//	m_Param表示事件附带的参数
		//	smC_DATA_SYNC		：无类型，无参数
		//	smC_NODE_EVENT		：第0字节表示节点注册、注销等类型。参数为string类型，表示节点名称，如果为无效xmValue，表示处理任意节点
		//	smC_SYSTEM_EVENT	：第0字节表示系统状态前换等类型，无参数
		//	smC_SYSTEM_COMMAND	：第0字节表示系统训练开始、结束等类型。当为快飞命令时，参数为int类型，仿真倍率，如果为无效xmValue，表示处理数值
		xmValue  m_Param;
	};

	class xmCommandModelTrigger : public xmModelTrigger
	{
	public:
		xmCommandModelTrigger() : xmModelTrigger()
		{
		};
		virtual ~xmCommandModelTrigger()
		{
		};
		virtual xmETriggerType Type() const
		{
			return TT_COMMAND_TRIGGER;
		}
	protected:
		virtual xmString _PrintXML() const;
		virtual xmString _PrintTrigger() const;
		virtual xmString _PrintParam() const;

	public:
		smCtrlCommand::ECCType m_eCommand;
		//	表示触发事件的名称
		xmString m_Name;
		//	m_Param表示事件附带的参数
		//	smC_DATA_SYNC		：无类型，无参数
		//	smC_NODE_EVENT		：第0字节表示节点注册、注销等类型。参数为string类型，表示节点名称，如果为无效xmValue，表示处理任意节点
		//	smC_SYSTEM_EVENT	：第0字节表示系统状态前换等类型，无参数
		//	smC_SYSTEM_COMMAND	：第0字节表示系统训练开始、结束等类型。当为快飞命令时，参数为int类型，仿真倍率，如果为无效xmValue，表示处理数值
		xmValue  m_Param;
	};

	class xmDataModelTrigger : public xmModelTrigger
	{
	public:
		xmDataModelTrigger() : xmModelTrigger()
		{
		};
		virtual ~xmDataModelTrigger()
		{
		};
		virtual xmETriggerType Type() const
		{
			return TT_DATA_TRIGGER;
		}
	protected:
		virtual xmString _PrintXML() const;
		virtual xmString _PrintTrigger() const;
		virtual xmString _PrintParam() const;

	public:
		xmString m_Name;
		xmValue  m_Value;
	};
}
#endif  //  __SSA_UMSF_MTRIGGER_H

