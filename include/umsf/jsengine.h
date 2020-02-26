#ifndef __SSA_UMSF_JSENGINE_H
#define __SSA_UMSF_JSENGINE_H
#include "sengine.h"

namespace ssa
{
	class xmJsEngine : public xmScriptEngine
	{
	public:
		xmJsEngine(const char* strScript) : xmScriptEngine(strScript) {};
		virtual ~xmJsEngine() {};
		virtual EEngineType Type() const
		{
			return etJS;
		}
	};
}

#endif
