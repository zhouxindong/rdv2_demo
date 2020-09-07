#ifndef __SSA_VDR_DATACENTER_H
#define __SSA_VDR_DATACENTER_H

#include "innerobjs.h"
#include "../base/async.h"
#include "../base/rwlock.h"
#include "memorypage.h"
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
#include <shared_mutex>
#endif

namespace ssa
{
	class xmDataCenter
	{
#if(_MSC_VER >= 1900) //vs2015�����ϰ汾
		mutable std::shared_mutex     m_Mutex;
		mutable std::shared_mutex     m_LocalMutex;
#else
		mutable xmMutex     m_Mutex;
		mutable xmMutex     m_LocalMutex;
#endif

		mutable xmObjsMgr   m_ObjsMgr;

	public:
		//�������״̬,��ֵΪ0xFFFFFFFFʱ����ʾ�����
		int               m_nOfflineSycles;// = 100;

	public:
		xmDataCenter();

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
		//�������ݼ������ֽ����ڴ���
		int               BuildMemoryWithVersion(const std::string& strName, bool isLocal, bool hasMirror, bool isShare, int nVersion = 0xFFFFFFFF);
		//����������Ϣ���ڴ�������д��־��λ��
		int               ReadContentFromMirror(const std::string& strName, unsigned int nStartPos, char* pBuf , unsigned int uLength, bool isReset = true);
		int               ReadContent(const std::string& strName, unsigned int nStartPos, char* pBuf, unsigned int uLength, bool isReset = true);
		int               WriteContent(const std::string& strName, unsigned int nStartPos, char* pBuf, unsigned int uLength);

		int               ReadContentFromMirror(const std::string& strName, unsigned int nStartPos,  xmByteBuffer& bufVal, bool isReset = true);
		int               ReadContent(const std::string& strName, unsigned int nStartPos, xmByteBuffer& bufVal, bool isReset = true);
		int               WriteContent(const std::string& strName, unsigned int nStartPos, const xmByteBuffer& bufVal);

		//�������ݼ������ָ�λ���в����ĸ�������ֵ�����������������µġ�
		void              ReferenceCount(const std::string& strName, bool bMirrorMemory) const;

		//�������ݼ������ָ�λ���в����ĸ�������ֵ�����������������µġ�
		void              ReferenceCountAll(bool bMirrorMemory) const;

		//�����������,Ĭ��1000�����ں������û���յ�������źţ���Ϊ��������״̬
		void              OfflineCheck();

		bool              IsDataSetValid(const std::string& strName) const;

		//�����������
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

		//�ж������Ƿ��и���
		virtual int       IsRefreshed(const char* strName) const;
		//ͨ��ֵ��д
		int               GetValue(const char* strName, char* pBuf, unsigned int uLength);
		int               SetValue(const char* strName, const char* pBuf, unsigned int uLength, bool isWriteSign = true);

		//��·����ֵ��д
		int               GetValue(const char* strName, const char* strPath, char* pBuf, unsigned int uLength);
		int               SetValue(const char* strName, const char* strPath, const char* pBuf, unsigned int uLength ,bool isWriteSign = true);

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