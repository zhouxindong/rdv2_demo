/******************************************************************************/
/** @file rtti.h
@brief
	定义一组运行时类型、指针、继承关系识别模板和函数。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	无
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_RTTI_H
#define __SSA_BASE_RTTI_H
#include "type.h"


namespace ssa
{
	/** xmIsTypeT 的模板函数，获取变量类型
	@return 变量类型
	@see xmIsTypeT
	*/
	template<typename T>
	xmEDataType xmIsType(const T&)
	{
		return xmIsTypeT<T>::TYPE;
	}
	/** xmIsTypeT 的模板函数，获取变量大小
	@return 变量大小
	@see xmIsTypeT
	*/
	template<typename T>
	size_t xmTypeSize(const T& aParam)
	{
		return xmIsTypeT<T>::Size(aParam);
	}
	/** xmIsTypeT 的模板函数，获取变量地址
	@return 变量地址
	@see xmIsTypeT
	*/
	template<typename T>
	void* xmTypeBuffer(T& aParam)
	{
		return xmIsTypeT<T>::Buffer(aParam);
	}
	template<typename T>
	const void* xmTypeBuffer(const T& aParam)
	{
		return xmIsTypeT<T>::Buffer(aParam);
	}

	/** xmIsPointerT 的模板函数，判断变量是否为指针
	@return 是指针返回true
	@see xmIsPointerT
	*/
	template<typename T>
	bool xmIsPointer(const T&)
	{
		return xmIsPointerT<T>::YES;
	}

	/** xmIsDerivedT 的模板函数，判断类型T是否为类型B的子类
	@return 是子类返回true
	@see xmIsDerivedT
	*/
	template<typename DerivedT, typename BaseT>
	bool xmIsDerived(const DerivedT&, const BaseT&)
	{
		return xmIsDerivedT<DerivedT, BaseT>::YES;
	}

	template<typename T>
	bool xmIsConst(T&)
	{
		return xmIsConstT<T>::YES;
	}



	/******************************************************************************/
	/** @class xmIsTypeT
	@brief
		运行时类型识别模板。
	@details
		通过对该模板的专门化，可以对一个变量类型进行运行时类型识别、长度计算和取得该变量
		的存储空间。目前能够识别的变量包括char、unsigend char、short、unsigend short、
		int、unsigend int、long、unsigend long、float、double以及无符号字符数组的识别。
		其他类型如果需要被识别，可通过专门化该模板实现。
	@note 
		模板提供了一组模板函数封装，包括： xmIsType() 、 xmTypeSize() 、 xmTypeBuffer() 
		可直接返回变量类型。
	@par 多线程安全
		不适用
	@par 示例
		@par
		@code
			char c = 0;

			//	使用模板类
			xmIsTypeT<char>::TYPE == xmDT_INT8;	//	true
			xmIsTypeT<char>::Size(c) == 1;		//	true
			xmIsTypeT<char>::Buffer(c) == &c;	//	true
			
			//	使用模板函数
			xmIsType(c) == xmDT_INT8;			//	true
			xmTypeSize(c) == 1;					//	true
			xmTypeBuffer(c) == &c;				//	true
		@endcode
	@par 修改记录
		@par
		2017.09.20
			-# 修改了模板的名称，为了区分函数，在模板名称后面增加了字符T；
			-# 添加了对应xmIsPointerT和xmIsDerivedT的函数。
		2014.03.03：
			-# 代码初次完成。
	*******************************************************************************/
	template <typename T> 
	class xmIsTypeT
	{
	public:
		/** 获取变量类型，不同专门化应修改为不同数值
		*/
		static const xmEDataType TYPE = xmDT_UNKNOWN;

		/** 获取变量大小，默认使用sizeof运算符
		*/
		static size_t Size(const T& Value)
		{
			return sizeof(T);
		}

		/** 获取变量地址，默认使用&运算符
		*/
		static const void* Buffer(const T& Value)
		{
			return (&Value);
		}
		static void* Buffer(T& Value)
		{
			return (&Value);
		}
	};
	
	//	识别基本数据类型
#define __SET_DB_BASE_TYPE(T, name)					\
	template <> class xmIsTypeT<T>					\
	{												\
	public:											\
		typedef T NumberType;						\
		static const xmEDataType TYPE = name;		\
		static size_t Size(const T& Value)			\
		{											\
			return sizeof(T);					    \
		}											\
		static const void* Buffer(const T& Value)	\
		{											\
			return (&Value);						\
		}											\
		static void* Buffer(T& Value)				\
		{											\
			return (&Value);						\
		}											\
	}
//	__SET_DB_BASE_TYPE(bool,			xmDT_BOOL);
	__SET_DB_BASE_TYPE(char,			xmDT_INT8);
	__SET_DB_BASE_TYPE(unsigned char,	xmDT_UINT8);
	__SET_DB_BASE_TYPE(short,			xmDT_INT16);
	__SET_DB_BASE_TYPE(unsigned short,	xmDT_UINT16);
	__SET_DB_BASE_TYPE(int,				xmDT_INT32);
	__SET_DB_BASE_TYPE(unsigned int,	xmDT_UINT32);
	__SET_DB_BASE_TYPE(long,            xmDT_INT32);
	__SET_DB_BASE_TYPE(unsigned long,   xmDT_UINT32);
	__SET_DB_BASE_TYPE(long long,       xmDT_INT64);
	__SET_DB_BASE_TYPE(unsigned long long, xmDT_UINT64);
	__SET_DB_BASE_TYPE(float,			xmDT_FLOAT);
	__SET_DB_BASE_TYPE(double,			xmDT_DOUBLE);
#undef __SET_DB_BASE_TYPE

	//	识别无符号字符数组  xmDT_BUFFER
	template <unsigned int N> 
	class xmIsTypeT<const unsigned char[N]>
	{
	public:
		static const xmEDataType TYPE = xmDT_BUFFER;
		static size_t Size(const unsigned char (&Value)[N])
		{
			return N;
		}
		static const void* Buffer(const unsigned char (&Value)[N])
		{
			return Value;
		}
		static void* Buffer(unsigned char(&Value)[N])
		{
			return Value;
		}
	};

	//	识别字符数组
	//template <unsigned int N> 
	//class xmIsTypeT<const char[N]>
	//{
	//public:
	//	static const xmEDataType TYPE = xmDT_STRING;
	//	static size_t Size(const char (&Value)[N])
	//	{
	//		return N;
	//	}
	//	static const void* Buffer(const char (&Value)[N])
	//	{
	//		return Value;
	//	}
	//};
	//	识别STL字符串
	//template <> 
	//class xmIsTypeT<std::string>				
	//{											
	//public:										
	//	static const xmEDataType TYPE = xmDT_STRING;
	//	static size_t Size(const std::string& Value)
	//	{										
	//		return (Value.size()+1);			
	//	}										
	//	static const void* Buffer(const std::string& Value)
	//	{										
	//		return Value.c_str();				
	//	}										
	//};



	/******************************************************************************/
	/** @class xmIsPointerT
	@brief
		运行时指针识别模板。
	@details
		识别一个变量，是否为指针类型。
	@note 
		模板提供了 xmIsPointer() 模板函数封装，可直接返回结果。
	@par 多线程安全
		不适用
	@par 示例
		@par
		@code
			unsigned char* p = NULL;

			//	使用模板类
			xmIsPointerT<unsigned char*>::YES == 1;	//	true
			xmIsPointerT<unsigned char>::NO == 1;	//	true
			xmIsPointerT<unsigned char*>::BaseT p2 = 0;	//	相当于：unsigned char p2 = 0；
			xmIsPointerT<unsigned char*>::ThisT p3 = 0;	//	相当于：unsigned char* p3 = 0；

			//	使用模板函数
			xmIsPointer(p) == true;			//	true
		@endcode
	@par 修改记录
		@par
		2014.03.03：
			-# 代码初次完成。
	*******************************************************************************/
	template<typename T> 
	class xmIsPointerT
	{
	public:
		enum
		{
			YES = 0,	/**< 是指针类型 */
			NO  = 1,	/**< 不是指针类型 */
		};
		typedef T BaseT;	/**< 指针的基类型 */
		typedef T ThisT;	/**< 指针类型 */

		/** 将一个指针转换为指定类型指针
		*/
		static ThisT ToThisT(const void* p)
		{
			return *((T*)p);
		}
	};
	template<typename T> 
	class xmIsPointerT<T*>
	{
	public:
		enum
		{
			YES = 1,
			NO  = 0,
		};
		typedef T  BaseT;
		typedef T* ThisT;
		static ThisT ToThisT(const void* p)
		{
			return (BaseT*)p;
		}
	};



	/******************************************************************************/
	/** @class xmIsDerivedT
	@brief
		运行时类继承关系识别模板。
	@details
		识别DerivedT类是否为BaseT类的一个子类。
	@note 
		模板提供了 xmIsDerived() 模板函数封装，可直接返回结果。
	@par 多线程安全
		不适用
	@par 示例
		@par
		@code
			class A {};
			A a;
			class B {};
			B b;
			class C : public A {};
			C c;

			//	使用模板类
			xmIsDerivedT<B, A>::YES == false;
			xmIsDerivedT<B, A>::NO  == true;
			xmIsDerivedT<C, A>::YES == true;
			xmIsDerivedT<C, A>::NO  == false;

			//	使用模板函数
			xmIsDerived(b, a) == false;
			xmIsDerived(c, a) == true;
		@endcode
	@par 修改记录
		@par
		2014.03.03：
			-# 代码初次完成。
	*******************************************************************************/
	template<typename DerivedT, typename BaseT> 
	class xmIsDerivedT
	{
	private:
		class No{};
		class Yes{ No __no[2]; };
		static Yes Test(BaseT*){return Yes();};
		static No  Test(...){return No();};
	public:
		enum
		{
			YES = (sizeof(Test(static_cast<DerivedT*>(0))) == sizeof(Yes)),	/**< 是继承关系 */
			NO  = (sizeof(Test(static_cast<DerivedT*>(0))) != sizeof(Yes)),	/**< 不是继承关系 */
		};
	};


	template <typename T>
	class xmIsConstT
	{
	public:
		enum
		{
			YES = 0,
			NO = 1,
		};
		typedef T BaseT;
		typedef T ThisT;
	};
	template<typename T>
	class xmIsConstT<const T>
	{
	public:
		enum
		{
			YES = 1,
			NO = 0,
		};
		typedef T  BaseT;
		typedef const T ThisT;
	};
}
#endif //	__SSA_BASE_RTTI_H