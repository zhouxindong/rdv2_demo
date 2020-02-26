#ifndef __SSA_UMSF_TSCRIPT_H
#define __SSA_UMSF_TSCRIPT_H
#include "luaengine.h"
#include "idefine.h"
namespace ssa
{
	class xmMrtc;
	//	ʹ��Lua�Ĵ������ű�
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

		//	����ssa_*���������ǿɹ�LUA�ű����õĺ���
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
		//	ģ�����л���
		xmMrtc* const m_pMrtc;
		//	��������
		std::map<xmString, std::pair<xmEIOType, xmPtr<xmValue>>> m_mapValue;
		//	��������������Ϣ����
		xmPtr<xmValue> m_pParamValue;
	};
}
#endif