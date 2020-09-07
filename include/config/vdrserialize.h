#pragma once

#ifndef __VDR_SERIALIZE_H
#define __VDR_SERIALIZE_H

#include "../base/base.h"
#include "../vdr/vdr.h"
#include "../vdr/accesspoint.h"
#include "vdrconfig.h"
using namespace ssa;

/**
*  VDR用户序列化. 将VDR中的数据信息保存，并可以从数据中恢复VDR。
*/
class xmCONFIG_EXPORT xmVDRSerialize
{
public:
	xmVDRSerialize();
	~xmVDRSerialize();

public:
	/**
	* 保存VDR数据信息，必须要和一个VDR连接。
	* @param pAp 是指被连接的入口指针。
	* @param dsName 需要保存的数据集名字，可以多个数据集名字。
	* @param fileName 保存文件的名字。
	*/
	bool  SaveDataSetSnapshots(xmAccessPoint* pAp, std::vector<std::string> dsName, const char* fileName);
	/**
	* 保存VDR所有数据信息，必须要和一个VDR连接。
	* @param pAp 是指被连接的入口指针。
	* @param fileName 保存文件的名字。
	*/
	bool  SaveDataSetSnapshots(xmAccessPoint* pAp, const char* fileName);
	/**
	* 恢复VDR数据信息，必须要和一个VDR连接。
	* @param pAp 是指被连接的入口指针。
	* @param fileName 保存文件的名字。
	*/
	bool  RestoreDataSetSnapshots(xmAccessPoint* pAp, const char* fileName,bool bNeedInitWithSavedValue = true);
	/**
	* 恢复VDR数据信息，必须要和一个VDR连接，这个函数接口是为了恢复现场时使用。
	* @param pAp 是指被连接的入口指针。
	* @param nEndPoint 是更新那一端的参数，0为默认，目前与1同，1表示更新所有节点的信息，2表示只更新服务器。
	* @param fileName  保存文件的名字。
	*/
	bool  RestoreDataSetSnapshots(xmAccessPoint* pAp, const char* fileName,int nEndPoint, bool bNeedInitWithSavedValue = true);
};

#endif // !__VDR_SERIALIZE_H

