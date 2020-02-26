// rdv2_dispatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "add.h"
#include <iostream>

using namespace rdv2;
using namespace std;

class dispatch_rdv : public rdv_2
{
public:
	void on_data_update(const ssa::xmMSGDataEvent& d) override
	{
		if (d.m_strDataName == int8_da)
		{
			int8_t val = *d.m_bbValue.Data();
			cout << "int8: " << (int)val << endl;
		}
		else if (d.m_strDataName == uint8_da)
		{
			uint8_t val = *d.m_bbValue.Data();
			cout << "uint8: " << (int)val << endl;
		}
		else if (d.m_strDataName == int16_da)
		{
			int16_t val = *(int16_t*)d.m_bbValue.Data();
			cout << "int16: " << val << endl;
		}
		else if (d.m_strDataName == uint16_da)
		{
			uint16_t val = *(uint16_t*)d.m_bbValue.Data();
			cout << "uint16: " << val << endl;
		}
		else if (d.m_strDataName == int32_da)
		{
			int32_t val = *(int32_t*)d.m_bbValue.Data();
			cout << "int32: " << val << endl;
		}
		else if (d.m_strDataName == uint32_da)
		{
			uint32_t val = *(uint32_t*)d.m_bbValue.Data();
			cout << "uint32: " << val << endl;
		}
		else if (d.m_strDataName == int64_da)
		{
			int64_t val = *(int64_t*)d.m_bbValue.Data();
			cout << "int64: " << val << endl;
		}
		else if (d.m_strDataName == uint64_da)
		{
			uint64_t val = *(uint64_t*)d.m_bbValue.Data();
			cout << "uint64: " << val << endl;
		}
		else if (d.m_strDataName == float_da)
		{
			float val = *(float*)d.m_bbValue.Data();
			cout << "float: " << val << endl;
		}
		else if (d.m_strDataName == double_da)
		{
			double val = *(double*)d.m_bbValue.Data();
			cout << "double: " << val << endl;
		}
		else if (d.m_strDataName == string_da)
		{
			string val = d.m_bbValue.Data();
			cout << val << endl;
		}
		else if (d.m_strDataName == buffer_da)
		{
			int size = d.m_bbValue.Size();
			uint8_t* val = (uint8_t*)d.m_bbValue.Data();
			for (int i = 0; i < size; ++i)
			{
				cout << (int)val[i] << " ";
			}
			cout << endl;
		}
		else if (d.m_strDataName == struct_da)
		{
			ssa::xmValue val = d.m_Value;
			cout << "p1=" << (int)val["p1"].i8() << ", " << "p2=" << val["p2"].i32() << endl;
			cout << "p3.sp1=" << val["p3"]["sp1"].f32() << "  p3.sp2=" << val["p3.sp2"].f64() << endl;
			cout << "p4=" << val["p4"].u32() << endl;

			auto val_buf = val["p5"].buffer();
			cout << val_buf;
		}
		else if (d.m_strDataName == binary_da)
		{
			uint8_t* data = (uint8_t*)d.m_bbValue.Data();
			bistream bis(data, d.m_bbValue.Size());
			int8_t br_int8; bis >> br_int8;
			uint8_t br_uint8; bis >> br_uint8;
			int16_t br_int16; bis >> br_int16;
			uint16_t br_uint16; bis >> br_uint16;
			cout << "binary block: " << (int)br_int8 << " " << (int)br_uint8 << " " << br_int16 << " " << br_uint16 << endl;
			// 后面不变的数据就不一一读取了
		}
	}
};

int main()
{
	dispatch_rdv rdv;
	rdv.startup();

	add(rdv); // add dataset & data
	rdv.dispatch({ int8_da, uint8_da, int16_da, uint16_da, int32_da, uint32_da,
	int64_da, uint64_da, float_da, double_da, string_da, buffer_da, struct_da, binary_da });
	Sleep(500);

	_SPIN_LOCK.test_and_set();
	thread t_w(write_proc, &rdv); // start set_value thread

	cout << "Press any key to quit!" << endl;
	cin.get();

	_SPIN_LOCK.clear();
	t_w.join();

	return 0;
}

