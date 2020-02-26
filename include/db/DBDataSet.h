/**************************************************
文件名    :xmDBDataSet.h
功能      :为用户提供通用数据库访问接口
相关文件  :xmDBDataSet.cpp
作者      :侯全超
-----------------------------------------------------
备注      : 初始接口描述
-----------------------------------------------------
修改记录 : 
日 期        版本   修改人         修改内容 
2011/02/18   1.0    <xxx>          创建初稿
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
类名称   : xmDBDataSet
功能     : 为用户提供通用数据库访问接口
------------------------------------------------------
备注     : 具体实现需要从配置文件中读取相关参数
典型用法 : 
          xmDBDataSet* pMyDataSet = new xmDBDataSet();
          添加记录：
		  pMyDataSet->Init("添加方案1");
		  pMyDataSet->SetValue("参数1",value1);
		  ......
		  pMyDataSet->SetValue("参数n",valuen);
		  pMyDataSet->ExecuteQuery();

		  修改记录：
		  pMyDataSet->Init("修改方案1");
		  pMyDataSet->SetValue("参数1",value1);
		  ......
		  pMyDataSet->SetValue("参数n",valuen);
		  pMyDataSet->ExecuteQuery();
		  备注：参数可能是限定条件，也可能是要修改的值，
		  具体需要配置文件限定

		  删除记录：
		  pMyDataSet->Init("删除方案1");
		  pMyDataSet->SetValue("参数1",value1);
		  ......
		  pMyDataSet->SetValue("参数n",valuen);
		  pMyDataSet->ExecuteQuery();
		  备注：参数是限定条件，例如年龄>30等，此处需要将
		  30作为参数传入

		  查询记录：
		  pMyDataSet->Init("查询方案1");
		  pMyDataSet->SetValue("参数1",value1);
		  ......
		  pMyDataSet->SetValue("参数n",valuen);
		  pMyDataSet->ExecuteQuery();
		  while(pMyDataSet->fetch()==0)
		  {
			 pMyDataSet->GetValue("参数1",valueN+1);
		     ......
		     pMyDataSet->GetValue("参数n",valueN+m); 
		  }
		  备注：参数value1-valuen是限定条件，valueN+1--
		  valueN+m是要从一条记录中获取的参数值。
		  具体需要配置文件限定

		  //所支持的日期类型
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
		函数名   : xmDBDataSet
		功能     : 构造函数：指定配置文件（带路径）
		参数     : strCfgFilePath
		返回值   : 无
		备注     : 构造函数共两个：无参数的默认使用当前路径下的
		xmdbconfig.xml作为配置文件；这个有参数的使用指定文件作为
		配置文件。
		-------------------------------------------------------
		典型用法 :xmDBDataSet* pMyDataSet = new xmDBDataSet("E:\\skrvd\\code\\SSA\\samples\\useXmdbi\\xmdbconfig.xml");
		*******************************************************/
		xmDBDataSet(string strCfgFilePath);


		/******************************************************
		函数名   : Init
		功能     : 完成数据库操作初始化：指定将要使用的配置方案
		参数     : schema
		返回值   : -1表示出错 0 表示成功
		备注     : schema是配置文件中设置好的数据库使用方案，可
		以是插入操作，也可以是查询、更新或删除操作，
		使用方案的名称可在配置文件中任意指定。
		-------------------------------------------------------
		典型用法 :Init("查询1");
		Init("更新2");
		*******************************************************/
		int Init(const char* schema);

		int GetResultCount();
		/******************************************************
		函数名   : SetValue
		功能     : 为要使用的配置方案设定参数值
		参数     : strName，参数名称；T value：参数值
		返回值   : int
		备注     : strName是配置文件中设置好的参数名称，与数据库
		中表字段的对应关系也有配置文件指定，此处的参
		数名称用户可以随意指定。但类型必须和数据库中
		表字段的类型一致。
		例如：数据库中存储用户名时对应的列名为
		"username"，类型为varchar；外部使用时，可以
		使用“用户名”为此参数赋值，“用户名”和
		“username”的对应关系由配置文件设定，但用户
		名的类型必须是字符串型，不能是其他类型。
		-------------------------------------------------------
		典型用法 :SetValue("用户名","李明");	          
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
		函数名   : GetValue
		功能     : 从当前记录中获取指定参数的值
		参数     : strName，参数名称；T value：参数值
		返回值   : int
		备注     : strName是配置文件中设置好的参数名称，与数据库
		中表字段的对应关系也有配置文件指定，此处的参
		数名称用户可以随意指定。但类型必须和数据库中
		表字段的类型一致。
		例如：数据库中存储用户名时对应的列名为
		"username"，类型为varchar；外部使用时，可以
		使用“用户名”为此参数赋值，“用户名”和
		“username”的对应关系由配置文件设定，但用户
		名的类型必须是字符串型，不能是其他类型。
		-------------------------------------------------------
		典型用法 : string name;
		GetValue("用户名",name);	          
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
		函数名   : ExecuteQuery
		功能     : 执行数据库操作：按照指定的配置方案执行
		参数     : 
		返回值   : -1表示出错 0 表示成功
		备注     : 按指定配置方案执行数据库操作，可以是查询，也
		可以是添加、更新、删除。
		-------------------------------------------------------
		典型用法 :ExecuteQuery();
		*******************************************************/
		int ExecuteQuery();
		int ssa::xmDBDataSet::Test();
		/******************************************************
		函数名   : ExecuteOneSql
		功能     : 执行一句用户指定的sql语句
		参数     : 
		返回值   : -1表示出错 0 表示成功
		备注     : 用户需要自己编写sql语句，并对执行结果负责
		-------------------------------------------------------
		典型用法 :ExecuteOneSql("delete from test2 where id>2");
		*******************************************************/
		int ExecuteOneSql(string strSql);

		/******************************************************
		函数名   : GetRowBuffer
		功能     : 获取当前结果集中的一条数据
		参数     : 
		返回值   : -1表示出错 0 表示成功
		备注     : 用户需要自己处理行数据，在fetch()成功后执行。
		-------------------------------------------------------
		典型用法 :char** myrow = GetRowBuffer();
		*******************************************************/
		char** GetRowBuffer();

		/******************************************************
		函数名   : Fetch
		功能     : 从数据集中获取一条记录
		参数     : 
		返回值   : -1表示出错 0 表示成功
		备注     : 返回0表示获取成功：用户可以通过GetValue()从
		数据条目中获取数据。
		-------------------------------------------------------
		典型用法 :if(Fetch()==0){...};
		*******************************************************/
		int Fetch();

		/******************************************************
		函数名   : GetInt
		功能     : 从数据集中获取Int型参数的值
		参数     : name：参数名称
		返回值   : int
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :if(Fetch()==0){int n = GetInt("年龄");};
		*******************************************************/
		int GetInt(string name);

		/******************************************************
		函数名   : GetUInt
		功能     : 从数据集中获取uint32_t型参数的值
		参数     : name：参数名称
		返回值   : uint32_t
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :if(Fetch()==0){uint32_t n = GetUInt("年龄");};
		*******************************************************/
		uint32_t GetUInt(string name);

		/******************************************************
		函数名   : GetFloat
		功能     : 从数据集中获取Float型参数的值
		参数     : name：参数名称
		返回值   : Float
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :if(Fetch()==0){float weight = GetFloat("体重");};
		*******************************************************/
		float GetFloat(string name);


		/******************************************************
		函数名   : GetString
		功能     : 从数据集中获取string型参数的值
		参数     : name：参数名称
		返回值   : string
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :if(Fetch()==0){string str = GetString("地址");};
		*******************************************************/
		string& GetString(string name);

		/******************************************************
		函数名   : GetChar
		功能     : 从数据集中获取Char型参数的值
		参数     : name：参数名称
		返回值   : char
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :if(Fetch()==0){char c = GetChar("标志");};
		*******************************************************/
		char GetChar(string name);

		/******************************************************
		函数名   : GetDate
		功能     : 从数据集中获取日期型参数的值
		参数     : name：参数名称
		返回值   : tm
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :if(Fetch()==0){tm n = GetDate("出生日期");};
		*******************************************************/
		tm GetDate(string name);


		
		/******************************************************
		函数名   : SetInt
		功能     : 访问数据库时（增、删、改、查）时，向SQL语句中设置Int型参数的值
		参数     : name：参数名称
		返回值   : -1表示出错 0 表示成功
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :int n = SetInt("年龄",value);
		*******************************************************/
		int SetInt(string name,int value);

		/******************************************************
		函数名   : SetUInt
		功能     : 访问数据库时（增、删、改、查）时，向SQL语句中设置Int型参数的值
		参数     : name：参数名称
		返回值   : -1表示出错 0 表示成功
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :int n = SetInt("年龄",value);
		*******************************************************/
		int SetUInt(string name,uint32_t value);

		/******************************************************
		函数名   : SetFloat
		功能     : 访问数据库时（增、删、改、查）时，向SQL语句中设置Float型参数的值
		参数     : name：参数名称
		返回值   : -1表示出错 0 表示成功
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :int n = SetFloat("体重",value);
		*******************************************************/
		int SetFloat(string name,float value);


		/******************************************************
		函数名   : SetString
		功能     : 访问数据库时（增、删、改、查）时，向SQL语句中设置string型参数的值
		参数     : name：参数名称
		返回值   : -1表示出错 0 表示成功
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :int n = SetString("地址",value);
		*******************************************************/
		int SetString(string name,string value);


		/******************************************************
		函数名   : SetChar
		功能     : 访问数据库时（增、删、改、查）时，向SQL语句中设置char型参数的值
		参数     : name：参数名称
		返回值   : -1表示出错 0 表示成功
		备注     : 参数名称在配置文件中指定
		-------------------------------------------------------
		典型用法 :int n = SetChar("标志",value);
		*******************************************************/
		int SetChar(string name,char value);


		/******************************************************
		函数名   : SetDate
		功能     : 访问数据库时（增、删、改、查）时，向SQL语句中设置时间类型（tm）参数的值
		参数     : name：参数名称
		返回值   : -1表示出错 0 表示成功
		备注     : 参数名称在配置文件中指定
		//所支持的日期类型
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
		典型用法 :int n = SetDate("出生日期",value);
		*******************************************************/
		int SetDate(string name,tm value);

		/******************************************************
		函数名   : GetMaxId
		功能     : 获取当前配置方案所指定的表中的最大ID
		参数     : 无
		返回值   : int型，数据表中的最大ID	  -1表示出错
		备注     : 配合数据插入操作，可以获取当前插入成功的条目的ID
		-------------------------------------------------------
		典型用法 :int n = GetMaxId();
		*******************************************************/
		int GetMaxId();

		/******************************************************
		函数名   : GetCount
		功能     : 获取当前配置方案所指定的表中的条目数
		参数     : 无
		返回值   : int型，数据表中的记录行数  -1表示出错
		备注     : 
		-------------------------------------------------------
		典型用法 :int n = GetCount();
		*******************************************************/
		int GetCount();

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
		函数名   : PrintMyScheme
		功能     : 输出访问数据库配置方案信息
		参数     : 无
		返回值   : 无
		备注     : 用作测试配置方案读取情况
		-------------------------------------------------------
		典型用法 :
		*******************************************************/
		void PrintMyScheme();

		/******************************************************
		函数名   : GetCurSql
		功能     : 输出当前的Sql语句
		参数     : 无
		返回值   : 无
		备注     : 用作测试当前的Sql语句准备情况
		-------------------------------------------------------
		典型用法 :
		*******************************************************/
		string GetCurSql();

		/******************************************************
		函数名   : SetBlobData
		功能     : 插入BLOB类型数据到数据库
		参数     : 无
		返回值   : -1表示出错 0 表示成功
		备注     : 大数据类型专用
		-------------------------------------------------------
		典型用法 :
		*******************************************************/
		int SetBlobData(string strName,SDbiBuf dbufValue,int nId = 0);

		/******************************************************
		函数名   : GetBlobData
		功能     : 从数据库获取BLOB类型数据
		参数     : 无
		返回值   : -1表示出错 0 表示成功
		备注     : 大数据类型专用
		-------------------------------------------------------
		典型用法 :
		*******************************************************/
		int GetBlobData(string strName,SDbiBuf& dbufValue,int nId);

		/******************************************************
		函数名   : GetBlobSql
		功能     : 获取访问大数据类型的SQL语句
		参数     : 无
		返回值   : 无
		备注     : 大数据类型专用，调试用，用于在访问之前了解Sql语句准备情况
		-------------------------------------------------------
		典型用法 :
		*******************************************************/
		string GetBlobSql();

		/******************************************************
		函数名   : GetInsertId
		功能     : 返回由以前的INSERT或UPDATE语句为AUTO_INCREMENT列生成的值。
		参数     : 无
		返回值   : xmUInt64型,成功返回大于零的整数，失败返回0
		备注     : 在包含AUTO_INCREMENT字段的表中执行了INSERT语句后，应使用该函数。
				   Mysql数据库专用。
		-------------------------------------------------------
		典型用法 :   PDataSet->Init("添加信息")  //执行Insert操作
		             ......
					 if(PDataSet->ExecuteQuery() == 0)
					 {
					    int currentId =	PDataSet->GetInsertId();
					 }
		*******************************************************/
		uint64_t GetInsertId();

		/******************************************************
		函数名   : IsOpen
		功能     : 判断当前数据库连接是否可用
		参数     : 无
		返回值   : bool型
		备注     : 
		-------------------------------------------------------
		典型用法 :
		*******************************************************/
		bool IsOpen();

	private:
		// 数据库连接基类的指针
		CDbiBase* m_pDbi;
		//用户名
		string m_strUser;
		//密码
		string m_strPassWd;
		//目标数据库
		string m_strTargetDb;
		//目标服务器地址（mysql用,通常为localhost或IP地址）
		string m_strTargetServer;
		// 访问数据库的配置方案描述
		SchemeMap m_mapMyScheme;
		// 当前的数据库配置方案
		string m_strCurScheme;
		// 当前要执行的SQL语句/模板
		string m_strCurSql;
		//最后一条出错信息
		string m_strLastError;
		//数据配置文件名称
		string m_strDbCfgFile;
		//访问大数据类型的SQL语句
		string m_strBlobSql;	 
		//解决返回字符串出现的异常（wx相关）
		string m_strValue;
	private:
		// 从配置文件中读取访问数据库的配置方案
		int ReadSchemeFromFile(string strCfgFile);

		// 获取指定名称的参数在数据集中的列序号
		int GetColIndex(string name);

		/******************************************************
		函数名   : GetParamAlias
		功能     : 获取参数别名：用于完成Sql语句中的值替换
		参数     : name
		返回值   : 无
		备注     : 因为C++中不支持同一级构造函数之间相互调用，只
		能将具体内容抽出来，作为一个函数。
		*******************************************************/
		// 
		string GetParamAlias(string name);

		/******************************************************
		函数名   : InitData
		功能     : 完成数据初始化：同构造函数
		参数     : strCfgFilePath
		返回值   : 无
		备注     : 因为C++中不支持同一级构造函数之间相互调用，只
				   能将具体内容抽出来，作为一个函数。
		*******************************************************/
		void InitData(string strCfgFilePath);

		/******************************************************
		函数名   : GetTableNameFromSql
		功能     : 从SQL语句中获取数据表的名称
		参数     : strSql
		返回值   : 数据表的名称
		备注     : 
		*******************************************************/
		string GetTableNameFromSql(string strSql);

		/******************************************************
		函数名   : VerifiedDbCfgFile
		功能     : 验证配置文件是否存在
		参数     : strFilePath
		返回值   : true：配置文件存在；false：配置文件不存在
		备注     : 
		*******************************************************/
		bool VerifiedDbCfgFile(string strFilePath);

		
};

}

#endif //_XM_MYSQL51_CDATA_SET_H