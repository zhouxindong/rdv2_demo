#ifndef __SSA_UMSF_PROCESSOR_LUA_H
#define __SSA_UMSF_PROCESSOR_LUA_H
#include "processor.h"
#include "mascript.h"

namespace ssa
{
	class xmProcessorLua : public xmIProcessor, public xmModelAssistScript
	{
	public:
		xmProcessorLua(const xmString& strPath, const xmString& strScriptAbsPath, xmMrtc* pMrtc, const xmString& strDataName);
		virtual ~xmProcessorLua();

		virtual xmRet System2Model(const xmValue& systemValue, xmValue& modelValue);
		virtual xmRet Model2System(const xmValue& modelValue,  xmValue& systemValue);



	protected:
		//	预输入，按照数据输入规则，由仿真系统获取数据，结果并不更新到模型缓存中，而是输出给调用者
		//	预输入的作用，是为了在触发器中，尽快将仿真系统中的当前数值获取到，以判断是否进行触发
		//	预输入并不会修改模型缓存中对应数据的数值
		//	注：触发器中，没有将数据预输出的仿真系统中的需求
		static int ssa_PreInput(lua_State *L);

		virtual bool LockData() const
		{
			return false;
		}
	private:
		static xmProcessorLua* get_xmProcessorLua(lua_State *L)
		{
			return (xmProcessorLua*)read_pointer(L, "xmProcessorLua");
		}

		xmRet _TransformValue(const char* luaFunc, xmPtr<xmClass>& pOutClass, const xmValue& inValue, xmValue& outValue);
		xmPtr<xmClass> m_pModelValueClass;
		xmPtr<xmClass> m_pSystemValueClass;
	};
}
#endif  //  __SSA_UMSF_PROCESSOR_LUA_H

