#pragma once
#ifndef __SSA_VDR_LOGICLAYER_H
#define __SSA_VDR_LOGICLAYER_H

#include "sensmsghandler.h"
#include "order.h"

namespace ssa
{
	class xmLogicLayer
	{
		typedef xmObjectRegistry<xmOrder, int>  OrderRegistry;

	public:
		OrderRegistry m_OrderRegistry;
	};
}

#endif //__SSA_VDR_LOGICLAYER_H