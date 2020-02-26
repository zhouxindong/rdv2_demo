#pragma once
#ifndef __SSA_VDR_PACKCODES_H
#define __SSA_VDR_PACKCODES_H

namespace ssa
{
	typedef enum __tagPackCodes
	{
		xmPACK_NOTHING = 0,

		xmPACK_CONTROLTYPE_BEGIN,

		//�ͻ�����������ֿ�ʱ�����Ȼ�ȡһ��VDRID�����Ա�����VDR�������ʱ�����⡣
		xmPACK_NEW_ASK,
		//�ͻ�����������ֿ�ʱ�ظ�
		xmPACK_NEW_RSP,

		//�ͻ����������ڵ�
		xmPACK_ADDNODE_ASK,
		//�ͻ����յ�����ڵ�ظ�
		xmPACK_ADDNODE_RSP,
		//����ɾ���ڵ�
		xmPACK_REMOVENODE_ASK,
		//ɾ�����ݽڵ�
		xmPACK_REMOVENODE_RSP,

		//����ڵ��������ݼ�
		xmPACK_LINKNODE2DATASET_ASK,
		//�ͻ����յ��ڵ��������ݼ�
		xmPACK_LINKNODE2DATASET_RSP,

		//����ڵ㲻�������ݼ�
		xmPACK_UNLINKNODE2DATASET_ASK,
		//�ͻ����յ��ڵ㲻�������ݼ�
		xmPACK_UNLINKNODE2DATASET_RSP,

		xmPACK_STRUCT_SYNC_ASK,
		xmPACK_STRUCT_SYNC_RSP,

		//�ͻ�������������ݼ���
		xmPACK_ADDDATASET_ASK,
		//�ͻ����յ��������ݼ��ϻظ�
		xmPACK_ADDDATASET_RSP,
		//�ͻ��˽��յ����ݼ�����Ϣ
		xmPACK_TRANSDATASET,

		//�ͻ��������������
		xmPACK_ADDDATA_ASK,
		//�ͻ����յ��������ݻظ�
		xmPACK_ADDDATA_RSP,
		//�ͻ��˽���������Ϣ
		xmPACK_TRANSDATA,
		//���������һ����������
		xmPACK_TRANSMULTIDATA,

		//����ɾ�����ݼ���
		xmPACK_REMOVEDATASET_ASK,
		//ɾ�����ݼ��ϻظ�
		xmPACK_REMOVEDATASET_RSP,

		//����ɾ������
		xmPACK_REMOVEDATA_ASK,
		//ɾ�����ݻظ�
		xmPACK_REMOVEDATA_RSP,

		//�ͻ������뽨�����������ʱ����ڼ������ݺ�ͬ�����ݺͽ����ڴ��֮ǰ
		xmPACK_BUILDINDEX_ASK,
		//�������±��ص�����
		xmPACK_UPDATEINDEX_ASK,
		//����������ɵĻظ�
		xmPACK_BUILDINDEX_RSP,

		//�ͻ�����������ͬ��
		xmPACK_SYNCDATA_ASK,
		//֪ͨ�ͻ��˿���ͬ��
		xmPACK_SYNCDATA_READY,
		//�ͻ��˽��յ�����ͬ���ظ�
		xmPACK_SYNCDATA_RSP,

		//���뽨���ڴ��
		xmPACK_BUILDMEMORY_ASK,
		//�����ڴ��ظ�
		xmPACK_BUILDMEMORY_RSP,

		xmPACK_CONTROLTYPE_END,

		//�ͻ���ѯ�����ݼ�
		xmPACK_FETCH_DATASET_ASK,
		//�ͻ��˽��յ�ѯ�����ݼ��ظ�
		xmPACK_FETCH_DATASET_RSP,

		xmPACK_VALTYPE_BEGIN = 100,
		//�����ڴ�ͬ����ͨ���ǿͻ��˵������и��£����ݼ���ֱ�Ӵ���
		xmPACK_MEMORYSYNC_ASK,
		//�ڴ�ӷ������ͻ���ͬ��
		xmPACK_MEMORYSYNC_RSP,
		xmPACK_MEMORYSYNC_BROADCAST,

		//�����ڴ�ͬ����ͨ���Ƿ�������Ҫ�������ͻ��˷��ͣ�����Ŀǰ���ΪRTI�����ⲿ��
		xmPACK_MEMORYSYNC_RTE_ASK,
		//�������пͻ��˵��ڴ���Ϣ
		xmPACK_MEMORYSYNC_RTE_RSP,

		xmPACK_SYNCCYCLE_CALLBACK,
		xmPACK_VALTYPE_END,


		xmPACK_TIMETYPE_BEGIN = 200,
		//���뷢��ʱ������
		xmPACK_TICK_ASK,
		//ʱ����������ظ�
		xmPACK_TICK_RSP,
		xmPACK_TICK_TOALL,
		xmPACK_TIMETYPE_END,

		xmPACK_APP_BEGIN = 300,
		//////����ΪPUB-SUBר��
		//��������
		xmPACK_DATASUB_ASK,
		//��������ظ�
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

		//////////����Ϊ����
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
