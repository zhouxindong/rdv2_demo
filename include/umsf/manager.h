/******************************************************************************/
/** @file manager.h
@brief
	定义xmManager类。
@author 邓华 dehxya@163.com
@par 所属模块
	umsf
@par 相关文件
	manager.cpp
@see ssa::xmUMSF
@copyright (C), 2011-2022, Gonna. All rights reserved.
*******************************************************************************/
#ifndef __SSA_UMSF_MANAGER_H
#define __SSA_UMSF_MANAGER_H
#include "mrm.h"
#include "publicres.h"
#include "smfstatistics.h"

namespace ssa
{
	class xmUMSF;
	class xmManager : public xmMsgHandler, public xmSMFStatistics
	{
		friend xmUMSF;
	public:
		//	仿真系统
		xmISystem* System() const
		{
			return m_pPublicRes->System();
		}

		//	仿真节点
		xmINode* Node()
		{
			return m_pPublicRes->Node();
		}

		//	遍历MRM
		size_t ModelCount() const
		{
			return m_mapMrm.Size();
		}

		xmMrm* Mrm(size_t uIndex)
		{
			if (uIndex >= m_mapMrm.Size())
			{
				return NULL;
			}
			return m_mapMrm.IndexValue(uIndex);
		}

		xmMrm* Mrm(const xmString& strModelName)
		{
			t_MrmMap::t_Pos pos = m_mapMrm.Find(strModelName);
			if (pos == m_mapMrm.INVALID_POS())
			{
				return NULL;
			}
			return m_mapMrm.PosValue(pos);
		}

		//	整个UMSF中所有消息队列长度统计
		size_t TotalMsgCount() const;
		void TotalMsgPause();
		void TotalMsgResume();


		//	启动工作负载统计
		virtual void StartWLS(unsigned int uTime)
		{
			StartWorkLoadStatistics(uTime, (xmThreadw::OnWLSDoneFunc)log_WLS, this);
		}
	private:
		//	输出工作负载
		static void log_WLS(double fWLPrecent, void* pParam)
		{
			xmLog::info(((xmManager*)pParam)->m_strFlag, "Work load %.2f%% pre second.", ((xmMsgHandler*)pParam)->WorkLoad());
		}


	private:
		//	仅供xmUMSF使用
		xmManager(void);
		~xmManager(void);

		xmRet Initialize(const char* strUmsfConfigFile, const char* strModelConfigFile);
		xmRet Release(unsigned int uTimeoutMillis = INFINITE);
		bool IsRunning() const;
		xmString m_strLastError;

	private:
		xmString m_strNodeID;
		xmString m_strNodeName;
		bool     m_bSystemLog;		//	是否记录仿真系统发送来的日志信息

		//	管理记录所有注册到仿真系统中的输出数据
		typedef xmOrderMap<xmString, xmPtr<xmClass>> t_CombineClassMap;
		//	管理MRM
		typedef xmOrderMap<xmString, xmPtr<xmMrm> > t_MrmMap;
		//	管理MRTC
		typedef xmOrderMap<xmString, xmMrtc*> t_MrtcPtrMap;


		//	模型运行管理器（Model Running Manager）表，一个模型类对应一个MRM
		t_MrmMap m_mapMrm;
		//	模型运行时环境（Model Runtime Context）表，一个模型实例对应一个MRTC
		t_MrtcPtrMap m_mapMrtc;
		//	记录所有注册到仿真系统中的输出数据
		//	key表示数据名称或组合后的数据名称
		//	value表示该数据的类型，如果为空，表示该数据由MRTC直接注册，否则表示经过组合后的数据类型
		t_CombineClassMap m_mapSystemData;

		//	UMSF运行中的公共资源
		//	包括仿真系统访问接口、触发器、预处理器
		xmPublicResource* m_pPublicRes;

		//	记录日志目标
		std::vector<size_t> m_vLogTarget;

		//	操作消息队列时的保护
		mutable xmMutex m_MsgHandleMutex;

		void _LoadUmsfConfig(const char* strUmsfConfigFile);
		void _LoadModelConfig(const char* strModelConfigFile);
		void _LoadModel(tinyxml2::XMLElement* root);
		//	加载一个PACKAGE配置信息
		//	mapInstance是目前尚未使用的实例列表
		//	PACKAGE是xml对象
		//	strPath是这个PACKAGE的数据路径
		//	packageName：输出参数，表示这个Package的名称
		//	packageValue：输出参数，表示这个Package的数值
		//	返回数值的类指针
		xmClass* _LoadPackage(
			xmOrderMap<xmString, xmMrtc*>& mapInstance, 
			xmOrderMap<xmString, std::vector<xmString>>& mapInstanceData,
			tinyxml2::XMLElement* PACKAGE,
			const xmString& strPath, 
			xmString& packageName, 
			xmValue& packageValue);
		//	如果vData不为NULL，其中记录的数据均已在记录中删除
		//	返回NULL，表示不组合，vData如果为NULL，表示使用所有输出数据，否则表示使用vData中指定的输出数据
		xmClass* _LoadUsing(
			xmOrderMap<xmString, xmMrtc*>& mapInstance,
			xmOrderMap<xmString, std::vector<xmString>>& mapInstanceData,
			tinyxml2::XMLElement* USING,
			xmMrtc*& pMrtc,						//	实例指针
			std::vector<xmString>& usingData,	//	使用的数据
			xmString& strCombineName,			//	组合名称
			xmValue& combineValue				//	组合数值
		);
		void __log_level_config(size_t uTargetID, void* configField);

		//	1、检查数据是否重复
		//	2、仿真系统中注册数据
		//	3、记录输出数据
		void iSystem_RegisterData(const xmString& strDataName, const xmValue& aValue, xmClass* pClass, const xmString& strAttribute = NULL);
		//	生成路径
		xmString _MakePath(const xmString& strPathHead, const xmString& strPathTail);

		//	分发消息
		void DispatchMsg(const xmMessage& aMsg)
		{
			for (size_t i = 0; i < m_mapMrm.Size(); i++)
			{
				m_mapMrm.IndexValue(i)->DispatchMsg(aMsg);
			}
		}
		//	仿真系统消息响应
		void __OnWorldClick(const smWorldClick& aMsg);
		void __OnSystemTime(const smSystemTime& aMsg);
		void __OnDataUpdate(const smDataUpdate& aMsg);
		void __OnCtrlCommand(const smCtrlCommand& aMsg);
		void __OnSystemEvent(const smSystemEvent& aMsg);
		void __OnBulletinGet(const smBulletinGet& aMsg);
		void __OnMessageGet(const smMessageGet& aMsg);
		void __OnSystemLog(const smSystemLog& aMsg);
		void OnWorldClick(const smWorldClick& aMsg);
		void OnSystemTime(const smSystemTime& aMsg);
		void OnDataUpdate(const smDataUpdate& aMsg);
		void OnCtrlCommand(const smCtrlCommand& aMsg);
		void OnSystemEvent(const smSystemEvent& aMsg);
		void OnBulletinGet(const smBulletinGet& aMsg);
		void OnMessageGet(const smMessageGet& aMsg);
		void OnSystemLog(const smSystemLog& aMsg);
		xmCREATE_MESSAGE_MAP(xmManager);
	};
}
#endif	//	__SSA_UMSF_MANAGER_H