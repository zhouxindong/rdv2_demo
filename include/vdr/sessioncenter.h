#ifndef __SSA_VDR_SEESIONMGR_H
#define __SSA_VDR_SEESIONMGR_H

#include "session.h"
#include "../base/async.h"
#include <shared_mutex>

namespace ssa
{
	class xmSessionGroup
	{
	private:
		friend class xmSessionCenter;
		typedef xmObjectRegistry<xmSession, unsigned int>     SessionRegistry;
		SessionRegistry m_SessionRegistry;

		xmSession*  Create(unsigned int nSessionID, xmICommProxy * pCommProxy);
		xmSession*  GetSession(unsigned int nSessionID);
	};

	class xmSessionCenter
	{
		typedef xmObjectRegistry<xmSessionGroup , std::string>   SeesionGroupRegistry;
		SeesionGroupRegistry m_SessionGroupRegistry;
		std::shared_mutex    m_Mutex;

	public:
		xmSession*  Create(const std::string& strName , unsigned int sessionID, xmICommProxy * pCommProxy);
		xmSession*  GetSession(const std::string& strName, unsigned int nSessionID);
		void        PauseSession(const std::string& strName, unsigned int nSessionID);
		void        ClearSession(const std::string& strName, unsigned int nSessionID);
		void        ContinueSession(const std::string& strName, unsigned int nSessionID);
	};
}

#endif