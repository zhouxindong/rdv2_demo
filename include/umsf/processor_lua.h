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
		//	Ԥ���룬����������������ɷ���ϵͳ��ȡ���ݣ�����������µ�ģ�ͻ����У����������������
		//	Ԥ��������ã���Ϊ���ڴ������У����콫����ϵͳ�еĵ�ǰ��ֵ��ȡ�������ж��Ƿ���д���
		//	Ԥ���벢�����޸�ģ�ͻ����ж�Ӧ���ݵ���ֵ
		//	ע���������У�û�н�����Ԥ����ķ���ϵͳ�е�����
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

