// rdv2_operate.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "rdv2.h"
#include <cassert>
#include <string>

using namespace rdv2;
using namespace std;

int main()
{
	rdv_2 rdv; // ʹ��ȱʡ��XML�����ļ�
	rdv.startup();

	string ds_name = "�״����ݼ�";
	string int8_da = "int8����";
	string uint8_da = "uint8����";
	string int16_da = "int16����";
	string uint16_da = "uint16����";
	string int32_da = "int32����";
	string uint32_da = "uint32����";
	string int64_da = "int64����";
	string uint64_da = "uint64����";
	string float_da = "float����";
	string double_da = "double����";
	string string_da = "string����";
	string buffer_da = "buffer����";
	string struct_da = "struct����";
	string binary_da = "binary����";

	ssa::xmAccessPoint* ap = rdv.add_dataset(ds_name);
	assert(ap != nullptr);

	// begin add data to dataset
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

	ssa::xmAccessPoint* ap2 = rdv.add_data(double_da, ds_name, ssa::xmEDataType::xmDT_DOUBLE);
	assert(nullptr == ap2);

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

	// �����������ݿ�, bos����������ָ���ֽ���ȱʡΪlittle-endian
	bostream bos;
	bos << 99;
	bos << (double)3.1415926;
	bos << false;
	bos << float(89.923);
	rdv.add_data(binary_da, ds_name, bos);

	// finish add data, and update dataset
	ssa::xmAccessPoint* ap11 = rdv.end_update(ds_name);
	assert(ap11 == ap);

	string node_name = "�״����ݽڵ�";
	ssa::xmAccessPoint* ap3 = rdv.add_node(node_name, ds_name);
	assert(ap3 != ap);

	string ds2_name = "�ڶ������ݼ�";
	rdv.add_dataset(ds2_name);
	ssa::xmAccessPoint* ap22 = rdv.link_node(node_name, ds2_name);
	assert(ap22 == ap3);

	ssa::xmAccessPoint* ap4 = rdv.attach(ds_name);
	assert(ap4 == ap);
	ssa::xmAccessPoint* ap5 = rdv.attach(buffer_da, false);
	assert(ap5 == ap);

	system("pause");
    return 0;
}

