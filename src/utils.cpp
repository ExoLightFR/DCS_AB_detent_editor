#include <DCS_AB_detent_editor.h>
#include <string>
#include <stdexcept>
#include <windows.h>

std::string  wcstring_to_mbstring(std::wstring const &src)
{
	if (src.empty())
		return "";

	size_t new_buf_len = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, NULL, 0, NULL, NULL);
	char    *multibyte_buffer = new char[new_buf_len];
	WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, multibyte_buffer, new_buf_len, NULL, NULL);
	std::string retval(multibyte_buffer);
	delete[] multibyte_buffer;
	return retval;
}

std::wstring  mbstring_to_wcstring(std::string const &src)
{
	if (src.empty())
		return L"";

	// https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
	size_t new_buf_len = MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, NULL, 0);
	WCHAR *widechar_buffer = new WCHAR[new_buf_len];
	MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, widechar_buffer, new_buf_len);
	std::wstring retval(widechar_buffer);
	delete[] widechar_buffer;
	return retval;
}

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
