/**************************************************
文件名    :QueryScheme.h
功能      :存储从配置文件中读取的配置方案
相关文件  :QueryScheme.cpp
作者      :侯全超
-----------------------------------------------------
备注      : 初始接口描述
-----------------------------------------------------
修改记录 : 
日 期        版本   修改人         修改内容 
2009/08/06   1.0    <侯全超>       创建初稿
*****************************************************/


#ifndef _XM_QUERY_PARAM_H
#define _XM_QUERY_PARAM_H


#pragma once


#ifdef WIN32
#include "dbibase.h"
#else
#include "dbibase.h"
#endif


//#include <map>
using namespace std;	

namespace ssa {
	class CQueryScheme
	{
	public:
		CQueryScheme(void);
		~CQueryScheme(void);
	public:
		//基本SQL语句
		string strSql;
		//传入参数映射
		map<string,string>	m_mapIn;
		//传出参数映射
		map<string,string>	m_mapOut;
		//查询所得：数据集中的列和列序号
		map<string,int>     m_mapCol;
		//是否完成初始化
		bool m_bInit;
		//是否是扩展应用
		bool m_bExtend;

		// 资源回收，清空map表
		int clear(void);
	};
}

#endif //_XM_QUERY_PARAM_H