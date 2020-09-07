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

		//	ģ���ڵ���ssa_GetData()��PreInput()�ӿ�ʱ�����������Ԥ�������е��ã����ܼ������������MRTC�߳�����
		//	�ڴ������п��Ե��ã���Ϊ�����������ڴ������̻߳������߳�
		virtual bool LockData() const = 0;
	private:
		//	��������
		std::map<xmString, std::pair<xmEIOType, xmPtr<xmValue>>> m_mapValue;
		//	���ݻ���
		std::map<xmString, xmRuntimeData*> m_mapData;
	};
}
#endif  //  __SSA_UMSF_PROCESSOR_LUA_H

