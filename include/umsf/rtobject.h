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

		//	����
		virtual const xmString& Name() const = 0;
		//	��ʶ
		const xmString& Flag() const;

		//	��־����
		void LogControl(bool bOpen) const
		{
			m_bLog = bOpen;
		}
		bool IsLogged() const
		{
			return m_bLog;
		}

		//	ִ��ʱ��ͳ��
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

		//	ʹ��ǰ����Ҫ������г�ʼ��
		mutable xmString m_strFlag;
		//	����Flag
		virtual void _MakeFlag() const = 0;

	private:
		//	��־����
		mutable bool m_bLog;
		//	ִ��ʱ��ͳ�ƿ���
		mutable bool m_bExecuteTime;
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_RTOBJECT_H

