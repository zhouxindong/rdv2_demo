#ifndef __SSA_UMSF_RTDATA_H
#define __SSA_UMSF_RTDATA_H
#include "rtobject.h"
#include "publicres.h"
namespace ssa
{
	//	定义运行时的模型数据，运行时数据可能要与仿真系统或节点数据进行匹配
	//	运行时数据的IO只能以数据为单位，不能深入到属性。
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
			bool m_bMatchTarget;				//	数据匹配目标，是否匹配新的目标
			xmEDataMatchTarget m_eMatchTarget;	//	数据匹配目标，每次匹配更新
			xmString m_strMatchName;	//	仿真系统中的名称，如果为空，则保持不变
			xmString m_strInitString;	//	模型缓存中的初始值，如果为空，则保持不变
			xmString m_strProcessor;	//	预处理器，如果为空，则保持不变
			xmString m_strProcessorDir;	//	预处理器路径，如果为空，则保持不变
			unsigned int m_uAutoReset;	//	数据注册到系统中时，是否设置为自动复位，0表示不需要自动复位，否则表示自动复位的节拍数，如果为0xFFFFFFFF，则保持不变
			xmString m_strDataSet;		//	数据注册到系统中时，使用什么数据集，如果为空，则保持不变
		}SMatchInfo;

		static xmRuntimeData* CreateRuntimeData(xmPublicResource* pPubRes, xmMrtc* pMrtc, xmEIOType ioType, const xmModelData* pModelData);

		//	设置Buffer链接，将数据使用的内存与MRTC中的缓存链接起来
		void SetLinkBuffer(void* pBuffer)
		{
			m_CacheValue.Link(pBuffer, true);
		}

		//	数据对于模型的名称
		const xmString& DataName() const;
		//	获取数据在模型配置XML文件中的一个属性
		const char* DataAttr(const char* strAttrName) const;
		//	数据大小
		size_t DataSize() const
		{
			return m_CacheValue.Size();
		}
		//	数据在模型缓存中的数值，该数值是模型直接能够看到的
		xmValue DataValue() const
		{
			return m_CacheValue.PropertyValue();
		}
		//	获取初始值，初始值是对于模型缓存的初始值
		const xmValue& InitValue() const;
		//	仅对缓存复位，匹配值无需复位
		//	如果model.config文件中指定了新的初始值，则使用新的初始值复位，
		//	否则使用模型描述文件中指定的初始值复位
		void DataReset();
		//	获取标签信息
		size_t TagCount() const;
		const xmString& TagName(size_t uPos) const;
		const unsigned int TaggedIndex(const xmString& strTagName) const;
		bool IsTagged(const xmString& strTagName) const;


		//	模型数据匹配后的名称
		const xmString& MatchName() const
		{
			return m_strMatchName;
		}
		xmEDataMatchTarget MatchTarget() const
		{
			return m_eMatchTarget;
		}

		//	模型数据在仿真系统中的数据路径，通过该路径进行数据读写
		const xmString& SystemPath() const
		{
			return m_strSystemPath;
		}
		//	模型数据在仿真系统中的数值
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

		//	设置数据匹配，且只能匹配一次。匹配内容包括：数据名称、类型、初始值、预处理器与描述
		//	输入数据的matchName可以是数据路径
		//	输出数据的matchName只能是数据
		//	内部数据只能匹配pInitValue
		void SetMatchInfo(const SMatchInfo& matchInfo);
		//	设置数据组合路径，只能用于输出数据
		void SetCombinePath(const xmString& combinePath);

		xmRet DataIO();
		virtual xmEIOType IOType() const = 0;
		xmEDataCheckType CheckType() const;
		//	预IO，按照数据IO规则，与仿真系统交换数据，交换后的结果，并不更新到模型缓存或系统中，而是输出到aValue参数
		//	中，如模型输入参数，预IO是将仿真系统的数据，经过预处理器后，输入到aValue中，输出参数，预IO是将模型缓存中
		//	的数据，经过预处理后，输出到aValue参数中
		virtual xmRet PreIO(xmValue& aValue) = 0;

		//	数据是否由VDR自动复位，只有注册到仿真系统中的输出数据才能被设置为自动复位
		unsigned int AutoResetClick() const
		{
			return m_uAutoReset;
		}
		//	数据由模型自动复位
		unsigned int ModelResetCycle() const;
		//	指定数据注册的数据集，只有注册到仿真系统中的输出数据才能被指定数据集
		const xmString& DataSet() const
		{
			return m_strDataSet;
		}
		//	数据QoS
		int DataQoS() const
		{
			return m_nQoS;
		}

		//	打印信息
		xmString PrintInfo() const;
		const xmString& PrintXml() const;

		//	获取错误信息
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
		//	数据在模型中的描述
		const xmModelData* const m_pModelData;
		//	数据在缓存中的数值
		xmValue m_CacheValue;
		//	匹配后的缓存中的初始值
		xmValue m_InitValue;

		//	将数据匹配名称与数据系统访问路径分别存储，可以简单、快速的实现多次路径组合
		//	数据匹配目标
		xmEDataMatchTarget m_eMatchTarget;
		//	模型数据匹配名称，模型数据匹配到仿真系统时的匹配名，默认与模型数据相同。
		xmString m_strMatchName;
		//	系统访问路径，数据匹配名称加上组合路径，构成系统访问路径，默认与模型数据相同
		xmString m_strSystemPath;
		//	预处理器，及预处理后的数据，预处理器由xmPublicResource创建和管理，数据仅负责使用
		xmIProcessor* m_pProcessor;
		xmValue m_SystemValue;
		//	数据QoS，1表示允许丢包（数值类、状态类），2表示不允许丢包（指令类），当指定为0时使用默认的方式处理
		//	允许扩展其他类型
		int m_nQoS;
		//	注册到系统时，是否指定新的数据集，如果为空，则注册到节点默认数据集中
		xmString m_strDataSet;
		//	注册到系统时，是否设置自动复位属性
		unsigned int m_uAutoReset;

		//	模型缓存自动复位的输出次数计数
		unsigned int m_uModelReset;

		//	错误号，当错误号变化时，记录日志，避免错误提示太多
		//	Last Error Code
		xmRet m_nLEC_Processor;
		xmRet m_nLEC_DataIO;
		//	Last Error Message
		xmString m_strLEM_Processor;
		xmString m_strLEM_DataIO;


		//	封装了对预处理器和仿真系统的使用接口
		//	出现错误后：抛出异常
		xmRet iProcessor_Model2System(const xmValue& modelValue, xmValue& systemValue);
		xmRet iProcessor_System2Model(const xmValue& systemValue, xmValue& modelValue);
		xmRet iSystem_GetValue(xmValue& dataValue);
		xmRet iSystem_SetValue(const xmValue& dataValue);
		//	此处，类无法匹配。因为有的类是数据直接使用的类，可以替换，如果在属性中使用的类，则无法替换。
		//	类的匹配，只能通过预处理器
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_RTDATA_H

