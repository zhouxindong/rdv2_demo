/******************************************************************************/
/** @file manager.h
@brief
	����xmManager�ࡣ
@author �˻� dehxya@163.com
@par ����ģ��
	umsf
@par ����ļ�
	manager.cpp
@see ssa::xmUMSF
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_UMSF_MANAGER_H
#define __SSA_UMSF_MANAGER_H
#include "mrm.h"
#include "publicres.h"

namespace ssa
{
	class xmUMSF;
	class xmManager : public xmMsgHandler
	{
		friend xmUMSF;
	public:
		//	����ϵͳ
		xmISystem* System() const
		{
			return m_pPublicRes->System();
		}
		//	����ڵ�
		xmINode* Node()
		{
			return m_pPublicRes->Node();
		}

		xmTimeTriggerContainer* TimeTrigger()
		{
			return m_pPublicRes->m_pTimeTriggerContainer;
		}
		size_t TimeTriggerCount()
		{
			return m_pPublicRes->m_pTimeTriggerContainer->GetTriggerCount();
		}
		void TimeTriggerInfo(size_t uPos, unsigned int& uTriggerTime, unsigned int& uTriggerClick, const char*& strModelName, const char*& strInstanceName, const char*& strGroupName)
		{
			const xmTrigger* pTrigger = m_pPublicRes->m_pTimeTriggerContainer->GetTrigger(uPos);
			uTriggerTime = m_pPublicRes->m_pTimeTriggerContainer->GetTriggerTime(uPos);
			uTriggerClick = m_pPublicRes->m_pTimeTriggerContainer->GetTriggerClick(uPos);
			strModelName = pTrigger->m_strModelName;
			strInstanceName = pTrigger->m_strInstanceName;
			strGroupName = pTrigger->m_strGroupName;
		}

		xmEventTriggerContainer* EventTrigger()
		{
			return m_pPublicRes->m_pEventTriggerContainer;
		}
		size_t EventTriggerCount()
		{
			return m_pPublicRes->m_pEventTriggerContainer->GetTriggerCount();
		}
		void EventTriggerInfo(size_t uPos, xmString& strEvent, const char*& strModelName, const char*& strInstanceName, const char*& strGroupName, xmString& strParamValue)
		{
			const xmTrigger* pTrigger = m_pPublicRes->m_pEventTriggerContainer->GetTrigger(uPos);
			strEvent = pTrigger->PrintTrigger();
			strModelName = pTrigger->m_strModelName;
			strInstanceName = pTrigger->m_strInstanceName;
			strGroupName = pTrigger->m_strGroupName;
			strParamValue = pTrigger->PrintParam();
		}

		size_t DataTriggerCount()
		{
			return m_pPublicRes->m_vDataTriggerContainer.size();
		}
		xmDataTriggerContainer* DataTrigger(size_t uPos)
		{
			return m_pPublicRes->m_vDataTriggerContainer[uPos]->second;
		}
		void DataTriggerInfo(size_t uPos, const char*& strDataModelName, const char*& strDataMatchName, const char*& strModelName, const char*& strInstanceName, const char*& strGroupName, xmString& strParamValue)
		{
			xmDataTriggerContainer* pDataTriggerContainer = m_pPublicRes->m_vDataTriggerContainer[uPos]->second;
			const xmTrigger* pTrigger = pDataTriggerContainer->GetTrigger();

			strDataModelName = pDataTriggerContainer->GetDataName();
			strDataMatchName = pDataTriggerContainer->GetMatchName();
			xmAssert(m_pPublicRes->m_vDataTriggerContainer[uPos]->first == strDataMatchName);
			strModelName = pTrigger->m_strModelName;
			strInstanceName = pTrigger->m_strInstanceName;
			strGroupName = pTrigger->m_strGroupName;
			strParamValue = pTrigger->PrintParam();
		}

		//	����MRM
		size_t ModelCount() const
		{
			return m_mapMrm.Size();
		}
		xmMrm* Mrm(size_t uIndex)
		{
			if (uIndex >= m_mapMrm.Size())
			{
				return NULL;
			}
			return m_mapMrm.IndexValue(uIndex);
		}
		xmMrm* Mrm(const xmString& strModelName)
		{
			t_MrmMap::t_Pos pos = m_mapMrm.Find(strModelName);
			if (pos == m_mapMrm.INVALID_POS())
			{
				return NULL;
			}
			return m_mapMrm.PosValue(pos);
		}

		//	������������ͳ��
		void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)__log_WLS, this);
		}
		//	�ص�����ִ��ʱ��ͳ��
		xmRet ExecuteTimeStatistics(xmEManagerFunction eFunc, bool bDo)
		{
			if (eFunc == MF_ALL)
			{
				for (size_t i = 0; i < MF_ALL; i++)
				{
					m_bExecuteTime[i] = bDo;
				}
			}
			else if (eFunc != MF_ERROR)
			{
				m_bExecuteTime[eFunc] = bDo;
			}
			else
			{
				return xmE_FAIL;
			}
			return xmE_SUCCESS;
		}
		double ExecuteTime(xmEManagerFunction eFunc)
		{
			return (eFunc == MF_ALL || eFunc == MF_ERROR) ? 0.0 : m_fExecuteTime[eFunc];
		}
		//	����UMSF��������Ϣ���г���ͳ��
		size_t TotalMsgCount() const;
	private:
		//	ģ����Ϣ��Ӧ�Ĵ���ʱ�䣬��λ������
		xmCALC_RUNTIME(MANAGER_ETS);
		bool   m_bExecuteTime[MF_ALL];
		double m_fExecuteTime[MF_ALL];
		//	��¼��Ϣ��Ӧ����ִ�к�ʱ
		void __log_ETS(const xmString& strFunction)
		{
			xmLog::info("Manager", "%s execute time %.3fms.",
				strFunction.String(),
				xmRUN_TIME(MANAGER_ETS));
		}
		static void __log_WLS(double fWLPrecent, void* pParam)
		{
			xmLog::info("Manager", "Work load %.2f%% pre second.", ((xmMsgHandler*)pParam)->WorkLoad());
		}
		void __log_level_config(size_t uTargetID, void* configField);
	private:
		//	����xmUMSFʹ��
		xmManager(void);
		~xmManager(void);

		xmRet Initialize(const char* strUmsfConfigFile, const char* strModelConfigFile);
		xmRet Release(unsigned int uTimeoutMillis = INFINITE);
		xmString m_strLastError;

	private:
		//	�����¼����ע�ᵽ����ϵͳ�е��������
		typedef xmOrderMap<xmString, xmPtr<xmClass>> t_CombineClassMap;
		//	����MRM
		typedef xmOrderMap<xmString, xmPtr<xmMrm> > t_MrmMap;
		//	����MRTC
		typedef xmOrderMap<xmString, xmMrtc*> t_MrtcPtrMap;
		//	��������
		typedef std::multimap<xmString, xmPtr<xmDataTriggerContainer> >::iterator t_DataTriggerMapIter;


		//	ģ�����й�������Model Running Manager����һ��ģ�����Ӧһ��MRM
		t_MrmMap m_mapMrm;
		//	ģ������ʱ������Model Runtime Context����һ��ģ��ʵ����Ӧһ��MRTC
		t_MrtcPtrMap m_mapMrtc;
		//	��¼����ע�ᵽ����ϵͳ�е��������
		//	key��ʾ�������ƻ���Ϻ����������
		//	value��ʾ�����ݵ����ͣ����Ϊ�գ���ʾ��������MRTCֱ��ע�ᣬ�����ʾ������Ϻ����������
		t_CombineClassMap m_mapSystemData;

		//	UMSF�����еĹ�����Դ
		//	��������ϵͳ���ʽӿڡ���������Ԥ������
		xmPublicResource* m_pPublicRes;

		//	��¼��־Ŀ��
		std::vector<size_t> m_vLogTarget;

		//	������Ϣ����ʱ�ı���
		mutable xmMutex m_MsgHandleMutex;

		void _LoadUmsfConfig(const char* strUmsfConfigFile);
		void _LoadModelConfig(const char* strModelConfigFile);
		//	����һ��PACKAGE������Ϣ
		//	mapInstance��Ŀǰ��δʹ�õ�ʵ���б�
		//	PACKAGE��xml����
		//	strPath�����PACKAGE������·��
		//	packageName�������������ʾ���Package������
		//	packageValue�������������ʾ���Package����ֵ
		//	������ֵ����ָ��
		xmClass* _LoadPackage(
			xmOrderMap<xmString, xmMrtc*>& mapInstance, 
			xmOrderMap<xmString, std::vector<xmString>>& mapInstanceData,
			tinyxml2::XMLElement* PACKAGE,
			const xmString& strPath, 
			xmString& packageName, 
			xmValue& packageValue);
		//	���vData��ΪNULL�����м�¼�����ݾ����ڼ�¼��ɾ��
		//	����NULL����ʾ����ϣ�vData���ΪNULL����ʾʹ������������ݣ������ʾʹ��vData��ָ�����������
		xmClass* _LoadUsing(
			xmOrderMap<xmString, xmMrtc*>& mapInstance,
			xmOrderMap<xmString, std::vector<xmString>>& mapInstanceData,
			tinyxml2::XMLElement* USING,
			xmMrtc*& pMrtc,						//	ʵ��ָ��
			std::vector<xmString>& usingData,	//	ʹ�õ�����
			xmString& strCombineName,			//	�������
			xmValue& combineValue				//	�����ֵ
		);
		//	1����������Ƿ��ظ�
		//	2������ϵͳ��ע������
		//	3����¼�������
		void iSystem_RegisterData(const xmString& strDataName, const xmValue& aValue, xmClass* pClass, const xmString& strAttribute = NULL);
		//	����·��
		xmString _MakePath(const xmString& strPathHead, const xmString& strPathTail);

		//	����ϵͳ��Ϣ��Ӧ
		void __OnWorldClick(const smWorldClick& aMsg);
		void __OnSystemTime(const smSystemTime& aMsg);
		void __OnDataUpdate(const smDataUpdate& aMsg);
		void __OnCtrlCommand(const smCtrlCommand& aMsg);
		void __OnSystemEvent(const smSystemEvent& aMsg);
		void __OnBulletinGet(const smBulletinGet& aMsg);
		void __OnMessageGet(const smMessageGet& aMsg);
		void OnWorldClick(const smWorldClick& aMsg);
		void OnSystemTime(const smSystemTime& aMsg);
		void OnDataUpdate(const smDataUpdate& aMsg);
		void OnCtrlCommand(const smCtrlCommand& aMsg);
		void OnSystemEvent(const smSystemEvent& aMsg);
		void OnBulletinGet(const smBulletinGet& aMsg);
		void OnMessageGet(const smMessageGet& aMsg);
		xmCREATE_MESSAGE_MAP(xmManager);
	};
}
#endif	//	__SSA_UMSF_MANAGER_H