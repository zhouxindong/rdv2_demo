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
		virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO);
		virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO);
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

		//	添加打算要注册到仿真系统中的敏感数据
		void AddSensData(const xmString& strSysDataName);

		//	使用预处理器文件路径，注册一个新的预处理器，如果该文件已经打开，则直接返回
		//	否则，创建并返回一个新的预处理器。
		xmIProcessor* RegisterProcessor(const xmString& strFilePath, const xmString& strFileDir, xmMrtc* pMrtc, const xmString& strDataName);

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

		//	创建和销毁仿真系统
		xmRet CreateSystem(const char* strInterfaceFile, const char* pProperty, bool bClear);
		void DestroySystem();

		//	尝试向仿真系统/节点中注册数据触发器中的敏感数据
		xmRet RegisterSensData();

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

		//	记录所有待注册到系统的敏感数据
		std::map<xmString, int> m_mapPendingDataName;
		//	数据预处理器
		xmOrderMap<xmString, xmIProcessor*> m_mapProcessor;
	};
}
#endif	//	__SSA_UMSF_MANAGER_H