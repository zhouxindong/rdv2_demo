#pragma once

#ifndef _RDV2_RDV2CONF_H_
#define _RDV2_RDV2CONF_H_

#include <map>
#include <algorithm>
#include <string>
#include <sstream>

#include "zvals.h"
#include "any.h"
#include "tinyxml2_wrap.h"
#include "enum.h"
#include "inier.hpp"

_RDV2_BEGIN

/*
** configuration mode
*/
enum class ConfigMode {
	Inbuilt = 0,
	xml,
	ini
};

static const char* Label_Init_Value = "INIT";

BETTER_ENUM(ConfigItem, char,
	VDR_region = 1,
	VDR_id,
	VDR_group_ip,
	VDR_group_port,
	VDR_local_ip,
	VDR_local_port,

	cmd_name,
	cmd_speed_name,
	cmd_load,
	cmd_init,
	cmd_start,
	cmd_freeze,
	cmd_unfreeze,
	cmd_set_speed,
	cmd_finish,

	global_ini,

	confResHost,
	
	confManaIp,
	confManaPort,
	confManaRegion
)

/*
** base class for differents config resource
*/
class config_base {
public:
	class ini_config_open_error {};
	class ini_config_parse_error {
	public:
		int error_line_number;
		ini_config_parse_error(int line) : error_line_number(line) {}
	};

	template <typename _Tx>
	_Tx get(ConfigItem item) const {
		auto iter = _items.find(item);
		if (iter == _items.end())
			return _Tx();
		return iter->second.as<_Tx>();
	}

	template <typename _Tx>
	config_base& set(ConfigItem item, const _Tx& val) {		
		_items[item] = rdv2::any(val);
		return *this;
	}
	
	virtual ~config_base() {}

protected:
	std::map<ConfigItem, rdv2::any> _items;

	const std::string vdrAttrSection	= "vdrAttr";
	const std::string resSrvAttr		= "resSrvAttr";

	const std::string confVdrId			= "confVdrId";
	const std::string confSimuIp		= "confSimuIp";
	const std::string confSimuPort		= "confSimuPort";
	const std::string confSimuRegion	= "confSimuRegion";
	const std::string confManaIp		= "confManaIp";
	const std::string confManaPort		= "confManaPort";
	const std::string confManaRegion	= "confManaRegion";
	const std::string confResHost		= "confResHost";

	template <class _Tx>
	void __set_integer(ConfigItem item, const INIReader& reader, std::string section)
	{
		set(item, (_Tx)reader.GetInteger(section, item._to_string(), 0));
	}

	void __set_string(ConfigItem item, const INIReader& reader, std::string section)
	{
		set(item, reader.Get(section, item._to_string(), ""));
	}

	template <class _Tx>
	void __set_integer(ConfigItem item, const INIReader& reader, std::string section, std::string name)
	{
		set(item, (_Tx)reader.GetInteger(section, name, 0));
	}

	void __set_string(ConfigItem item, const INIReader& reader, std::string section, std::string name)
	{
		set(item, reader.Get(section, name, ""));
	}

	void _parse_global(std::string file)
	{
		INIReader inier(file);
		int parsed_code = inier.ParseError();
		if (parsed_code == -1)
			throw ini_config_open_error();
		else if (parsed_code > 0)
			throw ini_config_parse_error(parsed_code);

		__set_integer<uint16_t>(ConfigItem::VDR_id, inier, vdrAttrSection, confVdrId);
		__set_string(ConfigItem::VDR_group_ip, inier, vdrAttrSection, confSimuIp);
		__set_integer<uint16_t>(ConfigItem::VDR_group_port, inier, vdrAttrSection, confSimuPort);
		__set_integer<uint16_t>(ConfigItem::VDR_region, inier, vdrAttrSection, confSimuRegion);
		__set_string(ConfigItem::confManaIp, inier, vdrAttrSection, confManaIp);
		__set_integer<uint16_t>(ConfigItem::confManaPort, inier, vdrAttrSection, confManaPort);
		__set_integer<uint16_t>(ConfigItem::confManaRegion, inier, vdrAttrSection, confManaRegion);
		__set_string(ConfigItem::confResHost, inier, resSrvAttr, confResHost);
	}
};

/*
** Inbuilt_config 
*/
template <int N1 = 224, int N2 = 0, int N3 = 0, int N4 = 155>
class inbuilt_config : public config_base {
public:
	inbuilt_config() {
		std::ostringstream oss;
		oss << N1 << '.' << N2 << '.' << N3 << '.' << N4;

		set(ConfigItem::VDR_region,		(uint16_t)174);
		set(ConfigItem::VDR_id,			(uint16_t)1811);
		set(ConfigItem::VDR_group_ip,	oss.str());
		set(ConfigItem::VDR_group_port, (uint16_t)9000);
		set(ConfigItem::VDR_local_ip,	std::string("localhost"));
		set(ConfigItem::VDR_local_port, (uint16_t)9078);

		set(ConfigItem::cmd_name,		std::string("运行控制指令"));
		set(ConfigItem::cmd_speed_name, std::string("仿真步长"));
		set(ConfigItem::cmd_load,		(uint16_t)800);
		set(ConfigItem::cmd_init,		(uint16_t)1000);
		set(ConfigItem::cmd_start,		(uint16_t)1100);
		set(ConfigItem::cmd_freeze,		(uint16_t)1200);
		set(ConfigItem::cmd_unfreeze,	(uint16_t)1300);
		set(ConfigItem::cmd_set_speed,	(uint16_t)1400);
		set(ConfigItem::cmd_finish,		(uint16_t)2000);

		set(ConfigItem::confResHost,	std::string("192.168.0.10:3001"));
		set(ConfigItem::confManaIp,		std::string("224.0.0.109"));
		set(ConfigItem::confManaPort,	(uint16_t)9009);
		set(ConfigItem::confManaRegion, (uint16_t)179);
	}
};

/*
** config from xml file
*/
class xml_config : public config_base {

	using element = tinyxml2::XMLElement;

public:
	class xml_config_load_error {};
	xml_config(const char* file = "rdv2_config.xml") : _tiw(file) {	
		if (!_tiw.get_ready()) throw xml_config_load_error();

		std::vector<element*> ids = _tiw.all("ID");
		for_each(ids.cbegin(), ids.cend(),
			[&](const element* ele) {
			__parse_element(ele);
		});
	}

private:
	void __parse_element(const element* ele) {
		using namespace better_enums;
		auto item = ConfigItem::_from_string_nocase_nothrow(ele->GetText());
		if (!item.value())
			return;

		switch (item.value()) {
		case ConfigItem::VDR_region:
		case ConfigItem::VDR_id:
		case ConfigItem::VDR_group_port:
		case ConfigItem::VDR_local_port:
		case ConfigItem::cmd_load:
		case ConfigItem::cmd_init:
		case ConfigItem::cmd_start:
		case ConfigItem::cmd_freeze:
		case ConfigItem::cmd_unfreeze:
		case ConfigItem::cmd_set_speed:
		case ConfigItem::cmd_finish:
			set(item.value(), (uint16_t)ele->IntAttribute(Label_Init_Value));
			break;

		case ConfigItem::VDR_group_ip:
		case ConfigItem::VDR_local_ip:
		case ConfigItem::cmd_name:
		case ConfigItem::cmd_speed_name:
			set(item.value(), std::string(ele->Attribute(Label_Init_Value)));
			break;

		case ConfigItem::global_ini:
			_parse_global(std::string(ele->Attribute(Label_Init_Value)));
			break;
		}
	}

private:
	tinyxml2_wrap _tiw;
};

/*
** config from ini file
*/
class ini_config : public config_base {
public:

	
	ini_config(const char* file = "rdv2_config.ini") {
		INIReader inier(file);
		int parsed_code = inier.ParseError();
		if (parsed_code == -1)
			throw ini_config_open_error();
		else if (parsed_code > 0)
			throw ini_config_parse_error(parsed_code);
		__parse_ini(inier);
	}

private:
	void __parse_ini(const INIReader& reader) {
		std::string vdr_section = "VDR";
		std::string cmd_section = "cmd";
		std::string global_section = "global";

		__set_integer<uint16_t>(ConfigItem::VDR_region, reader, vdr_section);
		__set_integer<uint16_t>(ConfigItem::VDR_id, reader, vdr_section);
		__set_string(ConfigItem::VDR_group_ip, reader, vdr_section);
		__set_integer<uint16_t>(ConfigItem::VDR_group_port, reader, vdr_section);
		__set_string(ConfigItem::VDR_local_ip, reader, vdr_section);
		__set_integer<uint16_t>(ConfigItem::VDR_local_port, reader, vdr_section);

		__set_string(ConfigItem::cmd_name, reader, cmd_section);
		__set_string(ConfigItem::cmd_speed_name, reader, cmd_section);
		__set_integer<uint16_t>(ConfigItem::cmd_load, reader, cmd_section);
		__set_integer<uint16_t>(ConfigItem::cmd_init, reader, cmd_section);
		__set_integer<uint16_t>(ConfigItem::cmd_start, reader, cmd_section);
		__set_integer<uint16_t>(ConfigItem::cmd_freeze, reader, cmd_section);
		__set_integer<uint16_t>(ConfigItem::cmd_unfreeze, reader, cmd_section);
		__set_integer<uint16_t>(ConfigItem::cmd_set_speed, reader, cmd_section);
		__set_integer<uint16_t>(ConfigItem::cmd_finish, reader, cmd_section);

		ConfigItem g = ConfigItem::global_ini;
		std::string global_ini = reader.Get(global_section, g._to_string(), "");
		if (global_ini == std::string())
			return;
		_parse_global(global_ini);
	}
};

template <int N1 = 224, int N2 = 0, int N3 = 0, int N4 = 155>
config_base* create_config(ConfigMode mode, const char* file) {
	switch (mode) {
	case ConfigMode::Inbuilt:
		return new inbuilt_config<N1, N2, N3, N4>();

	case ConfigMode::xml:
		return new xml_config(file);

	case ConfigMode::ini:
		return new ini_config(file);

	default:
		return nullptr;
	}
}

_RDV2_END

#endif /* _RDV2_RDV2CONF_H_ */