// rdv2_watch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "add.h"
#include <iostream>

using namespace rdv2;
using namespace std;

void int8_watcher(any& a) {
	cout << "(global function)int8 data: " << (int)a.as<int8_t>() << endl;
}

class my_wacher {
	std::string _name;
public:
	my_wacher(std::string& name) : _name(name) {}

	void operator()(any& a) {
		cout << "(functor object)" << _name.c_str() << ": " 
			<< (int)a.as<uint8_t>() << endl;
	}
};

int main()
{
	rdv_2 rdv;
	rdv.startup();

	add(rdv); // add dataset & data
	Sleep(500);

	_SPIN_LOCK.test_and_set();
	thread t_w(write_proc, &rdv); // start set_value thread

	rdv.watch(int8_da, ssa::xmEDataType::xmDT_INT8, valuer(int8_watcher));
	rdv.watch(uint8_da, ssa::xmEDataType::xmDT_UINT8, valuer(my_wacher(string("watcher obj"))));
	
	rdv.watch(int16_da, ssa::xmEDataType::xmDT_INT16,
		valuer([](any& a) {
		cout << "int16: " << a.as<int16_t>() << endl;
	}));

	rdv.watch(uint16_da, ssa::xmEDataType::xmDT_UINT16,
		valuer([](any& a) {
		cout << "uint16: " << a.as<int16_t>() << endl;
	}));

	rdv.watch(int32_da, ssa::xmEDataType::xmDT_INT32,
		valuer([](any& a) {
		cout << "int32: " << a.as<int32_t>() << endl;
	}));

	rdv.watch(uint32_da, ssa::xmEDataType::xmDT_UINT32,
		valuer([](any& a) {
		cout << "uint32: " << a.as<uint32_t>() << endl;
	}));

	rdv.watch(int64_da, ssa::xmEDataType::xmDT_INT64,
		valuer([](any& a) {
		cout << "int64: " << a.as<int64_t>() << endl;
	}));

	rdv.watch(uint64_da, ssa::xmEDataType::xmDT_UINT64,
		valuer([](any& a) {
		cout << "uint64: " << a.as<uint64_t>() << endl;
	}));

	rdv.watch(float_da, ssa::xmEDataType::xmDT_FLOAT,
		valuer([](any& a) {
		cout << "float: " << a.as<float>() << endl;
	}));

	rdv.watch(double_da, ssa::xmEDataType::xmDT_DOUBLE,
		valuer([](any& a) {
		cout << "double: " << a.as<double>() << endl;
	}));

	rdv.watch(string_da, ssa::xmEDataType::xmDT_STRING,
		valuer([](any& a) {
		cout << "string: " << a.as<string>() << endl;
	}));

	rdv.watch(buffer_da, ssa::xmEDataType::xmDT_BUFFER,
		valuer([](any& a) {
		vector<uint8_t> buf = a.as<vector<uint8_t>>();
		cout << buf;
	}));

	rdv.watch(struct_da, ssa::xmEDataType::xmDT_STRUCT,
		valuer([](any& a) {
		ssa::xmValue val = a.as<ssa::xmValue>();
		cout << "p1=" << (int)val["p1"].i8() << ", " << "p2=" << val["p2"].i32() << endl;
		cout << "p3.sp1=" << val["p3"]["sp1"].f32() << "  p3.sp2=" << val["p3.sp2"].f64() << endl;
		cout << "p4=" << val["p4"].u32() << endl;

		auto val_buf = val["p5"].buffer();
		cout << val_buf;
	}));

	rdv.watch(binary_da, xmDT_BSTREAM, valuer([](any& a) {
		bistream bis = std::move(a.as<bistream>());
		int8_t br_int8; bis >> br_int8;
		uint8_t br_uint8; bis >> br_uint8;
		int16_t br_int16; bis >> br_int16;
		uint16_t br_uint16; bis >> br_uint16;
		cout << "binary block: " << (int)br_int8 << " " << (int)br_uint8 << " " << br_int16 << " " << br_uint16 << endl;
		// 后面不变的数据就不一一读取了
	}));

	rdv.unwatch(int8_da);
	rdv.unwatch(uint8_da);
	rdv.unwatch(int16_da);
	rdv.unwatch(uint32_da);
	rdv.unwatch(float_da);

	cout << "Press any key to quit!" << endl;
	cin.get();

	_SPIN_LOCK.clear();
	t_w.join();

    return 0;
}

