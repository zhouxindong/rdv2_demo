#pragma once

#ifndef __SSA_VDR_DEFS_H
#define __SSA_VDR_DEFS_H

namespace ssa
{
	/**
	*  �ڵ�Ȩ�����ͣ�AP������Ȩ�����͡�
	*/
	typedef enum __tagAPAuthority
	{
		xmEAPA_NOTHING = 0,     /**< δ���塣 */
		xmEAPA__OBSERVER,		/**< �۲졣 */	
		xmEAPA__WORKER,		    /**< ������ */	
		xmEAPA__CONTROLLER,	    /**< ���ơ� */	
		xmEAPA__GOD,			/**< ������ */	
		xmEAPA__MAX,
	} xmEAPAuthority;

	/**
	*  �����ڴ�ͬ���ķ���
	*/
	typedef enum __tagSyncDirection
	{
		xmESD_UNKOWN = 0,	        /**< ��ʾ�����巽�� */	
		xmESD_TOCLIENT,		        /**< ��ĳһ���ͻ��˵��� */
		xmESD_TOCLIENTS,		    /**< ��ͻ����ǵ��� */
		xmESD_OUTDIRBEGIN,	        /**< ��ʾ���ⷽ��ģʽ��ʼ�� */	
		xmESD_TOSERVER,		        /**< ����������� */	
		xmESD_DOUBLEDIRECTION,		/**< ��˫�� */
		xmEAS_BROADCAST,		    /**< ֱ�������н����߹㲥�� */
	}xmESyncDirection;

	/**
	*  �Ự���͡�
	*/
	typedef enum __tagSessionType
	{
		xmEST_LOGIN = 1,		    /**< ע�ἰϵͳ��ʼ�����̡� */	
		xmEST_TICK,		            /**< ʱ���źš� */

		xmEST_MULTISESSION_BEGAIN = 100,
		xmEST_VALSYNC,		        /**< ��ֵͬ���� */
		xmEST_BULLETIN,             /**< �������Ϣ�� */
		xmEST_PUBSUB,		        /**< �������ġ� */

		xmEST_ADMIN_BEGAIN = 200,
		xmEST_LOG,                  /**< ��־�� */
		xmEST_MAX,
	} xmESessionType;

	/**
	* VDRͨ��ģʽ��
	*/
	typedef enum __tagVDRCommProxyType
	{
		xmECM_NOTHING = 0, /**< δ���塣 */
		xmECM_LAN,		   /**< �������� */
		xmECM_LOCAL,	   /**< ���ء� */
		xmECM_RT,		   /**< ʵʱ������ģʽ�£��� */
		xmECM_WAN,		   /**< �������� */
		xmECM_USR_BEGIN,   /**< �û��Զ����ͨ��ģʽ��ʼID�� */
	} xmEVDRCommProxyType;

	/**
	* VDR�˽�ɫ���塣
	*/
	typedef enum __tagEndpointType
	{
		xmEET_NOTHING = 0,         /**< δ���塣 */
		xmEET_ANY,                 /**< �������;��ɣ���Ҫ�����ڲ�ͨ��ʱʹ�ã��ڶ���ʱ��ͨ�����Ƿ��������ǿͻ��ˡ� */
		xmEET_SERVER,		       /**< �������ˣ�����ϵͳ����������Ϣ��ֵ�Ĵ洢�Ͳ��ԵĹ��� */
		xmEET_CLIENTMIN = 10,      /**< �ͻ��˺�ϵͳ�������ķֽ磬����ϵͳ�ڲ��ж�ʹ�á� */
		xmEET_CLIENT,	           /**< �ͻ��ˣ��������ڵ㡣 */
		xmEET_RTE,		           /**< RTE�ˣ�Ԥ��Ϊһ̨����ʹ��һ������ʱ������ */
		xmEET_RADIO,		       /**< ��������ͻ��˹㲥���ݡ� */
	} xmEEndpointType;

	/**
	* VDRϵͳ����״̬��
	*/
	typedef enum __tagSystemState
	{
		xmESS_NOTHING = 0,  /**< δ���塣 */
		xmESS_NORMAL,	    /**< ����״̬�� */
		xmESS_REPLAY,	    /**< �ط�״̬�� */
		xmESS_MAX,
	} xmESystemState;

	/**
	* VDRϵͳ�н���ʱ��ͬ����
	*/	
	class xmTimeStamp
	{
	public:
		long long        m_lTimeStamp; /**< ʱ����� */
		long long        m_lFrameCount;/**< ֡������ */

		xmTimeStamp()
		{
			 m_lTimeStamp = 0;
			 m_lFrameCount = 0;
		}
	};

	/**
	* VDRϵͳ��ͬ������Ҫ��
	*/
	typedef enum __tagQos
	{
		xmEQoS_DEFAULT = 0,       /**< ʹ��Ĭ�ϴ��䷽ʽ�� */
		xmEQoS_UNRELIABLE,        /**< ʹ�þ�����Ĵ��䷽ʽ������֤��ȫ�ɿ��� */
		xmEQoS_RELIABLE,          /**< ʹ�ÿɿ����䷽ʽ�� */
		xmEQoS_MAX = 64,          /**< ����Qos���͡� */
	}xmEQos;

#define xmMAX_PKG_LENGTH        64*1024     /**< �������ݰ�����,Ϊ����Ӧ����UDP�ĳ���Ҫ�� */
#define xmMAX_SINGLE_DATA_VALUE_LENGTH 63*1024     /**< ÿ�����ݶ�Ӧֵ���ĳ��ȣ���������ˣ���Ҫ�û����������зֽ⴦�� */

#define xmMAX_DATASET_CABILITY  unsigned int(((unsigned int)0xFFFFFFFF/2) - 1 )     /**< ������ɵ����ݼ������� */

#define xmMAX_WAITING_TIME      600000     /**< ��ѯʱ�����ȴ�ʱ�䣬��λms�� */

#define xmVERSION               000900      /**< 00��汾�� 09�а汾�� 00С�汾�š� */

}

#endif
