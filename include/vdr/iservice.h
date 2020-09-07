#pragma once
#ifndef __SSA_VDR_ISERVICE_H
#define __SSA_VDR_ISERVICE_H

//#include "icommproxy.h"

namespace ssa
{
	class xmICommProxy;
	class xmISyncProxy;
	class xmVDR_EXPORT xmIService
	{
	public:
		xmIService() {};
		virtual ~xmIService() {};
		virtual void               SetCommProxy(xmICommProxy* pCommProxy) = 0;
		virtual xmICommProxy*      GetCommProxy() { return 0; };
		virtual void               SetSyncProxy(xmISyncProxy* pSyncProxy) {};
		virtual xmISyncProxy*      GetSyncProxy() { return 0; };

		virtual xmVDRAttr*         GetVDRAttr() = 0;
		//测试是否与服务器通信正常
		virtual bool               IsCommOK() = 0;
		//判断是否初始化完毕
		virtual bool               IsValid() = 0;
		//创建会话
		virtual void               CreateSessionGroup(std::string strName) = 0;
		virtual void               CreateGlobalSingleSessions() = 0;
		//停止服务
		virtual void               StopSessions() = 0;
		//关闭服务
		virtual void               CloseSessions(bool isDel = false) = 0;
		//关闭所有资源
		virtual void               Close() = 0;

		//将数据包分发处理
		virtual int                Dispatch(const char* pBuf, int nLen) = 0;
		//只在刚开始建立会话时使用。
		virtual int                DispatchWithWaitTime(const xmPacket& pkt, xmByteBuffer& wParam,bool usePost, int waitTime = 0) = 0;
		virtual int                DispatchWithoutWaitTime(const xmPacket& pkt, bool usePost) = 0;

		virtual void               BeginSync() {};
		virtual void               DoSync() {};
		virtual void               EndSync() {};

		virtual long long          GetTimeStamp() = 0;
		virtual long long          GetFrameCount() = 0;
		virtual unsigned int       GetSNCode() = 0;
	};
}

#endif