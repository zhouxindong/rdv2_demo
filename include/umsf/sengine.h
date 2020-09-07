#ifndef __SSA_UMSF_SENGINE_H
#define __SSA_UMSF_SENGINE_H
#include "udefine.h"

namespace ssa
{
	class xmScriptEngine
	{
	public:
		typedef enum
		{
			etLUA,
			etJS
		} EEngineType;
		virtual ~xmScriptEngine() {};
		//	�ű�������
		virtual EEngineType Type() const = 0;
	protected:
		//	strScriptΪ�ű��ļ�·����ű����ݣ���������ű��ļ�·�����ɲ�ͬ�ű�ʵ�־���
		//	strScriptAbsPathΪ�ű��ļ�����·������β��������\��
		xmScriptEngine(const char* strScript, const char* strScriptAbsPath) : m_strStript(strScript), m_strAbsPath(strScriptAbsPath) {};
		const xmString m_strStript;
		const xmString m_strAbsPath;
	};
}

#endif
