/******************************************************************************/
/** @file struct.h
@brief
	定义和实现xmStruct类
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	无
@see ssa::xmClass
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_STRUCT_H
#define __SSA_BASE_STRUCT_H
#include "class.h"
#include <string>
#include <map>
#include <vector>

namespace ssa
{
	/******************************************************************************/
	/** @class xmStruct
	@brief
		用于将 xmClass 中的结构提取出来，附加额外的信息。
	@details
		xmStruct 类可在 xmClass 的结构中，附加某种类型的额外信息，与 xmClass 配合，
		描述更多的内容。每一个属性，无论是基本数据类型还是结构类型，都将被附加信息。
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
				'p5':9				\
			}";
			xmClass aClass(xmStringUtil::Replace(strJson, "\'", "\""));

			//void initInt(int& n)
			//{
			//	n = 2;
			//}
			xmStruct<int> aStrcut(aClass, initInt);
			aStrcut["p1"].Info() = 9;
			aStrcut["p3"].Info() = 8;
			aStrcut["p3.sp1"].Info() = 77;
			//	9 2 8 77
			printf("%d %d %d %d\n", aStrcut["p1"].Info(), aStrcut["p2"].Info(), aStrcut["p3"].Info(), aStrcut["p3.sp1"].Info());
			//	9 2 8 77
			printf("%d %d %d %d\n", aStrcut[0].Info(), aStrcut[1].Info(), aStrcut[2].Info(), aStrcut[2][0].Info());

			xmStruct<xmString> bStruct(aStrcut);
			xmStruct<int> cStruct(aStrcut);
			//	9 2 8 77
			printf("%d %d %d %d\n", cStruct["p1"].Info(), cStruct["p2"].Info(), cStruct["p3"].Info(), cStruct["p3.sp1"].Info());
		@endcode

	@par 多线程安全
		否
	@note
		xmStruct必须与xmClass配合使用，只能由xmClass构建，xmStruct一旦构建后，其结构不能修改，
		但附加信息的内容可以修改。
	@see xmClass
	@par 修改记录
		@par
		2017.11.15 
			-# 代码初次完成，由xmValue中提取。
	*******************************************************************************/
	template <typename T>
	class xmStruct
	{
	private:
		typedef std::pair<std::map<xmString, void*>::iterator, bool> t_mapInsertResult;

	public:
		/** @typedef t_AdditionalInfo
			定义附加在结构上的信息类型。
		*/
		typedef T t_AdditionalInfo;
		/** @typedef t_InitFunc
			定义对附加信息进行初始化的处理函数。
		*/
		typedef void(*t_InitFunc)(T&);
		/** @typedef t_PrintFunc
			定义对附加信息进行打印的处理函数。
		*/
		typedef xmString(*t_PrintFunc)(const T&);


		/** 使用xmClass对象构造xmStruct对象
		@param [in] aClass
			一个xmClass对象
		@param [in] initFunc
			可以为附加信息指定一个初始化函数，函数定义参见 xmStruct::t_InitFunc 。默认为NULL，不进行特殊的初始化处理。
		@param [in] printFunc
			可以为附加信息指定一个打印函数，函数定义参见 xmStruct::t_InitFunc 。默认为NULL，不进行特殊的初始化处理。
		*/
		xmStruct(const xmClass& aClass, t_InitFunc initFunc = NULL, t_PrintFunc printFunc = NULL) : _Print(printFunc)
		{
			if (initFunc != NULL)
			{
				initFunc(m_Info);
			}

			for (int i = 0; i < aClass.PropertyCount(); i++)
			{
				const char* strName = aClass.PropertyName(i);
				t_mapInsertResult ret = m_mMapTable.insert(std::make_pair(strName, new xmStruct<T>(*aClass.PropertyClass(i), initFunc, printFunc)));
				xmAssert(ret.second);
				m_vMapTable.push_back(ret.first);
			}
		};

		/** 使用另一个xmStruct对象构造xmStruct对象
		@param [in] aStruct
			另一个aStruct对象
		@param [in] initFunc
			可以为附加信息指定一个初始化函数，函数定义参见 xmStruct::t_InitFunc 。默认为NULL，
			不进行特殊的初始化处理。
		@param [in] printFunc
			可以为附加信息指定一个打印函数，函数定义参见 xmStruct::t_InitFunc 。默认为NULL，不进行特殊的初始化处理。
		@note
			两个xmStruct对象的模板类型不一定相同，构造后，两者仅结构相同，附加信息不同。
		*/
		template <typename B>
		xmStruct(const xmStruct<B>& aStruct, t_InitFunc initFunc = NULL, t_PrintFunc printFunc = NULL) : _Print(printFunc)
		{
			if (initFunc != NULL)
			{
				initFunc(m_Info);
			}

			for (int i = 0; i < aStruct.PropertyCount(); i++)
			{
				const char* strName = aStruct.PropertyName(i);
				t_mapInsertResult ret = m_mMapTable.insert(std::make_pair(strName, new xmStruct<T>(*aStruct.PropertyStruct(i), initFunc, printFunc)));
				xmAssert(ret.second);
				m_vMapTable.push_back(ret.first);
			}
		};

		/** 拷贝构造函数
		@param [in] aStruct
			另一个aStruct对象
		@note
			两个xmStruct对象的模板类相同，构造后，两者不仅结构相同，附加信息也相同，并使用相同的打印函数。
		*/
		xmStruct(const xmStruct<T>& aStruct) : m_Info(aStruct.Info()), _Print(aStruct._Print)
		{
			for (int i = 0; i < aStruct.m_vMapTable.size(); i++)
			{
				const char* strName = aStruct.PropertyName(i);
				t_mapInsertResult ret = m_mMapTable.insert(std::make_pair(strName, new xmStruct<T>(*aStruct.PropertyStruct(i))));
				xmAssert(ret.second);
				m_vMapTable.push_back(ret.first);
			}
		};

		/** 析构函数
		*/
		~xmStruct() 
		{
			std::map<xmString, void*>::iterator pos;
			for (pos = m_mMapTable.begin(); pos != m_mMapTable.end(); pos++)
			{
				delete (xmStruct<T>*)pos->second;
			}
		};


		/** 拷贝信息
		@param [in] aStruct
			另一个aStruct对象
		@exception xmExIllegalInput 
			当输入的xmStruct对象不符合要求时抛出。
		@note
			aStruct所具有的结构，必须与本对象相同或或者为其子集。
		@attention
			必须确认拷贝对象与被拷贝对象的结构关系，否则会出现复制一半而被中断的尴尬情况。
		*/
		xmStruct<T>& CopyInfo(const xmStruct<T>& aStruct)
		{
			Info() = aStruct.Info();
			for (size_t i = 0; i<aStruct.PropertyCount(); i++)
			{
				xmStruct<T>* pStruct = PropertyStruct(aStruct.PropertyName(i));
				if (pStruct == NULL)
				{
					throw xmExIllegalInput();
				}
				pStruct->CopyInfo(*aStruct.PropertyStruct(i));
			}
			return *this;
		}

		/** 判断两个结构是否相等。
		@param [in] aClass
			另一个数据类
		@return 相等返回true。
		@note
			两个结构相等是指其属性的名称、个数、位置和附加信息等相同。
		*/
		bool IsEqual(const xmStruct<T>& aStruct) const
		{
			if (PropertyCount() != aStruct.PropertyCount())
			{
				return false;
			}
			if (Info() != aStruct.Info())
			{
				return false;
			}
			for (size_t i = 0; i < PropertyCount(); i++)
			{
				if (!PropertyStruct(i)->IsEqual(*aStruct.PropertyStruct(i)))
				{
					return false;
				}
			}
			return true;
		}

		/** 判断两个结构或者一个结构一个类是否相似。
		@param [in] aClass
			另一个数据类
		@return 相似返回true。
		@note
			相似是指其描述的属性名称、个数、位置等相同，但附加信息可以不相同。
		*/
		bool IsSimilar(const xmClass& aClass) const
		{
			if (PropertyCount() != aClass.PropertyCount())
			{
				return false;
			}
			for (int i = 0; i < PropertyCount(); i++)
			{
				if (PropertyName(i) != aClass.PropertyName(i))
				{
					return false;
				}
				if (!PropertyStruct(i)->IsSimilar(*aClass.PropertyClass(i)))
				{
					return false;
				}
			}
			return true;
		}
		template <typename B>
		bool IsSimilar(const xmStruct<B>& aStruct) const
		{
			if (PropertyCount() != aStruct.PropertyCount())
			{
				return false;
			}
			for (size_t i = 0; i < PropertyCount(); i++)
			{
				if (!PropertyStruct(i)->IsSimilar(*aStruct.PropertyStruct(i)))
				{
					return false;
				}
			}
			return true;
		}


		/** 读取附加信息
		*/
		inline const T& Info() const
		{
			return m_Info;
		}
		/** 读取或修改附加信息
		*/
		inline T& Info()
		{
			return const_cast<T&>(static_cast<const xmStruct<T>&>(*this).Info());
		}

		/** 读取属性数
		@return 属性数，如果不是结构化数据，则返回0
		@see xmClass::PropertyCount()
		*/
		inline size_t PropertyCount() const
		{
			return m_vMapTable.size();
		}

		/** 获取第uPos个属性名称
		@param [in] uPos
			属性序号，由0开始。
		@return 属性名称字符串指针，uPos越界时返回NULL。
		@exception xmExIllegalCall
			当该对象不是结构化数据时抛出。
		@see xmClass::PropertyName()
		*/
		inline const char* PropertyName(int uPos) const
		{
			if (m_vMapTable.size() == 0)
			{
				throw xmExIllegalCall();
			}

			if (uPos >= m_vMapTable.size())
			{
				return NULL;
			}
			return m_vMapTable[uPos]->first.String();
		}

		/** 获取第uPos个属性。
		@param [in] uPos
			属性序号，由0开始。
		@return 属性类指针，uPos越界时返回NULL。
		@exception xmExIllegalCall
			当该对象不是结构化数据时抛出。
		@see xmClass::PropertyClass()
		*/
		inline const xmStruct<T>* PropertyStruct(int uPos) const
		{
			if (m_vMapTable.size() == 0)
			{
				throw xmExIllegalCall();
			}

			if (uPos >= m_vMapTable.size())
			{
				return NULL;
			}
			return (const xmStruct<T>*)m_vMapTable[uPos]->second;
		}
		/** 获取第uPos个属性。
		@param [in] uPos
			属性序号，由0开始。
		@return 属性类指针，uPos越界时返回NULL。
		@exception xmExIllegalCall
			当该对象不是结构化数据时抛出。
		@see xmClass::PropertyClass()
		*/
		inline xmStruct<T>* PropertyStruct(int uPos)
		{
			return const_cast<xmStruct<T>*>(static_cast<const xmStruct<T>&>(*this).PropertyStruct(uPos));
		}

		/** 获取指定名字的属性。
		@param [in] strName
			属性名称，可以使用“.”符号，直接获取下层级属性类对象指针。
		@return 属性类指针，当指定的属性名称不存在时返回NULL。
		@exception xmExIllegalCall
			当该对象不是结构化数据时抛出。
		@see xmClass::PropertyClass()
		*/
		inline const xmStruct<T>* PropertyStruct(const char* strName) const
		{
			if (m_vMapTable.size() == 0)
			{
				throw xmExIllegalCall();
			}

			const xmStruct<T>* pClass = this;
			xmStringVector vName = xmStringUtil::Split(strName, ".", false);
			for (size_t i = 0; (i < vName.size()) && (pClass != NULL); i++)
			{
				std::map<xmString, void*>::const_iterator pos = pClass->m_mMapTable.find(vName[i].Trim());
				if (pos == pClass->m_mMapTable.end())
				{
					return NULL;
				}
				pClass = (const xmStruct<T>*)pos->second;
			}
			return pClass;
		}
		/** 获取指定名字的属性。
		@param [in] strName
			属性名称，可以使用“.”符号，直接获取下层级属性类对象指针。
		@return 属性类指针，当指定的属性名称不存在时返回NULL。
		@exception xmExIllegalCall
			当该对象不是结构化数据时抛出。
		@see xmClass::PropertyClass()
		*/
		inline xmStruct<T>* PropertyStruct(const char* strName)
		{
			return const_cast<xmStruct<T>*>(static_cast<const xmStruct<T>&>(*this).PropertyStruct(strName));
		}


		/** 以位置下标的方式获取属性类
		@see PropertyStruct()
		*/
		inline const xmStruct<T>& operator[](int uPos) const
		{
			return *PropertyStruct(uPos);
		}
		/** 以位置下标的方式获取属性类
		@see PropertyStruct()
		*/
		inline xmStruct<T>& operator[](int uPos)
		{
			return *PropertyStruct(uPos);
		}
		/** 以名称下标的方式获取属性类
		@see PropertyStruct()
		*/
		inline const xmStruct<T>& operator[](const char* strName) const
		{
			return *PropertyStruct(strName);
		}
		/** 以名称下标的方式获取属性类
		@see PropertyStruct()
		*/
		inline xmStruct<T>& operator[](const char* strName)
		{
			return *PropertyStruct(strName);
		}


		/** 以方便阅读的方式打印xmStruct结构和信息
		*/
		inline xmString PrintInfo() const
		{
			return __Format("");
		}

	private:
		T m_Info;
		std::map<xmString, void*> m_mMapTable;
		std::vector<std::map<xmString, void*>::const_iterator> m_vMapTable;
		t_PrintFunc _Print;

		xmString __Format(const char* strHead) const
		{
			xmString strRet;
			xmString _Hear = strHead;
			_Hear += "    ";

			if (_Print != NULL)
			{
				strRet = xmStringUtil::Format("%s", _Print(Info()).String());
			}
			if (PropertyCount() == 0)
			{
				return strRet;
			}

			strRet += xmStringUtil::Format("#{\n%s", _Hear.String());
			for (size_t i = 0; i < PropertyCount(); i++)
			{
				if (i == PropertyCount() - 1)
				{
					strRet += xmStringUtil::Format("\"%s\":%s\n%s", PropertyName(i), PropertyStruct(i)->__Format(_Hear).String(), strHead);
				}
				else
				{
					strRet += xmStringUtil::Format("\"%s\":%s,\n%s", PropertyName(i), PropertyStruct(i)->__Format(_Hear).String(), _Hear.String());
				}
			}
			strRet += "}";

			return strRet;
		}
	};
}

#endif	//__SSA_BASE_STRUCT_H
