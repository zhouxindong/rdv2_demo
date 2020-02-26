#ifndef __SSA_BASE_THREADW_H
#define __SSA_BASE_THREADW_H

#include <cstring>
#include <sstream>
#include <map>

#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "waitable.h"
#include "waitset.h"
#include "async.h"

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
	class xmBASE_EXPORT xmThreadw : public xmWaitable
	{
	public:
		//	定义事件处理函数和输入参数的入口
		typedef int (*WaitableProcesser)(void*, xmWaitable*);
		typedef struct __tagProcesserAttribute
		{
			__tagProcesserAttribute(WaitableProcesser pProcesser, void* pParam = NULL) : m_pProcesser(pProcesser), m_pParam(pParam){}
			WaitableProcesser m_pProcesser;
			void* m_pParam;
		}ProcesserAttribute;
		//	定义线程开始前的预处理函数和输入参数入口
		typedef bool (*PreProcesser)(void*);
		typedef struct __tagPreProcesserAttribute
		{
			__tagPreProcesserAttribute(PreProcesser pProcesser = NULL, void* pParam = NULL) : m_pProcesser(pProcesser), m_pParam(pParam){}
			PreProcesser m_pProcesser;
			void* m_pParam;
		}PreProcesserAttribute;
		//	定义线程结束后的后处理函数和输入参数入口
		typedef int (*PostProcesser)(void*);
		typedef struct __tagPostProcesserAttribute
		{
			__tagPostProcesserAttribute(PostProcesser pProcesser = NULL, void* pParam = NULL) : m_pProcesser(pProcesser), m_pParam(pParam){}
			PostProcesser m_pProcesser;
			void* m_pParam;
		}PostProcesserAttribute;

	public:
		xmThreadw(void):
			xmWaitable(0), 
			m_eReady(false, false), 
			m_hThread(INVALID_THREAD_VALUE), 
			m_pDefParam(NULL), 
			m_nPriority(xmTP_NORMAL), 
			m_dwThreadID(0),
			m_uWLSCyc(0),
			m_fWLNumber(0.0),
			m_fWLPrecent(0.0),
			m_OnWLSDone(NULL),
			m_pWlsParam(NULL),
			m_wAccuracy(0),
			m_TimerID(NULL)
		{
			m_WaitSet.Add(m_eQuit);
			m_WaitSet.Add(m_eWLS);
		};
		virtual ~xmThreadw()
		{
		};

		inline bool IsRunning() const
		{
			return m_hThread != INVALID_THREAD_VALUE;
		}
		inline xmEThreadPriority GetPriority() const
		{
			return m_nPriority;
		}

		bool AddWaitable(xmWaitable& w, ProcesserAttribute& pa);
		int Start(void* pDefParam = NULL, int nPriority = xmTP_NORMAL, bool bSuspended = false, PreProcesserAttribute PreProcAttr = PreProcesserAttribute(), PostProcesserAttribute PostProcAttr = PostProcesserAttribute());
		inline bool Suspend(void);
		inline bool Resume(void);
		void Quit();
		bool SetPriority(xmEThreadPriority nPrio);

		//	统计MRTC在一段时间内的运算负载情况
		typedef void(*OnWLSDoneFunc)(double fWLPrecent, void* pParam);
		xmRet StartWorkLoadStatistics(unsigned int uTime, OnWLSDoneFunc OnWLSDone = NULL, void* pParam = NULL);
		void StopWorkLoadStatistics(void);
		double WorkLoad(void) const
		{
			return m_fWLPrecent;
		}

	private:
		//	工作负载统计周期，单位：秒
		unsigned int m_uWLSCyc;
		//	计算的工作负载，单位：毫秒
		double m_fWLNumber;
		//	工作负载比例：单位：百分比
		double m_fWLPrecent;
		//	进行工作负载统计的信号，初始无信号，自动复位
		xmEvent m_eWLS;
		//	完成工作负载统计的信号，初始无信号，自动复位
		xmEvent m_eWLSDone;
		//	工作负载统计完成后的回调函数
		OnWLSDoneFunc m_OnWLSDone;
		//	调用m_OnWLSDone函数时传入的参数
		void* m_pWlsParam;

		//	高精度定时器
		UINT m_wAccuracy;
		UINT m_TimerID;
		static void PASCAL PrecisionTimerProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
		{
			xmThreadw* pThread = (xmThreadw*)dwUser;
			if (pThread->m_uWLSCyc)
			{
				pThread->m_eWLS.Set();
				if (pThread->m_OnWLSDone != NULL)
				{
					if (pThread->m_eWLSDone.Wait(1000))
					{
						pThread->m_OnWLSDone(pThread->m_fWLPrecent, pThread->m_pWlsParam);
					}
				}
			}
		}


	public:
		/*==============================================================================
		函数名：实现Waitable类的接口
		说  明：获取互斥量句柄或描述符
		==============================================================================*/
#if defined(xmOS_WINDOWS)
		HANDLE GetHandle() const
		{
			return m_hThread;
		}
#elif defined(xmOS_LINUX)
		int GetHandle() const
		{
			return m_hThread;
		}
#endif
	protected:
		virtual void OnGetObject()
		{
			CloseHandle(m_hThread);

			m_hThread = INVALID_THREAD_VALUE;
			m_dwThreadID = 0;
			m_pDefParam = NULL;
			m_nPriority = xmTP_NORMAL;
			m_WaitSet.Clear();
			m_eQuit.Reset();
			m_eReady.Reset();
			m_mapWaitable.clear();

			m_WaitSet.Add(m_eQuit);

			m_PreProcAttr.m_pProcesser = NULL;
			m_PreProcAttr.m_pParam = NULL;
			m_PostProcAttr.m_pProcesser = NULL;
			m_PostProcAttr.m_pParam = NULL;
		}
	private:
		//	线程句柄
#if defined(xmOS_WINDOWS)
		HANDLE m_hThread;
#elif defined(xmOS_LINUX)
		pthread_t m_hThread;
#endif	
		DWORD m_dwThreadID;
		void* m_pDefParam;
		xmEThreadPriority m_nPriority;

		xmWaitSet m_WaitSet;
		xmEvent m_eQuit;
		xmEvent m_eReady;
		std::map<xmWaitable*, ProcesserAttribute> m_mapWaitable;

#if defined(xmOS_WINDOWS)
		static DWORD WINAPI s_ThreadFunc(LPVOID lpThreadParameter);
#elif defined(xmOS_LINUX)
		static void* s_ThreadFunc(void* lpThreadParameter);
#endif

		PreProcesserAttribute m_PreProcAttr;
		PostProcesserAttribute m_PostProcAttr;
	};
}

#endif	//	__SSA_BASE_THREADW_H
