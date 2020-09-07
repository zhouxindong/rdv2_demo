#ifndef __SSA_UMSF_IMESSAGE_H
#define __SSA_UMSF_IMESSAGE_H
#include "../base/base.h"
#include "idefine.h"

namespace ssa
{
	//	仿真系统能够发送给UMSF Manager的消息
	//	System Message Code
	typedef enum
	{
		smC_WORLD_CLICK  = 0x0100,	//	仿真系统节拍，参数：节拍数
		smC_SYSTEM_TIME  = 0x0110,	//	系统时间，包括世界墙时和仿真时间
		smC_DATA_UPDATE  = 0x0200,	//	数据更新，指某一单个数据数值被写入，参数：数据名称与数据值
		smC_BULLETIN_GET = 0x0300,	//	公告接收，获取了自己订阅的公告。参数：公告主题和公告内容。
		smC_MESSAGE_GET  = 0x0400,	//	消息接收，接收到系统发送的消息。参数：消息发送者和消息内容。
									//	公告与消息：
									//		1、公告是主动订阅，消息是被动接收。
									//		2、公告的主题可以是任意类型或内容，消息的发送者必须是节点名称
		smC_SYSTEM_EVENT = 0x1100,	//	系统事件，包括切换至正常状态、切换至回放状态、节点登入、登出等，其中节点事件可带参数：节点名称
		smC_CTRL_COMMAND = 0x1200,	//	控制指令，包括训练开始、训练结束、IO冻结、IO解冻、快飞等。其中快飞指令可以带参数，具体定义为：
									//		> 1：快飞，
									//		<-1：慢飞，
									//		= 1：正常飞行，
									//		= 0：暂停计算，
									//		=-1：未定义。
									//	xmLog::todo()下列仅定义，未实现：
									//	冻结和暂停计算的区别：冻结是既停止模型IO，也停止模型计算；暂停计算是停止模型计算，但不停止模型IO
									//	冻结实现：MRTC停止接收任何消息
									//	暂停计算：模型的OnDrive接口停止回调
									//	仿真倍率，目前不同飞行倍率，UMSF仅进行模型回调，自身不进行响应。
		smC_SYSTEM_LOG	 = 0x8000
	} smESystemMessageCode;

	//	所有系统消息均携带的信息，下列信息，均与仿真系统具体业务无关
	class smMessageBaseInfo
	{
	public:
		smMessageBaseInfo() : m_lWallTime(0), m_lClickCount(0) {};
		smMessageBaseInfo(long long lWallTime, long long lClickCount) : m_lWallTime(lWallTime), m_lClickCount(lClickCount) {};
		smMessageBaseInfo(const smMessageBaseInfo& mbi) : m_lWallTime(mbi.m_lWallTime), m_lClickCount(mbi.m_lClickCount) {};
		virtual ~smMessageBaseInfo() {};
	public:
		//	真实世界墙时，由1970.01.01 00:00:00.000到现在的豪秒数
		const long long m_lWallTime;
		//	节拍计数
		const long long m_lClickCount;
	};

	//	仿真系统节拍，目前仅包含消息基础信息
	class smWorldClick : public xmMessage, public smMessageBaseInfo
	{
	public:
		smWorldClick() {};
		smWorldClick(const smMessageBaseInfo& mbi) : smMessageBaseInfo(mbi)
		{
		};
		smWorldClick(const smWorldClick& msg) : smMessageBaseInfo(msg)
		{
		};
		virtual ~smWorldClick(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new smWorldClick(*this);
		};
	public:
		xmDECLARE_MESSAGE_CLASS(smC_WORLD_CLICK);
	};
	xmDECLARE_MESSAGE_FUNC(smWorldClickFunc, smWorldClick);

	//	系统时间，改时间与仿真系统具体业务有关
	class smSystemTime : public xmMessage, public smMessageBaseInfo
	{
	public:
		smSystemTime(xmATime tSystemTime) : m_tSystemTime(tSystemTime)
		{
		};
		smSystemTime(xmATime tSystemTime, xmATime tSimuTime, const smMessageBaseInfo& mbi) : smMessageBaseInfo(mbi), m_tSystemTime(tSystemTime)
		{
		};
		smSystemTime(const smSystemTime& msg) : smMessageBaseInfo(msg), m_tSystemTime(msg.m_tSystemTime)
		{
		};
		virtual ~smSystemTime(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new smSystemTime(*this);
		};
	public:
		//	系统仿真时间，未定义
		const xmATime m_tSystemTime;
		xmDECLARE_MESSAGE_CLASS(smC_SYSTEM_TIME);
	};
	xmDECLARE_MESSAGE_FUNC(smSystemTimeFunc, smSystemTime);

	//	数据更新，指某一单个数据更新
	class smDataUpdate : public xmMessage, public smMessageBaseInfo
	{
	public:
		smDataUpdate(const xmString& strDataName, const xmValue& dataValue) : m_DataName(strDataName), m_DataValue(dataValue)
		{
		};
		smDataUpdate(const xmString& strDataName, const xmValue& dataValue, const smMessageBaseInfo& mbi) : smMessageBaseInfo(mbi), m_DataName(strDataName), m_DataValue(dataValue)
		{
		};
		smDataUpdate(const smDataUpdate& msg) : smMessageBaseInfo(msg), m_DataName(msg.m_DataName), m_DataValue(msg.m_DataValue)
		{
		};
		virtual ~smDataUpdate(void) {};
		virtual xmMessage* Clone(void) const
		{
			return new smDataUpdate(*this);
		};
	public:
		const xmString m_DataName;
		const xmValue  m_DataValue;
		xmDECLARE_MESSAGE_CLASS(smC_DATA_UPDATE);
	};
	xmDECLARE_MESSAGE_FUNC(smDataUpdateFunc, smDataUpdate);

	//	仿真系统控制指令
	class smCtrlCommand : public xmMessage, public smMessageBaseInfo
	{
	public:
		typedef enum _tagCCType
		{
			CC_ANYONE = 0,
			CC_START = 1,
			CC_FINISH,
			CC_FREEZE,
			CC_UNFREEZE,
			CC_SPEED,
			CC_COMMAND		//	自定义命令
		}ECCType;
	public:
		smCtrlCommand(ECCType eType, const xmString& strName, const xmValue& vuParam) :
			m_eType(eType),
			m_strName(CtrlCommandName(eType, strName)),
			m_vuParam(vuParam)
		{
		};
		smCtrlCommand(ECCType eType, const xmString& strName, const xmValue& vuParam, const smMessageBaseInfo& mbi) :
			smMessageBaseInfo(mbi),
			m_eType(eType),
			m_strName(CtrlCommandName(eType, strName)),
			m_vuParam(vuParam)
		{
		};
		smCtrlCommand(const smCtrlCommand& msg) : 
			smMessageBaseInfo(msg),
			m_eType(msg.m_eType),
			m_strName(msg.m_strName),
			m_vuParam(msg.m_vuParam)
		{
		};
		virtual ~smCtrlCommand(void) {};
		virtual xmMessage* Clone(void) const
		{
			return new smCtrlCommand(*this);
		};
	public:
		//	类型
		const ECCType   m_eType;
		//	名称
		const xmString  m_strName;
		//	参数
		const xmValue   m_vuParam;

		static const char* CtrlCommandName(ECCType eType, const xmString& strName)
		{
			switch (eType)
			{
			case CC_ANYONE:
				return "ANYONE";
			case CC_START:
				return "START";
			case CC_FINISH:
				return "FINISH";
			case CC_FREEZE:
				return "FREEZE";
			case CC_UNFREEZE:
				return "UNFREEZE";
			case CC_SPEED:
				return "SPEED";
			case CC_COMMAND:
				return strName;
			default:
				return NULL;
			}
		}
		xmDECLARE_MESSAGE_CLASS(smC_CTRL_COMMAND);
	};
	xmDECLARE_MESSAGE_FUNC(smCtrlCommandFunc, smCtrlCommand);

	//	系统事件
	class smSystemEvent : public xmMessage, public smMessageBaseInfo
	{
	public:
		typedef enum _tagSEType
		{
			SE_ANYONE = 0,
			SE_SYSTEM_TOREPLAY = 1,
			SE_SYSTEM_TONORMAL,
			SE_NODE_LOGIN,
			SE_NODE_LOGOUT,
			SE_DATA_SYNC,	//	预留事件，并未处理
			SE_EVENT		//	预留，自定义事件
		}ESEType;
	public:
		smSystemEvent(ESEType eType, const xmString& strName, const xmValue& vuParam) :
			m_eType(eType),
			m_strName(SystemEventName(eType, strName)),
			m_vuParam(vuParam)
		{
		};
		smSystemEvent(ESEType eType, const xmString& strName, const xmValue& vuParam, const smMessageBaseInfo& mbi) :
			smMessageBaseInfo(mbi),
			m_eType(eType),
			m_strName(SystemEventName(eType, strName)),
			m_vuParam(vuParam)
		{
		};
		smSystemEvent(const smSystemEvent& msg) : 
			smMessageBaseInfo(msg),
			m_eType(msg.m_eType),
			m_strName(msg.m_strName),
			m_vuParam(msg.m_vuParam)
		{
		};
		virtual ~smSystemEvent(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new smSystemEvent(*this);
		};
	public:
		//	类型
		const ESEType   m_eType;
		//	名称
		const xmString  m_strName;
		//	参数
		const xmValue   m_vuParam;

		static const char* SystemEventName(ESEType eType, const xmString& strName)
		{
			switch (eType)
			{
			case SE_ANYONE:
				return "ANYONE";
			case SE_SYSTEM_TOREPLAY:
				return "SYSTEM_TOREPLAY";
			case SE_SYSTEM_TONORMAL:
				return "SYSTEM_TONORMAL";
			case SE_NODE_LOGIN:
				return "NODE_LOGIN";
			case SE_NODE_LOGOUT:
				return "NODE_LOGOUT";
			case SE_DATA_SYNC:
				return "DATA_SYNC";
			case SE_EVENT:
				return strName;
			default:
				return NULL;
			}
		}
		xmDECLARE_MESSAGE_CLASS(smC_SYSTEM_EVENT);
	};
	xmDECLARE_MESSAGE_FUNC(smSystemEventFunc, smSystemEvent);

	//	收到公告
	class smBulletinGet : public xmMessage, public smMessageBaseInfo
	{
	public:
		smBulletinGet(const xmValue& topic, const xmValue& content) : m_Topic(topic), m_Content(content)
		{
		};
		smBulletinGet(const xmValue& topic, const xmValue& content, const smMessageBaseInfo& mbi) : smMessageBaseInfo(mbi), m_Topic(topic), m_Content(content)
		{
		};
		smBulletinGet(const smBulletinGet& msg) : smMessageBaseInfo(msg), m_Topic(msg.m_Topic), m_Content(msg.m_Content)
		{
		};
		virtual ~smBulletinGet(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new smBulletinGet(*this);
		};
	public:
		//	公告主题
		const xmValue m_Topic;
		//	公告内容
		const xmValue m_Content;
		xmDECLARE_MESSAGE_CLASS(smC_BULLETIN_GET);
	};
	xmDECLARE_MESSAGE_FUNC(smBulletinGetFunc, smBulletinGet);

	//	收到消息
	class smMessageGet : public xmMessage, public smMessageBaseInfo
	{
	public:
		smMessageGet(const xmString& strSender, const xmValue& content) : m_strSender(strSender), m_Content(content)
		{
		};
		smMessageGet(const xmString& strSender, const xmValue& content, const smMessageBaseInfo& mbi) : smMessageBaseInfo(mbi), m_strSender(strSender), m_Content(content)
		{
		};
		smMessageGet(const smMessageGet& msg) : smMessageBaseInfo(msg), m_strSender(msg.m_strSender), m_Content(msg.m_Content)
		{
		};
		virtual ~smMessageGet(void)
		{
		};
		virtual xmMessage* Clone(void) const
		{
			return new smMessageGet(*this);
		};
	public:
		//	消息发送者
		const xmString m_strSender;
		//	消息内容
		const xmValue m_Content;
		xmDECLARE_MESSAGE_CLASS(smC_MESSAGE_GET);
	};
	xmDECLARE_MESSAGE_FUNC(smMessageGetFunc, smMessageGet);

	//	仿真系统日志
	class smSystemLog : public xmMessage, public smMessageBaseInfo
	{
	public:
		smSystemLog(xmLog::ELogLevel eLogLevel, const xmString& strLogContent) : m_eLogLevel(eLogLevel), m_strLogContent(strLogContent)
		{
		};
		smSystemLog(xmLog::ELogLevel eLogLevel, const xmString& strLogContent, const smMessageBaseInfo& mbi) : smMessageBaseInfo(mbi), m_eLogLevel(eLogLevel), m_strLogContent(strLogContent)
		{
		};
		smSystemLog(const smSystemLog& msg) : smMessageBaseInfo(msg), m_eLogLevel(msg.m_eLogLevel), m_strLogContent(msg.m_strLogContent)
		{
		};
		virtual ~smSystemLog(void) {};
		virtual xmMessage* Clone(void) const
		{
			return new smSystemLog(*this);
		};
	public:
		const xmLog::ELogLevel m_eLogLevel;
		const xmString m_strLogContent;
		xmDECLARE_MESSAGE_CLASS(smC_SYSTEM_LOG);
	};
	xmDECLARE_MESSAGE_FUNC(smSystemLogFunc, smSystemLog);
}
#endif  //  __SSA_UMSF_IMESSAGE_H

