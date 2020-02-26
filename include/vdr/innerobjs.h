#ifndef __SSA_VDR_INNEROBJS_H
#define __SSA_VDR_INNEROBJS_H

#include "export.h"
#include "data.h"
#include "dataset.h"
#include "node.h"
#include "objectregistry.h"
#include "memorypage.h"
#include <shared_mutex>

namespace ssa
{
	struct xmMemoryIndex
	{
	public:
		unsigned int m_ValuePos = 0;
		unsigned int m_RefPos = 0;
	};

	class xmNodeWithDetail
	{
	public:
		xmNode m_Node;
	};

	class xmDataSetWithDetail;
	class xmDataWithDetail
	{
	public:
		xmDataWithDetail();
		~xmDataWithDetail();

		void InitMainMemoryWithDescribe();
		void InitMirrorMemoryWithDescribe();
		void Destroy();

	public:
		xmData                   m_Data;
		//判断是否数据有更新
		unsigned char            m_isUpdate;
		//所属数据集合的名字
	    //char                     m_strParentDataSet[xmENameLenth::xmENL_DATASET];
		std::string              m_strParentDataSet;
		// 所属数据集合的指针
		xmDataSetWithDetail*     m_pDataSet = 0;
		//按照数据集合中数据排序后获取的位置，按照字节排序。
		unsigned int             m_uIndexOffset = 0;
		//目前只是预留，原打算用于更新状态时计数
		int                      m_nCounter = 0;
		//对应的内存空间的指针，为了快速检索
		xmMemoryIndex            m_MemoryIndex;
		xmMemoryPage*            m_pMainMemoryPage = 0;
		//本地内存，用于临时存储本身所需要发布的数据值
		xmMemoryPage*            m_pMirrorMemoryPage = 0;
		//xmMemoryIndex  m_MirrorMemoryTableIndex;
		//分配内存的初始地址
		//留作Value类型的指针
		void*                    m_lpValueMainAddr = 0;
		//留作Value类型的指针
		void*                    m_lpValueMirrorAddr = 0;
	};

	typedef enum __tagDataSetState
	{
		xmEINS_NOTHING = 0,
		xmEINS_JOINING,
		xmEINS_JOINED,
		xmEINS_DATA_SYNC,		    //	
		xmEINS_INDEX_BUILDING,		//	
		xmEINS_INDEX_BUILDED,		//	
		xmEINS_MEMORY_BUILDING,     //	
		xmEINS_MEMORY_BUILDED,      //	
		xmEINS_VALUE,	            //
		xmEINS_MAX,
	} xmEDataSetState;

	class xmDataSetWithDetail
	{
		typedef xmObjectRegistry<xmDataWithDetail, std::string>      DataWithDetailRegistry;
		typedef xmObjectRegistry<xmDataWithDetail, unsigned int>     DataWithDetail2IntRegistry;
		//包含的数据按照索引排列
		DataWithDetail2IntRegistry           m_mapDatasIndex;
		//需要在周期内复位的数值
		DataWithDetailRegistry               m_mapNeedResetDatas;
		//包含的所有的数据
		DataWithDetailRegistry               m_mapDatasInSet;

	public:
		xmDataSet                            m_DataSet;
		//数据集合的分配内存大小
		unsigned int                         m_uMemLength = 0;
		//是否已经分配好内存可以开始使用
		bool                                 m_bIsMainMemoryReady;
		//是否已经分配好Mirror内存可以开始使用
		bool                                 m_bIsMirrorMemoryReady;
		//用于保证DataSet初始化过程的状态，最好使用状态机。
		xmEDataSetState                      m_State;
		//是否有新写入的值
		bool                                 m_bHaveNewValue = false;
		//记录一共更新了多少次
		long long                            m_lRefreshCount = 0;
		//用于记录在线状态,0表示处于在线状态
		long long                            m_nUnActivePeriod = 0;

	private:		
		std::shared_mutex                   m_Mutex;
		//内存的索引结构
		xmMemoryIndex                        m_MemoryIndex;
		//主内存结构
		xmMemoryPage*                        m_pMainMemoryPage = 0;
		//本地内存，用于临时存储本身所需要发布的数据值
		xmMemoryPage*                        m_pMirrorMemoryPage = 0;

	public:
		xmDataSetWithDetail();
		~xmDataSetWithDetail();
		//获取当前的状态值
		xmEDataSetState       GetState() { return m_State; };

		//按照数据集合名字建立内存区
		int                   BuildMemory(bool isLocal, bool bWithMirror, bool isShare, int nVersion );
		unsigned int          GetMemorySize() const;
		bool                  IsMemoryValid() const;
		void                  ClearMemoryValidSign();

		void                  AddData(const xmDataWithDetail*);
		void                  RemoveData(const std::string&);
		unsigned int          GetDataNames(std::vector<std::string>& vName);
		unsigned int          GetDataIDs(std::vector<unsigned int>& vData);
		unsigned int          GetDatas(std::vector<xmDataWithDetail*>& vData);

		int                   GetValue(unsigned int pos, char* out, unsigned int uSize);
		int                   GetMirrorValue(unsigned int pos, char* out, unsigned int uSize);
		int                   SetValue(unsigned int pos, char* in, unsigned int uSize);
		int                   SetMirrorValue(unsigned int pos, char* in, unsigned int uSize);

		//引用计数，用于自动计算经过了几个周期。
		void                  ReferenceCount(bool bMirrorMemory = false);
		//复位数值，用于几个周期后，自动复位数据。
		void                  DataReset2InitState(bool bMirrorMemory,bool isNoDelay);
		//清空所有写标志
		void                  ClearSign(bool bMirrorMemory = false);
		void                  SynSign(bool bMainMemory2MirrorMemory);
		void                  SyncMainMemory2Mirror();

		void                  Destroy();
		void                  Clear();

	private:
		//服务器端和客户端按照数据集合建立索引
		int                   BuildIndex(bool isLocal);
		int                   BuildMainMemory(bool isShare);
		int                   BuildMirrorMemory(bool isShare);
	};

	//用于管理全局的对象
	class xmObjsMgr
	{
	public:
		typedef xmObjectRegistry<xmDataSetWithDetail>                    DataSetRegistry;
		typedef xmObjectRegistry<xmDataWithDetail>                       DataRegistry;
		typedef xmObjectRegistry<xmNodeWithDetail,std::string>           NodeRegistry;
		typedef xmObjectRegistry<xmDataSetWithDetail, unsigned int>      DataSetIntRegistry;
		typedef xmObjectRegistry<xmDataWithDetail, unsigned int>         DataIntRegistry;

		NodeRegistry       m_NodeRegistry;
		DataSetRegistry    m_DataSetRegistry;
		DataRegistry       m_DataRegistry;
		//按照ID排序的数据集合
		DataSetIntRegistry m_DataSetByIDRegistry;
		//按照ID排列的数据
		DataIntRegistry    m_DataByIDRegistry;
	};
}
#endif //__SSA_INNEROBJS_H