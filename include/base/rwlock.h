/*******************************************************************************
Copyright (C), 2011-2022, V5. All rights reserved.
模 块 名：base
文 件 名：rwlock.h
相关文件：无
文件版本: 5.0
功能概述：实现可在线程间和进程间使用的读写锁
多 线 程：支持
异常安全：不支持
详细说明：无
备注信息：无
--------------------------------------------------------------------------------
修改记录：
    日期                    版本            修改人            联系方式
    2014.03.24              V5.0            邓华              dehxya@163.com
        --> 代码初次完成。
*******************************************************************************/
#ifndef __SSA_BASE_RWLOCK_H
#define __SSA_BASE_RWLOCK_H
#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "stringutil.h"
#include "waitset.h"
#include "async.h"
#include "singleton.h"
#ifdef xmOS_LINUX
#include <pthread.h>
#endif

namespace ssa
{	
	class xmBASE_EXPORT xmRWLock : private xmUncopyable
	{
	public:
		xmRWLock(int nName = 0);
		~xmRWLock(void);

		bool ReadLock(unsigned int uTimeoutMillis = INFINITE);
		bool TryReadLock()
		{
			return ReadLock(0);
		}
		bool WriteLock(unsigned int uTimeoutMillis = INFINITE);
		bool TryWriteLock()
		{
			return WriteLock(0);
		}
		void Unlock();


		class ReadGuard
		{
		public:
			ReadGuard(xmRWLock& rwl, unsigned int nTimeoutMillis = INFINITE) : m_pRWLock(&rwl)
			{
				m_pRWLock->ReadLock(nTimeoutMillis);
			}
			~ReadGuard(void)
			{
				m_pRWLock->Unlock();
			}
		private:
			xmRWLock* m_pRWLock;
			ReadGuard(const ReadGuard&) {};
			ReadGuard& operator=(const ReadGuard&)
			{
				return *this;
			}
		};
		class WriteGuard
		{
		public:
			WriteGuard(xmRWLock& rwl, unsigned int nTimeoutMillis = INFINITE) : m_pRWLock(&rwl)
			{
				m_pRWLock->WriteLock(nTimeoutMillis);
			}
			~WriteGuard(void)
			{
				m_pRWLock->Unlock();
			}
		private:
			xmRWLock* m_pRWLock;
			WriteGuard(const WriteGuard&) {};
			WriteGuard& operator=(const WriteGuard&)
			{
				return *this;
			}
		};

	private:
#if defined(xmOS_WINDOWS)
		xmMutex  m_Mutex;
		xmEvent  m_ReadEvent;
		xmEvent  m_WriteEvent;

		unsigned int m_uReaders;		//	获得读权限的对象
		unsigned int m_uWritersWaiting;	//	正在等待的对象
		unsigned int m_uWriters;		//	获得写权限的对象

		xmWaitSet m_WaitRead;
		xmWaitSet m_WaitWrite;
#elif defined(xmOS_LINUX)
		pthread_rwlock_t m_rwl;
#endif	
	};
}
#endif	//	__SSA_BASE_RWLOCK_H
