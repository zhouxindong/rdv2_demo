/*******************************************************************************
Copyright (C), 2011-2022, ACC12. All rights reserved.
模 块 名：base
文 件 名：msghandler.h
相关文件：message.h、msghandler.cpp
文件版本: 0.5
功能概述：消息处理类的头文件。
多 线 程：支持
异常安全：否
作    者：邓华
联系方式：dehxya@163.com
详细说明：每一个消息处理类包括一个消息队列和一个用来处理发送给该消息处理类消息的线程
         （Message Handle Thread，MHT）。目前不考虑增加一个消息处理类拥有多个消息
		 处理线程以加大消息处理能力的功能，因为如果多个线程同时处理消息，将带来两个
		 显著问题：一是消息的处理和发送失去了时间上的对应关系；二是使用者的消息处理
		 函数必须是可重入的，为开发程序带来了困难。
--------------------------------------------------------------------------------
修改记录：
	日期				版本				修改人			联系方式
	2015/04/01		V5.0			邓华				dehxya@163.com
		--> 针对SSA V5进行了移植，将消息处理线程的实现由xmThread改为xmThreadw，去掉了
		消息处理中的异常记录
	2011/11/26      0.5				邓华				dehxya@163.com
	    --> 去掉了公有消息队列的支持，所有xmMsgHandler均拥有自己的消息队列。
		优化了部分实现。
	2010/03/10		0.2				邓华				dehxya@163.com
		--> 使用SSA的Base包修改了部分实现
	2009/07/12		0.1				邓华				dehxya@163.com
		--> First Draft
*******************************************************************************/
#ifndef __SSA_BASE_MSGHANDLER_H
#define __SSA_BASE_MSGHANDLER_H
#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "buffer.h"
#include "threadw.h"
#include "message.h"
#include "timeutil.h"
#include "async.h"
#include "waitset.h"

/*******************************************************************************
名    称：xmCREATE_MESSAGE_MAP()
功    能：在类的声明中使用，为一个xmMsgHandler类的子类创建消息分配表
参    数：
          theHandlerClass：消息处理类名称
说    明：详见xmMsgHandler说明
*******************************************************************************/
#define xmCREATE_MESSAGE_MAP(theHandlerClass)                                   \
private:                                                                        \
	static const ssa::xmMsgHandler::__SMsgTableEntry __m_MsgTableEntries[];     \
	virtual ssa::xmMsgFunc __GetMsgFunc(unsigned int uMsgID);					\
	virtual void __foo(void){}




/*******************************************************************************
名    称：xmBEGIN_MESSAGE_MAP()、xmON_MESSAGE()、xmEND_MESSAGE_MAP()
功    能：在类的实现中使用，用于定义一个消息处理类接收的消息和消息处理函数之间的
          关系
参    数：
          theHandlerClass：消息处理类名称
		  theMsgID       ：该消息处理类可以处理的消息ID
		  theMsgFunc     ：该消息的消息处理函数类型
		  OnMsg          ：在消息处理类中声明的消息处理函数名称
说    明：详见xmMsgHandler说明
*******************************************************************************/
#define xmBEGIN_MESSAGE_MAP(theHandlerClass)                                        \
	const ssa::xmMsgHandler::__SMsgTableEntry theHandlerClass::__m_MsgTableEntries[] =   \
	{
#define xmON_MESSAGE(theMsgID, theMsgFunc, OnMsg)                                   \
		ssa::xmMsgHandler::__SMsgTableEntry(theMsgID, (ssa::xmMsgFunc)static_cast<theMsgFunc>(&OnMsg)),
#define xmEND_MESSAGE_MAP(theHandlerClass)                                          \
		ssa::xmMsgHandler::__SMsgTableEntry(0, NULL)                                \
	};                                                                              \
	ssa::xmMsgFunc theHandlerClass::__GetMsgFunc(unsigned int uMsgID)               \
	{                                                                               \
		if(uMsgID == 0)                                                             \
		{                                                                           \
			return NULL;                                                            \
		}                                                                           \
		for(int i=0; __m_MsgTableEntries[i].m_uMsgID != 0; i++)                     \
		{                                                                           \
			if(__m_MsgTableEntries[i].m_uMsgID == uMsgID)                           \
			{                                                                       \
				return __m_MsgTableEntries[i].m_pfnMsgFunc;                         \
			}                                                                       \
		}                                                                           \
		return NULL;                                                                \
	}



/*******************************************************************************
名    称：xmMH_DESTRUCT_IMMEDIATELY()、xmMH_DESTRUCT_WAIT_IDLE()
功    能：用于安全的销毁一个消息处理类
参    数：
          无
说    明：当销毁一个消息处理类时，发送给该消息处理类的消息有可能还没有被处理完，
          依然存在于消息队列中等待处理。下面两个宏必须在消息处理类的析构函数中首
		  先被调用，它们为用户提供两种安全退出消息循环的方式：xmMH_DESTRUCT_IMMEDIATELY
		  宏用于立即退出消息循环，所有未被处理的消息将被丢掉；xmMH_DESTRUCT_WAITIDLE
		  宏则等待直到所有待处理消息被处理完。
*******************************************************************************/
#define	xmMH_DESTRUCT_IMMEDIATELY()	\
	do								\
	{								\
		Pause();					\
		ClearMsgQueue();			\
	}while(0)
#define	xmMH_DESTRUCT_WAITIDLE()	\
	do								\
	{								\
		Pause();					\
		WaitIdle();					\
	}while(0)





namespace ssa
{
	/*******************************************************************************
	类名称：xmMsgHandler
	功  能：所有SSA消息处理类的父类
	说  明：xmMsgHandler为用户提供实现消息处理类的接口。用户实现一个消息处理类，必须
	        完成下列编码：
				1、创建一个自定义消息处理类，继承于xmMsgHandler；
				2、声明并实现消息处理函数；
				3、在自定义消息处理类的声明中使用xmCREATE_MESSAGE_MAP宏定义消息分配表；
				4、在类的实现文件中使用xmBEGIN_MESSAGE_MAP、xmON_MESSAGE、xmEND_MESSAGE_MAP
				   宏指定消息分配关系；
			    5、在析构函数中首先调用xmMH_DESTRUCT_IMMEDIATELY或者
				   xmMH_DESTRUCT_WAIT_IDLE宏。

			例如，一个用户自定义的xmTestHandler消息处理类，可以这样编写：
			//	In TestHandler.h
			//	This xmTestHandler will hanlde xmMSGMine message
			//	xmMSGMine is a user define message.
			#include "MSGMine.h"

			class xmTestHandler : public xmMsgHandler
			{
			public:
				xmTestHandler(void){};
				virtual ~xmTestHandler(void);
			public:
				//	Other codes.
				... ...

				//	xmMSGMine Message Callback Function.
				void OnMSGMine(const xmMSGMine& aMsg);

				xmCREATE_MESSAGE_MAP(xmTestHandler);
			};


			//	In TestHandler.cpp
			#include "TestHandler.h"

			xmBEGIN_MESSAGE_MAP(xmTestHandler)
				xmON_MESSAGE(xmMSG_MINE, MSGMineFunc, OnMSGMine)
			xmEND_MESSAGE_MAP(xmTestHandler)

			xmTestHandler::~xmTestHandler(void)
			{
				xmMH_DESTRUCT_IMMEDIATELY();	// or xmMH_DESTRUCT_WAIT_IDLE();

				//	Other Codes.
				... ...
			}
			void xmTestHandler::OnMSGMine(const xmMSGMine& aMsg)
			{
				printf("xmTestHandler Received a xmMSGMine Message\n");
			}
	*******************************************************************************/
	class xmBASE_EXPORT xmMsgHandler : public xmWaitable
	{
	public:
		/*==============================================================================
		函 数 名：xmMsgHandler()、~xmMsgHandler()
		功    能：构造和析构函数
		参    数：
				  nPriority：指定消息处理线程的优先级；
				  uMaxLen：最大的消息队列长度，如果为0，则不限制；
		返 回 值：无
		抛出异常：无
		==============================================================================*/
		xmMsgHandler(bool bPause = false, int nPriority = xmTP_NORMAL, unsigned int uMaxLen = 0);
		virtual ~xmMsgHandler(void);

		/*==============================================================================
		函 数 名：SendMsg()、PostMsg()、RushMsg()
		功    能：向一个消息处理类发送一个消息
		参    数：
				  aReceiver：消息接收者
				  aMessage：消息
		返 回 值：
				  0：发送成功；
				  xmE_MQ_FULL：消息队列满；
				  xmE_MQ_CLOSED：消息处理线程暂停接收消息
		抛出异常：无
		说    明：发送消息有Send、Post、Rush三种方式，SendMsg为同步处理过程，直接调用消息处
		          理函数，直到该消息处理结束后返回。PostMsg、RushMsg均为异步处理过程，在将
				  消息放入消息队列后立即返回，其发送的消息由消息处理线程异步处理。两者不同在
				  于PostMsg将消息放到队尾，以FIFO方式进行处理；而RushMsg将消息插入到队首，
				  一旦消息处理线程完成上一个消息的处理，将立即处理RushMsg发送的消息。
		==============================================================================*/
		inline xmRet SendMsg(const xmMessage& aMessage);
		static xmRet SendMsg(xmMsgHandler& aReceiver, const xmMessage& aMessage)
		{
			return aReceiver.SendMsg(aMessage);
		}
		inline xmRet PostMsg(const xmMessage& aMessage);
		static xmRet PostMsg(xmMsgHandler& aReceiver, const xmMessage& aMessage)
		{
			return aReceiver.PostMsg(aMessage);
		}
		inline xmRet RushMsg(const xmMessage& aMessage);
		static xmRet RushMsg(xmMsgHandler& aReceiver, const xmMessage& aMessage)
		{
			return aReceiver.RushMsg(aMessage);
		}

		/*==============================================================================
		函 数 名：GetMsgCount()
		功    能：获得未处理的消息个数
		参    数：无
		返 回 值：队列长度
		抛出异常：无
		说    明：
		         该返回值不包括当前正在被处理的消息，也就是说，如果GetMsgCount返回0，可能有
				 一个消息正在被MHT处理，并非idle状态。
		==============================================================================*/
		inline unsigned int GetMsgCount(void) const
		{
			xmMutex::Guard mutexGuard(m_QueueMutex);
			return (unsigned int)m_lMsgQueue.size();
		}

		/*==============================================================================
		函 数 名：ClearMsgQueue()、WaitIdle()
		功    能：清空消息队列
		参    数：
		          uMilliseconds：等待idle的最长时间，单位：毫秒，如果为INFINITE则持续等待。
		返 回 值：
		         xmEC(ED_TIMEOUT)：等待超时
				 xmEC(ED_SUCCESS)：处理成功
		抛出异常：无
		说    明：
		         提供两种清空消息队列的方式，ClearMsgQueue直接删除所有未处理的消息，WaitIdle
				 则等待所有消息被处理完成。两函数返回时，均可以确保消息队列中无消息，同时没有
				 消息正在被处理。
				 ClearMsgQueue返回前，如发送消息，则发送消息的线程将被挂起，直到该函数返回。
		==============================================================================*/
		void ClearMsgQueue(void);
		bool WaitIdle(unsigned int uMilliseconds = INFINITE);

		/*==============================================================================
		函 数 名：Pause()、Resume()、CanReceive()
		功    能：改变接收消息的状态
		参    数：无
		返 回 值：无
		抛出异常：无
		说    明：
		         当调用RecvPause暂停接收消息后，如果再发送消息给该xmMsgHandle，则发送消息的
				 函数返回xmEC(ED_REFUSED)，直到调用RecvResume恢复接收消息。
		==============================================================================*/
		inline void Pause(void)
		{
			m_bRecvMsg = false;
		};
		inline void Resume(void)
		{
			m_bRecvMsg = true;
		}
		inline bool CanReceive(void) const
		{
			return m_bRecvMsg;
		}
		
#if defined(xmOS_WINDOWS)
		HANDLE GetHandle() const
		{
			return m_QueueIdle.GetHandle();
		}
#elif defined(xmOS_LINUX)
		int GetHandle() const
		{
			return m_QueueIdle.GetHandle();
		}
#endif	

		xmRet StartWorkLoadStatistics(unsigned int uTime, xmThreadw::OnWLSDoneFunc OnWLSDone = NULL, void* pParam = NULL)
		{
			return m_MHT.StartWorkLoadStatistics(uTime, OnWLSDone, pParam);
		}
		void StopWorkLoadStatistics(void)
		{
			m_MHT.StopWorkLoadStatistics();
		}
		double WorkLoad(void) const
		{
			return m_MHT.WorkLoad();
		}
	private:
		//	处理消息的线程
		xmThreadw m_MHT;
		static int MHThreadFunc(void* pParam, xmWaitable* pWaitale)
		{
			((xmMsgHandler*)pParam)->__OnMessage();
			return 0;
		}

		//	在队列头弹出一个消息
		inline xmMessage* __PopFront(void);
		//	当消息消息被处理完成后，用于释放信号量
		inline void __HandleDone(void);
		//	被消息处理线程使用的消息处理函数
		inline void __OnMessage(void);
		//	处理一个消息
		inline void __MessageHandle(const xmMessage& aMessage);

		//	消息队列
		std::list<xmMessage*> m_lMsgQueue;
		//	消息信号，其信号数与当前队列中未处理的消息数相同，不包括正在处理的消息
		xmSemaphore m_sHaveMsg;
		//	操作消息队列时的保护
		mutable xmMutex m_QueueMutex;

		//	消息队列空闲状态事件
		xmEvent m_QueueIdle;

		//	是否允许接收消息
		volatile bool  m_bRecvMsg;
		//	是否处理消息
		volatile bool  m_bHandleMsg;

		//	消息队列允许的最大长度，如果为0则不限制，当当前消息队列长度超过m_uMaxLen时，PostMsg和RushMsg会失败
		const unsigned int m_uMAX_LEN;

	protected:
		/*******************************************************************************
		Message and Message Function Struct
		记录了消息ID及处理该类消息函数的对应关系
		*******************************************************************************/
		struct xmBASE_EXPORT __SMsgTableEntry
		{
		public:
			__SMsgTableEntry(unsigned int uMsgID, xmMsgFunc pfnMsgFunc) : m_uMsgID(uMsgID), m_pfnMsgFunc(pfnMsgFunc){};

			unsigned int m_uMsgID;
			xmMsgFunc m_pfnMsgFunc;
		};
	private:
		virtual xmMsgFunc __GetMsgFunc(unsigned int uMsgID){ return NULL;};
		virtual void __foo(void) = 0;
	};
}
#endif	//	__SSA_BASE_MSGHANDLER_H