/*******************************************************************************
Copyright (C), 2011-2022, ACC12. All rights reserved.
模 块 名：xmumsf
文 件 名：mrm.h
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
	类名称：xmMrm
	功  能：Model Running Manager
	说  明：负责模型和VDR之间的数据、类匹配，在VDR中注册类和数据，驱动模型计算和数据IO
	*******************************************************************************/
	class xmMrm
	{
	public:
		//	model_config为：model.config/ROOT/MODEL字段
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


		//	获取模型实例数量
		size_t InstanceCount() const
		{
			return m_mapModelInstance.Size();
		}
		//	获取模型实例
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

		//	xmLog::todo() 动态创建和删除实例，此接口部分完成：
		//	（1）可以根据MUI创建一个实例，但是尚未将输出数据注册到网络中
		//	（2）可以根据名称删除一个实例，但是尚未将已注册的VDR数据删除、未将注册的敏感数据取消
		xmMrtc* AddInstance(const xmMui* pMui);
		xmRet DeleteInstance(const xmString& strInstanceName);

		//	下面函数是对所有实例都进行相同的操作
		//	获取模型驱动节拍数
		//	通过模型配置文件中的驱动周期设置和系统节拍周期计算得出
		unsigned int GetDriveClick() const
		{
			return m_uDriveClick;
		}

		//	复位本模型全部实例的组激活状态
		void ResetGroupEnable(bool bIsEnabled)
		{
			for (size_t i = 0; i < m_mapModelInstance.Size(); i++)
			{
				m_mapModelInstance.IndexValue(i)->ResetGroupEnable(bIsEnabled);
			}
		}

		//	启动本模型全部实例
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

		//	用于将一个Message分发给不同的模型实例
		void DispatchMsg(const xmMessage& aMessage)
		{
			for (size_t i=0; i<m_mapModelInstance.Size(); i++)
			{
				m_mapModelInstance.IndexValue(i)->TestActiveAndPostMessage(aMessage);
			}
		}

	private:
		xmPublicResource* const m_pPublicRes;

		//	模型名称
		xmString m_strModelName;
		//	模型描述信息
		xmString m_strModelDes;

		//	模型运行的静态接口，由分析模型的XML数据文件获得
		xmPtr<xmMdi> m_pMdi;

		//	模型计算文件绝对路径
		xmString m_strModelCalculationFile;
		//	模型描述文件绝对路径
		xmString m_strModelDescriptionFile;

		//	驱动节拍数，单位：节拍数
		unsigned int m_uDriveClick;

		//	模型实例，一个模型类可以生成多个模型实例
		typedef xmOrderMap<xmString, xmPtr<xmMrtc> > t_MrtcMap;
		t_MrtcMap m_mapModelInstance;
	};
}
#endif	//	__SSA_UMSF_MRM_H