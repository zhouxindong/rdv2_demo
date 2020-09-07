#ifndef __SSA_UMSF_LUAENGINE_H
#define __SSA_UMSF_LUAENGINE_H
#include "sengine.h"
#include "lua/include/lua.hpp"
#include <map>

namespace ssa
{
	class xmLuaEngine : public xmScriptEngine
	{
	public:
		//	strScript����ԡ�@����ͷ�����ʾһ���ű��ļ�·���������ʾ�ű�����
		xmLuaEngine(const char* strScript, const char* strScriptAbsPath);
		virtual ~xmLuaEngine();
		virtual EEngineType Type() const
		{
			return etLUA;
		}
	public:
		//	���ؽű�
		void load();

		//	ע�ṩ�ű�ʹ�õ�ȫ�ֺ�����ȫ������
		void register_function(const char* strName, lua_CFunction fn);

		//	дע���
		int write_pointer(const char* key, void* value)
		{
			return write_pointer(L, key, value);
		}
		//	��ע���
		void* read_pointer(const char* key)
		{
			return read_pointer(L, key);
		}
		//	��xmValueѹ��ջ
		xmRet push_value(const xmValue& aValue)
		{
			return push_value(L, aValue);
		}
		//	��ջ����Ԫ�ظ�ֵ��ָ�����͵�xmValue������ɹ����򵯳���Ԫ��
		xmRet pop_value(xmValue& aValue)
		{
			return pop_value(L, aValue);
		}

		//	���ַ�������ѹ��ջ
		int push_vstring(const xmPtr<const char *[]>& vString, size_t uCount)
		{
			return push_vstring(L, vString, uCount);
		}

		//	����LUA����
		int load_function(const char* name);
		//	����LUA����
		int call_function(int nParamCnt, int nReturnCnt);

	protected:
		lua_State* const L;

		static int write_pointer(lua_State* ls, const char* key, void* value);
		static void* read_pointer(lua_State* ls, const char* key);
		static xmRet push_value(lua_State* ls, const xmValue& aValue);
		//	��ջ��Ԫ�ص�������ֵ��aValue�������Ƿ�ֵ�ɹ���ջ��Ԫ�ؾ��ᱻ������
		static xmRet pop_value(lua_State* ls, xmValue& aValue);
		static int push_vstring(lua_State* ls, const xmPtr<const char *[]>& vString, size_t uCount);
		static void set_return(lua_State *L, const xmValue& aValue)
		{
			push_value(L, aValue);
		}
		static void set_return(lua_State *L, const xmPtr<const char *[]>& vString, size_t uCount)
		{
			push_vstring(L, vString, uCount);
		}

		//	����LUA�ű�����֧�ֵĹ��ú���
		static int ssa_LeftShift(lua_State *L);
		static int ssa_RightShift(lua_State *L);
		static int ssa_AND(lua_State *L);
		static int ssa_OR(lua_State *L);
		static int ssa_XOR(lua_State *L);
		static int ssa_NOT(lua_State *L);

		static int ssa_GetByte(lua_State *L);
		static int ssa_SetByte(lua_State *L);
		static int ssa_ReverseByte(lua_State *L);

		static int ssa_GetErrorMessage(lua_State *L);

		static int ssa_Log(lua_State *L);
	};
}

#endif
