#pragma once
#ifndef __SSA_VDRADMIN_H
#define __SSA_VDRADMIN_H

#include "vdr.h"

namespace ssa
{
	class xmVDR_EXPORT xmVdrAdmin
	{
	public:
		xmVdrAdmin(xmVDR* pVdr);
		~xmVdrAdmin();

		//使用.号分割名字,使用xmValue获取数值
		int  GetValue(const char* strName, xmValue& bValue, const char* flag = ".");
		//使用.号分割名字
		int  SetValue(const char* strName, const xmValue& bValue, const char* flag = ".");
		//使用.号分割名字
		int  SetValue(const char* strName, const char* bValue, unsigned int uLength, const char* flag = ".");

		int  GetValue(const char* strName, char* pBuf, unsigned int uLength);
		int  SetValue(const char* strName, char* pBuf, unsigned int uLength);
		int  ConfigSystemDataSetEventHandler(xmEDataSetEvent neDataSetEvent, xmMsgHandler* pHandler = 0);
		int  ConfigSystemNodeEventHandler(xmENodeEvent neNodeEvent, xmMsgHandler* pHandler = 0);
		bool RemoveDataSet(const std::vector<std::string>& strNames);
		int  SyncContent(const char* strDataSetName, xmESyncDirection am);
		bool SyncMainMemory2Mirror(const char* strDataSet);
		void Promote(bool isUp = true);
	private:
		xmVDR* m_pVdr;
	};
}

#endif