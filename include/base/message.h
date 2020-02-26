/*******************************************************************************
Copyright (C), 2011-2022, V5. All rights reserved.
模 块 名：base
文 件 名：message.h
相关文件：msghandler.h、msghandler.cpp
文件版本: 5.0
功能概述：定义了所有SSA消息的父类，实现了两种系统定义的消息。
多 线 程：不支持，多线程并发访问时需要显式的保护
异常安全：不支持
详细说明：一个SSA消息包括三部分，在代码中分别使用下列符号表示：
			消息ID			：MsgID
			消息类			：MsgClass
			消息处理函数类型：MsgFunc，包括消息处理函数（OnMsg）	 
备注信息：后续有待将数据内容的描述与数据分离，使得同一个描述适用于不同的数据，使得进行
		 拷贝构造时，节省重新映射的过程。
--------------------------------------------------------------------------------
修改记录：
    日期                    版本            修改人            联系方式
	2015/04/01				V5.0			邓华				  dehxya@163.com
		--> 针对SSA V5进行了移植
	2010/03/10				V0.2			邓华	              dehxya@163.com
        --> 在MsgHandler的声明文件中独立，修改了部分实现
    2009/07/12				V0.1		    邓华              dehxya@163.com
        --> 代码初次完成。
*******************************************************************************/
#ifndef __SSA_BASE_MESSAGE_H
#define __SSA_BASE_MESSAGE_H
#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "buffer.h"


namespace ssa
{
	/*******************************************************************************
	名    称：xmDECLARE_MESSAGE_CLASS()
	功    能：在类的声明中使用，用于将一个xmMessage类的子类定义为消息
	参    数：theMsgID：消息的ID号，自定义消息的ID号必须大于xmMSG_USR
	说    明：详见xmMessage说明
	*******************************************************************************/
#define xmDECLARE_MESSAGE_CLASS(theMsgID)								    \
    private:																\
		virtual unsigned int __GetMsgID(void) const {return theMsgID;};	


	/*******************************************************************************
	名    称：xmDECLARE_MESSAGE_FUNC()
	功    能：在类的声明代码之后使用，必须与类的声明代码存在与同一个文件。用于声明一
	          个自定义消息类的消息处理函数的类型。
	参    数：
			  theMsgFuncType：自定义消息的消息处理函数类型，由用户自己定义
              theMsgClass   ：自定义消息类的类名
	说    明：详见xmMessage说明
	*******************************************************************************/
#define xmDECLARE_MESSAGE_FUNC(theMsgFuncType, theMsgClass)				\
    typedef void (ssa::xmMsgHandler::*theMsgFuncType)(const theMsgClass&)




	/*******************************************************************************
	类名称：xmMessage
	功  能：所有SSA消息类的父类
	说  明：xmMessage为用户提供实现自定义消息的接口。用户实现自定义消息，必须完成下列
	        编码：
				1、为自定义消息分配一个消息ID号，此ID号必须大于xmMSG_USR；
				2、创建一个自定义消息类，继承于xmMessage；
				3、在自定义消息类的声明文件中使用xmDECLARE_MESSAGE_CLASS和
				   xmDECLARE_MESSAGE_FUNC宏；
				4、实现Clone()方法
	        例如，一个用户自定义的xmMSGMine消息，可以这样编写：
				//	In MSGMine.h
				#define xmMSG_MINE	(xmMSG_USR + 1)
				class xmMSGMine : public xmMessage
				{
				public:
					xmMSGMine(void);
					virtual ~xmMSGMine(void);
				public:
					virtual xmMessage* Clone(void) const
					{
						return new xmMSGMine(*this);
					};
					//	Other codes.
					... ...

				xmDECLARE_MESSAGE_CLASS(xmMSG_MINE);
				};
				xmDECLARE_MESSAGE_FUNC(MSGMineFunc, xmMSGMine);
	*******************************************************************************/
    class xmMsgHandler;
    class xmBASE_EXPORT xmMessage
    {
		friend class xmMsgHandler;
	public:
        xmMessage(void){};
        virtual ~xmMessage(void){};

	private:
		/*==============================================================================
		函数名称：Clone()
		用    途：创建一个自身镜像
		参    数：
		          无
		返 回 值：
		          xmMessage类指针返回
	    说    明：Clone函数创建一个自定义消息类，并将其指针转换为xmMessage类指针返回。
		          创建的新消息，Clone函数不需要负责销毁，将有xmMsgHandler负责销毁。
	    ==============================================================================*/
		virtual xmMessage* Clone(void) const = 0;

		/*==============================================================================
		函数名称：__GetMsgID()
		用    途：获得一个消息的ID号
		参    数：
		          无
		返 回 值：
		          消息ID
		说    明：此函数用户不需要关心，也不需要使用，是为了进行消息分配而设计的。
		==============================================================================*/
        virtual unsigned int __GetMsgID(void) const = 0/*{ return 0; }*/;
    };
	//	定义了可以处理xmMessage消息的消息处理函数类型，这个类型用户可以不用关心，仅被
	//	xmMsgHandler类使用
    xmDECLARE_MESSAGE_FUNC(xmMsgFunc, xmMessage);






	/*******************************************************************************
	下列代码定义了两种SSA自身提供的消息，供用户和SSA中其它代码使用
	*******************************************************************************/
	//	xmMSGWindows消息的消息ID
#define xmMSG_WINDOWS       0x1
	//	xmMSGBuffer消息的消息ID
#define xmMSG_BUFFER		0x2
	//	用户可以使用的最小消息ID
#define xmMSG_USR           0x1000


	/*******************************************************************************
	类名称：xmMSGWindows
	功  能：SSA自定义的消息，模拟Windows系统的消息内容
	说  明：
	*******************************************************************************/
#if defined(xmOS_LINUX)
	typedef unsigned int WPARAM;
	typedef long  LPARAM;
#endif	
    class xmBASE_EXPORT xmMSGWindows : public xmMessage
    {
    public:
        xmMSGWindows(void)
		{
			m_uMsg = 0;
			m_wParam = 0;
			m_lParam = 0;
		}
        xmMSGWindows(const xmMSGWindows& msgWnd)
		{
			*this = msgWnd;
		}
		virtual ~xmMSGWindows(void){};

    public:
        unsigned int m_uMsg;
        WPARAM m_wParam;
        LPARAM m_lParam;
        xmMSGWindows& operator=(const xmMSGWindows& msgWnd)
		{
			m_uMsg = msgWnd.m_uMsg;
			m_wParam = msgWnd.m_wParam;
			m_lParam = msgWnd.m_lParam;
			return *this;
		}
        bool operator==(const xmMSGWindows& msgWnd) const
		{
			if (m_uMsg != msgWnd.m_uMsg)
			{
				return false;
			}
			if (m_wParam != msgWnd.m_wParam)
			{
				return false;
			}
			if (m_lParam != msgWnd.m_lParam)
			{
				return false;
			}
			return true;
		}

	private:
        virtual xmMessage* Clone(void) const
		{
			return new xmMSGWindows(*this);
		}
        xmDECLARE_MESSAGE_CLASS(xmMSG_WINDOWS);
    };
    xmDECLARE_MESSAGE_FUNC(xmMSGWindowsFunc, xmMSGWindows);


	/*******************************************************************************
	类名称：xmMSGBuffer
	功  能：SSA自定义的消息，可以传递一个xmDynamicBuffer。
	说  明：
	*******************************************************************************/
	class xmBASE_EXPORT xmMSGBuffer : public xmMessage
	{
	public:
		xmMSGBuffer(void){};
		xmMSGBuffer(const xmMSGBuffer& msgBuf)
		{
			*this = msgBuf;
		}
		virtual ~xmMSGBuffer(void){};

	public:
		xmBuffer m_DBuffer;

		xmMSGBuffer& operator=(const xmMSGBuffer& msgBuf)
		{
			m_DBuffer.Malloc(msgBuf.m_DBuffer);
			return *this;
		}
		bool operator==(const xmMSGBuffer& msgBuf) const
		{
			return (m_DBuffer == msgBuf.m_DBuffer);
		}

	private:
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGBuffer(*this);
		}

		xmDECLARE_MESSAGE_CLASS(xmMSG_BUFFER);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGBufferFunc, xmMSGBuffer);
}
#endif	//	__SSA_BASE_MESSAGE_H