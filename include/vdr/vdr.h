#ifndef __SSA_VDR_H
#define __SSA_VDR_H

#include "export.h"
#include "data.h"
#include "dataset.h"
#include "icommproxy.h"
#include "iservice.h"
#include "vdrattr.h"
#include "systemhandler.h"

#ifdef xmMODE_RELEASE
#	pragma warning(disable:4482)
#endif

namespace ssa
{
	/**
	*  VDR系统调用接口，用于系统设计使用，完成如下的工作：
	*  1.VDR结构的生成；
	*  2.系统日志消息配置；
	*  3.系统结构的维护；
	*  4.时钟消息触发；
	*  5.系统设置。
	*/
	/*返回值的定义，下面用到的返回值为int的，通常是使用下列枚举
	typedef enum __tagRspCodes
	{
	xmRSP_ACK_BEGIN = 10000, //xmEPackCodes::xmPACK_MAX,
	xmRSP_ACK_OK,            //10001  成功
	xmRSP_DATASET_EXIST,     //10002  数据集已存在
	xmRSP_DATASET_NOT_EXIST, //10003  数据集不存在
	xmRSP_DATA_EXIST,        //10004  数据已存在
	xmRSP_DATA_NOT_EXIST,    //10005  数据不存在
	xmRSP_DATASET_REGION,
	xmRSP_DATASET_NOT_READY, //10007  数据集未准备好
	xmRSP_REPLAY_STATE,
	xmRSP_NODE_EXIST,        //10009  节点已存在
	xmRSP_NODE_NOT_EXIST,    //10010  节点不存在
	xmRSP_BUILD_INDEX_FAILED,//10011  索引建立失败
	}xmERspCodes;
	*/

	/**
	*  WARNING：
	*  在创建完成后,所有可能往外部发送数据，且需要等待回应消息的接口谨慎在消息回调进程中使用,因为消息队列的嵌套等待，可能会造成消息的拥塞。
	*  如：AddNode，AddDataSet，AddData以及对应的删除等操作谨慎在与其对应的通过ConfigNodeEventHandler、ConfigDataSetEventHandler、ConfigDataEventHandler等配置的消息处理线程中调用。
	*  如：SyncContent在方向指定为xmESyncDirection::xmESD_TOCLIENT时谨慎在与其对应的ConfigMSyncHandler、ConfigDPCHandler等配置的值更新回调消息处理线程中调用。
	*/

	class xmVDR_EXPORT xmVDR
	{
	private:
		xmVDR(const xmVDR& aVDR);
		
	public:
		xmVDR(void);
		virtual ~xmVDR(void);
		/**
		* 获取当前VDRSDK的版本信息。
		* @return 版本信息，两位表示一个版本，比如：010101表示vision1.1.1，011221表示vision1.12.21。
		*/
		static  int            Version();
		/**
		* 判断当前VDR是否已准备好可以使用。
		* @return 结果，true表示成功，false表示失败。
		*/
		bool                   IsValid();
		/**
		* 使用vdr属性描述信息，创建VDR。
		* @param pVdrAttr VDR属性。
		* @param bFetchInfo,表示在创建VDR时，是否需要更新所有数据信息。
		* @param bUseNewest,新建VDR，每个VDR使用前必须新建，且指定模式。如果已经存在相同ID的VDR，是否用最新的覆盖。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           Create(xmVDRAttr* pVdrAttr, bool bFetchInfo = true, bool bUseNewest = false);
		/**
		* 销毁VDR。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           Destroy();
		/**
		* 获取VDR属性。
		* @return VDR属性指针。
		*/
		virtual xmVDRAttr*     GetVDRAttr(void);
		/**
		* 切换系统状态至新状态，模拟器中可以用于回放使用，此状态下，系统所有的数据集合均转换到新的存储区域。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            SystemState2Replay(void);
		/**
		* 系统状态切换至正常。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            SystemState2Normal(void);
		/**
		* 获取当前系统工作状态。
		* @return 系统工作状态。
		*/
		virtual xmESystemState GetSystemState(void);
		/**
		* 配置Log消息处理器，接收所有的日志消息，用户可通过它自行决定如何显示日志。
		* @param pHandler 消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigLogHandler(xmMsgHandler* pHandler);
		/**
		* 向VDR增加节点。
		* @param nd 节点信息。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            AddNode(xmNode& nd);
		/**
		* 删除VDR节点。
		* @param strNodeName 节点名字。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            RemoveNode(const char* strNodeName);
		/**
		* 获取VDR节点数目。
		* @return 当前VDR中的节点数。
		*/
		virtual unsigned int   GetNodeCount(void);
		/**
		* 按照内部索引顺序获取单个节点的描述信息。
		* @param uIndex 索引顺序，从GetNodeCount获取结果是最大值，从0开始计数。
		* @param nd 节点描述信息引用，函数返回成功时，nd表示获取的节点信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetNode(unsigned int uIndex, xmNode& nd);
		/**
		* 按照名字获取单个节点的描述信息。
		* @param strName 节点名字。
		* @param nd 节点描述信息引用，函数返回成功时，nd表示获取的节点信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetNode(const char* strNodeName, xmNode& nd);
		/**
		* 向VDR增加数据集。
		* @param nd 数据集信息。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            AddDataSet(xmDataSet& ds);
		/**
		* 删除VDR中数据集。
		* @param strDataSetName 数据集名字。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            RemoveDataSet(const char* strDataSetName);
		/**
		* 获取数据集数目。
		* @return 数据集个数。
		*/
		virtual unsigned int   GetDataSetCount(void);
		/**
		* 按照内部索引顺序获取单个数据集的描述信息。
		* @param uIndex 索引顺序，从GetDataSetCount获取结果是最大值，从0开始计数。
		* @param ds 数据集描述信息引用，函数返回成功时，ds表示获取的数据集信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetDataSet(unsigned int uIndex, xmDataSet& ds);
		/**
		* 按照名字获取单个数据集的描述信息。
		* @param strDataSetName 数据集名字。
		* @param ds 数据集描述信息引用，函数返回成功时，ds表示获取的数据集信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetDataSet(const char* strDataSetName, xmDataSet& ds);
		/**
		* 按照名字获取单个数据集的描述信息，包括数据集内部所包含的数据信息。
		* @param strDataSetName 数据集名字。
		* @param ds 数据集描述信息引用，函数返回成功时，ds表示获取的数据集信息。
		* @param vData 数据描述信息引用，函数返回成功时，vData表示数据集中当前所包含的所有数据信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetDataSet(const char* strDataSetName, xmDataSet& ds, std::vector<xmData>& vData);
		/**
		* 按照数据集名字获取数据集对应的存储空间长度，用于大块读写内存需求时使用。
		* @param strDataSetName 数据集名字。
		* @return 结果，-1表示失败，其他代码表示正确读取内存的长度。
		*/
		virtual unsigned int   GetDataSetMemoryLength(const char* strDataSetName);
		/**
		* 按照数据集名字，起始位置，和大小读取数据集内存数据。
		* @param strDataSetName 数据集名字。
		* @param nPos 读内存起始位置。
		* @param pOut 内存读操作输出缓冲地址。
		* @param nSize 内存读操作长度。
		* @return 结果，-1表示失败，其他代码表示正确读取内存的长度。
		*/
		virtual int            GetDataSetMemory(const char* strDataSetName,int nPos,char* pOut,int nSize);
		/**
		* 按照数据集名字，起始位置，和大小写数据集内存数据。
		* @param strDataSetName 数据集名字。
		* @param nPos 读内存起始位置。
		* @param pOut 内存写操作输入缓冲地址。
		* @param nSize 内存写操作长度。
		* @return 结果，-1表示失败，其他代码表示正确写内存的长度。
		*/
		virtual int            SetDataSetMemory(const char* strDataSetName, int nPos, char* pIn, int nSize);
		/**
		* 按照数据集名字，判断数据集当前是否处于活跃状态。
		* @param strDataSetName 数据集名字。
		* @return 结果，0表示处于活跃状态，其他代码表示多少个周期数据没有处于活跃状态了。
		*/
		virtual long long      CheckActiveState(const char* strDataSetName);
		/**
		* 设置判断数据集当前是否处于活跃状态的时间间隔。
		* @param nCircles 几个时钟周期没有收到新的消息，认为数据集处于离线状态,当值为0xFFFFFFFF时，表示不检测。
    	*/
		virtual void           SetUnActivePeriod(int nCircles);
		/**
		* 设置默认的Qos方式,1表示允许丢包，2表示不允许丢包，目前VDR初始化时，将默认方式置为了2（所有数据不允许丢包）。
		* @param nQos 数据传输质量。
		*/
		virtual void           SetDefaultQos(int nQos);
		/**
		* 向VDR增加数据。
		* @param data 数据信息，使用默认数据传输质量。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            AddData(xmData& data);
		/**
		* 向VDR增加数据,增加了对数据传输质量的判断，目前包括了允许丢包和不允许丢包两个类别。
		* @param data  数据信息。
		* @param nQos  数据传输质量，目前包括了不能丢信息和可以丢信息两类，1表示允许丢包（数值类、状态类），2表示不允许丢包（指令类），当指定为0时使用默认的方式处理。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            AddData(xmData& data, int nQos);
		/**
		* 删除VDR中数据。
		* @param strDataName 数据名字。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            RemoveData(const char* strDataName);
		/**
		* 获取数据数目。
		* @return 数据个数。
		*/
		virtual unsigned int   GetDataCount();
		/**
		* 按照内部索引顺序获取单个数据的描述信息。
		* @param uIndex 索引顺序，从GetDataCount获取结果是最大值，从0开始计数。
		* @param data 数据描述信息引用，函数返回成功时，data表示获取的数据信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetData(unsigned int uIndex, xmData& data);
		/**
		* 按照名字获取单个数据的描述信息。
		* @param strDataName 数据名字。
		* @param data 数据描述信息引用，函数返回成功时，data表示获取的数据信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetData(const char* strDataName, xmData& data);
		/**
		* 查找数据所属数据集合。
		* @param strDataName 数据名字。
		* @return 所属数据集的名字。
		*/
		virtual const char*    BelongTo(const char* strDataName);
		/**
		* 连接节点及数据集。
		* @param strNodeName 节点名字。
		* @param strDataSetName 数据集名字。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           Link(const char* strNodeName, const char* strDataSetName);
		/**
		* 断开节点及数据集连接。
		* @param strNodeName 节点名字。
		* @param strDataSetName 数据集名字。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           UnLink(const char* strNodeName, const char* strDataSetName);
		/**
		* 为了和以前的兼容，新增参数。重新分配数据结构，通常在加入数据后，更新服务器和本地的数据结构。
		* @param strDataSetName 数据集名字。
		* @param nEndPoint      表示某一端更新信息，0表示服务器和客户端两边都更新，1表示只有客户端更新，2表示只有服务器端更新。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            Update(const char* strDataSetName,int nEndPoint = 0);
		/**
		* 从服务器获取最新的VDR信息，更新VDR。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            FetchAllInfo();
		/**
		* 从服务器获取最新的数据集信息，更新VDR中数据集信息。
		* @param strDataSetName 数据集名字。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            FetchDSInfo(const char* strDataSetName);
		/**
		* 从服务器获取最新的数据集值，更新VDR中数据集值。
		* @param strDataSetName 数据集名字。
		* @return 结果，xmERspCodes::xmRSP_ACK_OK(10001)表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            FetchDSValue(const char* strDataSetName);

		/**
		* 判断数据是否有更新。
		* @param strDataName 数据名字。
		* @return 结果，0表示未更新，大于等于1表示更新，-1表示失败。
		*/
		virtual int            IsRefreshed(const char* strDataName);
		/**
		* 数据值同步，此处的同步是将所有已经存在的数据集进行同步，在回放状态时无效，慎用。
		* @param strDataSetName 同步的数据集名字。
		* @param syncDir 向那个方向进行同步，方向具体参见xmESyncDirection说明。
		typedef enum __tagSyncDirection
		{
			xmESD_UNKOWN = 0,	        /**< 表示无意义方向。
			xmESD_TOCLIENT,		        /**< 向某一个客户端单向。 
			xmESD_TOCLIENTS,		    /**< 向客户端们单向。
			xmESD_OUTDIRBEGIN,	        /**< 表示向外方向模式开始。 
			xmESD_TOSERVER,		        /**< 向服务器单向。 
			xmESD_DOUBLEDIRECTION,		/**< 向双向。 
			xmEAS_BROADCAST,		    /**< 直接向所有接收者广播。 
		}xmESyncDirection;
		*
		* @param bUseRTE 是否使用RTE模式，RTE是指共享内存模式。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            SyncContent(xmESyncDirection syncDir, bool bUseRTE);
		virtual int            SyncContent(const char* strDataSetName, xmESyncDirection syncDir, bool bUseRTE);

		/**
		* 触发一次时钟信号，输入参数为时钟基数，同步周期，同步周期是时钟基数的倍数，表示几个时钟会进行一次值同步。
		* @param nTickTime 时钟基数。
		* @param nSyncCycles 同步周期。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            TickSignal(int nTickTime, int nSyncCycles);

		/**
		* 获取信号的等待时间。
		* @return 等待时间，以毫秒为单位。
		*/
		long                   GetWaitTime();
		/**
		* 设置需要等待的信号的等待时间，以毫秒为单位。
		* @param nTime 信号的等待时间。
		*/
		void                   SetWaitTime(long nTime = xmMAX_WAITING_TIME);

		/**
		* 设置日志显示级别,-1表示都不显示，0表示显示0级，1表示1级，.........，显示级别大于0且越大时，显示的日志信息越全。
		* @param nlevel 日志的显示级别。
		*/
		void                   SetLogLevel(int nlevel);
		/**
		* 使用何种同步逻辑。
		* @param nProxyID 同步逻辑的ID号。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户判断是否正确。
		*/
		virtual int            ConfigSyncProxy(int nProxyID, const char* dllName = "");

		/**
		* 设置是否使用本地时钟进行仿真周期回调,系统默认使用本地时钟驱动。
		* @param bIsUseLocalTimer 参数为true时，表示使用本地时钟驱动，false表示使用系统时钟。
		*/
		void                   UseLocalTickSignal(bool bIsUseLocalTimer = true);

	private:
		friend class           xmAccessPoint;
		friend class           xmVdrAdmin;
		//更新本地的关联索引
		//数据分配空间，两种情况，一种是只需要更新与服务器一致，一种是需要服务器和客户端同时更新。
		//在一个过程中，先调用BuildMemory(,false)；SyncDatas；然后BuildMemory(,true)
		virtual int            BuildMemory(const char* strDataSetName, bool bIsLocal = false);

	protected:
		xmLogHandler           m_LogHandler;
		xmIService*            m_pService;
		xmICommProxy*          m_pCommProxy;
		xmVDRAttr*             m_pVDRAttr;
	};
}

#endif	//	__SSA_IVDR_H