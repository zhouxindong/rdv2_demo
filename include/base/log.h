#ifndef __SSA_BASE_LOG_H
#define __SSA_BASE_LOG_H
#include "stringutil.h"
#include "singleton.h"
#include "timeutil.h"
#include "msghandler.h"
#include "rwlock.h"
#include "omap.h"

namespace ssa
{
	//	xmLog使用了读写锁，会造成一定程度的性能损失
	class xmLogTarget;
	class xmBASE_EXPORT xmLog : xmUncopyable
	{
	public:
		typedef enum __tagLogTarget
		{
			LT_STDIO = 0,
			LT_FILE,
			LT_PIPE
		}ELogTarget;
		typedef enum __tagLogLevel
		{
			LL_LOG = 0,
			LL_DEBUG,
			LL_INFO,
			LL_WARNING,
			LL_ERROR,
			LL_FATAL,
			LL_ALL,
		}ELogLevel;
		static const char* LevelName(ELogLevel eLevel);

		typedef void (*OnLogMessageFunc)(const xmString&, void*);

		//	创建管道日志目标时，管道的配置属性
		typedef struct _tagPipeLogAttr
		{
			_tagPipeLogAttr() : m_fpOnLogMessage(NULL), m_pParam(NULL) {};
			_tagPipeLogAttr(const _tagPipeLogAttr& attr) : m_strPipeName(attr.m_strPipeName), m_fpOnLogMessage(attr.m_fpOnLogMessage), m_pParam(attr.m_pParam) {};
			xmString m_strPipeName;
			OnLogMessageFunc m_fpOnLogMessage;
			void* m_pParam;
		}SPipeLogAttrbute;

		//////////////////////////////////////////////////////////////////////////
		//	日志管理函数，与下列日志记录函数不可重入
		//////////////////////////////////////////////////////////////////////////
		typedef size_t t_LogTargrtID;
		static t_LogTargrtID AddLogTarget(ELogTarget eTarget, const void* strAttrbute = NULL);
		//	如果uMilliseconds为0，表示立即清除所有未输出日志内容，之后删除日志目标
		//	如果uMilliseconds为非0，表示等待日志内容输出一段时间，在这段时间之后，无论是否输出完成，
		//	都会清除未输出日志内容，之后删除日志目标
		static void RemoveLogTarget(t_LogTargrtID uID, unsigned int uMilliseconds = INFINITE);	
		static void ClearLogTarget(unsigned int uMilliseconds = INFINITE);
		//	如果uMilliseconds为0，表示立即清除所有未输出日志内容
		//	如果uMilliseconds为非0，表示等待日志内容输出一段时间，在这段时间之后，无论是否输出完成，
		//	都会清除未输出日志内容
		static size_t RemoveUnprintedLog(t_LogTargrtID uID, unsigned int uMilliseconds = INFINITE);
		static size_t ClearUnprintedLog(unsigned int uMilliseconds = INFINITE);
		static size_t LogTargetCount();
		static t_LogTargrtID LogTargetID(size_t uIndex);
		static ELogTarget LogTargetType(t_LogTargrtID uID);
		static const void* LogTargetAttrbute(t_LogTargrtID uID);

		static void EnableTarget(bool IsEnabled);
		static void EnableTarget(t_LogTargrtID uID, bool IsEnabled);
		static void EnableLevel(ELogLevel eLevel, bool IsEnabled);
		static void EnableLevel(t_LogTargrtID uID, ELogLevel eLevel, bool IsEnabled);

		static void StartWLS(t_LogTargrtID uID, unsigned int uTime, xmThreadw::OnWLSDoneFunc OnWLSDone = NULL, void* pParam = NULL);
		static void StopWLS(t_LogTargrtID uID);
		static double WorkLoad(t_LogTargrtID uID);
		static unsigned int MsgCount(t_LogTargrtID uID);
		static bool WaitIdle(t_LogTargrtID uID, unsigned int uMilliseconds = INFINITE);
		static bool WaitIdle(unsigned int uMilliseconds = INFINITE);

		//////////////////////////////////////////////////////////////////////////
		//	日志记录函数，下列函数之间可以重入
		//////////////////////////////////////////////////////////////////////////
		//	todo函数什么都不干
		static void todo(const char* strFormat, ...);
		//	log函数不添加任何日志时间、等级、题目等前缀信息，直接输出内容，结尾添加“\n”；
		static void log(const char* strFormat, ...);
		//	下列五个日志函数在日志内容前添加任何日志时间、等级、题目等前缀信息，结尾添加“\n”；
		static void log(ELogLevel eLevel, const char* strTital, const char* strContent);
		static void debug(const char* strTital, const char* strFormat, ...);
		static void info(const char* strTital, const char* strFormat, ...);
		static void warning(const char* strTital, const char* strFormat, ...);
		static void error(const char* strTital, const char* strFormat, ...);
		static void fatal(const char* strTital, const char* strFormat, ...);
	private:
		//	Key表示日志目标的流水号
		static xmOrderMap<t_LogTargrtID, xmPtr<xmLogTarget> > m_mapLogTarget;
		//	添加日志目标时，生成流水号
		static t_LogTargrtID m_uLogTargetID;
		//	记录STDIO的ID
		static t_LogTargrtID m_uStdioTargetID;
		static xmRWLock m_rwLock;
	};
}

#endif	//__SSA_BASE_LIB_H
