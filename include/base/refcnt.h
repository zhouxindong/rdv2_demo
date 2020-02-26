/******************************************************************************/
/** @file refcnt.h
@brief
	定义xmRefCnt类，实现对象的引用计数，暂不开放给用户使用。
@details
	分别以基类和模板两种方式实现了引用计数类。引用计数是指一个变量被复制的次数。当一
	个变量被以拷贝构造或operator=()的方式复制给另一个变量时，我们说这个变量的引用计数加1。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	refcnt.cpp
@todo 
	
@par 修改记录
	@par
	2014.03.19
		-# 代码初次完成。
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef  __SSA_BASE_REFCNT_H
#define  __SSA_BASE_REFCNT_H
#include "define.h"
#include <list>


namespace ssa
{
	/*******************************************************************************
	名  称：xmRefCnt
	功  能：作为基类，实现引用计数功能
	说  明：xmRefCnt以基类方式实现了引用计数，所有继承于xmRefCnt的类，均具有引用计数的能力。
		   如：
			class MyRefCnt : public xmRefCnt
			{
			public:
				MyRefCnt(int nNumber):m_nNumber(nNumber){};
				MyRefCnt(const MyRefCnt& rht):xmRefCnt(rht), m_nNumber(rht.m_nNumber){};
				~MyRefCnt(){};
				inline int GetNumber() const{return m_nNumber;};
				inline void SetNumber(int nNumber){m_nNumber = nNumber;};
			private:
				int m_nNumber;
			};

			MyRefCnt rc1(1);	//	m_nNumber = 1
			MyRefCnt rc2(2);	//	m_nNumber = 2
			MyRefCnt rc3 = rc1;	//	m_nNumber = 1，RefCnt++
			MyRefCnt rc4(4);	//	m_nNumber = 4
			rc4 = rc1;			//	m_nNumber = 1，RefCnt++，MyRefCnt默认operator=
								//	操作，使得m_nNumber==1，父类的operator=操作使得
								//	RefCnt++。
			r1 == r2;			//	false，不是同一引用
			r1 == r3;			//	true，是同一引用
			r3 == r4;			//	true，是同一引用
			rc4.SetNumber(4);	//	m_nNumber = 4，此时rc4与rc1、rc3还是同一引用，但实际
								//	的数值并不相同。
			上例中，如果希望在MyRefCnt中重写operator=()，需要在函数中显式的调用父类的
			operator=()。以MyRefCnt为例实现子类的operator=()函数：
			inline MyRefCnt& operator=(const MyRefCnt& rht)
			{
				xmRefCnt::operator=(rht);	//	复制父类内容
				m_nNumber = rht.m_nNumber;	//	复制子类内容
				return *this;
			}
	备  注：此方法要求子类谨慎的编写拷贝构造函数和operator=()函数，因为父类xmRefCnt仅处理
			指针，而对实际子类增加的内容不做检查。因此可能出现子类的两个实例A、B虽然被认为
			是同一个引用，但实际内容并不相同（见上例）。解决此问题的方法，可以通过遍历所有
			的引用来进行操作同步，但更好的办法是使用xmTRefCnt。
	*******************************************************************************/
	class xmRefCnt
	{
	public:
		typedef std::list<xmRefCnt*>::iterator tRefIter;
		xmRefCnt(void) : m_pList(NULL)
		{
			AddTo(*this);
		};
		xmRefCnt(const xmRefCnt& rht) : m_pList(NULL)
		{
			AddTo(rht);
		};
		virtual ~xmRefCnt()
		{
			RemoveMe();
		};

		//	获取当前引用数
		inline size_t GetRefCnt() const 
		{
			return m_pList->size();
		}

		//	遍历
		inline tRefIter begin() const
		{
			return m_pList->begin();
		}
		inline tRefIter end() const
		{
			return m_pList->end();
		}
		inline const xmRefCnt& get(tRefIter iter) const
		{
			return **iter;
		}
		inline xmRefCnt& get(tRefIter iter)
		{
			return **iter;
		}

		//	操作符重载
		inline xmRefCnt& operator=(const xmRefCnt& rht)
		{
			AddTo(rht);
			return *this;
		}
		inline bool operator==(const xmRefCnt& rht) const
		{
			return IsSameRef(rht);
		}
		inline bool operator!=(const xmRefCnt& rht) const
		{
			return !IsSameRef(rht);
		}
	protected:
		inline const std::list<xmRefCnt*>& GetRefList() const
		{
			return *m_pList;
		}
		//	添加一个引用
		size_t AddTo(const xmRefCnt& rht)
		{
			RemoveMe();

			if (rht.m_pList == NULL)
			{
				m_pList = new std::list<xmRefCnt*>;
			}
			else if (m_pList != rht.m_pList)
			{
				m_pList = rht.m_pList;
			}
			else
			{
				return 0;
			}
			m_pList->push_back(this);
			return m_pList->size();
		}

		//	判断是否为同一个引用
		inline bool IsSameRef(const xmRefCnt& rht) const
		{
			return m_pList == rht.m_pList;
		}

	private:
		//	删除自身引用
		void RemoveMe()
		{
			if (m_pList != NULL)
			{
				m_pList->remove(this);
				if (m_pList->empty())
				{
					delete m_pList;
				}
			}
			return;
		}

		mutable std::list<xmRefCnt*>* m_pList;
	};


	/*******************************************************************************
	名  称：xmTRefCnt
	功  能：以模板的方式，实现引用计数功能
	说  明：在xmRefCnt和xmTRefCnt之间，建议使用后者。xmTRefCnt以模板方式实现了引用计数，如：
			xmTRefCnt<int> rc1(1);		//	rc1.Ref() = 1
			xmTRefCnt<int> rc2(2);		//	rc2.Ref() = 2
			xmTRefCnt<int> rc3 = rc1;	//	rc3.Ref() = 1，RefCnt++
			xmTRefCnt<int> rc4(4);		//	rc4.Ref() = 4
			rc4 = rc1;					//	rc4.Ref() = 1，RefCnt++

			r1 == r2;			//	false，不是同一引用
			r1 == r3;			//	true，是同一引用
			r3 == r4;			//	true，是同一引用
			rc4.Ref() = 4;		//	rc1、rc3、rc4为同一个引用，改变其中一个的数值，其他
								//	引用同步改变
			rc1.Ref() == 4;		//	true
			rc3.Ref() == 4;		//	true	
	备  注：要求传入的类型T具有T(void)类型的构造函数或者具有拷贝构造函数。
	*******************************************************************************/
	template<typename T>
	class xmTRefCnt : public xmRefCnt
	{
	public:
		typedef T BaseType;
		typedef xmTRefCnt<BaseType> RefType;
		xmTRefCnt(void) : m_pInstance(NULL)
		{
			m_pInstance = new BaseType;
		};
		explicit xmTRefCnt(const BaseType& rht) : xmRefCnt(), m_pInstance(NULL)
		{
			m_pInstance = new BaseType(rht);
		};
		xmTRefCnt(const xmTRefCnt& rht) : xmRefCnt(rht), m_pInstance(rht.m_pInstance)
		{
		};
		~xmTRefCnt()
		{
			FreeMe();
		};

		inline const BaseType& Ref() const
		{
			return *m_pInstance;
		}
		inline BaseType& Ref()
		{
			return *m_pInstance;
		}
		inline xmTRefCnt& operator=(const xmTRefCnt& rht)
		{
			if (*this == rht)
			{
				return *this;
			}

			FreeMe();

			xmRefCnt::operator=(rht);
			m_pInstance = rht.m_pInstance;
			return *this;
		}

		inline const RefType& get(tRefIter iter) const
		{
			return dynamic_cast<const RefType&>(xmRefCnt::get(iter));
		}
		inline RefType& get(tRefIter iter)
		{
			return const_cast<RefType&>(static_cast<const RefType&>(*this).get(iter));
		}
	private:
		BaseType* m_pInstance;
		inline void FreeMe()
		{
			if (GetRefCnt() == 1)
			{
				delete m_pInstance;
			}
		}
	};
}

#endif	//	__SSA_BASE_REFCNT_H