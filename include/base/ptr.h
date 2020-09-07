/******************************************************************************/
/** @file ptr.h
@brief
	实现了可以自动销毁的普通对象智能指针和数组对象智能指针。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	无
@see ssa::xmPtr
@par 修改记录
	@par
	2020.03.15
		-# 简化了代码，约束了设计，新的xmPtr只能一次赋值，赋值后不能改表指针内容。
		同时，修复了使用数组类只能指针中的内存泄漏问题。
	2014.03.03
		-# 代码初次完成。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef  __SSA_BASE_PTR_H
#define  __SSA_BASE_PTR_H
#include "define.h"
#include <list>

namespace ssa
{
	/******************************************************************************/
	/** @class __FatherPtr
	@brief
		智能指针的基类，用户不需要直接使用。
	@details
		实现了大部分智能指针的功能，仅供xmPtr类继承。
	@par 多线程安全
		否
	@see xmPtr
	*******************************************************************************/
	template <typename T>
	class __FatherPtr
	{
		//	禁止用户直接创建__FatherPtr实例
	protected:
		inline __FatherPtr() : m_pPtr(NULL), m_pList(NULL) {};
		inline __FatherPtr(T* ptr) : m_pPtr(NULL), m_pList(NULL)
		{
			_Bind(ptr, NULL);
		}
		inline __FatherPtr(const __FatherPtr<T>& ptr) : m_pPtr(NULL), m_pList(NULL)
		{
			_Bind(ptr.m_pPtr, ptr.m_pList);
		}
		inline virtual ~__FatherPtr() {};

		//	指针和数组有不同的删除方式
		inline virtual void _DeleteThis() = 0;

		T* m_pPtr;
		std::list<void*>* m_pList;



	public:
		/** 获取指针的引用计数
		@return 指针被使用次数。
		@note
			智能指针被复制时，会导致引用计数增加，销毁会导致引用计数减小，当减到0时，指向的对象会被销毁。
		*/
		inline unsigned int UserCount() const
		{
			return IsNoNULL() ? m_pList->size() : 0;
		}

		/** 判断指针是否为空
		@return 为空返回true
		*/
		inline bool IsNULL() const
		{
			return (m_pPtr == NULL);
		}
		/** 判断指针是否为非空
		@return 非空返回true
		*/
		inline bool IsNoNULL() const
		{
			return (m_pPtr != NULL);
		}

		/** 将指针设置为空
		@note
		本函数将导致指针对象自身指向为空，指向原内存空间的引用计数减一，如果应用计数
		为0，将销毁指向的数据。
		*/
		inline void Destroy()
		{
			if (IsNULL())
			{
				return;
			}
			m_pList->remove(this);
			if (m_pList->empty())
			{
				delete m_pList;
				_DeleteThis();
			}
			_ClearThis();
		}

		/** 销毁指针指向的数据，同时将所有的对该对象的引用计数清空
		*/
		inline void Clear()
		{
			if (IsNULL())
			{
				return;
			}
			m_pList->remove(this);
			std::for_each(m_pList->begin(), m_pList->end(), ClearPtr());
			delete m_pList;
			_DeleteThis();
			_ClearThis();
		}

	public:
		/** 将指针转换为基本指针类型
		*/
		inline operator T*(void)
		{
			return m_pPtr;
		}
		/** 将指针转换为静态基本指针类型
		*/
		inline operator const T*(void) const
		{
			return m_pPtr;
		}

		/** 指针的“->”运算符
		*/
		inline T* operator->()
		{
			return m_pPtr;
		}
		/** 指针的“->”运算符
		*/
		inline const T* operator->() const
		{
			return m_pPtr;
		}
		/** 指针的“.”运算符
		*/
		inline T& operator*()
		{
			return *(operator->());
		}
		/** 指针的“.”运算符
		*/
		inline const T& operator*() const
		{
			return *(operator->());
		}


		/** 相等判断
		*/
		inline bool operator==(const __FatherPtr<T>& ptr) const
		{
			return (m_pPtr == ptr.m_pPtr);
		}
		/** 相等判断
		*/
		inline bool operator==(const T* ptr) const
		{
			return (m_pPtr == ptr);
		}
		/** 不等判断
		*/
		inline bool operator!=(const __FatherPtr<T>& ptr) const
		{
			return !operator==(ptr);
		}
		/** 不等判断
		*/
		inline bool operator!=(const T* ptr) const
		{
			return !operator==(ptr);
		}
		/** 布尔判断
		*/
		inline bool operator!() const
		{
			return IsNULL();
		}

	protected:
		template <typename B>
		inline void _Bind(B* ptr, std::list<void *>* pList)
		{
			if (IsNoNULL())
			{
				Destroy();
			}
			if (ptr == NULL)
			{
				return;
			}

			m_pPtr = ptr;
			if (pList != NULL)
			{
				m_pList = pList;
			}
			else
			{
				m_pList = new std::list<void*>;
			}
			m_pList->push_back(this);
		}

		inline void _ClearThis()
		{
			m_pPtr = NULL;
			m_pList = NULL;
		}
		//	Function Object（函数对象）
		struct ClearPtr
		{
		public:
			inline void operator()(void* ptr) const
			{
				((__FatherPtr<T>*)ptr)->_ClearThis();
			}
		};
	private:
		//	不允许其他类型进行赋值操作
		//template <typename B>
		//inline __FatherPtr(B* ptr) {};
		//template <typename B>
		//inline __FatherPtr(const __FatherPtr<B>& ptr) {};
		//template <typename B>
		//inline __FatherPtr<T>& operator=(B* ptr)
		//{
		//	return *this;
		//}
		//template <typename B>
		//inline __FatherPtr<T>& operator=(const __FatherPtr<B>& ptr)
		//{
		//	return *this;
		//}
	};






	/******************************************************************************/
	/** @class xmPtr
	@brief
		普通对象的智能指针，不提供下标访问接口。
	@details
		一、构建
		@par
		@code
			//	使用New构建
			xmPtr<Class> pC1 = new Class;
			xmPtr<Class> pC2 = pC1;
			xmPtr<Class> pC3 = NULL;
			xmPtr<Class> pC4;
			pC4 = new Class;

			//	数组
			xmPtr<Class[]> pC5 = new Class[5];
			xmPtr<Class[]> pC6 = pC5;
			xmPtr<Class[]> pC7;
			Class* classArray = new Class[5];
			pC7 = classArray;
		@endcode

		二、使用
		@par
		@code
			//	xmPtr能够像普通指针那样使用
			pC1->foo();
			(*pC2).foo();
			pC4[0].foo();
			pC5[1].foo();

			pC1 == pC2;	//	true
			pC1 == pC3; //	false

			//	赋值给普通指针
			myClass* p = pC1;

			//	可以使用STL容器。
			list<xmPtr<Class> > cList;
			cList.push_back(new Class(10));
			cList.push_back(new Class(20));
			cList.push_back(new Class(30));
		@endcode

		三、销毁
		@par
			提供SetNULL和Destroy两个接口，主动销毁一个指针；
			-# SetNULL：解除调用者对内存空间的指向，如果该段内存空间还有其他xmPtr使用，则不会影响该段内存空间的存在
			-# Destroy：销毁内存空间，所有指向该内存空间的xmPtr均变为NULL。
	@par 多线程安全
		否
	@attention
		不要把不是由new创建的指针，如一个已经定义对象的地址赋值给xmPtr，如：\n
		@code
			Class aClass(10);
			xmPtr<Class> pC3 = &aClass;
		@endcode
	@attention
		类型不同的xmPtr不允许赋值，如：\n
		@code
			xmPtr<Class> pC = new Class;
			xmPtr<int> pI = new int;
			char* pChar = NULL;
			pC = pI;	//	编译错误
			pC = pChar;	//	编译错误
		@endcode
	@attention
		但是xmPtr可以通过强制类型转换接口赋值给不同类型的基本指针，如：\n
		@code
			xmPtr<int> ptr = new int;
			char* pChar = NULL;
			pChar = ptr;
		@endcode
	@attention
		同一类型的元素和数组示例化的xmPtr，SSA中认为是不同类型，同样不可以相互赋值。\n
		@code
			xmPtr<Class> pC1 = new Class;
			xmPtr<Class[]> pC2 = new Class[4];
			pC1 = pC2; //	编译错误
		@endcode
	@attention
		由xmPtr管理的指针，不要使用delete销毁，应该由xmPtr自己决定何时销毁。
	@see xmPtr<T[]>
	*******************************************************************************/
	template <typename T>
	class xmPtr : public __FatherPtr<T>
	{
		friend xmPtr<const T>;
		friend xmPtr <T[]>;
	public:
		inline xmPtr() {};
		inline xmPtr(T* ptr) : __FatherPtr(ptr) {};
		inline xmPtr(const xmPtr<T>& ptr) : __FatherPtr(dynamic_cast<const __FatherPtr<T>&>(ptr)) {};
		inline virtual ~xmPtr()	
		{
			Destroy();
		};
	protected:
		inline virtual void _DeleteThis()
		{
			delete m_pPtr;
		}

	public:
		/** 使用普通指针对智能指针赋值
		*/
		inline xmPtr<T>& operator=(T* ptr)
		{
			_Bind(ptr, NULL);
			return *this;
		}
		/** 使用另一个智能指针赋值
		*/
		inline xmPtr<T>& operator=(const xmPtr<T>& ptr)
		{
			_Bind(ptr.m_pPtr, ptr.m_pList);
			return *this;
		}
	};


	/******************************************************************************/
	/** @class xmPtr<const T>
	@brief
		长类型指针，可以由一般类型指针赋值。
	@see xmPtr
	*******************************************************************************/
	template <typename T>
	class xmPtr<const T> : public __FatherPtr<const T>
	{
	public:
		inline xmPtr() {};
		inline xmPtr(T* ptr) : __FatherPtr(ptr) {};
		inline xmPtr(const T* ptr) : __FatherPtr(ptr) {};
		inline xmPtr(const xmPtr<T>& ptr) : __FatherPtr()
		{
			_Bind(ptr.m_pPtr, ptr.m_pList);
		}
		inline xmPtr(const xmPtr<const T>& ptr) : __FatherPtr(dynamic_cast<const __FatherPtr<const T>&>(ptr)) {};
		inline virtual ~xmPtr() 
		{
			Destroy();
		}
	protected:
		inline virtual void _DeleteThis()
		{
			delete m_pPtr;
		}

	public:
		inline xmPtr<const T>& operator=(T* ptr)
		{
			_Bind(ptr, NULL);
			return *this;
		}
		inline xmPtr<const T>& operator=(const T* ptr)
		{
			_Bind(ptr, NULL);
			return *this;
		}
		inline xmPtr<const T>& operator=(const xmPtr<T>& ptr)
		{
			_Bind(ptr.m_pPtr, ptr.m_pList);
			return *this;
		}
		inline xmPtr<const T>& operator=(const xmPtr<const T>& ptr)
		{
			_Bind(ptr.m_pPtr, ptr.m_pList);
			return *this;
		}
	};

	/******************************************************************************/
	/** @class xmPtr<T[]>
	@brief
		数组对象的智能指针，提供下标和索引访问接口。
	@see xmPtr
	*******************************************************************************/
	template <typename T>
	class xmPtr<T[]> : public __FatherPtr<T>
	{
	public:
		inline xmPtr() {};
		inline xmPtr(T* ptr) : __FatherPtr(ptr) {};
		inline xmPtr(const xmPtr<T[]>& ptr) : __FatherPtr(dynamic_cast<const xmPtr<T[]>&>(ptr)) {};
		inline virtual ~xmPtr()	
		{
			Destroy();
		};
	protected:
		inline virtual void _DeleteThis()
		{
			delete[] m_pPtr;
		}

	public:
		inline xmPtr<T[]>& operator=(T* ptr)
		{
			_Bind(ptr, NULL);
			return *this;
		}
		inline xmPtr<T[]>& operator=(const xmPtr<T[]>& ptr)
		{
			_Bind(ptr.m_pPtr, ptr.m_pList);
			return *this;
		}

		/** 指针的下标运算符
		*/
		inline T& operator[](int pos)
		{
			return m_pPtr[pos];
		}
		/** 指针的下标运算符
		*/
		inline const T& operator[](int pos) const
		{
			return m_pPtr[pos];
		}

		/** 指针的正向索引运算符
		*/
		inline T* operator+(size_t pos)
		{
			return m_pPtr + pos;
		}
		/** 指针的正向索引运算符
		*/
		inline const T* operator+(size_t pos) const
		{
			return m_pPtr + pos;
		}
		/** 指针的逆向索引运算符
		*/
		inline T* operator-(size_t pos)
		{
			return m_pPtr - pos;
		}
		/** 指针的逆向索引运算符
		*/
		inline const T* operator-(size_t pos) const
		{
			return m_pPtr - pos;
		}
	};


	/******************************************************************************/
	/** @class xmPtr<void>
	@brief
		对 xmPtr<> 专门化，禁止使用void类型指针。
	@par 多线程安全
		否
	@see xmPtr
	*******************************************************************************/
	template<>
	class xmPtr<void>
	{
	private:
		inline xmPtr() {};
		inline xmPtr(void* ptr) {};
		inline xmPtr(const xmPtr<void>& ptr) {};
		inline virtual ~xmPtr() {};
	};
}

#endif