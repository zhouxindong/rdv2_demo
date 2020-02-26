#pragma once
#ifndef __SSA_VDR_ORDER_H
#define __SSA_VDR_ORDER_H

#include "sensmsghandler.h"
#include "datacenter.h"

namespace ssa
{
	class xmOrder
	{
	public:
		struct DataWithVauePtr
		{
		public:
			std::string m_strName;
			char*       m_ptrValue;
			int         m_nLength;
		};
		std::vector<DataWithVauePtr>          m_vDataWithVauePtr;

	public:
		std::vector<std::string>              m_vWantSubData;
		std::vector<std::string>              m_vSubedData;
		std::vector<std::string>              m_vSubedDataSet;
		xmValue                               m_mapValue;


	public:
		void           AddSubedData(const std::string& strName);
		void           AddSubedDataSet(const std::string& strName);
		void           RemoveSubedData(const std::string& strName);
		void           RemoveSubedDataSet(const std::string& strName);

		void                      GetSubedData(std::vector<std::string>& vSub);
		std::vector<std::string>& GetSubedData();
		void                      GetSubedDataSet(std::vector<std::string>& vSub);
		std::vector<std::string>& GetSubedDataSet();
		//如果为0标识成功，大于0表明第几个参数不行。
		int                       GenerateSubedDataMap(xmDataCenter* pDateCenter,int& nLength);
	};
}

#endif //__SSA_VDR_ORDER_H
