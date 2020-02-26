/******************************************************************************/
/** @file stringutil.h
@brief
	定义字符串并实现字符串处理工具。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	stringutil.cpp
@see ssa::xmString
@see ssa::xmStringVector
@see ssa::xmStringUtil
@par 修改记录
	@par
	2018.06.11
		-# 将xmIsTypeT对xmString的类型识别，由xmDT_BUFFER改为xmDT_STRING。
	@par
	2018.05.17
		-# 对xmStringUtil中的函数进行了输入参数检查。
	@par
	2018.01.03
		-# 重新定义了Split函数的功能，当uMaxSplits非0时，由原来的丢掉剩余字符串，改为将剩余字符串作为最后一个元素，不被拆分。
	@par
	2017.10.12 重大更改
		-# 增加了 xmString 类，并使用 xmString 重新定义了 xmStringVector 类型。
	@par
	2014.03.24
		-# 代码初次完成。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_STRINGUTIL_H
#define __SSA_BASE_STRINGUTIL_H
#include "rtti.h"
#include "ptr.h"
#include <vector>

namespace ssa
{
	/** @enum xmENumSystem
	@brief
		当需要将一个字符串转换为数字的时候，使用 xmENumSystem 来描述字符串表示数字的
		进制格式。
	*/
	typedef enum
	{
		xmDHBO = 0,	/**< 自动判断，依据：如以0开头认为是8进制、以0x开头认为是16进制、以
					     0b开头认为是二进制，其它认为是十进制，所有的比较大小写不敏感。 */
		xmDEC,		/**< 十进制 */
		xmHEX,		/**< 十六进制 */
		xmBIN,		/**< 二进制 */
		xmOCT,		/**< 八进制 */
	}xmENumSystem;

	/** @typedef xmStringVector
		定义字符串向量。
	*/
	class xmString;
	class xmStringUtil;
	class xmBASE_EXPORT xmStringVector
	{
		friend xmStringUtil;
	private:
		xmPtr<xmString>* m_pVector;
		size_t m_uSize;

		xmStringVector(const std::vector<xmString>& aVector);
		void _Clear()
		{
			if (m_pVector != NULL)
			{
				delete[] m_pVector;
				m_pVector = NULL;
				m_uSize = 0;
			}
		}
	public:
		xmStringVector() : m_pVector(NULL), m_uSize(0)
		{}
		xmStringVector(const xmStringVector& aVector) : m_pVector(NULL), m_uSize(0)
		{
			*this = aVector;
		}
		~xmStringVector()
		{
			_Clear();
		}

		const xmString& String(size_t uIndex) const;
		xmString& String(size_t uIndex)
		{
			return const_cast<xmString&>(static_cast<const xmStringVector&>(*this).String(uIndex));
		}
		size_t Size() const
		{
			return m_uSize;
		}

		xmStringVector& operator=(const xmStringVector& aVector);
		const xmString& operator[](size_t uIndex) const
		{
			return String(uIndex);
		}
		xmString& operator[](size_t uIndex)
		{
			return String(uIndex);
		}
	};



	/******************************************************************************/
	/** @class xmString
	@brief
		xmString 实现了对字符串类型的封装。
	@details
		由于STL在作为动态链接库接口方面存在的问题，因此设计了 xmString 替代 std::string
		来实现可在接口函数中传递的字符串。
	@note 
		对于xmString来说，将字符串指针为NULL和长度为0的字符串，认为是同样的空字符串，IsEmpty()
		函数返回都为true。但在使用过程中，如果调用String()函数获取字符串指针，则空字符串返回
		一个有效字符串指针，指向一个长度为0的字符串。如果使用强制类型转换(char*)操作来获取字符串
		指针，则空字符串返回一个数值为NULL的无效字符串指针。此设计主要考虑，一是打印字符串时调用
		String() 函数，这样可以避免打印NULL指针出错。二是对于设计输入为字符串指针的函数，将
		xmString对象传入，函数可以进行为NULL判断。
	@par 多线程安全
		否
	@see xmStringVector
	@see xmStringUtil
	*******************************************************************************/
	class xmBASE_EXPORT xmString
	{
	private:
		//	字符串指针，或者为sNULL，或者指向一个长度不为0的字符串。
		//	对于xmString，m_pString永远不为NULL，主要为了能够作为参数传递给其他字符串操作函数
		//	当xmString等于sNULL时，长度为0。
		char* m_pString;
		static char* const sNULL;

	public:
		/******************************************************************************/
		/** @name 构造函数与析构函数
		@{
		*/

		/** 构造空字符串
		@note 
			空字符串是指不包含任何字符的字符串，在 xmString 的实现中，空字符串的字符指针不是
			指向NULL，而是指向""，因此可以将空字符串作为参数进行传递。
		@par 示例
			@par
			@code
				xmString aString;				//	创建空字符串
				strlen(aString.String());		//	获取长度，为0
				printf("%s", aString.String());	//	打印空字符串，但是什么都不会看到
			@endcode
		*/
		xmString(void);

		/** 使用字符指针构造字符串
		@param [in] aString
			字符指针，如果为NULL，则构造空字符串。
		*/
		xmString(const char* aString);

		/** 拷贝构造函数
		@param [in] aString
			另一个字符串对象。
		*/
		xmString(const xmString& aString);

		/** 析构函数
		*/
		virtual ~xmString();
		
		/**	@} ************************************************************************/



		/******************************************************************************/
		/** @name 基本操作函数
		@{
		*/

		/** 判断字符串是否为空
		*/
		inline bool IsEmpty(void) const
		{
			return (m_pString == sNULL);
		}

		/** 获取不可修改内容的字符串指针
		*/
		inline const char* String() const
		{
			return m_pString;
		}

		/** 获取可以修改内容的字符串指针
		*/
		inline char* String(void)
		{
			return m_pString;
		}

		/** 获取字符串长度
		*/
		inline size_t Length() const
		{
			return strlen(m_pString);
		}
		//inline size_t Size() const
		//{
		//	return IsEmpty() ? 0 : (strlen(m_pString)+1);
		//}

		/** 清空字符串
		*/
		inline void Clear()
		{
			if (!IsEmpty())
			{
				delete[] m_pString;
				m_pString = sNULL;
			}
			return;
		}

		/**	@} ************************************************************************/




		/******************************************************************************/
		/** @name 以字符串指针为对象，实现运算符重载
		这组函数的设计，主要为了避免当传递字符串指针时，产生没有必要的 xmString 对象构造。
		@{
		*/

		inline xmString& operator=(const char* aString);
		inline xmString operator+(const char* aString) const;
		inline xmString& operator+=(const char* aString);
		inline bool operator==(const char* aString) const;
		inline bool operator!=(const char* aString) const
		{
			return !operator==(aString);
		}
		inline bool operator<(const char* aString) const
		{
			return __IsLessThan_L(String(), aString);
		}
		inline bool operator<=(const char* aString) const
		{
			return !__IsLessThan_R(aString, String());
		}
		inline bool operator>(const char* aString) const
		{
			return __IsLessThan_R(aString, String());
		}
		inline bool operator>=(const char* aString) const
		{
			return !__IsLessThan_L(String(), aString);
		}


		/**	@} ************************************************************************/




		/******************************************************************************/
		/** @name 以 xmString 为对象，实现运算符重载
		@{
		*/

		inline xmString& operator=(const xmString& aString)
		{
			return operator=(aString.String());
		}
		inline xmString operator+(const xmString& aString) const
		{
			return operator+(aString.String());
		}
		inline xmString operator+=(const xmString& aString)
		{
			return operator+=(aString.String());
		}
		inline bool operator==(const xmString& aString) const
		{
			return operator==(aString.String());
		}
		inline bool operator!=(const xmString& aString) const
		{
			return operator!=(aString.String());
		}
		inline bool operator<(const xmString& aString) const
		{
			return __IsLessThan_LR(String(), aString.String());
		}
		inline bool operator<=(const xmString& aString) const
		{
			return !__IsLessThan_LR(aString.String(), String());
		}
		inline bool operator>(const xmString& aString) const
		{
			return __IsLessThan_LR(aString.String(), String());
		}
		inline bool operator>=(const xmString& aString) const
		{
			return !__IsLessThan_LR(String(), aString.String());
		}


		inline const char& operator[](int pos) const
		{
			return m_pString[pos];
		}
		inline char& operator[](int pos)
		{
			return m_pString[pos];
		}
		

		inline operator char*(void)
		{
			return IsEmpty() ? NULL : String();
		}
		inline operator const char*(void) const
		{
			return IsEmpty() ? NULL : String();
		}

		/**	@} ************************************************************************/





		/******************************************************************************/
		/** @name 对 xmStringUtil 的实现
		下列函数以字符串对象自身为源，实现 xmStringUtil 提供的操作。
		@{
		*/

		/** 字符串裁剪
		@see xmStringUtil::Trim()
		*/
		xmString& Trim(const char* strTarget = xmBLANK_STRING);
		/** 左侧字符串裁剪
		@see xmStringUtil::TrimLeft()
		*/
		xmString& TrimLeft(const char* strTarget = xmBLANK_STRING);
		/** 右侧字符串裁剪
		@see xmStringUtil::TrimRight()
		*/
		xmString& TrimRight(const char* strTarget = xmBLANK_STRING);
		/** 删除指定字符
		@see xmStringUtil::EraseChar()
		*/
		xmString& EraseChar(const char* strTarget, size_t nStartPos = 0, unsigned int uMaxErase = 0);
		/** 删除指定字符串
		@see xmStringUtil::EraseString()
		*/
		xmString& EraseString(const char* strTarget, size_t nStartPos = 0, unsigned int uMaxErase = 0);
		/** 插入字符串
		@see xmStringUtil::Insert()
		*/
		xmString& Insert(size_t nPos, const char* strInsertWith);
		/** 删除字符
		@see xmStringUtil::Delete()
		*/
		xmString& Delete(size_t nPos, size_t nCount = 0);
		/** 英文字符转换为小写
		@see xmStringUtil::ToLowerCase()
		*/
		xmString& ToLowerCase();
		/** 英文字符转换为大写
		@see xmStringUtil::ToUpperCase()
		*/
		xmString& ToUpperCase();
		/** 字符串替换
		@see xmStringUtil::Replace()
		*/
		xmString& Replace(const char* strReplaceTarget, const char* strReplaceWith, size_t nStartPos = 0, unsigned int uMaxReplace = 0);
		/** 将编码转换为GB2312
		@see xmStringUtil::UTF8ToGB2312()
		*/
		xmString& UTF8ToGB2312();
		/** 将编码转换为UTF8
		@see xmStringUtil::GB2312ToUTF8()
		*/
		xmString& GB2312ToUTF8();
		/** 格式化字符串
		@see xmStringUtil::Format()
		*/
		xmString& Format(const char* strFormat, ...);

		/** 提取出一个子字符串
		@see xmStringUtil::SubString()
		*/
		xmString SubString(size_t nStartPos, size_t nCount = 0) const;
		/** 字符串拆分
		@see xmStringUtil::Split()
		*/
		xmStringVector Split(const char* strSeparator = xmBLANK_STRING, bool bCompress = true, unsigned int uMaxSplits = 0) const;
		/** 判断是否由某个字符串开始
		@see xmStringUtil::IsStartWith()
		*/
		bool IsStartWith(const char* strPattern, bool bCaseSensitive = true) const;
		/** 判断是否由某个字符串结束
		@see xmStringUtil::IsEndWith()
		*/
		bool IsEndWith(const char* strPattern, bool bCaseSensitive = true) const;
		/** 查找字符
		@see xmStringUtil::FindChar()
		*/
		size_t FindChar(const char* strPattern, bool bCaseSensitive = true, size_t nStartPos = 0) const;
		/** 查找字符串
		@see xmStringUtil::FindString()
		*/
		size_t FindString(const char* strPattern, bool bCaseSensitive = true, size_t nStartPos = 0) const;
		/** 将字符转换为整数
		@see xmStringUtil::ToLong()
		*/
		long long ToLong(xmENumSystem eNumSys = xmDHBO) const;
		/** 将字符串转换为浮点数
		@see xmStringUtil::ToDouble()
		*/
		double ToDouble() const;

		/**	@} ************************************************************************/


	protected:
		//	不为NULL并且长度不为0的字符串操作
		void __MakeString(const char* aString, size_t uLength);
		static char* __ConnectString(const char* thisString, size_t thisLen, const char* otherString, size_t otherLen);
		static size_t __TestString(const char* aString);
		static bool __IsLessThan(const char* strLeft, const char* strRight);	//	左右均不是xmString
		static bool __IsLessThan_L(const char* strLeft, const char* strRight);	//	左边是xmString
		static bool __IsLessThan_R(const char* strLeft, const char* strRight);	//	右边是xmString
		static bool __IsLessThan_LR(const char* strLeft, const char* strRight);	//	左右都是xmString
	};

	/******************************************************************************/
	/** @class xmIsTypeT<xmString>
	@brief
		通过对xmIsTypeT模板进行专门化，实现对xmString的RTTI。
	@par 多线程安全
		不适用
	@see xmIsTypeT
	@par 修改记录
		@par
		2017.09.28
			-# 代码初次完成。
	*******************************************************************************/
	template <>
	class xmIsTypeT<xmString>
	{
	public:
		static const xmEDataType TYPE = xmDT_STRING;
		static size_t Size(const xmString& aString)
		{
			return aString.IsEmpty() ? 0 : (aString.Length() + 1);
		}
		static const void* Buffer(const xmString& aString)
		{
			return aString.String();
		}
		static void* Buffer(xmString& aString)
		{
			return aString.String();
		}
	};


	/******************************************************************************/
	/** @class xmStringUtil
	@brief
		提供一组静态函数，用于进行字符串处理操作。
	@see xmString
	*******************************************************************************/
	class xmBASE_EXPORT xmStringUtil
	{
	public:
		/** 判断一个字符串是否为空。
		@param [in] strSrc
			被操作的字符串指针。
		@return 为空返回true。
		@note
			字符串为空是指其指针指向NULL或者长度为0
		*/
		static bool IsEmpty(const char* strSrc)
		{
			if (strSrc == NULL)
			{
				return true;
			}
			if (strSrc[0] == 0)
			{
				return true;
			}
			return false;
		}

		/** 字符串裁剪，将字符串左侧和右侧连续的包含有指定字符的内容删除，不影响中间内容。
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] strTarget
			需要被裁剪的字符串内容，strTarget中的每一个字符均会被裁剪，默认包括空格、制表符（\\t）、
			回车（\\r）、换行（\\n）四个字符。
		@return 裁剪后的字符串对象
		@par 示例
			@par
			@code
				//	执行后aString等于"Hello World"
				xmString aString = xmStringUtil::Trim("  Hello World  ");
			@endcode
		*/
		static xmString Trim(const char* strSrc, const char* strTarget = xmBLANK_STRING);
		/** 左侧字符串裁剪
		@see xmStringUtil::Trim()
		*/
		static xmString TrimLeft(const char* strSrc, const char* strTarget = xmBLANK_STRING);
		/** 右侧字符串裁剪
		@see xmStringUtil::Trim()
		*/
		static xmString TrimRight(const char* strSrc, const char* strTarget = xmBLANK_STRING);


		/** 删除指定字符，将字符串中包含有指定字符的内容删除。
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] strTarget
			需要被删除的字符内容。
		@param [in] nStartPos
			删除的起始位置，默认为0。
		@param [in] uMaxErase
			最大删除个数，如果为0，表示不限制。
		@return 删除后的字符串对象
		@par 示例
			@par
			@code
				//	执行后aString等于"elloWorld"
				xmString aString = xmStringUtil::EraseChar("  Hello World  ", " H");
			@endcode
		*/
		static xmString EraseChar(
			const char* strSrc,
			const char* strTarget,
			size_t nStartPos = 0,
			unsigned int uMaxErase = 0);
		/** 删除指定字符串，将字符串中包含有指定字符串的内容删除。
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] strTarget
			需要被删除的字符串内容。
		@param [in] nStartPos
			删除的起始位置，默认为0。
		@param [in] uMaxErase
			最大删除个数，如果为0，表示不限制。
		@return 删除后的字符串对象
		@par 示例
			@par
			@code
				//	执行后aString等于" ello World  "
				xmString aString = xmStringUtil::EraseString("  Hello World  ", " H");
			@endcode
		*/
		static xmString EraseString(
			const char* strSrc,
			const char* strTarget,
			size_t nStartPos = 0,
			unsigned int uMaxErase = 0);

		/** 插入字符串，在指定位置插入指定字符串
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] nPos
			被插入的位置。
		@param [in] strInsertWith
			被插入的字符串。
		@return 插入后的字符串对象
		@par 示例
			@par
			@code
				//	执行后aString等于" Hi, Hello World  "
				xmString aString = xmStringUtil::Insert("  Hello World  ", 1, "Hi,");
			@endcode
		*/
		static xmString Insert(const char* strSrc, size_t nPos, const char* strInsertWith);
		/** 删除字符，在指定位置删除指定个数的字符
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] nPos
			被删除的位置（该位置的字符也将被删除）。
		@param [in] nCount
			删除个数，为0表示删除到字符串结尾。
		@return 删除后的字符串对象
		@par 示例
			@par
			@code
				//	执行后aString等于"Hello World  "
				xmString aString = xmStringUtil::Delete("  Hello World  ", 0, 2);
			@endcode
		*/
		static xmString Delete(const char* strSrc, size_t nPos, size_t nCount = 0);

		/** 英文字符转换为小写
		@param [in] strSrc
			被操作的字符串指针。
		@return 转换后的字符串对象
		*/
		static xmString ToLowerCase(const char* strSrc);
		/** 英文字符转换为大写
		@param [in] strSrc
			被操作的字符串指针。
		@return 转换后的字符串对象
		*/
		static xmString ToUpperCase(const char* strSrc);

		/** 字符串替换
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] strReplaceTarget
			被替换的内容。
		@param [in] strReplaceWith
			替换为的内容。
		@param [in] nStartPos
			开始替换位置。
		@param [in] uMaxReplace
			最大替换个数。
		@return 替换后的字符串对象
		@par 示例
			@par
			@code
				//	执行后aString等于"Hi World  "
				xmString aString = xmStringUtil::Replace("  Hello World  ", "Hello", "Hi");
			@endcode
		*/
		static xmString Replace(
			const char* strSrc,
			const char* strReplaceTarget,
			const char* strReplaceWith,
			size_t nStartPos = 0,
			unsigned int uMaxReplace = 0);


		/** 将编码转换为GB2312
		@param [in] strSrc
			被操作的字符串指针。
		@return 转换后的字符串对象
		*/
		static xmString UTF8ToGB2312(const char* strSrc);
		/** 将编码转换为UTF-8
		@param [in] strSrc
			被操作的字符串指针。
		@return 转换后的字符串对象
		*/
		static xmString GB2312ToUTF8(const char* strSrc);


		/** 格式化字符串
		@param [in] strFormat
			格式控制字符串。
		@param [in] vaArgs
			被格式化参数表。
		@return 格式化后的字符串对象
		*/
		static xmString FormatArg(const char* strFormat, const va_list& vaArgs);
		/** 格式化字符串
		@param [in] strFormat
			格式控制字符串。
		@param [in] ...
			被格式化参数表。
		@return 格式化后的字符串对象
		*/
		static xmString Format(const char* strFormat, ...);



		/** 提取出一个子字符串
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] nStartPos
			提取的起始位置。
		@param [in] nCount
			子字符串的长度，为0表示提取到结尾。
		@return 子字符串
		*/
		static xmString SubString(const char* strSrc, size_t nStartPos, size_t nCount = 0);

		/** 字符串拆分，将目标字符串按照分隔符拆分成字符串向量。
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] strSeparator
			分隔符，默认包括空格、制表符（\\t）、回车（\\r）、换行（\\n）四个字符。
		@param [in] bCompress
			是否压缩分隔，表示当遇到连续的几个分隔符时，是压缩成一个，还是分别处理。例如：
			字符串“asd,,ddd”，分隔符为“,”，如果压缩，则拆分为两个元素“asd”和“ddd”，如果
			不压缩，则拆分为三个元素：“asd”、空字符串和“ddd”。
		@param [in] uMaxSplits
			最大拆分个数，如果为0，则无限制。如果非0，则剩余字符串将作为最后一个元素，不被拆分。
			例如：字符串“aa,bb,cc,dd,ee,ff”，分隔符为“,”，uMaxSplits为3，则返回三个元素的字符串向量，
			分别为“aa”、“bb”、“cc,dd,ee,ff”。uMaxSplits并不保证返回字符串向量的个数一定等于uMaxSplits，
			上例中，如果uMaxSplits为10，则依然返回元素个数为6的字符串向量。
		@return 拆分后的字符串向量
		@attention
			此函数不会对aString进行去掉空字符的预处理，因此，如果使用 xmWHITE_STRING 作为
			分隔符，并且字符串的首或尾存在空字符，则在会产生空字符串元素。
		@par 示例
			@par
			@code
				//	执行后aVector含4个字符串，分别为空字符串、"Hello"、"World"和空字符串
				xmStringVector aVector = xmStringUtil::Split("  Hello World  ");
			@endcode
		*/
		static xmStringVector Split(
			const char* strSrc,
			const char* strSeparator = xmBLANK_STRING,
			bool bCompress = true,
			unsigned int uMaxSplits = 0);

		/** 连接两个字符串
		@param [in] str1
			被操作的字符串指针。
		@param [in] str2
			被操作的字符串指针。
		@return 新字符串
		*/
		static xmString Cat(const char* str1, const char* str2);

		/** 判断是否由某个字符串开始
		@param [in] strToTest
			被操作的字符串指针。
		@param [in] strPattern
			测试目标字符串。
		@param [in] bCaseSensitive
			是否大小写敏感。
		@return 是返回true
		*/
		static bool IsStartWith(const char* strToTest, const char* strPattern, bool bCaseSensitive = true);
		/** 判断是否由某个字符串结束
		@see xmStringUtil::IsStartWith()
		*/
		static bool IsEndWith(const char* strToTest, const char* strPattern, bool bCaseSensitive = true);



		/** 查找字符
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] strPattern
			查找目标，所有该参数里包括的字符均是查找目标。
		@param [in] bCaseSensitive
			是否大小写敏感。
		@param [in] nStartPos
			开始查找的位置。
		@return 返回第一个查找到的字符位置，未找到返回xmINVALID_POS。
		@todo 
			目前仅支持简单的字符串比较查找，将在后续版本中增加对正则表达式的支持。
		*/
		static size_t FindChar(
			const char* strSrc,
			const char* strPattern,
			bool bCaseSensitive = true,
			size_t nStartPos = 0);
		/** 查找字符串
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] strPattern
			查找目标。
		@param [in] bCaseSensitive
			是否大小写敏感。
		@param [in] nStartPos
			开始查找的位置。
		@return 返回第一个查找到的字符串位置，未找到返回xmINVALID_POS。
		@todo 
			目前仅支持简单的字符串比较查找，将在后续版本中增加对正则表达式的支持。
		*/
		static size_t FindString(
			const char* strSrc,
			const char* strPattern,
			bool bCaseSensitive = true,
			size_t nStartPos = 0);
		

		/** 将字符转换为整数，支持正负符号和不同进制表达的数据
		@param [in] strSrc
			被操作的字符串指针。
		@param [in] eNumSys
			指定字符串描述的进制。
		@return long类型的转换结果。
		@exception xmExIllegalInput
			当strSrc为NULL或字符串无法转换为整数时抛出。
		@see xmStringUtil::xmENumSystem
		*/
		static long long ToLong(const char* strSrc, xmENumSystem eNumSys = xmDHBO);

		/** 将字符串转换为浮点数
		@param [in] strSrc
			被操作的字符串指针。
		@return double类型的转换结果。
		@exception xmExIllegalInput
			当strSrc为NULL或字符串无法转换为浮点数时抛出。
		@see xmStringUtil::xmENumSystem
		*/
		static double ToDouble(const char* strSrc);



		/** 将一个数以二进制格式打印成字符串。
		@param [in] Number
			一个数字，可以为整数或浮点数。
		@param [in] bHaltByte
			是否半字节分隔，true表示每4个Bit一组，false表示以每8个Bit一组，默认为false。
		@param [in] strSeparator
			分隔符，将在每组打印输出中使用该参数描述的字符串分隔，当为""时，不分隔。
		@param [in] bReverse
			是否按字节序逆序打印。默认情况下高Byte的高Bit在前，低Byte的低Bit在后。如果为true，
			则打印出来的字符串低Byte的高Bit在前，高Byte的低Bit在后。如0x0201逆序输出为：
			“00000001 00000010”。
		@return 输出字符串。
		*/
		template<typename T>
		static xmString Num2Bin(T Number, bool bHaltByte = false, const char* strSeparator = " ", bool bReverse = false)
		{
			xmIsTypeT<T>::NumberType __Number = Number;

			//	数字反向
			if (bReverse)
			{
				T temp;
				unsigned char* pTemp = (unsigned char*)&temp;
				unsigned char* pThis = (unsigned char*)&__Number;
				for (int i=0; i<sizeof(T); i++)
				{
					pTemp[i] = pThis[sizeof(T)-1-i];
				}
				__Number = temp;
			}

			const unsigned int uByteCnt = sizeof(T);
			std::string strRet;
			unsigned char* pos = (unsigned char*)&__Number;
			for (int i=uByteCnt-1; i>=0; i--)
			{
				for (int j=7; j>=0; j--)
				{
					strRet.push_back(((pos[i]&(1<<j)) == 0) ? '0' : '1');
				}
			}

			unsigned int uHaltByte = bHaltByte ? 2: 1;	//	半字节分隔
			if (strSeparator == NULL)
			{
				strSeparator = "";
			}

			for (size_t pos=1; pos<uByteCnt*uHaltByte; pos++)
			{
				strRet.insert(pos*(8/uHaltByte) + (pos-1), strSeparator);
			}
			return strRet.c_str();
		}


		/** 将一个数以十六进制格式打印成字符串
		@param [in] Number
			一个数字，可以为整数或浮点数。
		@param [in] strSeparator
			分隔符，将在每组打印输出中使用该参数描述的字符串分隔，当为""时，不分隔。
		@param [in] bReverse
			是否按字节序逆序打印。默认高Byte在前，低Byte在后。。
		@return 输出字符串。
		*/
		template<typename T>
		static xmString Num2Byte(T Number, const char* strSeparator = " ", bool bReverse = false)
		{
			xmIsTypeT<T>::NumberType __Number = Number;

			if (strSeparator == NULL)
			{
				strSeparator = "";
			}

			//	数字反向
			if (bReverse)
			{
				T temp;
				unsigned char* pTemp = (unsigned char*)&temp;
				unsigned char* pThis = (unsigned char*)&__Number;
				for (int i=0; i<sizeof(T); i++)
				{
					pTemp[i] = pThis[sizeof(T)-1-i];
				}
				__Number = temp;
			}


			const unsigned int uByteCnt = sizeof(T);
			std::string strRet;

			//	生成字符串
			unsigned char* pos = (unsigned char*)&__Number;
			for (int i=uByteCnt-1; i>=0; i--)
			{
				strRet += Format("%02X%s", pos[i], strSeparator);
			}
			return TrimRight(strRet.c_str(), strSeparator);
		}

	private:
		/*==============================================================================
		函 数 名：UTF8ToUnicode、UnicodeToUTF8、GB2312ToUnicode、UnicodeToGB2312
		功    能：将一个字符，在UTF8、GB2312、Unicode之间转换
		参    数：
			pUTF8：2字节字符，符合UTF8编码
			pUnicode：4字节字符，符合Unicode编码
			pGB2312：4字节字符，符合GB2312编码
		返 回 值：成功返回xmE_SUCCESS
		抛出异常：无
		==============================================================================*/
		static int UTF8ToUnicode(const char* pUTF8, unsigned short* pUnicode);
		static int UnicodeToUTF8(const unsigned short* pUnicode, char* pUTF8);
		static int GB2312ToUnicode(const char *pGB2312, unsigned short* pUnicode);
		static int UnicodeToGB2312(const unsigned short* pUnicode, char *pGB2312);

	private:
		xmStringUtil();
		~xmStringUtil();
		xmStringUtil(const xmStringUtil&);
		xmStringUtil& operator=(const xmStringUtil&);
	};
}
ssa::xmString xmBASE_EXPORT operator+(const char* thisString, const ssa::xmString& otherString);
bool xmBASE_EXPORT operator==(const char* thisString, const ssa::xmString& otherString);
bool xmBASE_EXPORT operator!=(const char* thisString, const ssa::xmString& otherString);

#endif  //__SSA_BASE_STRINGUTIL_H
