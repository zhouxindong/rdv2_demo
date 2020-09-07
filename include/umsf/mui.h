/*******************************************************************************
Copyright (C), 2011-2022, ACC12. All rights reserved.
ģ �� ����xmumsf
�� �� ����mui.h
����ļ�����
�ļ��汾: 5.0
���ܸ�����
�� �� �̣�������
�쳣��ȫ��������
��ϸ˵����
��ע��Ϣ����
--------------------------------------------------------------------------------
�޸ļ�¼��
	����	        �汾        �޸���        ��ϵ��ʽ
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
	�����ƣ�xmMui
	��  �ܣ�Model Using Information
	˵  �����������ʹ��ģ�ʹ���ʵ������¼model.config�ļ���<MODEL>/<INSTANCE>Ԫ����Ϣ
	*******************************************************************************/
	class xmMui
	{
	public:
		//	model_configΪ��model.config/ROOT/MODEL�ֶ�
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
		//	ģ�����еľ�̬�ӿڣ��ɷ���ģ�͵�XML�����ļ����
		const xmMdi* const m_pMdi;
		//	ģ������
		const xmString m_strModelName;

		//	ʵ������
		xmString m_strInstanceName;
		//	ʵ�����������ļ�
		xmString m_strInstanceConfigureFile;
		//	ʵ��Ĭ��ƥ��Ŀ��
		xmEDataMatchTarget m_eDefaultDataMatchTarget;
		//	ʵ��Ĭ�����ݼ�
		xmString m_strDefaultDataSet;
		//	ʵ���Ƿ��Զ�����
		bool m_bIsAutoRun;
		//	ʵ������ƥ����Ϣ
		std::map<xmString, xmPtr<xmRuntimeData::SMatchInfo>> m_mapDataMatchInfo;
		
		void _MergeMatchInfo(const xmModelData& modelData, xmRuntimeData::SMatchInfo& miTarget, const xmRuntimeData::SMatchInfo& miOther) const;
		void _UpdateMatchInfo(const xmModelData& modelData, const xmRuntimeData::SMatchInfo& matchInfo);

		//	ͨ��<DATA>��<INCLUDE_TAG>��<INCLUDE_GROUP>������ǩ��ѡ��ָ��IO���͵�ģ��ʵ������
		size_t _SelectData(tinyxml2::XMLElement* element, xmEIOType ioType, std::map<const xmModelData*, int>& dataMap);
		void _Match_INPUT_TARGET(tinyxml2::XMLElement* data_match);
		void _Match_OUTPUT_TARGET(tinyxml2::XMLElement* data_match);
		void _Match_IO_PROCESSOR(tinyxml2::XMLElement* data_match, xmEIOType ioType);
		void _Match_OUTPUT_DATASET(tinyxml2::XMLElement* data_match);
		//	ͨ��<DATA>��<INNER_DATA>��<INPUT_DATA>��<OUTPUT_DATA>�ĸ���ǩ�������ݽ���ƥ��
		void _MatchData(tinyxml2::XMLElement* data_match, bool bAutoType, xmEIOType ioType = IOT_INNER);
	};
}
#endif	//	__SSA_UMSF_MUI_H