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
	//����login�Ựʱ��node��dataset��data�ȶ�����Ϣ�Ĵ������
	struct xmObj4PackNameWithVersion
	{
	public:
		xmObj4PackNameWithVersion()
		{
			m_nMemVersion = 0;
			m_nIndexVersion = 0;
		};
		//�����������
		std::string       m_strName;
		//�ڴ�İ汾��
		unsigned char     m_nMemVersion = 0;
		//�����İ汾��
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
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		xmData                 m_Data;
	};

	//�������һ����ʱ���ǰ���ÿ�����ݼ��ϵ����ݽ���ͳһ����ģ������Ҫ�������ݼ��ϵİ汾��
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//�ӽ����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
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
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//�ӽ����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//��Ҫ�����Ŀͻ��˵�����
		unsigned int                            m_nCountOfObjects;
		//Ϊ���жϿͻ����Ƿ��Ѿ���ʼ�������ڴ�����
		std::vector<xmObj4PackNameWithVersion>  m_vBuildMemName;
		//�ж��Ƿ���Ҫ��������Ҳ���³�ʼ��
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned  int nSize) {};
		//�ӽ����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//ͬ�������ݼ�����
		std::string              m_DataSetName;
		//ͬ�����򣬰���ָ����������ͻ��˻�����˫��
		unsigned char            m_uSyncDirection;
		//�ڴ��е���ʼλ�ã���Ϊ�˵����ݼ��Ĵ�С����65535ʱ���Գ��Ƚ��з���
		unsigned int             m_nMemStartOffset;
		//�������ڴ����ݳ���
		unsigned int             m_nMemLength;
		//�ڴ������ܳ���
		unsigned int             m_nMemTotalLength;
		//�Ƿ����һ�����ݣ�1�Ǳ�ʾ���һ����0Ϊ�м��
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

		//�����ݰ�д������
		virtual void           WriteData(unsigned char* in, unsigned int nSize) {};
		//�ӽ����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//Ϊ������ط�ʱ�����ݼ�¼�����󣬽���ǰʱ��ĵ�λ��ȷ������
		long long              m_lCurrentTime;
		//����ʱ��
		int                    m_nTickTime;
		//����ͬ�����ڣ���ʾ�������ڽ�������ͬ��
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
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//�������Ϊ16������
		std::string               m_strSender;
		//�������Ϊ16������
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
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out);
		//�����ݱ����н�������
		virtual void           Decode(unsigned char* in, unsigned int nSize);
	public:
		//����������
		std::string                  m_strSender;
		//���սڵ������
		unsigned int                 m_nObjsCount;
		//���սڵ������
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
		//�����ݴ��Ϊ�������ݱ���
		virtual unsigned int   Encode(unsigned char* out) { return 0; };
		//�����ݱ����н�������
		virtual void           Decode(unsigned char* in, unsigned int nSize) {};

		std::string            ToUtf8String();
	public:
		//�������Ϊ16������
		std::string               m_strLog;
		//��־�ļ��𣬿���������־��ʾ���߷���ȡ�
		unsigned int              m_nLogLevel;
	};
}

#endif