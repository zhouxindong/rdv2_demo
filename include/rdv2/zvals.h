#pragma once
#pragma once

#ifndef _RDV2_ZVALS_H_
#define _RDV2_ZVALS_H_

#define _RDV2_BEGIN namespace rdv2 {
#define _RDV2_END   }
#define _RDV2  ::rdv2::

/*
** defined class data member
** generated data set|get function automatic
** usage: name -> get_name|set_name (_name)
** readonly means no setter
** _INIT means use initialized value
*/
#pragma region auto_property

#define _DEF_PROPERTY_INIT(type, name, value) type _##name{ value };
#define _DEF_PROPERTY(type, name) type _##name;

#define _GET_PROPERTY(type, name) type get_##name() const {return _##name;}
#define _GET_PROPERTY_CONSTREF(type, name) const type & get_##name() const {return _##name;}
#define _GET_PROPERTY_REF(type, name) type & get_##name() const {return _##name;}

#define _SET_PROPERTY(type, name) void set_##name(const type & value){_##name = value;}

#define _PROPERTY(type, name) \
private:\
	_DEF_PROPERTY(type, name) \
public:\
	_GET_PROPERTY_CONSTREF(type, name) \
	_SET_PROPERTY(type, name)

#define _PROPERTY_INIT(type, name, value) \
private:\
	_DEF_PROPERTY_INIT(type, name, value) \
public:\
	_GET_PROPERTY_CONSTREF(type, name) \
	_SET_PROPERTY(type, name)

#define _PROPERTY_READONLY(type, name) \
private:\
	_DEF_PROPERTY(type, name) \
public:\
	_GET_PROPERTY_CONSTREF(type, name)

#define _PROPERTY_READONLY_INIT(type, name, value) \
private:\
	_DEF_PROPERTY_INIT(type, name, value) \
public:\
	_GET_PROPERTY_CONSTREF(type, name)

#pragma endregion

#define LOG(...) {\
	fprintf(stderr, "%s: Line %d:\t", __FILE__, __LINE__); \
	fprintf(stderr, __VA_ARGS__); \
	fprintf(stderr, "\n"); \
}

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the RDV_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// RDV_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifdef RDV2_EXPORTS
#define RDV2_API __declspec(dllexport)
#define RDV2_TEMPLATE_API __declspec(dllexport)
#else
#define RDV2_API __declspec(dllimport)
#define RDV2_TEMPLATE_API
#endif

#endif /* _RDV2_ZVALS_H_ */