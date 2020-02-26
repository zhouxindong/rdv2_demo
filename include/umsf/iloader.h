#ifndef __SSA_UMSF_ILOADER_H
#define __SSA_UMSF_ILOADER_H
#include "../base/base.h"
#include "imodel.h"

namespace ssa
{
#ifdef xmOS_WINDOWS
#define __MODEL_DRIVER_EXPORT_API __declspec(dllexport)
#else
#define __MODEL_DRIVER_EXPORT_API
#endif

#ifdef  __cplusplus
#define xmEXPORT_MODEL_DRIVER(Class)											\
    extern "C"																	\
    {																			\
    __MODEL_DRIVER_EXPORT_API ssa::xmIModelDriver* CreateModelDriver();			\
    __MODEL_DRIVER_EXPORT_API void DestroyModelDriver(ssa::xmIModelDriver* pDriver);\
    }																			\
    __MODEL_DRIVER_EXPORT_API ssa::xmIModelDriver* CreateModelDriver()			\
    {																			\
		return new Class();														\
    }																			\
    __MODEL_DRIVER_EXPORT_API void DestroyModelDriver(ssa::xmIModelDriver* pDriver)	\
    {																			\
        if(pDriver != NULL)														\
        {																		\
            delete pDriver;														\
        }																		\
    }
#else   // C Language
#define xmEXPORT_MODEL_DRIVER(Class)											\
    __MODEL_DRIVER_EXPORT_API ssa::xmIModelDriver* CreateModelDriver();			\
    __MODEL_DRIVER_EXPORT_API void DestroyModelDriver(ssa::xmIModelDriver* pDriver);\
    __MODEL_DRIVER_EXPORT_API ssa::xmIModelDriver* CreateModelDriver()			\
    {																			\
		return new Class();														\
    }																			\
    __MODEL_DRIVER_EXPORT_API void DestroyModelDriver(ssa::xmIModelDriver* pDriver)	\
    {																			\
        if(pDriver != NULL)														\
        {																		\
            delete pDriver;														\
        }																		\
    }
#endif  //  __cplusplus

	//	用于通过模型驱动，创建一个模型实例
	class xmIModelLoader : public xmUncopyable
	{
	public:
		xmIModelLoader(const xmString& strModelFile, xmIModelServer* pModelServer) : m_strModelFile(strModelFile), m_pModelServer(pModelServer) {};
		virtual ~xmIModelLoader() {};

		//	模型加载器所能加载模型计算文件的扩展名
		virtual const char* ModelFlag() const = 0;
		//	创建一个模型实例，一个模型加载器，只能创建一个模型实例
		virtual xmRet CreateModelInstance() = 0;
		//	获取模型加载器创建的模型实例接口
		virtual xmIModel* Model() = 0;
		//	获取错误信息
		virtual xmString GetLastError() const { return xmString(); };

	protected:
		const xmString m_strModelFile;
		xmIModelServer* const m_pModelServer;
	};
	//	用于创建若干模型加载器
	class xmIModelDriver : public xmUncopyable
	{
	public:
		xmIModelDriver() {};
		virtual ~xmIModelDriver(void) {};

		//	初始化和销毁模型驱动
		virtual xmRet InitializeDriver(const xmString& strConfig) { return xmE_SUCCESS; };
		virtual void DestroyDriver() {};
		//	模型驱动器所能驱动的模型计算文件扩展名
		virtual const char* ModelFlag() const = 0;
		//	创建和销毁模型加载器
		virtual xmIModelLoader* CreateModelLoader(const xmString& strModelFile, xmIModelServer* pModelServer) = 0;
		virtual void DestroyModelLoader(xmIModelLoader* pModelLoader) = 0;
		//	获取错误信息
		virtual xmString GetLastError() const { return xmString(); };
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_ILOADER_H

