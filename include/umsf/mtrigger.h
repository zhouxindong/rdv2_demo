#ifndef __SSA_UMSF_MTRIGGER_H
#define __SSA_UMSF_MTRIGGER_H
#include "imessage.h"

namespace ssa
{
	//	���崥��������
	class xmModelTrigger
	{
	public:
		xmModelTrigger() {};
		virtual ~xmModelTrigger() {};
		virtual xmEMsgHandleObject Type() const = 0;
		//	��XML��ʽ��ӡ�����������ֶ�
		const xmString& PrintXML() const;
		//	����ӡ����������
		const xmString& PrintTrigger() const;
		//	����ӡ����������
		const xmString& PrintParam() const;

		//	�������и��ӵ�Lua�ű�
		//	����Js����̫���ˣ���˴�������ֻʹ��Lua�ű�
		xmString m_strScript;

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
		xmTimeModelTrigger() : xmModelTrigger()
		{
		};
		virtual ~xmTimeModelTrigger()
		{
		};
		virtual xmEMsgHandleObject Type() const
		{
			return MHO_TIME_TRIGGER;
		}
	protected:
		virtual xmString _PrintXML() const;
		virtual xmString _PrintTrigger() const;
		virtual xmString _PrintParam() const;

	public:
		//	��λ������
		unsigned int m_uTime;
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
		virtual xmEMsgHandleObject Type() const
		{
			return MHO_EVENT_TRIGGER;
		}
	protected:
		virtual xmString _PrintXML() const;
		virtual xmString _PrintTrigger() const;
		virtual xmString _PrintParam() const;

	public:
		//	m_uEvent��ʾ�����¼�����1�ֽڱ�ʾ����ϵͳ���ͳ������¼���ָ���0�ֽڱ�ʾ�����¼���ָ�������
		unsigned int m_uEvent;
		//	��ʾ�����¼�������
		xmString m_Name;
		//	m_Param��ʾ�¼������Ĳ���
		//	smC_DATA_SYNC		�������ͣ��޲���
		//	smC_NODE_EVENT		����0�ֽڱ�ʾ�ڵ�ע�ᡢע�������͡�����Ϊstring���ͣ���ʾ�ڵ����ƣ����Ϊ��ЧxmValue����ʾ��������ڵ�
		//	smC_SYSTEM_EVENT	����0�ֽڱ�ʾϵͳ״̬ǰ�������ͣ��޲���
		//	smC_SYSTEM_COMMAND	����0�ֽڱ�ʾϵͳѵ����ʼ�����������͡���Ϊ�������ʱ������Ϊint���ͣ����汶�ʣ����Ϊ��ЧxmValue����ʾ������ֵ
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
		virtual xmEMsgHandleObject Type() const
		{
			return MHO_DATA_TRIGGER;
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

