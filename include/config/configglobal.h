
#ifndef __CONFIG_GLOBAL_H
#define __CONFIG_GLOBAL_H

#include "..\base\environment.h"

#ifdef xmOS_WINDOWS
#ifdef __SSA_CONFIG_EXPORT
#define xmCONFIG_EXPORT __declspec(dllexport)
#else
#define xmCONFIG_EXPORT __declspec(dllimport)
#endif
#else  // xmOS_LINUX
#define xmCONFIG_EXPORT
#endif

#endif // !__CONFIG_GLOBAL_H
