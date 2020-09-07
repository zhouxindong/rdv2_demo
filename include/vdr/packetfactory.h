#pragma once
#ifndef __SSA_VDR_PACKETFACTORY_H
#define __SSA_VDR_PACKETFACTORY_H

#include "packet.h"

namespace ssa
{
	class xmPacketFactory
	{
		static void      DoPacket(xmPacket*& pkt, xmEPackCodes epc, bool isCreate);
	public:
		static xmPacket* CreatePacket(xmEPackCodes epc);
		static void      DestroyPacket(xmPacket* pkt);
	};
}

#endif