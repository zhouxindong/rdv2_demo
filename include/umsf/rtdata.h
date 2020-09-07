#ifndef __SSA_UMSF_RTDATA_H
#define __SSA_UMSF_RTDATA_H
#include "rtobject.h"
#include "publicres.h"
namespace ssa
{
	//	��������ʱ��ģ�����ݣ�����ʱ���ݿ���Ҫ�����ϵͳ��ڵ����ݽ���ƥ��
	//	����ʱ���ݵ�IOֻ��������Ϊ��λ���������뵽���ԡ�
	class xmModelData;
	class xmDataTrigger;
	class xmRuntimeData : public xmRuntimeObject
	{
		friend xmUMSF;
		friend xmDataTrigger;
	public:
		virtual ~xmRuntimeData() {};
		virtual const xmString& Name() const
		{
			return DataName();
		}
	protected:
		virtual void _MakeFlag() const;

	public:
		typedef struct _tagMatchInfo
		{
			_tagMatchInfo() : m_bMatchTarget(true), m_eMatchTarget(DMT_NULL), m_uAutoReset(0xFFFFFFFF) {};
			_tagMatchInfo(const _tagMatchInfo& mi) : 
				m_bMatchTarget(mi.m_bMatchTarget),
				m_eMatchTarget(mi.m_eMatchTarget), 
				m_strMatchName(mi.m_strMatchName),
				m_strInitString(mi.m_strInitString),
				m_strProcessor(mi.m_strProcessor),
				m_strProcessorDir(mi.m_strProcessorDir),
				m_uAutoReset(mi.m_uAutoReset),
				m_strDataSet(mi.m_strDataSet)
			{};
			bool m_bMatchTarget;				//	����ƥ��Ŀ�꣬�Ƿ�ƥ���µ�Ŀ��
			xmEDataMatchTarget m_eMatchTarget;	//	����ƥ��Ŀ�꣬ÿ��ƥ�����
			xmString m_strMatchName;	//	����ϵͳ�е����ƣ����Ϊ�գ��򱣳ֲ���
			xmString m_strInitString;	//	ģ�ͻ����еĳ�ʼֵ�����Ϊ�գ��򱣳ֲ���
			xmString m_strProcessor;	//	Ԥ�����������Ϊ�գ��򱣳ֲ���
			xmString m_strProcessorDir;	//	Ԥ������·�������Ϊ�գ��򱣳ֲ���
			unsigned int m_uAutoReset;	//	����ע�ᵽϵͳ��ʱ���Ƿ�����Ϊ�Զ���λ��0��ʾ����Ҫ�Զ���λ�������ʾ�Զ���λ�Ľ����������Ϊ0xFFFFFFFF���򱣳ֲ���
			xmString m_strDataSet;		//	����ע�ᵽϵͳ��ʱ��ʹ��ʲô���ݼ������Ϊ�գ��򱣳ֲ���
		}SMatchInfo;

		static xmRuntimeData* CreateRuntimeData(xmPublicResource* pPubRes, xmMrtc* pMrtc, xmEIOType ioType, const xmModelData* pModelData);

		//	����Buffer���ӣ�������ʹ�õ��ڴ���MRTC�еĻ�����������
		void SetLinkBuffer(void* pBuffer)
		{
			m_CacheValue.Link(pBuffer, true);
		}

		//	���ݶ���ģ�͵�����
		const xmString& DataName() const;
		//	��ȡ������ģ������XML�ļ��е�һ������
		const char* DataAttr(const char* strAttrName) const;
		//	���ݴ�С
		size_t DataSize() const
		{
			return m_CacheValue.Size();
		}
		//	������ģ�ͻ����е���ֵ������ֵ��ģ��ֱ���ܹ�������
		xmValue DataValue() const
		{
			return m_CacheValue.PropertyValue();
		}
		//	��ȡ��ʼֵ����ʼֵ�Ƕ���ģ�ͻ���ĳ�ʼֵ
		const xmValue& InitValue() const;
		//	���Ի��渴λ��ƥ��ֵ���踴λ
		//	���model.config�ļ���ָ�����µĳ�ʼֵ����ʹ���µĳ�ʼֵ��λ��
		//	����ʹ��ģ�������ļ���ָ���ĳ�ʼֵ��λ
		void DataReset();
		//	��ȡ��ǩ��Ϣ
		size_t TagCount() const;
		const xmString& TagName(size_t uPos) const;
		const unsigned int TaggedIndex(const xmString& strTagName) const;
		bool IsTagged(const xmString& strTagName) const;


		//	ģ������ƥ��������
		const xmString& MatchName() const
		{
			return m_strMatchName;
		}
		xmEDataMatchTarget MatchTarget() const
		{
			return m_eMatchTarget;
		}

		//	ģ�������ڷ���ϵͳ�е�����·����ͨ����·���������ݶ�д
		const xmString& SystemPath() const
		{
			return m_strSystemPath;
		}
		//	ģ�������ڷ���ϵͳ�е���ֵ
		xmValue SystemValue() const
		{
			if (m_SystemValue.IsInvalid())
			{
				xmAssert(m_pProcessor == NULL);
				return m_CacheValue.PropertyValue();
			}
			else
			{
				xmAssert(m_pProcessor != NULL);
				return m_SystemValue.PropertyValue();
			}
		}

		//	��������ƥ�䣬��ֻ��ƥ��һ�Ρ�ƥ�����ݰ������������ơ����͡���ʼֵ��Ԥ������������
		//	�������ݵ�matchName����������·��
		//	������ݵ�matchNameֻ��������
		//	�ڲ�����ֻ��ƥ��pInitValue
		void SetMatchInfo(const SMatchInfo& matchInfo);
		//	�����������·����ֻ�������������
		void SetCombinePath(const xmString& combinePath);

		xmRet DataIO();
		virtual xmEIOType IOType() const = 0;
		xmEDataCheckType CheckType() const;
		//	ԤIO����������IO���������ϵͳ�������ݣ�������Ľ�����������µ�ģ�ͻ����ϵͳ�У����������aValue����
		//	�У���ģ�����������ԤIO�ǽ�����ϵͳ�����ݣ�����Ԥ�����������뵽aValue�У����������ԤIO�ǽ�ģ�ͻ�����
		//	�����ݣ�����Ԥ����������aValue������
		virtual xmRet PreIO(xmValue& aValue) = 0;

		//	�����Ƿ���VDR�Զ���λ��ֻ��ע�ᵽ����ϵͳ�е�������ݲ��ܱ�����Ϊ�Զ���λ
		unsigned int AutoResetClick() const
		{
			return m_uAutoReset;
		}
		//	������ģ���Զ���λ
		unsigned int ModelResetCycle() const;
		//	ָ������ע������ݼ���ֻ��ע�ᵽ����ϵͳ�е�������ݲ��ܱ�ָ�����ݼ�
		const xmString& DataSet() const
		{
			return m_strDataSet;
		}
		//	����QoS
		int DataQoS() const
		{
			return m_nQoS;
		}

		//	��ӡ��Ϣ
		xmString PrintInfo() const;
		const xmString& PrintXml() const;

		//	��ȡ������Ϣ
		xmRet GetLastErrorCode_Processor() const
		{
			return m_nLEC_Processor;
		}
		xmRet GetLastErrorCode_DataIO() const
		{
			return m_nLEC_DataIO;
		}
		const xmString& GetLastErrorMessage_Processor() const
		{
			return m_strLEM_Processor;
		}
		const xmString& GetLastErrorMessage_DataIO() const
		{
			return m_strLEM_DataIO;
		}

	protected:
		xmRuntimeData(xmPublicResource* pPubRes, xmMrtc* pMrtc, const xmModelData* pModelData);
		virtual xmRet _DataIO() = 0;

		xmPublicResource* const m_pPublicRes;
		//	������ģ���е�����
		const xmModelData* const m_pModelData;
		//	�����ڻ����е���ֵ
		xmValue m_CacheValue;
		//	ƥ���Ļ����еĳ�ʼֵ
		xmValue m_InitValue;

		//	������ƥ������������ϵͳ����·���ֱ�洢�����Լ򵥡����ٵ�ʵ�ֶ��·�����
		//	����ƥ��Ŀ��
		xmEDataMatchTarget m_eMatchTarget;
		//	ģ������ƥ�����ƣ�ģ������ƥ�䵽����ϵͳʱ��ƥ������Ĭ����ģ��������ͬ��
		xmString m_strMatchName;
		//	ϵͳ����·��������ƥ�����Ƽ������·��������ϵͳ����·����Ĭ����ģ��������ͬ
		xmString m_strSystemPath;
		//	Ԥ����������Ԥ���������ݣ�Ԥ��������xmPublicResource�����͹������ݽ�����ʹ��
		xmIProcessor* m_pProcessor;
		xmValue m_SystemValue;
		//	����QoS��1��ʾ����������ֵ�ࡢ״̬�ࣩ��2��ʾ����������ָ���ࣩ����ָ��Ϊ0ʱʹ��Ĭ�ϵķ�ʽ����
		//	������չ��������
		int m_nQoS;
		//	ע�ᵽϵͳʱ���Ƿ�ָ���µ����ݼ������Ϊ�գ���ע�ᵽ�ڵ�Ĭ�����ݼ���
		xmString m_strDataSet;
		//	ע�ᵽϵͳʱ���Ƿ������Զ���λ����
		unsigned int m_uAutoReset;

		//	ģ�ͻ����Զ���λ�������������
		unsigned int m_uModelReset;

		//	����ţ�������ű仯ʱ����¼��־�����������ʾ̫��
		//	Last Error Code
		xmRet m_nLEC_Processor;
		xmRet m_nLEC_DataIO;
		//	Last Error Message
		xmString m_strLEM_Processor;
		xmString m_strLEM_DataIO;


		//	��װ�˶�Ԥ�������ͷ���ϵͳ��ʹ�ýӿ�
		//	���ִ�����׳��쳣
		xmRet iProcessor_Model2System(const xmValue& modelValue, xmValue& systemValue);
		xmRet iProcessor_System2Model(const xmValue& systemValue, xmValue& modelValue);
		xmRet iSystem_GetValue(xmValue& dataValue);
		xmRet iSystem_SetValue(const xmValue& dataValue);
		//	�˴������޷�ƥ�䡣��Ϊ�е���������ֱ��ʹ�õ��࣬�����滻�������������ʹ�õ��࣬���޷��滻��
		//	���ƥ�䣬ֻ��ͨ��Ԥ������
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_RTDATA_H

