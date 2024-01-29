#pragma once
#include <map>
#include <string>
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"

namespace std
{
	string	to_string(sol::object const& obj);
}

/*
* A TableValue can either be a Lua value (sol::object), i.e. bool, string, number, string...
* or it can be another table, to allow for nested Lua tables.
*/
struct TableValue
{
	using table_t = std::map<std::string, TableValue>;
	using value_t = sol::object;

	std::variant<table_t, value_t>	val;
};

std::map<std::string, TableValue>	lua_table_to_map(sol::table const& table);

std::string	dump_lua_table(TableValue::table_t const& table, int depth = 1);
