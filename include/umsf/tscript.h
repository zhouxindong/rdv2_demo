#ifndef __SSA_UMSF_TSCRIPT_H
#define __SSA_UMSF_TSCRIPT_H
#include "mascript.h"
namespace ssa
{
	class xmMrtc;
	//	使用Lua的触发器脚本
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

		//	所有ssa_*函数，都是可供LUA脚本调用的函数
		static int ssa_PreInput(lua_State *L);
		static int ssa_GetParam(lua_State *L);

		virtual bool LockData() const
		{
			return true;
		}

	private:
		//	触发器附带的消息参数
		xmPtr<xmValue> m_pParamValue;

		const xmString m_strFlag;
	};
}
#endif