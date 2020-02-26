// rdv_bstream.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rdv2.h"
#include <string>
#include <vector>
#include <cassert>
#include <iostream>

using namespace std;
using namespace rdv2;

int main()
{
	int8_t i8 = -100;
	uint8_t ui8 = 199;
	int16_t i16 = -1999;
	uint16_t ui16 = 32532;
	int32_t i32 = 9828;
	uint32_t ui32 = 2938234;
	int64_t i64 = -23342536;
	uint64_t ui64 = 3829582349283;
	float f32 = 2829832.323583f;
	double d64 = 38259238528239232.32342;
	bool btrue = true;
	string str("这是一个测试字符串. This is a test string");
	uint8_t buf[10]{ 0,1,2,3,4,5,6,7,8,9 };
	vector<uint8_t> v{ 11,22,33,44,55 };

	bostream bos(Endian::Big); // 指定大头字节序，缺省为小头字节序，Windows为小头字节序
	bos << i8 << ui8 << i16 << ui16 << i32 << ui32 << i64 << ui64 << f32 << d64;
	bos << btrue << str;

	// buf数组输出两遍，第一次使用两个指针，第二次使用指针和长度
	bos << make_pair(buf, buf + sizeof(buf)) << make_pair(buf, sizeof(buf));
	bos << v;
	 
	// 获取数据输出的二进制流
	pair<const uint8_t*, size_t> p = bos.buffer(); 

	// 使用相同字节序创建二进制流输入器
	// 可以使用3种方式创建流输入器
	// 1. 字节数组的首指针，尾后指针
	// 2. 字节数组的首指针，数组长度
	// 3. vector<uint8_t>对象
	bistream bis(p.first, p.first + p.second, Endian::Big);

	int8_t i8_r;
	uint8_t ui8_r;
	int16_t i16_r;
	uint16_t ui16_r;
	int32_t i32_r;
	uint32_t ui32_r;
	int64_t i64_r;
	uint64_t ui64_r;
	float f32_r;
	double d64_r;
	bool btrue_r;
	string str_r;
	std::vector<uint8_t> v_r;

	bis >> i8_r >> ui8_r >> i16_r >> ui16_r >> i32_r >> ui32_r >> i64_r >> ui64_r;
	bis >> f32_r >> d64_r;
	bis >> btrue_r;
	bis >> make_pair(&str_r, str.size());

	assert(i8 == i8_r);
	assert(ui8 == ui8_r);
	assert(i16 = i16_r);
	assert(ui16 == ui16_r);
	assert(i32 == i32_r);
	assert(ui32 == ui32_r);
	assert(i64 == i64_r);
	assert(ui64 == ui64_r);
	assert(f32 == f32_r);
	assert(d64 == d64_r);
	assert(btrue == btrue_r);
	assert(str == str_r);

	pair<std::vector<uint8_t>*, size_t> pv = make_pair(&v_r, sizeof(buf));
	bis >> pv; // 从输入流中读取指定数量的字节至v_r中
	assert(v_r.size() == sizeof(buf));
	for (int i = 0; i < v_r.size(); ++i)
	{
		assert(buf[i] == v_r[i]);
	}

	v_r.clear();
	pv = make_pair(&v_r, sizeof(buf));
	bis >> pv; // 输出了两遍数组，也输入两次
	assert(v_r.size() == sizeof(buf));
	for (int i = 0; i < v_r.size(); ++i)
	{
		assert(buf[i] == v_r[i]);
	}

	pv = make_pair(&v_r, v.size());
	bis >> pv;
	assert(v_r.size() == v.size());
	for (int i = 0; i < v.size(); ++i)
	{
		assert(v[i] == v_r[i]);
	}

	cout << "test successfull!!\n";
	system("pause");
    return 0;
}

