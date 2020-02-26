#ifndef __SSA_UMSF_LUALOADER_H
#define __SSA_UMSF_LUALOADER_H
#include "mloader.h"

namespace ssa
{
	class xmLuaModel;
	class xmLuaLoader : public xmIModelLoader
	{
	public:
		xmLuaLoader(const xmString& strModelFile, xmIModelServer* pModelServer) : xmIModelLoader(strModelFile, pModelServer) {};
		virtual ~xmLuaLoader()	{};

		virtual xmRet CreateModelInstance();
		virtual const char* ModelFlag()  const
		{
			return "lua";
		}
		virtual xmIModel* Model();

	private:
		xmPtr<xmLuaModel> m_pLuaModel;
	};
}
#endif  //  __SSA_UMSF_LUALOADER_H

