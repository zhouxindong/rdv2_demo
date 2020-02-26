#ifndef __SSA_BASE_PROCESS_H
#define __SSA_BASE_PROCESS_H
#include "assertmsg.h"
#include "errors.h"
#include "exceptions.h"
#include "stringutil.h"
#include "singleton.h"
#include <cstring>
#include <sstream>

#if defined(xmOS_WINDOWS)
#include <process.h>
#include <TlHelp32.h>
#elif defined(xmOS_LINUX)
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <ctime>
#endif

namespace ssa
{
	class xmBASE_EXPORT xmProcess : private xmUncopyable
	{
	public:
		static HDESK CreateDesktop(const char* strName);
		static bool DestroyDesktop(HDESK hDesktop);

		static int Create(const char* strFullPath,	//	被启动进程的路径
			char* strCommandLine,					//	传给进程的参数
			const char* strWorkPath = NULL,			//	进程工作路径，如果为NULL，则默认与父进程相同路径
			const char* strDesktop = NULL);			//	进程工作桌面，如果为NULL，默认与父进程使用相同桌面，
													//	否则以隐藏的方式在指定桌面启动。该桌面在使用前必须先创建
		static bool IsAlive(const char* strFullPath);
		static bool Kill(const char* strFullPath);

	protected:
		static HANDLE __GetHandle(const char* strFullPath);

	private:
		xmProcess(){}
		~xmProcess(){};
	};
}
#endif	//__SSA_BASE_PROCESS_H