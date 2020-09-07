/******************************************************************************/
/** @file smfstatistics.h
@brief
	定义统计接收系统消息类执行效率的接口。
@author 邓华 dehxya@163.com
@par 所属模块
	umsf
@par 相关文件
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

		//	回调函数执行时间统计
		xmRet ExecuteTimeStatistics(xmESystemMessageFunction eFunc, bool bDo);
		double ExecuteTime(xmESystemMessageFunction eFunc);
		bool IsExecuteTimeStatistics(xmESystemMessageFunction eFunc) const;

		//	启动工作负载统计
		virtual void StartWLS(unsigned int uTime) = 0;

	protected:
		const xmString m_strFlag;

		
		//	模型消息响应的处理时间，单位：毫秒
		xmCALC_RUNTIME(SMF_ETS);
		bool   m_bExecuteTime[SMF_ALL];
		double m_fExecuteTime[SMF_ALL];


		//	记录消息响应函数执行耗时
		void log_ETS(const xmString& strFunction)
		{
			xmLog::info(m_strFlag, "%s execute time %.3fms.", strFunction.String(), xmRUN_TIME(SMF_ETS));
		}
	};
}
#endif	//	__SSA_UMSF_SMFSTATISTICS_H