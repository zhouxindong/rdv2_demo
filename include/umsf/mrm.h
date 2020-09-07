/*******************************************************************************
Copyright (C), 2011-2022, ACC12. All rights reserved.
ģ �� ����xmumsf
�� �� ����mrm.h
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
#ifndef __SSA_UMSF_MRM_H
#define __SSA_UMSF_MRM_H
#include "publicres.h"
#include "mdi.h"
#include "mui.h"
#include "mrtc.h"
#include "tinyxml2/tinyxml2.h"
#include "udefine.h"

namespace ssa
{
	/*******************************************************************************
	�����ƣ�xmMrm
	��  �ܣ�Model Running Manager
	˵  ��������ģ�ͺ�VDR֮������ݡ���ƥ�䣬��VDR��ע��������ݣ�����ģ�ͼ��������IO
	*******************************************************************************/
	class xmMrm
	{
	public:
		//	model_configΪ��model.config/ROOT/MODEL�ֶ�
		xmMrm(xmPublicResource* pPubRes);
		virtual ~xmMrm();

	public:
		void Load(tinyxml2::XMLElement* model_config);
		void Unload();

		const xmString& ModelName() const
		{
			return m_strModelName;
		}

		const xmMdi* Mdi() const
		{
			return m_pMdi;
		}


		//	��ȡģ��ʵ������
		size_t InstanceCount() const
		{
			return m_mapModelInstance.Size();
		}
		//	��ȡģ��ʵ��
		xmMrtc* Mrtc(size_t uIndex)
		{
			if (uIndex >= m_mapModelInstance.Size())
			{
				return NULL;
			}
			return m_mapModelInstance.IndexValue(uIndex);
		}
		xmMrtc* Mrtc(const xmString& strInstanceName)
		{
			t_MrtcMap::t_Pos pos = m_mapModelInstance.Find(strInstanceName);
			if (pos == m_mapModelInstance.INVALID_POS())
			{
				return NULL;
			}
			return m_mapModelInstance.PosValue(pos);
		}

		//	xmLog::todo() ��̬������ɾ��ʵ�����˽ӿڲ�����ɣ�
		//	��1�����Ը���MUI����һ��ʵ����������δ���������ע�ᵽ������
		//	��2�����Ը�������ɾ��һ��ʵ����������δ����ע���VDR����ɾ����δ��ע�����������ȡ��
		xmMrtc* AddInstance(const xmMui* pMui);
		xmRet DeleteInstance(const xmString& strInstanceName);

		//	���溯���Ƕ�����ʵ����������ͬ�Ĳ���
		//	��ȡģ������������
		//	ͨ��ģ�������ļ��е������������ú�ϵͳ�������ڼ���ó�
		unsigned int GetDriveClick() const
		{
			return m_uDriveClick;
		}

		//	��λ��ģ��ȫ��ʵ�����鼤��״̬
		void ResetGroupEnable(bool bIsEnabled)
		{
			for (size_t i = 0; i < m_mapModelInstance.Size(); i++)
			{
				m_mapModelInstance.IndexValue(i)->ResetGroupEnable(bIsEnabled);
			}
		}

		//	������ģ��ȫ��ʵ��
		void CreateInstance(void)
		{
			for (size_t i = 0; i < m_mapModelInstance.Size(); i++)
			{
				if (m_mapModelInstance.IndexValue(i)->IsAutoRun())
				{
					m_mapModelInstance.IndexValue(i)->CreateInstance(false);
				}
			}
		}
		void DestroyInstance(void)
		{
			for (size_t i = 0; i < m_mapModelInstance.Size(); i++)
			{
				m_mapModelInstance.IndexValue(i)->DestroyInstance();
			}
		}
		void ReadyToGo(void)
		{
			for (size_t i = 0; i < m_mapModelInstance.Size(); i++)
			{
				m_mapModelInstance.IndexValue(i)->ReadyToGo();
			}
		}

		//	���ڽ�һ��Message�ַ�����ͬ��ģ��ʵ��
		void DispatchMsg(const xmMessage& aMessage)
		{
			for (size_t i=0; i<m_mapModelInstance.Size(); i++)
			{
				m_mapModelInstance.IndexValue(i)->TestActiveAndPostMessage(aMessage);
			}
		}

	private:
		xmPublicResource* const m_pPublicRes;

		//	ģ������
		xmString m_strModelName;
		//	ģ��������Ϣ
		xmString m_strModelDes;

		//	ģ�����еľ�̬�ӿڣ��ɷ���ģ�͵�XML�����ļ����
		xmPtr<xmMdi> m_pMdi;

		//	ģ�ͼ����ļ�����·��
		xmString m_strModelCalculationFile;
		//	ģ�������ļ�����·��
		xmString m_strModelDescriptionFile;

		//	��������������λ��������
		unsigned int m_uDriveClick;

		//	ģ��ʵ����һ��ģ����������ɶ��ģ��ʵ��
		typedef xmOrderMap<xmString, xmPtr<xmMrtc> > t_MrtcMap;
		t_MrtcMap m_mapModelInstance;
	};
}
#endif	//	__SSA_UMSF_MRM_H