#include <cassert>
#include <format>
#include <iomanip>
#include <shlobj_core.h>
#include "lua_parsing.h"

namespace std
{
	string	to_string(sol::object const& obj)
	{
		switch (obj.get_type())
		{
		case sol::type::boolean:
			return obj.as<bool>() ? "true" : "false";
		case sol::type::nil:
			return "nil";
		case sol::type::number:
		{
			double value = obj.as<double>();
			std::stringstream out;
			out << setprecision(14) << value;
			return out.str();
		}
		case sol::type::string:
			return std::format(R"("{}")", obj.as<string>());
		default:
			return "???";
		}
	}
}

/*
* Converts a sol::table from a Lua state to a std::map that can contain sol::objects and other,
* nested maps.
*/
std::map<std::string, TableValue>	lua_table_to_map(sol::table const& table)
{
	std::map<std::string, TableValue>	res;

	for (auto& [k, v] : table)
	{
		std::string key = k.as<std::string>();
		if (v.get_type() == sol::type::table)
			res.insert({ key, TableValue{lua_table_to_map(v.as<sol::table>())} });
		else
			res.insert({ key, TableValue{v} });
	}
	return res;
}

/*
* Returns a string that's a dump of the given table, converted from a sol::table.
* The depth parameter established the amount of tabulation for formatting.
*/
std::string	dump_lua_table(TableValue::table_t const& table, int depth)
{
	const std::string tabulation(depth, '\t');
	std::string res;

	for (auto& [k, v] : table)
	{
		if (std::holds_alternative<TableValue::table_t>(v.val))
		{
			res += std::format("{}[\"{}\"] = {{\n{}{}}},\n",
				tabulation,
				k,
				dump_lua_table(std::get<TableValue::table_t>(v.val), depth + 1),
				tabulation
			);
		}
		else
		{
			res += std::format("{}[\"{}\"] = {},\n",
				tabulation,
				k,
				std::to_string(std::get<TableValue::value_t>(v.val))
			);
		}
	}
	return res;
}
