/******************************************************************************/
/** @file value.h
@brief
	定义xmValue类。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	value.cpp
@see ssa::xmValue
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_VALUE_H
#define __SSA_BASE_VALUE_H
#include "rtti.h"
#include "exceptions.h"
#include "assertmsg.h"
#include "class.h"
#include "stringutil.h"
#include "ptr.h"
#include "omap.h"

namespace ssa
{
	class xmBuffer;
	class xmStaticBuffer;
	/******************************************************************************/
	/** @class xmValue
	@brief
		xmValue为仿真系统中不同类型的数据提供统一的访问接口。
	@details
		xmValue代表某一个类型确定的数据，因此，必须经过显式的初始化，明确数据类型才能使用，
		未经初始化的实例无法使用。初始化之后，类型（包括长度）不允许在计算过程中改变，在使
		用时，也必须显式的指明类型。\n
		使用xmValue可以对不同类型的数据进行统一的管理和使用。每个xmValue由记录内容的Buffer
		和记录结构的Class两部分构成，这两部分可能由xmValue自身创建，也可能指向已经创建好的
		外部对象。Class是否由自己创建，在构造函数时即已经确定，使用过程中无法改变；Buffer在
		默认情况下是由自己创建，但可以通过 Link() 或 Unlink() 函数改变，也可以通过拷贝构造
		函数继承，具体情况参见函数说明。
	@par 多线程安全
		否
	@note
		对于已经初始化的 xmValue 对象，不提供 operator=() 的操作，因为 operator=() 无法知
		道参与计算的双方的数据类型。但是对于未初始化对象，可以通过 operator=() 操作进行初始化。
	@remarks 
		xmValue 使用 xmClass 描述类型。
	@see xmClass
	@par 修改记录
		@par
		2019.12.15
			-# 将Unstructured()的输出，由std::map改为了xmOrderMap。
		2019.02.14
			-# 对xmValue(xmEDataType, size_t, const char*)构造函数、Initialize(xmEDataType, size_t)初始化函数
			进行了扩展，允许xmEDataType参数允许通过使用xmDT_STRING构造xmDT_BUFFER实例。
		2019.01.10
			-# 增加了以xmStringVector为输入的属性值获取方法。
		2019.01.08
			-# 调整了AssignValue的函数，对两个xmValue实例的类型进行了检查，增加了Assign函数，可以用任意类型进行赋值。
		2018.12.27
			-# 使用按数据类型取值的i32()这类接口时，如果类型不匹配，由产生断言失败调整为抛出xmExIllegalCall异常。
		2018.12.22
			-# 增加了string()接口，便于存储字符串的xmValue实例（为xmDT_BUFFER类型，最后一个字节为0）作为字符串使用。
		2018.11.30
			-# 增加了使用字符串构造xmValue实例的方法，此方法构造出来的xmValue实例是xmDT_BUFFER类型。
			-# 增加了使用xmBuffer对象构造xmValue实例的方法，此方法构造出来的xmValue实例是xmDT_BUFFER类型。
		2018.06.30 
			-# 删除了xmValue& LoadValue(std::istream& iValueInfo)接口。
			-# 将Unstructured()在头文件中实现，以便使用STL中的map容器。
		2018.06.19
			-# 增加了去结构化的接口Unstructured()。
		2018.05.10
			-# 改变了Initialize(const xmValue& aValue)函数的行为，将原来的Buffer归属由aValue
			决定，改为Buffer属于自己。修改后，所有通过xmValue构造函数创建的以及通过Initialize()
			系列函数初始化的xmValue对象，其Buffer都是由自己管理和使用。
			-# 开放了IsMyBuffer()和IsMyClass()两个接口（原来为protected）类型，用于归属判断。
		@par
		2018.04.09
			-# 将PrintJson接口，改名为PrintValue接口。
		@par
		2018.01.04 
			-# 添加了AssignValue、IsValid函数，用于两个类型完全一致的xmValue对象赋值和判断有效。
		@par
		2017.11.1
			-# 去掉了使用JSON直接构造的接口，合并了基本数据类型和Buffer类型构造函数。
		@par
		2017.09.28 重大更改
			-# 按照下述原则重新设计了接口：xmValue 必须具有一个类型，该类型可以是基本
			数据类型、Buffer或者一个 xmClass。xmValue 的类型不允许改变，只能改变数值。
		@par
		2015.05.13 重大更改
			-# 去掉了与 xmClass 之间的相互引用；
			-# 重新定义了属性，将对数据结构的描述彻底放到 xmClass 中，大大精简了代码，
			提升了复制 xmValue 的速度。
		@par
		2015.05.11
			-# 增加了m_eType属性，和Type、Set、Get接口。
			-# 增加了Print接口。
		@par
		2015.05.08
			-# 将TypeSize()接口转移到define.h文件中，变为SSA空间全局函数。
		@par
		2015.04.22
			-# 增加了TypeSize()静态接口
		@par
		2015.04.11
			-# 提取出了xmClass类，将对数据的映射内容与数据分开。并重新设计了Getter和
			Setter接口、operator[size_t]接口
		@par
		2015.03.22
			-# 明确了 xmValue 的定位，重新设计了对外接口。
		@par
		2014.03.24 
			-# 代码初次完成。
	*******************************************************************************/

	class xmBASE_EXPORT xmValue
	{
		friend xmIsTypeT<xmValue>;
		friend xmBuffer;
	public:
		/******************************************************************************/
		/** @name 构造函数与析构函数
		@{
		*/

		/** 创建一个全空的对象
		@note 
			空对象必须通过 Initialize() 函数初始化后，才能使用。
		*/
		xmValue() : m_pBuffer(NULL), m_pClass(NULL), m_uAttribute(MY_NOTHING)
		{
		}

		/** 使用基本数据类型或者String、Buffer构造
		@param [in] eType
			xmValue的类型，取值为基本数据类型、 xmDT_STRING 或 xmDT_BUFFER 。
			使用xmDT_STRING时，实际构造的xmValue仍为xmDT_BUFFER类型，仅对strInitValue
			参数的理解不同。
		@param [in] uBufferSize
			当eType为基本数据类型时时，该参数被忽略；当eType为 xmDT_STRING 或 xmDT_BUFFER 时，
			表示Buffer的大小，此时不能为0。
		@param [in] strInitValue
			用字符串表示的初始值。当eType为xmDT_STRING时，该参数为具体字符串。
		@exception xmExIllegalInput
			输入错误。
		@note
			此方法构造的基本数据类型实例时，Buffer是自己的，Class不是自己的。\n
			此方法构造的Buffer类型实例时，Buffer是自己的，Class也是自己的。
		@par 示例
			@par
			@code
				xmValue aValue(xmDT_INT32);		
				xmValue aValue(xmDT_BUFFER, 16);
			@endcode
		*/
		xmValue(xmEDataType eType, size_t uBufferSize = 0, const char* strInitValue = NULL);

		/** 使用字符串构造一个Buffer类型的xmValue
		@param [in] strInitValue
			字符串指针
		@note
			此方法构造的实例，Buffer长度为字符串长度+1。
			此方法构造的实例，Buffer是自己的，Class不是自己的。
			此方法构造的实例，数据类型是xmDT_BUFFER。
		*/
		xmValue(const char* strInitValue);

		/** 使用xmBuffer构造一个Buffer类型的xmValue
		@param [in] strInitValue
			xmBuffer对象
		@note
			此方法构造的实例，Buffer是自己的，Class不是自己的。
			此方法构造的实例，数据类型是xmDT_BUFFER。
		*/
		xmValue(const xmBuffer& strInitValue);

		/** 使用数据类直接构造
		@param [in] aClass
			xmValue的类型。
		@param [in] bCopyClass
			指定是否由构造出来的xmValue实例独立拥有该Class实例。
		@param [in] strInitValue
			用字符串表示的初始值。
		@note
			此方法构造的实例，Buffer是自己的，Class是否自己创建由bCopyClass参数决定。
		@attention
			在使用此构造函数时，如果传递一个xmClass的临时变量，必须将bCopyClass参数设置
			为true，否则当临时变量销毁时，xmValue将使用一个不存在的xmClass描述自身类型。
		@par 示例
			@par
			@code
				xmClass aClass(xmDT_INT32);
				xmValue aValue1(aClass);		//	aClass不能在aValue1之前销毁，否则使用aValue时会出错
				xmValue aValue2(aClass, true);	//	aClass与aValue2无关，可以随时销毁，aValue2已经复制了
												//	一个aClass
			@endcode
		*/
		explicit xmValue(const xmClass& aClass, bool bCopyClass = false, const char* strInitValue = NULL) : m_pBuffer(NULL), m_pClass(NULL), m_uAttribute(MY_NOTHING)
		{
			Initialize(aClass, bCopyClass);
			if (!xmString(strInitValue).IsEmpty())
			{
				LoadValue(strInitValue);
			}
		}

		/** 拷贝构造
		@param [in] aValue
			被复制对象。
		@note
			此方法构造一个内容与属性均与aValue一致新的xmValue实例。此方法构造的实例，Buffer是自己的，
			Class是否属于自己由aValue决定，如果不属于自己，则新的实例与aValue共同指向相同的外部Class。
		@attention
			下面两个函数的返回值处理是不一致的：
				xmValue Func1()
				{
					//	... ...
					return xmValue(xmDT_INT32);
				}
				xmValue Func2()
				{
					xmValue temp(xmDT_INT32)
					//	... ...
					return temp;
				}
			对于下面这种调用：
				xmValue  v1 = Func1();
				xmValue  v2(Func1());
				xmValue* v3 = new xmValue(Func1());
			v1、v2、v3三个变量的创建，不会调用拷贝构造函数。
			但如果将Func1函数换为Func2函数，如：
				xmValue  v1 = Func2();
				xmValue  v2(Func2());
				xmValue* v3 = new xmValue(Func2());
			则会调用v1、v2、v3的拷贝构造函数。
		*/
		xmValue(const xmValue& aValue) : m_pBuffer(NULL), m_pClass(NULL), m_uAttribute(MY_NOTHING)
		{
			Initialize(aValue);
		}

		/** 析构函数
		@note
			xmValue析构时，会销毁由自己创建的Buffer和Class。
		*/
		virtual ~xmValue(void)
		{
			__Clear();
		};



		/** 使用基本数据类型或者Buffer初始化
		@param [in] eType
			xmValue的类型，取值为基本数据类型或 xmDT_STRING 或 xmDT_BUFFER 。
		@param [in] uBufferSize
			当eType为基本数据类型时时，该参数被忽略；当eType为 xmDT_BUFFER 时，
			表示Buffer的大小，此时不能为0。当eType为 xmDT_STRING 时，认为与 xmDT_BUFFER
			相同处理。
		@exception xmExIllegalCall
			对已经设定类型的 xmValue 调用 Initialize 函数时抛出。
		@exception xmExIllegalInput
			输入错误时抛出。
		@note
			所有的初始化函数只能在空的xmValue对象中调用。\n
			此方法构造的基本数据类型实例时，Buffer是自己的，Class不是自己的。\n
			此方法构造的Buffer类型实例时，Buffer是自己的，Class也是自己的。
		@par 示例
			@par
			@code
				xmValue aValue;
				aValue.Initialize(xmDT_INT32);		
			@endcode
		*/
		xmValue& Initialize(xmEDataType eType, size_t uBufferSize = 0);

		/** 使用数据类初始化
		@param [in] aClass
			xmValue的类型。
		@param [in] bCopyClass
			指定是否由构造出来的xmValue实例独立拥有该Class实例。
		@exception xmExIllegalCall
			对已经设定类型的 xmValue 调用 Initialize 函数时抛出。
		@note
			此方法构造的实例，Buffer是自己的，Class是否自己创建由bCopyClass参数决定。
		@attention
			在使用此构造函数时，如果传递一个xmClass的临时变量，必须将bCopyClass参数设置
			为true，否则当临时变量销毁时，xmValue将使用一个不存在的xmClass描述自身类型。
		@par 示例
			@par
			@code
				xmClass aClass(xmDT_INT32);
				xmValue aValue1(aClass);		//	aClass不能在aValue1之前销毁，否则使用aValue时会出错
				xmValue aValue2(aClass, true);	//	aClass与aValue2无关，可以随时销毁，aValue2已经复制了
												//	一个aClass
			@endcode
		*/
		xmValue& Initialize(const xmClass& aClass, bool bCopyClass = false);

		/** 使用xmValue对象初始化
		@param [in] aValue
			被复制对象。
		@exception xmExIllegalCall
			对已经设定类型的 xmValue 调用 Initialize 函数时抛出。
		@note
			此方法构造一个内容与属性均与aValue一致新的xmValue实例。此方法构造的实例，Buffer是自己的，
			Classr是否属于自己由aValue决定，如果不属于自己，则新的实例与aValue共同指向相同的外部Class。
		*/
		xmValue& Initialize(const xmValue& aValue);


	private:
		/** 使用xmValue对象初始化
		@param [in] aValue
			被复制对象。
		@see Initialize(const xmValue&)
		*/
		inline xmValue& operator=(const xmValue& aValue)
		{
			return Initialize(aValue);
		}

		/**	@} ************************************************************************/




	public:
		/******************************************************************************/
		/** @name 链接类函数
		@attention
			下列几个函数，都将改变已经创建的xmValue对象对Buffer和Class的指向，
		@{
		*/

		/** 使用xmValue操作一个已经存在的外部内存空间
		@param [in] pBuffer
			被操作的外部内存空间。
		@param [in] bKeepValue
			是否保存原有数值。
		@return
			实例自身引用。
		@note
			调用成功后，其原来的空间将被释放或解除链接，无法继续使用。
		@attention
			pBuffer的长度不得小于实例自身类型所设定的长度。
		*/
		xmValue& Link(void* pBuffer, bool bKeepValue = false);

		/** 使用xmValue操作一个已经存在的变量
		@param [in] aParam
			被操作的变量实例。
		@param [in] bKeepValue
			是否保存原有数值。
		@return
			实例自身引用。
		@exception xmExIllegalInput
			当aParam的长度小于实例自身类型所设定的长度时抛出。
		*/
		//template <typename T>
		//xmValue& Link(T& aParam, bool bKeepValue = false)
		//{
		//	if (Size() > xmIsTypeT<T>::Size(aParam))
		//	{
		//		throw xmExIllegalInput();
		//	}
		//	return Link(xmIsTypeT<T>::Buffer(aParam), bKeepValue);
		//}

		/** 解除对外部空间的链接
		@param [in] bKeepValue
			是否保存原有数值。
		@return
			实例自身引用。
		@note
			解除链接后，将创建一个新的自己拥有的内存空间，长度由实例类型决定。
		*/
		xmValue& Unlink(bool bKeepValue = false);

		/** 判断是否链接外部内存空间
		@return 
			是返回true
		*/
		inline bool IsLinked()
		{
			return !IsMyBuffer();
		}
		
		/** 将数据的类型改变为与原类型“相似”的新类型
		@exception xmExIllegalInput
			当新类型与原类型不相似时抛出。
		@see xmClass::IsSimilar()
		*/
		xmValue& SwitchClass(const xmClass& newClass, bool bCopyClass = false);

		/**	@} ************************************************************************/





		/******************************************************************************/
		/** @name 基本操作函数
		@{
		*/

		/** 判断xmValue是否无效
		@return 无效返回true。
		*/
		inline bool IsInvalid() const
		{
			return (m_pBuffer == NULL);
		}
		/** 判断xmValue是否有效
		@return 有效返回true。
		*/
		inline bool IsValid() const
		{
			return (m_pBuffer != NULL);
		}
		/** 判断两个xmValue是否内存相等
		@return 相等返回true。
		*/
		inline bool IsMemoryEqual(const xmValue& aValue) const
		{
			if (Size() != aValue.Size())
			{
				return false;
			}
			if (IsInvalid())
			{
				return true;
			}
			return (memcmp(m_pBuffer, aValue.m_pBuffer, Size()) == 0);
		}
		/** 判断两个数据是否相等。
		@param [in] aValue
			另一个数据
		@return 相等返回true。
		@note
			两个数据相等是指其Class相等，并且Buffer内的数值相同。如：
			@par
			@code
				xmValue v1(xmDT_INT32);
				v1.i32() = 10;
				xmValue v2(xmDT_UINT32);
				v2.u32() = 10;
				xmValue v3(xmDT_INT32);
				v3.i32() = 5;

				v1 == v2;	//	false，类型不同
				v1 == v3;	//	false，数值不同
			@endcode

			如果希望仅比较数值，应先通过GeterSeter接口取值后比较。
		*/
		inline bool IsEqual(const xmValue& aValue) const
		{
			if (Class() != aValue.Class())
			{
				return false;
			}
			return (memcmp(m_pBuffer, aValue.m_pBuffer, Size()) == 0);
		}

		/** 获取记录结构的数据描述类
		@return 数据类指针。
		*/
		inline const xmClass* Class() const
		{
			return m_pClass;
		}
		/** 获取数据类型
		@return 数据类型。
		*/
		inline xmEDataType Type() const
		{
			return (m_pClass==NULL) ? xmDT_UNKNOWN : m_pClass->Type();
		}
		/** 获取由类型所设定的内存长度
		@return 内存长度。
		*/
		inline size_t Size() const
		{
			return (m_pClass == NULL) ? 0 : m_pClass->Size();
		}
		/** 将实例的内存空间通过xmStaticBuffer提供的接口进行使用，此函数可适用于任意类型的数据。
		@return 一个xmStaticBuffer实例。
		@see xmStaticBuffer
		*/
		xmStaticBuffer asBuffer() const;
		/** 将实例的内存空间通过xmStaticBuffer提供的接口进行使用，此函数可适用于任意类型的数据。
		@return 一个xmStaticBuffer实例。
		@see xmStaticBuffer
		*/
		xmStaticBuffer asBuffer();


		/** 将xmValue对象去结构化
		@param [out] mapValue
			将结构化的xmValue对象，生成一组基本数据类型的对象。结果输出到一个map表中。
		@return
			无。
		@note
			对map表中的对象进行操作，与直接操作xmValue对象效果一致。如果xmValue对象是基本数据类型，
			则输出的map表为空。
		@par 示例
			@par
			@code
				//-------------------------------------------------
				//	数据类描述文件名称：data.json
				//	数据类描述文件内容：
				//	{
				//		"p1":"INT8",
				//		"p2":"INT32",
				//		"p3":{
				//			"sp1":"FLOAT",
				//			"sp2":"DOUBLE"
				//		},
				//		"p4":"UINT32",
				//		"p5":9
				//	}
				//-------------------------------------------------
				std::ifstream ifJson("data.json");
				xmClass c1(ifJson)
				ifJson.close();
				xmValue v1(c1);

				xmOrderMap<xmString, xmPtr<xmValue>> mapValue;
				v1.Unstructured(mapValue);
				for(size_t i = 0; i < mapValue.Size(); i++)
				{
					printf("%s = %s\n", mapValue.IndexKey(i).String(), mapValue.IndexValue(i)->PrintValue().String());
				}

				//=================================================
				打印输出结果
				p1 = 0
				p2 = 0
				p3.sp1 = 0.000000
				p3.sp2 = 0.000000
				p4 = 0
				p5 =   0   0   0   0   0   0   0   0   0
			@endcode
		*/
		void Unstructured(xmOrderMap<xmString, xmPtr<xmValue>>& mapValue)
		{
			mapValue.Clear();
			if (Type() != xmDT_STRUCT)
			{
				return;
			}

			for (int i = 0; i < PropertyCount(); i++)
			{
				xmPtr<xmValue> pValue = new xmValue(PropertyValue(i));
				xmAssert(pValue->IsValid());

				if (pValue->Type() == xmDT_STRUCT)
				{
					xmOrderMap<xmString, xmPtr<xmValue>> mapProptyValue;
					pValue->Unstructured(mapProptyValue);

					xmString strProptyName = xmStringUtil::Format("%s.", PropertyName(i));
					for (size_t j = 0; j < mapProptyValue.Size(); j++)
					{
						mapValue.Add(strProptyName + mapProptyValue.IndexKey(j), mapProptyValue.IndexValue(j));
					}
				}
				else
				{
					mapValue.Add(PropertyName(i), pValue);
				}
			}
		}

		/** 如果是结构化数据，则以JSON格式打印数据，可以直接用于 LoadValue() 函数设置同类型
		xmValue对象。否则直接打印内容。
		@par 示例
			@par
			@code
				//-------------------------------------------------
				//	数据类描述文件名称：data.json
				//	数据类描述文件内容：
				//	{
				//		"p1":"INT8",
				//		"p2":"INT32",
				//		"p3":{
				//			"sp1":"FLOAT",
				//			"sp2":"DOUBLE"
				//		},
				//		"p4":"UINT32",
				//		"p5":9
				//	}
				//-------------------------------------------------
				std::ifstream ifJson("data.json");
				xmClass c1(ifJson)
				ifJson.close();
				printf("%s\n", c1.PrintInfo());

				xmClass c2(xmDT_INT32);
				printf("%s\n", c2.PrintInfo());

				xmClass c3(xmDT_BUFFER, 8);
				printf("%s\n", c3.PrintInfo());


				xmValue v1(c1);
				printf("%s\n", v1.PrintInfo().String());
				xmValue v2(c2);
				printf("%s\n", v2.PrintInfo().String());
				xmValue v3(c3);
				printf("%s\n", v3.PrintInfo().String());

				//=================================================
				c1 打印输出结果
					CLASS(30)				//	类型为CLASS，总长度为30
						p1:INT8 = 0			//	属性名:类型 = 存储偏移位置
						p2:INT32 = 1		//	类型为INT32，存储在偏移为1的位置
						p3:CLASS(12) = 5	//	类型为CLASS，总长度为12，存储在偏移为5的位置
							sp1:FLOAT = 5
							sp2:DOUBLE = 9
						p4:UINT32 = 17
						p5:BUFFER(9) = 21	//	类型为BUFFER，总长度为9，存储在偏移为21的位置
					END
				c2 打印输出结果
					INT32
				c3 打印输出结果
					BUFFER(8)
					
				v1 打印输出结果
					CLASS(30)				//	类型为CLASS，总长度为30
						p1:INT8 = 0			//	属性名:类型 = 数值
						p2:INT32 = 0
						p3:CLASS(12)
							sp1:FLOAT = 0.000000
							sp2:DOUBLE = 0.000000
						p4:UINT32 = 0
						p5:BUFFER(9) = 00 00 00 00 00 00 00 00 00
					END
				v2 打印输出结果
					INT32 = 0
				v3 打印输出结果
					BUFFER(8) = 00 00 00 00 00 00 00 00
			@endcode
		*/
		xmString PrintValue(void) const;

		/** 以便于阅读的方式打印数据，可以方便开发、调试人员查看
		@see PrintValue()
		*/
		xmString PrintInfo(void) const;

		/**	@} ************************************************************************/





		/******************************************************************************/
		/** @name 相等判断函数
		@{
		*/

		/** 判断两个数据是否相等。
		@param [in] aValue
			另一个数据
		@return 相等返回true。
		@see
			IsEqual() 函数
		*/
		inline bool operator==(const xmValue& aValue) const
		{
			return IsEqual(aValue);
		}
		/** 判断两个数据是否不相等。
		@param [in] aValue
			另一个数据
		@return 不相等返回true。
		@see
			operator==() 函数
		*/
		inline bool operator!=(const xmValue& aValue) const
		{
			return !IsEqual(aValue);
		}

		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 非结构化数据访问函数
		@{
		*/

		/** 基本类型访问函数
		@return 作为基本类型的引用。
		@note
			作为基本数据类型时，xmValue提供了一组函数用于数值读写，包括 i8() 、 u8() 、 i16() 、
			u16() 、 i32() 、 u32() 、 f32() 、 f64() ，分别适用于不同类型的基本数据。这些
			函数由一个字母+数字构成，其中i开头的函数表示有符号整数，u开头表示无符号整数，f开头
			表示浮点数，数字表示位数。如i32表示操作32位有符号整数，f64表示操作双精度浮点数。函数
			返回对应类型的引用，可以直接取值或赋值，如：
			@par
			@code
				xmValue v(xmDT_INT32);
				v.i32() = 10;
				int i = v.i32();	//	i == 10
				short s = v.i32();	//	s == 10	
				
				//	运行异常，v定义的类型与使用的类型不一致
				//	s = v.i16();		
			@endcode
		@attention 上述函数中，数据定义的类型必须和使用的类型相匹配，否则会出现运行时异常。
		*/
#define __DEFINE_GeterSeter(T, Func) \
	inline const T##& Func##() const {if(xmIsTypeT<T>::TYPE != Type()) throw xmExIllegalCall(); return __V<T>();}	\
	inline T##& Func##() {return const_cast<T##&>(static_cast<const xmValue&>(*this).Func##());}		\
	inline T##& Func##(T value) { Func##() = value; return Func##();}		
		__DEFINE_GeterSeter(char, i8);
		__DEFINE_GeterSeter(unsigned char, u8);
		__DEFINE_GeterSeter(short, i16);
		__DEFINE_GeterSeter(unsigned short, u16);
		__DEFINE_GeterSeter(int, i32);
		__DEFINE_GeterSeter(unsigned int, u32);
		__DEFINE_GeterSeter(long long, i64);
		__DEFINE_GeterSeter(unsigned long long, u64);
		__DEFINE_GeterSeter(float, f32);
		__DEFINE_GeterSeter(double, f64);
#undef __DEFINE_GeterSeter


		/** 当表示Buffer类型时，通过此函数返回的xmStaticBuffer对象进行数值操作。
		@return    一个xmStaticBuffer实例。
		@attention 数据定义的类型必须是 xmDT_BUFFER ，否则会出现运行时异常。
		@see       xmStaticBuffer
		*/
		const xmStaticBuffer buffer() const;
		xmStaticBuffer buffer();
		xmStaticBuffer buffer(const xmBuffer& aBuffer);

		/** 当表示String类型时，通过此函数返回的字符串指针。
		@return    一个字符串指针。
		@attention 数据定义的类型必须是 xmDT_STRING ，否则会出现运行时异常。
		*/
		const char* string() const;
		char* string();

		/** 通过字符串，设置数据数值
		@param [in] strValueInfo
			描述数据数值的字符串。
		@return 对象自身引用。
		@exception xmExIllegalInput
			当输入字符串为NULL或长度为0时抛出。
		@exception xmExJsonValue
			当字符串格式错误或类型与定义不一致时抛出，抛出异常后，xmValue对象可能出现一半被
			设置一半未被设置的尴尬情况。
		@note 
			描述数据数值的结构应该与描述类型的结构相同或是其子集。
		@attention 
			用于描述Buffer类型的字符串，必须使用 xmBLANK_STRING 分隔。
		@par 示例
			@par
			@code
				//-------------------------------------------------
				//	数据类描述文件名称：data.json
				//	数据类描述文件内容：
				//	{
				//		"p1":"INT8",
				//		"p2":"INT32",
				//		"p3":{
				//			"sp1":"FLOAT",
				//			"sp2":"DOUBLE"
				//		},
				//		"p4":"UINT32",
				//		"p5":9
				//	}
				//-------------------------------------------------
				std::ifstream ifJson("data.json");
				xmValue aValue(ifJson)
				ifJson.close();

				//-------------------------------------------------
				//	数据值描述文件名称：value.json
				//	数据值描述文件内容：
				//	{
				//		"p1":100,
				//		"p2":200,
				//		"p3":{
				//			"sp1":310,
				//			"sp2":320
				//		},
				//		"p4":400,
				//		"p5":"11 2 013 4 0x15 6 7 8 19"
				//	}
				//	上述数据文件内容也可以为类型结构的一个子集，顺序也可以不一致，如
				//	{
				//		"p5":"11 2 013 4 0x15 6 7 8 19",	//	长度必须为p5在类型中定义的9个
				//		"p2":200,							//	如果写成浮点数，将会被转换为整数
				//		"p3":{
				//			"sp1":310						//	定义的是浮点数，310将被转换为浮点数
				//		}
				//	}
				//	但是每个被设置数值的属性名称必须与类型一致，且不允许出现类型中不
				//	存在的属性名称。
				//-------------------------------------------------
				std::ifstream ifValue("value.json");
				try
				{
					aValue.LoadValue(ifValue);
					printf("%s\n", aValue.Print().String());
				}
				catch (xmExJsonValue& e)
				{
					printf(e.Message());
				}
				ifValue.close();
			@endcode
		*/
		xmValue& LoadValue(const char* strValueInfo);
		/** 通过输入流，设置数据数值
		@param [in] iValueInfo
			输入流
		@see LoadValue(const char*)
		*/
		//xmValue& LoadValue(std::istream& iValueInfo)
		//{
		//	std::string strValueInfo;
		//	std::getline(iValueInfo, strValueInfo, (char)EOF);
		//	return LoadValue(strValueInfo.c_str());
		//}

		/** 通过同类型对象，设置数据数值
		@param [in] aValue
			输入值
		@attention 此函数认为两个xmValue对象的类型相同，直接进行内存拷贝。
		*/
		inline xmValue& AssignValue(const xmValue& aValue)
		{
			if (!Class()->IsEqual(*aValue.Class()))
			{
				throw xmExIllegalInput();
			}
			memcpy(m_pBuffer, aValue.m_pBuffer, aValue.Size());
			return *this;
		}
		/** 设置数据数值
		@param [in] aParam
			输入值
		@attention 直接进行内存拷贝赋值。
		*/
		template<typename T>
		inline xmValue& Assign(const T& aParam)
		{
			if (Size() < xmTypeSize(aParam))
			{
				throw xmExIllegalInput();
			}
			memcpy(m_pBuffer, xmTypeBuffer(aParam), xmTypeSize(aParam));
			return *this;
		}
		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 结构化数据属性访问函数
		@attention 下列所有函数仅在表示结构化数据时可以调用，否则会抛出xmExIllegalCall异常。
		@{
		*/


		/** 获取该数据拥有的属性数量
		@return 属性数量，如果不是结构化数据，则返回0。
		*/
		inline int PropertyCount() const
		{
			return m_pClass->PropertyCount();
		}

		/** 获取该数据的第uPos个属性名称
		@param [in] uPos
			属性序号，由0开始。
		@return 
			属性名称字符串指针。
		@exception xmExIllegalCall
			当该数据不是xmDT_STRUCT时抛出。
		@exception xmExIllegalInput
			当uPos超出属性数量范围时抛出。
		*/
		inline const char* PropertyName(int uPos) const
		{
			return m_pClass->PropertyName(uPos);
		}

		/** 获取该数据的第uPos个属性数值。
		@param [in] uPos
			属性序号，由0开始。
		@return 属性数值，当uPos超出属性数量范围时返回无效数值（ IsInvalid() 为true）
		@exception xmExIllegalCall
			当该数据不是xmDT_STRUCT时抛出。
		@remarks
			该返回值使用实例的内存空间创建，对该返回值的任何操作，都将反映到实例本身。
			必须通过下列方式使用该函数：
				xmValue p1 = aValue.PropertyValue(0);
				xmValue p2(aValue.PropertyValue(1));
				xmValue* p3 = new xmValue(aValue.PropertyValue(2));
			此时，并不会调用p1、p2、p3的xmValue(const xmValue&)构造函数，其Class和
			Buffer均不是自己的，而是指向aValue。
		*/
		inline xmValue PropertyValue(int uPos) const
		{
			const xmClass* pClass = m_pClass->PropertyClass(uPos);
			if (pClass == NULL)
			{
				return xmValue();
			}
			return xmValue(m_pBuffer + (pClass->Offset() - m_pClass->Offset()), *pClass, false);
		}

		/** 获取该数据指定名字的属性数值。
		@param [in] strName
			属性名称，如果为NULL，则返回一个xmValue自身的镜像。
		@return 属性数值，当指定的属性名称不存在时返回无效数值（ IsInvalid() 为true）
		@exception xmExIllegalCall
			当该数据类不是xmDT_STRUCT时抛出。
		@remarks
			该返回值使用实例的内存空间创建，对该返回值的任何操作，都将反映到实例本身。
		*/
		inline xmValue PropertyValue(const char* strPath = NULL) const
		{
			if (xmString(strPath).IsEmpty())
			{
				return xmValue(m_pBuffer, *m_pClass, false);
			}

			const xmClass* pClass = m_pClass->PropertyClass(strPath);
			if (pClass == NULL)
			{
				return xmValue();
			}
			return xmValue(m_pBuffer + (pClass->Offset() - m_pClass->Offset()), *pClass, false);
		}
		//	vPath为属性路径
		//	nStartPos表示该路径由第几个位置开始
		inline xmValue PropertyValue(const xmStringVector& vPath, int nStartPos = 0) const
		{
			if (vPath.Size() == 0)
			{
				return xmValue(m_pBuffer, *m_pClass, false);
			}

			const xmClass* pClass = m_pClass->PropertyClass(vPath, nStartPos);
			if (pClass == NULL)
			{
				return xmValue();
			}
			return xmValue(m_pBuffer + (pClass->Offset() - m_pClass->Offset()), *pClass, false);
		}
		/** 以位置下标的方式获取属性数值
		@see PropertyValue(int uPos) const
		*/
		inline xmValue operator[](int uPos) const
		{
			return PropertyValue(uPos);
		}

		/** 以名称下标的方式获取属性数值
		@see PropertyValue(const char* strName) const
		*/
		inline xmValue operator[](const char* strName) const
		{
			return PropertyValue(strName);
		}

		/**	@} ************************************************************************/





		/******************************************************************************/
		/** @name 强制Geter/Seter接口函数
		@{
		*/
		/** 强制赋值函数。
		@param [in] rht
			某基本数据类型对象。
		@return 实例引用。
		@exception xmExIllegalInput
			当xmValue实例本身不是基本数据类型时抛出。
		@attention
			当xmValue实例本身不是基本数据类型时，编译无法通过。
		@remarks
			此函数尽量不要使用，可能在以后的版本中会被取消。
		*/
		template<typename T>
		inline xmValue& Set(const T& rht)
		{
			switch (Type())
			{
			case xmDT_INT8:
				i8() = (char)rht;
				break;
			case xmDT_UINT8:
				u8() = (unsigned char)rht;
				break;
			case xmDT_INT16:
				i16() = (short)rht;
				break;
			case xmDT_UINT16:
				u16() = (unsigned short)rht;
				break;
			case xmDT_INT32:
				i32() = (int)rht;
				break;
			case xmDT_UINT32:
				u32() = (unsigned int)rht;
				break;
			case xmDT_INT64:
				i64() = (long long)rht;
				break;
			case xmDT_UINT64:
				u64() = (unsigned long long)rht;
				break;
			case xmDT_FLOAT:
				f32() = (float)rht;
				break;
			case xmDT_DOUBLE:
				f64() = (double)rht;
				break;
			default:
				throw xmExIllegalInput();
				//if (Size() != xmTypeSize(rht))
				//{
				//	throw xmExIllegalInput();
				//}
				//memcpy(m_pBuffer, xmTypeBuffer(rht), xmTypeSize(rht));
				break;
			}
			return *this;
		}
		/** 强制取值函数。
		@param [in] rht
			某基本数据类型对象。
		@return 实例引用。
		@exception xmExIllegalInput
			当xmValue实例本身不是基本数据类型时抛出。
		@attention
			当xmValue实例本身不是基本数据类型时，编译无法通过。
		@remarks
			此函数尽量不要使用，可能在以后的版本中会被取消。
		*/
		template<typename T>
		inline const xmValue& Get(T& rht) const
		{
			xmAssert(Type() != xmDT_UNKNOWN);
			switch (Type())
			{
			case xmDT_INT8:
				rht = (T)i8();
				break;
			case xmDT_UINT8:
				rht = (T)u8();
				break;
			case xmDT_INT16:
				rht = (T)i16();
				break;
			case xmDT_UINT16:
				rht = (T)u16();
				break;
			case xmDT_INT32:
				rht = (T)i32();
				break;
			case xmDT_UINT32:
				rht = (T)u32();
				break;
			case xmDT_INT64:
				rht = (T)i64();
				break;
			case xmDT_UINT64:
				rht = (T)u64();
				break;
			case xmDT_FLOAT:
				rht = (T)f32();
				break;
			case xmDT_DOUBLE:
				rht = (T)f64();
				break;
			default:
				throw xmExIllegalInput();
				//if (Size() != xmTypeSize(rht))
				//{
				//	throw xmExIllegalInput();
				//}
				//memcpy(xmTypeBuffer(rht), m_pBuffer, xmTypeSize(rht));
				break;
			}
			return *this;
		}
		/**	@} ************************************************************************/




	public:
		/** 判断Buffer是不是由自己管理
		@return 是返回true
		*/
		inline bool IsMyBuffer() const
		{
			return (m_uAttribute & MY_BUFFER) != 0;
		}
		/** 判断Class是不是由自己管理
		@return 是返回true
		*/
		inline bool IsMyClass() const
		{
			return (m_uAttribute & MY_CLASS) != 0;
		}


	private:
		//	存储数值
		unsigned char* m_pBuffer;
		//	存储类型
		const xmClass* m_pClass;
		//	属性，用于判断Buffer和Class是不是由自己保存
		//	由自己保存的部分，将在析构的时候销毁
		mutable unsigned char m_uAttribute;
		enum
		{
			MY_NOTHING = 0x0,	//	xmValue实例既不保存Buffer，也不保存Class
			MY_BUFFER = 0x1,	//	xmValue实例保存Buffer
			MY_CLASS = 0x2		//	xmValue实例保存Buffer
		};


		/*==============================================================================
		函数名：xmValue()
		功  能：内部构造，使用一个已经存在的内存区和类型构建一个xmValue，参数bKeepClass表示
		新构建的xmValue是否要自己保存Class信息
		==============================================================================*/
		xmValue(void* pBuffer, const xmClass& aClass, bool bKeepClass) :
			m_pBuffer(static_cast<unsigned char*>(pBuffer)),
			m_pClass(bKeepClass ? new xmClass(aClass) : (&aClass)),
			m_uAttribute(bKeepClass ? MY_CLASS : MY_NOTHING)
		{
		}


		/*==============================================================================
		函数名：__PrintProperty()
		说  明：打印一条属性
		==============================================================================*/
		xmString __PrintProperty(const char* strHead, const char* strName) const;

		xmValue& __LoadValue(const void* pRoot);

		/*==============================================================================
		函数名：__Clear()
		说  明：清除整个xmValue
		==============================================================================*/
		inline void __Clear()
		{
			if (IsInvalid())
			{
				return;
			}

			if (IsMyBuffer())
			{
				delete[] m_pBuffer;
			}
			if (IsMyClass())
			{
				delete m_pClass;
			}

			m_pBuffer = NULL;
			m_uAttribute = MY_NOTHING;
			return;
		}



		/*==============================================================================
		函数名：__P()，__V()
		说  明：在指定位置提取一个类型为T的指针或值
		==============================================================================*/
		template <typename T>
		inline const T* __P(size_t pos = 0) const
		{
			return (const T*)(m_pBuffer + pos);
		}
		template <typename T>
		inline T* __P(size_t pos = 0)
		{
			return const_cast<T*>(static_cast<const xmValue&>(*this).__P<T>(pos));
		}
		template <typename T>
		inline const T& __V(size_t pos = 0) const
		{
			return *__P<T>(pos);
		}
		template <typename T>
		inline T& __V(size_t pos = 0)
		{
			return *__P<T>(pos);
		}
	};


	/******************************************************************************/
	/** @class xmIsTypeT<xmValue>
	@brief
		通过对xmIsTypeT模板进行专门化，实现对xmValue的RTTI。
	@par 多线程安全
		不适用
	@note
		xmValue被认为是xmDT_STRUCT类型。
	@see xmIsTypeT
	@par 修改记录
		@par
		2017.09.28
			-# 代码初次完成。
	*******************************************************************************/
	template <>
	class xmIsTypeT<xmValue>
	{
	public:
		static const xmEDataType TYPE = xmDT_STRUCT;
		static size_t Size(const xmValue& aValue)
		{
			return aValue.Size();
		}
		static const void* Buffer(const xmValue& aValue)
		{
			return aValue.m_pBuffer;
		}
		static void* Buffer(xmValue& aValue)
		{
			return aValue.m_pBuffer;
		}
	};
}

#endif	//__SSA_BASE_VALUE_H
