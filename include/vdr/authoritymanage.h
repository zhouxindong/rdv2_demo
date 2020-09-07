#pragma once

#ifndef __SSA_VDR_AUTHORITYMANAGE_H
#define __SSA_VDR_AUTHORITYMANAGE_H

#include "defs.h"
#include "dataset.h"
#include "node.h"
#include <unordered_map>
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
#include <shared_mutex>
#endif

namespace ssa
{
	class xmAuthoritySet
	{
	public:
		bool               IsAttached(const std::string& strDataSetName);
		void               AddAttached(const xmDataSet& ds);
		void               RenewAttachedInfo(const xmDataSet& ds);
		void               RemoveAttached(const std::string& strDataSetName);
		void               ClearAttached();
		std::unordered_map<std::string, xmDataSet>&      GetAttchedDataSet();


		bool               IsAuthorized(const std::string& ndName);
		void               AddAuthorized(const xmNode& nd);
		void               RemoveAuthorized(const std::string& ndName);
		void               ClearAuthorized();
		std::unordered_map<std::string, xmNode>&         GetAuthorizedNode();

	private:
		//�����ǰ��������ָ�����Ǹ����ݼ��Ͻ��в���
		std::unordered_map<std::string, xmDataSet>      m_mapAttchedDataSet;
		std::unordered_map<std::string, xmNode>         m_mapAuthorizedNode;
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
		std::shared_mutex  m_Mutex;
#else
		xmMutex            m_Mutex;
#endif

	};

	class xmAccessPoint;
	class xmAuthorityManage
	{
	private:
		std::unordered_map<xmAccessPoint*, xmAuthoritySet*>      m_mapAuthoritySet;
		//���б����������ݼ�
		std::unordered_map<std::string, xmDataSet>               m_mapAttchedDataSet;
		std::unordered_map<std::string, xmNode>                  m_mapAuthorizedNode;

	private:
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
		std::shared_mutex  m_Mutex;
#else
		xmMutex            m_Mutex;
#endif
	public:
		friend class xmAccessPoint;

	public:
		//�ж�ϵͳ������Щ���ݼ��Ǳ������˵�
		void               Update();
		void               RenewAttachedInfo(const xmDataSet& ds);
		bool               IsAttached(const std::string& strDataSetName);
		bool               IsAuthorized(const std::string& ndName);

		std::unordered_map<xmAccessPoint*, xmAuthoritySet*>&      GetAuthoritySet();
		std::unordered_map<std::string, xmDataSet>&               GetAttchedDataSet();
		std::unordered_map<std::string, xmNode>&                  GetAuthorizedNode();

	};
}

#endif