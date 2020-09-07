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
	*  数据集内存块同步消息xmMSGMemorySync
	*  使用的函数宏为：xmMSGMemorySyncFunc
	*  消息类型为：xmEEventCode::xmEEC_MSG_MEMORY_SYNC
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
		*  数据集名字
		*  此变量表明内存块同步消息来自于那个数据集。
		*/
		std::string  strDataSetName;
		/**
		*  原来是发送时的时间戳，现在用于标识报文的其他标识，内部约定
		*  m_cArray[0]标示事件信息，bit0表示发生了敏感事件，bit1表示发生了复位事件
		*  m_cArray[1]标示本次来临的数据是使用哪种Qos来的。
		*/
		char         m_cArray[8];

		/**
		*  数据值
		*  此变量表明内存块的信息
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