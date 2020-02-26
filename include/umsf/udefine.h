#ifndef __SSA_UMSF_DEFINE_H
#define __SSA_UMSF_DEFINE_H
#include "base/base.h"

namespace ssa
{
	__SSA_ERR_DEFINE(xmE_UMSF_CONFIG,	__EC_UMSF(1), "Illegal umsf.config file.");
	__SSA_ERR_DEFINE(xmE_MODEL_CONFIG,	__EC_UMSF(2), "Illegal model.config file.");
	__SSA_ERR_DEFINE(xmE_MODEL_XML,		__EC_UMSF(3), "Illegal [model].xml file.");
	__SSA_ERR_DEFINE(xmE_MODEL_DLL,     __EC_UMSF(4), "Illegal [model].dll file.");

	//__SSA_ERR_DEFINE(xmE_UNKNOWN_DATA, __EC_UMSF(20), "Unknown data.");

	__SSA_ERR_DEFINE(xmE_GROUP_IO,      __EC_UMSF(20), "Group IO failed.");
	__SSA_ERR_DEFINE(xmE_DATA_IO,       __EC_UMSF(21), "Data IO failed.");
	__SSA_ERR_DEFINE(xmE_LUA_LOAD,	    __EC_UMSF(30), "Load lua script failed.");
	__SSA_ERR_DEFINE(xmE_LUA_EXECUTE,   __EC_UMSF(31), "Execute lua script failed.");
	__SSA_ERR_DEFINE(xmE_PROCESSOR_TRANSFORM, __EC_UMSF(40), "Processor transform failed.");

#define xmDECLARE_EXCEPTION_LOG(EXC, strClassName, nCode)			\
	class EXC : public xmException									\
	{																\
	public:															\
		EXC(void) : xmException(nCode){};							\
		EXC(const char* title, const char* msg, ...) : xmException(nCode), m_strTitle(title)	\
		{															\
			va_list args;											\
			va_start(args, msg);									\
			FormatMsg(msg, args);									\
			va_end(args);											\
		};															\
		EXC(const EXC& exc) : xmException(exc), m_strTitle(exc.Title()) {};	\
		virtual ~EXC(void){};										\
		virtual void Throw(void) const { throw *this; };			\
		virtual const char* What(void) const { return strClassName; };	\
		const char* Title(void) const { return m_strTitle.String(); };	\
	private:														\
		const xmString m_strTitle;									\
	}

	xmDECLARE_EXCEPTION(xmExUmsfConfig,  "xmExUmsfConfig",  xmE_UMSF_CONFIG);
	xmDECLARE_EXCEPTION(xmExModelConfig, "xmExModelConfig", xmE_MODEL_CONFIG);
	xmDECLARE_EXCEPTION(xmExModelXml, "xmExModelXml", xmE_MODEL_XML);
	xmDECLARE_EXCEPTION(xmExModelDll, "xmExModelDll", xmE_MODEL_DLL);
	xmDECLARE_EXCEPTION_LOG(xmExModelConfigLog, "xmExModelConfig", xmE_MODEL_CONFIG);
	xmDECLARE_EXCEPTION_LOG(xmExModelXmlLog, "xmExModelXml", xmE_MODEL_XML);
	xmDECLARE_EXCEPTION_LOG(xmExModelDllLog, "xmExModelDll", xmE_MODEL_DLL);

	xmDECLARE_EXCEPTION(xmExLuaLoad,    "xmExLuaLoad", xmE_LUA_LOAD);
	xmDECLARE_EXCEPTION(xmExLuaExecute, "xmExLuaExecute", xmE_LUA_EXECUTE);

	xmDECLARE_EXCEPTION(xmExGroupIO, "xmExGroupIO", xmE_GROUP_IO);
	xmDECLARE_EXCEPTION(xmExDataIO, "xmExDataIO", xmE_DATA_IO);
	xmDECLARE_EXCEPTION(xmExProcessorTransform, "xmExProcessorTransform", xmE_PROCESSOR_TRANSFORM);

	//	读取并返回一个以“@”符号开头的文件或直接表达的文件内容
	xmString xmLoadFileString(const char* strFile);

#define UMSF_FLAG	"UMSF"
}

#endif //__SSA_UMSF_DEFINE_H