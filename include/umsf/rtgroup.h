#ifndef __SSA_UMSF_RTGROUP_H
#define __SSA_UMSF_RTGROUP_H
#include "rtobject.h"
#include "rtdata.h"
#include "trigger.h"

namespace ssa
{
	class xmModelGroup;
	class xmModelServer;
	class xmRuntimeGroup : public xmRuntimeObject
	{
		friend xmUMSF;
		friend xmMrtc;
		friend xmModelServer;
	public:
		xmRuntimeGroup(xmMrtc* pMrtc, const xmModelGroup* pModelGroup);
		virtual ~xmRuntimeGroup();
		virtual const xmString& Name() const;
	protected:
		virtual void _MakeFlag() const;

	public:
		const xmModelGroup& ModelGroup() const
		{
			return *m_pModelGroup;
		}
		const xmString& GroupName() const;
		const char* Attribute(const char* strAttrName) const;
		xmEIOType IOType() const;
		bool IsEnabled() const
		{
			return m_bIsEnabled;
		}
		void SetEnabled(bool bIsEnabled);
		bool IsAuto() const;

		const xmTrigger* Trigget() const
		{
			return m_pTrigger;
		}
		const std::vector<xmRuntimeData*>& DataList() const
		{
			return m_DataList;
		}


		//	返回IO错误的数据个数
		int GroupIO();


		xmString PrintInfo() const;
		const xmString& PrintXml() const;

		xmRet GetLastErrorCode_GroupIO() const
		{
			return m_nLEC_GroupIO;
		}
		const xmString& GetLastErrorMessage_GroupIO() const
		{
			return m_strLEM_GroupIO;
		}

	protected:
		std::vector<xmRuntimeData*> m_DataList;
		xmTrigger* m_pTrigger;
		bool m_bIsEnabled;
	private:
		const xmModelGroup* const m_pModelGroup;

		xmRet m_nLEC_GroupIO;
		xmString m_strLEM_GroupIO;

		//	返回出错位置
		size_t _GroupIO(size_t uBeginPos);
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_RTGROUP_H

