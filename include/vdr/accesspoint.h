#pragma once

#ifndef __SSA_VDR_ACCESSPOINT_H
#define __SSA_VDR_ACCESSPOINT_H

#include "export.h"
#include "sensmsgs.h"
#include "defs.h"
#include "bytebuffer.h"
#include "../base/value.h"

namespace ssa
{
	class xmVDR;
	/**
	*  VDR用户访问接口. 用户使用访问接口连接VDR，并从其中获取相关服务的结果，服务包括：
	*  1.数据读、写；
	*  2.敏感消息；
	*  3.公告消息；
	*  4.群发消息；
	*  5.发布订阅等。
	*/
	class xmVDR_EXPORT xmAccessPoint
	{
	public:
		xmAccessPoint(void);
		virtual ~xmAccessPoint(void);
		/**
		* 数据访问接口连接VDR，使用数据访问接口时，必须要和一个VDR连接。
		* @param pVdr 是指被连接的VDR指针。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           Connect(xmVDR* pVdr);
		/**
		* 数据访问接口连接VDR，使用数据访问接口时，必须要和一个VDR连接。
		* @param pVdrAttr 是指被连接的VDR属性指针。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           Connect(xmVDRAttr* pVdrAttr);
		/**
		* 数据访问接口连接VDR，使用数据访问接口时，必须要和一个VDR连接。
		* @param uVdrID 是指被连接的VDR的ID号。
		* @param uRegion 是指被连接的VDR的中的作用域。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           Connect(unsigned short uVdrID, unsigned short uRegion);
		/**
		* 数据访问接口获取已连接的VDR指针。
		* @return 连接结果，表示已连接的VDR指针。
		*/
		virtual xmVDR*         ConnectedVDR();
		/**
		* 数据访问接口退出接入点，此时不是退出VDR，VDR依然存在，可以再次连接访问接口。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            Quit(void);

		/**
		* 数据访问接口获取节点在VDR数据的操作权限授权，代表节点可以进行相关的操作，此授权也可以通过数据集直接操作获取。
		* 获取授权最终是获取对数据集的操作。在内部实际上分为两个步骤，一是查询节点所连接的数据集，二是吸附至节点的数据集。
		* @param strNodeName 是指被代表的节点的名字。
		* @return 授权结果，true表示成功，false表示失败。
		*/
		virtual bool           Authorize(const char* strNodeName);
		/**
		* 数据访问接口取消节点在VDR的操作权限授权。
		* @param strNodeName 是指被代表的节点的名字。
		* @return 取消授权结果，true表示成功，false表示失败。
		*/
		virtual bool           UnAuthorize(const char* strNodeName);
		/**
		* 获取数据访问接口授权的节点。
		* @return 授权节点的名字与节点信息的MAP表。
		*/
		std::map<std::string, xmNode>&    AuthorizedNodes();
		/**
		* 与节点授权类似，获取授权最终是获取对数据集的操作。此接口用于将数据访问接口依附在数据集上。
		* 也可以通过Node的Authorize进行授权，推荐用法是通过Node进行授权。
		* @param pVdr 是指被依附的VDR指针。
		* @param strDataSetName 是指被依附的数据集的名字。
		* @param nTry 是指被尝试多少次。
		* @return 依附结果，true表示成功，false表示失败。
		*/
		virtual bool           Attach(xmVDR* pVdr, const char* strDataSetName = "", int nTry = 65535);
		/**
		* 与节点授权类似，获取授权最终是获取对数据集的操作。此接口用于将数据访问接口依附在数据集上。
		* 也可以通过Node的Authorize进行授权，推荐用法是通过Node进行授权。
		* @param strDataSetName 是指被依附的数据集的名字。
		* @param nTry 是指被尝试多少次。
		* @return 依附结果，true表示成功，false表示失败。
		*/
		virtual bool           Attach(const char* strDataSetName, int nTry = 65535);
		/**
		* 取消数据访问接口对数据集的依附。
		* @param strDataSetName 是指被依附的数据集的名字。
		* @return 取消依附结果，true表示成功，false表示失败。
		*/
		virtual bool           UnAttach(const char* strDataSetName);
		/**
		* 获取数据访问接口依附的数据集合信息。
		* @return 依附的数据集合名字与数据集合信息的MAP表。
		*/
		std::map<std::string, xmDataSet>&  AttachedDataSets();
		/**
		* 判断数据访问接口是否已经有效依附。
		* @return 依附结果，true表示成功，false表示失败。
		*/
		bool                   IsValid();
		/**
		* 对于本接入点，使某个数据集具备或者不具备发布数据值的能力,通常用于在回放或者其他特殊状态时进行使用。
		* @param strDataSetName 是指数据集的名字。
		* @param havePublishAbility 是指数据集是否具有发布数据值的能力。
		* @return 无。
		*/
		void                   SetPublishAbility(const char* strDataSetName, bool havePublishAbility);

		/**
		* 对于本接入点，使某个数据集具备或者不具备自动发布数据值的能力。
		* @param strDataSetName 是指数据集的名字。
		* @param autoPublishSign 是指数据集是否具有由内部时钟机制发布数据值的能力，如果为true表示由时钟驱动，false表示手动进行数值同步。
		* @return 无。
		*/
		void                   SetAutoPublishSign(const char* strDataSetName, bool autoPublishSign);
		/**
		* 如果系统中存在多个节点同时写一个数据或者数据集的情况，首先要获取权限，解决多个写之间的冲突问题。
		* @param strDataSetName 是指同时写的数据集的名字。
		* @return 结果，true表示成功，false表示失败。
		*/
		virtual bool           GetToken(const char* strDataSetName = "");
		/**
		* 手动同步触发信号，分为两种情况，一种是需要上传新的数据，同时获取新的内存数据，一种情况仅仅获取服务器端的数据。
		* @param am 同步方向，包括了仅向服务器，仅向客户端，双向或者广播。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            SyncContent(xmESyncDirection am = xmESyncDirection::xmESD_TOSERVER);

		/**
		* 获取节点数目。
		* @return 节点个数。
		*/
		virtual int            GetNodeCount(void);
		/**
		* 按照内部索引顺序获取单个节点的描述信息。
		* @param uIndex 索引顺序，从GetNodeCount获取结果是最大值，从0开始计数。
		* @param nd 节点描述信息引用，函数返回成功时，nd表示获取的节点信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetNode(unsigned int uIndex, xmNode& nd);
		/**
		* 按照名字获取单个节点的描述信息。
		* @param strNodeName 节点名字。
		* @param nd 节点描述信息引用，函数返回成功时，nd表示获取的节点信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetNode(const char* strNodeName, xmNode& nd);
		/**
		* 获取数据集数目。
		* @return 数据集个数。
		*/
		virtual int            GetDataSetCount(void);
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
		virtual int            GetDataSetMemoryLength(const char* strDataSetName);
		/**
		* 按照数据集名字，起始位置，和大小读取数据集内存数据。
		* @param strDataSetName 数据集名字。
		* @param nPos 读内存起始位置。
		* @param pOut 内存读操作输出缓冲地址。
		* @param nLength 内存读操作长度。
		* @return 结果，-1表示失败，其他代码表示正确读取内存的长度。
		*/
		virtual int            GetDataSetMemory(const char* strDataSetName, int nPos, char* pOut, int nLength);
		/**
		* 按照数据集名字，起始位置，和大小写数据集内存数据。
		* @param strDataSetName 数据集名字。
		* @param nPos 读内存起始位置。
		* @param pOut 内存写操作输入缓冲地址。
		* @param nLength 内存写操作长度。
		* @return 结果，-1表示失败，其他代码表示正确写内存的长度。
		*/
		virtual int            SetDataSetMemory(const char* strDataSetName, int nPos, char* pIn, int nLength);
		/**
		* 获取数据数目。
		* @return 数据个数。
		*/
		virtual int            GetDataCount();
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
		* 配置默认敏感消息处理中间代理。
		* @param  nProxyID 敏感消息中间代理ID号。
		* @return 结果，0表示成功，-1表示失败，未找到代理，其他代码表示需要用户判断是否正确。
		*/
		virtual int            ConfigSensProxy(int nProxyID, const char* dllName = "");

		/**
		* 配置默认敏感消息处理器。
		* @param pHandler 敏感消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigDefaultHandler(xmMsgHandler* pHandler);
		/**
		* 配置节点消息处理器。消息xmMSGNodeEvent，使用xmEEC_MSG_NODE_EVENT。
		* @param neNodeEvent 敏感节点消息。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigNodeEventHandler(xmENodeEvent neNodeEvent, xmMsgHandler* pHandler = 0);
		/**
		* 配置数据集消息处理器。消息xmMSGDataSetEvent，使用xmEEC_MSG_DATASET_EVENT。
		* @param neDataSetEvent 敏感数据集消息。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigDataSetEventHandler(xmEDataSetEvent neDataSetEvent, xmMsgHandler* pHandler = 0);
		/**
		* 配置数据消息处理器。消息xmMSGDataEvent，使用xmEEC_MSG_DATA_EVENT。
		* 此消息处理会对所有数据的消息起作用，会大量消耗计算资源，请慎用，通常使用指定数据名字接口。
		* @param deDataEvent 敏感数据消息。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigDataEventHandler(xmEDataEvent deDataEvent, xmMsgHandler* pHandler = 0);
		/**
		* 配置数据消息处理器。消息xmMSGDataEvent，使用xmEEC_MSG_DATA_EVENT。
		* 此消息处理仅对名字所代表的数据消息起作用。
		* @param strDataName 数据名字
		* @param deDataEvent 敏感数据消息。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigDataEventHandler(const char* strDataName, xmEDataEvent deDataEvent, xmMsgHandler* pHandler = 0);
		/**
		* 配置时钟消息处理器。消息xmMSGTimerClick，使用xmEEC_MSG_WORLD_TIMER。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigTSCHandler(xmMsgHandler* pHandler = 0);

		/**
		* 配置数据同步时钟消息处理器。消息xmMSGValueSyncTimerClick，使用xmEEC_MSG_VALUE_SYNC_TIMER。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功， - 1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigVSTHandler(xmMsgHandler* pHandler = 0);
		/**
		* 配置数据处理过程消息处理器。消息xmMSGDPC（待实现），使用xmEEC_MSG_DPC。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigDPCHandler(xmMsgHandler* pHandler = 0);
		/**
		* 配置数据集内存块同步消息处理器。消息xmMSGMemorySync，使用xmEEC_MSG_MEMORY_SYNC。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigMSyncHandler(xmMsgHandler* pHandler = 0);
		/**
		* 配置系统状态切换消息处理器。消息xmMSGSwitchState，使用xmEEC_MSG_SYSTEM_STATE_SWITCH、xmEEC_MSG_SWITCH_NORMAL、xmEEC_MSG_SWITCH_REPLAY。
		* 系统状态目前就正常、回放两种状态。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigSSSHandler(xmMsgHandler* pHandler = 0);
		/**
		* 配置公告板消息处理器。消息xmMSGBulletin，使用xmEEC_MSG_BULLETIN。
		* @param strTopic 根据公告主题进行订阅。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigBulletinWithTopicHandler(const char* strTopic, xmMsgHandler* pHandler = 0);
		/**
		* 配置公告板消息处理器。消息xmMSGBulletin，使用xmEEC_MSG_BULLETIN。
		* @param strTopic 根据公告发送者名字进行订阅。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigBulletinWithSenderHandler(const char* strSender, xmMsgHandler* pHandler = 0);
		/**
		* 配置节点间主动消息的处理器。消息xmMSGMCast，使用xmEEC_MSG_MCAST。
		* 由发送端决定需要发送给那些节点。
		* @param pHandler 敏感消息处理器，当为0时，表示使用默认的消息处理器。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            ConfigMCastMSGHandler(xmMsgHandler* pHandler = 0);
		/**
		* 删除消息处理器。
		* @param pHandler 将要删除的敏感消息处理器指针。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            RemoveHandler(xmMsgHandler* pHandler);
		/**
		* 使用.号分割名字,xmValue获取数值，使用时可以只使用最顶层的数据名字。
		* @param strDataName 数据名字。
		* @param bValue 以xmValue形式存储的参数值。
		* @param flag  在结构化数据中名字路径分割符号，默认均为“.”。
		* @return 结果，大于0表示成功，返回为所获取的字节数，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetValue(const char* strDataName, xmValue& bValue, const char* flag = ".");
		/**
		* 使用.号分割名字,char*形式获取数值，使用时可以只使用最顶层的数据名字。
		* @param strDataName 数据名字。
		* @param bValue 以char*形式存储参数值的缓冲区地址。
		* @param uLength 获取数据值的大小，例如unsigned char为1个字节，int为4个字节，float为4个字节，以此类推。
		* @param flag  在结构化数据中名字路径分割符号，默认均为“.”。
		* @return 结果，大于0表示成功，返回为所获取的字节数，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetValue(const char* strDataName, char* bValue, unsigned int uLength, const char* flag = ".");
		/**
		* 使用.号分割名字,模板形式获取数值，使用时可以只使用最顶层的数据名字。
		* @param strDataName 数据名字。
		* @param bValue 以模板形式存储参数值。
		* @param flag  在结构化数据中名字路径分割符号，默认均为“.”。
		* @return 结果，大于0表示成功，返回为所获取的字节数，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		template<class T>
		int GetValue(const char* strDataName, T& Value, const char* flag = ".")
		{
			return GetValue(strDataName, (char*)&Value, sizeof(T), flag);
		}
		/**
		* 判断数据是否有更新。
		* @param strDataName 数据名字。
		* @return 结果，0表示未更新，大于等于1表示更新，-1表示失败。
		*/
		virtual int            IsRefreshed(const char* strDataName);
		/**
		* 使用.号分割名字,xmValue写数值，使用时可以只使用最顶层的数据名字。
		* @param strDataName 数据名字。
		* @param bValue 以xmValue形式存储的参数值。
		* @param flag  在结构化数据中名字路径分割符号，默认均为“.”。
		* @return 结果，大于0表示成功，返回为所设置的字节数，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            SetValue(const char* strDataName, const xmValue& bValue, const char* flag = ".");
		/**
		* 使用.号分割名字,char*形式写数值，使用时可以只使用最顶层的数据名字。
		* @param strDataName 数据名字。
		* @param bValue 以char*形式存储参数值的缓冲区地址。
		* @param uLength 写数据值的大小，例如unsigned char为1个字节，int为4个字节，float为4个字节，以此类推。
		* @param flag  在结构化数据中名字路径分割符号，默认均为“.”。
		* @return 结果，大于0表示成功，返回为所设置的字节数，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            SetValue(const char* strDataName, const char* bValue, unsigned int uLength, const char* flag = ".");
		/**
		* 使用.号分割名字,模板形式写数值，使用时可以只使用最顶层的数据名字。
		* @param strDataName 数据名字。
		* @param bValue 以模板形式存储参数值。
		* @param flag  在结构化数据中名字路径分割符号，默认均为“.”。
		* @return 结果，大于0表示成功，返回为所设置的字节数，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		template<class T>
		int SetValue(const char* strDataName, const T& Value, const char* flag = ".")
		{
			return SetValue(strDataName, (char*)&Value, sizeof(T), flag);
		}

		/**
		* 将公告信息向公告板粘贴。
		* @param strSender 发送者。
		* @param strTopic 公告主题。
		* @param pBuf  公告缓存地址。
		* @param uLength  公告信息长度。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            Paste2Bulletin(const char* strSender, const char* strTopic, const char* pBuf, unsigned int uLength);
		/**
		* 将特定节点发送消息。
		* @param strSender 发送者。
		* @param desNodes 接收消息的节点名字，可以为多个节点名字。
		* @param pBuf  消息缓存地址。
		* @param uLength  消息长度。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            MCast2Nodes(const char* strSender, const std::vector<std::string>& desNodes, const char* pBuf, unsigned int uLength);

		/**
		* 订购数据，返回订单号，在使用数据前，先订购需要更新的数据值，使用数据时可以通过订单号获取所有的数据值信息。
		* @param vecData 数据名字。
		* @return 订单号。
		*/
		virtual int            Subscribe(std::vector<std::string> vecData);
		/**
		* 完成订阅初始化，准备接收订阅数据。
		* @param nSubscribeID 订单号。
		* @param nLength 订单所需接收的缓冲区长度，调用后返回值。
		* @return 结果，如果为0标识成功，-1表示失败，大于0表明第几个参数不行，需要用户根据逻辑判断是否正确。
		*/
		virtual int            GenerateOrder(int nSubscribeID, int& nLength);
		/**
		* 根据订单号取消定购。
		* @param nSubscribeID 订单号。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            UnSubscribe(int nSubscribeID);
		/**
		* 取消所有的订购。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            UnSubscribeAll();
		/**
		* 获取所有订单号。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetSubscribe(std::vector<int>& vSubscribeID);
		/**
		* 根据订单号获取订单中数据名字。
		* @param nSubscribeID 订单号。
		* @param vecData 订单号nSubscribeID中所有的数据信息。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetSubscribeDetail(int nSubscribeID, std::vector<std::string>& vecData);
		/**
		* 根据订单号，获取整个订单的值，以结构化数据形式输出。待实现。
		* @param nSubscribeID 订单号。
		* @param val 整个订单的值。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetSubscribeValue(int nSubscribeID, xmValue& val);
		/**
		* 根据订单号，获取整个订单的值，以缓存形式输出。
		* @param nSubscribeID 订单号。
		* @param pVal 整个订单的值的缓冲区地址。
		* @param nLength 整个订单的值的缓冲区大小。
		* @return 结果，0表示成功，-1表示失败，其他代码表示需要用户根据逻辑判断是否正确。
		*/
		virtual int            GetSubscribeValue(int nSubscribeID, char* pVal, int& nLength);

	private:
		xmVDR*                 m_pAttchedVDR = 0;
		xmEAPAuthority         m_eAPAType;
	};
}

#endif //__SSA_VDR_ACCESSPOINT_H