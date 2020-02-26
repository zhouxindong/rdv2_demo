#ifndef __SSA_VDR_USERMSGS_H
#define __SSA_VDR_USERMSGS_H

#include "export.h"
#include "..\base\message.h"
#include "data.h"
#include "dataset.h"
#include "node.h"
#include "vdrattr.h"

namespace ssa
{
	/**
	*  �ڵ������Ϣ���͡�
	*/
	typedef enum __tagNodeEvent
	{
		xmENE_NOTHING = 0,  /**< �����塣 */
		xmENE_ADD,			/**< ��ӽڵ㡣 */
		xmENE_DEL,			/**< ɾ���ڵ㡣 */
		xmENE_UPDATE,		/**< ���½ڵ㡣 */
		xmENE_LINK,			/**< �ڵ��������ݼ��� */
		xmENE_UNLINK,		/**< �ڵ�Ͽ��������ݼ��� */
		xmENE_MAX,
	} xmENodeEvent;

	/**
	*  ���������Ϣ���͡�
	*/
	typedef enum __tagDataEvent
	{
		xmEDE_NOTHING = 0,  /**< �����塣 */
		xmEDE_ADD,			/**< ������ݡ� */
		xmEDE_DEL,			/**< ɾ�����ݡ� */
		xmEDE_UPDATE,		/**< �������ݡ� */
		xmEDE_RESET,		/**< ��λ���ݡ� */
		xmEDE_MAX,
	} xmEDataEvent;

	/**
	*  ���ݼ������Ϣ���͡�
	*/
	typedef enum __tagDataSetEvent
	{
		xmEDSE_NOTHING = 0,   /**< �����塣 */
		xmEDSE_ADD,		      /**< ���ݼ���¼�� */
		xmEDSE_DEL,		      /**< ���ݼ��ǳ��� */
		xmEDSE_BUILDEDMEMORY, /**< ���ݼ��ڴ��ؽ���ɡ� */
		xmEDSE_MAX,
	} xmEDataSetEvent;

	/**
	*  ������Ϣ���͡�
	*/
	typedef enum __tagBulletinEvent
	{
		xmEBE_NOTHING = 0,    /**< �����塣 */
		xmEBE_NEW,		      /**< �¹������١� */
		xmEBE_MAX,
	} xmEBulletinEvent;
	
	/**
	*  Ⱥ����Ϣ���͡�
	*/
	typedef enum __tagMCastMSGEvent
	{
		xmEMCE_NOTHING = 0,    /**< �����塣 */
		xmEMCE_NEW,		       /**< ��Ⱥ����Ϣ���١� */
		xmEMCE_MAX,
	} xmEMCastMSGEvent;

	/**
	*  ��Ϣ���롣
	*/
	typedef enum __tagEventCode
	{
		xmEEC_NOTHING = 100,    /**< �����壬����ʾ��ʼ�� */
		xmEEC_MSG_DEFAULT,		/**< Ĭ����Ϣ���롣 */
		xmEEC_MSG_NODE_EVENT,   /**< �ڵ���Ϣ�� */
		xmEEC_MSG_DATASET_EVENT,/**< ���ݼ���Ϣ�� */
		xmEEC_MSG_DATA_EVENT,   /**< ������Ϣ�������ݲ�����Ϣ�� */
		xmEEC_MSG_WORLD_TIMER,  /**< ��׼ʱ����Ϣ�� */
		xmEEC_MSG_VALUE_SYNC_TIMER,/**< ����ֵͬ��ʱ�ӡ� */
		xmEEC_MSG_MEMORY_SYNC,  /**< ���ݼ��ڴ��ͬ����Ϣ�� */
		xmEEC_MSG_BULLETIN,		/**< ������Ϣ�� */
		xmEEC_MSG_MCAST,		/**< �鲥��Ϣ�� */
		xmEEC_MSG_SYSTEM_STATE_SWITCH,/**< ״̬�л���Ϣ�� */
		xmEEC_MSG_SWITCH_NORMAL,/**< ״̬�л���������Ϣ�� */
		xmEEC_MSG_SWITCH_REPLAY,/**< ״̬�л����ط���Ϣ�� */
		xmEEC_MSG_LOG,		    /**< ��־��Ϣ�� */
		xmEEC_MSG_DPC,          /**< ���ݴ�����Ϣ�� */
		xmEEC_MAX,
	} xmEEventCode;

	/**
	*  �ڵ��¼���ϢxmMSGNodeEvent
	*  ʹ�õĺ�����Ϊ��xmMSGNodeEventFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_NODE_EVENT
	*/
	class xmMSGNodeEvent : public xmMessage
	{
	public:
		xmMSGNodeEvent(void)
		{
			m_strNodeName = "";
			m_NodeEvent = xmENE_NOTHING;
		};
		xmMSGNodeEvent(const xmMSGNodeEvent& msgNodeEvent)
		{
			m_strNodeName = msgNodeEvent.m_strNodeName;
			m_NodeEvent = msgNodeEvent.m_NodeEvent;
		};
		void xmMSGNodeEvent::operator=(const xmMSGNodeEvent& msgNodeEvent)
		{
			m_strNodeName = msgNodeEvent.m_strNodeName;
			m_NodeEvent = msgNodeEvent.m_NodeEvent;
		};

		virtual ~xmMSGNodeEvent(void) {};


		virtual xmMessage* Clone(void) const
		{
			return new xmMSGNodeEvent(*this);
		};

	public:
		/**
		*  �ڵ����֡�
		*/
		std::string     m_strNodeName;
		/**
		*  �ڵ���Ϣ��
		*/
		xmENodeEvent    m_NodeEvent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_NODE_EVENT);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGNodeEventFunc, xmMSGNodeEvent);

	/**
	*  ���ݼ��¼���ϢxmMSGDataSetEvent
	*  ʹ�õĺ�����Ϊ��xmMSGDataSetEventFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_DATASET_EVENT
	*/
	class xmMSGDataSetEvent : public xmMessage
	{
	public:
		xmMSGDataSetEvent(void)
		{
			m_strDataSetName = "";
			m_DataSetEvent = xmEDSE_NOTHING;
		};
		xmMSGDataSetEvent(const xmMSGDataSetEvent& msgDataSetEvent)
		{
			m_strDataSetName = msgDataSetEvent.m_strDataSetName;
			m_DataSetEvent = msgDataSetEvent.m_DataSetEvent;
		};
		void xmMSGDataSetEvent::operator=(const xmMSGDataSetEvent& msgDataSetEvent)
		{
			m_strDataSetName = msgDataSetEvent.m_strDataSetName;
			m_DataSetEvent = msgDataSetEvent.m_DataSetEvent;
		};

		virtual ~xmMSGDataSetEvent(void){};


		virtual xmMessage* Clone(void) const
		{
			return new xmMSGDataSetEvent(*this);
		};

	public:
		/**
		*  ���ݼ����֡�
		*/
		std::string     m_strDataSetName;
		/**
		*  ���ݼ���Ϣ��
		*/
		xmEDataSetEvent m_DataSetEvent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_DATASET_EVENT);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGDataSetEventFunc, xmMSGDataSetEvent);
	
	/**
	*  �����¼���ϢxmMSGDataEvent
	*  ʹ�õĺ�����Ϊ��xmMSGDataEventFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_DATA_EVENT
	*/
	class xmMSGDataEvent : public xmMessage
	{
	public:

		xmMSGDataEvent(void)
		{
			m_evDataEvent = xmEDE_NOTHING;
		};
		xmMSGDataEvent(const xmMSGDataEvent& msgData)
		{
			m_strDataName = msgData.m_strDataName;
			m_evDataEvent    = msgData.m_evDataEvent;
			m_eDataType = msgData.m_eDataType;
			m_bbValue     = msgData.m_bbValue;
			m_Value.Initialize(msgData.m_Value);
			m_bDataSetEvent   = msgData.m_bDataSetEvent;
		};

		void xmMSGDataEvent::operator=(const xmMSGDataEvent& msgData)
		{
			m_strDataName = msgData.m_strDataName;
			m_evDataEvent = msgData.m_evDataEvent;
			m_eDataType = msgData.m_eDataType;
			m_bbValue = msgData.m_bbValue;
			m_Value.Initialize(msgData.m_Value);
			m_bDataSetEvent = msgData.m_bDataSetEvent;
		}


		virtual ~xmMSGDataEvent(void){};

		virtual xmMessage* Clone(void) const
		{
			return new xmMSGDataEvent(*this);
		};

	public:
		/**
		*  �������ơ�
		*/
		std::string        m_strDataName;
		/**
		*  ���ݲ������͡�
		*/
		xmEDataEvent       m_evDataEvent;
		/**
		*  �������͡�
		*/
		xmEDataType        m_eDataType;
		/**
		*  ����ֵһ��ʹ�ýṹ��������ʽ��
		*/
		xmValue            m_Value;
		/**
		*  ����ֵ����ʹ�����ݻ�������ʽ��
		*/
		xmByteBuffer       m_bbValue;
		/**
		*  �Ƿ��������ݼ�������Ϣ��
		*/
		bool               m_bDataSetEvent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_DATA_EVENT);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGDataEventFunc, xmMSGDataEvent);

	
	/**
	*  ���ݼ���֮��ֱ�ӷ��͵���ϢxmMSGBulletin�����������еĽڵ���Խ��յ���
	*  ʹ�õĺ�����Ϊ��xmMSGBulletinFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_BULLETIN
	*/
	class xmMSGBulletin : public xmMessage
	{
	public:
		xmMSGBulletin(void){};
		xmMSGBulletin(const xmMSGBulletin& msgBulletin)
		{
			m_strSender = msgBulletin.m_strSender;
			m_strTopic = msgBulletin.m_strTopic;
			m_dbContent = msgBulletin.m_dbContent;
		}

		void xmMSGBulletin::operator=(const xmMSGBulletin& msgBulletin)
		{
			m_strSender = msgBulletin.m_strSender;
			m_strTopic = msgBulletin.m_strTopic;
			m_dbContent = msgBulletin.m_dbContent;
		}

		virtual ~xmMSGBulletin(void){};
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGBulletin(*this);
		}

	public:
		/**
		*  ��Ϣ�����ߡ�
		*/
		std::string        m_strSender;
		/**
		*  ��Ϣ�����⡣
		*/
		std::string        m_strTopic;
		/**
		*  ��Ϣ�ľ������ݡ�
		*/
		xmByteBuffer       m_dbContent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_BULLETIN);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGBulletinFunc, xmMSGBulletin);

	/**
	*  �ڵ�����Ⱥ���͵�������ϢxmMSGMCast����ĳЩ�ڵ��������͡�
	*  ʹ�õĺ�����Ϊ��xmMSGMCastFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_MCAST
	*/
	class xmMSGMCast : public xmMessage
	{
	public:
		xmMSGMCast(void) {};
		xmMSGMCast(const xmMSGMCast& msgMCast)
		{
			m_strSender = msgMCast.m_strSender;
			m_dbContent = msgMCast.m_dbContent;
		}

		void xmMSGMCast::operator=(const xmMSGMCast& msgMCast)
		{
			m_strSender = msgMCast.m_strSender;
			m_dbContent = msgMCast.m_dbContent;
		}

		virtual ~xmMSGMCast(void) {};
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGMCast(*this);
		}

	public:
		/**
		*  ��Ϣ�����ߡ�
		*/
		std::string        m_strSender;
		/**
		*  ��Ϣ�ľ������ݡ�
		*/
		xmByteBuffer       m_dbContent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_MCAST);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGMCastFunc, xmMSGMCast);

	/**
	*  ��־��ϢxmMSGLog
	*  ʹ�õĺ�����Ϊ��xmMSGLogFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_LOG
	*/
	class xmMSGLog : public xmMessage
	{
	public:
		xmMSGLog(void) {};
		xmMSGLog(const xmMSGLog& msgLog)
		{
			m_strLog = msgLog.m_strLog;
		}

		void xmMSGLog::operator=(const xmMSGLog& msgLog)
		{
			m_strLog = msgLog.m_strLog;
		}

		virtual ~xmMSGLog(void) {};

		virtual xmMessage* Clone(void) const
		{
			return new xmMSGLog(*this);
		}

	public:
		/**
		*  ��־��Ϣ�������ݡ�
		*/
		std::string   m_strLog;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_LOG);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGLogFunc, xmMSGLog);


	/**
	*  VDRʱ�ӽ���֪ͨ��ϢxmMSGTimerClick
	*  ʹ�õĺ�����Ϊ��xmMSGTimerClickFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_WORLD_TIMER
	*/
	class xmMSGTimerClick : public xmMessage
	{
	public:
		xmMSGTimerClick(void){};
		xmMSGTimerClick(const xmMSGTimerClick& msgTimerClick)
		{
			m_lTimeCount = msgTimerClick.m_lTimeCount;
			m_lAbsTime = msgTimerClick.m_lAbsTime;
			m_nTickTime = msgTimerClick.m_nTickTime;
			m_nSyncCycles = msgTimerClick.m_nSyncCycles;
		}

		void xmMSGTimerClick::operator=(const xmMSGTimerClick& msgTimerClick)
		{
			m_lTimeCount = msgTimerClick.m_lTimeCount;
			m_lAbsTime = msgTimerClick.m_lAbsTime;
			m_nTickTime = msgTimerClick.m_nTickTime;
			m_nSyncCycles = msgTimerClick.m_nSyncCycles;
		}

		virtual ~xmMSGTimerClick(void){};
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGTimerClick(*this);
		}

	public:
		/**
		*  ʱ�Ӽ������ӱ��ؽ��յ�����ʱ����Ϣ��ʼ������
		*/
		long long m_lTimeCount;
		/**
		*  ����ʱ�䣬����������ʱ��ʱ�ķ�����ʱ�䡣
		*/
		long long m_lAbsTime;
		/**
		*  ����ʱ�ӡ�
		*/
		int  m_nTickTime;
		/**
		*  ͬ����������
		*/
		int  m_nSyncCycles;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_WORLD_TIMER);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGTimerClickFunc, xmMSGTimerClick);

	/**
	*  VDR����ͬ��ʱ��֪ͨ��ϢxmMSGValueSyncTimerClick
	*  ʹ�õĺ�����Ϊ��xmMSGValueSyncTimerClickFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_VALUE_SYNC_TIMER
	*/
	class xmMSGValueSyncTimerClick : public xmMSGTimerClick
	{
	public:
		xmMSGValueSyncTimerClick(void) {};
		xmMSGValueSyncTimerClick(const xmMSGValueSyncTimerClick& msgTimerClick)
		{
			m_lTimeCount = msgTimerClick.m_lTimeCount;
			m_lAbsTime = msgTimerClick.m_lAbsTime;
			m_nTickTime = msgTimerClick.m_nTickTime;
			m_nSyncCycles = msgTimerClick.m_nSyncCycles;
		}

		void xmMSGValueSyncTimerClick::operator=(const xmMSGValueSyncTimerClick& msgTimerClick)
		{
			m_lTimeCount = msgTimerClick.m_lTimeCount;
			m_lAbsTime = msgTimerClick.m_lAbsTime;
			m_nTickTime = msgTimerClick.m_nTickTime;
			m_nSyncCycles = msgTimerClick.m_nSyncCycles;
		}

		virtual ~xmMSGValueSyncTimerClick(void) {};
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGValueSyncTimerClick(*this);
		}

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_VALUE_SYNC_TIMER);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGValueSyncTimerClickFunc, xmMSGValueSyncTimerClick);

	/**
	*  ���ݼ��ڴ��ͬ����ϢxmMSGMemorySync
	*  ʹ�õĺ�����Ϊ��xmMSGMemorySyncFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_MEMORY_SYNC
	*/
	class xmMSGMemorySync : public xmMessage
	{
	public:
		xmMSGMemorySync(void){};
		xmMSGMemorySync(const xmMSGMemorySync& msgDataSync)
		{
			strDataSetName = msgDataSync.strDataSetName;
			m_lAbsTime = msgDataSync.m_lAbsTime;
			m_Buffer = msgDataSync.m_Buffer;
		};

		void xmMSGMemorySync::operator=(const xmMSGMemorySync& msgDataSync)
		{
			strDataSetName = msgDataSync.strDataSetName;
			m_lAbsTime = msgDataSync.m_lAbsTime;
			m_Buffer = msgDataSync.m_Buffer;
		};

		virtual ~xmMSGMemorySync(void){};
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGMemorySync(*this);
		}

	public:
		/**
		*  ���ݼ�����
		*  �˱��������ڴ��ͬ����Ϣ�������Ǹ����ݼ���
		*/
		std::string  strDataSetName;
		/**
		*  ����ʱ��ʱ���
		*  �˱��������ڴ��ͬ����Ϣʱ��ʱ�䡣
		*/
		long long    m_lAbsTime;

		/**
		*  ����ֵ
		*  �˱��������ڴ�����Ϣ��
		*/
		xmByteBuffer m_Buffer;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_MEMORY_SYNC);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGMemorySyncFunc, xmMSGMemorySync);


	/**
	*  ϵͳ�л���ϢxmMSGSwitchState
	*  ʹ�õĺ�����Ϊ��xmMSGSwitchStateFunc
	*  ��Ϣ����Ϊ��xmEEventCode::xmEEC_MSG_SYSTEM_STATE_SWITCH
	*/
	class xmMSGSwitchState : public xmMessage
	{
	public:
		xmMSGSwitchState(void){};
		xmMSGSwitchState(const xmMSGSwitchState& msgSwitchState)
		{
			m_strSender = msgSwitchState.m_strSender;
			m_eSystemState = msgSwitchState.m_eSystemState;
		};
		void xmMSGSwitchState::operator=(const xmMSGSwitchState& msgSwitchState)
		{
			m_strSender = msgSwitchState.m_strSender;
			m_eSystemState = msgSwitchState.m_eSystemState;
		};
		virtual ~xmMSGSwitchState(void){};
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGSwitchState(*this);
		}

	public:
		/**
		*  ϵͳ�л���Ϣ�����ߵ����֡�
		*/
		std::string    m_strSender;
		/**
		*  ϵͳ״̬��
		*/
		xmESystemState m_eSystemState;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_SYSTEM_STATE_SWITCH);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGSwitchStateFunc, xmMSGSwitchState);

}

#endif //	__SSA_VDR_USERMSGS_H