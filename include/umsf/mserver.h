#ifndef __SSA_UMSF_MSERVER_H
#define __SSA_UMSF_MSERVER_H
#include "imodel.h"

namespace ssa
{
	class xmMrtc;
	class xmModelServer : public xmIModelServer
    {
	public:
		xmModelServer(xmMrtc* pMrtc) : m_pMrtc(pMrtc) {};
		virtual ~xmModelServer() {};

	public:
		/*******************************************************************************
		系统服务
		*******************************************************************************/
		virtual void Log(xmLog::ELogLevel eLevel, const char* strContent);


		/*******************************************************************************
		仿真系统信息
		*******************************************************************************/
		virtual unsigned int GetSimulateState(void) const;
		virtual int GetSimulateSpeed(void) const;
		virtual time_t GetSystemTime(void) const;
		virtual const xmATime& GetWallTime(void) const;
		virtual const xmATime& GetSimuTime(void) const;
		virtual size_t GetClickCount(void) const;
		virtual unsigned int GetClickCycle(void) const;
		virtual unsigned int GetDriveTime(void) const;
		virtual unsigned int GetDriveClick(void) const;


		/*******************************************************************************
		使用MRTC资源
		*******************************************************************************/
		virtual const xmString& ModelName(void) const;
		virtual const xmString& InstanceName(void) const;
		virtual bool IsCallbackLogged(xmEModelInterface eMi) const;

		virtual size_t InnerDataCount(void) const;
		virtual const xmString& InnerDataName(size_t uPos) const;
		virtual const char* InnerDataAttr(size_t uPos, const char* strAttrName) const;
		virtual const char* InnerDataAttr(const xmString& strDataName, const char* strAttrName) const;
		virtual size_t InputDataCount(void) const;
		virtual const xmString& InputDataName(size_t uPos) const;
		virtual const char* InputDataAttr(size_t uPos, const char* strAttrName) const;
		virtual const char* InputDataAttr(const xmString& strDataName, const char* strAttrName) const;
		virtual size_t OutputDataCount(void) const;
		virtual const xmString& OutputDataName(size_t uPos) const;
		virtual const char* OutputDataAttr(size_t uPos, const char* strAttrName) const;
		virtual const char* OutputDataAttr(const xmString& strDataName, const char* strAttrName) const;

		virtual size_t TagName(xmPtr<const char*[]>& vTagName) const;
		virtual size_t TagData(const xmString& strTagName, xmPtr<const char*[]>& vDataName) const;
		virtual size_t DataTag(const xmString& strDataName, xmPtr<const char*[]>& vTagName) const;
		virtual bool IsDataTagged(const xmString& strDataName, const xmString& strTagName) const;
		virtual bool IsDataLogged(const xmString& strDataName);

		virtual xmValue InnerData(const xmString& strDataPath) const;
		virtual xmValue InnerData(size_t uPos) const;
		virtual xmValue InputData(const xmString& strDataPath) const;
		virtual xmValue InputData(size_t uPos) const;
		virtual xmValue OutputData(const xmString& strDataPath) const;
		virtual xmValue OutputData(size_t uPos) const;
		virtual xmValue ModelData(const xmString& strDataPath, xmEIOType& ioType) const;
		virtual xmValue ModelData(const xmString& strDataPath) const;

		virtual xmRWLock& InnerCacheLock(void);
		virtual xmRWLock& InputCacheLock(void);
		virtual xmRWLock& OutputCacheLock(void);

		virtual size_t GroupCount(void) const;
		virtual const xmString& GroupName(size_t uPos) const;
		virtual xmEIOType GroupType(size_t uPos) const;
		virtual xmEIOType GroupType(const xmString& strGroupName) const;
		virtual const char* GroupAttr(size_t uPos, const char* strAttrName) const;
		virtual const char* GroupAttr(const xmString& strGroupName, const char* strAttrName) const;
		virtual size_t GroupData(size_t uPos, xmPtr<const char*[]>& vDataName) const;
		virtual size_t GroupData(const xmString& strGroupName, xmPtr<const char*[]>& vDataName) const;
		virtual bool  GetGroupEnabled(size_t uPos);
		virtual bool  GetGroupEnabled(const xmString& strGroupName);
		virtual xmRet SetGroupEnabled(size_t uPos, bool bIsEnable);
		virtual xmRet SetGroupEnabled(const xmString& strGroupName, bool bIsEnabled);
		virtual bool IsGroupLogged(const xmString& strGroupName);

		virtual xmRet ApplyGroupIO(const xmString& strGroupName, bool bCompulsive = false);
		virtual xmRet ApplyDataIO(const xmString& strDataName);
		virtual int   ManualGroupIO(const xmString& strGroupName, bool bLockData = false, bool bCompulsive = false, bool bSyncNow = false);
		virtual xmRet ManualDataIO(const xmString& strDataName, bool bLockData = false, bool bSyncNow = false);

		virtual const char* InterfaceVersion(void) const { return xmIMODELSERVER_VERSION; }
	private:
		xmMrtc* const m_pMrtc;
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_MSERVER_H

