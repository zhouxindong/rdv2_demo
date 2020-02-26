#ifndef __SSA_BASE_THREAD_H
#define __SSA_BASE_THREAD_H

#include <cstring>
#include <sstream>

#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "async.h"
#include "singleton.h"

#if defined(xmOS_WINDOWS)
#include <process.h>
#elif defined(xmOS_LINUX)
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <ctime>
#endif


namespace ssa
{
	//	获得线程ID
	//	定义无效线程ID
	//	获得线程工作状态
	class xmBASE_EXPORT xmThread : private xmUncopyable
	{
	public:
		enum E_State
		{
			NEW,
			RUNNING,
			PAUSED,
			JOINED
		};

		//定义线程函数
		typedef int (*xmThreadFunc)(void*);
		
		xmThread(void);
		virtual ~xmThread();

		void SetPriority(int nPrio);
		void Terminate();
		inline int GetState() const;

		//通过函数继承方式实现线程，需要处理的内容放在此内。
		virtual int Run(void* lpVoid){return -1;}

		//通过线程函数启动线程。
		//线程函数：		typedef int (*xmThreadFunc)(void*)
		inline void Start(void* lpVoid ,int nPrio = xmTP_NORMAL, unsigned stackSize = 0)
		{
			StartByFunc(NULL, lpVoid, nPrio, stackSize);
		}
		void StartByFunc(xmThreadFunc lpFunc , void* lpVoid , int nPrio = xmTP_NORMAL, unsigned stackSize = 0);
		//	结束线程并等待线程停止
		void Join();

		static inline void Sleep(unsigned msec = 0);
		static inline unsigned long GetCurrentThreadID();


#if defined(xmOS_WINDOWS)
		inline HANDLE GetTD(void)
#elif defined(xmOS_LINUX)
		inline pthread_t GetTD(void)
#endif
		{
			return m_hThreadDescriptor;
		}

		inline unsigned int GetTID(void)
		{
#if defined(xmOS_WINDOWS)
			return m_uThreadID;
#elif defined(xmOS_LINUX)
			return m_hThreadDescriptor;
#endif
		}

	private:
		E_State m_eState;
		xmMutex m_Mutex;
		static inline xmMutex& Mutex2()
		{
			static xmMutex m;
			return m;
		}
		
		//	定义线程描述符
#if defined(xmOS_WINDOWS)
		HANDLE m_hThreadDescriptor;
#elif defined(xmOS_LINUX)
		pthread_t m_hThreadDescriptor;
#endif
		//	定义线程ID
		unsigned int m_uThreadID;

		//	启动线程传入的参数
		typedef struct tagValue
		{
			xmThread*       lpThread;
			void*           lpVoid;
			xmThreadFunc    lpFunc;
		}thrValue;
		thrValue m_thrVal;

#if defined(xmOS_WINDOWS)
		static unsigned int __stdcall CallableEntry(void* data);
#elif defined(xmOS_LINUX)
		static void* CallableEntry(void *data);
#endif
	};
}

#endif
