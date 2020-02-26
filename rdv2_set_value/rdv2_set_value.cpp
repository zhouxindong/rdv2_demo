// rdv2_set_value.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rdv2.h"
#include <string>
#include <numeric>
#include <iostream>

using namespace rdv2;
using namespace std;

int main()
{
	rdv_2 rdv; // 使用缺省的XML配置文件
	rdv.startup();

#pragma region add dataset & data

	string ds_name = "雷达数据集";
	string int8_da = "int8数据";
	string uint8_da = "uint8数据";
	string int16_da = "int16数据";
	string uint16_da = "uint16数据";
	string int32_da = "int32数据";
	string uint32_da = "uint32数据";
	string int64_da = "int64数据";
	string uint64_da = "uint64数据";
	string float_da = "float数据";
	string double_da = "double数据";
	string string_da = "string数据";
	string buffer_da = "buffer数据";
	string struct_da = "struct数据";
	string binary_da = "binary数据";

	rdv.add_dataset(ds_name);
	rdv.begin_update(ds_name);

	rdv.add_data(int8_da, ds_name, ssa::xmEDataType::xmDT_INT8);
	rdv.add_data(uint8_da, ds_name, ssa::xmEDataType::xmDT_UINT8);
	rdv.add_data(int16_da, ds_name, ssa::xmEDataType::xmDT_INT16);
	rdv.add_data(uint16_da, ds_name, ssa::xmEDataType::xmDT_UINT16);
	rdv.add_data(int32_da, ds_name, ssa::xmEDataType::xmDT_INT32);
	rdv.add_data(uint32_da, ds_name, ssa::xmEDataType::xmDT_UINT32);
	rdv.add_data(int64_da, ds_name, ssa::xmEDataType::xmDT_INT64);
	rdv.add_data(uint64_da, ds_name, ssa::xmEDataType::xmDT_UINT64);
	rdv.add_data(float_da, ds_name, ssa::xmEDataType::xmDT_FLOAT);
	rdv.add_data(double_da, ds_name, ssa::xmEDataType::xmDT_DOUBLE);
	rdv.add_data(string_da, ds_name, ssa::xmEDataType::xmDT_STRING, 64);
	rdv.add_data(buffer_da, ds_name, ssa::xmEDataType::xmDT_BUFFER, 16);

	// "{	\
			//	'p1':'INT8',		\
		//	'p2':'INT32',		\
		//	'p3':{				\
		//		'sp1':'FLOAT',	\
		//		'sp2':'DOUBLE'	\
		//	},					\
		//	'p4':'UINT32',		\
		//	'p5':'BUFFER9'		\
		//}";
	Compound_type root({
		new Simple_type("p1", xmClassType::INT8$),
		new Simple_type("p2", xmClassType::INT32$),
		new Compound_type("p3",{
			new Simple_type("sp1", xmClassType::FLOAT$),
			new Simple_type("sp2", xmClassType::DOUBLE$)
		}),
		new Simple_type("p4", xmClassType::UINT32$),
		new Buffer_type("p5", 9)
	});
	ssa::xmClass c = root.xm_class();
	ssa::xmValue v(c);
	rdv.add_data(struct_da, ds_name, ssa::xmEDataType::xmDT_STRUCT, 0, &v);

	// 二进制流数据块, bos构造器可以指定字节序，缺省为little-endian
	bostream bos;
	bos << (int8_t)1;
	bos << (uint8_t)2;
	bos << (int16_t)-99;
	bos << (uint16_t)100;
	bos << 323;
	bos << (uint32_t)999;
	bos << (int64_t)3325;
	bos << (uint64_t)234234;
	bos << (float)89.923;
	bos << 3.1415926;
	bos << false;
	string bstr("this is a string. 这是一个字符串!");
	bos << bstr;

	uint8_t buf[16];
	for (int i = 0; i < sizeof(buf); ++i) {
		buf[i] = i + 1;
	}
	bos << make_pair(buf, buf + sizeof(buf)); // (首元素指针，尾后元素指针)

	uint8_t buf2[24];
	for (int i = 0; i < sizeof(buf2); ++i) {
		buf2[i] = i;
	}
	bos << make_pair(buf2, sizeof(buf2)); // (首元素指针，字节数组长度)

	std::vector<uint8_t> vs{ 11,22,33,44,55,66,77,88,99 };
	bos << vs;

	rdv.add_data(binary_da, ds_name, bos);
	rdv.end_update(ds_name);

	rdv.lock(ds_name); // 锁定数据集，使其不能再添加数据

#pragma endregion

#pragma region set_value

#undef max

	rdv.set_value(int8_da, numeric_limits<int8_t>::max());
	rdv.set_value(uint8_da, numeric_limits<uint8_t>::max());
	rdv.set_value(int16_da, numeric_limits<int16_t>::max());
	rdv.set_value(uint16_da, numeric_limits<uint16_t>::max());
	rdv.set_value(int32_da, numeric_limits<int32_t>::max());
	rdv.set_value(uint32_da, numeric_limits<uint32_t>::max());
	rdv.set_value(int64_da, numeric_limits<int64_t>::max());
	rdv.set_value(uint64_da, numeric_limits<uint64_t>::max());
	rdv.set_value(float_da, numeric_limits<float>::max());
	rdv.set_value(double_da, numeric_limits<double>::max());
	rdv.set_value(string_da, "今天是2019年10月21日下午17时09分");

#define max(a,b) (((a)>(b))?(a):(b))

	uint8_t buf3[16]{ 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16 };
	rdv.set_value(buffer_da, buf, 16);

	v["p1"].i8() = 10;
	v["p2"].i32() = 20;
	v["p3.sp1"].f32() = 31;
	v["p3"]["sp2"].f64() = 32;
	v["p4"].u32() = 40;
	v["p5"].buffer().Set(0, xmB4(51, 52, 53, 54, 55, 56, 57, 58, 59));
	rdv.set_value(struct_da, v);

	// 二进制数据块在初始化时其实已经赋值了，此时无需再赋值
	// 如果执行了下面的赋值代码，则数据块的起始部分被覆盖，后面数据仍保留
	// 读取数据时，如果数据覆盖导致覆盖边缘数据发生了错位，可以通过读取N个uint8_t定位到正确位置
	//bostream bos2;
	//bos2 << (float)100.1;
	//bos2 << 3456;
	//bos2 << 1278;
	//bos2 << true;
	//rdv.set_value(binary_da, bos2);


#pragma endregion

	Sleep(100);

#pragma region get_value

	auto r_int8		= rdv.get_value<int8_t>(int8_da);
	auto r_uint8	= rdv.get_value<uint8_t>(uint8_da);
	auto r_int16	= rdv.get_value<int16_t>(int16_da);
	auto r_uint16	= rdv.get_value<uint16_t>(uint16_da);
	auto r_int32	= rdv.get_value<int32_t>(int32_da);
	auto r_uint32	= rdv.get_value<uint32_t>(uint32_da);
	auto r_int64	= rdv.get_value<int64_t>(int64_da);
	auto r_uint64	= rdv.get_value<uint64_t>(uint64_da);
	auto r_float	= rdv.get_value<float>(float_da);
	auto r_double	= rdv.get_value<double>(double_da);

	if (r_int8.first) cout << "int8: " << (int)r_int8.second << endl;
	if (r_uint8.first) cout << "uint8: " << (int)r_uint8.second << endl;
	if (r_int16.first) cout << "int16: " << r_int16.second << endl;
	if (r_uint16.first) cout << "uint16: " << r_uint16.second << endl;
	if (r_int32.first) cout << "int32: " << r_int32.second << endl;
	if (r_uint32.first) cout << "uint32: " << r_uint32.second << endl;
	if (r_int64.first) cout << "int64: " << r_int64.second << endl;
	if (r_uint64.first) cout << "uint64: " << r_uint64.second << endl;
	if (r_float.first) cout << "float: " << r_float.second << endl;
	if (r_double.first) cout << "double: " << r_double.second << endl;

	string str;
	bool r_str = rdv.get_value(string_da, str);
	if (r_str) cout << str << std::endl;

	std::vector<uint8_t> v1;
	bool r_buf = rdv.get_value(buffer_da, v1);
	if (r_buf) 	cout << v1;

	ssa::xmValue val3;
	bool r_struct = rdv.get_value(struct_da, val3);
	if (r_struct)
	{
		cout << "p1=" << (int)val3["p1"].i8() << ", " << "p2=" << val3["p2"].i32() << endl;
		cout << "p3.sp1=" << val3["p3"]["sp1"].f32() << "  p3.sp2=" << val3["p3.sp2"].f64() << endl;
		cout << "p4=" << val3["p4"].u32() << endl;

		auto val_buf = val3["p5"].buffer();
		cout << val_buf;
	}

	bistream bis;
	auto recv_bis = rdv.get_value(binary_da, bis);
	if (recv_bis) {
		int8_t br_int8; bis >> br_int8;
		uint8_t br_uint8; bis >> br_uint8;
		int16_t br_int16; bis >> br_int16;
		uint16_t br_uint16; bis >> br_uint16;
		int32_t br_int32; bis >> br_int32;
		uint32_t br_uint32; bis >> br_uint32;
		int64_t br_int64; bis >> br_int64;
		uint64_t br_uint64; bis >> br_uint64;
		float br_float; bis >> br_float;
		double br_double; bis >> br_double;

		cout << "binary data block: " << endl;
		cout << (int)br_int8 << " " << (int)br_uint8 << " " << br_int16 << " " << br_uint16 << endl;
		cout << br_int32 << " " << br_uint32 << " " << br_int64 << " " << br_uint64 << endl;
		cout << br_float << " " << br_double << endl;

		bool b; bis >> b;
		string s; 
		bis >> make_pair(&s, bstr.size());
		cout << "bool: " << b << endl;
		cout << s << endl;

		vector<uint8_t> v;
		auto pv = make_pair(&v, (size_t)16);
		bis >> pv;
		cout << v;

		pv = make_pair(&v, (size_t)24);
		bis >> pv;
		cout << v;

		pv = make_pair(&v, vs.size());
		bis >> pv;
		cout << v;
	}


#pragma endregion

	system("pause");
	return 0;
}

