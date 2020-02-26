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
	*  节点相关消息类型。
	*/
	typedef enum __tagNodeEvent
	{
		xmENE_NOTHING = 0,  /**< 无意义。 */
		xmENE_ADD,			/**< 添加节点。 */
		xmENE_DEL,			/**< 删除节点。 */
		xmENE_UPDATE,		/**< 更新节点。 */
		xmENE_LINK,			/**< 节点连接数据集。 */
		xmENE_UNLINK,		/**< 节点断开连接数据集。 */
		xmENE_MAX,
	} xmENodeEvent;

	/**
	*  数据相关消息类型。
	*/
	typedef enum __tagDataEvent
	{
		xmEDE_NOTHING = 0,  /**< 无意义。 */
		xmEDE_ADD,			/**< 添加数据。 */
		xmEDE_DEL,			/**< 删除数据。 */
		xmEDE_UPDATE,		/**< 更新数据。 */
		xmEDE_RESET,		/**< 复位数据。 */
		xmEDE_MAX,
	} xmEDataEvent;

	/**
	*  数据集相关消息类型。
	*/
	typedef enum __tagDataSetEvent
	{
		xmEDSE_NOTHING = 0,   /**< 无意义。 */
		xmEDSE_ADD,		      /**< 数据集登录。 */
		xmEDSE_DEL,		      /**< 数据集登出。 */
		xmEDSE_BUILDEDMEMORY, /**< 数据集内存重建完成。 */
		xmEDSE_MAX,
	} xmEDataSetEvent;

	/**
	*  公告消息类型。
	*/
	typedef enum __tagBulletinEvent
	{
		xmEBE_NOTHING = 0,    /**< 无意义。 */
		xmEBE_NEW,		      /**< 新公告来临。 */
		xmEBE_MAX,
	} xmEBulletinEvent;
	
	/**
	*  群发消息类型。
	*/
	typedef enum __tagMCastMSGEvent
	{
		xmEMCE_NOTHING = 0,    /**< 无意义。 */
		xmEMCE_NEW,		       /**< 新群发消息来临。 */
		xmEMCE_MAX,
	} xmEMCastMSGEvent;

	/**
	*  消息代码。
	*/
	typedef enum __tagEventCode
	{
		xmEEC_NOTHING = 100,    /**< 无意义，仅表示起始。 */
		xmEEC_MSG_DEFAULT,		/**< 默认消息代码。 */
		xmEEC_MSG_NODE_EVENT,   /**< 节点消息。 */
		xmEEC_MSG_DATASET_EVENT,/**< 数据集消息。 */
		xmEEC_MSG_DATA_EVENT,   /**< 数据消息，含数据操作消息。 */
		xmEEC_MSG_WORLD_TIMER,  /**< 基准时钟消息。 */
		xmEEC_MSG_VALUE_SYNC_TIMER,/**< 数据值同步时钟。 */
		xmEEC_MSG_MEMORY_SYNC,  /**< 数据集内存块同步消息。 */
		xmEEC_MSG_BULLETIN,		/**< 公告消息。 */
		xmEEC_MSG_MCAST,		/**< 组播消息。 */
		xmEEC_MSG_SYSTEM_STATE_SWITCH,/**< 状态切换消息。 */
		xmEEC_MSG_SWITCH_NORMAL,/**< 状态切换至正常消息。 */
		xmEEC_MSG_SWITCH_REPLAY,/**< 状态切换至回放消息。 */
		xmEEC_MSG_LOG,		    /**< 日志消息。 */
		xmEEC_MSG_DPC,          /**< 数据处理消息。 */
		xmEEC_MAX,
	} xmEEventCode;

	/**
	*  节点事件消息xmMSGNodeEvent
	*  使用的函数宏为：xmMSGNodeEventFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_NODE_EVENT
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
		*  节点名字。
		*/
		std::string     m_strNodeName;
		/**
		*  节点消息。
		*/
		xmENodeEvent    m_NodeEvent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_NODE_EVENT);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGNodeEventFunc, xmMSGNodeEvent);

	/**
	*  数据集事件消息xmMSGDataSetEvent
	*  使用的函数宏为：xmMSGDataSetEventFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_DATASET_EVENT
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
		*  数据集名字。
		*/
		std::string     m_strDataSetName;
		/**
		*  数据集消息。
		*/
		xmEDataSetEvent m_DataSetEvent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_DATASET_EVENT);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGDataSetEventFunc, xmMSGDataSetEvent);
	
	/**
	*  数据事件消息xmMSGDataEvent
	*  使用的函数宏为：xmMSGDataEventFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_DATA_EVENT
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
		*  数据名称。
		*/
		std::string        m_strDataName;
		/**
		*  数据操作类型。
		*/
		xmEDataEvent       m_evDataEvent;
		/**
		*  数据类型。
		*/
		xmEDataType        m_eDataType;
		/**
		*  数据值一，使用结构化描述形式。
		*/
		xmValue            m_Value;
		/**
		*  数据值二，使用数据缓冲区形式。
		*/
		xmByteBuffer       m_bbValue;
		/**
		*  是否属于数据集操作消息。
		*/
		bool               m_bDataSetEvent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_DATA_EVENT);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGDataEventFunc, xmMSGDataEvent);

	
	/**
	*  数据集合之间直接发送的消息xmMSGBulletin，设置了敏感的节点可以接收到。
	*  使用的函数宏为：xmMSGBulletinFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_BULLETIN
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
		*  消息发送者。
		*/
		std::string        m_strSender;
		/**
		*  消息的主题。
		*/
		std::string        m_strTopic;
		/**
		*  消息的具体内容。
		*/
		xmByteBuffer       m_dbContent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_BULLETIN);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGBulletinFunc, xmMSGBulletin);

	/**
	*  节点主动群发送的数据消息xmMSGMCast，向某些节点主动推送。
	*  使用的函数宏为：xmMSGMCastFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_MCAST
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
		*  消息发送者。
		*/
		std::string        m_strSender;
		/**
		*  消息的具体内容。
		*/
		xmByteBuffer       m_dbContent;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_MCAST);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGMCastFunc, xmMSGMCast);

	/**
	*  日志消息xmMSGLog
	*  使用的函数宏为：xmMSGLogFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_LOG
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
		*  日志消息具体内容。
		*/
		std::string   m_strLog;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_LOG);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGLogFunc, xmMSGLog);


	/**
	*  VDR时钟节拍通知消息xmMSGTimerClick
	*  使用的函数宏为：xmMSGTimerClickFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_WORLD_TIMER
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
		*  时钟计数，从本地接收到发送时钟消息开始计数。
		*/
		long long m_lTimeCount;
		/**
		*  绝对时间，服务器发出时钟时的服务器时间。
		*/
		long long m_lAbsTime;
		/**
		*  基础时钟。
		*/
		int  m_nTickTime;
		/**
		*  同步周期数。
		*/
		int  m_nSyncCycles;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_WORLD_TIMER);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGTimerClickFunc, xmMSGTimerClick);

	/**
	*  VDR数据同步时钟通知消息xmMSGValueSyncTimerClick
	*  使用的函数宏为：xmMSGValueSyncTimerClickFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_VALUE_SYNC_TIMER
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
	*  数据集内存块同步消息xmMSGMemorySync
	*  使用的函数宏为：xmMSGMemorySyncFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_MEMORY_SYNC
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
		*  数据集名字
		*  此变量表明内存块同步消息来自于那个数据集。
		*/
		std::string  strDataSetName;
		/**
		*  发送时的时间戳
		*  此变量表明内存块同步消息时的时间。
		*/
		long long    m_lAbsTime;

		/**
		*  数据值
		*  此变量表明内存块的信息。
		*/
		xmByteBuffer m_Buffer;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_MEMORY_SYNC);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGMemorySyncFunc, xmMSGMemorySync);


	/**
	*  系统切换消息xmMSGSwitchState
	*  使用的函数宏为：xmMSGSwitchStateFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_SYSTEM_STATE_SWITCH
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
		*  系统切换消息发送者的名字。
		*/
		std::string    m_strSender;
		/**
		*  系统状态。
		*/
		xmESystemState m_eSystemState;

		xmDECLARE_MESSAGE_CLASS(xmEEventCode::xmEEC_MSG_SYSTEM_STATE_SWITCH);
	};
	xmDECLARE_MESSAGE_FUNC(xmMSGSwitchStateFunc, xmMSGSwitchState);

}

#endif //	__SSA_VDR_USERMSGS_H