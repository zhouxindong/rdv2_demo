#ifndef __SSA_UMSF_MCLASS_H
#define __SSA_UMSF_MCLASS_H
#include "udefine.h"

namespace ssa
{
	class xmMdi;
	class xmModelClass
	{
		friend xmMdi;
	public:
		//	��������Class��JSON�ַ�����������ʼֵ��JSON�ַ���������һ��xmModelClass�������ʧ�ܣ����׳��쳣��
		static xmPtr<xmModelClass> Create(const xmString& strClassJson, const xmString& strInitJson);

		~xmModelClass()
		{
			if (m_pInitValue != NULL)
			{
				delete m_pInitValue;
			}
			if (m_pClass != NULL)
			{
				delete m_pClass;
			}
		}

		inline const xmString& Name() const
		{
			return m_strName;
		}
		const xmString& Describe() const
		{
			return m_strDes;
		}
		inline const xmClass& Class() const
		{
			return *m_pClass;
		}
		inline const xmValue& InitValue() const
		{
			return *m_pInitValue;
		}

		const xmString& PrintXML() const;
	protected:
		xmModelClass() : m_pClass(NULL), m_pInitValue(NULL) {};
		xmString m_strName;					//	����������
		xmString m_strDes;					//	����������
		const xmClass* m_pClass;			//	��¼��ṹ
		const xmValue* m_pInitValue;		//	ʹ�������ṹ��Ĭ�ϳ�ʼֵ
	private:
		mutable xmString m_strPrint;
//		unsigned int m_uCID;				//	Class ID�����Ψһ��ʶ��ͨ����m_strPrint����hash�����ã�ֻҪ��������ȣ���CIDֵ�����
	};
}
#endif  //  __SSA_UMSF_MCLASS_H

