#ifndef __SSA_PROXYFACTORY_H
#define __SSA_PROXYFACTORY_H

#pragma once
#include "export.h"
#include "iproxy.h"
#include "../base/singleton.h"

namespace ssa
{
	class xmAbstractFactory
	{
	public:
		virtual xmIProxy* CreateProxy() = 0;
	};

	template<class T> class xmProxyFactory : public xmAbstractFactory
	{
	public:
		xmIProxy* CreateProxy()
		{
			return new T();
		}
	};

	class xmVDR_EXPORT xmProxyFactoryMgr : public xmSingleton<xmProxyFactoryMgr>
	{
	public:
		~xmProxyFactoryMgr();

		xmIProxy* CreateProxy(int id);

		void RegisterFactory(xmAbstractFactory* pf);

		xmAbstractFactory* GetFactory(int id);

		void Clear();
	};

	//没有搞清楚xmProxyFactoryMgr::Instance().RegisterFactory(new xmProxyFactory<T>())为什么不能在全局执行。
	template<class T>
	class xmRegisterProxy
	{
	public:
		xmRegisterProxy()
		{
			xmProxyFactoryMgr cfg;
			cfg.RegisterFactory(new xmProxyFactory<T>());
		}
	};

#define REGISTER_PROXY(pcp) static xmRegisterProxy<pcp> rc_##pcp;

};

#endif