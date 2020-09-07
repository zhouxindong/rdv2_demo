/******************************************************************************/
/** @file smfstatistics.h
@brief
	����ͳ�ƽ���ϵͳ��Ϣ��ִ��Ч�ʵĽӿڡ�
@author �˻� dehxya@163.com
@par ����ģ��
	umsf
@par ����ļ�
	smfstatistics.cpp
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_UMSF_SMFSTATISTICS_H
#define __SSA_UMSF_SMFSTATISTICS_H
#include "idefine.h"
#include "../base/base.h"

namespace ssa
{
	class xmSMFStatistics
	{
	public:
		xmSMFStatistics(const xmString& strFlag);
		virtual ~xmSMFStatistics() {};

		void ResetExecuteTime();

		//	�ص�����ִ��ʱ��ͳ��
		xmRet ExecuteTimeStatistics(xmESystemMessageFunction eFunc, bool bDo);
		double ExecuteTime(xmESystemMessageFunction eFunc);
		bool IsExecuteTimeStatistics(xmESystemMessageFunction eFunc) const;

		//	������������ͳ��
		virtual void StartWLS(unsigned int uTime) = 0;

	protected:
		const xmString m_strFlag;

		
		//	ģ����Ϣ��Ӧ�Ĵ���ʱ�䣬��λ������
		xmCALC_RUNTIME(SMF_ETS);
		bool   m_bExecuteTime[SMF_ALL];
		double m_fExecuteTime[SMF_ALL];


		//	��¼��Ϣ��Ӧ����ִ�к�ʱ
		void log_ETS(const xmString& strFunction)
		{
			xmLog::info(m_strFlag, "%s execute time %.3fms.", strFunction.String(), xmRUN_TIME(SMF_ETS));
		}
	};
}
#endif	//	__SSA_UMSF_SMFSTATISTICS_H