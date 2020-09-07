#pragma once

#ifndef __VDR_SERIALIZE_H
#define __VDR_SERIALIZE_H

#include "../base/base.h"
#include "../vdr/vdr.h"
#include "../vdr/accesspoint.h"
#include "vdrconfig.h"
using namespace ssa;

/**
*  VDR�û����л�. ��VDR�е�������Ϣ���棬�����Դ������лָ�VDR��
*/
class xmCONFIG_EXPORT xmVDRSerialize
{
public:
	xmVDRSerialize();
	~xmVDRSerialize();

public:
	/**
	* ����VDR������Ϣ������Ҫ��һ��VDR���ӡ�
	* @param pAp ��ָ�����ӵ����ָ�롣
	* @param dsName ��Ҫ��������ݼ����֣����Զ�����ݼ����֡�
	* @param fileName �����ļ������֡�
	*/
	bool  SaveDataSetSnapshots(xmAccessPoint* pAp, std::vector<std::string> dsName, const char* fileName);
	/**
	* ����VDR����������Ϣ������Ҫ��һ��VDR���ӡ�
	* @param pAp ��ָ�����ӵ����ָ�롣
	* @param fileName �����ļ������֡�
	*/
	bool  SaveDataSetSnapshots(xmAccessPoint* pAp, const char* fileName);
	/**
	* �ָ�VDR������Ϣ������Ҫ��һ��VDR���ӡ�
	* @param pAp ��ָ�����ӵ����ָ�롣
	* @param fileName �����ļ������֡�
	*/
	bool  RestoreDataSetSnapshots(xmAccessPoint* pAp, const char* fileName,bool bNeedInitWithSavedValue = true);
	/**
	* �ָ�VDR������Ϣ������Ҫ��һ��VDR���ӣ���������ӿ���Ϊ�˻ָ��ֳ�ʱʹ�á�
	* @param pAp ��ָ�����ӵ����ָ�롣
	* @param nEndPoint �Ǹ�����һ�˵Ĳ�����0ΪĬ�ϣ�Ŀǰ��1ͬ��1��ʾ�������нڵ����Ϣ��2��ʾֻ���·�������
	* @param fileName  �����ļ������֡�
	*/
	bool  RestoreDataSetSnapshots(xmAccessPoint* pAp, const char* fileName,int nEndPoint, bool bNeedInitWithSavedValue = true);
};

#endif // !__VDR_SERIALIZE_H

