/**************************************************
文件名    :DbiBase.cpp
功能      :为实现通用数据库访问接口提供底层实现
相关文件  :DbiBase.h
作者      :侯全超
-----------------------------------------------------
备注      : 
-----------------------------------------------------
修改记录 : 
日 期        版本   修改人         修改内容 
2009/08/08   1.0    <侯全超>       创建初稿
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
// 定义返回的错误码
/************************************************************/
//#define ERR_NO_ERROR			0			// 无错误
//#define ERR_UNDEFINED			-100			// 没有定义的异常或错误
//#define ERR_SQL_EXCEPTION		-101			// 发生SQL异常
//#define ERR_DB_NOT_OPEN			-102			// 尚未建立数据库连接
//
//
//#define ERR_STMT_ISNULL     	-103			// Statement is null
//#define ERR_STATUS_UNPREPARED	-104			// Statement::status is not RESULT_SET_AVAILABLE
//#define ERR_RESULT_SET_ISNULL	-105			// ResultSet is null
//
//#define ERR_OPER_NOT_SUPPORTED	-110		// 不支持的数据操作
//


namespace ssa {

	/************************************************************/
	// 数据库访问接口中通用的缓冲区
	typedef struct dbi_buf {
		unsigned char* pBuf;	// 数据缓冲区
		unsigned int bufSize;		// 缓冲区大小

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

		//最后一条出错信息
		string m_strLastError;
	//public:
		// 查询获取的数据集
		//ResultSet* m_pRs;

	public:
		// 构造函数，将根据用户指定的连接参数建立到系统数据库的连接
		CDbiBase(string server, string user, string passwd, string db);

		~CDbiBase();

	public:		

		// 判断是否已经成功连接到了数据库
		bool isOpen(void) 
		{
			return (m_pConn != NULL);
		}

		// 字符串替换
		// 说明：将字符串strIn内的所有子字符串strOld替换为strNew，当前仅
		//      实现为将字符‘'’替换为“''”
		static string string_dbEncode(string strIn)
		{
			string strDest;
			string::iterator pos;//string::iterator pos;
			for(int i=0; i<strIn.size(); i++)
			{
				if(strIn[i] == '\'')
					strDest += "''";
				else if(strIn[i] == '\\')  //这是为mysql加的，20110307
					strDest += "\\\\";
				else
					strDest += strIn[i];
			}

			return strDest;
		}
			  		 

		//替换字符串
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

		//将字符串转换为大写
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


		//从给定类型获取字符串，用于填充SQL语句
		static string GetStrFromInt(int n)
		{		   			
			char buf[50];
			sprintf(buf, "%d", n);
			return string(buf);
		}

		//从给定类型获取字符串，用于填充SQL语句
		static string GetStrFromUInt(uint32_t n)
		{		   			
			char buf[50];
			sprintf(buf, "%u", n);
			return string(buf);
		}

		//从给定类型获取字符串，用于填充SQL语句
		static string GetStrFromFloat(float n)
		{		   			
			char buf[50];
			sprintf(buf, "%f", n);
			return string(buf);
		}

		//从给定类型获取字符串，用于填充SQL语句
		static string GetStrFromDouble(double n)
		{		   			
			char buf[50];
			sprintf(buf, "%f", n);
			return string(buf);
		}

		//从给定类型获取字符串，用于填充SQL语句
		static string GetStrFromString(string n)
		{		   					
			string temp = CDbiBase::string_dbEncode(n);
			
			return "'"+temp+"'";
		}

		//从给定类型获取字符串，用于填充SQL语句
		static string GetStrFromChar(char c)
		{				
			string temp;
			temp = c;
			return "'"+temp+"'";
		}

		//从给定类型获取字符串，用于填充SQL语句: 专门用于高级查询时的扩展应用
		static string GetStrFromStringExtend(string n)
		{		   					
			return CDbiBase::string_dbEncode(n);			 
		}

		//从给定类型获取字符串，用于填充SQL语句
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

		// 执行给定的SQL语句
		int Execute(string strSql);
		

		// 获取下一条记录
		int fetch(void);
		// 从数据集中获取指定列的Int型数据
		int GetInt(int col);			   
		// 从数据集中获取指定列的UInt32型数据
		uint32_t GetUInt(int col);
		// 从数据集中获取指定列的Float型数据
		float GetFloat(int col);
		// 从数据集中获取指定列的string型数据
		string GetString(int col);
		// 从数据集中获取指定列的日期(tm)型数据
		tm GetDate(int col);
		// 初始化数据集中列序map表
		int InitColumnMap(map<string,int>& colmap);
		// 执行一条从数据库表获取一个数值字段的SQL查询操作
		int getOneIntValueFromTable(string sqlStmt, int& nRtn);
		//返回由以前的INSERT或UPDATE语句为AUTO_INCREMENT列生成的值。
		uint64_t GetNearInsertId();
		//获取当前的一行数据
		MYSQL_ROW GetCurrentRow();

		/******************************************************
		函数名   : GetLastError
		功能     : 获取最后一条出错信息
		参数     : 无
		返回值   : 最后一条出错信息
		备注     : 
		-------------------------------------------------------
		典型用法 :string str = GetLastError();
		*******************************************************/
		string GetLastError();

		/******************************************************
		函数名   : UpdateBlobData
		功能     : 更新大类型数据字段
		参数     : 无
		返回值   : 0 成功，-1 失败
		备注     : 
		-------------------------------------------------------
		典型用法 :无
		*******************************************************/
		int UpdateBlobData(string strBlobSql,SDbiBuf dbufValue);

		/******************************************************
		函数名   : GetBlobData
		功能     : 获取大类型数据字段
		参数     : 无
		返回值   : 0 成功，-1 失败
		备注     : 
		-------------------------------------------------------
		典型用法 :无
		*******************************************************/
		int GetBlobData(string strBlobSql,SDbiBuf& dbufValue);
		int GetFieldCount();

};// End of class CDbiBase
}// end of ssa namespace

#endif //  _XM_DBI_BASE_H