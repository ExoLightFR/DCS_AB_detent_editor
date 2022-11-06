#include "AModule.h"

const std::array<const char *, 3> AModule::supported_modules = {
    "M-2000C",
    "Mirage-F1",
    "DEBUG: This module doesn't exist" };

std::shared_ptr<AModule>    AModule::getModule(std::string const& DCS_path, std::string const &module_name)
{
    std::wstring DCS_ws_path = mbstring_to_wcstring(DCS_path);

    if (module_name == "M-2000C")
        return std::shared_ptr<AModule>(new Module_M2000C(DCS_ws_path));

    else if (module_name == "Mirage-F1")
        return std::shared_ptr<AModule>(new Module_MirageF1(DCS_ws_path));

    else
        return std::shared_ptr<AModule>(new Module_NotFound);
}

std::shared_ptr<AModule>    AModule::getModule(std::string const& DCS_path, std::wstring const &module_name)
{
    return AModule::getModule(DCS_path, wcstring_to_mbstring(module_name));
}

std::string const& AModule::name() const
{
    return _name;
}

bool    AModule::is_installed() const
{
    return _is_installed;
}



Module_M2000C::Module_M2000C(std::wstring const& DCS_ws_path) : AModule(_display_name)
{
    const std::wstring  module_path = DCS_ws_path + L"\\Mods\\aircraft\\" + _module_name;

    DWORD attribs = GetFileAttributesW(module_path.c_str());
    if (attribs != INVALID_FILE_ATTRIBUTES)
        _is_installed = attribs & FILE_ATTRIBUTE_DIRECTORY;

    WCHAR *saved_games_path;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_SavedGames, 0, NULL, &saved_games_path);
    if (hr != S_OK || saved_games_path == NULL)
        throw std::runtime_error("Unable to get Saved Games folder path");
    std::wstring    options_lua_path = saved_games_path;
    options_lua_path += L"\\DCS.openbeta\\Config\\options.lua";
    CoTaskMemFree(saved_games_path);

    std::ifstream   options_ifs(options_lua_path.c_str());
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

    WCHAR       *saved_games_path;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_SavedGames, 0, NULL, &saved_games_path);
    if (hr != S_OK || saved_games_path == NULL)
        throw std::runtime_error("Unable to get Saved Games folder path");
    std::wstring    options_lua_path = saved_games_path;
    options_lua_path += L"\\DCS.openbeta\\Config\\options.lua";
    CoTaskMemFree(saved_games_path);

    std::ifstream   options_ifs(options_lua_path.c_str());
    if (!options_ifs.is_open())
        throw std::runtime_error("Couldn't open options.lua");

    std::stringstream   options_sstream;
    options_sstream << options_ifs.rdbuf();
    options_ifs.close();
    std::string file_content = options_sstream.str();
    auto [begin_detent, end_detent] = _get_detent_from_file(file_content);
    file_content.replace(begin_detent, end_detent, std::to_string(pos * 100) + ',');

    std::ofstream   options_ofs(options_lua_path.c_str(), std::ios_base::trunc);
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

    auto LF_char = std::find(cursor, mirage_block_end, '\n');
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

    WCHAR *saved_games_path;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_SavedGames, 0, NULL, &saved_games_path);
    if (hr != S_OK || saved_games_path == NULL)
        throw std::runtime_error("Unable to get Saved Games folder path");
    std::wstring    options_lua_path = saved_games_path;
    options_lua_path += L"\\DCS.openbeta\\Config\\options.lua";
    CoTaskMemFree(saved_games_path);

    std::ifstream   options_ifs(options_lua_path.c_str());
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


    WCHAR *saved_games_path;

    HRESULT hr = SHGetKnownFolderPath(FOLDERID_SavedGames, 0, NULL, &saved_games_path);
    if (hr != S_OK || saved_games_path == NULL)
        throw std::runtime_error("Unable to get Saved Games folder path");
    std::wstring    options_lua_path = saved_games_path;
    options_lua_path += L"\\DCS.openbeta\\Config\\options.lua";
    CoTaskMemFree(saved_games_path);

    std::ifstream   options_ifs(options_lua_path.c_str());
    if (!options_ifs.is_open())
        throw std::runtime_error("Couldn't open options.lua");

    std::stringstream   options_sstream;
    options_sstream << options_ifs.rdbuf();
    options_ifs.close();
    std::string file_content = options_sstream.str();
    auto [begin_detent, end_detent] = _get_detent_from_file(file_content);
    file_content.replace(begin_detent, end_detent, std::to_string(pos * 100) + ',');

    std::ofstream   options_ofs(options_lua_path.c_str(), std::ios_base::trunc);
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
    constexpr const std::string_view mirage_key = "[\"Mirage-F1\"] = {";
    constexpr const std::string_view AB_detent_key = "[\"F1SelectedABDetentPos\"]";

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

    auto LF_char = std::find(cursor, mirage_block_end, '\n');
    if (LF_char == mirage_block_end)
        throw std::runtime_error("Corrupted lua file, oh no");

    return { cursor, LF_char };
}



Module_NotFound::Module_NotFound() : AModule("DEBUG: Module not found")
{
}

int Module_NotFound::set_detent(float pos)
{
    (void)pos;
    throw std::logic_error("Tried to set detent on non-existing module");
}

float Module_NotFound::get_detent() const
{
    throw std::logic_error("Tried to get detent on non-existing module");
}
