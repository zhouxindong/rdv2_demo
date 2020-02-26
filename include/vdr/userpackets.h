#pragma once
#ifndef __SSA_VDR_USERPACKETS_H
#define __SSA_VDR_USERPACKETS_H

#include "defs.h"
#include "packet.h"
#include "data.h"
#include "dataset.h"
#include "node.h"

namespace ssa
{
#pragma pack(1)
	//用于login会话时，node、dataset、data等对象信息的打包传输
	struct xmObj4PackNameWithVersion
	{
	public:
		xmObj4PackNameWithVersion()
		{
			m_nMemVersion = 0;
			m_nIndexVersion = 0;
		};
		//传输对象名字
		std::string       m_strName;
		//内存的版本号
		unsigned char     m_nMemVersion = 0;
		//索引的版本号
		unsigned char     m_nIndexVersion = 0;
	};

#pragma pack()

	class xmCommonPacket : public xmPacket
	{
	public:
		xmCommonPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_NEW_ASK;
		};
		xmCommonPacket(const xmCommonPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
		}
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize) ;
	};

	class xmTransNodePacket : public xmPacket
	{
	public:
		xmTransNodePacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_ADDNODE_ASK;
		};
		// copy constructor
		xmTransNodePacket(const xmTransNodePacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_Node.m_strName = pkt.m_Node.m_strName;
			m_Node.m_Attr = pkt.m_Node.m_Attr;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		xmNode                 m_Node;
	};

	class xmLinkNode2DataSetPacket : public xmPacket
	{
	public:
		xmLinkNode2DataSetPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_LINKNODE2DATASET_ASK;
		};
		// copy constructor
		xmLinkNode2DataSetPacket(const xmLinkNode2DataSetPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_Node = pkt.m_Node;
			m_DataSet = pkt.m_DataSet;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		std::string            m_Node;
		std::string            m_DataSet;
	};

	class xmTransDataSetPacket : public xmPacket
	{
	public:
		xmTransDataSetPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_ADDDATASET_ASK;
		};
		// copy constructor
		xmTransDataSetPacket(const xmTransDataSetPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_DataSet.m_strName = pkt.m_DataSet.m_strName;
			m_DataSet.m_Attr = pkt.m_DataSet.m_Attr;
			m_DataSet.m_nMemVersion = pkt.m_DataSet.m_nMemVersion;
			m_DataSet.m_nIndexVersion = pkt.m_DataSet.m_nIndexVersion;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		xmDataSet              m_DataSet;
	};

	class xmTransDataPacket : public xmPacket
	{
	public:
		xmTransDataPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_ADDDATA_ASK;
		};
		// copy constructor
		xmTransDataPacket(const xmTransDataPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_Data = pkt.m_Data;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		xmData                 m_Data;
	};

	//多个数据一起传送时，是按照每个数据集合的数据进行统一打包的，因此需要带上数据集合的版本号
	class xmTransMultiDataPacket : public xmPacket
	{
	public:
		xmTransMultiDataPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_TRANSMULTIDATA;
			m_nCount = 0;
		};
		// copy constructor
		xmTransMultiDataPacket(const xmTransMultiDataPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_vData = pkt.m_vData;
			m_nCount = pkt.m_nCount;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		unsigned int           m_nCount;
		std::vector<xmData>    m_vData;
	};

	class xmOperUniversalPacket : public xmPacket
	{
	public:
		xmOperUniversalPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_NOTHING;
		};
		// copy constructor
		xmOperUniversalPacket(const xmOperUniversalPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_nCountOfObjects = pkt.m_nCountOfObjects;
			m_vName = pkt.m_vName;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//从将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		unsigned int                            m_nCountOfObjects;
		std::vector<xmObj4PackNameWithVersion>  m_vName;
	};

	class xmSubDataPacket : public xmOperUniversalPacket
	{
	public:
		xmSubDataPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_DATASUB_ASK;
		};
		xmSubDataPacket(const xmSubDataPacket &pkt) : xmOperUniversalPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_nCountOfObjects = pkt.m_nCountOfObjects;
			m_vName = pkt.m_vName;
			m_nDataSetCount = pkt.m_nDataSetCount;
			m_vDataSetName = pkt.m_vDataSetName;
		}
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		unsigned int                     m_nDataSetCount;
		std::vector<std::string>         m_vDataSetName;
	};

	class xmSyncDataPacket : public xmOperUniversalPacket
	{
	public:
		xmSyncDataPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_SYNCDATA_ASK;
		};	
	};

	class xmBuildIndexPacket : public xmOperUniversalPacket
	{
	public:
		xmBuildIndexPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_BUILDINDEX_ASK;
		};
	};

	class xmBuildMemoryPacket : public xmPacket
	{
	public:
		xmBuildMemoryPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_BUILDMEMORY_ASK;
			m_uIsLocal = true;
		};
		// copy constructor
		xmBuildMemoryPacket(const xmBuildMemoryPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_nCountOfObjects = pkt.m_nCountOfObjects;
			m_vBuildMemName = pkt.m_vBuildMemName;
			m_uIsLocal = pkt.m_uIsLocal;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//从将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//需要操作的客户端的数量
		unsigned int                            m_nCountOfObjects;
		//为了判断客户端是否已经初始化好了内存区域
		std::vector<xmObj4PackNameWithVersion>  m_vBuildMemName;
		//判断是否需要服务器端也重新初始化
		unsigned char                           m_uIsLocal;
	};

	class xmMemoryValSyncPacket : public xmPacket
	{
	public:
		xmMemoryValSyncPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_VALSYNC;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_MEMORYSYNC_ASK;
			m_nMemLength = 0;
			m_nMemStartOffset = 0;
			m_nMemTotalLength = 0;
			m_nLast = 1;
		};
		// copy constructor
		xmMemoryValSyncPacket(const xmMemoryValSyncPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_nMemLength = pkt.m_nMemLength;
			m_DataSetName = pkt.m_DataSetName;
			m_uSyncDirection = pkt.m_uSyncDirection;
			m_nMemStartOffset = pkt.m_nMemStartOffset;
			m_nMemTotalLength = pkt.m_nMemTotalLength;
			m_nLast = pkt.m_nLast;

		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//从将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//同步的数据集名字
		std::string              m_DataSetName;
		//同步方向，包括指向服务器、客户端或者是双向
		unsigned char            m_uSyncDirection;
		//内存中的起始位置，是为了当数据集的大小超过65535时，对长度进行分区
		unsigned int             m_nMemStartOffset;
		//本包的内存数据长度
		unsigned int             m_nMemLength;
		//内存数据总长度
		unsigned int             m_nMemTotalLength;
		//是否最后一包数据，1是表示最后一包，0为中间包
		unsigned char            m_nLast;
	};

	class xmTimeTickPacket : public xmPacket
	{
	public:
		xmTimeTickPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_TICK;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_TICK_ASK;
		};
		// copy constructor
		xmTimeTickPacket(const xmTimeTickPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_lCurrentTime = pkt.m_lCurrentTime;
			m_nTickTime = pkt.m_nTickTime;
			m_nSyncCycles = pkt.m_nSyncCycles;
		}

		//往数据包写入数据
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//从将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//为了满足回放时，数据记录的需求，将当前时间的单位精确到毫秒
		long long              m_lCurrentTime;
		//基础时钟
		int                    m_nTickTime;
		//数据同步周期，表示几个周期进行数据同步
		int                    m_nSyncCycles;
	};

	class xmBulletinPacket : public xmPacket
	{
	public:
		xmBulletinPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_BULLETIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_BULLETIN_ASK;
		};
		xmBulletinPacket(const xmBulletinPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_strSender = pkt.m_strSender;
			m_strTopic = pkt.m_strTopic;
		}
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//最长的主题为16个汉字
		std::string               m_strSender;
		//最长的主题为16个汉字
		std::string               m_strTopic;
	};

	class xmMCast2NodesPacket : public xmOperUniversalPacket
	{
	public:
		xmMCast2NodesPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOGIN;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_MCAST2NODES_ASK;
		};
		xmMCast2NodesPacket(const xmMCast2NodesPacket &pkt) : xmOperUniversalPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_nCountOfObjects = pkt.m_nCountOfObjects;
			m_vName = pkt.m_vName;
			m_nObjsCount = pkt.m_nObjsCount;
			m_vObjsName = pkt.m_vObjsName;
			m_strSender = m_strSender;
		}
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out);
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//发送者名字
		std::string                  m_strSender;
		//接收节点的数量
		unsigned int                 m_nObjsCount;
		//接收节点的名字
		std::vector<std::string>     m_vObjsName;
	};

	class xmLogPacket : public xmPacket
	{
	public:
		xmLogPacket()
		{
			m_pktHead.m_uSessionID = xmESessionType::xmEST_LOG;
			m_pktHead.m_uPackCode = xmEPackCodes::xmPACK_LOG;
		};
		xmLogPacket(const xmLogPacket &pkt) : xmPacket(pkt)
		{
			m_pktHead = pkt.m_pktHead;
			m_strLog = pkt.m_strLog;
			m_nLogLevel = pkt.m_nLogLevel;
		}
		//将数据打包为网络数据报文
		virtual unsigned int   Encode(unsigned char* out) { return 0; };
		//从数据报文中解析数据
		virtual void           Decode(unsigned char* in, unsigned int nSize) {};

		std::string            ToUtf8String();
	public:
		//最长的主题为16个汉字
		std::string               m_strLog;
		//日志的级别，可以用于日志显示或者分类等。
		unsigned int              m_nLogLevel;
	};
}

#endif