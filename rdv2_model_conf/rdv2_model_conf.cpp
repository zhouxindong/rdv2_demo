// rdv2_model_conf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rdv2.h"
#include <iostream>

using namespace std;
using namespace rdv2;

int main()
{
	rdv_2 rdv;
	rdv.startup();

	//rdv.export_model_conf("1.config"); // add <OUTPUT_DATA> to VDR

	rdv.export_model_conf("1.config", { OUTPUT_DATA_LABEL, INNER_DATA_LABEL }); // add <OUTPUT_DATA> && <INNER_DATA> to VDR

	cout << rdv.model_inst_name("1.config") << endl;
	cout << rdv.model_node_name("1.config") << endl;

	map<string, string> l2g = rdv.local2gsa("1.config");
	for_each(l2g.cbegin(), l2g.cend(), [](auto& p) {
		cout << "local: " << p.first << " gsa: " << p.second << endl;
	});

	map<string, string> g2l = rdv.gsa2local("1.config");
	for_each(g2l.cbegin(), g2l.cend(), [](auto& p) {
		cout << "gsa: " << p.first << " local: " << p.second << endl;
	});

	auto p = rdv.parse_data("1.config", INPUT_DATA_LABEL);
	for_each(p.cbegin(), p.cend(), [](pair<string, Type2_component*> item) {
		cout << item.first << string(": ") << item.second->to_string() << endl;
	});

	for_each(p.cbegin(), p.cend(), [](pair<string, Type2_component*> item) {
		delete item.second;
	});

	cout << "INSTNAME: " << rdv.model_inst_name("1.config") << endl;
	cout << "NODENAME: " << rdv.model_node_name("1.config") << endl;
	cout << "INST_SERV_ENTRY: " << rdv.inst_serv_entry("1.config") << endl;

	system("pause");

    return 0;
}

