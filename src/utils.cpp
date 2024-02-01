#include <DCS_AB_detent_editor.h>
#include <stdexcept>
#include <shlobj_core.h>

std::wstring RegGetString(HKEY hKey, const std::wstring &subKey, const std::wstring &value)
{
	DWORD dataSize{};
	LONG retCode = ::RegGetValue(
		hKey,
		subKey.c_str(),
		value.c_str(),
		RRF_RT_REG_SZ,
		nullptr,
		nullptr,
		&dataSize
	);
	if (retCode != ERROR_SUCCESS)
		throw std::runtime_error("Cannot read string from registry");
	std::wstring data;
	data.resize(dataSize / sizeof(wchar_t));
	retCode = ::RegGetValue(
		hKey,
		subKey.c_str(),
		value.c_str(),
		RRF_RT_REG_SZ,
		nullptr,
		&data[0],
		&dataSize
	);
	if (retCode != ERROR_SUCCESS)
		throw std::runtime_error("Cannot read string from registry");
	data.shrink_to_fit();
	return data;
}

int RegGetString(HKEY hKey, const std::wstring &subKey, const std::wstring &value,
	std::wstring &outstr)
{
	DWORD dataSize{};
	LONG retCode = ::RegGetValueW(
		hKey,
		subKey.c_str(),
		value.c_str(),
		RRF_RT_REG_SZ,
		nullptr,
		nullptr,
		&dataSize
	);
	if (retCode != ERROR_SUCCESS)
		return 1;
	outstr.resize(dataSize / sizeof(wchar_t));
	retCode = ::RegGetValueW(
		hKey,
		subKey.c_str(),
		value.c_str(),
		RRF_RT_REG_SZ,
		nullptr,
		&outstr[0],
		&dataSize
	);
	if (retCode != ERROR_SUCCESS)
		return 2;
	outstr.shrink_to_fit();
	return 0;
}

InteropString	get_saved_games_path()
{
	WCHAR* saved_games_path;

	HRESULT hr = SHGetKnownFolderPath(FOLDERID_SavedGames, 0, NULL, &saved_games_path);
	if (hr != S_OK || saved_games_path == NULL)
		throw std::runtime_error("Unable to get Saved Games folder path");

	InteropString saved_games = std::wstring(saved_games_path);
	CoTaskMemFree(saved_games_path);
	return saved_games;
}

std::string	trim_str(std::string_view str)
{
	int i = 0;
	while (i < str.length() && isspace(str[i]))
		++i;
	int j = (int)str.length() - 1;
	while (j > 0 && isspace(str[j]))
		--j;
	return std::string(str, i, std::max(j - i + 1, 0));
}
