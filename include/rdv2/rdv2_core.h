#pragma once
#ifndef _RDV2_RDV2CORE_H_
#define _RDV2_RDV2CORE_H_

#ifdef _DEBUG
#pragma comment(lib, "based.lib")
#pragma comment(lib, "vdrd.lib")
#else 
#pragma comment(lib, "base.lib")
#pragma comment(lib, "vdr.lib")
#endif

#ifdef RDV2_EXPORTS
#define RDV2_API __declspec(dllexport)
#else
#define RDV2_API __declspec(dllimport)
#ifdef _DEBUG
#pragma comment(lib, "rdv2d.lib")
#else 
#pragma comment(lib, "rdv2.lib")
#endif
#endif


#include "zvals.h"
#include "base.h"
#include "vdr.h"
#include "accesspoint.h"
#include "enum.h"
#include "any.h"
#include <vector>
#include <sstream>
#include <initializer_list>
#include <functional>
#include <ostream>
#include <string>
#include <type_traits>
#include <algorithm>
#include "tinyxml2_wrap.h"
#include <memory>
#include <map>

_RDV2_BEGIN



#pragma region rdv_exceptions

/*
** exceptions rdv
*/
class rdv2_exception
{
public:
	rdv2_exception(const std::string msg = std::string("")) : _msg(msg) {}
	const std::string message() const { return _msg; }
private:
	std::string _msg;
};

class vdr_created_fail		: public rdv2_exception {};
class dataset_not_found		: public rdv2_exception {};
class dataset_locked		: public rdv2_exception {};
class data_not_found		: public rdv2_exception {};
class data_redupli			: public rdv2_exception {};
class node_redupli			: public rdv2_exception {};
class datatype_unsupported	: public rdv2_exception {};
class node_not_found		: public rdv2_exception {};
class access_point_redupli	: public rdv2_exception {};

class bad_operation			: public rdv2_exception
{
	using rdv2_exception::rdv2_exception;
};

#pragma endregion

#pragma region xmClass wrap 

BETTER_ENUM(xmClassType, char,
	INT8$ = 1,
	UINT8$,
	INT16$,
	UINT16$,
	INT32$,
	UINT32$,
	INT64$,
	UINT64$,
	FLOAT$,
	DOUBLE$
)

/*
** loki: some useful functions
*/
struct RDV2_API loki_rdv2 {
	static std::string trim_dollar(xmClassType ct);
	static std::vector<std::string> split(
		const std::string& str, const std::string& delim);
};

/*
** easy for ssa::xmClass
** Design Mode: Composite
*/

class RDV2_API Type_component {
protected:
	Type_component(const std::string& name) : _name(name) {}

public:
	virtual ~Type_component() {}

	virtual Type_component* clone() const = 0;
	virtual Type_component& add(Type_component*) = 0;
	virtual std::string to_string() const = 0;
	ssa::xmClass xm_class();

protected:
	std::string _name;
};

class RDV2_API Simple_type : public Type_component{
public:
	Simple_type(const std::string& name, xmClassType ct)
		: Type_component(name), _ct(ct) {}
	Simple_type(const Simple_type&) = default;

	Type_component* clone() const override {
		return new Simple_type(*this);
	}
	Type_component& add(Type_component*) override { return *this; }

	std::string to_string() const override;

private:
	xmClassType _ct;
};

class RDV2_API Buffer_type : public Type_component {
public:
	Buffer_type(const std::string& name, size_t size)
		: Type_component(name), _size(size) {}
	Buffer_type(const Buffer_type&) = default;

	Type_component* clone() const override {
		return new Buffer_type(*this);
	}

	Type_component& add(Type_component*) override { return *this; }

	std::string to_string() const override;

private:
	size_t _size;
};

class RDV2_API Compound_type : public Type_component {
public:
	Compound_type(std::initializer_list<Type_component*> l = {})
		: Type_component("ROOT") {
		for (Type_component* item : l) {
			_items.push_back(item);
		}
	}

	Compound_type(const std::string& name, std::initializer_list<Type_component*> l = {})
		: Type_component(name)
	{
		for (Type_component* item : l) {
			_items.push_back(item);
		}
	}

	Type_component* clone() const override;

	Type_component& add(Type_component* t) override {
		_items.push_back(t->clone());
		return *this;
	}

	std::string to_string() const override;

	~Compound_type();

private:
	std::vector<Type_component*> _items;
};

#pragma endregion

#pragma region GetValueType

enum class GetValueType : byte
{
	Numeric = 0,
	String,
	Buffer,
	Struct,
	BinStream
};

#pragma endregion

#pragma region valuer

/*
** wrap the callback when watched data updated
*/
class RDV2_API valuer
{
public:
	valuer(std::function<void(any&)> functor =
		[](any&) {})
		: _functor(functor) {}

	void call(any& a) {
		_functor(a);
	}
private:
	std::function<void(any&)> _functor;
};

#pragma endregion

#pragma region extra ssa::xmEDataType definition

static const ssa::xmEDataType xmDT_BSTREAM = ssa::xmEDataType(ssa::xmEDataType::xmDT_MAX + 200);

#pragma endregion

#pragma region ostream_ex

template <typename _Tx>
struct ostream_disval
{
	typedef typename std::conditional<
		std::is_same<_Tx, int8_t>::value || std::is_same<_Tx, uint8_t>::value,
		int32_t,
		_Tx>::type type;
};

#ifdef _RDV2_PRIVATE
/*
conflict with STL
*/
template <typename _Tx, int n>
std::ostream& operator << (std::ostream& o, _Tx(&a)[n])
{
	using type = ostream_disval<_Tx>::type;
	o << std::string("size: ") << n;
	for (int i = 0; i < n; ++i)
	{
		if (i % 16 == 0)
			o << std::endl;

		o << (type)a[i] << std::string(" ");
	}
	o << std::endl;
	return o;
}

#endif

inline std::ostream& operator << (std::ostream& o, ssa::xmStaticBuffer& buf)
{
	size_t size = buf.Size();
	o << std::string("size: ") << size;
	uint8_t val;
	for (size_t i = 0; i < size; ++i) 
	{
		if (i % 16 == 0) o << std::endl;
		buf.Get<uint8_t>(i, val);
		o << (int)val << std::string(" ");
	}
	o << std::endl;
	return o;
}

template <
	typename _Tx,
	template <typename _Tx, typename Allocator = std::allocator<_Tx>> class _Cont>
std::ostream& operator << (std::ostream& o, const _Cont<_Tx>& c)
{
	using type = ostream_disval<_Tx>::type;
	typename _Cont<_Tx>::size_type count = 0;
	o << std::string("size: ") << c.size();
	std::for_each(c.cbegin(), c.cend(),
		[&](auto& item) {
		if (count++ % 16 == 0)
			o << std::endl;
		o << (type)item << std::string(" ");
	});
	o << std::endl;
	return o;
}

#pragma endregion

#pragma region Control Command

enum class CtrlCmd : byte
{
	Load,
	Init,
	Start,
	Freeze,
	Unfreeze,
	Speed,
	Finish
};

#pragma endregion

_RDV2_END

#endif /* _RDV2_RDV2CORE_H_ */

