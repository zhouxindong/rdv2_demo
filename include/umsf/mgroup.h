#ifndef __SSA_UMSF_MGROUP_H
#define __SSA_UMSF_MGROUP_H
#include "mtrigger.h"

namespace tinyxml2
{
	class XMLElement;
}
namespace ssa
{
	//	��������������
	class xmMdi;
	class xmModelGroup
	{
		friend xmMdi;
	public:
		//	��¼��xml�ļ���������Group�Ƿ񼤻�
		//	0��������
		//	1������
		//	2���Զ��ж�
		typedef enum __tagGroupState
		{
			GS_NONACTIVATED = 0,
			GS_ACTIVATED,
			GS_AUTOACTIVATED
		}xmGroupState;

		xmModelGroup(tinyxml2::XMLElement* xmlElement) : m_pTrigger(NULL), m_eActivation(GS_ACTIVATED), m_xmlElement(xmlElement) {};
		virtual ~xmModelGroup() 
		{
			if (m_pTrigger != NULL)
			{
				delete m_pTrigger;
			}
		};
		const xmString& Name() const
		{
			return m_strName;
		}
		const char* Attribute(const char* strAttrName) const;

		const xmString& Data(size_t i) const
		{
			return m_DataList[i];
		}
		const std::vector<xmString>& DataList() const
		{
			return m_DataList;
		}
		const xmModelTrigger* Trigger() const
		{
			return m_pTrigger;
		}
		xmGroupState IsActivated() const
		{
			return m_eActivation;
		}
	public:
		const xmString& PrintXML() const;

	protected:
		xmString m_strName;
		std::vector<xmString> m_DataList;
		xmModelTrigger* m_pTrigger;
		xmGroupState m_eActivation;

		tinyxml2::XMLElement* const m_xmlElement;	//	��¼���������XMLԪ��
	private:
		mutable xmString m_strPrint;

		const char* __PrintActivate() const
		{
			switch (m_eActivation)
			{
			case GS_NONACTIVATED:
				return "false";
			case GS_ACTIVATED:
				return "true";
			case GS_AUTOACTIVATED:
				return "auto";
			default:
				return NULL;
			}
		}
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_MGROUP_H

