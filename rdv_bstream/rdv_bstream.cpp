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
	string str("����һ�������ַ���. This is a test string");
	uint8_t buf[10]{ 0,1,2,3,4,5,6,7,8,9 };
	vector<uint8_t> v{ 11,22,33,44,55 };

	bostream bos(Endian::Big); // ָ����ͷ�ֽ���ȱʡΪСͷ�ֽ���WindowsΪСͷ�ֽ���
	bos << i8 << ui8 << i16 << ui16 << i32 << ui32 << i64 << ui64 << f32 << d64;
	bos << btrue << str;

	// buf����������飬��һ��ʹ������ָ�룬�ڶ���ʹ��ָ��ͳ���
	bos << make_pair(buf, buf + sizeof(buf)) << make_pair(buf, sizeof(buf));
	bos << v;
	 
	// ��ȡ��������Ķ�������
	pair<const uint8_t*, size_t> p = bos.buffer(); 

	// ʹ����ͬ�ֽ��򴴽���������������
	// ����ʹ��3�ַ�ʽ������������
	// 1. �ֽ��������ָ�룬β��ָ��
	// 2. �ֽ��������ָ�룬���鳤��
	// 3. vector<uint8_t>����
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
	bis >> pv; // ���������ж�ȡָ���������ֽ���v_r��
	assert(v_r.size() == sizeof(buf));
	for (int i = 0; i < v_r.size(); ++i)
	{
		assert(buf[i] == v_r[i]);
	}

	v_r.clear();
	pv = make_pair(&v_r, sizeof(buf));
	bis >> pv; // ������������飬Ҳ��������
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

