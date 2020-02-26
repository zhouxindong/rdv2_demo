/******************************************************************************/
/** @file publicres.h
@brief
	UMSF运行中，触发器、预处理器等公共资源的使用接口。
@author 邓华 dehxya@163.com
@par 所属模块
	umsf
@par 相关文件
	publicres.cpp
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_UMSF_PUBLICRES_H
#define __SSA_UMSF_PUBLICRES_H
#include "isystem.h"
#include "inode.h"
#include "trigger.h"
#include "processor.h"
#include "tinyxml2/tinyxml2.h"

namespace ssa
{
	class xmNodeScope : public xmINode
	{
	public:
		xmNodeScope() {};
		virtual ~xmNodeScope() {};

		virtual xmRet RegisterMsgHandler(xmMsgHandler* pHandler);
		virtual xmRet RegisterData(const xmString& strDataName, const xmValue& aValue, const xmString& strAttribute = NULL);
		virtual xmRet RegisterSensDate(const xmString& strDataPath);
		virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue);
		virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue);
		virtual xmString PrintData(const xmString& strDataName = NULL);

		xmValue* FindData(const xmString strDataName);

	private:
		mutable xmMutex m_HandlerMutex;
		mutable xmMutex m_DataMutex;
		//	消息处理接口
		std::vector<xmMsgHandler*> m_vHandler;
		//	数据表
		xmOrderMap<xmString, xmPtr<xmValue>> m_mapData;
		//	敏感数据路径
		std::multimap<xmString, xmString> m_mapSensitivePath;
	};


	class xmManager;
	class xmPublicResource
	{
		friend xmManager;
	public:
		//	仿真系统接口
		xmISystem* System()
		{
			return m_pSystem;
		}
		//	节点接口
		xmINode* Node()
		{
			return m_pNodeScope;
		}

		//	注册触发器，整个UMSF共用触发器
		void RegisterTrigger(xmTrigger* pTrigger);
		//	使用预处理器文件路径，注册一个新的预处理器，如果该文件已经打开，则直接返回
		//	否则，创建并返回一个新的预处理器。
		xmIProcessor* RegisterProcessor(const xmString& strFilePath);

		//	获取一个元素的ENABLE属性，如果不存在ENABLE属性，则返回默认值
		static bool IsENABLE(tinyxml2::XMLElement* element, bool bDefaultValue = true)
		{
			bool bIsEnabled = bDefaultValue;
			if (element != NULL)
			{
				element->QueryBoolAttribute("ENABLE", &bIsEnabled);
			}
			return bIsEnabled;
		}

		//	将以ms为单位的时间，根据网络节拍周期，转换为驱动节拍
		static unsigned int Time2Click(unsigned int uTime, unsigned int uClickCycle)
		{
			if (uTime == 0)
			{
				return 0;
			}
			xmAssertAlways(uClickCycle != 0);
			unsigned int uClick = uTime / uClickCycle;
			return (uClick == 0) ? 1 : uClick;
		}

	private:
		//	下列函数只能由xmManager
		//	xmManager创建公共资源后，供所有MRM和MRTC及数据使用
		xmPublicResource(void);
		~xmPublicResource(void);

		//	预销毁公共资源，主要进行触发器的销毁工作
		void PreDestroy();

		//	创建和销毁仿真系统
		xmRet CreateSystem(const char* strInterfaceFile, const char* pProperty);
		void DestroySystem();

		//	尝试向仿真系统/节点中注册数据触发器中的敏感数据
		xmRet RegisterSensData();
		//	向仿真系统/节点中再次注册数据触发器中的敏感数据
		xmRet RetryRegisterSensData();

	private:
		//	仿真系统接口，唯一，供所有模型使用
		xmISystem* m_pSystem;
		//	仿真系统驱动加载库
		xmLib m_SystemDll;
		typedef xmISystem* (*tCreateSystem)(const char* strConfig);
		typedef void(*tDestroySystem)(xmISystem* pSystem);
		tCreateSystem m_pfnCreateSystem;
		tDestroySystem m_pfnDestroySystem;

		//	本地节点数据空间
		xmPtr<xmNodeScope> m_pNodeScope;

		//	触发器
		xmPtr<xmTimeTriggerContainer> m_pTimeTriggerContainer;
		xmPtr<xmEventTriggerContainer> m_pEventTriggerContainer;
		//	记录所有数据触发器，key是触发器敏感数据在系统中的名称
		std::multimap<xmString, xmPtr<xmDataTriggerContainer> > m_mapDataTriggerContainer;
		std::vector<std::multimap<xmString, xmPtr<xmDataTriggerContainer>>::iterator> m_vDataTriggerContainer;
		//	记录所有注册失败的数据触发器
		std::multimap<xmString, xmDataTriggerContainer*> m_mapPendingDataTriggerContainer;
		//	数据预处理器
		xmOrderMap<xmString, xmIProcessor*> m_mapProcessor;
	};
}
#endif	//	__SSA_UMSF_MANAGER_H