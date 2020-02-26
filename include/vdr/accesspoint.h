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
	*  VDR�û����ʽӿ�. �û�ʹ�÷��ʽӿ�����VDR���������л�ȡ��ط���Ľ�������������
	*  1.���ݶ���д��
	*  2.������Ϣ��
	*  3.������Ϣ��
	*  4.Ⱥ����Ϣ��
	*  5.�������ĵȡ�
	*/
	class xmVDR_EXPORT xmAccessPoint
	{
	public:
		xmAccessPoint(void);
		virtual ~xmAccessPoint(void);
		/**
		* ���ݷ��ʽӿ�����VDR��ʹ�����ݷ��ʽӿ�ʱ������Ҫ��һ��VDR���ӡ�
		* @param pVdr ��ָ�����ӵ�VDRָ�롣
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Connect(xmVDR* pVdr);
		/**
		* ���ݷ��ʽӿ�����VDR��ʹ�����ݷ��ʽӿ�ʱ������Ҫ��һ��VDR���ӡ�
		* @param pVdrAttr ��ָ�����ӵ�VDR����ָ�롣
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Connect(xmVDRAttr* pVdrAttr);
		/**
		* ���ݷ��ʽӿ�����VDR��ʹ�����ݷ��ʽӿ�ʱ������Ҫ��һ��VDR���ӡ�
		* @param uVdrID ��ָ�����ӵ�VDR��ID�š�
		* @param uRegion ��ָ�����ӵ�VDR���е�������
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Connect(unsigned short uVdrID, unsigned short uRegion);
		/**
		* ���ݷ��ʽӿڻ�ȡ�����ӵ�VDRָ�롣
		* @return ���ӽ������ʾ�����ӵ�VDRָ�롣
		*/
		virtual xmVDR*         ConnectedVDR();
		/**
		* ���ݷ��ʽӿ��˳�����㣬��ʱ�����˳�VDR��VDR��Ȼ���ڣ������ٴ����ӷ��ʽӿڡ�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            Quit(void);

		/**
		* ���ݷ��ʽӿڻ�ȡ�ڵ���VDR���ݵĲ���Ȩ����Ȩ������ڵ���Խ�����صĲ���������ȨҲ����ͨ�����ݼ�ֱ�Ӳ�����ȡ��
		* ��ȡ��Ȩ�����ǻ�ȡ�����ݼ��Ĳ��������ڲ�ʵ���Ϸ�Ϊ�������裬һ�ǲ�ѯ�ڵ������ӵ����ݼ��������������ڵ�����ݼ���
		* @param strNodeName ��ָ������Ľڵ�����֡�
		* @return ��Ȩ�����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Authorize(const char* strNodeName);
		/**
		* ���ݷ��ʽӿ�ȡ���ڵ���VDR�Ĳ���Ȩ����Ȩ��
		* @param strNodeName ��ָ������Ľڵ�����֡�
		* @return ȡ����Ȩ�����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           UnAuthorize(const char* strNodeName);
		/**
		* ��ȡ���ݷ��ʽӿ���Ȩ�Ľڵ㡣
		* @return ��Ȩ�ڵ��������ڵ���Ϣ��MAP��
		*/
		std::map<std::string, xmNode>&    AuthorizedNodes();
		/**
		* ��ڵ���Ȩ���ƣ���ȡ��Ȩ�����ǻ�ȡ�����ݼ��Ĳ������˽ӿ����ڽ����ݷ��ʽӿ����������ݼ��ϡ�
		* Ҳ����ͨ��Node��Authorize������Ȩ���Ƽ��÷���ͨ��Node������Ȩ��
		* @param pVdr ��ָ��������VDRָ�롣
		* @param strDataSetName ��ָ�����������ݼ������֡�
		* @param nTry ��ָ�����Զ��ٴΡ�
		* @return ���������true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Attach(xmVDR* pVdr, const char* strDataSetName = "", int nTry = 65535);
		/**
		* ��ڵ���Ȩ���ƣ���ȡ��Ȩ�����ǻ�ȡ�����ݼ��Ĳ������˽ӿ����ڽ����ݷ��ʽӿ����������ݼ��ϡ�
		* Ҳ����ͨ��Node��Authorize������Ȩ���Ƽ��÷���ͨ��Node������Ȩ��
		* @param strDataSetName ��ָ�����������ݼ������֡�
		* @param nTry ��ָ�����Զ��ٴΡ�
		* @return ���������true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Attach(const char* strDataSetName, int nTry = 65535);
		/**
		* ȡ�����ݷ��ʽӿڶ����ݼ���������
		* @param strDataSetName ��ָ�����������ݼ������֡�
		* @return ȡ�����������true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           UnAttach(const char* strDataSetName);
		/**
		* ��ȡ���ݷ��ʽӿ����������ݼ�����Ϣ��
		* @return ���������ݼ������������ݼ�����Ϣ��MAP��
		*/
		std::map<std::string, xmDataSet>&  AttachedDataSets();
		/**
		* �ж����ݷ��ʽӿ��Ƿ��Ѿ���Ч������
		* @return ���������true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		bool                   IsValid();
		/**
		* ���ڱ�����㣬ʹĳ�����ݼ��߱����߲��߱���������ֵ������,ͨ�������ڻطŻ�����������״̬ʱ����ʹ�á�
		* @param strDataSetName ��ָ���ݼ������֡�
		* @param havePublishAbility ��ָ���ݼ��Ƿ���з�������ֵ��������
		* @return �ޡ�
		*/
		void                   SetPublishAbility(const char* strDataSetName, bool havePublishAbility);

		/**
		* ���ڱ�����㣬ʹĳ�����ݼ��߱����߲��߱��Զ���������ֵ��������
		* @param strDataSetName ��ָ���ݼ������֡�
		* @param autoPublishSign ��ָ���ݼ��Ƿ�������ڲ�ʱ�ӻ��Ʒ�������ֵ�����������Ϊtrue��ʾ��ʱ��������false��ʾ�ֶ�������ֵͬ����
		* @return �ޡ�
		*/
		void                   SetAutoPublishSign(const char* strDataSetName, bool autoPublishSign);
		/**
		* ���ϵͳ�д��ڶ���ڵ�ͬʱдһ�����ݻ������ݼ������������Ҫ��ȡȨ�ޣ�������д֮��ĳ�ͻ���⡣
		* @param strDataSetName ��ָͬʱд�����ݼ������֡�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           GetToken(const char* strDataSetName = "");
		/**
		* �ֶ�ͬ�������źţ���Ϊ���������һ������Ҫ�ϴ��µ����ݣ�ͬʱ��ȡ�µ��ڴ����ݣ�һ�����������ȡ�������˵����ݡ�
		* @param am ͬ�����򣬰����˽��������������ͻ��ˣ�˫����߹㲥��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            SyncContent(xmESyncDirection am = xmESyncDirection::xmESD_TOSERVER);

		/**
		* ��ȡ�ڵ���Ŀ��
		* @return �ڵ������
		*/
		virtual int            GetNodeCount(void);
		/**
		* �����ڲ�����˳���ȡ�����ڵ��������Ϣ��
		* @param uIndex ����˳�򣬴�GetNodeCount��ȡ��������ֵ����0��ʼ������
		* @param nd �ڵ�������Ϣ���ã��������سɹ�ʱ��nd��ʾ��ȡ�Ľڵ���Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetNode(unsigned int uIndex, xmNode& nd);
		/**
		* �������ֻ�ȡ�����ڵ��������Ϣ��
		* @param strNodeName �ڵ����֡�
		* @param nd �ڵ�������Ϣ���ã��������سɹ�ʱ��nd��ʾ��ȡ�Ľڵ���Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetNode(const char* strNodeName, xmNode& nd);
		/**
		* ��ȡ���ݼ���Ŀ��
		* @return ���ݼ�������
		*/
		virtual int            GetDataSetCount(void);
		/**
		* �����ڲ�����˳���ȡ�������ݼ���������Ϣ��
		* @param uIndex ����˳�򣬴�GetDataSetCount��ȡ��������ֵ����0��ʼ������
		* @param ds ���ݼ�������Ϣ���ã��������سɹ�ʱ��ds��ʾ��ȡ�����ݼ���Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetDataSet(unsigned int uIndex, xmDataSet& ds);
		/**
		* �������ֻ�ȡ�������ݼ���������Ϣ��
		* @param strDataSetName ���ݼ����֡�
		* @param ds ���ݼ�������Ϣ���ã��������سɹ�ʱ��ds��ʾ��ȡ�����ݼ���Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetDataSet(const char* strDataSetName, xmDataSet& ds);
		/**
		* �������ֻ�ȡ�������ݼ���������Ϣ���������ݼ��ڲ���������������Ϣ��
		* @param strDataSetName ���ݼ����֡�
		* @param ds ���ݼ�������Ϣ���ã��������سɹ�ʱ��ds��ʾ��ȡ�����ݼ���Ϣ��
		* @param vData ����������Ϣ���ã��������سɹ�ʱ��vData��ʾ���ݼ��е�ǰ������������������Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetDataSet(const char* strDataSetName, xmDataSet& ds, std::vector<xmData>& vData);
		/**
		* �������ݼ����ֻ�ȡ���ݼ���Ӧ�Ĵ洢�ռ䳤�ȣ����ڴ���д�ڴ�����ʱʹ�á�
		* @param strDataSetName ���ݼ����֡�
		* @return �����-1��ʾʧ�ܣ����������ʾ��ȷ��ȡ�ڴ�ĳ��ȡ�
		*/
		virtual int            GetDataSetMemoryLength(const char* strDataSetName);
		/**
		* �������ݼ����֣���ʼλ�ã��ʹ�С��ȡ���ݼ��ڴ����ݡ�
		* @param strDataSetName ���ݼ����֡�
		* @param nPos ���ڴ���ʼλ�á�
		* @param pOut �ڴ��������������ַ��
		* @param nLength �ڴ���������ȡ�
		* @return �����-1��ʾʧ�ܣ����������ʾ��ȷ��ȡ�ڴ�ĳ��ȡ�
		*/
		virtual int            GetDataSetMemory(const char* strDataSetName, int nPos, char* pOut, int nLength);
		/**
		* �������ݼ����֣���ʼλ�ã��ʹ�Сд���ݼ��ڴ����ݡ�
		* @param strDataSetName ���ݼ����֡�
		* @param nPos ���ڴ���ʼλ�á�
		* @param pOut �ڴ�д�������뻺���ַ��
		* @param nLength �ڴ�д�������ȡ�
		* @return �����-1��ʾʧ�ܣ����������ʾ��ȷд�ڴ�ĳ��ȡ�
		*/
		virtual int            SetDataSetMemory(const char* strDataSetName, int nPos, char* pIn, int nLength);
		/**
		* ��ȡ������Ŀ��
		* @return ���ݸ�����
		*/
		virtual int            GetDataCount();
		/**
		* �����ڲ�����˳���ȡ�������ݵ�������Ϣ��
		* @param uIndex ����˳�򣬴�GetDataCount��ȡ��������ֵ����0��ʼ������
		* @param data ����������Ϣ���ã��������سɹ�ʱ��data��ʾ��ȡ��������Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetData(unsigned int uIndex, xmData& data);
		/**
		* �������ֻ�ȡ�������ݵ�������Ϣ��
		* @param strDataName �������֡�
		* @param data ����������Ϣ���ã��������سɹ�ʱ��data��ʾ��ȡ��������Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetData(const char* strDataName, xmData& data);
		/**
		* ���������������ݼ��ϡ�
		* @param strDataName �������֡�
		* @return �������ݼ������֡�
		*/
		virtual const char*    BelongTo(const char* strDataName);


		/**
		* ����Ĭ��������Ϣ�����м����
		* @param  nProxyID ������Ϣ�м����ID�š�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ�δ�ҵ��������������ʾ��Ҫ�û��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigSensProxy(int nProxyID, const char* dllName = "");

		/**
		* ����Ĭ��������Ϣ��������
		* @param pHandler ������Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigDefaultHandler(xmMsgHandler* pHandler);
		/**
		* ���ýڵ���Ϣ����������ϢxmMSGNodeEvent��ʹ��xmEEC_MSG_NODE_EVENT��
		* @param neNodeEvent ���нڵ���Ϣ��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigNodeEventHandler(xmENodeEvent neNodeEvent, xmMsgHandler* pHandler = 0);
		/**
		* �������ݼ���Ϣ����������ϢxmMSGDataSetEvent��ʹ��xmEEC_MSG_DATASET_EVENT��
		* @param neDataSetEvent �������ݼ���Ϣ��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigDataSetEventHandler(xmEDataSetEvent neDataSetEvent, xmMsgHandler* pHandler = 0);
		/**
		* ����������Ϣ����������ϢxmMSGDataEvent��ʹ��xmEEC_MSG_DATA_EVENT��
		* ����Ϣ�������������ݵ���Ϣ�����ã���������ļ�����Դ�������ã�ͨ��ʹ��ָ���������ֽӿڡ�
		* @param deDataEvent ����������Ϣ��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigDataEventHandler(xmEDataEvent deDataEvent, xmMsgHandler* pHandler = 0);
		/**
		* ����������Ϣ����������ϢxmMSGDataEvent��ʹ��xmEEC_MSG_DATA_EVENT��
		* ����Ϣ������������������������Ϣ�����á�
		* @param strDataName ��������
		* @param deDataEvent ����������Ϣ��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigDataEventHandler(const char* strDataName, xmEDataEvent deDataEvent, xmMsgHandler* pHandler = 0);
		/**
		* ����ʱ����Ϣ����������ϢxmMSGTimerClick��ʹ��xmEEC_MSG_WORLD_TIMER��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigTSCHandler(xmMsgHandler* pHandler = 0);

		/**
		* ��������ͬ��ʱ����Ϣ����������ϢxmMSGValueSyncTimerClick��ʹ��xmEEC_MSG_VALUE_SYNC_TIMER��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ��� - 1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigVSTHandler(xmMsgHandler* pHandler = 0);
		/**
		* �������ݴ��������Ϣ����������ϢxmMSGDPC����ʵ�֣���ʹ��xmEEC_MSG_DPC��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigDPCHandler(xmMsgHandler* pHandler = 0);
		/**
		* �������ݼ��ڴ��ͬ����Ϣ����������ϢxmMSGMemorySync��ʹ��xmEEC_MSG_MEMORY_SYNC��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigMSyncHandler(xmMsgHandler* pHandler = 0);
		/**
		* ����ϵͳ״̬�л���Ϣ����������ϢxmMSGSwitchState��ʹ��xmEEC_MSG_SYSTEM_STATE_SWITCH��xmEEC_MSG_SWITCH_NORMAL��xmEEC_MSG_SWITCH_REPLAY��
		* ϵͳ״̬Ŀǰ���������ط�����״̬��
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigSSSHandler(xmMsgHandler* pHandler = 0);
		/**
		* ���ù������Ϣ����������ϢxmMSGBulletin��ʹ��xmEEC_MSG_BULLETIN��
		* @param strTopic ���ݹ���������ж��ġ�
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigBulletinWithTopicHandler(const char* strTopic, xmMsgHandler* pHandler = 0);
		/**
		* ���ù������Ϣ����������ϢxmMSGBulletin��ʹ��xmEEC_MSG_BULLETIN��
		* @param strTopic ���ݹ��淢�������ֽ��ж��ġ�
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigBulletinWithSenderHandler(const char* strSender, xmMsgHandler* pHandler = 0);
		/**
		* ���ýڵ��������Ϣ�Ĵ���������ϢxmMSGMCast��ʹ��xmEEC_MSG_MCAST��
		* �ɷ��Ͷ˾�����Ҫ���͸���Щ�ڵ㡣
		* @param pHandler ������Ϣ����������Ϊ0ʱ����ʾʹ��Ĭ�ϵ���Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigMCastMSGHandler(xmMsgHandler* pHandler = 0);
		/**
		* ɾ����Ϣ��������
		* @param pHandler ��Ҫɾ����������Ϣ������ָ�롣
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            RemoveHandler(xmMsgHandler* pHandler);
		/**
		* ʹ��.�ŷָ�����,xmValue��ȡ��ֵ��ʹ��ʱ����ֻʹ�������������֡�
		* @param strDataName �������֡�
		* @param bValue ��xmValue��ʽ�洢�Ĳ���ֵ��
		* @param flag  �ڽṹ������������·���ָ���ţ�Ĭ�Ͼ�Ϊ��.����
		* @return ���������0��ʾ�ɹ�������Ϊ����ȡ���ֽ�����-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetValue(const char* strDataName, xmValue& bValue, const char* flag = ".");
		/**
		* ʹ��.�ŷָ�����,char*��ʽ��ȡ��ֵ��ʹ��ʱ����ֻʹ�������������֡�
		* @param strDataName �������֡�
		* @param bValue ��char*��ʽ�洢����ֵ�Ļ�������ַ��
		* @param uLength ��ȡ����ֵ�Ĵ�С������unsigned charΪ1���ֽڣ�intΪ4���ֽڣ�floatΪ4���ֽڣ��Դ����ơ�
		* @param flag  �ڽṹ������������·���ָ���ţ�Ĭ�Ͼ�Ϊ��.����
		* @return ���������0��ʾ�ɹ�������Ϊ����ȡ���ֽ�����-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetValue(const char* strDataName, char* bValue, unsigned int uLength, const char* flag = ".");
		/**
		* ʹ��.�ŷָ�����,ģ����ʽ��ȡ��ֵ��ʹ��ʱ����ֻʹ�������������֡�
		* @param strDataName �������֡�
		* @param bValue ��ģ����ʽ�洢����ֵ��
		* @param flag  �ڽṹ������������·���ָ���ţ�Ĭ�Ͼ�Ϊ��.����
		* @return ���������0��ʾ�ɹ�������Ϊ����ȡ���ֽ�����-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		template<class T>
		int GetValue(const char* strDataName, T& Value, const char* flag = ".")
		{
			return GetValue(strDataName, (char*)&Value, sizeof(T), flag);
		}
		/**
		* �ж������Ƿ��и��¡�
		* @param strDataName �������֡�
		* @return �����0��ʾδ���£����ڵ���1��ʾ���£�-1��ʾʧ�ܡ�
		*/
		virtual int            IsRefreshed(const char* strDataName);
		/**
		* ʹ��.�ŷָ�����,xmValueд��ֵ��ʹ��ʱ����ֻʹ�������������֡�
		* @param strDataName �������֡�
		* @param bValue ��xmValue��ʽ�洢�Ĳ���ֵ��
		* @param flag  �ڽṹ������������·���ָ���ţ�Ĭ�Ͼ�Ϊ��.����
		* @return ���������0��ʾ�ɹ�������Ϊ�����õ��ֽ�����-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            SetValue(const char* strDataName, const xmValue& bValue, const char* flag = ".");
		/**
		* ʹ��.�ŷָ�����,char*��ʽд��ֵ��ʹ��ʱ����ֻʹ�������������֡�
		* @param strDataName �������֡�
		* @param bValue ��char*��ʽ�洢����ֵ�Ļ�������ַ��
		* @param uLength д����ֵ�Ĵ�С������unsigned charΪ1���ֽڣ�intΪ4���ֽڣ�floatΪ4���ֽڣ��Դ����ơ�
		* @param flag  �ڽṹ������������·���ָ���ţ�Ĭ�Ͼ�Ϊ��.����
		* @return ���������0��ʾ�ɹ�������Ϊ�����õ��ֽ�����-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            SetValue(const char* strDataName, const char* bValue, unsigned int uLength, const char* flag = ".");
		/**
		* ʹ��.�ŷָ�����,ģ����ʽд��ֵ��ʹ��ʱ����ֻʹ�������������֡�
		* @param strDataName �������֡�
		* @param bValue ��ģ����ʽ�洢����ֵ��
		* @param flag  �ڽṹ������������·���ָ���ţ�Ĭ�Ͼ�Ϊ��.����
		* @return ���������0��ʾ�ɹ�������Ϊ�����õ��ֽ�����-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		template<class T>
		int SetValue(const char* strDataName, const T& Value, const char* flag = ".")
		{
			return SetValue(strDataName, (char*)&Value, sizeof(T), flag);
		}

		/**
		* ��������Ϣ�򹫸��ճ����
		* @param strSender �����ߡ�
		* @param strTopic �������⡣
		* @param pBuf  ���滺���ַ��
		* @param uLength  ������Ϣ���ȡ�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            Paste2Bulletin(const char* strSender, const char* strTopic, const char* pBuf, unsigned int uLength);
		/**
		* ���ض��ڵ㷢����Ϣ��
		* @param strSender �����ߡ�
		* @param desNodes ������Ϣ�Ľڵ����֣�����Ϊ����ڵ����֡�
		* @param pBuf  ��Ϣ�����ַ��
		* @param uLength  ��Ϣ���ȡ�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            MCast2Nodes(const char* strSender, const std::vector<std::string>& desNodes, const char* pBuf, unsigned int uLength);

		/**
		* �������ݣ����ض����ţ���ʹ������ǰ���ȶ�����Ҫ���µ�����ֵ��ʹ������ʱ����ͨ�������Ż�ȡ���е�����ֵ��Ϣ��
		* @param vecData �������֡�
		* @return �����š�
		*/
		virtual int            Subscribe(std::vector<std::string> vecData);
		/**
		* ��ɶ��ĳ�ʼ����׼�����ն������ݡ�
		* @param nSubscribeID �����š�
		* @param nLength ����������յĻ��������ȣ����ú󷵻�ֵ��
		* @return ��������Ϊ0��ʶ�ɹ���-1��ʾʧ�ܣ�����0�����ڼ����������У���Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GenerateOrder(int nSubscribeID, int& nLength);
		/**
		* ���ݶ�����ȡ��������
		* @param nSubscribeID �����š�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            UnSubscribe(int nSubscribeID);
		/**
		* ȡ�����еĶ�����
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            UnSubscribeAll();
		/**
		* ��ȡ���ж����š�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetSubscribe(std::vector<int>& vSubscribeID);
		/**
		* ���ݶ����Ż�ȡ�������������֡�
		* @param nSubscribeID �����š�
		* @param vecData ������nSubscribeID�����е�������Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetSubscribeDetail(int nSubscribeID, std::vector<std::string>& vecData);
		/**
		* ���ݶ����ţ���ȡ����������ֵ���Խṹ��������ʽ�������ʵ�֡�
		* @param nSubscribeID �����š�
		* @param val ����������ֵ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetSubscribeValue(int nSubscribeID, xmValue& val);
		/**
		* ���ݶ����ţ���ȡ����������ֵ���Ի�����ʽ�����
		* @param nSubscribeID �����š�
		* @param pVal ����������ֵ�Ļ�������ַ��
		* @param nLength ����������ֵ�Ļ�������С��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetSubscribeValue(int nSubscribeID, char* pVal, int& nLength);

	private:
		xmVDR*                 m_pAttchedVDR = 0;
		xmEAPAuthority         m_eAPAType;
	};
}

#endif //__SSA_VDR_ACCESSPOINT_H