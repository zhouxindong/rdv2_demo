#ifndef __SSA_BASE_WAITABLE_H
#define __SSA_BASE_WAITABLE_H
#include "exceptions.h"
#include "assertmsg.h"
//#include "singleton.h"

namespace ssa
{
	/******************************************************************************/
	/** @class xmUncopyable
	@brief
		实现一个不可复制的对象
	@par 多线程安全
		不适用
	@par 示例
		@par
		@code
			class myClass : private xmUncopyable
			{
			public:
				myClass() : m_iParam(0){};
			private:
				int m_iParam;
			};

			myClass C1;		//	没问题
			myClass C2(C1);	//	编译错误。
		@endcode
	*******************************************************************************/
	class xmBASE_EXPORT xmUncopyable
	{
	protected:
		xmUncopyable(){};
		virtual ~xmUncopyable(){};
	private:
		xmUncopyable(const xmUncopyable&);
		xmUncopyable& operator=(const xmUncopyable&);
	};

	class xmBASE_EXPORT xmWaitable : private xmUncopyable
	{
		friend class xmWaitSet;
	public:
		enum
		{
			NOT_READY = 0,		// bin: 00000000
			READ = 1,			// bin: 00000001
			WRITE = 2,			// bin: 00000010
			READ_AND_WRITE = 3, // bin: 00000011
			EXCEPT = 4			// bin: 00000100
		};

		xmWaitable(int nName = 0) : m_pUserData(NULL), m_uWaitFlags(READ|WRITE), m_uRecvFlags(NOT_READY), m_nName(nName)
		{
		};
		virtual ~xmWaitable()
		{
		}

		/*==============================================================================
		函数名：SetWaitingEvents(), GetWaitingSignal()
		说  明：设置和获取希望等待的事件
		==============================================================================*/
		virtual void SetWaitingEvents(unsigned int flagsToWaitFor) 
		{
			m_uWaitFlags = flagsToWaitFor;
			m_uRecvFlags = NOT_READY;
		}
		unsigned int GetWaitingSignal(unsigned int signalFlag = NOT_READY) const;

		/*==============================================================================
		函数名：ReadSignalled(), WriteSignalled(), ErrorSignalled(), CheckSignalled()
		说  明：判断实际发生的事件
		==============================================================================*/
		inline bool ReadSignalled() const
		{
			return (m_uRecvFlags & READ) != 0;
		}
		inline bool WriteSignalled() const
		{
			return (m_uRecvFlags & WRITE) != 0;
		}
		inline bool ErrorSignalled() const
		{
			return (m_uRecvFlags & EXCEPT) != 0;
		}

		virtual inline bool CheckSignalled() const
		{
			return m_uRecvFlags != NOT_READY;
		}

		/*==============================================================================
		函数名：GetUserData(), SetUserData()
		说  明：设置和获取用户数据
		==============================================================================*/
		inline void SetUserData(void* data)
		{
			m_pUserData = data;
		}
		inline void* GetUserData() const
		{
			return m_pUserData;
		}

		/*==============================================================================
		函数名：GetName()
		说  明：获取名称
		==============================================================================*/
		inline int GetName() const
		{
			return m_nName;
		}

		/*==============================================================================
		函数名：GetHandle()
		说  明：获取Waitable句柄
		==============================================================================*/
	public:
#ifdef xmOS_WINDOWS
		virtual HANDLE GetHandle() const = 0;
#else
		virtual int GetHandle() const = 0;
#endif


	protected:
		mutable unsigned int m_uRecvFlags;

		/*==============================================================================
		函数名：OnGetObject()
		说  明：当等待的事件发生后，该函数由Waitset回调，完成特定工作
		==============================================================================*/
		virtual void OnGetObject(){}


		inline void SetRecvFlags(unsigned int uFlag) const
		{
			m_uRecvFlags = uFlag;
		}
		inline void AddRecvFlag(unsigned int uFlag) const
		{
			m_uRecvFlags |= uFlag;
		}
		inline void RemoveRecvFlag(unsigned int uFlag) const
		{
			m_uRecvFlags &= (~uFlag);
		}		
		inline void ClearRecvFlags() const
		{
			m_uRecvFlags = NOT_READY;
		}


	private:
		void* m_pUserData;
		unsigned int m_uWaitFlags;
		const int m_nName;
	};
}

#endif	//__SSA_BASE_WAITABLE_H
