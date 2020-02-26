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
    类名称：xmMdi
    功  能：定义Model Declare Information
    说  明：由MRM创建，供MRTC使用。MDI定义每个模型在配置文件中描述的静态信息，所有MDI中的信息，
	       仅与模型自身计算过程有关，与仿真系统无关。
    *******************************************************************************/
	class xmMdi
    {
    public:
		typedef xmOrderMap<xmString, xmPtr<const xmModelClass> > t_ClassMap;

		typedef xmOrderMap<xmString, xmPtr<const xmModelData> > t_DataMap;
		typedef xmOrderMap<xmString, const xmModelData*> t_DataPtrMap;

		typedef xmOrderMap<xmString, xmPtr<const xmModelGroup> > t_GroupMap;
		typedef xmOrderMap<xmString, const xmModelGroup*> t_GroupPtrMap;

		xmMdi(const xmString& strModelName);
        ~xmMdi(void);
		//	加载XML文件
		//	strFileName：模型配置文件名称，如gnc.xml
		void LoadXML(const char* strFileName);
	public:
		/*******************************************************************************
		供 MRTC 使用的接口
		*******************************************************************************/
		//	模型名称
		const xmString& ModelName() const
		{
			return m_strModelName;
		}
		//	获取驱动周期
		unsigned int DriveTime() const
		{
			return m_uDriveTime;
		}
		//	获取内部、输入、输出三类数据表
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
		//	获取输入、输出两类数据组表
		const t_GroupPtrMap& InputGroup() const
		{
			return m_mapInputGroup;
		}
		const t_GroupPtrMap& OutputGroup() const
		{
			return m_mapOutputGroup;
		}
		//	获取模型的调度特性
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
		调试接口
		*******************************************************************************/
		//	打印DRIVE_CYCLE字段信息
		xmString PrintDriveCycle() const;
		//	打印CLASS_DECLARE字段信息
		xmString PrintClassDeclare(const xmString& strClassName = NULL) const;
		//	打印DATA_DECLARE字段信息
		xmString PrintDataDeclare(const xmString& strDataName = NULL) const;
		//	打印GROUP_DECLARE字段信息
		xmString PrintGroupDeclare(const xmString& strGroupName = NULL) const;
		//	打印SCHEDULE_CONFIG字段信息
		xmString PrintScheduleConfig() const;

	private:
		//	返回时间
		int LoadDRIVE_CYCLE(tinyxml2::XMLElement* root);
		//	返回CLASS数量
		size_t LoadCLASS_DECLARE(tinyxml2::XMLElement* root);
		//	返回DATA数量
		size_t LoadDATA_DECLARE(tinyxml2::XMLElement* root);
		//	返回GROUP数量
		size_t LoadGROUP_DECLARE(tinyxml2::XMLElement* root);
		//	返回是否成功
		xmRet LoadSCHEDULE_CONFIG(tinyxml2::XMLElement* root);

	private:
		//	加载CLASS_DECLARE/CLASS/PROPERTY
		//	输入是一个PROPERTY字段所在的根字段，输出一个使用“{}”包裹的Class描述JSON字符串，如果定义了初始值，则输出一个
		//	使用“{}”包裹的初始值JSON字符串，如果未定义初始值，则strInitJson不变。
		void _LoadSTRUCT(tinyxml2::XMLElement* property_field, xmString& strClassJson, xmString& strInitJson);
		
		//	加载DATA_DECLARE/*_DATA/DATA
		size_t _LoadDATA(tinyxml2::XMLElement* root, t_DataPtrMap& mapData, xmEIOType ioType);

		//	加载GROUP_DECLARE/*_GROUP/GROUP
		size_t _LoadGROUP(tinyxml2::XMLElement* root, const t_DataPtrMap& mapData, t_GroupPtrMap& mapGroup);

		//	加载GROUP_DECLARE/*_GROUP/GROUP/TRIGGER
		xmModelTrigger* _LoadGROUP_TRIGGER(tinyxml2::XMLElement* root);

		//	加载GROUP_DECLARE/*_GROUP/GROUP/DATA
		size_t _LoadGROUP_DATA(tinyxml2::XMLElement* root, std::vector<xmString>& vDataList, const t_DataPtrMap& mapData);
		void _AddData2Group(const xmString& strDataName, std::vector<xmString>& vDataList, t_DataPtrMap& mapGroupData, const t_DataPtrMap& mapData, bool bThrowException);

	private:
		tinyxml2::XMLDocument m_xmlDoc;

		const xmString m_strModelName;
		/*******************************************************************************
		XML配置信息
		*******************************************************************************/
		//	DRIVE_CYCLE字段
		unsigned int m_uDriveTime;	//	单位为毫秒

		//	CLASS_DECLARE字段
		t_ClassMap m_mapClassDeclare;
		//	记录数据声明中，和数据定义在一起的匿名的数据类
		//	key为该数据名称，value为改类型指针
		t_ClassMap m_mapAnonymousClass;

		//	DATA_DECLARE字段，全部数据信息
		t_DataMap m_mapDataDeclare;
		t_DataPtrMap m_mapInnerData;		//	读写，内部数据，指不在INPUT_GROUP和OUT_GROUP中描述的数据
		t_DataPtrMap m_mapInputData;		//	只读，所有输入数据
		t_DataPtrMap m_mapOutputData;		//	读写，所有输出数据
		//	标签下的全部数据，key为标签名称，value为以遍历序号为顺序的描述数据信息
		xmOrderMap<xmString, std::map<unsigned int, xmModelData*>> m_mapTagDataInfo;
		//	标签下的自增信息
		xmOrderMap<xmString, unsigned int> m_mapTagIndexInfo;

		//	GROUP_DECLARE字段，全部数据IO分组信息
		t_GroupMap m_mapGroupDeclare;
		t_GroupPtrMap m_mapInputGroup;		//	输入数据组
		t_GroupPtrMap m_mapOutputGroup;		//	输出数据组

		//	SCHEDULE_CONFIG字段，配置模型的调度信息
		bool m_bIsRespondStart;
		bool m_bIsRespondFreeze;
		bool m_bIsRespondSpeed;
	};
}
#endif  //  __SSA_UMSF_MDI_H

