#ifndef __SSA_UMSF_PROCESSOR_H
#define __SSA_UMSF_PROCESSOR_H
#include "base/base.h"

namespace ssa
{
	class xmMrtc;
	class xmIProcessor
	{
	public:
		//	strPath：预处理计算文件或者一段LUA脚本
		//	如果是计算文件，要求用符号“@”开头后面接文件路径，可以是*.dll或者*.lua文件
		static xmIProcessor* Create(const xmString& strPath, const xmString& strDir, xmMrtc* pMrtc, const xmString& strDataName);
		static void Destroy(xmIProcessor* pProcessor);

		//	作为输出的两个参数，可以传入无效的xmValue实例，由预处理器创建类型
		//	这两个函数的实现，必须是可重入的
		//	System2Model函数：将仿真系统值，转换为模型计算值，适用于数据输入；
		//	Model2System函数：将模型计算值，转换为仿真系统值，适用于数据输出。
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

