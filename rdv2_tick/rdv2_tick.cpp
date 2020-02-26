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
	// 1. ʹ������������
	//my_rdv() : rdv_2(ConfigMode::Inbuilt) {}

	// 2. ʹ��INI�����ļ�
	// ��IDE������ʱ�ĵ�ǰ����Ŀ¼Ϊrdv2_demo\rdv2_tick
	// ����ini�ļ���Ҫ����rdv2_tickĿ¼��
	//my_rdv() : rdv_2(ConfigMode::ini, "rdv2_config.ini") {}

	// 3. ʹ��XML�����ļ�(ȱʡΪrdv2_config.xml)
	// XML������ȱʡ��ʽ��������ʽ����
	// �����ļ��Ĵ��·��ͬ2

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
	// Ҳ����ֱ�Ӵ���rdv_2����ָ����������
	//rdv_2 rdv(ConfigMode::Inbuilt, "");
	//rdv_2 rdv(ConfigMode::ini, "myconfig.ini");
	//rdv_2 rdv(ConfigMode::xml, "myconfig.xml");

	my_rdv rdv;
	// ������VDRǰ�������޸�������
	//rdv.conf().set<string>(ConfigItem::VDR_group_ip, string("224.0.0.100"));
	rdv.startup();
	rdv.enable_tick();




	system("pause");
    return 0;
}

