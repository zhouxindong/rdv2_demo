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
		xmScriptEngine(const char* strScript) : m_strStript(strScript){};
		const xmString m_strStript;
	};
}

#endif
