#include <filesystem>
#include <fstream>
#include "AModule.hpp"
#include "lua_parsing.h"

namespace fs = std::filesystem;

ModuleM2000C::ModuleM2000C(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	_name = DISPLAY_NAME;
	InteropString module_path = DCS_install_path + "\\Mods\\aircraft\\" + MODULE_NAME.data();
	_conf_file = DCS_saved_games_path + "\\Confxig\\options.lua";

	_installed = fs::exists(module_path.get_mbs()) && fs::is_directory(module_path.get_mbs());

	if (_installed)
		_enabled = update_detent_from_conf_file();
}

/*
* Returns whether detent was successfully fetched from config file or not.
*/
bool	ModuleM2000C::update_detent_from_conf_file()
{
	sol::state	lua;
	bool		success = true;
	// For some reason I can't find a better way to not throw on error...
	auto error_handler = [&](lua_State*, sol::protected_function_result) {
		success = false; return sol::protected_function_result();
		};

	lua.open_libraries(sol::lib::base);
	lua.safe_script_file(_conf_file, error_handler);
	if (!success)
		return false;

	// Why does this not throw when erroring without a handler, but safe_script_file does?
	if (!lua["options"]["plugins"][MODULE_NAME.data()]["THROTTLE_AB_DETENT"].valid())
		return false;

	sol::table m2000 = lua["options"]["plugins"][MODULE_NAME.data()];
	std::optional<double> detent = m2000["THROTTLE_AB_DETENT"];
	if (detent)
	{
		_detent = (float)*detent;
		return true;
	}
	else
		return false;
}

bool	ModuleM2000C::set_detent(float val_0_100)
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file(_conf_file);

	if (!lua["options"]["plugins"][MODULE_NAME.data()]["THROTTLE_AB_DETENT"].valid())
		return false;

	sol::table m2000 = lua["options"]["plugins"][MODULE_NAME.data()];
	m2000["THROTTLE_AB_DETENT"] = val_0_100;

	TableValue::table_t	options = lua_table_to_map(lua["options"]);
	std::ofstream options_lua_file(_conf_file.get_wcs(), std::ios::out | std::ios::binary);
	options_lua_file
		<< "options = {\n"
		<< dump_lua_table(options)
		<< "}\n";

	if (!options_lua_file.fail())	// Don't use good(), return !fail(), not the same thing lmao
	{
		_detent = val_0_100;
		return true;
	}
	else
		return false;
}
