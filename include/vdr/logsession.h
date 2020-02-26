#pragma once
#ifndef __SSA_VDR_LOGSESSION_H
#define __SSA_VDR_LOGSESSION_H
#include "session.h"
#include "userpackets.h"

namespace ssa
{
	class xmLogSession : public xmSession
	{

	public:
		xmLogSession(int id);
		virtual ~xmLogSession();

		typedef xmObjectRegistry<xmLogSession::OpcodeHandler<xmLogSession>, unsigned short>  OpcodeHandlerRegistry;

		virtual void      HandlePacket(const xmPacket& pkt);
		virtual void      FillOpcodeHandlerHashTable();
		xmESessionType    GetSessionType() const { return xmESessionType::xmEST_LOG; };

	private:
		virtual bool      HandleLog(const xmPacket& pkt);

	private:
		OpcodeHandlerRegistry m_OpcodeHandlerFactory;

	};

	class xmVDR_EXPORT xmSingletonLog
	{
		xmSINGLETON_DECLARE(xmSingletonLog)
	public:
		~xmSingletonLog()
		{
			delete m_pLogSession;
			m_bIsValid = false;
			m_nShowLevel = 0;
		};

		void         InitSingletonLog()
		{
			m_pLogSession = new xmLogSession(xmESessionType::xmEST_LOG);
			m_bIsValid = true;
			m_nShowLevel = 0;
		};

		void         DeleteSingletonLog()
		{
			delete m_pLogSession;
			m_pLogSession = 0;
			m_nShowLevel = 0;
		}

		int          PostLog(int nLogLevel, const char *lpcszFormat, ...);
		void         SetShowLevel(int nLevel);

	public:
		xmLogSession* m_pLogSession;
		bool          m_bIsValid;
		int           m_nShowLevel;
	};
}

#endif // !__SSA_VDR_LOGSESSION_H
