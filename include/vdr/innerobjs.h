#ifndef __SSA_VDR_INNEROBJS_H
#define __SSA_VDR_INNEROBJS_H

#include "export.h"
#include "data.h"
#include "dataset.h"
#include "node.h"
#include "objectregistry.h"
#include "memorypage.h"
#include <shared_mutex>

namespace ssa
{
	struct xmMemoryIndex
	{
	public:
		unsigned int m_ValuePos = 0;
		unsigned int m_RefPos = 0;
	};

	class xmNodeWithDetail
	{
	public:
		xmNode m_Node;
	};

	class xmDataSetWithDetail;
	class xmDataWithDetail
	{
	public:
		xmDataWithDetail();
		~xmDataWithDetail();

		void InitMainMemoryWithDescribe();
		void InitMirrorMemoryWithDescribe();
		void Destroy();

	public:
		xmData                   m_Data;
		//�ж��Ƿ������и���
		unsigned char            m_isUpdate;
		//�������ݼ��ϵ�����
	    //char                     m_strParentDataSet[xmENameLenth::xmENL_DATASET];
		std::string              m_strParentDataSet;
		// �������ݼ��ϵ�ָ��
		xmDataSetWithDetail*     m_pDataSet = 0;
		//�������ݼ���������������ȡ��λ�ã������ֽ�����
		unsigned int             m_uIndexOffset = 0;
		//Ŀǰֻ��Ԥ����ԭ�������ڸ���״̬ʱ����
		int                      m_nCounter = 0;
		//��Ӧ���ڴ�ռ��ָ�룬Ϊ�˿��ټ���
		xmMemoryIndex            m_MemoryIndex;
		xmMemoryPage*            m_pMainMemoryPage = 0;
		//�����ڴ棬������ʱ�洢��������Ҫ����������ֵ
		xmMemoryPage*            m_pMirrorMemoryPage = 0;
		//xmMemoryIndex  m_MirrorMemoryTableIndex;
		//�����ڴ�ĳ�ʼ��ַ
		//����Value���͵�ָ��
		void*                    m_lpValueMainAddr = 0;
		//����Value���͵�ָ��
		void*                    m_lpValueMirrorAddr = 0;
	};

	typedef enum __tagDataSetState
	{
		xmEINS_NOTHING = 0,
		xmEINS_JOINING,
		xmEINS_JOINED,
		xmEINS_DATA_SYNC,		    //	
		xmEINS_INDEX_BUILDING,		//	
		xmEINS_INDEX_BUILDED,		//	
		xmEINS_MEMORY_BUILDING,     //	
		xmEINS_MEMORY_BUILDED,      //	
		xmEINS_VALUE,	            //
		xmEINS_MAX,
	} xmEDataSetState;

	class xmDataSetWithDetail
	{
		typedef xmObjectRegistry<xmDataWithDetail, std::string>      DataWithDetailRegistry;
		typedef xmObjectRegistry<xmDataWithDetail, unsigned int>     DataWithDetail2IntRegistry;
		//���������ݰ�����������
		DataWithDetail2IntRegistry           m_mapDatasIndex;
		//��Ҫ�������ڸ�λ����ֵ
		DataWithDetailRegistry               m_mapNeedResetDatas;
		//���������е�����
		DataWithDetailRegistry               m_mapDatasInSet;

	public:
		xmDataSet                            m_DataSet;
		//���ݼ��ϵķ����ڴ��С
		unsigned int                         m_uMemLength = 0;
		//�Ƿ��Ѿ�������ڴ���Կ�ʼʹ��
		bool                                 m_bIsMainMemoryReady;
		//�Ƿ��Ѿ������Mirror�ڴ���Կ�ʼʹ��
		bool                                 m_bIsMirrorMemoryReady;
		//���ڱ�֤DataSet��ʼ�����̵�״̬�����ʹ��״̬����
		xmEDataSetState                      m_State;
		//�Ƿ�����д���ֵ
		bool                                 m_bHaveNewValue = false;
		//��¼һ�������˶��ٴ�
		long long                            m_lRefreshCount = 0;
		//���ڼ�¼����״̬,0��ʾ��������״̬
		long long                            m_nUnActivePeriod = 0;

	private:		
		std::shared_mutex                   m_Mutex;
		//�ڴ�������ṹ
		xmMemoryIndex                        m_MemoryIndex;
		//���ڴ�ṹ
		xmMemoryPage*                        m_pMainMemoryPage = 0;
		//�����ڴ棬������ʱ�洢��������Ҫ����������ֵ
		xmMemoryPage*                        m_pMirrorMemoryPage = 0;

	public:
		xmDataSetWithDetail();
		~xmDataSetWithDetail();
		//��ȡ��ǰ��״ֵ̬
		xmEDataSetState       GetState() { return m_State; };

		//�������ݼ������ֽ����ڴ���
		int                   BuildMemory(bool isLocal, bool bWithMirror, bool isShare, int nVersion );
		unsigned int          GetMemorySize() const;
		bool                  IsMemoryValid() const;
		void                  ClearMemoryValidSign();

		void                  AddData(const xmDataWithDetail*);
		void                  RemoveData(const std::string&);
		unsigned int          GetDataNames(std::vector<std::string>& vName);
		unsigned int          GetDataIDs(std::vector<unsigned int>& vData);
		unsigned int          GetDatas(std::vector<xmDataWithDetail*>& vData);

		int                   GetValue(unsigned int pos, char* out, unsigned int uSize);
		int                   GetMirrorValue(unsigned int pos, char* out, unsigned int uSize);
		int                   SetValue(unsigned int pos, char* in, unsigned int uSize);
		int                   SetMirrorValue(unsigned int pos, char* in, unsigned int uSize);

		//���ü����������Զ����㾭���˼������ڡ�
		void                  ReferenceCount(bool bMirrorMemory = false);
		//��λ��ֵ�����ڼ������ں��Զ���λ���ݡ�
		void                  DataReset2InitState(bool bMirrorMemory,bool isNoDelay);
		//�������д��־
		void                  ClearSign(bool bMirrorMemory = false);
		void                  SynSign(bool bMainMemory2MirrorMemory);
		void                  SyncMainMemory2Mirror();

		void                  Destroy();
		void                  Clear();

	private:
		//�������˺Ϳͻ��˰������ݼ��Ͻ�������
		int                   BuildIndex(bool isLocal);
		int                   BuildMainMemory(bool isShare);
		int                   BuildMirrorMemory(bool isShare);
	};

	//���ڹ���ȫ�ֵĶ���
	class xmObjsMgr
	{
	public:
		typedef xmObjectRegistry<xmDataSetWithDetail>                    DataSetRegistry;
		typedef xmObjectRegistry<xmDataWithDetail>                       DataRegistry;
		typedef xmObjectRegistry<xmNodeWithDetail,std::string>           NodeRegistry;
		typedef xmObjectRegistry<xmDataSetWithDetail, unsigned int>      DataSetIntRegistry;
		typedef xmObjectRegistry<xmDataWithDetail, unsigned int>         DataIntRegistry;

		NodeRegistry       m_NodeRegistry;
		DataSetRegistry    m_DataSetRegistry;
		DataRegistry       m_DataRegistry;
		//����ID��������ݼ���
		DataSetIntRegistry m_DataSetByIDRegistry;
		//����ID���е�����
		DataIntRegistry    m_DataByIDRegistry;
	};
}
#endif //__SSA_INNEROBJS_H