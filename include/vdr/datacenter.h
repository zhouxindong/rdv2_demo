#ifndef __SSA_VDR_DATACENTER_H
#define __SSA_VDR_DATACENTER_H

#include "innerobjs.h"
#include "../base/async.h"
#include "../base/rwlock.h"
#include "memorypage.h"
#include <shared_mutex>

namespace ssa
{
	class xmDataCenter
	{
		mutable std::shared_mutex     m_Mutex;
		mutable std::shared_mutex     m_LocalMutex;

		mutable xmObjsMgr   m_ObjsMgr;

	public:
		//检测离线状态,当值为0xFFFFFFFF时，表示不检测
		int               m_nOfflineSycles = 100;

	public:
		bool              AddNode(const xmNode& pNode);
		bool              RemoveNode(const std::string& strName);

		bool              AddDataSet(const xmDataSet& targetDataSet);
		bool              AddDataSet(const xmDataSetWithDetail* pInnerDataSet);
		bool              RemoveDataSet(const std::string& strName);

		bool              AddData(const xmData & data);
		bool              AddData(const xmDataWithDetail* pInnerData);
		bool              UpdateData(const xmDataWithDetail* pInnerData);
		bool              RemoveData(const std::string& strName);

 		bool              Prepare4DataSetBuild(const xmDataSet& targetDataSet);
		//按照数据集合名字建立内存区
		int               BuildMemoryWithVersion(const std::string& strName, bool isLocal, bool hasMirror, bool isShare, int nVersion = 0xFFFFFFFF);
		//拷贝最新信息的内存区，将写标志复位。
		int               ReadContentFromMirror(const std::string& strName, unsigned int nStartPos, char* pBuf , unsigned int uLength, bool isReset = true);
		int               ReadContent(const std::string& strName, unsigned int nStartPos, char* pBuf, unsigned int uLength, bool isReset = true);
		int               WriteContent(const std::string& strName, unsigned int nStartPos, char* pBuf, unsigned int uLength);

		int               ReadContentFromMirror(const std::string& strName, unsigned int nStartPos,  xmByteBuffer& bufVal, bool isReset = true);
		int               ReadContent(const std::string& strName, unsigned int nStartPos, xmByteBuffer& bufVal, bool isReset = true);
		int               WriteContent(const std::string& strName, unsigned int nStartPos, const xmByteBuffer& bufVal);

		//按照数据集合名字复位所有参数的更新索引值，表明参数不是最新的。
		void              ReferenceCount(const std::string& strName, bool bMirrorMemory) const;
		//复位情况处理
		void              DataReset2InitState(const std::string& strName, bool bMirrorMemory);

		//按照数据集合名字复位所有参数的更新索引值，表明参数不是最新的。
		void              ReferenceCountAll(bool bMirrorMemory) const;
		//复位情况处理
		void              DataReset2InitStateAll(bool bMirrorMemory);

		//处理离线情况,默认1000个周期后，如果还没有收到激活的信号，认为处于离线状态
		void              OfflineCheck();

		bool              IsDataSetValid(const std::string& strName) const;

		//清除数据中心
		void              Clear();
		bool              ClearDataSetData(const std::string& strName);

		xmNodeWithDetail*      GetNode(const std::string& strName);
		xmNodeWithDetail*      GetNodeByIndex(const unsigned int nIndex);
		xmDataWithDetail*      GetData(const std::string& strName);
		xmDataWithDetail*      GetDataByIndex(const unsigned int nIndex);
		xmDataSetWithDetail*   GetDataSet(const std::string& strName);
		xmDataSetWithDetail*   GetDataSet(const unsigned int nID);
		xmDataSetWithDetail*   GetDataSetByIndex(const unsigned int nIndex);
		unsigned int           GetDataSetNames(std::vector<std::string>& vName);

		//判断数据是否有更新
		virtual int       IsRefreshed(const char* strName) const;
		//通用值读写
		int               GetValue(const char* strName, char* pBuf, unsigned int uLength);
		int               SetValue(const char* strName, const char* pBuf, unsigned int uLength);

		//带路径的值读写
		int               GetValue(const char* strName, const char* strPath, char* pBuf, unsigned int uLength);
		int               SetValue(const char* strName, const char* strPath, const char* pBuf, unsigned int uLength);

		unsigned int      GetNodeCount() const;
		unsigned int      GetLatestNodeSN() const;
		unsigned int      GetDataSetCount() const;
		unsigned int      GetLatestDataSetSN() const;
		unsigned int      GetDataCount() const;
		unsigned int      GetLatestDataSN() const;

	private:
		bool              ClearDataSetDataImpl(const std::string& strName);
		bool              RemoveDataImpl(const std::string& strName);
		bool              RemoveDataSetImpl(const std::string& strName);
		bool              UpdateDataImpl(const xmDataWithDetail* pInnerData);
		bool              AddDataImpl(const xmDataWithDetail* pInnerData);

	};
}

#endif