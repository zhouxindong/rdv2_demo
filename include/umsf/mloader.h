#ifndef __SSA_UMSF_MLOADER_H
#define __SSA_UMSF_MLOADER_H
#include "iloader.h"
#include "udefine.h"

namespace ssa
{
	class xmModelLoader
    {
	public:
		static xmIModelLoader* Create(const xmString& strModelFile, xmIModelServer* pModelServer);
		static void Destroy(xmIModelLoader* pLoader);
		static bool RegisterDriver(const xmString& strPath, const xmString& strConfig);
		static void ClearDriver();

	private:
		//	通过Model Loader Driver，相当于执行
		//	pLoader = new xmDllLoader(strModelFile, pModelServer);
		//	pLoader = new xmLuaLoader(strModelFile, pModelServer);
		typedef struct __tagExtModelLoaderDriver
		{
			__tagExtModelLoaderDriver() : pModelDriver(NULL) {}
			xmLib m_DriverLib;
			xmIModelDriver* pModelDriver;
		}SExtModelLoaderDriver;

		xmModelLoader() {};
		~xmModelLoader() {};
		static xmOrderMap<xmString, xmPtr<SExtModelLoaderDriver>> m_mapModelLoaderDriver;
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_MLOADER_H

