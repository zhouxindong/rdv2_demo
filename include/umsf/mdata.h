#ifndef __SSA_UMSF_MDATA_H
#define __SSA_UMSF_MDATA_H
#include "idefine.h"
#include "mclass.h"

namespace tinyxml2
{
	class XMLElement;
}
namespace ssa
{
	class xmUMSF;
	class xmMdi;
	class xmModelData
	{
		friend xmUMSF;
		friend xmMdi;
	public:
		xmModelData(tinyxml2::XMLElement* xmlElement) : 
			m_pModelClass(NULL), m_pInitValue(NULL), m_xmlElement(xmlElement), m_uAutoReset(0), m_eIOType(IOT_INNER), m_eCheckType(DCT_AUTO)
		{};
		~xmModelData()
		{
			if (m_pInitValue != NULL)
			{
				delete m_pInitValue;
			}
		};
		static const unsigned int MAX_TAG_INDEX;

		const xmString& Name() const
		{
			return m_strName;
		}
		size_t Size() const
		{
			return m_pInitValue->Size();
		}
		xmEDataType Type() const
		{
			return m_pInitValue->Type();
		}
		xmString TypeName() const
		{
			xmEDataType eType = Type();
			if (eType == xmDT_STRUCT)
			{
				return m_pModelClass->Name();
			}
			else if (eType == xmDT_BUFFER || eType == xmDT_STRING)
			{
				return xmStringUtil::Format("%s%d", xmTypeName(eType), m_pInitValue->Size());
			}
			else
			{
				return xmTypeName(eType);
			}
		}

		const xmValue& InitValue() const
		{
			return *m_pInitValue;
		}
		unsigned int AutoResetTime() const
		{
			return m_uAutoReset;
		}
		xmEIOType IOType() const
		{
			return m_eIOType;
		}
		xmEDataCheckType CheckType() const
		{
			return m_eCheckType;
		}

		unsigned int AddTag(const xmStringVector& vTagInfo);
		bool UpdateTaggedIndex(const xmString& strTagName, unsigned int uTaggedIndex);
		size_t TagCount() const
		{
			return m_mapDataTag.Size();
		}
		const xmString& TagName(size_t uPos) const
		{
			return m_mapDataTag.IndexKey(uPos);
		}
		//	判断这个数据，在某个标签中遍历的序号
		const unsigned int TaggedIndex(const xmString& strTagName) const
		{
			xmOrderMap<xmString, unsigned int>::t_cPos pos = m_mapDataTag.Find(strTagName);
			if (pos == m_mapDataTag.INVALID_POS())
			{
				return (unsigned int)xmINVALID_INDEX;
			}
			return m_mapDataTag.PosValue(pos);
		}
		bool IsTagged(const xmString& strTagName) const
		{
			if (strTagName.IsEmpty())
			{
				return (m_mapDataTag.Size() != 0);
			}
			return (m_mapDataTag.Find(strTagName) != m_mapDataTag.INVALID_POS());
		}


		const char* Attribute(const char* strAttrName) const;


		const xmString& PrintXML() const;

	protected:
		xmString m_strName;					//	数据名称
		const xmModelClass* m_pModelClass;	//	记录类结构
		const xmValue* m_pInitValue;		//	使用上述结构的默认初始值
		unsigned int m_uAutoReset;			//	数据在模型缓存中自动复位设置，表示该输出数据经过多少次输出到系统后，复位为初始值
		xmEIOType m_eIOType;				//	数据的输入输出类型
		xmEDataCheckType m_eCheckType;		//	数据IO时类型检查的方式
		//	key为标签名称，value为这个数据在该标签中的遍历序号
		//	指定的序号，应小于0x80000000，未指定的序号，由0x80000000开始自增
		xmOrderMap<xmString, unsigned int> m_mapDataTag; //	数据标签，一个模型数据，可以设置多个数据标签

		tinyxml2::XMLElement* const m_xmlElement;	//	记录该数据的XML元素
	private:
		mutable xmString m_strPrint;
	};
}
#endif  //  __SSA_UMSF_MDATA_H

