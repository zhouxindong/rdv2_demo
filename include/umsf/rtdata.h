#ifndef __SSA_UMSF_RTDATA_H
#define __SSA_UMSF_RTDATA_H
//#include "processor.h"
//#include "isystem.h"
#include "udefine.h"
#include "publicres.h"
namespace ssa
{
	//	定义运行时的模型数据，运行时数据可能要与仿真系统或节点数据进行匹配
	//	运行时数据的IO只能以数据为单位，不能深入到属性。
	class xmModelData;
	class xmRuntimeData
	{
	public:
		typedef struct _tagMatchInfo
		{
			_tagMatchInfo() : m_eMatchTarget(DMT_NULL), m_strMatchName(NULL), m_strInitString(NULL), m_pProcessor(NULL), m_uAutoReset(0) {};
			xmEDataMatchTarget m_eMatchTarget;	//	数据匹配目标
			xmString m_strMatchName;	//	仿真系统中的名称
			xmString m_strInitString;	//	模型缓存中的初始值
			xmIProcessor* m_pProcessor;	//	预处理器
			unsigned int m_uAutoReset;	//	数据注册到系统中时，是否设置为自动复位，0表示不需要自动复位，否则表示自动复位的节拍数
		}SMatchInfo;

		static xmRuntimeData* CreateRuntimeData(xmEIOType ioType, const xmModelData* pModelData);
		virtual ~xmRuntimeData() 
		{
		};

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
			if (m_pSystemValue.IsNoNULL())
			{
				xmAssert(m_pProcessor != NULL);
				return m_pSystemValue->PropertyValue();
			}
			else
			{
				xmAssert(m_pProcessor == NULL);
				return m_CacheValue.PropertyValue();
			}
		}

		//	设置数据匹配，且只能匹配一次。匹配内容包括：数据名称、类型、初始值、预处理器与描述
		//	输入数据的matchName可以是数据路径
		//	输出数据的matchName只能是数据
		//	内部数据只能匹配pInitValue
		void SetMatchInfo(const SMatchInfo& matchInfo);
		//	设置数据组合路径，只能用于输出数据
		void SetCombinePath(const xmString& combinePath);

		void DataIO(xmPublicResource* pPubRes)
		{
			if (m_pSystemValue.IsNoNULL())
			{
				DataIO(pPubRes, *m_pSystemValue, m_CacheValue);
			}
			else
			{
				DataIO(pPubRes, xmValue(), m_CacheValue);
			}
		}
		virtual void DataIO(xmPublicResource* pPubRes, xmValue& systemValue, xmValue& cacheValue) = 0;
		virtual xmEIOType IOType() const = 0;
		//	数据是否自动复位，只有注册到仿真系统中的输出数据才能被设置为自动复位
		virtual unsigned int AutoResetClick() const = 0;

		xmString PrintInfo() const;
		const xmString& PrintXml() const;
	protected:
		xmRuntimeData(const xmModelData* pModelData);


		//	数据在模型中的描述
		const xmModelData* const m_pModelData;
		//	数据在缓存中的数值
		xmValue m_CacheValue;
		//	匹配后的缓存中的初始值
		xmPtr<xmValue> m_pInitValue;

		//	将数据匹配名称与数据系统访问路径分别存储，可以简单、快速的实现多次路径组合
		//	数据匹配目标
		xmEDataMatchTarget m_eMatchTarget;
		//	模型数据匹配名称，数据匹配名称加上组合路径，构成系统访问路径
		xmString m_strMatchName;
		//	模型数据匹配到仿真系统时的匹配名，默认与模型数据相同
		xmString m_strSystemPath;
		//	预处理器，及预处理后的数据
		xmIProcessor* m_pProcessor;
		xmPtr<xmValue> m_pSystemValue;

		//	注册到系统时，是否设置自动复位属性
		unsigned int m_uAutoReset;
		//	模型缓存自动复位的输出次数计数
		unsigned int m_uModelReset;

		//	封装了对预处理器和仿真系统的使用接口
		//	出现错误后：抛出异常
		void iProcessor_Model2System(const xmValue& modelValue, xmValue& systemValue);
		void iProcessor_System2Model(const xmValue& systemValue, xmValue& modelValue);
		void iSystem_GetValue(xmPublicResource* pPubRes, xmValue& dataValue);
		void iSystem_SetValue(xmPublicResource* pPubRes, const xmValue& dataValue);
		//	此处，类无法匹配。因为有的类是数据直接使用的类，可以替换，如果在属性中使用的类，则无法替换。
		//	类的匹配，只能通过预处理器
	};
}   //  namespace ssa
#endif  //  __SSA_UMSF_RTDATA_H

