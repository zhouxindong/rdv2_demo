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
	class xmMSGNodeEvent : public xmMessage , public xmTimeStamp
	{
	public:
		xmMSGNodeEvent(void)
		{
			m_strNodeName = "";
			m_NodeEvent = xmENE_NOTHING;
		};
		xmMSGNodeEvent(const xmMSGNodeEvent& msg)
		{
			m_strNodeName = msg.m_strNodeName;
			m_NodeEvent = msg.m_NodeEvent;

			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		};
		void xmMSGNodeEvent::operator=(const xmMSGNodeEvent& msg)
		{
			m_strNodeName = msg.m_strNodeName;
			m_NodeEvent = msg.m_NodeEvent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
	class xmMSGDataSetEvent : public xmMessage, public xmTimeStamp
	{
	public:
		xmMSGDataSetEvent(void)
		{
			m_strDataSetName = "";
			m_DataSetEvent = xmEDSE_NOTHING;
		};
		xmMSGDataSetEvent(const xmMSGDataSetEvent& msg)
		{
			m_strDataSetName = msg.m_strDataSetName;
			m_DataSetEvent = msg.m_DataSetEvent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		};
		void xmMSGDataSetEvent::operator=(const xmMSGDataSetEvent& msg)
		{
			m_strDataSetName = msg.m_strDataSetName;
			m_DataSetEvent = msg.m_DataSetEvent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
	class xmMSGDataEvent : public xmMessage, public xmTimeStamp
	{
	public:

		xmMSGDataEvent(void)
		{
			m_evDataEvent = xmEDE_NOTHING;
		};
		xmMSGDataEvent(const xmMSGDataEvent& msg)
		{
			m_strDataName = msg.m_strDataName;
			m_evDataEvent    = msg.m_evDataEvent;
			m_eDataType = msg.m_eDataType;
			m_bbValue     = msg.m_bbValue;
			m_Value.Initialize(msg.m_Value);
			m_bDataSetEvent   = msg.m_bDataSetEvent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		};

		void xmMSGDataEvent::operator=(const xmMSGDataEvent& msg)
		{
			m_strDataName = msg.m_strDataName;
			m_evDataEvent = msg.m_evDataEvent;
			m_eDataType = msg.m_eDataType;
			m_bbValue = msg.m_bbValue;
			m_Value.Initialize(msg.m_Value);
			m_bDataSetEvent = msg.m_bDataSetEvent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
	class xmMSGBulletin : public xmMessage, public xmTimeStamp
	{
	public:
		xmMSGBulletin(void){};
		xmMSGBulletin(const xmMSGBulletin& msg)
		{
			m_strSender = msg.m_strSender;
			m_strTopic = msg.m_strTopic;
			m_dbContent = msg.m_dbContent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		}

		void xmMSGBulletin::operator=(const xmMSGBulletin& msg)
		{
			m_strSender = msg.m_strSender;
			m_strTopic = msg.m_strTopic;
			m_dbContent = msg.m_dbContent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
	class xmMSGMCast : public xmMessage, public xmTimeStamp
	{
	public:
		xmMSGMCast(void) {};
		xmMSGMCast(const xmMSGMCast& msg)
		{
			m_strSender = msg.m_strSender;
			m_dbContent = msg.m_dbContent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		}

		void xmMSGMCast::operator=(const xmMSGMCast& msg)
		{
			m_strSender = msg.m_strSender;
			m_dbContent = msg.m_dbContent;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
	class xmMSGLog : public xmMessage, public xmTimeStamp
	{
	public:
		xmMSGLog(void) {};
		xmMSGLog(const xmMSGLog& msg)
		{
			m_strLog = msg.m_strLog;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		}

		void xmMSGLog::operator=(const xmMSGLog& msg)
		{
			m_strLog = msg.m_strLog;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
	class xmMSGTimerClick : public xmMessage, public xmTimeStamp
	{
	public:
		xmMSGTimerClick(void){};
		xmMSGTimerClick(const xmMSGTimerClick& msg)
		{
			m_nTickTime = msg.m_nTickTime;
			m_nSyncCycles = msg.m_nSyncCycles;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		}

		void xmMSGTimerClick::operator=(const xmMSGTimerClick& msg)
		{
			m_nTickTime = msg.m_nTickTime;
			m_nSyncCycles = msg.m_nSyncCycles;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		}

		virtual ~xmMSGTimerClick(void){};
		virtual xmMessage* Clone(void) const
		{
			return new xmMSGTimerClick(*this);
		}

	public:
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
		xmMSGValueSyncTimerClick(const xmMSGValueSyncTimerClick& msg)
		{
			m_nTickTime = msg.m_nTickTime;
			m_nSyncCycles = msg.m_nSyncCycles;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		}

		void xmMSGValueSyncTimerClick::operator=(const xmMSGValueSyncTimerClick& msg)
		{
			m_nTickTime = msg.m_nTickTime;
			m_nSyncCycles = msg.m_nSyncCycles;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
	class xmMSGMemorySync : public xmMessage, public xmTimeStamp
	{
	public:
		xmMSGMemorySync(void){};
		xmMSGMemorySync(const xmMSGMemorySync& msg)
		{
			strDataSetName = msg.strDataSetName;
			memcpy(m_cArray, msg.m_cArray,8);
			m_Buffer = msg.m_Buffer;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		};

		void xmMSGMemorySync::operator=(const xmMSGMemorySync& msg)
		{
			strDataSetName = msg.strDataSetName;
			memcpy(m_cArray, msg.m_cArray, 8);
			m_Buffer = msg.m_Buffer;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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
		*  ԭ���Ƿ���ʱ��ʱ������������ڱ�ʶ���ĵ�������ʶ���ڲ�Լ��
		*  m_cArray[0]��ʾ�¼���Ϣ��bit0��ʾ�����������¼���bit1��ʾ�����˸�λ�¼�
		*  m_cArray[1]��ʾ�������ٵ�������ʹ������Qos���ġ�
		*/
		char         m_cArray[8];

		/**
		*  ����ֵ
		*  �˱��������ڴ�����Ϣ
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
	class xmMSGSwitchState : public xmMessage, public xmTimeStamp
	{
	public:
		xmMSGSwitchState(void){};
		xmMSGSwitchState(const xmMSGSwitchState& msg)
		{
			m_strSender = msg.m_strSender;
			m_eSystemState = msg.m_eSystemState;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
		};
		void xmMSGSwitchState::operator=(const xmMSGSwitchState& msg)
		{
			m_strSender = msg.m_strSender;
			m_eSystemState = msg.m_eSystemState;
			m_lFrameCount = msg.m_lFrameCount;
			m_lTimeStamp = msg.m_lTimeStamp;
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