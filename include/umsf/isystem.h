#ifndef __SSA_UMSF_ISYSTEM_H
#define __SSA_UMSF_ISYSTEM_H
#include "imessage.h"
/******************************************************************************/
/** @class xmISystem
@brief
	����ϵͳ�ķ��ʽӿ�
@details
	������UMSF�����ϵͳ������ȫ�����ܣ���ͬ����ϵͳ�ľ���ʵ�֣��ɼ̳иýӿڡ�

@par ���̰߳�ȫ
	��
*******************************************************************************/
namespace ssa
{
	/*******************************************************************************
	*  ����ĺ����ڰ����û���DLL�е�������ϵͳ�ӿ�
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

		//	dataPath�����Դ����������ƣ�Ҳ���Դ�������·��
		//	dataValue��GetValue�У��������Դ���δ��ʼ����xmValue
		virtual xmRet GetValue(const xmString& dataPath, xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO) = 0;
		virtual xmRet SetValue(const xmString& dataPath, const xmValue& dataValue, xmEDataCheckType eCheckType = DCT_AUTO) = 0;

		//	��ȡ����״̬������״̬��ESimuState��϶���
		virtual unsigned int GetSimulateState(void) const { return SSS_FINISH; };
		//	��ȡ��ǰ���汶��
		virtual int GetSimulateSpeed(void) const { return 1; };
		//	��ȡÿ��������ĵ�ʱ�䣬�ɷ���ϵͳ��ƾ�������VDR��������UMSF�������ļ�����������λ������
		virtual unsigned int GetClickCycle(void) const { return 10; };
		//	��ȡ��ǰ����ʵʱ����������λ������
		virtual size_t GetClickCount(void) const { return 0; };
		//	��ȡ��ǰϵͳʱ�䣬��λ��ȡ����ϵͳ
		virtual time_t GetSystemTime(void) const { return 0; };

		//	ǿ��ͬ���������������ݣ�strDataName���Ϊ�գ���ͬ��ȫ������
		//	��ָ��ͬ������INPUTΪ��������ͬ�������أ�OUTPUTΪ��������ͬ�������磬Ĭ�Ͻ���˫��ͬ����
		virtual xmRet SyncDate(const xmString& strDataName = xmString(), int nDirction = (IOT_INPUT | IOT_OUTPUT)) { return xmE_SUCCESS; };

		//	��ӡ����ϵͳ������ע������ݣ����ӡָ�����Ƶ�����
		virtual xmString PrintData(const xmString& strDataName = NULL) { return xmString(); };

	public:
		//	isystem.h�ļ��汾
		//	�û�����Ҫ�޸ģ�������UMSF�Խӿڰ汾�ıȶ�
		virtual const char* InterfaceVersion(void) const { return xmISYSTEM_VERSION; };

	protected:
		xmISystem() {};
	};
}
#endif  //  __SSA_UMSF_ISYSTEM_H

