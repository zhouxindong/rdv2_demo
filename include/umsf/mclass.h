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
		//	根据描述Class的JSON字符串和描述初始值的JSON字符串，创建一个xmModelClass对象，如果失败，则抛出异常。
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
		xmString m_strName;					//	数据类名称
		xmString m_strDes;					//	数据类描述
		xmPtr<const xmClass> m_pClass;		//	记录类结构
		xmPtr<const xmValue> m_pInitValue;	//	使用上述结构的默认初始值
	private:
		mutable xmString m_strPrint;
//		unsigned int m_uCID;				//	Class ID，类的唯一标识，通过对m_strPrint进行hash计算获得，只要两个类相等，则CID值就相等
	};
}
#endif  //  __SSA_UMSF_MCLASS_H

