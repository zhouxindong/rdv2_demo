/*******************************************************************************
Copyright (C), 2011-2022, V5. All rights reserved.
模 块 名：base
文 件 名：condvar.h
相关文件：无
文件版本: 5.0
功能概述：实现了条件变量。
多 线 程：支持
异常安全：不支持
详细说明：条件变量在两个线程间实现基于条件的同步，如：
			xmCondVar cond;
			xmMutex   mutex;
			int n = 0;
			int ThreadFunc1(void* pParam)
			{
				mutex.Lock();
				n = 10;			//	改变条件
				mutex.Unlock();	//	先解锁
				cond.Notify();	//	后通知同步线程，条件改变
			}
			int ThreadFunc2(void* pParam)
			{
				mutex.Lock();
				while(n == 0)	//	条件，为了避免出现虚假解锁，因此解锁后应重新检查条件
				{
					cond.Wait();//	等待条件改变
				};
				mutex.Unlock();
			}
			ThreadFunc1函数也可以这样写：
			int ThreadFunc1(void* pParam)
			{
				mutex.Lock();
				n = 10;			//	改变条件
				cond.Notify();	//	先通知同步线程，条件改变
				mutex.Unlock();	//	后解锁
			}
			这种方法由于在ThreadFunc2获得消息后还没有解锁mutex，因此当cond.Wait()重新
			给mutex加锁时会导致其挂起。

备注信息：无。
--------------------------------------------------------------------------------
修改记录：
    日期                    版本            修改人            联系方式
	2015.03.22				V5.1			邓华	              dehxya@163.com
		--> 代码初次完成。
*******************************************************************************/
#ifndef __SSA_BASE_CONDVAR_H
#define __SSA_BASE_CONDVAR_H
#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "async.h"
#include "singleton.h"
#ifdef xmOS_LINUX
#include <sys/eventfd.h>
#endif

namespace ssa
{
	class xmBASE_EXPORT xmCondVar : private xmUncopyable
	{
	private:
#if defined(xmOS_WINDOWS) 
		xmMutex m_cvMutex;
		xmSemaphore m_semWait;
		xmSemaphore m_semDone;
		unsigned m_numWaiters;
		unsigned m_numSignals;
#elif defined(xmOS_LINUX)
		pthread_cond_t cond;
#endif

	public:
		xmCondVar();
		~xmCondVar();

		void Wait(xmMutex& mutex);
		void Notify();
	};
}

#endif	//	__SSA_BASE_CONDVAR_H
