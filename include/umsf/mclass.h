#ifndef __SSA_UMSF_MCLASS_H
#define __SSA_UMSF_MCLASS_H
#include "udefine.h"

namespace ssa
{
	class xmUMSF;
	class xmMdi;
	class xmModelClass
	{
		friend xmUMSF;
		friend xmMdi;
	public:
		//	��������Class��JSON�ַ�����������ʼֵ��JSON�ַ���������һ��xmModelClass�������ʧ�ܣ����׳��쳣��
		static xmPtr<xmModelClass> Create(const xmString& strClassJson, const xmString& strInitJson);

		~xmModelClass() {};

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
		xmModelClass() {};
		xmString m_strName;					//	����������
		xmString m_strDes;					//	����������
		xmPtr<const xmClass> m_pClass;		//	��¼��ṹ
		xmPtr<const xmValue> m_pInitValue;	//	ʹ�������ṹ��Ĭ�ϳ�ʼֵ
	private:
		mutable xmString m_strPrint;
//		unsigned int m_uCID;				//	Class ID�����Ψһ��ʶ��ͨ����m_strPrint����hash�����ã�ֻҪ��������ȣ���CIDֵ�����
	};
}
#endif  //  __SSA_UMSF_MCLASS_H

