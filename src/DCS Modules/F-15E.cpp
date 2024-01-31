#include "AModule.hpp"
#include "lua_parsing.h"

namespace fs = std::filesystem;
using namespace std::string_view_literals;

ModuleF15E::ModuleF15E(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	_name = DISPLAY_NAME;
	InteropString module_path = DCS_install_path + "\\Mods\\aircraft\\" + MODULE_NAME.data();
	_conf_file = DCS_saved_games_path + "\\Config\\options.lua";

	_installed = fs::exists(module_path.get_mbs()) && fs::is_directory(module_path.get_mbs());

	if (_installed)
		_enabled = update_detent_from_conf_file();
}

/*
* Returns whether detent was successfully fetched from config file or not.
*/
bool	ModuleF15E::update_detent_from_conf_file()
{
	sol::state lua;
	if (!safe_open_lua_context(lua))
		return false;

	// Don't use MODULE_NAME, for some reason it's named "F-15E" or "F-15ESE" depending on where you look
	if (!lua["options"]["plugins"]["F-15ESE"]["THROTTLE_AB_DETENT"].valid())
		return false;

	sol::table eagle_lmao_axelgt = lua["options"]["plugins"]["F-15ESE"];
	std::optional<double> detent = eagle_lmao_axelgt["THROTTLE_AB_DETENT"];
	if (detent)
	{
		_detent = (float)*detent;
		return true;
	}
	else
		return false;
}

auto	ModuleF15E::set_detent(float val_0_100) -> result_t
{
	sol::state lua;
	if (!safe_open_lua_context(lua))
		return tl::unexpected(std::format("Failed to open {} in Lua context", _conf_file));

	if (!lua["options"]["plugins"]["F-15ESE"]["THROTTLE_AB_DETENT"].valid())
		return tl::unexpected("Cannot find THROTTLE_AB_DETENT entry in F-15E options");

	sol::table eagle_lmao_axelgt = lua["options"]["plugins"]["F-15ESE"];
	eagle_lmao_axelgt["THROTTLE_AB_DETENT"] = val_0_100;

	TableValue::table_t	options = lua_table_to_map(lua["options"]);
	std::ofstream options_lua_file(_conf_file.get_wcs(), std::ios::out | std::ios::binary);
	options_lua_file
		<< "options = {\n"
		<< dump_lua_table(options)
		<< "}\n";

	if (options_lua_file.fail())	// Don't use good(), return !fail(), not the same thing lmao
#pragma warning(suppress : 4996)
		return tl::unexpected(std::format("Failed file operation: {}", strerror(errno)));

	_detent = val_0_100;
	return result_t();
}
