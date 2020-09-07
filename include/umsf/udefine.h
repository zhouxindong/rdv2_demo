#ifndef __SSA_UMSF_DEFINE_H
#define __SSA_UMSF_DEFINE_H
#include "base/base.h"
#include "idefine.h"

namespace ssa
{
	__SSA_ERR_DEFINE(xmE_UMSF_CONFIG,	__EC_UMSF(1), "Illegal umsf.config file.");
	__SSA_ERR_DEFINE(xmE_MODEL_CONFIG,	__EC_UMSF(2), "Illegal model.config file.");
	__SSA_ERR_DEFINE(xmE_MODEL_XML,		__EC_UMSF(3), "Illegal [model].xml file.");
	__SSA_ERR_DEFINE(xmE_MODEL_DLL,     __EC_UMSF(4), "Illegal [model].dll file.");
	__SSA_ERR_DEFINE(xmE_CONNECT_SERVER, __EC_UMSF(5), "Connection to VDR server failed.");

	//__SSA_ERR_DEFINE(xmE_UNKNOWN_DATA, __EC_UMSF(20), "Unknown data.");

	__SSA_ERR_DEFINE(xmE_GROUP_IO,			__EC_UMSF(20), "Group IO failed.");
	__SSA_ERR_DEFINE(xmE_DATA_IO,			__EC_UMSF(21), "Data IO failed.");
	__SSA_ERR_DEFINE(xmE_DATA_TYPE_MISMATCH,__EC_UMSF(22), "Data type mismatch.");
	__SSA_ERR_DEFINE(xmE_DATA_NOT_FOUND,	__EC_UMSF(23), "Data not found.");
	__SSA_ERR_DEFINE(xmE_GROUP_NOT_FOUND,   __EC_UMSF(24), "Group not found.");
	__SSA_ERR_DEFINE(xmE_SCRIPT_RETURN_NIL, __EC_UMSF(25), "Script returns nil.");
	__SSA_ERR_DEFINE(xmE_LUA_LOAD,			__EC_UMSF(30), "Load lua script failed.");
	__SSA_ERR_DEFINE(xmE_LUA_EXECUTE,		__EC_UMSF(31), "Execute lua script failed.");
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

	xmDECLARE_EXCEPTION(xmExUmsfConfig,    "xmExUmsfConfig",  xmE_UMSF_CONFIG);
	xmDECLARE_EXCEPTION(xmExModelConfig,   "xmExModelConfig", xmE_MODEL_CONFIG);
	xmDECLARE_EXCEPTION(xmExModelXml,      "xmExModelXml", xmE_MODEL_XML);
	xmDECLARE_EXCEPTION(xmExModelDll,      "xmExModelDll", xmE_MODEL_DLL);
	xmDECLARE_EXCEPTION(xmExConnectServer, "xmExConnectServer", xmE_CONNECT_SERVER);

	xmDECLARE_EXCEPTION_LOG(xmExModelConfigLog, "xmExModelConfig", xmE_MODEL_CONFIG);
	xmDECLARE_EXCEPTION_LOG(xmExModelXmlLog,    "xmExModelXml", xmE_MODEL_XML);
	xmDECLARE_EXCEPTION_LOG(xmExModelDllLog,    "xmExModelDll", xmE_MODEL_DLL);

	xmDECLARE_EXCEPTION(xmExLuaLoad,    "xmExLuaLoad", xmE_LUA_LOAD);
	xmDECLARE_EXCEPTION(xmExLuaExecute, "xmExLuaExecute", xmE_LUA_EXECUTE);

	xmDECLARE_EXCEPTION(xmExGroupIO, "xmExGroupIO", xmE_GROUP_IO);
	xmDECLARE_EXCEPTION(xmExDataIO, "xmExDataIO", xmE_DATA_IO);
	xmDECLARE_EXCEPTION(xmExProcessorTransform, "xmExProcessorTransform", xmE_PROCESSOR_TRANSFORM);

	xmDECLARE_EXCEPTION(xmExDataNotFound,  "xmExDataNotFound",  xmE_DATA_NOT_FOUND);
	xmDECLARE_EXCEPTION(xmExGroupNotFound, "xmExGroupNotFound", xmE_GROUP_NOT_FOUND);


#define UMSF_FLAG	"UMSF"
#define USING_FILE_SYMBOL '@'

	//	获取当前路径信息
	void xmUpdateCurrentPath();
	const xmString& xmGetCurrentPath();

	//	获取umsf.config文件绝对路径和model.config文件绝对路径
	void xmSetUmsfConfigPath(const xmString& strUmsfConfig);
	void xmSetModelConfigPath(const xmString& strModelConfig);
	const xmString& xmGetUmsfConfigPath();
	const xmString& xmGetModelConfigPath();

	//	文件路径服务，输入的原始文件路径将进行标准化转换：
	//	（1）如果是绝对路径，则直接返回该路径；
	//	（2）如果是相对路径，则是相对运行平台exe文件工作目录的相对路径；
	//	（3）如果是"$"符号 + 相对路径，则是相对strBasePath的相对路径，此时输入的strBasePath必须是绝对路径（包含文件名）；
	//	（4）所有相对路径，都会转换为绝对路径返回。
	//	（5）输入路径的首字符，允许使用“@”符号，处理过程中将被删除
	//	（6）文件中可以使用通配符“*”，此符号出现在文件名最后时，debug版程序中将被替换为字符d，release版程序中将被删除，
	//	     出现在其他位置时，将被替换为debug或release。
	xmString xmGetStandardPath(const xmString& strSourcePath, const xmString& strBasePath);

	//	在上面函数输出的标准路径中，提取文件目录，结尾不包含“\”
	xmString xmGetStandardDir(const xmString& strStandardPath);


	//	读取并返回一个以绝对路径描述的文本文件内容
	xmString xmLoadFileString(const char* strFile);

	void xmSetGlobalTime(long long lWallTime, long long lClickCount);
	long long xmWallTime();
	long long xmClickCount();

	//	设置和获取系统节拍周期，单位：毫秒
	void xmSetSystemClickCycle(unsigned int uCycle);
	unsigned int xmGetSystemClickCycle();

	//	设置和获取运行模式
	xmEUmsfMode xmGetUmsfMode();
	void xmSetUmsfMode(xmEUmsfMode eUmsfMode);
}

#endif //__SSA_UMSF_DEFINE_H