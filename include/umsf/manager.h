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
#include "smfstatistics.h"

namespace ssa
{
	class xmUMSF;
	class xmManager : public xmMsgHandler, public xmSMFStatistics
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

		//	����UMSF��������Ϣ���г���ͳ��
		size_t TotalMsgCount() const;
		void TotalMsgPause();
		void TotalMsgResume();


		//	������������ͳ��
		virtual void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)log_WLS, this);
		}
	private:
		//	�����������
		static void log_WLS(double fWLPrecent, void* pParam)
		{
			xmLog::info(((xmManager*)pParam)->m_strFlag, "Work load %.2f%% pre second.", ((xmMsgHandler*)pParam)->WorkLoad());
		}


	private:
		//	����xmUMSFʹ��
		xmManager(void);
		~xmManager(void);

		xmRet Initialize(const char* strUmsfConfigFile, const char* strModelConfigFile);
		xmRet Release(unsigned int uTimeoutMillis = INFINITE);
		bool IsRunning() const;
		xmString m_strLastError;

	private:
		xmString m_strNodeID;
		xmString m_strNodeName;
		bool     m_bSystemLog;		//	�Ƿ��¼����ϵͳ����������־��Ϣ

		//	�����¼����ע�ᵽ����ϵͳ�е��������
		typedef xmOrderMap<xmString, xmPtr<xmClass>> t_CombineClassMap;
		//	����MRM
		typedef xmOrderMap<xmString, xmPtr<xmMrm> > t_MrmMap;
		//	����MRTC
		typedef xmOrderMap<xmString, xmMrtc*> t_MrtcPtrMap;


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
		void _LoadModel(tinyxml2::XMLElement* root);
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
		void __log_level_config(size_t uTargetID, void* configField);

		//	1����������Ƿ��ظ�
		//	2������ϵͳ��ע������
		//	3����¼�������
		void iSystem_RegisterData(const xmString& strDataName, const xmValue& aValue, xmClass* pClass, const xmString& strAttribute = NULL);
		//	����·��
		xmString _MakePath(const xmString& strPathHead, const xmString& strPathTail);

		//	�ַ���Ϣ
		void DispatchMsg(const xmMessage& aMsg)
		{
			for (size_t i = 0; i < m_mapMrm.Size(); i++)
			{
				m_mapMrm.IndexValue(i)->DispatchMsg(aMsg);
			}
		}
		//	����ϵͳ��Ϣ��Ӧ
		void __OnWorldClick(const smWorldClick& aMsg);
		void __OnSystemTime(const smSystemTime& aMsg);
		void __OnDataUpdate(const smDataUpdate& aMsg);
		void __OnCtrlCommand(const smCtrlCommand& aMsg);
		void __OnSystemEvent(const smSystemEvent& aMsg);
		void __OnBulletinGet(const smBulletinGet& aMsg);
		void __OnMessageGet(const smMessageGet& aMsg);
		void __OnSystemLog(const smSystemLog& aMsg);
		void OnWorldClick(const smWorldClick& aMsg);
		void OnSystemTime(const smSystemTime& aMsg);
		void OnDataUpdate(const smDataUpdate& aMsg);
		void OnCtrlCommand(const smCtrlCommand& aMsg);
		void OnSystemEvent(const smSystemEvent& aMsg);
		void OnBulletinGet(const smBulletinGet& aMsg);
		void OnMessageGet(const smMessageGet& aMsg);
		void OnSystemLog(const smSystemLog& aMsg);
		xmCREATE_MESSAGE_MAP(xmManager);
	};
}
#endif	//	__SSA_UMSF_MANAGER_H