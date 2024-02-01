#include "DCS_AB_detent_editor.h"
#include "lua_parsing.h"

namespace fs = std::filesystem;
using namespace std::string_view_literals;

ModuleM2000C::ModuleM2000C(InteropString const& DCS_install_path,
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
bool	ModuleM2000C::update_detent_from_conf_file()
{
	sol::state lua;
	if (!safe_open_lua_context(lua))
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

auto	ModuleM2000C::set_detent(float val_0_100) -> result_t
{
	sol::state lua;
	if (!safe_open_lua_context(lua))
		return tl::unexpected(std::format("Failed to open {} in Lua context", _conf_file));

	if (!lua["options"]["plugins"][MODULE_NAME.data()]["THROTTLE_AB_DETENT"].valid())
		return tl::unexpected("Cannot find THROTTLE_AB_DETENT entry in M-2000C options");

	sol::table m2000 = lua["options"]["plugins"][MODULE_NAME.data()];
	m2000["THROTTLE_AB_DETENT"] = val_0_100;

	TableValue::table_t	options = lua_table_to_map(lua["options"]);
	std::ofstream options_lua_file(_conf_file.get_wcs(), std::ios::out | std::ios::binary);
	options_lua_file
		<< "options = {\n"
		<< dump_lua_table(options)
		<< "}\n";

	if (options_lua_file.fail())	// Don't use good(), return !fail(), not the same thing lmao
#pragma warning(suppress : 4996)	// Fuck off MSVC, strerror is FINE HERE
		return tl::unexpected(std::format("Failed file operation: {}", strerror(errno)));

	_detent = val_0_100;
	return result_t();
}
