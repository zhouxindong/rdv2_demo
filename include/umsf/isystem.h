#ifndef __SSA_UMSF_ISYSTEM_H
#define __SSA_UMSF_ISYSTEM_H
#include "imessage.h"
/******************************************************************************/
/** @class xmISystem
@brief
	仿真系统的访问接口
@details
	定义了UMSF与仿真系统交互的全部功能，不同仿真系统的具体实现，可继承该接口。

@par 多线程安全
	否
*******************************************************************************/
namespace ssa
{
	/*******************************************************************************
	*  下面的宏用于帮助用户由DLL中导出仿真系统接口
	*******************************************************************************/
#ifdef xmOS_WINDOWS
#define __SYSTEM_EXPORT_API __declspec(dllexport)
#else
#define __SYSTEM_EXPORT_API
#endif

#ifdef  __cplusplus
#define xmEXPORT_SYSTEM(Class)													\
    extern "C"																	\
    {																			\
    __SYSTEM_EXPORT_API ssa::xmISystem* CreateSystem(const char* strConfig);	\
    __SYSTEM_EXPORT_API void DestroySystem(ssa::xmISystem* pSystem);			\
    }																			\
    __SYSTEM_EXPORT_API ssa::xmISystem* CreateSystem(const char* strConfig)		\
    {																			\
		return new Class(strConfig);											\
    }																			\
    __SYSTEM_EXPORT_API void DestroySystem(ssa::xmISystem* pSystem)				\
    {																			\
        if(pSystem != NULL)														\
        {																		\
            delete pSystem;														\
        }																		\
    }
#else   // C Language
#define xmEXPORT_SYSTEM(Class)													\
    __SYSTEM_EXPORT_API ssa::xmISystem* CreateSystem(const char* strConfig);	\
    __SYSTEM_EXPORT_API void DestroySystem(ssa::xmISystem* pSystem);			\
    __SYSTEM_EXPORT_API ssa::xmISystem* CreateSystem(const char* strConfig)		\
    {																			\
		return new Class(strConfig);											\
    }																			\
    __SYSTEM_EXPORT_API void DestroySystem(ssa::xmISystem* pSystem)				\
    {																			\
        if(pSystem != NULL)														\
        {																		\
            delete pSystem;														\
        }																		\
    }
#endif  //  __cplusplus



#define xmISYSTEM_VERSION "V5.0.01_20200814"
	//#define xmISYSTEM_VERSION "V5.0.00_20190412"
	class xmISystem
	{
	public:
		virtual ~xmISystem() {};

	public:
		virtual xmString Version() = 0;
		virtual xmRet Login(const xmString& strNodeID, const xmString& strAttribute = NULL) = 0;
		virtual xmRet Logout() = 0;
		virtual bool IsLogined(void) { return false; };

		virtual xmRet RegisterMsgHandler(xmMsgHandler* pHandler) { return xmE_SUCCESS; };
		virtual xmRet RegisterData(const xmString& strDataName, const xmValue& aValue, const xmString& strAttribute = NULL) { return xmE_SUCCESS; };
		virtual xmRet RegisterSensDate(const xmString& strDataPath) { return xmE_SUCCESS; };

		virtual void  ReadyToGo() { return; };

		virtual void  SetClearFlag(bool bClear) { };
		virtual bool  GetClearFlag() const { return true; };

		//	dataPath：可以传入数据名称，也可以传入属性路径
		//	dataValue：GetValue中，参数可以传入未初始化的xmValue
		virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO) = 0;
		virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO) = 0;

		//	获取仿真状态，仿真状态由ESimuState组合定义
		virtual unsigned int GetSimulateState(void) const { return SSS_FINISH; };
		//	获取当前仿真倍速
		virtual int GetSimulateSpeed(void) const { return 1; };
		//	获取每个网络节拍的时间，由仿真系统设计决定，由VDR触发，在UMSF的配置文件中描述，单位：毫秒
		virtual unsigned int GetClickCycle(void) const { return 10; };
		//	获取当前网络实时节拍数，单位：个数
		virtual size_t GetClickCount(void) const { return 0; };
		//	获取当前系统时间，单位：取决与系统
		virtual time_t GetSystemTime(void) const { return 0; };

		//	强制同步本地与网络数据，strDataName如果为空，则同步全部数据
		//	可指定同步方向，INPUT为网络数据同步到本地，OUTPUT为本地数据同步到网络，默认进行双向同步，
		virtual xmRet SyncDate(const xmString& strDataName = xmString(), int nDirction = (IOT_INPUT | IOT_OUTPUT)) { return xmE_SUCCESS; };

		//	打印仿真系统中所有注册的数据，或打印指定名称的数据
		virtual xmString PrintData(const xmString& strDataName = NULL) { return xmString(); };

	public:
		//	isystem.h文件版本
		//	用户不需要修改，仅用于UMSF对接口版本的比对
		virtual const char* InterfaceVersion(void) const { return xmISYSTEM_VERSION; };

	protected:
		xmISystem() {};
	};
}
#endif  //  __SSA_UMSF_ISYSTEM_H

