#ifndef __SSA_VDR_SESSION_H
#define __SSA_VDR_SESSION_H

#pragma once
#include "../base/socket.h"
#include "../base/msghandler.h"
#include "defs.h"
#include "packet.h"
#include "icommproxy.h"
#include "objectregistry.h"
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
#include <shared_mutex>
#endif

namespace ssa
{
	class xmSession;

#define xmMSG_SESSION	(xmMSG_USR + 1)
	class xmMSGSession : public xmMessage
	{
	public:
		xmMSGSession(void) {};
		xmMSGSession(const xmMSGSession& msgSession)
		{
			m_ptMsg = msgSession.m_ptMsg;
			m_strContent = msgSession.m_strContent;
		}
		~xmMSGSession(void) {};
	public:
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGSession(*this);
		};

		xmPacket*   m_ptMsg;
		std::string m_strContent;

		xmDECLARE_MESSAGE_CLASS(xmMSG_SESSION);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGSessionFunc, xmMSGSession);


	class xmSessionHandler : public xmMsgHandler
	{
	public:
		//	ʹ�ö�������Ϣ�����̣߳�������ʵʱ��Ҫ��ϸߵĳ��ϡ�
		//	ͬʱ��Ϊ�˴ﵽ���߻��߸��͵Ĵ���ʵʱ�ԣ��û�������ָ����Ϣ�����̵߳����ȼ�
		xmSessionHandler(void) : m_pSe(0) {};
		~xmSessionHandler(void)
		{
			//	�������٣�����δ�������Ϣ��������
			xmMH_DESTRUCT_IMMEDIATELY();
		};

		void SetSession(xmSession* pSe)
		{
			m_pSe = pSe;
		}

		void OnMSGSession(const xmMSGSession& aMsg);

		xmCREATE_MESSAGE_MAP(xmSessionHandler);

	protected:
		xmSession* m_pSe;
	};

	//	�Ự״̬
	typedef enum __tagSessionState
	{
		xmESS_CLOSE,	    	       
		xmESS_CONNECT,         
	} xmESessionState;

	typedef enum __tagwParam
	{
		xmEP_NODE = 16,
		xmEP_DATASET= 16,
		xmEP_DATA = 32,
	} xmEwParam;

	class xmSession
	{

	public:
		xmSession();
		xmSession(int nSessionID);
		virtual ~xmSession();

		template<class T>
		class OpcodeHandler
		{
		public:
			OpcodeHandler() :handler(0)
			{
			};
			OpcodeHandler(bool (T::*_handler)(const xmPacket& pkt)) :handler(_handler)
			{
			};
			bool (T::*handler)(const xmPacket& pkt);
		};

		typedef xmObjectRegistry<xmSession::OpcodeHandler<xmSession>, xmEPackCodes>  OpcodeHandlerRegistry;

		//	������־
		typedef enum __tagSign
		{
			xmES_NOTHING = 0,
			xmES_ALLOW,	    	         //  allow
			xmES_SYSSETTUPOVER,          //  system setup over.
			xmES_ALLOWADDBEGIN,          //  add data begin.
			xmES_ADDOVER,                //  add over
			xmES_SYSASYN,
			xmES_NOPROBLEM,
		} xmESign;


		virtual void                 HandlePacket(const xmPacket& pkt) = 0;
		virtual void                 FillOpcodeHandlerHashTable() = 0;
		virtual xmESessionType       GetSessionType() const = 0;

		virtual void                 Close();
		virtual bool                 IsClosed();

		virtual int                  GetID() { return  m_nID; };
		virtual void                 Release() { m_bRelease = true; }
		virtual bool                 IsRelease() { return m_bRelease; }
		virtual xmESessionState      GetState() { return m_nState; }
		virtual void                 SetState(xmESessionState state) { m_nState = state; }

		void                         ClearMsgQ();
		void                         PauseMsgQ();
		void                         ContinueMsgQ();

		void                         SetCommproxy(xmICommProxy* pCommproxy) { m_pCommproxy = pCommproxy; };
		//Trans��Ϣ��Ϊ��Post��Send����ģʽ��ͨ��transMsgUsePostMode�����ã���transMsgUsePostModeΪtrur��ʾʹ��post��ʽ��falseʱ��ʾSend��ʽ
		virtual int                  TransMsg(const xmPacket& pkt, bool transMsgUsePostMode);
		virtual int                  TransMsg(const xmPacket& pkt, xmByteBuffer& wParam, bool transMsgUsePostMode, int waitTime);
		void                         FlowControl(const xmPacket& pkt, bool bUseSignal = false);
	protected:
		friend class xmSessionHandler;
		friend class xmSessionCenter;

		void                 Send(const xmPacket& pkt);

	protected:
		//session id
		int                  m_nID;
		xmESessionState      m_nState;
		bool                 m_bRelease;
		xmICommProxy*        m_pCommproxy;
		xmSessionHandler     m_SessionHandler;
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
		std::shared_mutex    m_Mutex;
#else
		xmMutex              m_Mutex;
#endif

	public:
		int                  m_nLienseNo;
		unsigned short       m_eCode;
		//�̶��ֽ�Ϊһ�飬��һ��ΪNode���ڶ���ΪDataSet��������ΪData
		xmByteBuffer         m_wParam;
		xmEvent              m_evtAsyn;
		unsigned int         m_uFlowSeq;
	};
};

#endif