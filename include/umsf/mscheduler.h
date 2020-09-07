/******************************************************************************/
/** @file mscheduler.h
@brief
	ģ�͵��ȳ������з��͸�ģ�͵���Ϣ�������䴦��
@author �˻� dehxya@163.com
@par ����ģ��
	umsf
@par ����ļ�
	publicres.cpp
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_UMSF_MSCHEDULER_H
#define __SSA_UMSF_MSCHEDULER_H
#include "trigger.h"
#include "publicres.h"
#include "smfstatistics.h"

namespace ssa
{
	class xmModelScheduler : public xmMsgHandler, public xmSMFStatistics
	{
	public:
		xmModelScheduler(xmMrtc* pMrtc);
		~xmModelScheduler(void);

		const xmString& Flag() const
		{
			return m_strFlag;
		}

		//	ע�ᴥ����
		void RegisterTrigger(xmTrigger* pTrigger);

		//	��ȡ����
		const xmTimeTriggerContainer* TimeTriggerContainer() const
		{
			return m_pTimeTriggerContainer;
		}
		const xmEventTriggerContainer* EventTriggerContainer() const
		{
			return m_pEventTriggerContainer;
		}
		const xmCommandTriggerContainer* CommandTriggerContainer() const
		{
			return m_pCommandTriggerContainer;
		}
		const xmDataTriggerContainer* DataTriggerContainer() const
		{
			return m_pDataTriggerContainer;
		}


		//	����ͳ�ƹ������غ�ִ��ʱ��
		virtual void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)log_WLS, this);
		}
	private:
		//	�����������
		static void log_WLS(double fWLPrecent, void* pParam)
		{
			xmLog::info(((xmModelScheduler*)pParam)->m_strFlag, "Work load %.2f%% pre second.", ((xmMsgHandler*)pParam)->WorkLoad());
		}

	private:
		xmMrtc* const m_pMrtc;
		const xmString m_strFlag;

		//	������
		xmPtr<xmTimeTriggerContainer> m_pTimeTriggerContainer;
		xmPtr<xmEventTriggerContainer> m_pEventTriggerContainer;
		xmPtr<xmCommandTriggerContainer> m_pCommandTriggerContainer;
		xmPtr<xmDataTriggerContainer> m_pDataTriggerContainer;
		
	private:
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
		xmCREATE_MESSAGE_MAP(xmModelScheduler);
	};
}
#endif	//	__SSA_UMSF_MSCHEDULER_H