#include <string>
#include <cassert>
#include <format>
#include <map>
#include <iomanip>
#include <Windows.h>
#include <shlobj.h>
#define SOL_ALL_SAFETIES_ON 1
#include "sol/sol.hpp"
#include "InteropString.hpp"
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

static std::string	testing_options_lua(InteropString options_lua)
{
	sol::state	lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file(options_lua);

	if (!lua["options"]["plugins"]["M-2000C"].valid())
		return "Error, M-2000C does not exist";

	sol::table m2000 = lua["options"]["plugins"]["M-2000C"];
	if (!m2000["THROTTLE_AB_DETENT"].valid())
		return "Error, THROTTLE_AB_DETENT does not exist";

	double old_value = m2000["THROTTLE_AB_DETENT"];
	m2000["THROTTLE_AB_DETENT"] = 80;
	double new_value = m2000["THROTTLE_AB_DETENT"];

	return std::format("Test: {}, {}", old_value, new_value);
}

#include <fstream>

[[nodiscard]] std::string	lua_testing_shit()
{
	WCHAR* saved_games_path;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_SavedGames, 0, NULL, &saved_games_path);
	if (hr != S_OK || saved_games_path == NULL)
		throw std::runtime_error("Unable to get Saved Games folder path");

	InteropString saved_games = saved_games_path;
	CoTaskMemFree(saved_games_path);

	// return testing_options_lua(saved_games + "\\DCS.openbeta\\Config\\options.lua";);

	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file(saved_games + "\\DCS.openbeta\\Config\\options.lua");

	TableValue::table_t	srs_conf = lua_table_to_map(lua["options"]);

	std::string res = dump_lua_table(srs_conf, 1);

	static bool did_copy = false;
	if (!did_copy)
	{
		std::string copy_path = saved_games + "\\DCS.openbeta\\Config\\options_copy.lua";
		std::ofstream copy(copy_path, std::ios::out | std::ios::binary); // binary removes CRLF
		copy << "options = {\n";
		copy << res;
		copy << "}\n";
		copy.close();
		did_copy = true;
	}

	return res;
}
