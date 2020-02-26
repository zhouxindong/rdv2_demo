#ifndef __SSA_COMMPROXYFACTORY_H
#define __SSA_COMMPROXYFACTORY_H

#pragma once
#include "icommproxy.h"
#include "../base/singleton.h"

namespace ssa
{
	class xmAbstractCommproxyFactory
	{
	public:
		virtual xmICommProxy* CreateProxy() = 0;
	};

	template<class T> class xmCommproxyFactory : public xmAbstractCommproxyFactory
	{
	public:
		xmICommProxy* CreateProxy()
		{
			return new T();
		}
	};

	class xmCommproxyFactoryMgr : public xmSingleton<xmCommproxyFactoryMgr>
	{
	public:
		~xmCommproxyFactoryMgr();

		xmICommProxy* CreateProxy(xmEVDRCommProxyType id);

		void RegisterFactory(xmAbstractCommproxyFactory* pf);

		xmAbstractCommproxyFactory* GetFactory(xmEVDRCommProxyType id);

		void Clear();
	};

	//没有搞清楚xmCommproxyFactoryMgr::Instance().RegisterFactory(new xmCommproxyFactory<T>())为什么不能在全局执行。
	template<class T>
	class xmRegisterCommProxy
	{
	public:
		xmRegisterCommProxy()
		{
			xmCommproxyFactoryMgr cfg;
			cfg.RegisterFactory(new xmCommproxyFactory<T>());
		}
	};

#define REGISTER_COMMPROXY(pcp) static xmRegisterCommProxy<pcp> rc##_pcp;

};

#endif