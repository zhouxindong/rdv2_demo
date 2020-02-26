#pragma once
#ifndef __SSA_VDR_ENDECODE_H
#define __SSA_VDR_ENDECODE_H

#include "export.h"
#include <errno.h>             // For errno
#include "packet.h"
#include "dataset.h"
#include "data.h"
#include "node.h"

using namespace std;

namespace ssa
{
	class xmVDR_EXPORT xmEnDecode
	{
	public:
		xmEnDecode();
		~xmEnDecode();

	public:
		static unsigned int   DataEncode(const xmData& in, unsigned char* out);
		static void           DataDecode(unsigned char* in, unsigned int uLength, xmData& out);
		static unsigned int   DataEncodeSize(const xmData& in);

		static unsigned int   DataSetEncode(const xmDataSet& in, unsigned char* out);
		static void           DataSetDecode(unsigned char* in, unsigned int uLength, xmDataSet& out);
		static unsigned int   DataSetEncodeSize(const xmDataSet& in);

		static unsigned int   NodeEncode(const xmNode& in, unsigned char* out);
		static void           NodeDecode(unsigned char* in, unsigned int uLength, xmNode& out);
		static unsigned int   NodeEncodeSize(const xmNode& in);
	};
}

#endif //__SSA_VDR_ENDECODE_H
