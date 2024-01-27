#include "InteropString.hpp"
#include <Windows.h>

InteropString	operator+(InteropString const& lhs, std::string const& rhs)
{
	InteropString ret = lhs;
	ret += rhs;
	return ret;
}

InteropString	operator+(InteropString const& lhs, std::wstring const& rhs)
{
	InteropString ret = lhs;
	ret += rhs;
	return ret;
}


std::string	InteropString::wcs_to_mbs(std::wstring const& wcs)
{
	if (wcs.empty())
		return "";

	size_t new_buf_len = WideCharToMultiByte(CP_UTF8, 0, wcs.c_str(), -1, NULL, 0, NULL, NULL);
	char* multibyte_buffer = new char[new_buf_len];
	WideCharToMultiByte(CP_UTF8, 0, wcs.c_str(), -1, multibyte_buffer, (int)new_buf_len, NULL, NULL);
	std::string retval(multibyte_buffer);
	delete[] multibyte_buffer;
	return retval;
}

std::wstring	InteropString::mbs_to_wcs(std::string const& mbs)
{
	if (mbs.empty())
		return L"";

	// https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-multibytetowidechar
	size_t new_buf_len = MultiByteToWideChar(CP_UTF8, 0, mbs.c_str(), -1, NULL, 0);
	WCHAR* widechar_buffer = new WCHAR[new_buf_len];
	MultiByteToWideChar(CP_UTF8, 0, mbs.c_str(), -1, widechar_buffer, (int)new_buf_len);
	std::wstring retval(widechar_buffer);
	delete[] widechar_buffer;
	return retval;
}
