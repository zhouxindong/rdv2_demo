#ifndef __SSA_VDR_INNEROBJS_H
#define __SSA_VDR_INNEROBJS_H

#include "export.h"
#include "data.h"
#include "dataset.h"
#include "node.h"
#include "objectregistry.h"
#include "memorypage.h"
#if(_MSC_VER >= 1900) //vs2015及以上版本
#include <shared_mutex>
#endif

namespace ssa
{
	struct xmMemoryIndex
	{
	public:
		unsigned int m_ValuePos;// = 0;
		unsigned int m_RefPos;// = 0;

		xmMemoryIndex()
		{
			m_ValuePos = 0;
			m_RefPos = 0;
		}
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
		xmDataSetWithDetail*     m_pDataSet;// = 0;
		//按照数据集合中数据排序后获取的位置，按照字节排序,在内存中的数据偏移量。
		unsigned int             m_uIndexOffset;// = 0;
		//目前只是预留，原打算用于更新状态时计数
		int                      m_nCounter;// = 0;
		//对应的内存空间的指针，为了快速检索
		xmMemoryIndex            m_MemoryIndex;
		xmMemoryPage*            m_pMainMemoryPage;// = 0;
		//本地内存，用于临时存储本身所需要发布的数据值
		xmMemoryPage*            m_pMirrorMemoryPage;// = 0;
		//xmMemoryIndex  m_MirrorMemoryTableIndex;
		//分配内存的初始地址
		//留作Value类型的指针
		void*                    m_lpValueMainAddr;// = 0;
		//留作Value类型的指针
		void*                    m_lpValueMirrorAddr;// = 0;
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
	public:
		xmDataSet                            m_DataSet;
		//数据集合的分配内存大小
		unsigned int                         m_uMemLength;// = 0;
		//是否已经分配好内存可以开始使用
		bool                                 m_bIsMainMemoryReady;
		//是否已经分配好Mirror内存可以开始使用
		bool                                 m_bIsMirrorMemoryReady;
		//用于保证DataSet初始化过程的状态，最好使用状态机。
		xmEDataSetState                      m_State;
		//是否有新写入的值
		bool                                 m_bHaveNewValue;// = false;
		//记录一共更新了多少次
		long long                            m_lRefreshCount;// = 0;
		//用于记录在线状态,0表示处于在线状态
		long long                            m_nUnActivePeriod;// = 0;
		//外部需要引用进行加锁
#if(_MSC_VER >= 1900) //vs2015及以上版本
		std::shared_mutex                    m_Mutex;
#else
		xmMutex                              m_Mutex;
#endif
		//用于区分指令信息和数据信息的分界线，可以判断出前面有几个指令信息
		unsigned int                         m_nDataZoneStartIndex[xmEQoS_MAX];// = 0;
		unsigned int                         m_nDataZoneEndIndex[xmEQoS_MAX];// = 0;
		//用于区分指令信息和数据信息在内存区域的分界线
		unsigned int                         m_nDataZoneStartMemPos[xmEQoS_MAX];// = 0;
		unsigned int                         m_nDataZoneEndMemPos[xmEQoS_MAX];// = 0;
																				//内存区间分为了不同的Qos部分，使用此参数表示更新发生在那个Qos部分，使用bit来表示。
		//typedef enum __tagQos
		//{
		//	xmEQoS_DEFAULT = 0,       /**< 使用默认传输方式。 */
		//	xmEQoS_UNRELIABLE,        /**< 使用尽量快的传输方式，不保证完全可靠。 */
		//	xmEQoS_RELIABLE,          /**< 使用可靠传输方式。 */
		//}xmEQos;
		//此处还会为未来的Qos做预留，目前最多支持64个类型的Qos模式
		long long                            m_bitNewValueSign;// = xmENewValuePart::xmEVP_DEFAULT;
	private:
		typedef xmObjectRegistry<xmDataWithDetail, std::string>      DataWithDetailRegistry;
		typedef xmObjectRegistry<xmDataWithDetail, unsigned int>     DataWithDetail2IntRegistry;
		//包含的数据按照索引排列
		DataWithDetail2IntRegistry           m_mapDatasIndex;
		//用于存放数据复位标志,配合复位处理
		char*                                m_pDatasResetSign;
		DataWithDetail2IntRegistry           m_mapDatasResetSign;
		//需要在周期内复位的数值
		DataWithDetailRegistry               m_mapNeedResetDatas;
		//包含的所有的数据
		DataWithDetailRegistry               m_mapDatasInSet;
		//内存的索引结构
		xmMemoryIndex                        m_MemoryIndex;
		//主内存结构
		xmMemoryPage*                        m_pMainMemoryPage;// = 0;
		//本地内存，用于临时存储本身所需要发布的数据值
		xmMemoryPage*                        m_pMirrorMemoryPage;// = 0;

	public:
		xmDataSetWithDetail();
		~xmDataSetWithDetail();
		//获取当前的状态值
		xmEDataSetState       GetState() { return m_State; };

		//按照数据集合名字建立内存区
		int                   BuildMemory(bool isLocal, bool bWithMirror, bool isShare, int nVersion );
		unsigned int          GetMemorySize() const;
		unsigned int          GetMemoryParamSize() const;
		bool                  IsMemoryValid() const;
		void                  ClearMemoryValidSign();

		void                  AddData(const xmDataWithDetail*);
		void                  RemoveData(const std::string&);
		unsigned int          GetDataNames(std::vector<std::string>& vName);
		unsigned int          GetDataIDs(std::vector<unsigned int>& vData);
		unsigned int          GetDatas(std::vector<xmDataWithDetail*>& vData);

		int                   GetValue(unsigned int pos, char* out, unsigned int uSize);
		int                   GetMirrorValue(unsigned int pos, char* out, unsigned int uSize);
		int                   GetMirrorValueWithSign(unsigned int pos, char* out, unsigned int uSize, unsigned int nSignPos, char& sign,bool isRefCount = false);
		int                   GetMirrorValueImpl(unsigned int pos, char* out, unsigned int uSize);
		int                   SetValue(unsigned int pos, char* in, unsigned int uSize);
		int                   SetMirrorValue(unsigned int pos, char* in, unsigned int uSize);
		int                   SetMirrorValueImpl(unsigned int pos, char* in, unsigned int uSize);

		//引用计数，用于自动计算经过了几个周期,并根据要求复位。
		void                  ReferenceCount(bool bMirrorMemory = false);
		//清空所有写标志
		void                  ClearSign(bool bMirrorMemory = false);
		//立刻进行复位判断并清除标志位
		void                  ResetValueAndClearSign(bool bMirrorMemory = false);
		//同步两个内存区的值
		void                  SynSign(bool bMainMemory2MirrorMemory);
		//同步主存区到镜像区
		void                  SyncMainMemory2Mirror();

		void                  Destroy();
		void                  Clear();
		void                  UpdateNewValueCommingSign(int nUpdateValuePos);
		void                  ResetNewValueCommingSign();

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