#ifndef __SSA_UMSF_RTOBJECT_H
#define __SSA_UMSF_RTOBJECT_H
#include "udefine.h"
namespace ssa
{
	class xmMrtc;
	class xmRuntimeObject
	{
	public:
		xmRuntimeObject(xmMrtc* pMrtc);
		virtual ~xmRuntimeObject() {};

		//	名称
		virtual const xmString& Name() const = 0;
		//	标识
		const xmString& Flag() const;

		//	日志控制
		void LogControl(bool bOpen) const
		{
			m_bLog = bOpen;
		}
		bool IsLogged() const
		{
			return m_bLog;
		}

		//	执行时间统计
		void ExecuteTimeStatistics(bool bOpen) const
		{
			m_bExecuteTime = bOpen;
		}
		double ExecuteTime() const
		{
			return xmRUN_TIME(RTOBJECT_ETS);
		}
		bool IsExecuteTimeStatistics() const
		{
			return m_bExecuteTime;
		}
	protected:
		xmMrtc* const m_pMrtc;
		xmCALC_RUNTIME(RTOBJECT_ETS);

		//	使用前，需要子类进行初始化
		mutable xmString m_strFlag;
		//	生成Flag
		virtual void _MakeFlag() const = 0;

	private:
		//	日志控制
		mutable bool m_bLog;
		//	执行时间统计控制
		mutable bool m_bExecuteTime;
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_RTOBJECT_H

