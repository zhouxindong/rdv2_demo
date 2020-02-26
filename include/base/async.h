
#ifndef __SSA_BASE_ASYNC_H
#define __SSA_BASE_ASYNC_H

#include <cstring>
#include <sstream>
#ifdef xmOS_LINUX
#include <sys/eventfd.h>
#endif
#include "waitable.h"

namespace ssa
{	
	//	互斥量
	class xmBASE_EXPORT xmMutex : public xmWaitable
	{
	public:
		xmMutex(int nName = 0);
		~xmMutex(void);

		inline bool Lock(unsigned int nTimeoutMillis = INFINITE);
		inline bool Unlock(void);

#if defined(xmOS_WINDOWS)
		inline HANDLE GetHandle() const
		{
			return m_Handle;
		}
#elif defined(xmOS_LINUX)
		inline int GetHandle() const
		{
			return m_Handle;
		}
#endif	

		class Guard		
		{
		public:
			Guard(xmMutex& m, unsigned int nTimeoutMillis = INFINITE) : m_pMutex(&m)
			{
				m_pMutex->Lock(nTimeoutMillis);
			}
			~Guard(void)
			{
				m_pMutex->Unlock();
			}
		private:
			xmMutex* m_pMutex;
			Guard(const Guard& ){};
			Guard& operator=(const Guard&)
			{
				return *this;
			}
		};

	private:
		xmMutex(const xmMutex&){};

#if defined(xmOS_WINDOWS)
		HANDLE m_Handle;
#elif defined(xmOS_LINUX)
		pthread_mutex_t m_Handle;
#endif	
	};
		
	//	信号量
	//	清除所有信号
	//	当前信号个数
	class xmBASE_EXPORT xmSemaphore : public xmWaitable
	{
	public:
		xmSemaphore(unsigned int uInitValue = 0, int nName = 0);
		~xmSemaphore(void);


		inline bool Wait(unsigned int nTimeoutMillis = INFINITE);

		inline bool Signal(unsigned int uSignalCnt = 1);

#if defined(xmOS_WINDOWS)
		inline HANDLE GetHandle() const
		{
			return m_Handle;
		}
#elif defined(xmOS_LINUX)
		inline int GetHandle() const
		{
			return m_Handle;
		}
#endif	

	private:
		xmSemaphore(const xmSemaphore& ){};
#if defined(xmOS_WINDOWS)
		HANDLE m_Handle;
#elif defined(xmOS_LINUX)
		sem_t m_Handle;
#endif
	};



	//	事件
	class xmBASE_EXPORT xmEvent : public xmWaitable
	{
	public:
		xmEvent(bool bIsSet = false, bool bIsAutoReset = true, int nName = 0);
		~xmEvent(void);

		inline void Set(void);	//	有信号
		inline bool Wait(unsigned int timeoutMillis = INFINITE);
		inline void Reset(void);//	无信号

#if defined(xmOS_WINDOWS)
		inline HANDLE GetHandle() const
		{
			return m_Handle;
		}
#elif defined(xmOS_LINUX)
		inline int GetHandle() const
		{
			return m_Handle;
		}
#endif

	private:
		xmEvent(const xmEvent&){};
#if defined(xmOS_WINDOWS)
		HANDLE m_Handle;
#elif defined(xmOS_LINUX)
		int m_Handle;
#endif
	};
}

#endif
