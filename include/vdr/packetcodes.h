#pragma once
#ifndef __SSA_VDR_PACKCODES_H
#define __SSA_VDR_PACKCODES_H

namespace ssa
{
	typedef enum __tagPackCodes
	{
		xmPACK_NOTHING = 0,

		xmPACK_CONTROLTYPE_BEGIN,

		//客户端申请虚拟仓库时，首先获取一下VDRID，可以避免多个VDR服务存在时的问题。
		xmPACK_NEW_ASK,
		//客户端申请虚拟仓库时回复
		xmPACK_NEW_RSP,

		//客户端申请加入节点
		xmPACK_ADDNODE_ASK,
		//客户端收到加入节点回复
		xmPACK_ADDNODE_RSP,
		//申请删除节点
		xmPACK_REMOVENODE_ASK,
		//删除数据节点
		xmPACK_REMOVENODE_RSP,

		//请求节点连接数据集
		xmPACK_LINKNODE2DATASET_ASK,
		//客户端收到节点连接数据集
		xmPACK_LINKNODE2DATASET_RSP,

		//请求节点不连接数据集
		xmPACK_UNLINKNODE2DATASET_ASK,
		//客户端收到节点不连接数据集
		xmPACK_UNLINKNODE2DATASET_RSP,

		xmPACK_STRUCT_SYNC_ASK,
		xmPACK_STRUCT_SYNC_RSP,

		//客户端申请加入数据集合
		xmPACK_ADDDATASET_ASK,
		//客户端收到加入数据集合回复
		xmPACK_ADDDATASET_RSP,
		//客户端接收到数据集合信息
		xmPACK_TRANSDATASET,

		//客户端申请加入数据
		xmPACK_ADDDATA_ASK,
		//客户端收到加入数据回复
		xmPACK_ADDDATA_RSP,
		//客户端接收数据信息
		xmPACK_TRANSDATA,
		//多个数据在一个报文里面
		xmPACK_TRANSMULTIDATA,

		//申请删除数据集合
		xmPACK_REMOVEDATASET_ASK,
		//删除数据集合回复
		xmPACK_REMOVEDATASET_RSP,

		//申请删除数据
		xmPACK_REMOVEDATA_ASK,
		//删除数据回复
		xmPACK_REMOVEDATA_RSP,

		//客户端申请建立索引，此问必须在加入数据后，同步数据和建立内存表之前
		xmPACK_BUILDINDEX_ASK,
		//建立更新本地的索引
		xmPACK_UPDATEINDEX_ASK,
		//建立索引完成的回复
		xmPACK_BUILDINDEX_RSP,

		//客户端申请数据同步
		xmPACK_SYNCDATA_ASK,
		//通知客户端可以同步
		xmPACK_SYNCDATA_READY,
		//客户端接收到数据同步回复
		xmPACK_SYNCDATA_RSP,

		//申请建立内存表
		xmPACK_BUILDMEMORY_ASK,
		//建立内存表回复
		xmPACK_BUILDMEMORY_RSP,

		xmPACK_CONTROLTYPE_END,

		//客户端询问数据集
		xmPACK_FETCH_DATASET_ASK,
		//客户端接收到询问数据集回复
		xmPACK_FETCH_DATASET_RSP,

		xmPACK_VALTYPE_BEGIN = 100,
		//申请内存同步，通常是客户端的数据有更新，数据集合直接处理
		xmPACK_MEMORYSYNC_ASK,
		//内存从服务往客户端同步
		xmPACK_MEMORYSYNC_RSP,
		xmPACK_MEMORYSYNC_BROADCAST,

		//申请内存同步，通常是服务器需要给有所客户端发送，处理目前设计为RTI处理这部分
		xmPACK_MEMORYSYNC_RTE_ASK,
		//发往所有客户端的内存信息
		xmPACK_MEMORYSYNC_RTE_RSP,

		xmPACK_SYNCCYCLE_CALLBACK,
		xmPACK_VALTYPE_END,


		xmPACK_TIMETYPE_BEGIN = 200,
		//申请发送时钟数据
		xmPACK_TICK_ASK,
		//时钟数据请求回复
		xmPACK_TICK_RSP,
		xmPACK_TICK_TOALL,
		xmPACK_TIMETYPE_END,

		xmPACK_APP_BEGIN = 300,
		//////下面为PUB-SUB专用
		//申请数据
		xmPACK_DATASUB_ASK,
		//数据请求回复
		xmPACK_DATAPUB_RSP,
		xmPACK_BULLETIN_ASK,
		xmPACK_BULLETIN_RSP,
		xmPACK_MCAST2NODES_ASK,
		xmPACK_MCAST2NODES_RSP,
		xmPACK_APP_END,

		xmPACK_SYS_BEGIN = 1000,
		xmPACK_LOG,
		xmPACK_SYSSTATE_CHANGE_ASK,
		xmPACK_SYSSTATE_CHANGE_RSP,
		xmPACK_COMM_IS_OK_ASK,
		xmPACK_COMM_IS_OK_RSP,

		//////////以下为待用
		//xmPACK_TRANSDATABEGIN_ASK,
		//xmPACK_TRANSDATABEGIN_RSP,
		//xmPACK_TRANSDATA,
		//xmPACK_TRANSDATA_RSP,
		//xmPACK_TRANSDATAEND_ASK,
		//xmPACK_TRANSDATAEND_RSP,
		//xmPACK_SYSTEMSYN_ASK,
		//xmPACK_SYSTEMSYN_RSP,
		//xmPACK_LOGOUT_ASK,
		//xmPACK_LOGOUT_RSP,
		//xmPACK_CONTROLTYPE_END,

		//xmPACK_VALUETYPE_BEGIN,
		//xmPACK_VALUETYPE_END,

		//xmPACK_READYING,
		//xmPACK_READYING_RSP,
		//xmPACK_MAIL,
		//xmPACK_LARGE_MAIL,
		//xmPACK_CONNECT,
		//xmPACK_DATA_OPER_BEGIN,
		//xmPACK_DATA_IN,
		//xmPACK_DATA_OUT,
		//xmPACK_DATA_OPER_END,
		//xmPACK_DATA_ERROR,
		//xmPACK_SYNS_ASK,
		//xmPACK_SYNS_RSP,
		//xmPACK_LOGIN_END,
		//xmPACK_MEMBLOCK_SYN_TIMEIN,
		//xmPACK_VALUE,
		//xmPACK_READYING_MULTI,
		//xmPACK_SYNS_MULTI,
		//xmPACK_SYNS_MULTI_END,
		//xmPACK_LOGOUT_MULTI,
		//xmPACK_MEMBLOCK_SYN,
		//xmPACK_CHANGETO_REPLAY,
		//xmPACK_CHANGETO_NORMAL,
		//xmPACK_SERVER_DATA_SYN_TIMER_ASK,
		//xmPACK_SERVER_SYN_TIMER,
		//xmPACK_WORLD_TIMER_ASK,
		//xmPACK_WORLD_TIMER,
		//xmPACK_SYSSTATE_CHANGE_ASK,
		//xmPACK_SYSSTATE_CHANGE,
		xmPACK_MAX = 10000,
	}xmEPackCodes;


	typedef enum __tagAnswerCodes
	{
		xmANSW_NOTHING = 0,

		xmANSW_YES,
		xmANSW_NO,

		xmANSW_MAX,
	}xmEAnswerCodes;

	typedef enum __tagRspCodes
	{
		xmRSP_ACK_BEGIN = xmEPackCodes::xmPACK_MAX,
		xmRSP_ACK_OK,
		xmRSP_DATASET_EXIST,
		xmRSP_DATASET_NOT_EXIST,
		xmRSP_DATA_EXIST,
		xmRSP_DATA_NOT_EXIST,
		xmRSP_DATASET_REGION,
		xmRSP_DATASET_NOT_READY,
		xmRSP_REPLAY_STATE,
		xmRSP_NODE_EXIST,
		xmRSP_NODE_NOT_EXIST,
		xmRSP_BUILD_INDEX_FAILED,
		xmRSP_JOIN_DATASET_NOT_EXIST,
	}xmERspCodes;
}

#endif
