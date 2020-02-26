/**************************************************
�ļ���    :xmDBDataSet.h
����      :Ϊ�û��ṩͨ�����ݿ���ʽӿ�
����ļ�  :xmDBDataSet.cpp
����      :��ȫ��
-----------------------------------------------------
��ע      : ��ʼ�ӿ�����
-----------------------------------------------------
�޸ļ�¼ : 
�� ��        �汾   �޸���         �޸����� 
2011/02/18   1.0    <xxx>          ��������
*****************************************************/
#define _CRT_SECURE_NO_WARININGS
#pragma warning(disable:4996)
#ifndef _XM_MYSQL57_CDATA_SET_H
#define _XM_MYSQL57_CDATA_SET_H

#ifdef WIN32

#ifdef XMDBI_EXPORTS
#    define ASSIST_XMDBI_API __declspec(dllexport)
#else
#    define ASSIST_XMDBI_API __declspec(dllimport)
#endif

#else
//#define ASSIST_XMDBI_API
#ifdef XMDBI_EXPORTS
#    define ASSIST_XMDBI_API __declspec(dllexport)
#else
#    define ASSIST_XMDBI_API __declspec(dllimport)
#endif
#endif


//#define XMDB_DEBUG_PRINT_ON

//#pragma once
#ifdef WIN32
#pragma warning(disable:4996)
#endif

#include "DbiBase.h"
#include "QueryScheme.h"
#include "TypeIdentify.h"
#include "readXMLInterfaceHeader.h"
//#include "assert.h"
//#include <map>
/*****************************************************
������   : xmDBDataSet
����     : Ϊ�û��ṩͨ�����ݿ���ʽӿ�
------------------------------------------------------
��ע     : ����ʵ����Ҫ�������ļ��ж�ȡ��ز���
�����÷� : 
          xmDBDataSet* pMyDataSet = new xmDBDataSet();
          ��Ӽ�¼��
		  pMyDataSet->Init("��ӷ���1");
		  pMyDataSet->SetValue("����1",value1);
		  ......
		  pMyDataSet->SetValue("����n",valuen);
		  pMyDataSet->ExecuteQuery();

		  �޸ļ�¼��
		  pMyDataSet->Init("�޸ķ���1");
		  pMyDataSet->SetValue("����1",value1);
		  ......
		  pMyDataSet->SetValue("����n",valuen);
		  pMyDataSet->ExecuteQuery();
		  ��ע�������������޶�������Ҳ������Ҫ�޸ĵ�ֵ��
		  ������Ҫ�����ļ��޶�

		  ɾ����¼��
		  pMyDataSet->Init("ɾ������1");
		  pMyDataSet->SetValue("����1",value1);
		  ......
		  pMyDataSet->SetValue("����n",valuen);
		  pMyDataSet->ExecuteQuery();
		  ��ע���������޶���������������>30�ȣ��˴���Ҫ��
		  30��Ϊ��������

		  ��ѯ��¼��
		  pMyDataSet->Init("��ѯ����1");
		  pMyDataSet->SetValue("����1",value1);
		  ......
		  pMyDataSet->SetValue("����n",valuen);
		  pMyDataSet->ExecuteQuery();
		  while(pMyDataSet->fetch()==0)
		  {
			 pMyDataSet->GetValue("����1",valueN+1);
		     ......
		     pMyDataSet->GetValue("����n",valueN+m); 
		  }
		  ��ע������value1-valuen���޶�������valueN+1--
		  valueN+m��Ҫ��һ����¼�л�ȡ�Ĳ���ֵ��
		  ������Ҫ�����ļ��޶�

		  //��֧�ֵ���������
		  struct tm {
		  int tm_sec;     // seconds after the minute - [0,59] 
		  int tm_min;     // minutes after the hour - [0,59] 
		  int tm_hour;    // hours since midnight - [0,23] 
		  int tm_mday;    // day of the month - [1,31] 
		  int tm_mon;     // months since January - [0,11] 
		  int tm_year;    // years since 1900 
		  int tm_wday;    // days since Sunday - [0,6] 
		  int tm_yday;    // days since January 1 - [0,365] 
		  int tm_isdst;   // daylight savings time flag 
		  };
******************************************************/
namespace ssa {

	typedef std::map<string,CQueryScheme> SchemeMap;
	class ASSIST_XMDBI_API xmDBDataSet
	{
	public:
		
		xmDBDataSet(void);
		~xmDBDataSet(void);	 

		/******************************************************
		������   : xmDBDataSet
		����     : ���캯����ָ�������ļ�����·����
		����     : strCfgFilePath
		����ֵ   : ��
		��ע     : ���캯�����������޲�����Ĭ��ʹ�õ�ǰ·���µ�
		xmdbconfig.xml��Ϊ�����ļ�������в�����ʹ��ָ���ļ���Ϊ
		�����ļ���
		-------------------------------------------------------
		�����÷� :xmDBDataSet* pMyDataSet = new xmDBDataSet("E:\\skrvd\\code\\SSA\\samples\\useXmdbi\\xmdbconfig.xml");
		*******************************************************/
		xmDBDataSet(string strCfgFilePath);


		/******************************************************
		������   : Init
		����     : ������ݿ������ʼ����ָ����Ҫʹ�õ����÷���
		����     : schema
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : schema�������ļ������úõ����ݿ�ʹ�÷�������
		���ǲ��������Ҳ�����ǲ�ѯ�����»�ɾ��������
		ʹ�÷��������ƿ��������ļ�������ָ����
		-------------------------------------------------------
		�����÷� :Init("��ѯ1");
		Init("����2");
		*******************************************************/
		int Init(const char* schema);

		int GetResultCount();
		/******************************************************
		������   : SetValue
		����     : ΪҪʹ�õ����÷����趨����ֵ
		����     : strName���������ƣ�T value������ֵ
		����ֵ   : int
		��ע     : strName�������ļ������úõĲ������ƣ������ݿ�
		�б��ֶεĶ�Ӧ��ϵҲ�������ļ�ָ�����˴��Ĳ�
		�������û���������ָ���������ͱ�������ݿ���
		���ֶε�����һ�¡�
		���磺���ݿ��д洢�û���ʱ��Ӧ������Ϊ
		"username"������Ϊvarchar���ⲿʹ��ʱ������
		ʹ�á��û�����Ϊ�˲�����ֵ�����û�������
		��username���Ķ�Ӧ��ϵ�������ļ��趨�����û�
		�������ͱ������ַ����ͣ��������������͡�
		-------------------------------------------------------
		�����÷� :SetValue("�û���","����");	          
		*******************************************************/
		
		template<class T>
		int SetValue(const char* strName, T value)
		{
			string oldstr,newstr;
			oldstr = GetParamAlias(strName);
			if(oldstr == "")
			{
				char buf[150];
				sprintf(buf, "Can't find the param - [%s] in dbConfigFile: [%s].\n", strName,m_strDbCfgFile.c_str());
				m_strLastError = string(buf);
				printf("Can't find the param - [%s] in xmdbconfig.xml--SetValue.\n",strName);
				return -1;
			}
			/////////
			newstr = ""; 

			if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_UNKNOW)
			{
				m_strLastError = "Current Data Type is not supported.\n";
				printf("Current Data Type is not supported.\n");
				//////assert(0);
				//return -1;
				return -1;
			}
			newstr = XMDB_VALUE_TYPE<T>::GetStrFrom(value);
			//////////
			m_strCurSql = CDbiBase::StrReplace(m_strCurSql,oldstr,newstr);
			return 0;
		};

		
		/******************************************************
		������   : GetValue
		����     : �ӵ�ǰ��¼�л�ȡָ��������ֵ
		����     : strName���������ƣ�T value������ֵ
		����ֵ   : int
		��ע     : strName�������ļ������úõĲ������ƣ������ݿ�
		�б��ֶεĶ�Ӧ��ϵҲ�������ļ�ָ�����˴��Ĳ�
		�������û���������ָ���������ͱ�������ݿ���
		���ֶε�����һ�¡�
		���磺���ݿ��д洢�û���ʱ��Ӧ������Ϊ
		"username"������Ϊvarchar���ⲿʹ��ʱ������
		ʹ�á��û�����Ϊ�˲�����ֵ�����û�������
		��username���Ķ�Ӧ��ϵ�������ļ��趨�����û�
		�������ͱ������ַ����ͣ��������������͡�
		-------------------------------------------------------
		�����÷� : string name;
		GetValue("�û���",name);	          
		*******************************************************/
		template<class T>
		int GetValue(const char* strName, T& value)
		{	
			if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_UNKNOW)
			{				
				m_strLastError = "Current Data Type is not supported.\n";
				//printf("Current Data Type is not supported.\n");
				//////assert(0);
				//return -1;
				return -1;
			}

			if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_INT)
			{
				int n = GetInt(strName);
				memcpy(&value,&n,sizeof(int));
			}  
			else if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_UINT)
			{
				uint32_t n = GetUInt(strName);
				memcpy(&value,&n,sizeof(uint32_t));
			}
			else if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_FLOAT)
			{
				float x = GetFloat(strName);
				memcpy(&value,&x,sizeof(float));
			}
			else 
				if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_STRING)
			{
				string str0 = GetString(strName);
				XMDB_VALUE_TYPE<T>::Set(value,str0);
				//memcpy(&value,&str0[0],str0.length());
			}
			else if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_TM)
			{
				memcpy(&value,&GetDate(strName),sizeof(tm));				
			}
			else if(XMDB_VALUE_TYPE<T>::TYPE == XMDT_CHAR)
			{
				string str0 = GetString(strName);
				XMDB_VALUE_TYPE<T>::Set(value,str0);				
			}
			else
			{
				m_strLastError = "Current Data Type is not supported.\n";
				//printf("Current Data Type is not supported.\n");				
				return -1;
			}

			return 0;
		};

		/******************************************************
		������   : ExecuteQuery
		����     : ִ�����ݿ����������ָ�������÷���ִ��
		����     : 
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ��ָ�����÷���ִ�����ݿ�����������ǲ�ѯ��Ҳ
		��������ӡ����¡�ɾ����
		-------------------------------------------------------
		�����÷� :ExecuteQuery();
		*******************************************************/
		int ExecuteQuery();
		int ssa::xmDBDataSet::Test();
		/******************************************************
		������   : ExecuteOneSql
		����     : ִ��һ���û�ָ����sql���
		����     : 
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : �û���Ҫ�Լ���дsql��䣬����ִ�н������
		-------------------------------------------------------
		�����÷� :ExecuteOneSql("delete from test2 where id>2");
		*******************************************************/
		int ExecuteOneSql(string strSql);

		/******************************************************
		������   : GetRowBuffer
		����     : ��ȡ��ǰ������е�һ������
		����     : 
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : �û���Ҫ�Լ����������ݣ���fetch()�ɹ���ִ�С�
		-------------------------------------------------------
		�����÷� :char** myrow = GetRowBuffer();
		*******************************************************/
		char** GetRowBuffer();

		/******************************************************
		������   : Fetch
		����     : �����ݼ��л�ȡһ����¼
		����     : 
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ����0��ʾ��ȡ�ɹ����û�����ͨ��GetValue()��
		������Ŀ�л�ȡ���ݡ�
		-------------------------------------------------------
		�����÷� :if(Fetch()==0){...};
		*******************************************************/
		int Fetch();

		/******************************************************
		������   : GetInt
		����     : �����ݼ��л�ȡInt�Ͳ�����ֵ
		����     : name����������
		����ֵ   : int
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :if(Fetch()==0){int n = GetInt("����");};
		*******************************************************/
		int GetInt(string name);

		/******************************************************
		������   : GetUInt
		����     : �����ݼ��л�ȡuint32_t�Ͳ�����ֵ
		����     : name����������
		����ֵ   : uint32_t
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :if(Fetch()==0){uint32_t n = GetUInt("����");};
		*******************************************************/
		uint32_t GetUInt(string name);

		/******************************************************
		������   : GetFloat
		����     : �����ݼ��л�ȡFloat�Ͳ�����ֵ
		����     : name����������
		����ֵ   : Float
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :if(Fetch()==0){float weight = GetFloat("����");};
		*******************************************************/
		float GetFloat(string name);


		/******************************************************
		������   : GetString
		����     : �����ݼ��л�ȡstring�Ͳ�����ֵ
		����     : name����������
		����ֵ   : string
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :if(Fetch()==0){string str = GetString("��ַ");};
		*******************************************************/
		string& GetString(string name);

		/******************************************************
		������   : GetChar
		����     : �����ݼ��л�ȡChar�Ͳ�����ֵ
		����     : name����������
		����ֵ   : char
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :if(Fetch()==0){char c = GetChar("��־");};
		*******************************************************/
		char GetChar(string name);

		/******************************************************
		������   : GetDate
		����     : �����ݼ��л�ȡ�����Ͳ�����ֵ
		����     : name����������
		����ֵ   : tm
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :if(Fetch()==0){tm n = GetDate("��������");};
		*******************************************************/
		tm GetDate(string name);


		
		/******************************************************
		������   : SetInt
		����     : �������ݿ�ʱ������ɾ���ġ��飩ʱ����SQL���������Int�Ͳ�����ֵ
		����     : name����������
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :int n = SetInt("����",value);
		*******************************************************/
		int SetInt(string name,int value);

		/******************************************************
		������   : SetUInt
		����     : �������ݿ�ʱ������ɾ���ġ��飩ʱ����SQL���������Int�Ͳ�����ֵ
		����     : name����������
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :int n = SetInt("����",value);
		*******************************************************/
		int SetUInt(string name,uint32_t value);

		/******************************************************
		������   : SetFloat
		����     : �������ݿ�ʱ������ɾ���ġ��飩ʱ����SQL���������Float�Ͳ�����ֵ
		����     : name����������
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :int n = SetFloat("����",value);
		*******************************************************/
		int SetFloat(string name,float value);


		/******************************************************
		������   : SetString
		����     : �������ݿ�ʱ������ɾ���ġ��飩ʱ����SQL���������string�Ͳ�����ֵ
		����     : name����������
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :int n = SetString("��ַ",value);
		*******************************************************/
		int SetString(string name,string value);


		/******************************************************
		������   : SetChar
		����     : �������ݿ�ʱ������ɾ���ġ��飩ʱ����SQL���������char�Ͳ�����ֵ
		����     : name����������
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ���������������ļ���ָ��
		-------------------------------------------------------
		�����÷� :int n = SetChar("��־",value);
		*******************************************************/
		int SetChar(string name,char value);


		/******************************************************
		������   : SetDate
		����     : �������ݿ�ʱ������ɾ���ġ��飩ʱ����SQL���������ʱ�����ͣ�tm��������ֵ
		����     : name����������
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ���������������ļ���ָ��
		//��֧�ֵ���������
		  struct tm {
		  int tm_sec;     // seconds after the minute - [0,59] 
		  int tm_min;     // minutes after the hour - [0,59] 
		  int tm_hour;    // hours since midnight - [0,23] 
		  int tm_mday;    // day of the month - [1,31] 
		  int tm_mon;     // months since January - [0,11] 
		  int tm_year;    // years since 1900 
		  int tm_wday;    // days since Sunday - [0,6] 
		  int tm_yday;    // days since January 1 - [0,365] 
		  int tm_isdst;   // daylight savings time flag 
		  };
		-------------------------------------------------------
		�����÷� :int n = SetDate("��������",value);
		*******************************************************/
		int SetDate(string name,tm value);

		/******************************************************
		������   : GetMaxId
		����     : ��ȡ��ǰ���÷�����ָ���ı��е����ID
		����     : ��
		����ֵ   : int�ͣ����ݱ��е����ID	  -1��ʾ����
		��ע     : ������ݲ�����������Ի�ȡ��ǰ����ɹ�����Ŀ��ID
		-------------------------------------------------------
		�����÷� :int n = GetMaxId();
		*******************************************************/
		int GetMaxId();

		/******************************************************
		������   : GetCount
		����     : ��ȡ��ǰ���÷�����ָ���ı��е���Ŀ��
		����     : ��
		����ֵ   : int�ͣ����ݱ��еļ�¼����  -1��ʾ����
		��ע     : 
		-------------------------------------------------------
		�����÷� :int n = GetCount();
		*******************************************************/
		int GetCount();

		/******************************************************
		������   : GetLastError
		����     : ��ȡ���һ��������Ϣ
		����     : ��
		����ֵ   : ���һ��������Ϣ
		��ע     : 
		-------------------------------------------------------
		�����÷� :string str = GetLastError();
		*******************************************************/
		string GetLastError();

		/******************************************************
		������   : PrintMyScheme
		����     : ����������ݿ����÷�����Ϣ
		����     : ��
		����ֵ   : ��
		��ע     : �����������÷�����ȡ���
		-------------------------------------------------------
		�����÷� :
		*******************************************************/
		void PrintMyScheme();

		/******************************************************
		������   : GetCurSql
		����     : �����ǰ��Sql���
		����     : ��
		����ֵ   : ��
		��ע     : �������Ե�ǰ��Sql���׼�����
		-------------------------------------------------------
		�����÷� :
		*******************************************************/
		string GetCurSql();

		/******************************************************
		������   : SetBlobData
		����     : ����BLOB�������ݵ����ݿ�
		����     : ��
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ����������ר��
		-------------------------------------------------------
		�����÷� :
		*******************************************************/
		int SetBlobData(string strName,SDbiBuf dbufValue,int nId = 0);

		/******************************************************
		������   : GetBlobData
		����     : �����ݿ��ȡBLOB��������
		����     : ��
		����ֵ   : -1��ʾ���� 0 ��ʾ�ɹ�
		��ע     : ����������ר��
		-------------------------------------------------------
		�����÷� :
		*******************************************************/
		int GetBlobData(string strName,SDbiBuf& dbufValue,int nId);

		/******************************************************
		������   : GetBlobSql
		����     : ��ȡ���ʴ��������͵�SQL���
		����     : ��
		����ֵ   : ��
		��ע     : ����������ר�ã������ã������ڷ���֮ǰ�˽�Sql���׼�����
		-------------------------------------------------------
		�����÷� :
		*******************************************************/
		string GetBlobSql();

		/******************************************************
		������   : GetInsertId
		����     : ��������ǰ��INSERT��UPDATE���ΪAUTO_INCREMENT�����ɵ�ֵ��
		����     : ��
		����ֵ   : xmUInt64��,�ɹ����ش������������ʧ�ܷ���0
		��ע     : �ڰ���AUTO_INCREMENT�ֶεı���ִ����INSERT����Ӧʹ�øú�����
				   Mysql���ݿ�ר�á�
		-------------------------------------------------------
		�����÷� :   PDataSet->Init("�����Ϣ")  //ִ��Insert����
		             ......
					 if(PDataSet->ExecuteQuery() == 0)
					 {
					    int currentId =	PDataSet->GetInsertId();
					 }
		*******************************************************/
		uint64_t GetInsertId();

		/******************************************************
		������   : IsOpen
		����     : �жϵ�ǰ���ݿ������Ƿ����
		����     : ��
		����ֵ   : bool��
		��ע     : 
		-------------------------------------------------------
		�����÷� :
		*******************************************************/
		bool IsOpen();

	private:
		// ���ݿ����ӻ����ָ��
		CDbiBase* m_pDbi;
		//�û���
		string m_strUser;
		//����
		string m_strPassWd;
		//Ŀ�����ݿ�
		string m_strTargetDb;
		//Ŀ���������ַ��mysql��,ͨ��Ϊlocalhost��IP��ַ��
		string m_strTargetServer;
		// �������ݿ�����÷�������
		SchemeMap m_mapMyScheme;
		// ��ǰ�����ݿ����÷���
		string m_strCurScheme;
		// ��ǰҪִ�е�SQL���/ģ��
		string m_strCurSql;
		//���һ��������Ϣ
		string m_strLastError;
		//���������ļ�����
		string m_strDbCfgFile;
		//���ʴ��������͵�SQL���
		string m_strBlobSql;	 
		//��������ַ������ֵ��쳣��wx��أ�
		string m_strValue;
	private:
		// �������ļ��ж�ȡ�������ݿ�����÷���
		int ReadSchemeFromFile(string strCfgFile);

		// ��ȡָ�����ƵĲ��������ݼ��е������
		int GetColIndex(string name);

		/******************************************************
		������   : GetParamAlias
		����     : ��ȡ�����������������Sql����е�ֵ�滻
		����     : name
		����ֵ   : ��
		��ע     : ��ΪC++�в�֧��ͬһ�����캯��֮���໥���ã�ֻ
		�ܽ��������ݳ��������Ϊһ��������
		*******************************************************/
		// 
		string GetParamAlias(string name);

		/******************************************************
		������   : InitData
		����     : ������ݳ�ʼ����ͬ���캯��
		����     : strCfgFilePath
		����ֵ   : ��
		��ע     : ��ΪC++�в�֧��ͬһ�����캯��֮���໥���ã�ֻ
				   �ܽ��������ݳ��������Ϊһ��������
		*******************************************************/
		void InitData(string strCfgFilePath);

		/******************************************************
		������   : GetTableNameFromSql
		����     : ��SQL����л�ȡ���ݱ������
		����     : strSql
		����ֵ   : ���ݱ������
		��ע     : 
		*******************************************************/
		string GetTableNameFromSql(string strSql);

		/******************************************************
		������   : VerifiedDbCfgFile
		����     : ��֤�����ļ��Ƿ����
		����     : strFilePath
		����ֵ   : true�������ļ����ڣ�false�������ļ�������
		��ע     : 
		*******************************************************/
		bool VerifiedDbCfgFile(string strFilePath);

		
};

}

#endif //_XM_MYSQL51_CDATA_SET_H