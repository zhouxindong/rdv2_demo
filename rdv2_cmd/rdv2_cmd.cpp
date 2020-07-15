// rdv2_cmd.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "send.h"
#include <iostream>

using namespace std;
using namespace rdv2;

uint64_t load_cnt = 0;
uint64_t init_cnt = 0;
uint64_t start_cnt = 0;
uint64_t finish_cnt = 0;
uint64_t freeze_cnt = 0;
uint64_t unfreeze_cnt = 0;

class my_rdv : public rdv_2
{
public:
	void on_load() override {
		cout << std::string("on_load: ") << ++load_cnt << endl;
	}

	void on_init() override {
		cout << std::string("on_init: ") << ++init_cnt << endl;
	}

	void on_start() override {
		cout << std::string("on_start: ") << ++start_cnt << endl;
	}

	void on_finish() override {
		cout << std::string("on_finish: ") << ++finish_cnt << endl;
	}

	void on_freeze() override {
		cout << std::string("on_freeze: ") << ++freeze_cnt << endl;
	}

	void on_unfreeze() override {
		cout << std::string("on_unfreeze: ") << ++unfreeze_cnt << endl;
	}

	void on_speed(int step) override {
		cout << std::string("on_speed(") << step << std::string(")") << endl;
	}
};

// 1. add command data and watched 
//int main()
//{
//	my_rdv rdv;
//	rdv.startup();
//
//	rdv.register_cmd("运行控制指令测试数据集");
//	_SPIN_LOCK.test_and_set();
//	thread t_send(send_proc, &rdv); 
//
//	rdv.listen_cmd(true);
//
//	cout << "Press any key to quit!" << endl;
//	cin.get();
//
//	_SPIN_LOCK.clear();
//	t_send.join();
//
//	return 0;
//}

// 2. watch command but not add command data before
//int main()
//{
//	my_rdv rdv;
//	rdv.startup();
//
//	//rdv.register_cmd("运行控制指令测试数据集");
//	_SPIN_LOCK.test_and_set();
//	thread t_send(send_proc, &rdv);
//
//	rdv.listen_cmd(true);
//
//	cout << "Press any key to quit!" << endl;
//	cin.get();
//
//	_SPIN_LOCK.clear();
//	t_send.join();
//
//	return 0;
//}

// 3. watch command first, and then add command data to VDR
int main()
{
	my_rdv rdv;
	rdv.startup();

	rdv.listen_cmd(true);

	cout << "press any key to add command data to VDR\n";
	cin.get();

	rdv.register_cmd("运行控制指令测试数据集");
	_SPIN_LOCK.test_and_set();
	thread t_send(send_proc, &rdv);

	cout << "Press any key to quit!" << endl;
	cin.get();

	_SPIN_LOCK.clear();
	t_send.join();

	return 0;
}
