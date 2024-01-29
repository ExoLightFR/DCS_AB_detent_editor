#include "AModule2.hpp"
#include <map>
#include <functional>
#include <cassert>
#include <fstream>
#include <Windows.h>
#include "lua_parsing.h"

InteropString	get_saved_games_path();

using namespace v2;
using namespace std::literals;

std::unique_ptr<AModule>
AModule::get_module(InteropString const& DCS_path, std::string_view module_name)
{
	using functor_t	= std::function<std::unique_ptr<AModule>()>;
	using unique	= std::unique_ptr<AModule>;

	std::map<std::string_view, functor_t>	factories = {
		{ "M-2000C",	[&]{return unique(new ModuleM2000C(DCS_path));}		},
		{ "Mirage-F1",	[&]{return unique(new ModuleMirageF1);}				},
		{ "F-15E",		[&]{return unique(new ModuleF15E);}					},
	};

	assert(factories.size() == supported_modules.size());

	if (!factories.contains(module_name))
		return nullptr;
	else
		return factories.at(module_name)();
}

/* ============================================================================================== */
/* ======================================== Mirage 2000C ======================================== */
/* ============================================================================================== */

ModuleM2000C::ModuleM2000C(InteropString const& saved_games_path)
{
	_name = DISPLAY_NAME;
	InteropString module_path = saved_games_path + "\\Mods\\aircraft\\" + MODULE_NAME.data();
	_conf_file = saved_games_path + "\\options.lua";

	DWORD attribs = GetFileAttributesW(module_path.get_wcs().c_str());
	if (attribs == INVALID_FILE_ATTRIBUTES)
		return;
	else
		_installed = attribs & FILE_ATTRIBUTE_DIRECTORY;

	if (_installed)
		_enabled = update_detent_from_conf_file();

}

/*
* Returns whether detent was successfully fetched from config file or not.
*/
bool	ModuleM2000C::update_detent_from_conf_file()
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file(_conf_file);

	if (!lua["options"]["plugins"]["M-2000C"]["THROTTLE_AB_DETENT"].valid())
		return false;

	sol::table m2000 = lua["options"]["plugins"]["M-2000C"];
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

	if (!lua["options"]["plugins"]["M-2000C"]["THROTTLE_AB_DETENT"].valid())
		return false;

	sol::table m2000 = lua["options"]["plugins"]["M-2000C"];
	m2000["THROTTLE_AB_DETENT"] = val_0_100;

	TableValue::table_t	options = lua_table_to_map(lua["options"]);
	std::ofstream options_lua_file(_conf_file.get_wcs(), std::ios::out | std::ios::binary);
	options_lua_file
		<< "options = {\n"
		<< dump_lua_table(options)
		<< "}\n";

	return !options_lua_file.fail();	// Don't return good(), return !fail(), not the same thing lmao
}
