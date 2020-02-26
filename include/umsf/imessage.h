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
		smC_CTRL_COMMAND = 0x1200	//	����ָ�����ѵ����ʼ��ѵ��������IO���ᡢIO�ⶳ����ɵȡ����п��ָ����Դ����������嶨��Ϊ��
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
	} smESystemMessageCode;


	//	����ϵͳ����
	class smWorldClick : public xmMessage
	{
	public:
		smWorldClick(size_t uClickCount, time_t uSystemTime) : m_uClickCount(uClickCount), m_uSystemTime(uSystemTime)
		{
		};
		smWorldClick(const smWorldClick& msg) : m_uClickCount(msg.m_uClickCount), m_uSystemTime(msg.m_uSystemTime)
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
		//	��ǰ������
		const size_t m_uClickCount;
		//	��ǰϵͳʱ��
		const time_t m_uSystemTime;
		xmDECLARE_MESSAGE_CLASS(smC_WORLD_CLICK);
	};
	xmDECLARE_MESSAGE_FUNC(smWorldClickFunc, smWorldClick);

	//	ϵͳʱ��
	class smSystemTime : public xmMessage
	{
	public:
		smSystemTime(xmATime tWallTime, xmATime tSimuTime) : m_tWallTime(tWallTime), m_tSimuTime(tSimuTime)
		{
		};
		smSystemTime(const smSystemTime& msg) : m_tWallTime(msg.m_tWallTime), m_tSimuTime(msg.m_tSimuTime)
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
		//	����ǽʱ
		const xmATime m_tWallTime;
		//	����ʱ��
		const xmATime m_tSimuTime;
		xmDECLARE_MESSAGE_CLASS(smC_SYSTEM_TIME);
	};
	xmDECLARE_MESSAGE_FUNC(smSystemTimeFunc, smSystemTime);

	//	���ݸ��£�ָĳһ�������ݸ���
	class smDataUpdate : public xmMessage
	{
	public:
		smDataUpdate(const xmString& strDataName, const xmValue& dataValue) : m_DataName(strDataName), m_DataValue(dataValue)
		{
		};
		smDataUpdate(const smDataUpdate& msg) : m_DataName(msg.m_DataName), m_DataValue(msg.m_DataValue)
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
	class smCtrlCommand : public xmMessage
	{
	public:
		typedef enum _tagCCType
		{
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
		smCtrlCommand(const smCtrlCommand& msg) : 
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
		xmDECLARE_MESSAGE_CLASS(smC_CTRL_COMMAND);

	private:
		static const char* CtrlCommandName(ECCType eType, const xmString& strName)
		{
			switch (eType)
			{
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
	};
	xmDECLARE_MESSAGE_FUNC(smCtrlCommandFunc, smCtrlCommand);

	//	ϵͳ�¼�
	class smSystemEvent : public xmMessage
	{
	public:
		typedef enum _tagSEType
		{
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
		smSystemEvent(const smSystemEvent& msg) : 
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
		xmDECLARE_MESSAGE_CLASS(smC_SYSTEM_EVENT);

	private:
		static const char* SystemEventName(ESEType eType, const xmString& strName)
		{
			switch (eType)
			{
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
	};
	xmDECLARE_MESSAGE_FUNC(smSystemEventFunc, smSystemEvent);

	//	�յ�����
	class smBulletinGet : public xmMessage
	{
	public:
		smBulletinGet(const xmValue& topic, const xmValue& content) : m_Topic(topic), m_Content(content)
		{
		};
		smBulletinGet(const smBulletinGet& msg) : m_Topic(msg.m_Topic), m_Content(msg.m_Content)
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
	class smMessageGet : public xmMessage
	{
	public:
		smMessageGet(const xmString& strSender, const xmValue& content) : m_strSender(strSender), m_Content(content)
		{
		};
		smMessageGet(const smMessageGet& msg) : m_strSender(msg.m_strSender), m_Content(msg.m_Content)
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
}
#endif  //  __SSA_UMSF_IMESSAGE_H

