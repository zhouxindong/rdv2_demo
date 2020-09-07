#ifndef __SSA_UMSF_MDI_H
#define __SSA_UMSF_MDI_H
#include "tinyxml2/tinyxml2.h"
#include "mclass.h"
#include "mdata.h"
#include "mgroup.h"
#include "mtrigger.h"
#include "udefine.h"

namespace ssa
{
	/*******************************************************************************
    �����ƣ�xmMdi
    ��  �ܣ�����Model Declare Information
    ˵  ������MRM��������MRTCʹ�á�MDI����ÿ��ģ���������ļ��������ľ�̬��Ϣ������MDI�е���Ϣ��
	       ����ģ�������������йأ������ϵͳ�޹ء�
    *******************************************************************************/
	class xmUMSF;
	class xmMdi
    {
		friend xmUMSF;
	public:
		typedef xmOrderMap<xmString, xmPtr<const xmModelClass> > t_ClassMap;

		typedef xmOrderMap<xmString, xmPtr<const xmModelData> > t_DataMap;
		typedef xmOrderMap<xmString, const xmModelData*> t_DataPtrMap;

		typedef xmOrderMap<xmString, xmPtr<const xmModelGroup> > t_GroupMap;
		typedef xmOrderMap<xmString, const xmModelGroup*> t_GroupPtrMap;

		typedef xmOrderMap<xmString, std::map<unsigned int, const xmModelData*>> t_TagDataMap;

		xmMdi(const xmString& strModelName);
        ~xmMdi(void);
		//	����XML�ļ�
		//	strFileName��ģ�������ļ����ƣ���gnc.xml
		void LoadXML(const char* strMdfPath);
	public:
		/*******************************************************************************
		�� MRTC ʹ�õĽӿ�
		*******************************************************************************/
		//	ģ������
		const xmString& ModelName() const
		{
			return m_strModelName;
		}
		//	��ȡ��������
		unsigned int DriveTime() const
		{
			return m_uDriveTime;
		}
		//	��ȡ�ڲ������롢����������ݱ�
		const xmModelData* ModelData(const xmString& strDataName) const
		{
			t_DataMap::t_cPos pos = m_mapDataDeclare.Find(strDataName);
			if (pos == m_mapDataDeclare.INVALID_POS())
			{
				return NULL;
			}
			else
			{
				return m_mapDataDeclare.PosValue(pos);
			}
			//return (pos == m_mapDataDeclare.INVALID_POS()) ? NULL : m_mapDataDeclare.PosValue(pos);
		}
		const t_DataPtrMap& InnerData() const
		{
			return m_mapInnerData;
		}
		const t_DataPtrMap& InputData() const
		{
			return m_mapInputData;
		}
		const t_DataPtrMap& OutputData() const
		{
			return m_mapOutputData;
		}
		//	��ȡ���롢��������������
		const xmModelGroup* ModelGroup(const xmString& strGroupName) const
		{
			t_GroupMap::t_cPos pos = m_mapGroupDeclare.Find(strGroupName);
			if (pos == m_mapGroupDeclare.INVALID_POS())
			{
				return NULL;
			}
			else
			{
				return m_mapGroupDeclare.PosValue(pos);
			}
			//return (pos == m_mapGroupDeclare.INVALID_POS()) ? NULL : m_mapGroupDeclare.PosValue(pos);
		}
		const t_GroupPtrMap& InputGroup() const
		{
			return m_mapInputGroup;
		}
		const t_GroupPtrMap& OutputGroup() const
		{
			return m_mapOutputGroup;
		}
		//	��ȡ���ݱ�ǩ��Ϣ
		const t_TagDataMap& TagDataInfo() const
		{
			return m_mapTagDataInfo;
		}
		//	��ȡģ�͵ĵ�������
		bool IsRespondStart() const
		{
			return m_bIsRespondStart;
		}
		bool IsRespondFreeze() const
		{
			return m_bIsRespondFreeze;
		}
		bool IsRespondSpeed() const
		{
			return m_bIsRespondSpeed;
		}

    public:
		/*******************************************************************************
		���Խӿ�
		*******************************************************************************/
		//	��ӡDRIVE_CYCLE�ֶ���Ϣ
		xmString PrintDriveCycle() const;
		//	��ӡCLASS_DECLARE�ֶ���Ϣ
		xmString PrintClassDeclare(const xmString& strClassName = NULL) const;
		//	��ӡDATA_DECLARE�ֶ���Ϣ
		xmString PrintDataDeclare(const xmString& strDataName = NULL) const;
		//	��ӡGROUP_DECLARE�ֶ���Ϣ
		xmString PrintGroupDeclare(const xmString& strGroupName = NULL) const;
		//	��ӡSCHEDULE_CONFIG�ֶ���Ϣ
		xmString PrintScheduleConfig() const;

	private:
		//	����ʱ��
		int LoadDRIVE_CYCLE(tinyxml2::XMLElement* root);
		//	����CLASS����
		size_t LoadCLASS_DECLARE(tinyxml2::XMLElement* root);
		//	����DATA����
		size_t LoadDATA_DECLARE(tinyxml2::XMLElement* root);
		//	����GROUP����
		size_t LoadGROUP_DECLARE(tinyxml2::XMLElement* root);
		//	�����Ƿ�ɹ�
		xmRet LoadSCHEDULE_CONFIG(tinyxml2::XMLElement* root);

	private:
		//	����CLASS_DECLARE/CLASS/PROPERTY
		//	������һ��PROPERTY�ֶ����ڵĸ��ֶΣ����һ��ʹ�á�{}��������Class����JSON�ַ�������������˳�ʼֵ�������һ��
		//	ʹ�á�{}�������ĳ�ʼֵJSON�ַ��������δ�����ʼֵ����strInitJson���䡣
		//void _LoadSTRUCT_(tinyxml2::XMLElement* property_field, xmString& strClassJson, xmString& strInitJson);
		
		//	����DATA_DECLARE/*_DATA/DATA
		size_t _LoadDATA(tinyxml2::XMLElement* root, t_DataPtrMap& mapData, xmEIOType ioType);

		//	����GROUP_DECLARE/*_GROUP/GROUP
		size_t _LoadGROUP(tinyxml2::XMLElement* root, const t_DataPtrMap& mapData, t_GroupPtrMap& mapGroup, xmEIOType ioType);
		//	����GROUP_DECLARE/INPUT_GROUP/QUICK
		size_t _LoadQUICK(tinyxml2::XMLElement* root);
		
		//	����һ���������͵Ķ��󣬿�����һ�����ݣ�Ҳ������һ����������
		//	����NULL����ʾ������Ϊ������������
		//	����xmModelClass�е�NameΪNULL����ʾΪһ����Ƕ�ṹ����ʱ��dataValue��xmModelClass�е�InitValue��ͬ��
		//	Name��ΪNULLʱ����ʾһ���Ѿ�����������࣬��ʱ��dataValue��xmModelClass�е�InitValue���ܲ���ͬ��
		xmModelClass* _LoadDataObject(tinyxml2::XMLElement* root, bool bMultType, xmValue& dataValue, xmEDataCheckType& eCheckType);
		//	��һ��xmValue��ϳ�һ���µ�xmValue������µ�xmValue��Class�������ⲿ�������������
		const xmClass* _CombineValue(const xmOrderMap<xmString, int>& mapName, const std::vector<xmPtr<xmValue>>& vValue, xmValue& newValue);

		//	����GROUP_DECLARE/*_GROUP/GROUP/TRIGGER
		xmModelTrigger* _LoadGROUP_TRIGGER(tinyxml2::XMLElement* root);
		void _LoadTRIGGER_SCRIPT(tinyxml2::XMLElement* trigger, xmModelTrigger* pModelTrigger) const;

		//	����GROUP_DECLARE/*_GROUP/GROUP/DATA
		size_t _LoadGROUP_DATA(tinyxml2::XMLElement* root, std::vector<const xmModelData*>& vDataList, const t_DataPtrMap& mapData);
		void _AddData2Group(const xmString& strDataName, std::vector<const xmModelData*>& vDataList, t_DataPtrMap& mapGroupData, const t_DataPtrMap& mapData, bool bThrowException);

	private:
		//	ģ�������ļ�����·������ڣ������ļ�·������
		//	ģ�������ļ��У����ܴ��������ط�ʹ���ļ����ã�
		//	��1����ĳ�ʼֵ��JSON��ʽ
		//	��2�����ݵĳ�ʼֵ��JSON��ʽ
		//	��3���������ű���LUA��ʽ
		xmString m_xmlPath;
		tinyxml2::XMLDocument m_xmlDoc;

		//	ģ������
		const xmString m_strModelName;
		/*******************************************************************************
		XML������Ϣ
		*******************************************************************************/
		//	DRIVE_CYCLE�ֶ�
		unsigned int m_uDriveTime;	//	��λΪ����

		//	CLASS_DECLARE�ֶ�
		t_ClassMap m_mapClassDeclare;
		//	��¼���������У������ݶ�����һ���������������
		//	keyΪ���������ƣ�valueΪ������ָ��
		t_ClassMap m_mapAnonymousClass;

		//	DATA_DECLARE�ֶΣ�ȫ��������Ϣ
		t_DataMap m_mapDataDeclare;
		t_DataPtrMap m_mapInnerData;		//	��д���ڲ����ݣ�ָ����INPUT_GROUP��OUT_GROUP������������
		t_DataPtrMap m_mapInputData;		//	ֻ����������������
		t_DataPtrMap m_mapOutputData;		//	��д�������������
		//	��ǩ�µ�ȫ�����ݣ�keyΪ��ǩ���ƣ�valueΪ�Ա������Ϊ˳�������������Ϣ
		t_TagDataMap m_mapTagDataInfo;
		//	��ǩ�µ�������Ϣ
		xmOrderMap<xmString, unsigned int> m_mapTagIndexInfo;

		//	GROUP_DECLARE�ֶΣ�ȫ������IO������Ϣ
		t_GroupMap m_mapGroupDeclare;
		t_GroupPtrMap m_mapInputGroup;		//	����������
		t_GroupPtrMap m_mapOutputGroup;		//	���������

		//	SCHEDULE_CONFIG�ֶΣ�����ģ�͵ĵ�����Ϣ
		bool m_bIsRespondStart;
		bool m_bIsRespondFreeze;
		bool m_bIsRespondSpeed;
	};
}
#endif  //  __SSA_UMSF_MDI_H

