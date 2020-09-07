/*******************************************************************************
Copyright (C), 2011-2022, ACC12. All rights reserved.
模 块 名：xmumsf
文 件 名：mui.h
相关文件：无
文件版本: 5.0
功能概述：
多 线 程：不适用
异常安全：不适用
详细说明：
备注信息：无
--------------------------------------------------------------------------------
修改记录：
	日期	        版本        修改人        联系方式
*******************************************************************************/
#ifndef __SSA_UMSF_MUI_H
#define __SSA_UMSF_MUI_H
#include "tinyxml2/tinyxml2.h"
#include "udefine.h"
#include "publicres.h"
#include "mdi.h"
#include "rtdata.h"

namespace ssa
{
	/*******************************************************************************
	类名称：xmMui
	功  能：Model Using Information
	说  明：描述如何使用模型创建实例，记录model.config文件中<MODEL>/<INSTANCE>元素信息
	*******************************************************************************/
	class xmMui
	{
	public:
		//	model_config为：model.config/ROOT/MODEL字段
		xmMui(const xmMdi* pMdi) : m_strModelName(pMdi->ModelName()), m_pMdi(pMdi) {};
		virtual ~xmMui() {};

	public:
		void LoadXML(tinyxml2::XMLElement* instance);

		const xmMdi* Mdi() const
		{
			return m_pMdi;
		}

		const xmString& ModelName() const
		{
			return m_strModelName;
		}
		const xmString& InstanceName() const
		{
			return m_strInstanceName;
		}
		const xmString& ConfigureFile() const
		{
			return m_strInstanceConfigureFile;
		}
		xmEDataMatchTarget DefaultDataMatchTarget() const
		{
			return m_eDefaultDataMatchTarget;
		}
		const xmString& DefaultDataSet() const
		{
			return m_strDefaultDataSet;
		}
		bool IsAutoRun() const
		{
			return m_bIsAutoRun;
		}
		const xmRuntimeData::SMatchInfo* DataMatchInfo(const xmString& strDataName) const
		{
			std::map<xmString, xmPtr<xmRuntimeData::SMatchInfo>>::const_iterator pos = m_mapDataMatchInfo.find(strDataName);
			return (pos == m_mapDataMatchInfo.end()) ? NULL : pos->second;
		}
		const std::map<xmString, xmPtr<xmRuntimeData::SMatchInfo>>& DataMatchInfo() const
		{
			return m_mapDataMatchInfo;
		}
	private:
		//	模型运行的静态接口，由分析模型的XML数据文件获得
		const xmMdi* const m_pMdi;
		//	模型名称
		const xmString m_strModelName;

		//	实例名称
		xmString m_strInstanceName;
		//	实例数据配置文件
		xmString m_strInstanceConfigureFile;
		//	实例默认匹配目标
		xmEDataMatchTarget m_eDefaultDataMatchTarget;
		//	实例默认数据集
		xmString m_strDefaultDataSet;
		//	实例是否自动启动
		bool m_bIsAutoRun;
		//	实例数据匹配信息
		std::map<xmString, xmPtr<xmRuntimeData::SMatchInfo>> m_mapDataMatchInfo;
		
		void _MergeMatchInfo(const xmModelData& modelData, xmRuntimeData::SMatchInfo& miTarget, const xmRuntimeData::SMatchInfo& miOther) const;
		void _UpdateMatchInfo(const xmModelData& modelData, const xmRuntimeData::SMatchInfo& matchInfo);

		//	通过<DATA>、<INCLUDE_TAG>、<INCLUDE_GROUP>三个标签，选择指定IO类型的模型实例数据
		size_t _SelectData(tinyxml2::XMLElement* element, xmEIOType ioType, std::map<const xmModelData*, int>& dataMap);
		void _Match_INPUT_TARGET(tinyxml2::XMLElement* data_match);
		void _Match_OUTPUT_TARGET(tinyxml2::XMLElement* data_match);
		void _Match_IO_PROCESSOR(tinyxml2::XMLElement* data_match, xmEIOType ioType);
		void _Match_OUTPUT_DATASET(tinyxml2::XMLElement* data_match);
		//	通过<DATA>、<INNER_DATA>、<INPUT_DATA>、<OUTPUT_DATA>四个标签，对数据进行匹配
		void _MatchData(tinyxml2::XMLElement* data_match, bool bAutoType, xmEIOType ioType = IOT_INNER);
	};
}
#endif	//	__SSA_UMSF_MUI_H