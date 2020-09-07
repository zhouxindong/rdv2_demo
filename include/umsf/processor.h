#ifndef __SSA_UMSF_PROCESSOR_H
#define __SSA_UMSF_PROCESSOR_H
#include "base/base.h"

namespace ssa
{
	class xmMrtc;
	class xmIProcessor
	{
	public:
		//	strPath��Ԥ��������ļ�����һ��LUA�ű�
		//	����Ǽ����ļ���Ҫ���÷��š�@����ͷ������ļ�·����������*.dll����*.lua�ļ�
		static xmIProcessor* Create(const xmString& strPath, const xmString& strDir, xmMrtc* pMrtc, const xmString& strDataName);
		static void Destroy(xmIProcessor* pProcessor);

		//	��Ϊ������������������Դ�����Ч��xmValueʵ������Ԥ��������������
		//	������������ʵ�֣������ǿ������
		//	System2Model������������ϵͳֵ��ת��Ϊģ�ͼ���ֵ���������������룻
		//	Model2System��������ģ�ͼ���ֵ��ת��Ϊ����ϵͳֵ�����������������
		virtual xmRet System2Model(const xmValue& systemValue, xmValue& modelValue) = 0;
		virtual xmRet Model2System(const xmValue& modelValue, xmValue& systemValue) = 0;

		const xmString& GetPath() const
		{
			return m_strProcessorPath;
		}
		const xmString& Flag() const
		{
			return m_strFlag;
		}

	protected:
		xmIProcessor(const xmString& strPath, xmMrtc* pMrtc, const xmString& strDataName);
		virtual ~xmIProcessor() {}

		const xmString m_strProcessorPath;
		const xmString m_strDataName;

	private:
		const xmString m_strFlag;
	};
}
#endif  //  __SSA_UMSF_PROCESSOR_H

