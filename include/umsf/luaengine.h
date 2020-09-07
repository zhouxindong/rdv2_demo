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
		//	strScript如果以“@”开头，则表示一个脚本文件路径，否则表示脚本内容
		xmLuaEngine(const char* strScript, const char* strScriptAbsPath);
		virtual ~xmLuaEngine();
		virtual EEngineType Type() const
		{
			return etLUA;
		}
	public:
		//	加载脚本
		void load();

		//	注册供脚本使用的全局函数和全局数据
		void register_function(const char* strName, lua_CFunction fn);

		//	写注册表
		int write_pointer(const char* key, void* value)
		{
			return write_pointer(L, key, value);
		}
		//	读注册表
		void* read_pointer(const char* key)
		{
			return read_pointer(L, key);
		}
		//	将xmValue压入栈
		xmRet push_value(const xmValue& aValue)
		{
			return push_value(L, aValue);
		}
		//	将栈顶的元素赋值给指定类型的xmValue，如果成功，则弹出该元素
		xmRet pop_value(xmValue& aValue)
		{
			return pop_value(L, aValue);
		}

		//	将字符串数组压入栈
		int push_vstring(const xmPtr<const char *[]>& vString, size_t uCount)
		{
			return push_vstring(L, vString, uCount);
		}

		//	载入LUA函数
		int load_function(const char* name);
		//	调用LUA函数
		int call_function(int nParamCnt, int nReturnCnt);

	protected:
		lua_State* const L;

		static int write_pointer(lua_State* ls, const char* key, void* value);
		static void* read_pointer(lua_State* ls, const char* key);
		static xmRet push_value(lua_State* ls, const xmValue& aValue);
		//	将栈顶元素弹出，赋值给aValue。无论是否赋值成功，栈顶元素均会被弹出。
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

		//	所有LUA脚本，都支持的共用函数
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
