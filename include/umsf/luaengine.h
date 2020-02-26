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
		xmLuaEngine(const char* strScript);
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
		int push_value(const xmValue& aValue)
		{
			return push_value(L, aValue);
		}
		//	��ջ����Ԫ�ظ�ֵ��ָ�����͵�xmValue������ɹ����򵯳���Ԫ��
		int pop_value(xmValue& aValue)
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
		static int write_pointer(lua_State* ls, const char* key, void* value);
		static void* read_pointer(lua_State* ls, const char* key);
		static int push_value(lua_State* ls, const xmValue& aValue);
		static int pop_value(lua_State* ls, xmValue& aValue);
		static int push_vstring(lua_State* ls, const xmPtr<const char *[]>& vString, size_t uCount);

		lua_State* const L;
	};
}

#endif
