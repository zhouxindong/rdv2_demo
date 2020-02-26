#pragma once
#ifndef _RDV2_BIN2STREAM_H_
#define _RDV2_BIN2STREAM_H_

#include "zvals.h"
#include <vector>
//#include <type_traits>
#include <queue>
#include <string>

#ifdef RDV2_EXPORTS
#define RDV2_API __declspec(dllexport)
#else
#define RDV2_API __declspec(dllimport)
#endif

_RDV2_BEGIN

#pragma warning(disable:4251)

RDV2_API bool is_bigendian();

enum class RDV2_API Endian {
	Little,
	Big
};

class RDV2_API bostream;

RDV2_API bostream& operator << (bostream& bos, const int8_t& d);
RDV2_API bostream& operator << (bostream& bos, const uint8_t& d);
RDV2_API bostream& operator << (bostream& bos, const int16_t& d);
RDV2_API bostream& operator << (bostream& bos, const uint16_t& d);
RDV2_API bostream& operator << (bostream& bos, const int32_t& d);
RDV2_API bostream& operator << (bostream& bos, const uint32_t& d);
RDV2_API bostream& operator << (bostream& bos, const int64_t& d);
RDV2_API bostream& operator << (bostream& bos, const uint64_t& d);
RDV2_API bostream& operator << (bostream& bos, const float& d);
RDV2_API bostream& operator << (bostream& bos, const double& d);
RDV2_API bostream& operator << (bostream& bos, std::pair<const uint8_t*, const uint8_t*> iter);
RDV2_API bostream& operator << (bostream& bos, std::pair<const uint8_t*, size_t> ar);
RDV2_API bostream& operator << (bostream& bos, const std::vector<uint8_t>&);
RDV2_API bostream& operator << (bostream& bos, const bool& b);
RDV2_API bostream& operator << (bostream& bos, const std::string&);

class RDV2_API bostream {

	friend RDV2_API bostream& operator << (bostream& bos, const int8_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const uint8_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const int16_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const uint16_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const int32_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const uint32_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const int64_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const uint64_t& d);
	friend RDV2_API bostream& operator << (bostream& bos, const float& d);
	friend RDV2_API bostream& operator << (bostream& bos, const double& d);
	friend RDV2_API bostream& operator << (bostream& bos, std::pair<const uint8_t*, const uint8_t*> iter);
	friend RDV2_API bostream& operator << (bostream& bos, std::pair<const uint8_t*, size_t> ar);
	friend RDV2_API bostream& operator << (bostream& bos, const std::vector<uint8_t>&);
	friend RDV2_API bostream& operator << (bostream& bos, const bool& b);
	friend RDV2_API bostream& operator << (bostream& bos, const std::string&);


public:
	bostream(Endian endian = Endian::Little);

	bostream(const bostream& rhs);
	bostream(bostream&& rhs);
	bostream& operator = (const bostream& rhs);
	bostream& operator = (bostream&& rhs);

	std::pair<const uint8_t*, size_t> buffer() const;

private:
	std::vector<uint8_t> _buf;
	Endian _endian;
};

class RDV2_API bistream {
	friend RDV2_API bistream& operator >> (bistream& bis, int8_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, uint8_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, int16_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, uint16_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, int32_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, uint32_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, int64_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, uint64_t& d);
	friend RDV2_API bistream& operator >> (bistream& bis, float& d);
	friend RDV2_API bistream& operator >> (bistream& bis, double& d);
	friend RDV2_API bistream& operator >> (bistream& bis, std::pair<std::vector<uint8_t>*, size_t>);
	friend RDV2_API bistream& operator >> (bistream& bis, bool& b);
	friend RDV2_API bistream& operator >> (bistream& bis, std::pair<std::string*, size_t>);

public:
	bistream(Endian endian = Endian::Little);
	bistream(const uint8_t* beg, const uint8_t* end, Endian endian = Endian::Little);
	bistream(const uint8_t* beg, size_t size, Endian endian = Endian::Little);
	bistream(const std::vector<uint8_t>& v, Endian endian = Endian::Little);

	bistream(const bistream&);
	bistream(bistream&&);
	bistream& operator = (const bistream&);
	bistream& operator = (bistream&&);

	operator bool () const;
	bool can_read(size_t) const;
	Endian& endian() { return _endian; }
private:
	std::queue<uint8_t> _buf;
	Endian _endian;
	//bool _valid{ true };
};

RDV2_API bistream& operator >> (bistream& bis, int8_t& d);
RDV2_API bistream& operator >> (bistream& bis, uint8_t& d);
RDV2_API bistream& operator >> (bistream& bis, int16_t& d);
RDV2_API bistream& operator >> (bistream& bis, uint16_t& d);
RDV2_API bistream& operator >> (bistream& bis, int32_t& d);
RDV2_API bistream& operator >> (bistream& bis, uint32_t& d);
RDV2_API bistream& operator >> (bistream& bis, int64_t& d);
RDV2_API bistream& operator >> (bistream& bis, uint64_t& d);
RDV2_API bistream& operator >> (bistream& bis, float& d);
RDV2_API bistream& operator >> (bistream& bis, double& d);
RDV2_API bistream& operator >> (bistream& bis, std::pair<std::vector<uint8_t>*, size_t>);
RDV2_API bistream& operator >> (bistream& bis, bool& b);
RDV2_API bistream& operator >> (bistream& bis, std::pair<std::string*, size_t>);

_RDV2_END

#endif /* _RDV2_BIN2STREAM_H_ */


