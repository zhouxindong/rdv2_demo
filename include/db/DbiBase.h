/**************************************************
�ļ���    :DbiBase.cpp
����      :Ϊʵ��ͨ�����ݿ���ʽӿ��ṩ�ײ�ʵ��
����ļ�  :DbiBase.h
����      :��ȫ��
-----------------------------------------------------
��ע      : 
-----------------------------------------------------
�޸ļ�¼ : 
�� ��        �汾   �޸���         �޸����� 
2009/08/08   1.0    <��ȫ��>       ��������
*****************************************************/

#define _CRT_SECURE_NO_WARININGS
#ifndef _XM_DBI_BASE_H
#define _XM_DBI_BASE_H
#pragma warning(disable:4996)
//#pragma once
#ifdef WIN32
#pragma warning(disable:4996)
#endif

#include "..\base\base.h"
#ifdef WIN32
#include "winsock2.h"
#//include "mysql 5.1\include\mysql.h"
#include "..\..\extlibs\mysql/include/mysql.h"//mysql/include/mysql.h
//#include "xmConfig\ConfigTarget.h"
//#include "xmConfig\ConfigTargetManager.h"
//#include "xmBase\FuncDefines.h"
#else
#include "mysql.h"
//#include "xmConfig/ConfigTarget.h"
//#include "xmConfig/ConfigTargetManager.h"
//#include "xmBase/FuncDefines.h"
#endif
//

using namespace std;
using namespace ssa;

	
/************************************************************/
// ���巵�صĴ�����
/************************************************************/
//#define ERR_NO_ERROR			0			// �޴���
//#define ERR_UNDEFINED			-100			// û�ж�����쳣�����
//#define ERR_SQL_EXCEPTION		-101			// ����SQL�쳣
//#define ERR_DB_NOT_OPEN			-102			// ��δ�������ݿ�����
//
//
//#define ERR_STMT_ISNULL     	-103			// Statement is null
//#define ERR_STATUS_UNPREPARED	-104			// Statement::status is not RESULT_SET_AVAILABLE
//#define ERR_RESULT_SET_ISNULL	-105			// ResultSet is null
//
//#define ERR_OPER_NOT_SUPPORTED	-110		// ��֧�ֵ����ݲ���
//


namespace ssa {

	/************************************************************/
	// ���ݿ���ʽӿ���ͨ�õĻ�����
	typedef struct dbi_buf {
		unsigned char* pBuf;	// ���ݻ�����
		unsigned int bufSize;		// ��������С

		dbi_buf() {
			pBuf = NULL;
			bufSize = 0;
		}

		void releaseBuffer()
		{
			if(bufSize > 0 && pBuf != NULL)	{
				delete[] pBuf;
				pBuf = NULL;
			}
			bufSize = 0;
		}
	}SDbiBuf;

	/************************************************************/

	class CDbiBase
	{
	private:
		MYSQL* m_pConn;
		MYSQL_ROW m_Row;	
		MYSQL_RES* m_pRes_set;

		//���һ��������Ϣ
		string m_strLastError;
	//public:
		// ��ѯ��ȡ�����ݼ�
		//ResultSet* m_pRs;

	public:
		// ���캯�����������û�ָ�������Ӳ���������ϵͳ���ݿ������
		CDbiBase(string server, string user, string passwd, string db);

		~CDbiBase();

	public:		

		// �ж��Ƿ��Ѿ��ɹ����ӵ������ݿ�
		bool isOpen(void) 
		{
			return (m_pConn != NULL);
		}

		// �ַ����滻
		// ˵�������ַ���strIn�ڵ��������ַ���strOld�滻ΪstrNew����ǰ��
		//      ʵ��Ϊ���ַ���'���滻Ϊ��''��
		static string string_dbEncode(string strIn)
		{
			string strDest;
			string::iterator pos;//string::iterator pos;
			for(int i=0; i<strIn.size(); i++)
			{
				if(strIn[i] == '\'')
					strDest += "''";
				else if(strIn[i] == '\\')  //����Ϊmysql�ӵģ�20110307
					strDest += "\\\\";
				else
					strDest += strIn[i];
			}

			return strDest;
		}
			  		 

		//�滻�ַ���
		static string StrReplace(string src,string old,string newstr)
		{		   			
			basic_string <char>::size_type index;
			
			static const basic_string <char>::size_type npos = -1;

			index = src.find(old);
			while(index != npos)
			{
				src.replace(index,old.length(),newstr);
				index = src.find(old);
			}
			return src;
		}

		//���ַ���ת��Ϊ��д
		static string StrToUp(string strSrc)
		{		   			
			int uiResultLen = strSrc.size();
			char* boolString = new char[uiResultLen + 1];
			strcpy(boolString,  strSrc.c_str());

			std::locale locTmp;
			std::locale loc(locTmp, std::locale::classic( ),
				std::locale::all);
			std::use_facet<std::ctype<char> > ( loc ).toupper
				( boolString, boolString + uiResultLen );
			string strDes = boolString;

			delete []boolString;

			return strDes;
		}


		//�Ӹ������ͻ�ȡ�ַ������������SQL���
		static string GetStrFromInt(int n)
		{		   			
			char buf[50];
			sprintf(buf, "%d", n);
			return string(buf);
		}

		//�Ӹ������ͻ�ȡ�ַ������������SQL���
		static string GetStrFromUInt(uint32_t n)
		{		   			
			char buf[50];
			sprintf(buf, "%u", n);
			return string(buf);
		}

		//�Ӹ������ͻ�ȡ�ַ������������SQL���
		static string GetStrFromFloat(float n)
		{		   			
			char buf[50];
			sprintf(buf, "%f", n);
			return string(buf);
		}

		//�Ӹ������ͻ�ȡ�ַ������������SQL���
		static string GetStrFromDouble(double n)
		{		   			
			char buf[50];
			sprintf(buf, "%f", n);
			return string(buf);
		}

		//�Ӹ������ͻ�ȡ�ַ������������SQL���
		static string GetStrFromString(string n)
		{		   					
			string temp = CDbiBase::string_dbEncode(n);
			
			return "'"+temp+"'";
		}

		//�Ӹ������ͻ�ȡ�ַ������������SQL���
		static string GetStrFromChar(char c)
		{				
			string temp;
			temp = c;
			return "'"+temp+"'";
		}

		//�Ӹ������ͻ�ȡ�ַ������������SQL���: ר�����ڸ߼���ѯʱ����չӦ��
		static string GetStrFromStringExtend(string n)
		{		   					
			return CDbiBase::string_dbEncode(n);			 
		}

		//�Ӹ������ͻ�ȡ�ַ������������SQL���
		//struct tm {
		//      int tm_sec;     /* seconds after the minute - [0,59] */
		//      int tm_min;     /* minutes after the hour - [0,59] */
		//      int tm_hour;    /* hours since midnight - [0,23] */
		//      int tm_mday;    /* day of the month - [1,31] */
		//      int tm_mon;     /* months since January - [0,11] */
		//      int tm_year;    /* years since 1900 */
		//      int tm_wday;    /* days since Sunday - [0,6] */
		//      int tm_yday;    /* days since January 1 - [0,365] */
		//      int tm_isdst;   /* daylight savings time flag */
		//      };
		static string GetStrFromDate(tm n)
		{		   			
			char buf[100];
			sprintf(buf, "'%d-%d-%d %d:%d:%d'", n.tm_year+1900,n.tm_mon+1,n.tm_mday,n.tm_hour,n.tm_min,n.tm_sec);
			return string(buf);
		}

		// ִ�и�����SQL���
		int Execute(string strSql);
		

		// ��ȡ��һ����¼
		int fetch(void);
		// �����ݼ��л�ȡָ���е�Int������
		int GetInt(int col);			   
		// �����ݼ��л�ȡָ���е�UInt32������
		uint32_t GetUInt(int col);
		// �����ݼ��л�ȡָ���е�Float������
		float GetFloat(int col);
		// �����ݼ��л�ȡָ���е�string������
		string GetString(int col);
		// �����ݼ��л�ȡָ���е�����(tm)������
		tm GetDate(int col);
		// ��ʼ�����ݼ�������map��
		int InitColumnMap(map<string,int>& colmap);
		// ִ��һ�������ݿ���ȡһ����ֵ�ֶε�SQL��ѯ����
		int getOneIntValueFromTable(string sqlStmt, int& nRtn);
		//��������ǰ��INSERT��UPDATE���ΪAUTO_INCREMENT�����ɵ�ֵ��
		uint64_t GetNearInsertId();
		//��ȡ��ǰ��һ������
		MYSQL_ROW GetCurrentRow();

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
		������   : UpdateBlobData
		����     : ���´����������ֶ�
		����     : ��
		����ֵ   : 0 �ɹ���-1 ʧ��
		��ע     : 
		-------------------------------------------------------
		�����÷� :��
		*******************************************************/
		int UpdateBlobData(string strBlobSql,SDbiBuf dbufValue);

		/******************************************************
		������   : GetBlobData
		����     : ��ȡ�����������ֶ�
		����     : ��
		����ֵ   : 0 �ɹ���-1 ʧ��
		��ע     : 
		-------------------------------------------------------
		�����÷� :��
		*******************************************************/
		int GetBlobData(string strBlobSql,SDbiBuf& dbufValue);
		int GetFieldCount();

};// End of class CDbiBase
}// end of ssa namespace

#endif //  _XM_DBI_BASE_H