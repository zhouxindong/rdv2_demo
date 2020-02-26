#ifndef __SSA_UMSF_PROCESSOR_H
#define __SSA_UMSF_PROCESSOR_H
#include "base/base.h"

namespace ssa
{
	class xmIProcessor
	{
	public:
		//	strPath��Ԥ��������ļ�����һ��LUA�ű�
		//	����Ǽ����ļ���Ҫ���÷��š�@����ͷ������ļ�·����������*.dll����*.lua�ļ�
		static xmIProcessor* Create(const xmString& strPath);
		static void Destroy(xmIProcessor* pProcessor);

		//	��Ϊ������������������Դ�����Ч��xmValueʵ������Ԥ��������������
		//	������������ʵ�֣������ǿ������
		//	System2Model������������ϵͳֵ��ת��Ϊģ�ͼ���ֵ���������������룻
		//	Model2System��������ģ�ͼ���ֵ��ת��Ϊ����ϵͳֵ�����������������
		virtual xmRet System2Model(const xmValue& systemValue, xmValue& modelValue) = 0;
		virtual xmRet Model2System(const xmValue& modelValue, xmValue& systemValue) = 0;

	protected:
		xmIProcessor() {};
		virtual ~xmIProcessor() {};
	};
}
#endif  //  __SSA_UMSF_PROCESSOR_H

