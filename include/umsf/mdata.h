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
		//	�ж�������ݣ���ĳ����ǩ�б��������
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
		xmString m_strName;					//	��������
		const xmModelClass* m_pModelClass;	//	��¼��ṹ
		const xmValue* m_pInitValue;		//	ʹ�������ṹ��Ĭ�ϳ�ʼֵ
		unsigned int m_uAutoReset;			//	������ģ�ͻ������Զ���λ���ã���ʾ��������ݾ������ٴ������ϵͳ�󣬸�λΪ��ʼֵ
		xmEIOType m_eIOType;				//	���ݵ������������
		xmEDataCheckType m_eCheckType;		//	����IOʱ���ͼ��ķ�ʽ
		//	keyΪ��ǩ���ƣ�valueΪ��������ڸñ�ǩ�еı������
		//	ָ������ţ�ӦС��0x80000000��δָ������ţ���0x80000000��ʼ����
		xmOrderMap<xmString, unsigned int> m_mapDataTag; //	���ݱ�ǩ��һ��ģ�����ݣ��������ö�����ݱ�ǩ

		tinyxml2::XMLElement* const m_xmlElement;	//	��¼�����ݵ�XMLԪ��
	private:
		mutable xmString m_strPrint;
	};
}
#endif  //  __SSA_UMSF_MDATA_H

