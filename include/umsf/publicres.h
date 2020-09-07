/******************************************************************************/
/** @file publicres.h
@brief
	UMSF�����У���������Ԥ�������ȹ�����Դ��ʹ�ýӿڡ�
@author �˻� dehxya@163.com
@par ����ģ��
	umsf
@par ����ļ�
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
		//	��Ϣ����ӿ�
		std::vector<xmMsgHandler*> m_vHandler;
		//	���ݱ�
		xmOrderMap<xmString, xmPtr<xmValue>> m_mapData;
		//	��������·��
		std::multimap<xmString, xmString> m_mapSensitivePath;
	};


	class xmManager;
	class xmPublicResource
	{
		friend xmManager;
	public:
		//	����ϵͳ�ӿ�
		xmISystem* System()
		{
			return m_pSystem;
		}
		//	�ڵ�ӿ�
		xmINode* Node()
		{
			return m_pNodeScope;
		}

		//	��Ӵ���Ҫע�ᵽ����ϵͳ�е���������
		void AddSensData(const xmString& strSysDataName);

		//	ʹ��Ԥ�������ļ�·����ע��һ���µ�Ԥ��������������ļ��Ѿ��򿪣���ֱ�ӷ���
		//	���򣬴���������һ���µ�Ԥ��������
		xmIProcessor* RegisterProcessor(const xmString& strFilePath, const xmString& strFileDir, xmMrtc* pMrtc, const xmString& strDataName);

		//	��ȡһ��Ԫ�ص�ENABLE���ԣ����������ENABLE���ԣ��򷵻�Ĭ��ֵ
		static bool IsENABLE(tinyxml2::XMLElement* element, bool bDefaultValue = true)
		{
			bool bIsEnabled = bDefaultValue;
			if (element != NULL)
			{
				element->QueryBoolAttribute("ENABLE", &bIsEnabled);
			}
			return bIsEnabled;
		}

		//	����msΪ��λ��ʱ�䣬��������������ڣ�ת��Ϊ��������
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
		//	���к���ֻ����xmManager
		//	xmManager����������Դ�󣬹�����MRM��MRTC������ʹ��
		xmPublicResource(void);
		~xmPublicResource(void);

		//	���������ٷ���ϵͳ
		xmRet CreateSystem(const char* strInterfaceFile, const char* pProperty, bool bClear);
		void DestroySystem();

		//	���������ϵͳ/�ڵ���ע�����ݴ������е���������
		xmRet RegisterSensData();

	private:
		//	����ϵͳ�ӿڣ�Ψһ��������ģ��ʹ��
		xmISystem* m_pSystem;
		//	����ϵͳ�������ؿ�
		xmLib m_SystemDll;
		typedef xmISystem* (*tCreateSystem)(const char* strConfig);
		typedef void(*tDestroySystem)(xmISystem* pSystem);
		tCreateSystem m_pfnCreateSystem;
		tDestroySystem m_pfnDestroySystem;

		//	���ؽڵ����ݿռ�
		xmPtr<xmNodeScope> m_pNodeScope;

		//	��¼���д�ע�ᵽϵͳ����������
		std::map<xmString, int> m_mapPendingDataName;
		//	����Ԥ������
		xmOrderMap<xmString, xmIProcessor*> m_mapProcessor;
	};
}
#endif	//	__SSA_UMSF_MANAGER_H