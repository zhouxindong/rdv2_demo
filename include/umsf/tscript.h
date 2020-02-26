#ifndef __SSA_UMSF_TSCRIPT_H
#define __SSA_UMSF_TSCRIPT_H
#include "luaengine.h"
#include "idefine.h"
namespace ssa
{
	class xmMrtc;
	//	使用Lua的触发器脚本
	class xmTriggerScript : public xmLuaEngine
	{
	public:
		xmTriggerScript(const char* strScript, xmMrtc* pMrtc);
		virtual ~xmTriggerScript() {};

		bool IsTriggered(const xmValue* pParam);
	private:
		bool call_trigger(const char* strFuncName);
	protected:
		static xmTriggerScript* get_this(lua_State *L)
		{
			return (xmTriggerScript*)read_pointer(L, "THIS");
		}
		static void set_return(lua_State *L, const xmValue& aValue, xmRWLock& rwl)
		{
			xmRWLock::ReadGuard rg(rwl);
			push_value(L, aValue);
		}
		static void set_return(lua_State *L, const xmValue& aValue)
		{
			push_value(L, aValue);
		}

		//	所有ssa_*函数，都是可供LUA脚本调用的函数
		static int ssa_Log(lua_State *L);
		static int ssa_GetSimulateState(lua_State *L);
		static int ssa_GetSimulateSpeed(lua_State *L);
		static int ssa_GetSystemTime(lua_State *L);
		static int ssa_GetClickCount(lua_State *L);
		static int ssa_GetClickCycle(lua_State *L);
		static int ssa_GetDriveTime(lua_State *L);
		static int ssa_GetDriveClick(lua_State *L);
		static int ssa_ModelName(lua_State *L);
		static int ssa_InstanceName(lua_State *L);
		static int ssa_GetParam(lua_State *L);
		static int ssa_GetData(lua_State *L);
		//static int ssa_ManualDataInput(lua_State *L);
		//static int ssa_ManualDataOutput(lua_State *L);

	private:
		//	模型运行环境
		xmMrtc* const m_pMrtc;
		//	参数缓存
		std::map<xmString, std::pair<xmEIOType, xmPtr<xmValue>>> m_mapValue;
		//	触发器附带的消息参数
		xmPtr<xmValue> m_pParamValue;
	};
}
#endif