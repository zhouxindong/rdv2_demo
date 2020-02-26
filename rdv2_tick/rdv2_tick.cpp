// rdv2_tick.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rdv2.h"

#include <iostream>
#include <chrono>

using namespace std;
using namespace rdv2;

class my_rdv : public rdv_2
{
public:
	// 1. 使用内置配置类
	//my_rdv() : rdv_2(ConfigMode::Inbuilt) {}

	// 2. 使用INI配置文件
	// 在IDE中运行时的当前工作目录为rdv2_demo\rdv2_tick
	// 所以ini文件需要放在rdv2_tick目录下
	//my_rdv() : rdv_2(ConfigMode::ini, "rdv2_config.ini") {}

	// 3. 使用XML配置文件(缺省为rdv2_config.xml)
	// XML配置是缺省方式，无需显式定义
	// 配置文件的存放路径同2

	void on_tick(const ssa::xmMSGTimerClick& t) override
	{
		//if (_first)
		//{
		//	_first = false;
		//	_start = chrono::system_clock::now();
		//}
		//if (++count > 100000)
		//{
		//	if (_timeout)
		//	{
		//		auto lasting = chrono::system_clock::now() - _start;
		//		std::cout << lasting.count() << '\n';
		//		_timeout = false;
		//	}
		//}

		std::cout << t.m_lAbsTime << '\n';
		std::cout << t.m_lTimeCount << '\n';
		std::cout << t.m_nSyncCycles << '\n';
		std::cout << t.m_nTickTime << '\n';
	}

private:
	int64_t count = 0;
	chrono::system_clock::time_point _start;
	bool _first = true;
	bool _timeout = true;
};

int main()
{
	// 也可以直接创建rdv_2对象，指定配置类型
	//rdv_2 rdv(ConfigMode::Inbuilt, "");
	//rdv_2 rdv(ConfigMode::ini, "myconfig.ini");
	//rdv_2 rdv(ConfigMode::xml, "myconfig.xml");

	my_rdv rdv;
	// 在连接VDR前，可以修改配置项
	//rdv.conf().set<string>(ConfigItem::VDR_group_ip, string("224.0.0.100"));
	rdv.startup();
	rdv.enable_tick();




	system("pause");
    return 0;
}

