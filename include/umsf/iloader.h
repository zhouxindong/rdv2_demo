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

	//	����ͨ��ģ������������һ��ģ��ʵ��
	class xmIModelLoader : public xmUncopyable
	{
	public:
		xmIModelLoader(const xmString& strModelFile, xmIModelServer* pModelServer) : m_strModelFile(strModelFile), m_pModelServer(pModelServer) {};
		virtual ~xmIModelLoader() {};

		//	ģ�ͼ��������ܼ���ģ�ͼ����ļ�����չ��
		virtual const char* ModelFlag() const = 0;
		//	����һ��ģ��ʵ����һ��ģ�ͼ�������ֻ�ܴ���һ��ģ��ʵ��
		virtual xmRet CreateModelInstance() = 0;
		//	��ȡģ�ͼ�����������ģ��ʵ���ӿ�
		virtual xmIModel* Model() = 0;
		//	��ȡ������Ϣ
		virtual xmString GetLastError() const { return xmString(); };

	protected:
		const xmString m_strModelFile;
		xmIModelServer* const m_pModelServer;
	};
	//	���ڴ�������ģ�ͼ�����
	class xmIModelDriver : public xmUncopyable
	{
	public:
		xmIModelDriver() {};
		virtual ~xmIModelDriver(void) {};

		//	��ʼ��������ģ������
		virtual xmRet InitializeDriver(const xmString& strConfig) { return xmE_SUCCESS; };
		virtual void DestroyDriver() {};
		//	ģ������������������ģ�ͼ����ļ���չ��
		virtual const char* ModelFlag() const = 0;
		//	����������ģ�ͼ�����
		virtual xmIModelLoader* CreateModelLoader(const xmString& strModelFile, xmIModelServer* pModelServer) = 0;
		virtual void DestroyModelLoader(xmIModelLoader* pModelLoader) = 0;
		//	��ȡ������Ϣ
		virtual xmString GetLastError() const { return xmString(); };
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_ILOADER_H

