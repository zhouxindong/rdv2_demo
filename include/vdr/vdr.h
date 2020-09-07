#ifndef __SSA_VDR_H
#define __SSA_VDR_H

#include "export.h"
#include "data.h"
#include "dataset.h"
#include "icommproxy.h"
#include "iservice.h"
#include "vdrattr.h"
#include "systemhandler.h"

#ifdef xmMODE_RELEASE
#	pragma warning(disable:4482)
#endif

namespace ssa
{
	/**
	*  VDRϵͳ���ýӿڣ�����ϵͳ���ʹ�ã�������µĹ�����
	*  1.VDR�ṹ�����ɣ�
	*  2.ϵͳ��־��Ϣ���ã�
	*  3.ϵͳ�ṹ��ά����
	*  4.ʱ����Ϣ������
	*  5.ϵͳ���á�
	*/
	/*����ֵ�Ķ��壬�����õ��ķ���ֵΪint�ģ�ͨ����ʹ������ö��
	typedef enum __tagRspCodes
	{
	xmRSP_ACK_BEGIN = 10000, //xmEPackCodes::xmPACK_MAX,
	xmRSP_ACK_OK,            //10001  �ɹ�
	xmRSP_DATASET_EXIST,     //10002  ���ݼ��Ѵ���
	xmRSP_DATASET_NOT_EXIST, //10003  ���ݼ�������
	xmRSP_DATA_EXIST,        //10004  �����Ѵ���
	xmRSP_DATA_NOT_EXIST,    //10005  ���ݲ�����
	xmRSP_DATASET_REGION,
	xmRSP_DATASET_NOT_READY, //10007  ���ݼ�δ׼����
	xmRSP_REPLAY_STATE,
	xmRSP_NODE_EXIST,        //10009  �ڵ��Ѵ���
	xmRSP_NODE_NOT_EXIST,    //10010  �ڵ㲻����
	xmRSP_BUILD_INDEX_FAILED,//10011  ��������ʧ��
	}xmERspCodes;
	*/

	/**
	*  WARNING��
	*  �ڴ�����ɺ�,���п������ⲿ�������ݣ�����Ҫ�ȴ���Ӧ��Ϣ�Ľӿڽ�������Ϣ�ص�������ʹ��,��Ϊ��Ϣ���е�Ƕ�׵ȴ������ܻ������Ϣ��ӵ����
	*  �磺AddNode��AddDataSet��AddData�Լ���Ӧ��ɾ���Ȳ��������������Ӧ��ͨ��ConfigNodeEventHandler��ConfigDataSetEventHandler��ConfigDataEventHandler�����õ���Ϣ�����߳��е��á�
	*  �磺SyncContent�ڷ���ָ��ΪxmESyncDirection::xmESD_TOCLIENTʱ�����������Ӧ��ConfigMSyncHandler��ConfigDPCHandler�����õ�ֵ���»ص���Ϣ�����߳��е��á�
	*/

	class xmVDR_EXPORT xmVDR
	{
	private:
		xmVDR(const xmVDR& aVDR);
		
	public:
		xmVDR(void);
		virtual ~xmVDR(void);
		/**
		* ��ȡ��ǰVDRSDK�İ汾��Ϣ��
		* @return �汾��Ϣ����λ��ʾһ���汾�����磺010101��ʾvision1.1.1��011221��ʾvision1.12.21��
		*/
		static  int            Version();
		/**
		* �жϵ�ǰVDR�Ƿ���׼���ÿ���ʹ�á�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		bool                   IsValid();
		/**
		* ʹ��vdr����������Ϣ������VDR��
		* @param pVdrAttr VDR���ԡ�
		* @param bFetchInfo,��ʾ�ڴ���VDRʱ���Ƿ���Ҫ��������������Ϣ��
		* @param bUseNewest,�½�VDR��ÿ��VDRʹ��ǰ�����½�����ָ��ģʽ������Ѿ�������ͬID��VDR���Ƿ������µĸ��ǡ�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Create(xmVDRAttr* pVdrAttr, bool bFetchInfo = true, bool bUseNewest = false);
		/**
		* ����VDR��
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Destroy();
		/**
		* ��ȡVDR���ԡ�
		* @return VDR����ָ�롣
		*/
		virtual xmVDRAttr*     GetVDRAttr(void);
		/**
		* �л�ϵͳ״̬����״̬��ģ�����п������ڻط�ʹ�ã���״̬�£�ϵͳ���е����ݼ��Ͼ�ת�����µĴ洢����
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            SystemState2Replay(void);
		/**
		* ϵͳ״̬�л���������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            SystemState2Normal(void);
		/**
		* ��ȡ��ǰϵͳ����״̬��
		* @return ϵͳ����״̬��
		*/
		virtual xmESystemState GetSystemState(void);
		/**
		* ����Log��Ϣ���������������е���־��Ϣ���û���ͨ�������о��������ʾ��־��
		* @param pHandler ��Ϣ��������
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigLogHandler(xmMsgHandler* pHandler);
		/**
		* ��VDR���ӽڵ㡣
		* @param nd �ڵ���Ϣ��
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            AddNode(xmNode& nd);
		/**
		* ɾ��VDR�ڵ㡣
		* @param strNodeName �ڵ����֡�
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            RemoveNode(const char* strNodeName);
		/**
		* ��ȡVDR�ڵ���Ŀ��
		* @return ��ǰVDR�еĽڵ�����
		*/
		virtual unsigned int   GetNodeCount(void);
		/**
		* �����ڲ�����˳���ȡ�����ڵ��������Ϣ��
		* @param uIndex ����˳�򣬴�GetNodeCount��ȡ��������ֵ����0��ʼ������
		* @param nd �ڵ�������Ϣ���ã��������سɹ�ʱ��nd��ʾ��ȡ�Ľڵ���Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetNode(unsigned int uIndex, xmNode& nd);
		/**
		* �������ֻ�ȡ�����ڵ��������Ϣ��
		* @param strName �ڵ����֡�
		* @param nd �ڵ�������Ϣ���ã��������سɹ�ʱ��nd��ʾ��ȡ�Ľڵ���Ϣ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            GetNode(const char* strNodeName, xmNode& nd);
		/**
		* ��VDR�������ݼ���
		* @param nd ���ݼ���Ϣ��
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            AddDataSet(xmDataSet& ds);
		/**
		* ɾ��VDR�����ݼ���
		* @param strDataSetName ���ݼ����֡�
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            RemoveDataSet(const char* strDataSetName);
		/**
		* ��ȡ���ݼ���Ŀ��
		* @return ���ݼ�������
		*/
		virtual unsigned int   GetDataSetCount(void);
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
		virtual unsigned int   GetDataSetMemoryLength(const char* strDataSetName);
		/**
		* �������ݼ����֣���ʼλ�ã��ʹ�С��ȡ���ݼ��ڴ����ݡ�
		* @param strDataSetName ���ݼ����֡�
		* @param nPos ���ڴ���ʼλ�á�
		* @param pOut �ڴ��������������ַ��
		* @param nSize �ڴ���������ȡ�
		* @return �����-1��ʾʧ�ܣ����������ʾ��ȷ��ȡ�ڴ�ĳ��ȡ�
		*/
		virtual int            GetDataSetMemory(const char* strDataSetName,int nPos,char* pOut,int nSize);
		/**
		* �������ݼ����֣���ʼλ�ã��ʹ�Сд���ݼ��ڴ����ݡ�
		* @param strDataSetName ���ݼ����֡�
		* @param nPos ���ڴ���ʼλ�á�
		* @param pOut �ڴ�д�������뻺���ַ��
		* @param nSize �ڴ�д�������ȡ�
		* @return �����-1��ʾʧ�ܣ����������ʾ��ȷд�ڴ�ĳ��ȡ�
		*/
		virtual int            SetDataSetMemory(const char* strDataSetName, int nPos, char* pIn, int nSize);
		/**
		* �������ݼ����֣��ж����ݼ���ǰ�Ƿ��ڻ�Ծ״̬��
		* @param strDataSetName ���ݼ����֡�
		* @return �����0��ʾ���ڻ�Ծ״̬�����������ʾ���ٸ���������û�д��ڻ�Ծ״̬�ˡ�
		*/
		virtual long long      CheckActiveState(const char* strDataSetName);
		/**
		* �����ж����ݼ���ǰ�Ƿ��ڻ�Ծ״̬��ʱ������
		* @param nCircles ����ʱ������û���յ��µ���Ϣ����Ϊ���ݼ���������״̬,��ֵΪ0xFFFFFFFFʱ����ʾ����⡣
    	*/
		virtual void           SetUnActivePeriod(int nCircles);
		/**
		* ����Ĭ�ϵ�Qos��ʽ,1��ʾ��������2��ʾ����������ĿǰVDR��ʼ��ʱ����Ĭ�Ϸ�ʽ��Ϊ��2���������ݲ�����������
		* @param nQos ���ݴ���������
		*/
		virtual void           SetDefaultQos(int nQos);
		/**
		* ��VDR�������ݡ�
		* @param data ������Ϣ��ʹ��Ĭ�����ݴ���������
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            AddData(xmData& data);
		/**
		* ��VDR��������,�����˶����ݴ����������жϣ�Ŀǰ�������������Ͳ��������������
		* @param data  ������Ϣ��
		* @param nQos  ���ݴ���������Ŀǰ�����˲��ܶ���Ϣ�Ϳ��Զ���Ϣ���࣬1��ʾ����������ֵ�ࡢ״̬�ࣩ��2��ʾ����������ָ���ࣩ����ָ��Ϊ0ʱʹ��Ĭ�ϵķ�ʽ����
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            AddData(xmData& data, int nQos);
		/**
		* ɾ��VDR�����ݡ�
		* @param strDataName �������֡�
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            RemoveData(const char* strDataName);
		/**
		* ��ȡ������Ŀ��
		* @return ���ݸ�����
		*/
		virtual unsigned int   GetDataCount();
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
		* ���ӽڵ㼰���ݼ���
		* @param strNodeName �ڵ����֡�
		* @param strDataSetName ���ݼ����֡�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           Link(const char* strNodeName, const char* strDataSetName);
		/**
		* �Ͽ��ڵ㼰���ݼ����ӡ�
		* @param strNodeName �ڵ����֡�
		* @param strDataSetName ���ݼ����֡�
		* @return �����true��ʾ�ɹ���false��ʾʧ�ܡ�
		*/
		virtual bool           UnLink(const char* strNodeName, const char* strDataSetName);
		/**
		* Ϊ�˺���ǰ�ļ��ݣ��������������·������ݽṹ��ͨ���ڼ������ݺ󣬸��·������ͱ��ص����ݽṹ��
		* @param strDataSetName ���ݼ����֡�
		* @param nEndPoint      ��ʾĳһ�˸�����Ϣ��0��ʾ�������Ϳͻ������߶����£�1��ʾֻ�пͻ��˸��£�2��ʾֻ�з������˸��¡�
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            Update(const char* strDataSetName,int nEndPoint = 0);
		/**
		* �ӷ�������ȡ���µ�VDR��Ϣ������VDR��
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            FetchAllInfo();
		/**
		* �ӷ�������ȡ���µ����ݼ���Ϣ������VDR�����ݼ���Ϣ��
		* @param strDataSetName ���ݼ����֡�
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            FetchDSInfo(const char* strDataSetName);
		/**
		* �ӷ�������ȡ���µ����ݼ�ֵ������VDR�����ݼ�ֵ��
		* @param strDataSetName ���ݼ����֡�
		* @return �����xmERspCodes::xmRSP_ACK_OK(10001)��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            FetchDSValue(const char* strDataSetName);

		/**
		* �ж������Ƿ��и��¡�
		* @param strDataName �������֡�
		* @return �����0��ʾδ���£����ڵ���1��ʾ���£�-1��ʾʧ�ܡ�
		*/
		virtual int            IsRefreshed(const char* strDataName);
		/**
		* ����ֵͬ�����˴���ͬ���ǽ������Ѿ����ڵ����ݼ�����ͬ�����ڻط�״̬ʱ��Ч�����á�
		* @param strDataSetName ͬ�������ݼ����֡�
		* @param syncDir ���Ǹ��������ͬ�����������μ�xmESyncDirection˵����
		typedef enum __tagSyncDirection
		{
			xmESD_UNKOWN = 0,	        /**< ��ʾ�����巽��
			xmESD_TOCLIENT,		        /**< ��ĳһ���ͻ��˵��� 
			xmESD_TOCLIENTS,		    /**< ��ͻ����ǵ���
			xmESD_OUTDIRBEGIN,	        /**< ��ʾ���ⷽ��ģʽ��ʼ�� 
			xmESD_TOSERVER,		        /**< ����������� 
			xmESD_DOUBLEDIRECTION,		/**< ��˫�� 
			xmEAS_BROADCAST,		    /**< ֱ�������н����߹㲥�� 
		}xmESyncDirection;
		*
		* @param bUseRTE �Ƿ�ʹ��RTEģʽ��RTE��ָ�����ڴ�ģʽ��
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            SyncContent(xmESyncDirection syncDir, bool bUseRTE);
		virtual int            SyncContent(const char* strDataSetName, xmESyncDirection syncDir, bool bUseRTE);

		/**
		* ����һ��ʱ���źţ��������Ϊʱ�ӻ�����ͬ�����ڣ�ͬ��������ʱ�ӻ����ı�������ʾ����ʱ�ӻ����һ��ֵͬ����
		* @param nTickTime ʱ�ӻ�����
		* @param nSyncCycles ͬ�����ڡ�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û������߼��ж��Ƿ���ȷ��
		*/
		virtual int            TickSignal(int nTickTime, int nSyncCycles);

		/**
		* ��ȡ�źŵĵȴ�ʱ�䡣
		* @return �ȴ�ʱ�䣬�Ժ���Ϊ��λ��
		*/
		long                   GetWaitTime();
		/**
		* ������Ҫ�ȴ����źŵĵȴ�ʱ�䣬�Ժ���Ϊ��λ��
		* @param nTime �źŵĵȴ�ʱ�䡣
		*/
		void                   SetWaitTime(long nTime = xmMAX_WAITING_TIME);

		/**
		* ������־��ʾ����,-1��ʾ������ʾ��0��ʾ��ʾ0����1��ʾ1����.........����ʾ�������0��Խ��ʱ����ʾ����־��ϢԽȫ��
		* @param nlevel ��־����ʾ����
		*/
		void                   SetLogLevel(int nlevel);
		/**
		* ʹ�ú���ͬ���߼���
		* @param nProxyID ͬ���߼���ID�š�
		* @return �����0��ʾ�ɹ���-1��ʾʧ�ܣ����������ʾ��Ҫ�û��ж��Ƿ���ȷ��
		*/
		virtual int            ConfigSyncProxy(int nProxyID, const char* dllName = "");

		/**
		* �����Ƿ�ʹ�ñ���ʱ�ӽ��з������ڻص�,ϵͳĬ��ʹ�ñ���ʱ��������
		* @param bIsUseLocalTimer ����Ϊtrueʱ����ʾʹ�ñ���ʱ��������false��ʾʹ��ϵͳʱ�ӡ�
		*/
		void                   UseLocalTickSignal(bool bIsUseLocalTimer = true);

	private:
		friend class           xmAccessPoint;
		friend class           xmVdrAdmin;
		//���±��صĹ�������
		//���ݷ���ռ䣬���������һ����ֻ��Ҫ�����������һ�£�һ������Ҫ�������Ϳͻ���ͬʱ���¡�
		//��һ�������У��ȵ���BuildMemory(,false)��SyncDatas��Ȼ��BuildMemory(,true)
		virtual int            BuildMemory(const char* strDataSetName, bool bIsLocal = false);

	protected:
		xmLogHandler           m_LogHandler;
		xmIService*            m_pService;
		xmICommProxy*          m_pCommProxy;
		xmVDRAttr*             m_pVDRAttr;
	};
}

#endif	//	__SSA_IVDR_H