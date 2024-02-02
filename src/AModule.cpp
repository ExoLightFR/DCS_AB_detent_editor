#include <functional>
#include <cassert>
#include "lua_parsing.h"

InteropString	get_saved_games_path();

using namespace std::literals;
namespace fs = std::filesystem;

std::unique_ptr<AModule>
AModule::get_module(std::string_view module_name, InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	using functor_t	= std::function<std::unique_ptr<AModule>()>;
	using unique	= std::unique_ptr<AModule>;

	std::map<std::string_view, functor_t>	factories = {
		{ "M-2000C",	[&]{return unique(new ModuleM2000C(DCS_install_path, DCS_saved_games_path));}	},
		{ "Mirage-F1",	[&]{return unique(new ModuleMirageF1(DCS_install_path, DCS_saved_games_path));}	},
		{ "F-15E",		[&]{return unique(new ModuleF15E(DCS_install_path, DCS_saved_games_path));}		},
		{ "F-4E",		[&]{return unique(new ModuleF4E(DCS_install_path, DCS_saved_games_path));}		},
		{ "F-15C",		[&]{return unique(new ModuleF15C(DCS_install_path, DCS_saved_games_path));}		},
	};

	assert(factories.size() == supported_modules.size());

	if (!factories.contains(module_name))
		return nullptr;
	else
		return factories.at(module_name)();
}

bool	AModule::safe_open_lua_context(sol::state& lua) const
{
	bool success = true;
	// For some reason I can't find a better way to not throw on error...
	auto error_handler = [&](lua_State*, sol::protected_function_result) {
		success = false;
		return sol::protected_function_result();
	};

	lua.open_libraries(sol::lib::base);
	lua.safe_script_file(_conf_file, error_handler);
	return success;
}
