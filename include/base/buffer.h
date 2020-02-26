/******************************************************************************/
/** @file buffer.h
@brief
	提供内存管理功能，实现了基本内存操作的 xmBuffer 类，并在此基础上实现了可链接的
	xmStaticBuffer 类和可调整大小的 xmDynamicBuffer 类。
	接口类型。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	buffer.cpp
@see ssa::xmBuffer
@see ssa::xmStaticBuffer
@see ssa::xmDynamicBuffer
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_BUFFER_H
#define __SSA_BASE_BUFFER_H
#include "rtti.h"
#include "stringutil.h"
#include "exceptions.h"
#ifdef xmOS_LINUX
#include <string.h>
#include <stdio.h>
#endif



namespace ssa
{
	class xmClass;
	class xmValue;
	/******************************************************************************/
	/** @class xmBuffer
	@brief
		提供内存管理功能。
	@details
		提供了基本的内存操作接口，可以与不同数据类型进行转换，并自动销毁。可用于替代常规内存
		使用功能。
	@par 多线程安全
		否
	@note 
		xmBuffer不提供operator=()的重载，以避免产生下列歧义：函数无法确定用户是希望复制内容，
		还是克隆空间，用户使用时也容易在这两方面产生理解上的不确定。
	@par 修改记录
		@par
		2018.07.29 
			-# 禁止使用operator=进行赋值。
			-# 添加了xmRet Malloc(const xmBuffer&)接口
		2018.06.12
			-# 将API中的Pointer<>()接口由protected类型，改为public类型。
		2017.11.1
			-# 修改了构造函数和Malloc函数，去掉了指定初始值的参数。
		2017.10.25
			-# 增加了直接使用字符串构建Buffer的接口。
		2017.10.15 
			-# 提取了 xmStaticBuffer 类和 xmDynamicBuffer 类。
		2014.03.24 
			-# 代码初次完成。
	*******************************************************************************/
	class xmBASE_EXPORT xmBuffer
	{
	public:
		/******************************************************************************/
		/** @name 构造函数与析构函数
		@{
		*/

		/** 创建空的对象
		*/
		xmBuffer(void) :m_pBuffer(NULL), m_uSize(0)
		{
		}

		/** 创建指定大小的内存空间，并使用0初始化
		@param [in] uMallocSize
			创建内存空间的大小。
		*/
		xmBuffer(size_t uMallocSize) : m_pBuffer(NULL), m_uSize(0)
		{
			Malloc(uMallocSize);
		}

		/** 创建指定大小的内存空间，并使用另一段内存数据初始化
		@param [in] uMallocSize
			创建内存空间的大小。
		@param [in] pBuffer
			初始化数据。
		@note 
			pBuffer的大小不能小于uMallocSize指定的大小。
		*/
		xmBuffer(size_t uMallocSize, const void* pBuffer) : m_pBuffer(NULL), m_uSize(0)
		{
			Malloc(uMallocSize, pBuffer);
		}

		/** 根据字符串描述的Buffer内容创建
		@param [in] aString
			使用某一格式的字符串构建，低字节在前，高字节在后。
		@param [in] strSeparator
			字符串的分隔符。
		@exception xmExIllegalInput
			当字符串格式错误或内容无法转换为数字时抛出，出现异常后，产生的xmBuffer对象是空对象。
		@see 
			Malloc(const char*, const char*)
		*/
		explicit xmBuffer(const char* aString, const char* strSeparator = xmBLANK_STRING) : m_pBuffer(NULL), m_uSize(0)
		{
			if (Malloc(aString, strSeparator) != xmE_SUCCESS)
			{
				throw xmExIllegalInput();
			}
		}

		/** 拷贝构造函数
		@param [in] aBuffer
			被复制的另一个对象。
		*/
		xmBuffer(const xmBuffer& aBuffer) : m_pBuffer(NULL), m_uSize(0)
		{
			Malloc(aBuffer);
		}

		/** 析构函数
		@note
			析构时，会销毁内存空间。
		*/
		virtual ~xmBuffer(void)
		{
			Free();
		}

		/** 
		@def xmB4(...)
			通过一组无符号8位整数，直接生成一个xmBuffer对象，主要用于xmBuffer的构造中。
		@param [in] ...
			一组无符号整数，低字节内容在前，高字节内容在后，如果数字大于0xFF，则
			被转换为unsigned char类型。
		@return
			xmBuffer对象。
		@par 示例
			@par
			@code
				xmBuffer aBuffer = xmB4(0x44, 0x33, 0x22, 0x11);
				xmBuffer aBuffer(xmB4(0x44, 0x33, 0x22, 0x11));
			@endcode
		*/
		#ifdef xmOS_WINDOWS
		#	define xmB4(...) ssa::xmBuffer::BuildBufferByByte(0, ## __VA_ARGS__, INFINITE)
		#else  //	Linux
		#	define xmB4(...) ssa::xmBuffer::BuildBufferByByte(0, ## args, INFINITE)
		#endif

		/** 通过 xmB4() 宏使用，使用一组无符号8位整数直接生成一个xmBuffer实例。
		@see xmB4()
		*/
		static xmBuffer BuildBufferByByte(size_t foo, ...);

		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 空间分配与销毁函数
		@{
		*/

		/** 创建指定大小的内存空间，并使用0初始化
		@param [in] uMallocSize
			创建内存空间的大小。
		@retval xmE_SUCCESS
			创建成功。
		@attention
			Malloc函数会销毁原来的内存空间。
		@note
			只有Malloc函数和Free函数会改变xmBuffer对象的长度。
		*/
		xmRet Malloc(size_t uMallocSize);

		/** 创建指定大小的内存空间，并使用另一段内存数据初始化
		@param [in] uMallocSize
			创建内存空间的大小。
		@param [in] pBuffer
			初始化数据。
		@retval xmE_SUCCESS
			创建成功。
		@retval xmE_ILLEGAL_INPUT
			pBuffer为NULL。
		@attention
			Malloc函数会销毁原来的内存空间。
		@note 
			pBuffer的大小不能小于uMallocSize指定的大小。
		*/
		xmRet Malloc(size_t uMallocSize, const void* pBuffer);

		/** 使用另一个xmBuffer对象创建内存空间
		@param [in] aBuffer
			另一个xmBuffer对象。
		@retval xmE_SUCCESS
			创建成功。
		@retval xmE_ILLEGAL_INPUT
			另一个xmBuffer对象为NULL。
		@attention
			Malloc函数会销毁原来的内存空间。
		*/
		xmRet Malloc(const xmBuffer& aBuffer)
		{
			return Malloc(aBuffer.Size(), aBuffer.Buffer());
		}

		/** 使用字符串描述的内容，创建内存空间
		@param [in] aString
			使用某一格式的字符串构建，格式要求详见下面说明。
		@param [in] strSeparator
			字符串的分隔符。
		@retval xmE_SUCCESS
			创建成功。
		@retval xmE_ILLEGAL_INPUT
			输入的字符串格式错误。
		@note
			使用字符串创建Buffer有两大类方法，一类是使用JSON描述，一类是使用内存描述。（1）使用JSON
			描述时，有下列几种格式：
				{"TYPE":"FLOAT",  "VALUE":100.0}
				{"TYPE":"FLOAT",  "VALUE":"100.0"}
				{"TYPE":"STRING", "VALUE":"hello world!"}
				{"TYPE":"BUFFER", "VALUE":"0x11, 0x12"}
				{"TYPE":"STRUCT", "VALUE":[
					{"TYPE":"INT32", "VALUE":-100}, 
					{"TYPE":"UINT8", "VALUE":200}, 
					{"TYPE":"INT64", "VALUE":300}
				]}
				{"TYPE":"STRUCT", "VALUE":[
					{"TYPE":"FLOAT",  "VALUE":100.0},
					{"TYPE":"FLOAT",  "VALUE":"100.0"},
					{"TYPE":"STRING", "VALUE":"hello world!"},
					{"TYPE":"BUFFER", "VALUE":"0x11, 0x12"},
					{"TYPE":"UINT64", "VALUE":100}, 
					{"TYPE":"INT16",  "VALUE":-200}, 
					{"TYPE":"DOUBLE", "VALUE":300.23456},
					{"TYPE":"UINT64", "VALUE":100}, 
					{"TYPE":"DOUBLE", "VALUE":-200}, 
					{"TYPE":"UINT64", "VALUE":300},
					{"TYPE":"STRUCT", "VALUE":[
						{"TYPE":"STRING", "VALUE":"hello world!"},
						{"TYPE":"BUFFER", "VALUE":"0x21, 0x22"},
						{"TYPE":"STRUCT", "VALUE":[
							{"TYPE":"INT32", "VALUE":100}, 
							{"TYPE":"INT32", "VALUE":200}, 
							{"TYPE":"INT32", "VALUE":300}
						]}
					]}
				]}
				{"TYPE":"STRUCT", "VALUE":[
					{"TYPE":"UINT64", "VALUE":100}, 
					{"TYPE":"INT16",  "VALUE":-200}, 
					{"TYPE":"DOUBLE", "VALUE":300.23456}
					{"TYPE":"UINT64", "VALUE":100}, 
					{"TYPE":"DOUBLE", "VALUE":-200}, 
					{"TYPE":"UINT64", "VALUE":300},
					{"TYPE":"STRUCT", "VALUE":[
						{"TYPE":"STRING", "VALUE":"hello world!"},
						{"TYPE":"BUFFER", "VALUE":"0x21, 0x22"},
						{"TYPE":"STRUCT", "VALUE":[
							{"TYPE":"INT32", "VALUE":100}, 
							{"TYPE":"INT32", "VALUE":200}, 
							{"TYPE":"INT32", "VALUE":300}
						]}
					]},
					{"TYPE":"UINT64", "VALUE":100}, 
					{"TYPE":"INT16",  "VALUE":-200}, 
					{"TYPE":"DOUBLE", "VALUE":300.23456}
					{"TYPE":"UINT64", "VALUE":100}, 
					{"TYPE":"DOUBLE", "VALUE":-200}, 
					{"TYPE":"UINT64", "VALUE":300},
					{"TYPE":"STRUCT", "VALUE":[
						{"TYPE":"STRING", "VALUE":"hello world!"},
						{"TYPE":"BUFFER", "VALUE":"0x21, 0x22"},
						{"TYPE":"STRUCT", "VALUE":[
							{"TYPE":"INT32", "VALUE":100}, 
							{"TYPE":"INT32", "VALUE":200}, 
							{"TYPE":"INT32", "VALUE":300}
						]}
					]}
				]}
			（2）使用内存描述时，用于创建Buffer的字符串，应是使用某个分隔符连接起来的一组能够转换为
			无符号8位整数的数字，低字节在前，高字节在后。如，使用空格做分隔符的“12 13 14 15 16”或
			使用“,”做分隔符的	“12,13,14,15,16”，字符串进行转换前，将进行预处理，删除首、尾的空白字符。\n
			每一组数字的表达支持二进制、八进制、十进制或十六进制，以0开头认为是8进制、以0x开头认为
			是16进制、以0b开头认为是二进制，其它认为是十进制，所有判断大小写不敏感。不同进制的数字
			可以在一个字符串中混合使用，如“11 2 013 4 0x15 6 7 8 19”。
		@attention
			Malloc函数会销毁原来的内存空间。
		*/
		xmRet Malloc(const char* aString, const char* strSeparator = xmBLANK_STRING);

		/** 销毁内存空间
		*/
		virtual void Free(void);
		
		/**	@} ************************************************************************/




		/******************************************************************************/
		/** @name 基本操作函数
		@{
		*/

		/** 判断对象是否为空
		@return 为空返回true。
		*/
		inline bool IsNULL(void) const
		{
			return (m_pBuffer == NULL);
		}
		/** 获取空间大小
		@return 空间大小。
		*/
		inline size_t Size(void) const
		{
			return m_uSize; 
		}
		/** 获取内存空间中指定偏移位置的地址指针
		@param [in] pos
			位置，默认为0。
		@return 空间指针，如果pos访问越限，返回NULL。
		*/
		inline const void* Buffer(size_t pos = 0) const
		{
			return (pos < m_uSize) ? (static_cast<const unsigned char*>(m_pBuffer) + pos) : NULL;
		}
		inline void* Buffer(size_t pos = 0)
		{
			return const_cast<void*>(static_cast<const xmBuffer&>(*this).Buffer(pos));
		}

		/** 将内存空间中指定偏移位置的地址指针转换为某类型指针
		@param [in] pos
			位置，默认为0。
		@return 空间指针，如果pos访问越限，返回NULL。
		*/
		template <typename T>
		inline const T* Pointer(size_t pos = 0) const
		{
			return (const T*)Buffer(pos);
		}
		template <typename T>
		inline T* Pointer(size_t pos = 0)
		{
			return (T*)Buffer(pos);
		}

		/** 比较函数，判断一个xmBuffer对象中某一段内存空间是否与另一段外部内存空间相等
		@param [in] pos
			xmBuffer对象位置偏移。
		@param [in] pBuffer
			外部空间地址。
		@param [in] uCompareSize
			比较长度。
		@retval true
			两者相等。
		@retval false
			不相等或访问越界。
		*/
		inline bool IsEqual(size_t pos, const void* pBuffer, size_t uCompareSize) const
		{
			if (pos + uCompareSize > Size())
			{
				return false;
			}
			return (memcmp(Buffer(pos), pBuffer, uCompareSize) == 0);
		}

		/** 比较函数，判断一个xmBuffer对象中某一段内存空间是否与另一个对象内存相等
		@param [in] pos
			xmBuffer对象位置。
		@param [in] Value
			另一个对象。
		@retval true
			两者相等。
		@retval false
			不相等或访问越界。
		*/
		template<typename T>
		inline bool IsEqual(size_t pos, const T& Value) const
		{
			return IsEqual(pos, xmTypeBuffer(Value), xmTypeSize(Value));
		}

		/** 比较函数，判断一个xmBuffer对象是否与另一个xmBuffer对象相等
		@param [in] aBuffer
			另一个xmBuffer对象。
		@return 相等返回true
		@note 
			两个xmBuffer相等是指长度相等、内容相同。
		*/
		inline bool operator==(const xmBuffer& aBuffer) const
		{
			if (Size() != aBuffer.Size())
			{
				return false;
			}
			return (memcmp(Buffer(), aBuffer.Buffer(), Size()) == 0);
		}

		/** 比较函数，判断一个xmBuffer对象是否与另一个xmBuffer对象不相等
		@see operator==()
		*/
		inline bool operator!=(const xmBuffer& Value) const
		{
			return !operator==(Value);
		}

		/** 提取函数，将一个xmBuffer对象或该对象中的某一段提取出另一个xmBuffer对象。
		@param [in] pos
			xmBuffer对象位置偏移。
		@param [in] uLength
			提取个数，为0时提取到结尾。
		@return 另一个xmBuffer对象。
		@note 
			返回的另一个xmBuffer对象与原始的xmBuffer对象相互独立，彼此操作不会相互影响。
		*/
		inline xmBuffer Extract(size_t pos, size_t uLength = 0) const
		{
			return xmBuffer((uLength == 0) ? (m_uSize - pos) : uLength, Buffer(pos));
		}

		/** 逆序函数，将一个xmBuffer对象里面的一段或全部内容逆序排列。
		@param [in] pos
			xmBuffer对象位置偏移，默认为0。
		@param [in] uLength
			逆序个数，为0时到结尾。
		@return xmBuffer对象引用。
		*/
		xmBuffer& Reverse(size_t pos = 0, size_t uLength = 0);


		/** 打印函数，将一个xmBuffer对象以字符串形式输出，输出格式为使用空格分隔的数值序列。
		@param [in] bHexadecimal
			每个数值是否以十六进制打印，true表示按照16进制打印，以方便阅读；false表示按照
			10进制打印。按照10进制打印的数值序列，可以直接作为Malloc(const char*, const char*)
			函数的输入，用于构建新的 xmBuffer 对象。默认为true。
		@param [in] bReverse
			是否逆序输出。正常顺序输出，低字节在前，逆序输出高字节在前。
		@return 字符串。
		*/
		xmString Print(bool bHexadecimal = true, bool bReverse = false) const;

		/**	@} ************************************************************************/




		/******************************************************************************/
		/** @name 数值访问函数
		@{
		*/

		/** 将一个xmBuffer对象或该对象中的某一段内存空间作为xmValue来使用
		@param [in] pos
			xmBuffer对象位置偏移。
		@param [in] aClass
			指定xmValue的类型。
		@param [in] bKeepClass
			是否由生成的xmValue对象自主保存aClass对象。
		@return 一个xmValue对象。
		@note 
			返回的xmValue对象与xmBuffer对象共用内存空间，通过xmValue对象改变的数值将反映到xmBuffer对象上。
		*/
		xmValue asValue(size_t pos, const xmClass& aClass, bool bKeepClass = false);


		/** 将一个xmBuffer对象或该对象中的某一段内存空间作为某种类型来使用
		@param [in] pos
			xmBuffer对象位置偏移，默认为0。
		@return 该类对象的引用。
		*/
		template<typename T>
		inline const T& asType(size_t pos = 0) const
		{
			return *Pointer<T>(pos);
		}
		template<typename T>
		inline T& asType(size_t pos = 0)
		{
			return *Pointer<T>(pos);
		}

		/** 取指定内存数值
		@param [in] pos
			xmBuffer对象位置偏移。
		@return 该地址数值的引用。
		*/
		inline const unsigned char& operator[](int pos) const
		{
			return asType<const unsigned char>(pos);
		}
		inline unsigned char& operator[](int pos)
		{
			return asType<unsigned char>(pos);
		}



		/** 设置全部内存中数值
		@param [in] uInitValue
			数值。
		@return 本对象引用。
		*/
		inline xmBuffer& Set(unsigned char uInitValue)
		{
			memset(m_pBuffer, uInitValue, m_uSize);
			return *this;
		}
		/** 使用一段外部内存数值设置某段或全部内存的数值
		@param [in] pos
			位置偏移。
		@param [in] pBuffer
			外部内存。
		@param [in] uBufferSize
			外部内存大小。
		@return 本对象引用。
		@attention
			本函数不进行边界检查，使用时注意不要越界。
		*/
		inline xmBuffer& Set(size_t pos, const void* pBuffer, size_t uBufferSize)
		{
			memcpy(Buffer(pos), pBuffer, uBufferSize);
			return *this;
		}
		/** 使用某个变量设置某段或全部内存的数值
		@param [in] pos
			位置偏移。
		@param [in] Value
			外部变量。
		@return 本对象引用。
		@attention
			本函数不进行边界检查，使用时注意不要越界。
		*/
		template<typename T>
		inline xmBuffer& Set(size_t pos, const T& Value)
		{
			return Set(pos, xmTypeBuffer(Value), xmTypeSize(Value));
		}


		/** 提取某段或全部内存的数值，赋值给外部内存
		@param [in] pos
			位置偏移。
		@param [in] pBuffer
			外部内存。
		@param [in] uBufferSize
			外部内存大小。
		@return 本对象引用。
		@attention
			本函数不进行边界检查，使用时注意不要越界。
		*/
		inline xmBuffer& Get(size_t pos, void* pBuffer, size_t uBufferSize)
		{
			memcpy(pBuffer, Buffer(pos), uBufferSize);
			return *this;
		}
		/** 提取某段或全部内存的数值，赋值给外部变量
		@param [in] pos
			位置偏移。
		@param [in] Value
			外部变量。
		@return 本对象引用。
		@attention
			本函数不进行边界检查，使用时注意不要越界。
		*/
		template<typename T>
		inline xmBuffer& Get(size_t pos, T& Value)
		{
			return Get(pos, xmTypeBuffer(Value), xmTypeSize(Value));
		}

		/**	@} ************************************************************************/

	protected:
		void* m_pBuffer;
		size_t m_uSize;

		//	Get Buffer&Size 获得一个指定类型变量的起始地址和长度
		//template<typename T>
		//inline static const void* __GetB(const T& Value)
		//{
		//	if (xmIsDerivedT<T, xmBuffer>::YES)			//	如果是xmBuffer或它的子类
		//	{
		//		const xmBuffer& dbValue = reinterpret_cast<const xmBuffer&>(Value);
		//		return dbValue.Buffer();
		//	}
		//	else if (xmIsDerivedT<T, std::string>::YES)	//	如果是std::string或它的子类
		//	{
		//		const std::string& strValue = reinterpret_cast<const std::string&>(Value);
		//		return xmIsTypeT<std::string>::Buffer(strValue);
		//	}
		//	//else if (xmIsDerivedT<T, xmString>::YES)	//	如果是xmString或它的子类
		//	//{
		//	//	const xmString& strValue = reinterpret_cast<const xmString&>(Value);
		//	//	return xmIsTypeT<xmString>::Buffer(strValue);
		//	//}
		//	else
		//	{
		//		return xmIsTypeT<T>::Buffer(Value);
		//	}
		//}
		//template<typename T>
		//inline static size_t __GetS(const T& Value)
		//{
		//	if (xmIsDerivedT<T, xmBuffer>::YES)			//	如果是xmBuffer或它的子类
		//	{
		//		const xmBuffer& dbValue = reinterpret_cast<const xmBuffer&>(Value);
		//		return dbValue.Size();
		//	}
		//	else if (xmIsDerivedT<T, std::string>::YES)	//	如果是std::string或它的子类
		//	{
		//		const std::string& strValue = reinterpret_cast<const std::string&>(Value);
		//		return xmIsTypeT<std::string>::Size(strValue);
		//	}
		//	//else if (xmIsDerivedT<T, xmString>::YES)	//	如果是xmString或它的子类
		//	//{
		//	//	const xmString& strValue = reinterpret_cast<const xmString&>(Value);
		//	//	return xmIsTypeT<xmString>::Size(strValue);
		//	//}
		//	else
		//	{
		//		return xmIsTypeT<T>::Size(Value);
		//	}
		//}

	private:
		//	禁止使用operator=进行赋值
		inline xmBuffer& operator=(const xmBuffer& Value)
		{
			return *this;
		}
	};

	/******************************************************************************/
	/** @class xmStaticBuffer
	@brief
		继承于xmBuffer，可以操作由外部创建的内存空间，能够改变内存数值但不能改变内存大小。
	@details
		xmStaticBuffer 存在正常（unlink）和链接（link）两种状态。正常状态下Buffer指向的
		内存空间由xmStaticBuffer创建和释放，此时与xmBuffer相同；xmStaticBuffer 可以通过 
		Link() 函数链接一个外部地址，进入链接状态，此时，Buffer指向的内存空间由其它代码创建， 
		xmStaticBuffer 可以修改其内容，被链接对象会产生同步变化。调用 Malloc() 	或 Free() 
		函数会清除链接状态，重复调用 Link() 函数会直接链接到其他地址，但上述调用均不会对被
		链接对象产生影响。
	@par 多线程安全
		否
	@par 修改记录
		@par
		2017.10.15 
			-# 代码初次完成。
	*******************************************************************************/
	class xmBASE_EXPORT xmStaticBuffer : public xmBuffer
	{
	private:
		mutable unsigned char m_uAttribute;
		enum
		{
			MY_NOTHING = 0x0,
			MY_BUFFER = 0x1
		};
		inline bool IsMyBuffer() const
		{
			return (m_uAttribute & MY_BUFFER) != 0;
		}

	public:
		/** 创建空的对象
		@see xmBuffer
		*/
		xmStaticBuffer(void) :xmBuffer(), m_uAttribute(MY_BUFFER)
		{
		}
		/** 创建指定大小的内存空间，并使用一个常量初始化
		@see xmBuffer
		*/
		xmStaticBuffer(size_t uMallocSize) : xmBuffer(uMallocSize), m_uAttribute(MY_BUFFER)
		{
		}
		/** 创建指定大小的内存空间，并使用另一段内存数据初始化
		@see xmBuffer
		*/
		xmStaticBuffer(size_t uMallocSize, const void* pBuffer) : xmBuffer(uMallocSize, pBuffer), m_uAttribute(MY_BUFFER)
		{
		}
		/** 通过复制一个xmBuffer对象创建
		@param [in] aBuffer
			被复制的另一个对象。
		*/
		xmStaticBuffer(const xmBuffer& aBuffer) : xmBuffer(aBuffer), m_uAttribute(MY_BUFFER)
		{
		}
		/** 拷贝构造函数，同时复制内容与状态
		@param [in] aBuffer
			被复制的另一个对象。
		*/
		xmStaticBuffer(const xmStaticBuffer& aBuffer) : xmBuffer(), m_uAttribute(MY_BUFFER)
		{
			if (aBuffer.IsMyBuffer())
			{
				Malloc(aBuffer.Size(), aBuffer.Buffer());
			}
			else
			{
				m_pBuffer = aBuffer.m_pBuffer;
				m_uSize = aBuffer.m_uSize;
				m_uAttribute = MY_NOTHING;
			}
		}
		/** 以链接状态构建一个对象
		@param [in] pBuffer
			被链接对象的内存地址。
		@param [in] uBufferSize
			被链接对象的长度。
		*/
		xmStaticBuffer(void* pBuffer, size_t uBufferSize) : xmBuffer(), m_uAttribute(MY_NOTHING)
		{
			Link(pBuffer, uBufferSize);
		}

		/** 析构函数
		@note
			析构时，会销毁内存空间或取消链接状态。
		*/
		virtual ~xmStaticBuffer(void)
		{
			Free();
		}

		/** 销毁内存空间或取消链接状态
		*/
		virtual void Free(void);


		/** 链接状态一个内存空间，进入链接状态
		@param [in] pBuffer
			被链接对象的内存地址。
		@param [in] uBufferSize
			被链接对象的长度。
		@note
			调用时，会销毁原来创建的内存空间或取消原来的链接对象。
		*/
		xmStaticBuffer& Link(void* pBuffer, size_t uBufferSize);

		/** 链接状态一个对象，进入链接状态
		@param [in] aParam
			被链接对象。
		@note
			调用时，会销毁原来创建的内存空间或取消原来的链接对象。
		*/
		template <typename T>
		xmStaticBuffer& Link(T& aParam)
		{
			return Link(xmIsTypeT<T>::Buffer(aParam), xmIsTypeT<T>::Size(aParam));
		}
		/** 判断是否为链接状态
		*/
		inline bool IsLinked() const
		{
			return (!IsMyBuffer());
		}
	};


	/******************************************************************************/
	/** @class xmDynamicBuffer
	@brief
		继承于xmBuffer，可以在不改变内存数值的基础上，改变内存空间的大小。
	@attention
		xmDynamicBuffer 提供了一组改变空间大小的接口，但仅能确保数据内容不被改变，存储数
		据的地址会因此发生变化。
	@par 多线程安全
		否
	@par 修改记录
		@par
		2017.10.15 
			-# 代码初次完成。
	*******************************************************************************/
	class xmBASE_EXPORT xmDynamicBuffer : public xmBuffer
	{
	public:
		/** 创建空的对象
		@see xmBuffer
		*/
		xmDynamicBuffer(void) :xmBuffer()
		{
		}
		/** 创建指定大小的内存空间，并使用一个常量初始化
		@see xmBuffer
		*/
		xmDynamicBuffer(size_t uMallocSize) : xmBuffer(uMallocSize)
		{
		}
		/** 创建指定大小的内存空间，并使用另一段内存数据初始化
		@see xmBuffer
		*/
		xmDynamicBuffer(size_t uMallocSize, const void* pBuffer) : xmBuffer(uMallocSize, pBuffer)
		{
		}
		/** 通过复制一个xmBuffer对象创建
		@param [in] aBuffer
			被复制的另一个对象。
		*/
		xmDynamicBuffer(const xmBuffer& aBuffer) : xmBuffer(aBuffer)
		{
		}
		/** 拷贝构造函数
		@param [in] aBuffer
			被复制的另一个对象。
		*/
		xmDynamicBuffer(const xmDynamicBuffer& aBuffer) : xmBuffer(aBuffer)
		{
		}
		/** 析构函数
		@see xmBuffer
		*/
		virtual ~xmDynamicBuffer(void)
		{
		}


		/** 重新分配长度，新的长度可以大于、小于或等于现有长度。如果新长度为0，则相当于调用 Free() 
		函数。如果大于现有长度，则扩充的空间使用uInitValue填充。如果小于现有长度，则截断。
		@param [in] uNewSize
			新长度。
		@param [in] uInitValue
			初始值，默认为0。
		*/
		xmDynamicBuffer& Resize(size_t uNewSize, unsigned char uInitValue = 0);


		/** 在一个xmBuffer的指定位置插入一段数据，并初始化为一个常数。
		@param [in] pos
			插入位置。
		@param [in] uBufferSize
			插入的数据长度。
		@param [in] uInitValue
			初始值，默认为0。
		@return 本对象引用。
		*/
		xmDynamicBuffer& Insert(size_t pos, size_t uBufferSize, unsigned char uInitValue);

		/** 在一个xmBuffer的指定位置插入一段数据。
		@param [in] pos
			插入位置。
		@param [in] uBufferSize
			插入的数据长度。
		@param [in] pBuffer
			插入的数据内容。
		@return 本对象引用。
		*/
		xmDynamicBuffer& Insert(size_t pos, size_t uBufferSize,  const void* pBuffer);

		/** 在一个xmBuffer的指定位置插入一个变量。
		@param [in] pos
			插入位置。
		@param [in] Value
			插入的数据内容。
		@return 本对象引用。
		*/
		template<typename T>
		inline xmDynamicBuffer& Insert(size_t pos, const T& Value)
		{
			return Insert(pos, xmTypeSize(Value), xmTypeBuffer(Value));
		}

		/******************************************************************************/
		/** @name 在前端和后端插入数据，参见 Insert() 函数
		@{
		*/
		xmDynamicBuffer& InsertFront(size_t uBufferSize, unsigned char uInitValue)
		{
			return Insert(0, uBufferSize, uInitValue);
		}
		inline xmDynamicBuffer& InsertFront(size_t uBufferSize, const void* pBuffer)
		{
			return Insert(0, uBufferSize, pBuffer);
		}
		template<typename T>
		inline xmDynamicBuffer& InsertFront(const T& Value)
		{
			return InsertFront(xmTypeSize(Value), xmTypeBuffer(Value));
		}

		xmDynamicBuffer& InsertBack(size_t uBufferSize, unsigned char uInitValue)
		{
			return Insert(Size(), uBufferSize, uInitValue);
		}
		inline xmDynamicBuffer& InsertBack(size_t uBufferSize, const void* pBuffer)
		{
			return Insert(Size(), uBufferSize, pBuffer);
		}
		template<typename T>
		inline xmDynamicBuffer& InsertBack(const T& Value)
		{
			return InsertBack(xmTypeSize(Value), xmTypeBuffer(Value));
		}
		/**	@} ************************************************************************/



		/** 在一个Buffer的指定位置（包括该位置）删除一段数据。
		@param [in] pos
			删除位置。
		@param [in] uDeleteSize
			删除的数据长度。
		@param [in] bToBack
			是否向后删除，默认是。
		@return 本对象引用。
		*/
		xmDynamicBuffer& Delete(size_t pos, size_t uDeleteSize, bool bToBack = true);

		/** 删除前端的一段数据。
		@param [in] uDeleteSize
			删除的数据长度。
		@return 本对象引用。
		*/
		inline xmDynamicBuffer& DeleteFront(size_t uDeleteSize)
		{
			return Delete(0, uDeleteSize, true);
		}
		/** 删除后端的一段数据。
		@param [in] uDeleteSize
			删除的数据长度。
		@return 本对象引用。
		*/
		inline xmDynamicBuffer& DeleteBack(size_t uDeleteSize)
		{
			return Delete(m_uSize - 1, uDeleteSize, false);
		}

		/** 将一个Buffer指定位置（包括该位置）之前的数据全部删除。
		@param [in] pos
			删除位置。
		@return 本对象引用。
		*/
		inline xmDynamicBuffer& Delete2Front(size_t pos)
		{
			return Delete(pos, pos + 1, false);
		}

		/** 将一个Buffer指定位置（包括该位置）之后的数据全部删除。
		@param [in] pos
			删除位置。
		@return 本对象引用。
		*/
		inline xmDynamicBuffer& Delete2Back(size_t pos)
		{
			return Delete(pos, m_uSize - pos, true);
		}



		/*==============================================================================
		函数名：操作符重载
		功  能：
		operator+ ：用于连接两个xmBuffer，将右值连接到左值的末尾
		operator+=：连接后赋值给左值
		参  数：见函数定义
		返回值：见函数说明。
		异  常：使用operator+=过程中，当xmBuffer为镜像状态时，抛出xmExOperRefused异常。
		说  明：无
		备  注：无
		--------------------------------------------------------------------------------
		示  例：无
		==============================================================================*/
		/** 将一个变量添加到Buffer后端，并生成一个新的xmDynamicBuffer。
		@param [in] Value
			被链接对象。
		@return 新的xmDynamicBuffer对象。
		*/
		template<typename T>
		inline xmDynamicBuffer operator+(const T& Value) const
		{
			xmDynamicBuffer aBuffer(*this);
			aBuffer.InsertBack(Value);
			return aBuffer;
		}

		/** 将一个变量添加到Buffer后端。
		@param [in] Value
			被链接对象。
		@return 本对象引用。
		*/
		template<typename T>
		inline xmDynamicBuffer& operator+=(const T& Value)
		{
			InsertBack(Value);
			return *this;
		}


	protected:
		xmDynamicBuffer& __Insert(size_t pos, size_t uBufferSize);
	};



	/******************************************************************************/
	/** @class xmIsTypeT<xmBuffer>、xmIsTypeT<xmStaticBuffer>、xmIsTypeT<xmDynamicBuffer>
	@brief
		通过对xmIsTypeT模板进行专门化，实现对xmBuffer系列类的RTTI。
	@par 多线程安全
		不适用
	@note
		xmBuffer被认为是xmDT_BUFFER类型。
	@see xmIsTypeT
	@par 修改记录
		@par
		2017.09.28
			-# 代码初次完成。
	*******************************************************************************/
	template <>
	class xmIsTypeT<xmBuffer>
	{
	public:
		static const xmEDataType TYPE = xmDT_BUFFER;
		static size_t Size(const xmBuffer& aBuffer)
		{
			return aBuffer.Size();
		}
		static const void* Buffer(const xmBuffer& aBuffer)
		{
			return aBuffer.Buffer();
		}
		static void* Buffer(xmBuffer& aBuffer)
		{
			return aBuffer.Buffer();
		}
	};
	template <>
	class xmIsTypeT<xmStaticBuffer>
	{
	public:
		static const xmEDataType TYPE = xmDT_BUFFER;
		static size_t Size(const xmStaticBuffer& aBuffer)
		{
			return aBuffer.Size();
		}
		static const void* Buffer(const xmStaticBuffer& aBuffer)
		{
			return aBuffer.Buffer();
		}
		static void* Buffer(xmStaticBuffer& aBuffer)
		{
			return aBuffer.Buffer();
		}
	};
	template <>
	class xmIsTypeT<xmDynamicBuffer>
	{
	public:
		static const xmEDataType TYPE = xmDT_BUFFER;
		static size_t Size(const xmDynamicBuffer& aBuffer)
		{
			return aBuffer.Size();
		}
		static const void* Buffer(const xmDynamicBuffer& aBuffer)
		{
			return aBuffer.Buffer();
		}
		static void* Buffer(xmDynamicBuffer& aBuffer)
		{
			return aBuffer.Buffer();
		}
	};



//#define __OPER_TYPE_DB(T)												\
//	inline xmDynamicBuffer operator+(const T& Value, const xmDynamicBuffer& aBuffer)	\
//	{																	\
//		xmDynamicBuffer newValue(Value);										\
//		newValue.Insert2Back(aBuffer);									\
//		return newValue;												\
//	}																	\
//	inline bool operator==(const T& Value, const xmDynamicBuffer& aBuffer)		\
//	{																	\
//		return (aBuffer==Value);										\
//	}																	\
//	inline bool operator!=(const T& Value, const xmDynamicBuffer& aBuffer)		\
//	{																	\
//		return (aBuffer!=Value);										\
//	}
#define __OPER_TYPE_DB(T)												\
	inline xmDynamicBuffer operator+(const T& Value, const xmDynamicBuffer& aBuffer)	\
	{																	\
		xmDynamicBuffer newValue(aBuffer);								\
		newValue.InsertFront(Value);									\
		return newValue;												\
	}

	__OPER_TYPE_DB(char);
	__OPER_TYPE_DB(unsigned char);
	__OPER_TYPE_DB(short);
	__OPER_TYPE_DB(unsigned short);
	__OPER_TYPE_DB(int);
	__OPER_TYPE_DB(unsigned int);
	__OPER_TYPE_DB(long);
	__OPER_TYPE_DB(unsigned long);
	__OPER_TYPE_DB(long long);
	__OPER_TYPE_DB(unsigned long long);
	__OPER_TYPE_DB(float);
	__OPER_TYPE_DB(double);
	__OPER_TYPE_DB(bool);
	__OPER_TYPE_DB(std::string);
#undef __OPER_TYPE_DB
}

#endif	//__SSA_BASE_BUFFER_H
