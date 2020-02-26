#ifndef __SSA_BASE_WAITSET_H
#define __SSA_BASE_WAITSET_H
#include <algorithm>
#include "exceptions.h"
#include "assertmsg.h"
#include "waitable.h"
#ifdef xmOS_LINUX
#include <sys/epoll.h>
#endif

#pragma warning(disable: 4251)

namespace ssa
{
	class xmBASE_EXPORT xmWaitSet
	{
	private:
		unsigned int m_uNumWaitables;	//	当前Waitable数量
#if defined(xmOS_WINDOWS)
		std::vector<xmWaitable*> m_Waitables;
		std::vector<HANDLE>      m_Handles;		//used to pass array of HANDLEs to WaitForMultipleObjects()
#elif defined(xmOS_LINUX) 
		int m_nEpollSet;
		std::vector<epoll_event> m_Revents;		//used for getting the result from epoll_wait()
#endif

	public:
		xmWaitSet(unsigned int maxSize = MAXIMUM_WAIT_OBJECTS);
		xmWaitSet(const xmWaitSet& ws);
		~xmWaitSet();

		inline unsigned int MaxSize()const
		{
			return (unsigned int)m_Waitables.size();
		}
		inline unsigned int Size()const
		{
			return m_uNumWaitables;
		}

		int Add(xmWaitable& w);
		void Clear();


		/*==============================================================================
		函数名：Wait()
		说  明：等待Waitset中的一个或多个信号，返回有信号的Waitable个数，如果vOut为非空，则
				返回有信号的waitable列表
		==============================================================================*/
		int Wait(std::vector<xmWaitable*>* vOut = NULL, unsigned int uTimeoutMillis = INFINITE);
		/*==============================================================================
		函数名：WaitOne()
		说  明：等待一个Waitset中的信号，返回Waitable的位置
		==============================================================================*/
		inline int WaitOne(unsigned int uTimeoutMillis = INFINITE)
		{
			return __Wait(uTimeoutMillis, false);
		}
		/*==============================================================================
		函数名：WaitAll()
		说  明：等待Waitset中的全部信号，返回实际等待到的信号个数
		==============================================================================*/
		inline int WaitAll(unsigned int uTimeoutMillis = INFINITE)
		{
			return __Wait(uTimeoutMillis, true);
		}

		inline xmWaitable& operator[](unsigned int uPos) const;
		inline xmWaitSet& operator=(const xmWaitSet& ws);

	private:
		struct _ClearFlags
		{
		public:
			inline void operator()(xmWaitable* w) const
			{
				w->ClearRecvFlags();
			}
		};
		struct _SetRWFlags
		{
		public:
			inline void operator()(xmWaitable* w) const
			{
				w->SetRecvFlags(xmWaitable::READ | xmWaitable::WRITE);
				w->OnGetObject();
			}
		};
		int __Wait(unsigned int uTimeoutMillis, bool bIsWaitForAllObjects);
	};
}

#endif
