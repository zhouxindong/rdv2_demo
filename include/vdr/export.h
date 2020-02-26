
#ifndef __SSA_VDR_EXPORT_H
#define __SSA_VDR_EXPORT_H

#include "../base/environment.h"

#ifdef xmOS_WINDOWS
#	ifdef __SSA_VDR_EXPORT
#		define xmVDR_EXPORT __declspec(dllexport)
#	else
#		define xmVDR_EXPORT __declspec(dllimport)
#	endif
#else  // xmOS_LINUX
#	define xmVDR_EXPORT
#endif


#endif //__SSA_VDR_EXPORT_H