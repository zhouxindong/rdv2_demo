#pragma once

#include "rdv2.h"
#include <string>
#include <thread>
#include <atomic>
#include <chrono>
#include <sstream>
#include <iomanip>

const std::string ds_name = "�״����ݼ�";
const std::string int8_da = "int8����";
const std::string uint8_da = "uint8����";
const std::string int16_da = "int16����";
const std::string uint16_da = "uint16����";
const std::string int32_da = "int32����";
const std::string uint32_da = "uint32����";
const std::string int64_da = "int64����";
const std::string uint64_da = "uint64����";
const std::string float_da = "float����";
const std::string double_da = "double����";
const std::string string_da = "string����";
const std::string buffer_da = "buffer����";
const std::string struct_da = "struct����";
const std::string binary_da = "binary����";

const std::string bstr("this is a string. ����һ���ַ���!");

void add(rdv2::rdv_2& rdv);

extern std::atomic_flag _SPIN_LOCK;

void write_proc(rdv2::rdv_2* rdv);

inline std::tuple<int, int, int, int, int, int, int>
time_ms(const std::chrono::system_clock::time_point& tp) {
	auto t = std::chrono::system_clock::to_time_t(tp);
	auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(total_ms % std::chrono::seconds(1));
	tm ptm;
	localtime_s(&ptm, &t);
	//tm* ptm = localtime(&t);
	return std::make_tuple(
		ptm.tm_year + 1900,
		ptm.tm_mon + 1,
		ptm.tm_mday,
		ptm.tm_hour,
		ptm.tm_min,
		ptm.tm_sec,
		(int)ms.count()
	);
}

inline std::string now()
{
	auto t = time_ms(std::chrono::system_clock::now());
	std::ostringstream oss;
	oss << std::get<0>(t) << "-";
	oss << std::setw(2) << std::setfill('0') << std::get<1>(t) << '-';
	oss << std::setw(2) << std::setfill('0') << std::get<2>(t) << ' ';
	oss << std::setw(2) << std::setfill('0') << std::get<3>(t) << ':';
	oss << std::setw(2) << std::setfill('0') << std::get<4>(t) << ':';
	oss << std::setw(2) << std::setfill('0') << std::get<5>(t) << '.';
	oss << std::setw(3) << std::setfill('0') << std::get<6>(t) << ' ';

	return oss.str();
}


