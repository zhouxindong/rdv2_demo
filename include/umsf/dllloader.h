#ifndef __SSA_UMSF_DLLLOADER_H
#define __SSA_UMSF_DLLLOADER_H
#include "mloader.h"

namespace ssa
{
	class xmDllLoader : public xmIModelLoader
	{
	public:
		xmDllLoader(const xmString& strModelFile, xmIModelServer* pModelServer);
		virtual ~xmDllLoader();
	protected:
		virtual xmRet CreateModelInstance();
		virtual const char* ModelFlag() const
		{
			return "dll";
		}
		virtual xmIModel* Model()
		{
			return m_pModel;
		}

	private:
		xmLib m_ModelDll;
		xmIModel* m_pModel;

		typedef xmIModel* (*tCreateModel)(xmIModelServer* pServer);
		typedef void(*tDestroyModel)(xmIModel* pModel);
		tCreateModel m_pfnCreateModel;
		tDestroyModel m_pfnDestroyModel;
	};
}
#endif  //  __SSA_UMSF_DLLLOADER_H

