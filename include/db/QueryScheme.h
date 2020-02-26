/**************************************************
�ļ���    :QueryScheme.h
����      :�洢�������ļ��ж�ȡ�����÷���
����ļ�  :QueryScheme.cpp
����      :��ȫ��
-----------------------------------------------------
��ע      : ��ʼ�ӿ�����
-----------------------------------------------------
�޸ļ�¼ : 
�� ��        �汾   �޸���         �޸����� 
2009/08/06   1.0    <��ȫ��>       ��������
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
		//����SQL���
		string strSql;
		//�������ӳ��
		map<string,string>	m_mapIn;
		//��������ӳ��
		map<string,string>	m_mapOut;
		//��ѯ���ã����ݼ��е��к������
		map<string,int>     m_mapCol;
		//�Ƿ���ɳ�ʼ��
		bool m_bInit;
		//�Ƿ�����չӦ��
		bool m_bExtend;

		// ��Դ���գ����map��
		int clear(void);
	};
}

#endif //_XM_QUERY_PARAM_H