#ifndef __SSA_VDR_OBJECTREGISTRY_H
#define __SSA_VDR_OBJECTREGISTRY_H

#include "export.h"
#include "../base/async.h"
#include <map>
#include <shared_mutex>

namespace ssa
{
	template<class T, class Key = std::string,class Mutex = std::shared_mutex>
	class xmObjectRegistry
	{
		typedef std::map<Key, T *> RegistryMapType;
		RegistryMapType m_registeredObjects;
		mutable Mutex   m_Mutex;
		unsigned int    m_nSN;
	public:

		xmObjectRegistry() { m_nSN = 0; };
		~xmObjectRegistry()
		{
			//for(typename RegistryMapType::iterator iter=m_registeredObjects.begin(); iter != m_registeredObjects.end(); ++iter)
			//	delete iter->second;
			m_registeredObjects.clear();
		}

		T* GetRegistryItem(Key key) const
		{
			std::shared_lock<std::shared_mutex> guard(m_Mutex);
			typename RegistryMapType::const_iterator iter = m_registeredObjects.find(key);
			T* p = ( iter == m_registeredObjects.end() ? 0 : iter->second );
			return p;
		}

		T* GetItemByIndex(unsigned int nIndex)
		{
			std::shared_lock<std::shared_mutex> guard(m_Mutex);
			if (nIndex >= 0 && nIndex < m_registeredObjects.size())
			{
				typename RegistryMapType::const_iterator iter = m_registeredObjects.begin();
				std::advance(iter, nIndex);
				T* p = iter->second;
				return p;
			}
			return 0;
		}

		bool InsertItem(T *obj, Key key, bool cover = false)
		{
			std::unique_lock<std::shared_mutex> guard(m_Mutex);
			typename RegistryMapType::iterator iter = m_registeredObjects.find(key);
			if( iter != m_registeredObjects.end() )
			{
				if( !cover )
				{
					return false;
				}
				T* p = iter->second;
				m_registeredObjects.erase(iter);
				delete p;
			}

			m_registeredObjects[key] = obj;
			m_nSN++;
			return true;
		}

		void RemoveItem(Key key, bool delete_object = false)
		{
			std::unique_lock<std::shared_mutex> guard(m_Mutex);
			typename RegistryMapType::iterator iter = m_registeredObjects.find(key);
			if( iter != m_registeredObjects.end() )
			{
				T* p = iter->second;
				m_registeredObjects.erase(iter);
				if (delete_object)
					delete p;
			}
		}

		void RemoveAllItems(bool delete_object = false)
		{
			std::unique_lock<std::shared_mutex> guard(m_Mutex);
			if (delete_object)
			{
				typename RegistryMapType::iterator iter = m_registeredObjects.begin();
				for (; iter != m_registeredObjects.end(); iter++)
				{
					if (iter->second)
					{
						delete iter->second;
						iter->second = 0;
					}
					//m_registeredObjects.erase(iter);
				}
			}
			m_registeredObjects.clear();
			m_nSN = 0;
		}

		bool HasItem(Key key) const
		{
			std::shared_lock<std::shared_mutex> guard(m_Mutex);
			bool b = (m_registeredObjects.find(key) != m_registeredObjects.end());
			return b;
		}

		unsigned int GetRegisteredKeys(std::vector<Key> &l) const
		{
			std::shared_lock<std::shared_mutex> guard(m_Mutex);
			unsigned int sz = l.size();
			l.resize(sz + m_registeredObjects.size());
			for(typename RegistryMapType::const_iterator iter = m_registeredObjects.begin(); iter != m_registeredObjects.end(); ++iter)
				l[sz++] = iter->first;
			return m_registeredObjects.size();
		}

		unsigned int GetRegisteredItems(std::vector<T*> &l) const
		{
			std::shared_lock<std::shared_mutex> guard(m_Mutex);
			unsigned int sz = l.size();
			l.resize(sz + m_registeredObjects.size());
			for (typename RegistryMapType::const_iterator iter = m_registeredObjects.begin(); iter != m_registeredObjects.end(); ++iter)
				l[sz++] = iter->second;
			return m_registeredObjects.size();
		}

		unsigned int GetLatestSN()
		{
			std::shared_lock<std::shared_mutex> guard(m_Mutex);
			return m_nSN;
		}

		unsigned int GetSize()
		{
			std::shared_lock<std::shared_mutex> guard(m_Mutex);
			return m_registeredObjects.size();
		}
	};
}
#endif
