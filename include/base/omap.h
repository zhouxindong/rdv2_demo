/******************************************************************************/
/** @file omap.h
@brief
	定义 xmOrderMap 模板类，实现一个map表，即可以按插入顺序遍历，也可以按一般map表
	顺序遍历，即可以按位置索引，也可以用迭代器索引。
@author 邓华 dehxya@163.com
@par 所属模块
	base
@par 相关文件
	无
@see ssa::xmOrderMap
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_BASE_OMAP_H
#define __SSA_BASE_OMAP_H
#include <map>
#include <vector>
#include "errors.h"

namespace ssa
{
	/******************************************************************************/
	/** @class xmOrderMap
	@remarks 
		为了避免t_Index类型和T_KEY类型无法区分，所有函数设计中，这两个类型不能同时作为同名
		函数的输入参数。
	@par 多线程安全
		否
	@par 修改记录
		@par
			2018.12.30
			-# 代码初次完成。
	*******************************************************************************/
	template<typename T_KEY, typename T_VALUE>
	class xmOrderMap
	{
	public:
		typedef typename std::map<T_KEY, T_VALUE>::iterator t_Pos;
		typedef typename std::map<T_KEY, T_VALUE>::const_iterator t_cPos;
		typedef size_t t_Index;
		//using t_Pos = typename std::map<T_KEY, T_VALUE>::iterator;

	private:
		typedef std::map<T_KEY, T_VALUE> t_Map;
		typedef std::pair<t_Pos, bool> t_InsertResult;
		typedef std::vector<t_Pos> t_Vector;

	public:
		xmOrderMap() {};
		~xmOrderMap() {};

		//	操作接口
		inline t_Pos Add(const T_KEY& key, const T_VALUE& value)
		{
			t_InsertResult ret = m_Map.insert(std::make_pair(key, value));
			if (ret.second)
			{
				m_Vector.push_back(ret.first);
				return ret.first;
			}
			else
			{
				return End();
			}
		}
		inline bool Remove(t_cPos pos)
		{
			if (pos == INVALID_POS())
			{
				return false;
			}
			std::vector<t_Pos>::const_iterator ite = std::find(m_Vector.begin(), m_Vector.end(), pos);
			m_Vector.erase(ite);
			m_Map.erase(pos);
			return true;
		}
		inline bool Remove(const T_KEY& key)
		{
			t_Pos pos = Find(key);
			return Remove(pos);
		}
		inline void Clear()
		{
			m_Vector.clear();
			m_Map.clear();
		}
		inline t_Index Size() const
		{
			return m_Map.size();
		}
		inline bool IsEmpty() const
		{
			return m_Map.empty();
		}

		
		//	获取Pos接口
		inline t_cPos INVALID_POS() const
		{
			return m_Map.cend();
		}
		inline t_Pos INVALID_POS()
		{
			return m_Map.end();
		}

		inline t_cPos Find(const T_KEY& key) const
		{
			return m_Map.find(key);
		}
		inline t_Pos Find(const T_KEY& key)
		{
			return m_Map.find(key);
		}

		inline t_cPos Begin() const
		{
			return m_Map.cbegin();
		}
		inline t_Pos Begin()
		{
			return m_Map.begin();
		}
		inline t_cPos End() const
		{
			return m_Map.cend();
		}
		inline t_Pos End()
		{
			return m_Map.end();
		}
		inline void MoveNext(t_cPos& pos) const
		{
			return pos++;
		}
		inline void MoveNext(t_Pos& pos)
		{
			return pos++;
		}

		//	使用接口
		inline t_cPos Pos(t_Index index) const
		{
			return (index < Size()) ? m_Vector[index] : INVALID_POS();
		}
		inline t_Pos Pos(t_Index index)
		{
			return (index < Size()) ? m_Vector[index] : INVALID_POS();
		}
		inline t_Index Index(t_cPos pos) const
		{
			if (pos == INVALID_POS())
			{
				return xmINVALID_INDEX;
			}
			std::vector<t_Pos>::const_iterator ite = std::find(m_Vector.begin(), m_Vector.end(), pos);
			if (ite == m_Vector.end())
			{
				return xmINVALID_INDEX;
			}
			return std::distance(std::begin(m_Vector), ite);
		}

		//	Pos-Key匹配使用接口
		inline const T_KEY& PosKey(t_cPos pos) const
		{
			return pos->first;
		}
		//	Index-Key匹配使用接口
		inline const T_KEY& IndexKey(t_Index index) const
		{
			return PosKey(Pos(index));
		}
		
		//	Pos-Value匹配使用接口
		inline const T_VALUE& PosValue(t_cPos pos) const
		{
			return pos->second;
		}
		inline T_VALUE& PosValue(t_Pos pos)
		{
			return pos->second;
		}
		//	Index-Value匹配使用接口
		inline const T_VALUE& IndexValue(t_Index index) const
		{
			return PosValue(Pos(index));
		}
		T_VALUE& IndexValue(t_Index index)
		{
			return PosValue(Pos(index));
		}
		//	Key-Value匹配使用接口
		inline const T_VALUE& KeyValue(const T_KEY& key) const
		{
			t_cPos pos = Find(key);
			if (pos == INVALID_POS())
			{
				throw xmExIllegalInput();
			}
			return PosValue(pos);
		}
		inline T_VALUE& KeyValue(const T_KEY& key)
		{
			return const_cast<T_VALUE&>(static_cast<const xmOrderMap&>(*this).KeyValue(key));
		}
		//	Key-Value匹配使用接口
		inline const T_VALUE& operator[](const T_KEY& key) const
		{
			return KeyValue(key);
		}
		inline T_VALUE& operator[](const T_KEY& key)
		{
			return KeyValue(key);
		}

		//	导出std::map
		inline size_t ExportMap(std::map<T_KEY, T_VALUE>& outMap) const
		{
			outMap = m_Map;
			return Size();
		}
		//	导出Key的std::vector
		inline size_t ExportKeyVector(std::vector<T_KEY>& outVector) const
		{
			outVector.clear();
			for (size_t i = 0; i < Size(); i++)
			{
				outVector.push_back(IndexKey(i));
			}
			return Size();
		}
		//	导出Value的std::vector
		inline size_t ExportValueVector(std::vector<T_VALUE>& outVector) const
		{
			outVector.clear();
			for (size_t i = 0; i < Size(); i++)
			{
				outVector.push_back(IndexValue(i));
			}
			return Size();
		}

	private:
		t_Map m_Map;
		t_Vector m_Vector;
	};
}

#endif	//__SSA_BASE_OMAP_H
