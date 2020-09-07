#pragma once
#ifndef __SSA_VDR_TOOLS_H
#define __SSA_VDR_TOOLS_H

#include "export.h"
#include <time.h>
#include <vector>
#include <string>

namespace ssa
{
	class xmVDR_EXPORT xmTools
	{
	public:
		static unsigned int GetRandSN(unsigned int nCode = 0);
		static std::vector<std::string> Split2(std::string str ,const char* flag);
		static long long GetSysTimeMicros();
	};
}


#endif
