#ifndef __SSA_UMSF_PROCESSOR_LUA_H
#define __SSA_UMSF_PROCESSOR_LUA_H
#include "processor.h"
#include "luaengine.h"

namespace ssa
{
	class xmProcessorLua : public xmIProcessor, public xmLuaEngine
	{
	public:
		xmProcessorLua(const xmString& strPath);
		virtual ~xmProcessorLua();

		virtual xmRet System2Model(const xmValue& systemValue, xmValue& modelValue);
		virtual xmRet Model2System(const xmValue& modelValue,  xmValue& systemValue);
	private:
		xmRet _TransformValue(const char* luaFunc, xmPtr<xmClass>& pOutClass, const xmValue& inValue, xmValue& outValue);
		xmPtr<xmClass> m_pModelValueClass;
		xmPtr<xmClass> m_pSystemValueClass;
	};
}
#endif  //  __SSA_UMSF_PROCESSOR_LUA_H

