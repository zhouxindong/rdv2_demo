#pragma once
#ifndef _RDV2_RDV2_H
#define _RDV2_RDV2_H

#include "rdv2_conf.h"
#include "rdv2_core.h"
#include "bin2stream.h"

#include <set>
#include <map>
#include <tuple>
#include <stack>

#ifdef RDV2_EXPORTS
#define RDV2_API __declspec(dllexport)
#else
#define RDV2_API __declspec(dllimport)
#endif

_RDV2_BEGIN

class rdv_2;

#pragma region parse user xml or config file

static const char* INPUT_DATA_LABEL		= "INPUT_DATA";
static const char* INNER_DATA_LABEL		= "INNER_DATA";
static const char* OUTPUT_DATA_LABEL	= "OUTPUT_DATA";
static const char* DATA_LABEL			= "DATA";
static const char* DATA_NAME_LABEL		= "GSA_NAME";
static const char* DATA_LOC_NAME_LABEL	= "LOCAL_NAME";
static const char* DATA_TYPE_LABEL		= "TYPE";
static const char* DATA_INIT_LABEL		= "INIT";

static const char* PROPERTY_LABEL		= "PROPERTY";
static const char* PROPERTY_TYPE_LABEL	= "TYPE";
static const char* PROPERTY_NAME_LABEL	= "NAME";
static const char* PROPERTY_INIT_LABEL	= "INIT";

static const char* CLASS_DECLARE_LABEL	= "CLASS_DECLARE";
static const char* CLASS_LABEL			= "CLASS";
static const char* CLASS_NAME_LABEL		= "NAME";

static const char* ROOT_LABEL			= "ROOT";
static const char* NODENAME_LABEL		= "NODENAME";
static const char* INSTNAME_LABEL		= "INSTNAME";
static const char* INST_SERV_ENTRY_LABEL = "INST_SERV_ENTRY";

static std::map<ssa::xmEDataType, std::string> xmEDataType2Text
{
	{ ssa::xmEDataType::xmDT_INT8 ,		"INT8" },
	{ ssa::xmEDataType::xmDT_UINT8,		"UINT8" },
	{ ssa::xmEDataType::xmDT_INT16,		"INT16" },
	{ ssa::xmEDataType::xmDT_UINT16,	"UINT16" },
	{ ssa::xmEDataType::xmDT_INT32,		"INT32" },
	{ ssa::xmEDataType::xmDT_UINT32,	"UINT32" },
	{ ssa::xmEDataType::xmDT_INT64,		"INT64" },
	{ ssa::xmEDataType::xmDT_UINT64,	"UINT64" },
	{ ssa::xmEDataType::xmDT_FLOAT,		"FLOAT" },
	{ ssa::xmEDataType::xmDT_DOUBLE,	"DOUBLE" }
};

/*
** used for user config file <XXXX_DATA>
*/
class RDV2_API Type2_component
{
public:
	using element = tinyxml2::XMLElement;

	Type2_component(const std::string name, const ssa::xmEDataType dt) :
		_name(name), _type(dt) {}
	virtual ~Type2_component() {}

	virtual Type2_component* clone() const = 0;
	virtual Type2_component& add(Type2_component*) { return *this; }
	virtual std::string to_string() const = 0;
	virtual ssa::xmAccessPoint* add2vdr(rdv_2* rdv, std::string ds_name) = 0;
	virtual void init_value(rdv_2* rdv) = 0;

	virtual Type2_component* rename(std::string n) { _name = n; return this; }
	virtual const std::vector<Type2_component*>* items() { return nullptr; }
	virtual const std::string name() const { return _name; }
	virtual const ssa::xmEDataType data_type() const { return _type; }
	virtual void top_ele(bool b) { return; }

protected:
	std::string _name;
	ssa::xmEDataType _type;
};

template <typename _Tx>
class Basic_type2 : public Type2_component
{
public:
	Basic_type2(const std::string name, const ssa::xmEDataType dt, _Tx val)
		: Type2_component(name, dt), _val(val) {}

	Type2_component* clone() const override {
		return new Basic_type2<_Tx>(*this);
	}

	std::string to_string() const override;
	ssa::xmAccessPoint* add2vdr(rdv_2* rdv, std::string ds_name) override;
	void init_value(rdv_2* rdv) override;
	const _Tx value() const {
		return _val;
	}

private:
	_Tx _val;
};

class RDV2_API Buffer_type2 : public Type2_component
{
public:
	Buffer_type2(const std::string name, size_t size)
		: Type2_component(name, ssa::xmEDataType::xmDT_BUFFER),
		_size(size) {}
	Type2_component* clone() const override {
		return new Buffer_type2(*this);
	}

	std::string to_string() const override;
	ssa::xmAccessPoint* add2vdr(rdv_2* rdv, std::string ds_name) override;
	void init_value(rdv_2* rdv) override {}
private:
	size_t _size;
};

class RDV2_API Compound_type2 : public Type2_component
{
public:
	Compound_type2(const std::string name, bool is_top_ele = false)
		: Type2_component(name, ssa::xmEDataType::xmDT_STRUCT),
		_is_top_ele(is_top_ele) {}

	Type2_component* clone() const override {
		Compound_type2* t = new Compound_type2(_name);
		for_each(_items.cbegin(), _items.cend(),
			[&](Type2_component* c) {
			t->add(c->clone());
		});
		t->_is_top_ele = _is_top_ele;
		return t;
	}
	Type2_component& add(Type2_component* t) override {
		_items.push_back(t->clone());
		return *this;
	}

	std::string to_string() const override;
	ssa::xmAccessPoint* add2vdr(rdv_2* rdv, std::string ds_name) override;
	void init_value(rdv_2* rdv) override;
	const std::vector<Type2_component*>* items() override {
		return &_items;
	}

	void top_ele(bool b) override {
		_is_top_ele = b;
	}

	~Compound_type2() {
		for_each(_items.cbegin(), _items.cend(),
			[&](Type2_component* c) {
			delete c;
		});
	}

private:
	std::vector<Type2_component*> _items;
	bool _is_top_ele;
	std::stack<Type2_component*> _stck;

private:
	void _check_stack(Type2_component* ct, ssa::xmValue* xv);
	bool _is_Compound_type2(Type2_component* ct);
	std::string _stringfy(std::stack<Type2_component*> s, std::string split = ".");
	void _set_init_value(Type2_component* ct, ssa::xmValue* xv);
};

/*
** <CLASS_DECLARE><XXXX_DATA><PROPERTY> Parser
*/
class RDV2_API cust_parser
{
public:
	using element = tinyxml2::XMLElement;

	std::map<std::string, Type2_component*>	parse_class_type(tinyxml2_wrap& tw);
	std::map<std::string, Type2_component*> parse_data_type(tinyxml2_wrap& tw, const char* parent = OUTPUT_DATA_LABEL);
	std::string parse_nodename(tinyxml2_wrap& tw);

private:
	std::pair<ssa::xmEDataType, size_t> _text2xmDT(std::string t);
	void _parse_property_type(element* ele, Type2_component* parent, std::map<std::string, Type2_component*>& cust_map);
	Type2_component* _parse_data_type(element * ele, std::map<std::string, Type2_component*>& cust_map);
	Type2_component* _parse_class_type(element * ele);

public:
	static const char* name_gsa_first(element* ele);
	static const char* name_local_first(element* ele);
};

template<typename _Tx>
inline std::string Basic_type2<_Tx>::to_string() const
{
	return "\"" + _name + "\"" + ":"
		+ "\"" + xmEDataType2Text[_type] + "\"";
}

template<typename _Tx>
inline ssa::xmAccessPoint * Basic_type2<_Tx>::add2vdr(rdv_2 * rdv, std::string ds_name)
{
	return rdv->add_data(_name, ds_name, _type);
}

template<typename _Tx>
inline void Basic_type2<_Tx>::init_value(rdv_2 * rdv)
{
	rdv->set_value(_name, _val);
}

#pragma endregion

class RDV2_API rdv_2 : public ssa::xmMsgHandler, public ssa::xmVDR {
public:
	rdv_2(
		ConfigMode mode = ConfigMode::xml, 
		const char* file = "rdv2_config.xml",
		bool delete_ds_quit = true);

	rdv_2(const rdv_2&) = delete;
	rdv_2(rdv_2&&) = delete;
	rdv_2& operator = (const rdv_2&) = delete;
	rdv_2& operator = (rdv_2&&) = delete;

	virtual ~rdv_2();

	rdv2::config_base& conf() const { return *_conf; }
	void startup(uint64_t time_out_ms = 5000);

public: // APIs
	bool enable_tick(); // enable tick callback
	/*
	** data set
	*/
	ssa::xmAccessPoint* add_dataset(std::string name, bool overrided = true);
	void del_dataset(std::string name);
	void lock(std::string name);
	void unlock(std::string name);
	void lock_all();

	void begin_update(std::string name);
	ssa::xmAccessPoint* end_update(std::string name);

	/*
	** data
	*/
	ssa::xmAccessPoint* add_data(std::string d_name, std::string ds_name,
		ssa::xmEDataType d_type, size_t size = 64, const ssa::xmValue* = nullptr);
	ssa::xmAccessPoint* add_data(std::string d_name, std::string ds_name,
		bostream& bos);
	void del_data(std::string name);

	/*
	** node
	*/
	ssa::xmAccessPoint* add_node(std::string n_name, std::string ds_name, bool excep = false);
	void del_node(std::string name);
	ssa::xmAccessPoint* link_node(std::string n_name, std::string ds_name, bool excep = false);

	/*
	** access point
	*/
	ssa::xmAccessPoint* attach(std::string name, bool is_dataset = true);

	/*
	** get|set_value 
	** _n mean through node to access data
	*/
	template <typename _Tx>
	std::pair<bool, _Tx> get_value(const std::string& name);
	template <typename _Tx>
	std::pair<bool, _Tx> get_value_n(const std::string& name, const std::string& n_name);
	template <typename _Tx>
	bool get_value(const std::string& name, _Tx& val);
	template <typename _Tx>
	bool get_value_n(const std::string& name, const std::string& n_name, _Tx& val);

	template <typename _Tx>
	bool set_value(const std::string& name, const _Tx& val);
	bool set_value(const std::string& name, const std::string& val);
	bool set_value(const std::string& name, uint8_t* buf, size_t size);
	bool set_value(const std::string& name, ssa::xmValue& val);
	bool set_value(const std::string& name, bostream& bos);

	template <typename _Tx>
	bool set_value_n(const std::string& name, const std::string& n_name, const _Tx& val);
	bool set_value_n(const std::string& name, const std::string& n_name, const std::string& val);
	bool set_value_n(const std::string& name, const std::string& n_name, uint8_t* buf, size_t size);
	bool set_value_n(const std::string& name, const std::string& n_name, ssa::xmValue& val);
	bool set_value_n(const std::string& name, const std::string& n_name, bostream& bos);

	/*
	** watch
	*/
	bool watch(std::string d, ssa::xmEDataType dt, valuer cb);
	void unwatch(std::string d);
	void dispatch(std::initializer_list<std::string> l);

	/*
	** ctl_cmd
	*/
	bool listen_cmd(bool b);
	virtual void on_load() {}
	virtual void on_init() {}
	virtual void on_start() {}
	virtual void on_freeze() {}
	virtual void on_unfreeze() {}
	virtual void on_speed(int step) {}
	virtual void on_finish() {}

	bool register_cmd(std::string ds_name);
	bool send_cmd(CtrlCmd cmd, int step = 1);

	/*
	** user model config
	*/
	void export_model_conf(std::string file, std::initializer_list<const char*> labels = { OUTPUT_DATA_LABEL });

	std::string model_inst_name(std::string file, const char* attr_label = INSTNAME_LABEL);
	std::string model_node_name(std::string file, const char* attr_label = NODENAME_LABEL);
	std::string inst_serv_entry(std::string file, const char* attr_label = INST_SERV_ENTRY_LABEL);
	std::map<std::string, std::string> local2gsa(std::string file);
	std::map<std::string, std::string> gsa2local(std::string file);

	std::map<std::string, Type2_component*> parse_data(std::string file, const char* parent);

private:
	void _create_vdr(uint64_t time_out_ms);

	/*
	** name is doesn't matter, can be anything
	** the ap can get_value() any data
	*/
	ssa::xmAccessPoint* _get_read_ap(std::string name = std::string());
	/*
	** name must be data name or dataset name
	*/
	ssa::xmAccessPoint* _get_full_ap(std::string name, bool is_dataset = true);
	ssa::xmAccessPoint* _get_node_ap(std::string node_name);

	/*
	** get numberic value
	*/
	template <typename _Tx>
	std::pair<bool, _Tx> _get_value(const std::string& name, ssa::xmAccessPoint* ap);

	bool _get_value(const std::string& name, ssa::xmAccessPoint* ap, std::string& val);
	bool _get_value(const std::string& name, ssa::xmAccessPoint* ap, std::vector<uint8_t>& val);
	bool _get_value(const std::string& name, ssa::xmAccessPoint* ap, ssa::xmValue& val);
	bool _get_value(const std::string& name, ssa::xmAccessPoint* ap, bistream& bis);

	template <typename _Tx>
	bool _set_value(const std::string& name, ssa::xmAccessPoint*ap, const _Tx& val);

	bool _set_value(const std::string& name, std::pair<ssa::xmData, ssa::xmAccessPoint*> p, const std::string& val);
	bool _set_value(const std::string& name, std::pair<ssa::xmData, ssa::xmAccessPoint*> p, uint8_t* buf, size_t size);
	bool _set_value(const std::string& name, std::pair<ssa::xmData, ssa::xmAccessPoint*> p, ssa::xmValue& val);
	bool _set_value(const std::string& name, std::pair<ssa::xmData, ssa::xmAccessPoint*> p, bostream& bos);

private:
	std::pair<bool, ssa::xmDataSet> __check_ds(std::string name);
	std::pair<bool, ssa::xmData> __check_data(std::string name);

	ssa::xmAccessPoint* __check_ap(std::string ap_name); // check if ap exists
	ssa::xmAccessPoint* __add_read_ap(std::string ap_name);
	ssa::xmAccessPoint* __add_full_ap(std::string ap_name, std::string ds_name);
	ssa::xmAccessPoint* __add_node_ap(std::string ap_name, std::string nd_name);
	std::tuple<std::string, std::string> __get_ap_name(std::string name, bool is_dataset, std::string suffix = "FULL");
	ssa::xmDataSet __get_ds(const ssa::xmData& d);
	std::vector<std::string> __data_belongs(std::string ds_name);
	void __clear_ds(std::string name);

	std::pair<ssa::xmData, ssa::xmAccessPoint*>	__init_set_value(const std::string& name);
	std::pair<ssa::xmData, ssa::xmAccessPoint*>	__init_set_value_n(const std::string& name, const std::string& n_name);

	ssa::xmAccessPoint* __check_updatable(const std::string& name);
	void __check_locked(const std::string& ds_name);
	ssa::xmDataSet __before_add_data(const std::string& d_name, const std::string& ds_name);

private:
	any __extract(const ssa::xmMSGDataEvent& msg, ssa::xmEDataType data_type);
	void _assert_ready();

private: // object states
	rdv2::config_base* _conf;
	bool _delete_ds_quit;
	bool _ticked{ false };
	bool _listen_cmd{ false };
	int _step{ 1 };
	bool _ready{ false };

	std::set<std::string> _data_sets;
	std::set<std::string> _nodes;
	std::map<std::string, ssa::xmAccessPoint*> _aps;
	std::map<std::string, std::pair<valuer, ssa::xmEDataType>> _valuers;
	std::set<std::string> _lockeds;
	std::set<std::string> _updatables;

private:
	std::string __ticker{ "__INNER_TICK__" };
	std::string __watcher{ "__INNER_WATCH__" };
	std::string __cmder{ "__CTL_CMD__" };

public: // VDR message handle
	virtual void on_tick(const ssa::xmMSGTimerClick& t) {}
	void on_data(const ssa::xmMSGDataEvent& d);
	virtual void on_data_update(const ssa::xmMSGDataEvent& d) {}

	xmCREATE_MESSAGE_MAP(rdv_2)
};

template<typename _Tx>
inline std::pair<bool, _Tx> rdv_2::_get_value(const std::string & name, ssa::xmAccessPoint* ap)
{
	try
	{
		_Tx val;
		return ap->GetValue(name.c_str(), val) == -1 ?
			std::make_pair(false, _Tx()) :
			std::make_pair(true, val);
	}
	catch (...)
	{
		return std::make_pair(false, _Tx());
	}
}

template<typename _Tx>
inline std::pair<bool, _Tx> rdv_2::get_value(const std::string & name)
{
	_assert_ready();
	return _get_value<_Tx>(name, _get_read_ap());
}

template<typename _Tx>
inline std::pair<bool, _Tx> rdv_2::get_value_n(const std::string & name, const std::string & n_name)
{
	_assert_ready();
	return _get_value<_Tx>(name, _get_node_ap(n_name));
}

template<typename _Tx>
inline bool rdv_2::get_value(const std::string & name, _Tx & val)
{
	_assert_ready();
	return _get_value(name, _get_read_ap(), val);
}

template<typename _Tx>
inline bool rdv_2::get_value_n(const std::string & name, const std::string & n_name, _Tx & val)
{
	_assert_ready();
	return _get_value(name, _get_node_ap(n_name), val);
}

template<typename _Tx>
inline bool rdv_2::set_value(const std::string & name, const _Tx & val)
{
	_assert_ready();
	try
	{
		ssa::xmAccessPoint* ap = attach(name, false);
		return _set_value(name, ap, val);
	}
	catch (...)
	{
		return false;
	}
}

template<typename _Tx>
inline bool rdv_2::set_value_n(const std::string & name, const std::string & n_name, const _Tx & val)
{
	_assert_ready();
	return _set_value(name, _get_node_ap(n_name), val);
}

template<typename _Tx>
inline bool rdv_2::_set_value(const std::string & name, ssa::xmAccessPoint*ap, const _Tx& val)
{
	try
	{
		if (!__check_data(name).first) return false;
		return ap->SetValue(name.c_str(), val) != -1;
	}
	catch (...)
	{
		return false;
	}
}

_RDV2_END

#endif /* _RDV2_RDV2_H */
