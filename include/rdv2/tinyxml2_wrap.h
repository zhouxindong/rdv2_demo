#pragma once
#ifndef _RDV2_TINYXML2_WRAP_H_
#define _RDV2_TINYXML2_WRAP_H_

#include "zvals.h"
#include "tinyxml2.h"
#include <vector>

_RDV2_BEGIN

using element = tinyxml2::XMLElement;

class tinyxml2_wrap {

	_PROPERTY_READONLY(bool, ready)

public:
	tinyxml2_wrap() {}

	tinyxml2_wrap(const char* file) {
		auto load = _doc.LoadFile(file);
		_ready = (load == tinyxml2::XML_SUCCESS);
	}

	tinyxml2::XMLError parse(const char* xml) {
		auto parsed = _doc.Parse(xml);
		_ready = (parsed == tinyxml2::XML_SUCCESS);
		return parsed;
	}

	std::vector<element*> all(const char* name) {
		if (!_ready) return std::vector<element*>();
		_vec.clear();
		__all(name, _doc.RootElement());
		return _vec;
	}

	std::vector<element*> all(const char* name, const char* parent)
	{
		if (!_ready) return std::vector<element*>();
		_vec.clear();
		__all(name, _doc.RootElement(), parent);
		return _vec;
	}

	std::vector<element*> all(const char* name, const char* parent, const char* pparent)
	{
		if (!_ready) return std::vector<element*>();
		_vec.clear();
		__all(name, _doc.RootElement(), parent, pparent);
		return _vec;
	}

	element* root()
	{
		if (!get_ready()) return nullptr;
		return _doc.RootElement();
	}

private:
	void __all(const char* name, element* ele)
	{
		if (::strcmp(name, ele->Name()) == 0) {
			_vec.push_back(ele);
		}
		if (ele->FirstChildElement()) {
			__all(name, ele->FirstChildElement());
		}
		if (ele->NextSiblingElement()) {
			__all(name, ele->NextSiblingElement());
		}
	}

	void __all(const char* name, element* ele, const char* parent) {
		if (::strcmp(name, ele->Name()) == 0) {
			if(ele->Parent() && 
				(::strcmp(parent, ele->Parent()->ToElement()->Name()) == 0))
			_vec.push_back(ele);
		}
		if (ele->FirstChildElement()) {
			__all(name, ele->FirstChildElement(), parent);
		}
		if (ele->NextSiblingElement()) {
			__all(name, ele->NextSiblingElement(), parent);
		}
	}

	void __all(const char* name, element* ele, const char* parent, const char* pparent)
	{
		if (::strcmp(name, ele->Name()) == 0) {
			if (ele->Parent() &&
				(::strcmp(parent, ele->Parent()->ToElement()->Name()) == 0))
				if(ele->Parent()->Parent() &&
					(::strcmp(pparent, ele->Parent()->Parent()->ToElement()->Name()) == 0))
					_vec.push_back(ele);
		}
		if (ele->FirstChildElement()) {
			__all(name, ele->FirstChildElement(), parent, pparent);
		}
		if (ele->NextSiblingElement()) {
			__all(name, ele->NextSiblingElement(), parent, pparent);
		}
	}

private:
	tinyxml2::XMLDocument _doc;
	std::vector<element*> _vec;
};

inline std::vector<element*> childs(element* ele, const char* name = (const char*)0)
{
	std::vector<element*> v;
	if (ele->NoChildren())
		return v;

	element* n = ele->FirstChildElement(name);
	while (n)
	{
		v.push_back(n);
		n = n->NextSiblingElement(name);
	}
	return v;
}

_RDV2_END

#endif /* _RDV2_TINYXML2_WRAP_H_ */
