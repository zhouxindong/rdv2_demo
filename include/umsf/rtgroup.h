#ifndef __SSA_UMSF_RTGROUP_H
#define __SSA_UMSF_RTGROUP_H
#include "rtdata.h"
#include "trigger.h"

namespace ssa
{
	class xmMrtc;
	class xmModelGroup;
	class xmRuntimeGroup
	{
		friend xmMrtc;
	public:
		xmRuntimeGroup(const xmModelGroup* pModelGroup, xmEIOType eIOType);
		virtual ~xmRuntimeGroup();
		const xmModelGroup& ModelGroup() const
		{
			return *m_pModelGroup;
		}
		const xmString& Name() const;
		const char* Attribute(const char* strAttrName) const;
		xmEIOType IOType() const
		{
			return m_eIOType;
		}
		bool IsEnabled() const
		{
			return m_bIsEnabled;
		}
		void SetEnabled(bool bIsEnabled)
		{
			m_bIsEnabled = bIsEnabled;
		}
		bool IsAuto() const;
		void GroupIO(xmPublicResource* pPubRes);

		xmString PrintInfo() const;
	protected:
		std::vector<xmRuntimeData*> m_DataList;
		xmTrigger* m_pTrigger;
		bool m_bIsEnabled;
	private:
		const xmModelGroup* const m_pModelGroup;
		const xmEIOType m_eIOType;
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_RTGROUP_H

