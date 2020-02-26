#pragma once
#ifndef __SSA_VDR_LOGICCONTROLLER_H
#define __SSA_VDR_LOGICCONTROLLER_H

#include "sensmsghandler.h"
#include "order.h"

namespace ssa
{
	class xmLogicController
	{
		typedef xmObjectRegistry<xmOrder, int>  OrderRegistry;

	public:
		OrderRegistry m_OrderRegistry;
	};
}

#endif //__SSA_VDR_LOGICCONTROLLER_H