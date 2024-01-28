#include "AModule2.hpp"
#include <map>
#include <functional>
#include <cassert>

using namespace v2;

std::unique_ptr<AModule>
AModule::get_module(InteropString const& DCS_path, std::string_view module_name)
{
	using functor_t	= std::function<std::unique_ptr<AModule>()>;
	using unique	= std::unique_ptr<AModule>;

	std::map<std::string_view, functor_t>	factories = {
		{ "M-2000C",	[]{return unique(new ModuleM2000C);}	},
		{ "Mirage-F1",	[]{return unique(new ModuleMirageF1);}	},
		{ "F-15E",		[]{return unique(new ModuleF15E);}		},
	};

	assert(factories.size() == supported_modules.size());

	if (!factories.contains(module_name))
		return nullptr;
	else
		return factories.at(module_name)();
}
