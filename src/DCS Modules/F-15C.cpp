#include "DCS_AB_detent_editor.h"
#include "AModule.hpp"
#include "lua_parsing.h"

namespace fs = std::filesystem;
using namespace std::string_view_literals;

ModuleF15C::ModuleF15C(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	_name = DISPLAY_NAME;
	InteropString module_path = DCS_install_path + "\\Mods\\aircraft\\" + MODULE_NAME.data();
	_conf_file = DCS_saved_games_path + R"(\Config\Input\F-15C\joystick\)"
		// TODO: Extract name & GUID from DirectInput
		"Throttle - HOTAS Warthog {3AF7E350-AF2C-11ee-8003-444553540000}.diff.lua";

	_installed = fs::exists(module_path.get_mbs()) && fs::is_directory(module_path.get_mbs());

	if (_installed)
		_enabled = true;
		// _enabled = update_detent_from_conf_file();
}

// Nice, that works!
auto	ModuleF15C::set_detent(float val_0_100) -> result_t
{
	_detent = val_0_100;
	auto curve_points = get_custom_curves(DEFAULT_DETENT, 100 - val_0_100, false);
	for (float i : curve_points)
		printf("%f ", i);
	printf("\n");
	return result_t();
}
