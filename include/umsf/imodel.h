#ifndef __SSA_UMSF_IMODEL_H
#define __SSA_UMSF_IMODEL_H
#include "../base/base.h"
#include "idefine.h"
namespace ssa
{
	/*******************************************************************************
	*  下面的宏用于帮助用户由DLL中导出模型
	*  一个DLL只能创建一个模型
	*******************************************************************************/
#ifdef xmOS_WINDOWS
#define __MODEL_EXPORT_API __declspec(dllexport)
#else
#define __MODEL_EXPORT_API
#endif

#ifdef  __cplusplus
#define xmEXPORT_MODEL(Class)													\
    extern "C"																	\
    {																			\
    __MODEL_EXPORT_API ssa::xmIModel* CreateModel(ssa::xmIModelServer* pServer);\
    __MODEL_EXPORT_API void DestroyModel(ssa::xmIModel* pModel);				\
    }																			\
    __MODEL_EXPORT_API ssa::xmIModel* CreateModel(ssa::xmIModelServer* pServer)	\
    {																			\
		if (ssa::xmString(pServer->InterfaceVersion()) != xmIMODELSERVER_VERSION)	\
		{																		\
			return NULL;														\
		}																		\
		return new Class(pServer);												\
    }																			\
    __MODEL_EXPORT_API void DestroyModel(ssa::xmIModel* pModel)					\
    {																			\
        if(pModel != NULL)														\
        {																		\
            delete pModel;														\
        }																		\
    }
#else   // C Language
#define xmEXPORT_MODEL(Class)													\
    __MODEL_EXPORT_API ssa::xmIModel* CreateModel(ssa::xmIModelServer* pServer);\
    __MODEL_EXPORT_API void DestroyModel(ssa::xmIModel* pModel);				\
    __MODEL_EXPORT_API ssa::xmIModel* CreateModel(ssa::xmIModelServer* pServer)	\
    {																			\
		if (ssa::xmString(pServer->InterfaceVersion()) != xmIMODELSERVER_VERSION)	\
		{																		\
			return NULL;														\
		}																		\
		return new Class(pServer);												\
    }																			\
    __MODEL_EXPORT_API void DestroyModel(ssa::xmIModel* pModel)					\
    {																			\
        if(pModel != NULL)														\
        {																		\
            delete pModel;														\
        }																		\
    }
#endif  //  __cplusplus


	/******************************************************************************/
	/** @class xmIModelServer
	@brief
		xmIModelServer 定义了UMSF能够为模型提供的服务接口。
	*******************************************************************************/
#define xmIMODELSERVER_VERSION "V5.0.13_20200102"
	//#define xmIMODELSERVER_VERSION "V5.0.12_20191217"
	//#define xmIMODELSERVER_VERSION "V5.0.11_20191023"
	//#define xmIMODELSERVER_VERSION "V5.0.10_20191022"
	//#define xmIMODELSERVER_VERSION "V5.0.00_20190529"
	class xmIModelServer
	{
	public:
		virtual ~xmIModelServer() {};

	public:
		/*******************************************************************************
		系统服务
		*******************************************************************************/
		//	输出日志信息
		virtual void Log(xmLog::ELogLevel eLevel, const char* strContent) = 0;


		/*******************************************************************************
		仿真系统服务
		*******************************************************************************/
		//	获取仿真状态
		virtual unsigned int GetSimulateState(void) const = 0;
		//	获取当前仿真倍速
		virtual int GetSimulateSpeed(void) const = 0;
		//	获取当前由服务器统一的系统时间，单位：取决于系统
		virtual time_t GetSystemTime(void) const = 0;
		//	获取世界墙时
		virtual const xmATime& GetWallTime(void) const = 0;
		//	获取仿真时间
		virtual const xmATime& GetSimuTime(void) const = 0;
		//	获取当前网络节拍数，单位：节拍个数
		virtual size_t GetClickCount(void) const = 0;
		//	获取每个网络节拍的时间，由仿真系统决定，单位：毫秒
		virtual unsigned int GetClickCycle(void) const = 0;
		//	获取模型配置的驱动周期，此时间，不一定是模型实际驱动时间。单位：毫秒
		virtual unsigned int GetDriveTime(void) const = 0;
		//	获取每个驱动周期的实际节拍数，GetClickCycle() × GetDriveClick() = 实际驱动时间，单位：节拍数。
		virtual unsigned int GetDriveClick(void) const = 0;

		/*******************************************************************************
		实例服务
		*******************************************************************************/
		//	获取模型、实例名称
		virtual const xmString& ModelName() const = 0;
		virtual const xmString& InstanceName(void) const = 0;


		/*******************************************************************************
		数据服务
		*******************************************************************************/
		//	数据信息
		virtual size_t InnerDataCount(void) const = 0;
		virtual const xmString& InnerDataName(size_t uPos) const = 0;
		virtual const char* InnerDataAttr(size_t uPos, const char* strAttrName) const = 0;
		virtual const char* InnerDataAttr(const xmString& strDataName, const char* strAttrName) const = 0;

		virtual size_t InputDataCount(void) const = 0;
		virtual const xmString& InputDataName(size_t uPos) const = 0;
		virtual const char* InputDataAttr(size_t uPos, const char* strAttrName) const = 0;
		virtual const char* InputDataAttr(const xmString& strDataName, const char* strAttrName) const = 0;

		virtual size_t OutputDataCount(void) const = 0;
		virtual const xmString& OutputDataName(size_t uPos) const = 0;
		virtual const char* OutputDataAttr(size_t uPos, const char* strAttrName) const = 0;
		virtual const char* OutputDataAttr(const xmString& strDataName, const char* strAttrName) const = 0;

		virtual size_t TagName(xmPtr<const char*[]>& vTagName) const = 0;
		virtual size_t TagData(const xmString& strTagName, xmPtr<const char*[]>& vDataName) const = 0;
		virtual size_t DataTag(const xmString& strDataName, xmPtr<const char*[]>& vTagName) const = 0;
		virtual bool IsDataTagged(const xmString& strDataName, const xmString& strTagName) const = 0;


		//	数据数值
		//	供模型使用的数据访问接口，strDataPath参数，为数据的模型名称，也可以用符号“.”直接访问
		//	结构化数据中的属性，如:"天宫状态.位置信息.X轴"
		//	对于输入数据，如果模型改变其数值，当进行Input更新时，会被系统数据改变
		/*	模型数据，可以通过直接方式使用，也可以通过定义一个新的变量与其关联的方式来使用，其中
			直接方式使用：
					float fMass = Server()->InnerData("飞船质量").f32();
					Server()->InnerData("飞船质量").f32() = fMass * 1.1;
				上例中，改变fMass的值，并不会改变数据“飞船质量”的数值。此种方式直观，易读，但是每次使用均需要
				进行一组Map表查询，效率较低，适合于使用不频繁的数据。
			关联方式使用：
					xmValue  vMass = Server()->InnerData("飞船质量");
					xmValue  vMass(Server()->InnerData("飞船质量"));
					xmValue* vMass = new xmValue(Server()->InnerData("飞船质量"));
					float& fMass = Server()->InnerData("飞船质量").f32();
				上例中，vMass和fMass与数据“飞船质量”建立了关联，改变vMass和fMass数值，将直接导致数据“飞船质量”
				数值的改变，如fMass *= 1.1，等同于Server()->InnerData("飞船质量").f32() = fMass * 1.1。此种
				方式效率较高，对Map表可以一次查询，多次使用，但需要定义更多的变量，适合于频繁使用的数据。注：此种
				方式不会对数据的读写进行限制，模型对模型的输入数据进行写操作和对模型的输出数据进行读操作都是允许的，
				只不过这种读写操作仅影响模型缓存中的数值，不会对系统数据产生影响。
			但是注意，这种关联必须在声明变量的时候进行，已经声明的变量，无法产生这种关联，如：
					xmValue vMass;
					vMass.Initialize(Server()->InnerData("飞船质量"));
				此时，vMass与Server()->InnerData("飞船质量")未建立关联，两者使用相同的数据结构，但存储数据的
				内存空间不同，因此改变vMass数值，不会对数据“飞船质量”产生影响。				
		*/
		virtual xmValue InnerData(size_t uPos) const = 0;
		virtual xmValue InnerData(const xmString& strDataPath) const = 0;
		virtual xmValue InputData(size_t uPos) const = 0;
		virtual xmValue InputData(const xmString& strDataPath) const = 0;
		virtual xmValue OutputData(size_t uPos) const = 0;
		virtual xmValue OutputData(const xmString& strDataPath) const = 0;
		virtual xmValue ModelData(const xmString& strDataPath, xmEIOType& ioType) const = 0;
		virtual xmValue ModelData(const xmString& strDataPath) const = 0;

		//	缓存使用接口保护
		//	严禁在模型回调函数中（不包含OnLoad、OnUnload）使用下列保护接口
		//	仅当模型另开线程进行计算时，才需要使用下列函数对数据进行保护。
		virtual xmRWLock& InnerCacheLock(void) = 0;
		virtual xmRWLock& InputCacheLock(void) = 0;
		virtual xmRWLock& OutputCacheLock(void) = 0;


		/*******************************************************************************
		组服务
		*******************************************************************************/
		//	组信息
		virtual size_t GroupCount(void) const = 0;
		virtual const xmString& GroupName(size_t uPos) const = 0;
		virtual xmEIOType GroupType(size_t uPos) const = 0;
		virtual xmEIOType GroupType(const xmString& strGroupName) const = 0;
		virtual const char* GroupAttr(size_t uPos, const char* strAttrName) const = 0;
		virtual const char* GroupAttr(const xmString& strGroupName, const char* strAttrName) const = 0;
		virtual size_t GroupData(size_t uPos, xmPtr<const char*[]>& vDataName) const = 0;
		virtual size_t GroupData(const xmString& strGroupName, xmPtr<const char*[]>& vDataName) const = 0;
		//	获取和设置组的激活状态
		virtual bool  GetGroupEnabled(size_t uPos) = 0;
		virtual bool  GetGroupEnabled(const xmString& strGroupName) = 0;
		virtual xmRet SetGroupEnabled(size_t uPos, bool bIsEnable) = 0;
		virtual xmRet SetGroupEnabled(const xmString& strGroupName, bool bIsEnable) = 0;


		/*******************************************************************************
		输入/输出控制服务
		*******************************************************************************/
		//	模型申请进行IO，不受任何限制，即无论指定组是否为活跃状态。IO为异步执行
		//	申请IO时，只能指定数据组或者数据，不能指定到数据属性
		virtual xmRet ApplyGroupIO(const xmString& strGroupName) = 0;
		virtual xmRet ApplyDataInput(const xmString& strDataName) = 0;
		virtual xmRet ApplyDataOutput(const xmString& strDataName) = 0;

		//	模型手动进行IO，没有进行数据锁保护，不受任何限制，即无论指定组是否为活跃状态。IO为同步执行
		//	手动IO时，只能指定数据组或者数据，不能指定到数据属性
		virtual xmRet ManualGroupIO(const xmString& strGroupName, bool bLockData = false) = 0;
		virtual xmRet ManualDataInput(const xmString& strDataName, bool bLockData = false) = 0;
		virtual xmRet ManualDataOutput(const xmString& strDataName, bool bLockData = false) = 0;

	public:
		//	xmIModelServer接口版本
		//	用户不需要修改，仅用于UMSF对接口版本的比对
		virtual const char* InterfaceVersion(void) const = 0;

	protected:
		xmIModelServer() {};
	};

#define xmIMODEL_VERSION "V5.0.01_20191023"
//#define xmIMODEL_VERSION "V5.0.00_20190412"
	class xmIModel : public xmUncopyable
	{
	public:
		xmIModel(xmIModelServer* pServer) : m_pServer(pServer) {};
		virtual ~xmIModel(void) {};

	public:
		virtual xmRet OnLoad(const char* strDataFile) { return xmE_SUCCESS; };
		virtual void OnUnload(void) {};
		virtual bool IsReady(void) = 0;

		//	下列回调函数，三个缓存均被UMSF进行了写保护
		//	控制命令响应回调函数
		virtual void OnStart(void) {};
		virtual void OnFinish(void) {};
		virtual void OnFreeze(void) {};
		virtual void OnUnfreeze(void) {};
		virtual void OnSpeed(int nSpeed) {};
		virtual void OnCommand(const xmString& cmdName, const xmValue& cmdParam) {};

		//  驱动接口回调函数
		//	节拍驱动
		virtual void OnDrive(unsigned long uClick) {};
		//	时间驱动
		virtual void OnTime(const xmATime& tWallTime, const xmATime& tSimuTime) {};

		//	模型缓存与仿真系统进行数据交换的回调函数
		virtual void OnPrepareGroupInput(const char* strGroupName) {};
		virtual void OnFinishGroupInput(const char* strGroupName, bool isDone) {};
		virtual void OnPrepareGroupOutput(const char* strGroupName) {};
		virtual void OnFinishGroupOutput(const char* strGroupName, bool isDone) {};
		virtual void OnApplyGroupIO(const char* strGroupName, bool isDone) {};
		virtual void OnApplyDataIO(const char* strDataName, bool isDone) {};

		//  状态信息
		virtual xmString GetVersion(void) { return NULL; };
		virtual int GetState(void) { return 0; };
		virtual int GetLastError(void) { return xmE_SUCCESS; };
		virtual xmString PrintLastError(void) { return NULL; };

	public:
		//	xmIModel接口版本
		//	用户不需要修改，仅用于UMSF对接口版本的比对
		virtual const char* InterfaceVersion(void) const { return xmIMODEL_VERSION; };

	protected:
		xmIModelServer* Server() const
		{
			return m_pServer;
		}
	private:
		xmIModelServer* const m_pServer;
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_IMODEL_H

