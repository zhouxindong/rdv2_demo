#ifndef __SSA_MODEL_ASSIIST_SCRIPT_H
#define __SSA_MODEL_ASSIIST_SCRIPT_H
#include "luaengine.h"
#include "idefine.h"

namespace ssa
{
	class xmMrtc;
	class xmRuntimeData;
	class xmModelAssistScript : public xmLuaEngine
	{
	public:
		xmModelAssistScript(const xmString& strPath, const xmString& strScriptAbsPath, xmMrtc* pMrtc);
		virtual ~xmModelAssistScript() {};

	protected:
		xmMrtc* const m_pMrtc;

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
		static int ssa_GetData(lua_State *L);
		static int ssa_PrintData(lua_State *L);

		static xmModelAssistScript* get_xmModelAssistScript(lua_State *L)
		{
			return (xmModelAssistScript*)read_pointer(L, "xmModelAssistScript");
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
		
		static int PreInput(lua_State *L, const xmString& strDataName, bool bSyncNow);

		//	模型在调用ssa_GetData()和PreInput()接口时，如果在数据预处理器中调用，则不能加锁，否则会与MRTC线程锁死
		//	在触发器中可以调用，因为触发器运行在触发器线程或者主线程
		virtual bool LockData() const = 0;
	private:
		//	参数缓存
		std::map<xmString, std::pair<xmEIOType, xmPtr<xmValue>>> m_mapValue;
		//	数据缓存
		std::map<xmString, xmRuntimeData*> m_mapData;
	};
}
#endif  //  __SSA_UMSF_PROCESSOR_LUA_H

