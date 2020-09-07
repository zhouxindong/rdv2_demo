#ifndef __SSA_UMSF_PROCESSOR_DLL_H
#define __SSA_UMSF_PROCESSOR_DLL_H
#include "processor.h"
namespace ssa
{
	class xmProcessorDll : public xmIProcessor
	{
	public:
		//	strPath是一个可以以“@”开头的动态链接库绝对路径
		xmProcessorDll(const xmString& strPath, xmMrtc* pMrtc, const xmString& strDataName);
		virtual ~xmProcessorDll();

		virtual xmRet System2Model(const xmValue& systemValue, xmValue& modelValue);
		virtual xmRet Model2System(const xmValue& modelValue, xmValue& systemValue);

	protected:
		xmLib m_ProcessorDll;

		typedef xmRet (*tSYSTEM2MODEL)(const xmValue& systemValue, xmValue& modelValue);
		typedef xmRet (*tMODEL2SYSTEM)(const xmValue& modelValue, xmValue& systemValue);
		tSYSTEM2MODEL m_pfnSystem2Model;
		tMODEL2SYSTEM m_pfnModel2System;
	};
}
#endif  //  __SSA_UMSF_PROCESSOR_DLL_H

