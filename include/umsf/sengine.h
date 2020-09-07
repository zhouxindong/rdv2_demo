#ifndef __SSA_UMSF_SENGINE_H
#define __SSA_UMSF_SENGINE_H
#include "udefine.h"

namespace ssa
{
	class xmScriptEngine
	{
	public:
		typedef enum
		{
			etLUA,
			etJS
		} EEngineType;
		virtual ~xmScriptEngine() {};
		//	脚本的类型
		virtual EEngineType Type() const = 0;
	protected:
		//	strScript为脚本文件路径或脚本内容，如何描述脚本文件路径，由不同脚本实现决定
		//	strScriptAbsPath为脚本文件绝对路径，结尾不包含“\”
		xmScriptEngine(const char* strScript, const char* strScriptAbsPath) : m_strStript(strScript), m_strAbsPath(strScriptAbsPath) {};
		const xmString m_strStript;
		const xmString m_strAbsPath;
	};
}

#endif
