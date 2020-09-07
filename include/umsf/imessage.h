#ifndef __SSA_UMSF_IMESSAGE_H
#define __SSA_UMSF_IMESSAGE_H
#include "../base/base.h"
#include "idefine.h"

namespace ssa
{
	//	����ϵͳ�ܹ����͸�UMSF Manager����Ϣ
	//	System Message Code
	typedef enum
	{
		smC_WORLD_CLICK  = 0x0100,	//	����ϵͳ���ģ�������������
		smC_SYSTEM_TIME  = 0x0110,	//	ϵͳʱ�䣬��������ǽʱ�ͷ���ʱ��
		smC_DATA_UPDATE  = 0x0200,	//	���ݸ��£�ָĳһ����������ֵ��д�룬��������������������ֵ
		smC_BULLETIN_GET = 0x0300,	//	������գ���ȡ���Լ����ĵĹ��档��������������͹������ݡ�
		smC_MESSAGE_GET  = 0x0400,	//	��Ϣ���գ����յ�ϵͳ���͵���Ϣ����������Ϣ�����ߺ���Ϣ���ݡ�
									//	��������Ϣ��
									//		1���������������ģ���Ϣ�Ǳ������ա�
									//		2�����������������������ͻ����ݣ���Ϣ�ķ����߱����ǽڵ�����
		smC_SYSTEM_EVENT = 0x1100,	//	ϵͳ�¼��������л�������״̬���л����ط�״̬���ڵ���롢�ǳ��ȣ����нڵ��¼��ɴ��������ڵ�����
		smC_CTRL_COMMAND = 0x1200,	//	����ָ�����ѵ����ʼ��ѵ��������IO���ᡢIO�ⶳ����ɵȡ����п��ָ����Դ����������嶨��Ϊ��
									//		> 1����ɣ�
									//		<-1�����ɣ�
									//		= 1���������У�
									//		= 0����ͣ���㣬
									//		=-1��δ���塣
									//	xmLog::todo()���н����壬δʵ�֣�
									//	�������ͣ��������𣺶����Ǽ�ֹͣģ��IO��Ҳֹͣģ�ͼ��㣻��ͣ������ֹͣģ�ͼ��㣬����ֹͣģ��IO
									//	����ʵ�֣�MRTCֹͣ�����κ���Ϣ
									//	��ͣ���㣺ģ�͵�OnDrive�ӿ�ֹͣ�ص�
									//	���汶�ʣ�Ŀǰ��ͬ���б��ʣ�UMSF������ģ�ͻص�������������Ӧ��
		smC_SYSTEM_LOG	 = 0x8000
	} smESystemMessageCode;

	//	����ϵͳ��Ϣ��Я������Ϣ��������Ϣ���������ϵͳ����ҵ���޹�
	class smMessageBaseInfo
	{
	public:
		smMessageBaseInfo() : m_lWallTime(0), m_lClickCount(0) {};
		smMessageBaseInfo(long long lWallTime, long long lClickCount) : m_lWallTime(lWallTime), m_lClickCount(lClickCount) {};
		smMessageBaseInfo(const smMessageBaseInfo& mbi) : m_lWallTime(mbi.m_lWallTime), m_lClickCount(mbi.m_lClickCount) {};
		virtual ~smMessageBaseInfo() {};
	public:
		//	��ʵ����ǽʱ����1970.01.01 00:00:00.000�����ڵĺ�����
		const long long m_lWallTime;
		//	���ļ���
		const long long m_lClickCount;
	};

	//	����ϵͳ���ģ�Ŀǰ��������Ϣ������Ϣ
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

	//	ϵͳʱ�䣬��ʱ�������ϵͳ����ҵ���й�
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
		//	ϵͳ����ʱ�䣬δ����
		const xmATime m_tSystemTime;
		xmDECLARE_MESSAGE_CLASS(smC_SYSTEM_TIME);
	};
	xmDECLARE_MESSAGE_FUNC(smSystemTimeFunc, smSystemTime);

	//	���ݸ��£�ָĳһ�������ݸ���
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

	//	����ϵͳ����ָ��
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
			CC_COMMAND		//	�Զ�������
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
		//	����
		const ECCType   m_eType;
		//	����
		const xmString  m_strName;
		//	����
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

	//	ϵͳ�¼�
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
			SE_DATA_SYNC,	//	Ԥ���¼�����δ����
			SE_EVENT		//	Ԥ�����Զ����¼�
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
		//	����
		const ESEType   m_eType;
		//	����
		const xmString  m_strName;
		//	����
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

	//	�յ�����
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
		//	��������
		const xmValue m_Topic;
		//	��������
		const xmValue m_Content;
		xmDECLARE_MESSAGE_CLASS(smC_BULLETIN_GET);
	};
	xmDECLARE_MESSAGE_FUNC(smBulletinGetFunc, smBulletinGet);

	//	�յ���Ϣ
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
		//	��Ϣ������
		const xmString m_strSender;
		//	��Ϣ����
		const xmValue m_Content;
		xmDECLARE_MESSAGE_CLASS(smC_MESSAGE_GET);
	};
	xmDECLARE_MESSAGE_FUNC(smMessageGetFunc, smMessageGet);

	//	����ϵͳ��־
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

