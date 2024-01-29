#include "AModule.h"

const std::array<const char *, AModule::NUM_SUPPORTED_MODULES> AModule::supported_modules = {
    "M-2000C",
    "Mirage-F1",
#ifdef _DEBUG
    "DEBUG: This module doesn't exist"
#endif
};

std::shared_ptr<AModule>    AModule::getModule(std::string const& DCS_path, std::string const &module_name)
{
    std::wstring DCS_ws_path = InteropString::mbs_to_wcs(DCS_path);

    if (module_name == "M-2000C")
        return std::shared_ptr<AModule>(new Module_M2000C(DCS_ws_path));

    else if (module_name == "Mirage-F1")
        return std::shared_ptr<AModule>(new Module_MirageF1(DCS_ws_path));

    else
        return std::shared_ptr<AModule>(new Module_NotFound);
}

std::shared_ptr<AModule>    AModule::getModule(std::string const& DCS_path, std::wstring const &module_name)
{
    return AModule::getModule(DCS_path, InteropString::wcs_to_mbs(module_name));
}

std::string const& AModule::name() const
{
    return _name;
}

bool    AModule::is_installed() const
{
    return _is_installed;
}

InteropString	AModule::getSavedGamesPath() const
{
	WCHAR* saved_games_path;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_SavedGames, 0, NULL, &saved_games_path);
	if (hr != S_OK || saved_games_path == NULL)
		throw std::runtime_error("Unable to get Saved Games folder path");

	InteropString saved_games = std::wstring(saved_games_path);
	CoTaskMemFree(saved_games_path);
	return saved_games;
}

Module_M2000C::Module_M2000C(std::wstring const& DCS_ws_path) : AModule(_display_name)
{
    const std::wstring  module_path = DCS_ws_path + L"\\Mods\\aircraft\\" + _module_name;

    DWORD attribs = GetFileAttributesW(module_path.c_str());
    if (attribs != INVALID_FILE_ATTRIBUTES)
        _is_installed = attribs & FILE_ATTRIBUTE_DIRECTORY;

	InteropString options_lua = getSavedGamesPath() + L"\\DCS.openbeta\\Config\\options.lua";

    std::ifstream   options_ifs(options_lua.get_wcs());
    if (!options_ifs.is_open())
        throw std::runtime_error("Couldn't open options.lua");

    std::stringstream   options_sstream;
    options_sstream << options_ifs.rdbuf();
    std::string file_content = options_sstream.str();
    auto [begin_detent, end_detent] = _get_detent_from_file(file_content);

    _detent = std::stof(std::string(begin_detent, end_detent)) / 100.0f;
}

int Module_M2000C::set_detent(float pos)
{
    if (_is_installed == false)
        throw std::logic_error("Tried to set AB on non-existant module\n\
You might not have this module installed");

	InteropString options_lua = getSavedGamesPath() + L"\\DCS.openbeta\\Config\\options.lua";

	std::ifstream   options_ifs(options_lua.get_wcs());
	if (!options_ifs.is_open())
        throw std::runtime_error("Couldn't open options.lua");

    std::stringstream   options_sstream;
    options_sstream << options_ifs.rdbuf();
    options_ifs.close();
    std::string file_content = options_sstream.str();
    auto [begin_detent, end_detent] = _get_detent_from_file(file_content);
    file_content.replace(begin_detent, end_detent, std::to_string(pos * 100) + ',');

    std::ofstream   options_ofs(options_lua.get_wcs(), std::ios_base::trunc);
    if (!options_ofs.is_open())
        throw std::runtime_error("Couldn't open options.lua");
    options_ofs << file_content;
    options_ofs.close();

    _detent = pos;

    return 0;
}

// Sure, trailing return type, why not? Neat C++11 feature, utterly useless here, thus indispensable
auto Module_M2000C::_get_detent_from_file(std::string &file_content) const -> iterator_pair
{
    constexpr const std::string_view mirage_key = "[\"M-2000C\"] = {";
    constexpr const std::string_view AB_detent_key = "[\"THROTTLE_AB_DETENT\"]";

    auto mirage_block_begin = std::search(file_content.begin(), file_content.end(),
        mirage_key.begin(), mirage_key.end());
    auto mirage_block_end = std::find(mirage_block_begin, file_content.end(), '}');
    if (mirage_block_begin >= file_content.end() || mirage_block_end >= file_content.end())
        throw std::runtime_error("cannot find M-2000C block in options.lua");

    auto cursor = std::search(mirage_block_begin, mirage_block_end,
        AB_detent_key.begin(), AB_detent_key.end());
    if (cursor == mirage_block_end)
        throw std::runtime_error("Couldn't find THROTTLE_AB_BLOCK key in lua");
    cursor += AB_detent_key.length() + 3; // Points to the value after the " = "

    auto LF_char = std::find_if(cursor, mirage_block_end,
		[](char c) { return c == '\r' || c == '\n'; });
    if (LF_char == mirage_block_end)
        throw std::runtime_error("Corrupted lua file, oh no");

    return {cursor, LF_char};
}

float   Module_M2000C::get_detent() const
{
    return _detent;
}



Module_MirageF1::Module_MirageF1(std::wstring const& DCS_ws_path) : AModule(_display_name)
{
    const std::wstring  module_path = DCS_ws_path + L"\\Mods\\aircraft\\" + _module_name;
    DWORD attribs = GetFileAttributesW(module_path.c_str());
    if (attribs != INVALID_FILE_ATTRIBUTES)
        _is_installed = attribs & FILE_ATTRIBUTE_DIRECTORY;

	InteropString options_lua = getSavedGamesPath() + L"\\DCS.openbeta\\Config\\options.lua";

	std::ifstream   options_ifs(options_lua.get_wcs());
	if (!options_ifs.is_open())
        throw std::runtime_error("Couldn't open options.lua");

    std::stringstream   options_sstream;
    options_sstream << options_ifs.rdbuf();
    std::string file_content = options_sstream.str();
    auto [begin_detent, end_detent] = _get_detent_from_file(file_content);

    _detent = std::stof(std::string(begin_detent, end_detent)) / 100.0f;
}

int Module_MirageF1::set_detent(float pos)
{
    if (_is_installed == false)
        throw std::logic_error("Tried to set AB on non-existant module\n\
You might not have this module installed");

	InteropString options_lua = getSavedGamesPath() + L"\\DCS.openbeta\\Config\\options.lua";

	std::ifstream   options_ifs(options_lua.get_wcs());
	if (!options_ifs.is_open())
        throw std::runtime_error("Couldn't open options.lua");

    std::stringstream   options_sstream;
    options_sstream << options_ifs.rdbuf();
    options_ifs.close();
    std::string file_content = options_sstream.str();
    auto [begin_detent, end_detent] = _get_detent_from_file(file_content);
    file_content.replace(begin_detent, end_detent, std::to_string(pos * 100) + ',');

    std::ofstream   options_ofs(options_lua.get_wcs(), std::ios_base::trunc);
    if (!options_ofs.is_open())
        throw std::runtime_error("Couldn't open options.lua");
    options_ofs << file_content;
    options_ofs.close();

    _detent = pos;

    return 0;
}

float Module_MirageF1::get_detent() const
{
    if (_is_installed == false) // transform in assert ? Should never happen
        throw std::logic_error("Tried to set AB on non-existant module\n\
You might not have this module installed");

    return _detent;
}

auto Module_MirageF1::_get_detent_from_file(std::string &file_content) const -> iterator_pair
{
    constexpr const std::string_view mirage_key		= "[\"Mirage-F1\"] = {";
    constexpr const std::string_view AB_detent_key	= "[\"F1SelectedABDetentPos\"]";

    auto mirage_block_begin = std::search(file_content.begin(), file_content.end(),
        mirage_key.begin(), mirage_key.end());
    auto mirage_block_end = std::find(mirage_block_begin, file_content.end(), '}');
    if (mirage_block_begin >= file_content.end() || mirage_block_end >= file_content.end())
        throw std::runtime_error("cannot find Mirage-F1 block in options.lua");

    auto cursor = std::search(mirage_block_begin, mirage_block_end,
        AB_detent_key.begin(), AB_detent_key.end());
    if (cursor == mirage_block_end)
        throw std::runtime_error("Couldn't find F1SelectedABDetentPos key in lua");
    cursor += AB_detent_key.length() + 3; // Points to the value after the " = "

    auto LF_char = std::find_if(cursor, mirage_block_end,
		[](char c) { return c == '\r' || c == '\n'; });
    if (LF_char == mirage_block_end)
        throw std::runtime_error("Corrupted lua file, oh no");

    return { cursor, LF_char };
}


#ifdef _DEBUG
# define MODULE_NOTFOUND_DISPLAYNAME    "DEBUG: Module_NotFound instance"
#else
# define MODULE_NOTFOUND_DISPLAYNAME    "ERROR"
#endif

Module_NotFound::Module_NotFound() : AModule(MODULE_NOTFOUND_DISPLAYNAME)
{
}

int Module_NotFound::set_detent(float)
{
    throw std::logic_error("Fatal: This should never happen. Contact the dev!");
}

float Module_NotFound::get_detent() const
{
    throw std::logic_error("Fatal: This should never happen. Contact the dev!");
}
