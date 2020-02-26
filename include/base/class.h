/******************************************************************************/
/** @file class.h
@brief
	定义和实现xmClass类、xmStruct类。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	class.cpp
@see ssa::xmClass
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_CLASS_H
#define __SSA_BASE_CLASS_H
#include "type.h"
#include "stringutil.h"
#include "exceptions.h"
#include "assertmsg.h"

namespace ssa
{
	class _TypeStruct;
	class _LoadClass;

	/******************************************************************************/
	/** @class xmClass
	@brief
		用于对仿真过程中不同数据的不同类型进行统一的描述。
	@details
		仿真系统中一个数据可能是基本数据类型、一段Buffer，也可能是带有某种KV（Key-Value）
		属性的结构化数据类型。xmClass使用 ssa::xmEDataType 描述基本数据类型和Buffer，当用于
		描述一个结构化数据时，使用JSON格式字符串，格式如下：
		@par
		@code
		{						//	Root层数据类，就一个对象，改对象内包括p1～p5共5个属性
			"p1":"INT8",		//	定义一个名称为p1，类型为i8的变量，该变量的按照位置偏移自动计算
			"p2":"INT32",		//	同级别的变量名称不能相同
			"p3":{				//	数据类中的每一个数据可以是一个基本类型（如p1、p2、sp1等），
				"sp1":"FLOAT",	//	可以是另外一个数据类（如p3），也可以是一个段空间（如p5）
				"sp2":"DOUBLE"
			},
			"p4":"UINT32",
			"p5":"BUFFER10"		//	定义一个长度为10的Buffer类型属性
		}
		@endcode

		一个结构化数据由若干属性（KV对）构成，每个属性包括属性名称、属性类型，属性类型可以是
		基本数据类型、Buffer，也可以是另一个结构化数据类型。当类型是基本数据类型时，允许的描
		述包括：基本数据（UINT8、INT8、UINT16、INT16、UINT32、INT32、UINT64、INT64、FLOAT、
		DOUBLE）或者BUFFER+数字（如BUFFER8，BUFFER64）。
	@par 多线程安全
		是
	@note
		xmClass一旦构建，不能修改，只能使用。
	@attention
		当xmClass表示结构化数据时，属性的名称中不能包含符号“.”。
	@remarks xmClass通常与xmValue配合使用，用于描述xmValue的类型。
	@see xmValue
	@par 修改记录
		@par
		2020.01.20
			-# 增加了对定长xmDT_STRING类型的支持。
		2019.01.10
			-# 增加了以xmStringVector为输入的属性类获取方法。
		2018.11.28 
			-# 增加了Combine()静态函数，用于实现实例的组合
			-# xmClass(xmEDataType, size_t)构造函数支持传入xmDT_STRUCT类型。
		2018.06.30 
			-# 删除了explicit xmClass(std::istream& iMapInfo)接口。
		2018.06.23 
			-# 扩展了xmClass(const char*)接口，除了可以通过JSON构造外，可以通过基本数据类型描述的
			字符串构造类实例。
			-# 同步修改了两个Print函数。
		@par
		2018.04.27
			-# 添加了 Combine() 接口，可以使用两个结构化xmClass对象，组合成一个新的对象。
		@par
		2018.01.04 
			-# 添加了 IsSimilar() 函数和 IsValid() 函数，用于判断两个xmClass对象是否相似和是否有效。
		@par
		2017.10.25
			-# PropertyClass(const char* strName) 函数支持直接获取下一级属性。
		@par
		2017.10.09
			-# 为了在接口不使用STL，将记录结构信息的map和vector封装为一个_TypeStruct类；
			-# 将遍历的接口由size_t改为int，以防止使用一个常数遍历时进行类型转换，如
			@par
			@code
				aClass[(size_t)0]	//	使用size_t接口，如果不进行显式的类型转换，
									//	编译器不知道将一个int类型是自动转换为size_t
									//	还是const char*
				aClass[0]			//	使用int接口
			@endcode
		@par
		2017.09.27 重大更改
			-# 重新定义了xmClass设计原则，一旦xmClass被创建，将不允许修改。因此去掉了
			修改xmClass的一些接口，如Append、Map等。
		@par
		2015.05.13 重大更改
			-# 去掉了与xmValue之间的相互引用；
			-# 增加了使用基本类型创建xmClass的接口。
		@par
			2015.05.08
			-# 将Count()变为PropertyCount()。
			-# 增加了m_eType属性，修改了相关函数。
		@par
			2015.04.22
			-# 增加了Append接口。
		@par
			2015.04.11
			-# 代码初次完成，由xmValue中提取。
	*******************************************************************************/
	class xmBASE_EXPORT xmClass
	{
		friend _LoadClass;
	public:
		/******************************************************************************/
		/** @name 构造函数与析构函数
		@{ 
		*/
		
		/** 使用基本数据类型或Buffer构造
		@param [in] eType
			xmClass的类型，可以是xmDT_STRUCT之外的其他xmEDataType值。如需要使用构建结构化
			类型，需要使用xmClass(const char* strMapInfo)或xmClass(std::istream& iMapInfo)
			如果传入xmDT_STRUCT，则创建一个空的结构类型，仅用于与其他实例组合
		@param [in] uBufferSize 
			当eType为xmDT_BUFFER或xmDT_STRING时，由uBufferSize指定Buffer的长度，否则忽略该参数。
		@exception xmExIllegalInput
			当eType为xmDT_NULL，或者eType为xmDT_BUFFER或xmDT_STRING，但uBufferSize为0时抛出。
		@par 示例
			@par
			@code
				xmClass aClass1(xmDT_INT32);		//	构造一个有符号整型的xmClass
				xmClass aClass1(xmDT_BUFFER, 16);	//	构造一个长度为16字节的Buffer类型的xmClass
			@endcode
		*/
		explicit xmClass(xmEDataType eType, size_t uBufferSize = 0);

		/** 使用字符串构造 
		@param [in] strMapInfo
			可以是符合JSON格式的数据类描述，也可以是基本数据类型描述。
		@exception xmExIllegalInput 
			当strMapInfo为NULL时抛出。
		@exception xmExJsonFile 
			当strMapInfo描述的JSON字符串错误或出现同名属性时抛出。
		@par 示例
			@par
			@code
				xmString strJson = "{	\
					'p1':'INT8',		\
					'p2':'INT32',		\
					'p3':{				\
						'sp1':'FLOAT',	\
						'sp2':'DOUBLE'	\
					},					\
					'p4':'UINT32',		\
					'p5':'BUFFER9'		\
				}";
				//	字符串中的单引号“'”不符合JSON格式规范，因此替换为双引号
				xmClass c1(xmStringUtil::Replace(strJson, "'", "\""));
				xmClass c2("UINT32");
				xmClass c3("BUFFER16");
			@endcode
		*/
		explicit xmClass(const char* strMapInfo);
		
		/** 使用JSON输入流构造 
		@param [in] iMapInfo
			符合JSON格式的输入流。
		@exception xmExJsonFile
			当iMapInfo描述的JSON流格式错误或出现同名属性时抛出。
		@par 示例
			@par
			@code
				//------------------------------------------------------------------------
				//	数据类描述文件名称：data.json
				//	数据类描述文件内容：
				//	{
				//		"p1":"INT8",		//	定义一个名称为p1，类型为i8的变量，该变量的按照位置偏移自动计算
				//		"p2":"INT32",		//	同级别的变量名称不能相同
				//		"p3":{				//	数据类中的每一个数据可以是一个基本类型（如p1、p2、sp1等），
				//			"sp1":"FLOAT",	//	可以是另外一个数据类（如p3），也可以是一个段空间（如p5）
				//			"sp2":"DOUBLE"
				//		},
				//		"p4":"UINT32",
				//		"p5":'BUFFER9'
				//	}
				//------------------------------------------------------------------------
				std::ifstream ifJson("data.json");
				xmClass c(ifJson)
				ifJson.close();
			@endcode
		*/
		//explicit xmClass(std::istream& iMapInfo) :
		//	m_uOffset(0),
		//	m_uSize(0),
		//	m_eType(xmDT_UNKNOWN),
		//	m_pStruct(NULL)
		//{
		//	std::string strMapInfo;
		//	std::getline(iMapInfo, strMapInfo, (char)EOF);
		//	__Load(strMapInfo.c_str());
		//}
		
		/** 拷贝构造函数
		@param [in] aClass
			另一个xmClass实例。
		*/
		xmClass(const xmClass& aClass);

		/** 析构函数
		*/
		virtual ~xmClass(void)
		{
			__Clear();
		};
		


		/** 组合两个xmClass实例
		@param [in] aClass
			被组合对象xmClass实例，必须是结构类型。
		@param [in] bClass
			组合对象xmClass实例，可以是基本数据类型，也可以是结构类型。
		@param [in] strName
			组合对象的名字，如果为空，则bClass必须为结构类型，并将bClass的每条属性插入到aClass中。
			如果为非空，则bClass可以为任何有效类型，并将其整体作为一条属性插入到aClass中。
		@param [in] uPropertyIndex
			aClass被插入bClass的属性位置索引，-1表示在最后一个属性后插入
		@return 
			返回一个新的xmClass实例指针。
		@note
			组合两个xmClass实例，将bClass插入到aClass的指定位置，不会修改aClass或bClass的结构。		
		*/
		static xmClass* Combine(const xmClass& aClass, const xmClass& bClass, const char* strName = NULL, int uPropertyIndex = -1);
		/** 按照顺序插入一组xmClass实例
		@param [in] pNameArray
			被组合对象xmClass实例名称列表。
		@param [in] pClassArray
			被组合对象xmClass实例名称列表。
		@param [in] uCount
			列表长度。
		@return 
			返回一个新的xmClass实例指针。
		*/
		static xmClass* Combine(const char** pNameArray, const xmClass** pClassArray, size_t uCount);
		/**	@} ************************************************************************/



	public:
		/******************************************************************************/
		/** @name 相等判断函数			
		@{
		*/
		
		/** 判断两数据类是否相等。
		@param [in] aClass
			另一个数据类
		@return 相等返回true。
		@note
			两个数据类相等是指其描述的类型相同、Buffer的长度相等。当描述结构化数据时，
			还要求其属性的名称、类型、个数、位置等相同。两个xmClass是否相同与其在整
			个数据链中的位置名称无关。		
		*/
		bool IsEqual(const xmClass& aClass) const;

		/** 判断两数据类是否相似。
		@param [in] aClass
			另一个数据类
		@return 相似返回true。
		@note
			两个数据类相似是指其描述的类型相同、Buffer的长度相等。当描述结构化数据时，
			还要求其属性的类型、个数、位置等相同，但属性名称可以不相同。
		*/
		bool IsSimilar(const xmClass& aClass) const;

		/** 判断两数据类是否相等
		@see xmClass::IsEqual(const xmClass& aClass) const
		*/
		inline bool operator==(const xmClass& aClass) const
		{
			return IsEqual(aClass);
		}
		/** 判断两数据类是否不相等
		@see xmClass::IsEqual(const xmClass& aClass) const
		*/
		inline bool operator!=(const xmClass& aClass) const
		{
			return !IsEqual(aClass);
		}
		
		/**	@} ************************************************************************/


		
		/******************************************************************************/
		/** @name 基本操作函数
		@{
		*/
		
		/**	判断该数据类是否为无效数据类
		@note 导致无效数据类产生的主要原因就是在使用JSON构建时出现了异常。 
		*/
		inline bool IsInvalid() const
		{
			return (m_eType == xmDT_UNKNOWN);
		}
		/**	判断该数据类是否为有效数据类
		*/
		inline bool IsValid() const
		{
			return (m_eType != xmDT_UNKNOWN);
		}
		/**	该数据类是否为顶层类。
		@note 用户直接构造出来的数据类均为顶层数据类，只有作为属性的数据类不是顶层数据类。
		*/
		inline bool IsRoot() const
		{
			return (m_uOffset == 0);
		}
		/**	该数据类在数据类链中的偏移。
		@note 顶层数据类偏移为0，只有非顶层数据类即某个属性的偏移为非0。 
		*/
		inline size_t Offset() const
		{
			return m_uOffset;
		}
		/**	该数据类所描述的数据占用的内存大小 
		*/
		inline size_t Size() const
		{
			return m_uSize;
		}
		/**	该数据类所描述的数据类型 
		*/
		inline xmEDataType Type() const
		{
			return m_eType;
		}
		/**	以JSON格式打印数据类，输出结果可以直接用于构造其他数据类 
		*/
		const xmString& PrintJson(void) const;
		/**	以便于阅读的方式打印数据类，可以方便开发、调试人员查看 
		@par 示例
			参见 ssa::xmValue::Print() 函数
		*/
		const xmString& PrintInfo(void) const;

		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 结构化数据类属性访问函数
		@attention 下列所有函数仅在xmClass表示结构化数据时可以调用，否则会抛出xmExIllegalCall异常。
		@{
		*/
		
		/** 获取该数据类拥有的属性数量
		@return 属性数量，如果不是结构化类型，则返回0。
		*/
		inline int PropertyCount() const;

		/** 获取该数据类的第uPos个属性名称
		@param [in] uPos
			属性序号，由0开始。
		@return 属性名称字符串指针，uPos越界时返回NULL。
		@exception xmExIllegalCall
			当该数据类不是xmDT_STRUCT时抛出。
		*/
		inline const char* PropertyName(int uPos) const;

		/** 获取该数据类的第uPos个属性。
		@param [in] uPos
			属性序号，由0开始。
		@return 属性类指针，uPos越界时返回NULL。
		@exception xmExIllegalCall
			当该数据类不是xmDT_STRUCT时抛出。
		@attention 
			此处的参数uPos不能设计为size_t类型，因为存在PropertyClass(const char*)函数，如果uPos
			为size_t类型，当调用PropertyClass(0)时，0被认为是int类型，编译器无法识别需要将0转换为
			size_t还是const char*，从而产生C2668错误。同理，下面的 xmClass& operator[](int) 的参
			数类型也必须是int。
		@remarks
			PropertyClass函数之所以不设计为返回引用，是为了避免产生不必要的拷贝复制。虽然下列用法
			是可以接受的：
			@par
			@code
				const xmClass& aClass = myClass.PropertyClass(0);
			@endcode
			但因为函数返回引用，如果用户将其赋值给一个实例时，会导致一个拷贝构造。如当用户这样使用时：
			@par
			@code
				xmClass aClass = myClass.PropertyClass(0);
			@endcode
			这个使用产生了不必要的拷贝复制，耗费了不必要的时间。因此仅返回指针，不返回引用。
		*/
		inline const xmClass* PropertyClass(int uPos) const;

		/** 获取该数据类指定名字的属性。
		@param [in] strName
			属性名称，可以使用“.”符号，直接获取下层级属性类对象指针。
		@return 属性类指针，当指定的属性名称不存在时返回NULL。
		@exception xmExIllegalCall
			当该数据类不是xmDT_STRUCT时抛出。
		@note
			参数strName前后的空白字符（如空格、制表符（\\t）、回车（\\r）、换行（\\n））均将被忽略，
			但不影响中间的空白字符，如“  My Name  ”会被作为“My Name”使用。\n
			如果参数strName为不包含“.”符号的字符串，则直接获取本级属性，如果包含“.”符号，则认为
			“.”后的内容是下一级属性。如
			@par
			@code
				//------------------------------------------------------------------------
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
				//------------------------------------------------------------------------
				std::ifstream ifJson("data.json");
				xmClass c(ifJson)
				ifJson.close();

				//	下列代码中，pClass1和pClass2指向同一个属性对象
				const xmClass* pClass1 = c.PropertyClass("p3")->PropertyClass("sp1");
				const xmClass* pClass2 = c.PropertyClass("p3.sp1");
			@endcode
		*/
		const xmClass* PropertyClass(const char* strPath = NULL) const;
		//	vPath为属性路径
		//	uStartPos表示该路径由第几个位置开始
		const xmClass* PropertyClass(const xmStringVector& vPath, int uStartPos = 0) const;

		/** 以位置下标的方式获取属性类
		@see PropertyClass(int uPos) const
		@attention
			此处的返回值设计为返回引用而不是返回指针，是为了能够以aClass[2][1]的方式使用。
		*/
		inline const xmClass& operator[](int uPos) const
		{
			return *PropertyClass(uPos);
		}

		/** 以名称下标的方式获取属性类
		@see PropertyClass(const char* strName) const
		*/
		inline const xmClass& operator[](const char* strName) const
		{
			return *PropertyClass(strName);
		}
		
		/**	@} ************************************************************************/



	private:
		/*******************************************************************************
		功  能：描述数据类型的基本信息
		说  明：当m_uOffset为0时，表示本数据类为顶层数据类，否则为某一个数据类的属性类
		*******************************************************************************/
		const size_t m_uOffset;		//	数据类在整个数据链中的初始位置偏移
		size_t m_uSize;				//	数据类所代表数据的长度
		xmEDataType m_eType;		//	数据类类型

		/*******************************************************************************
		功  能：描述结构化数据的KV属性信息
		说  明：
				1、当xmClass用于表示一个结构化数据（xmDT_STRUCT）时，使用_TypeStruct类描述该
				类型具有的KV属性信息；
				2、xmClass表示基本数据类型或者Buffer时，m_pStruct为NULL
		*******************************************************************************/
		_TypeStruct* m_pStruct;


		/*******************************************************************************
		功  能：下列两个变量分别用于记录数据类的打印信息
		说  明：由于xmClass一经初始化，就不会改变，因此打印的信息可以保存下来，这样第二
				次调用打印接口时可以不必再次拼接字符串。
		*******************************************************************************/
		mutable xmString m_strPrintJson;
		mutable xmString m_strPrintInfo;


		/*******************************************************************************
		函数名：xmClass()
		说  明：仅供合并两个实例时构造使用
		*******************************************************************************/
		explicit xmClass(size_t uOffset, size_t uSize, xmEDataType eType, _TypeStruct* pStruct) :
			m_uOffset(uOffset),
			m_uSize(uSize),
			m_eType(eType),
			m_pStruct(pStruct)
		{}

		/*******************************************************************************
		函数名：xmClass()
		说  明：仅供创建下一层属性结构时构造使用
		*******************************************************************************/
		explicit xmClass(size_t uOffset) :
			m_uOffset(uOffset), 
			m_uSize(0), 
			m_eType(xmDT_UNKNOWN),
			m_pStruct(NULL)
		{
		};

		//	使用JSON字符串构造一个xmClass
		void __Load(const char* strMapInfo);
		//	销毁
		void __Clear();
		//	获取本级属性
		const xmClass* __PropertyClass(const char* strName) const;
	};
}

#endif	//__SSA_BASE_CLASS_H
