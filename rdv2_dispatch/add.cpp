#include "stdafx.h"
#include "add.h"
#include <string>

using namespace std;
using namespace rdv2;

std::atomic_flag _SPIN_LOCK = ATOMIC_FLAG_INIT;

void add(rdv2::rdv_2 & rdv)
{
	rdv.add_dataset(ds_name);

	// 开始向ds_name数据集中添加数据
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
	ssa::xmValue xmVal(c);
	rdv.add_data(struct_da, ds_name, ssa::xmEDataType::xmDT_STRUCT, 0, &xmVal);

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

	// 结束向ds_name数据集添加数据，更新数据集
	rdv.end_update(ds_name);

	rdv.lock(ds_name); // 锁定数据集，使其不能再添加数据
}

void write_proc(rdv2::rdv_2* rdv)
{
	static thread_local int incr = 1;
	static thread_local double incr2 = 2.3532;
	static thread_local double beg = 8.232;

	while (_SPIN_LOCK.test_and_set(memory_order_acquire))
	{
		(*rdv).set_value(int8_da, (int8_t)++incr);
		(*rdv).set_value(uint8_da, (uint8_t)++incr);
		(*rdv).set_value(int16_da, (int16_t)++incr);
		(*rdv).set_value(uint16_da, (uint16_t)++incr);
		(*rdv).set_value(int32_da, ++incr);
		(*rdv).set_value(uint32_da, (uint32_t)++incr);
		(*rdv).set_value(int64_da, (int64_t)++incr);
		(*rdv).set_value(uint64_da, (uint64_t)++incr);

		beg += incr2;
		(*rdv).set_value(float_da, (float)beg);
		beg += incr2;
		(*rdv).set_value(double_da, beg);

		(*rdv).set_value(string_da, now());

		uint8_t buf3[16];
		for (int i = 0; i < 16; ++i)
		{
			buf3[i] = (uint8_t)++incr;
		}
		(*rdv).set_value(buffer_da, buf3, 16);

		ssa::xmValue xmVal;
		(*rdv).get_value(struct_da, xmVal);
		xmVal["p1"].i8() = (char)++incr;
		xmVal["p2"].i32() = ++incr;
		beg += incr2;
		xmVal["p3.sp1"].f32() = (float)beg;
		beg += incr2;
		xmVal["p3"]["sp2"].f64() = beg;
		xmVal["p4"].u32() = (uint32_t)++incr;
		xmVal["p5"].buffer().Set(0, xmB4(++incr, ++incr, ++incr, ++incr, ++incr,
			++incr, ++incr, ++incr, ++incr));
		(*rdv).set_value(struct_da, xmVal);

		bostream bos2;
		bos2 << (int8_t)++incr;
		bos2 << (uint8_t)++incr;
		bos2 << (int16_t)++incr;
		bos2 << (uint16_t)++incr;
		(*rdv).set_value(binary_da, bos2);

		Sleep(1000);
	}
}
