#pragma once
#ifndef _RDV2_ANY_H_
#define _RDV2_ANY_H_

#if __cplusplus >= 201703
#pragma message("When building projects with support for C++17 or higher, please use std::any")
#endif

#include <type_traits>
#include <typeinfo>

#include "zvals.h"

_RDV2_BEGIN

class any {
public:

	any() : _value(nullptr) {
	}

	template<typename T> any(T& value)
		: _value(new erasure_impl<typename std::decay<T&>::type>(value)) {
	}

	any(any const& value) : _value(value._value->clone()) {
	}

	//any(any& value) : _value(value._value->clone()) {
	//}

	template<typename T> any(T const& value)
		: _value(new erasure_impl<typename std::decay<T const&>::type>(value)) {
	}

	template<typename T> any(T* value)
		: _value(new erasure_impl<typename std::decay<T*>::type>(value)) {
	}

	template<typename T> any(T const* value)
		: _value(new erasure_impl<typename std::decay<T const*>::type>(value)) {
	}

	~any() {
		if (_value) {
			delete _value;
		}
	}

	template<typename T> any& operator=(T& value) {
		if (_value) {
			delete _value;
		}

		_value = new erasure_impl<typename std::decay<T&>::type>(value);
		return *this;
	}

	template<typename T> any& operator=(T const& value) {
		if (_value) {
			delete _value;
		}

		_value = new erasure_impl<typename std::decay<T const&>::type>(value);
		return *this;
	}

	template<typename T> any& operator=(T* value) {
		if (_value) {
			delete _value;
		}

		_value = new erasure_impl<typename std::decay<T*>::type>(value);
		return *this;
	}

	template<typename T> any& operator=(T const* value) {
		if (_value) {
			delete _value;
		}

		_value = new erasure_impl<typename std::decay<T const*>::type>(value);
		return *this;
	}

	any& operator=(any const& value) {
		if (_value) {
			delete _value;
		}

		_value = value._value->clone();
		return *this;
	}

	template<typename T> T& as() const {
		if (_value) return static_cast<erasure_impl<T>*>(_value)->value();
		throw std::bad_cast();
	}

	std::type_info const& type() const {
		if (_value) return _value->type();
		return typeid(nullptr);
	}

private:

	struct erasure {
		virtual ~erasure() {}
		virtual erasure* clone() = 0;
		virtual std::type_info const& type() const = 0;
	};

	template<typename T> class erasure_impl : public erasure {
	public:
		erasure_impl(T value) : _value(value) { }
		virtual erasure* clone() { return new erasure_impl<T>(_value); }
		virtual std::type_info const& type() const { return typeid(T); }
		T& value() { return _value; }
	private:
		T _value;
	};

	erasure* _value;
};

_RDV2_END

#endif /* _RDV2_ANY_H_ */