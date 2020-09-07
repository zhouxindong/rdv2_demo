#ifndef __SSA_UMSF_TSCRIPT_H
#define __SSA_UMSF_TSCRIPT_H
#include "mascript.h"
namespace ssa
{
	class xmMrtc;
	//	ʹ��Lua�Ĵ������ű�
	class xmTriggerScript : public xmModelAssistScript
	{
	public:
		xmTriggerScript(const char* strScript, const char* strScriptAbsPath, xmMrtc* pMrtc, const xmString& strGroupName);
		virtual ~xmTriggerScript() {};

		bool IsTriggered(const xmValue* pParam);
		const xmString& Flag() const
		{
			return m_strFlag;
		}

	protected:
		static xmTriggerScript* get_xmTriggerScript(lua_State *L)
		{
			return (xmTriggerScript*)read_pointer(L, "xmTriggerScript");
		}

		//	����ssa_*���������ǿɹ�LUA�ű����õĺ���
		static int ssa_PreInput(lua_State *L);
		static int ssa_GetParam(lua_State *L);

		virtual bool LockData() const
		{
			return true;
		}

	private:
		//	��������������Ϣ����
		xmPtr<xmValue> m_pParamValue;

		const xmString m_strFlag;
	};
}
#endif