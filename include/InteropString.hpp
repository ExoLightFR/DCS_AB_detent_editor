#pragma once

#include <string>

class InteropString
{
private:
	std::string		_mbs;
	std::wstring	_wcs;

public:
	InteropString() = default;
	InteropString(std::string const& mbs) : _mbs(mbs), _wcs(mbs_to_wcs(mbs)) {}
	InteropString(std::wstring const& wcs) : _mbs(wcs_to_mbs(wcs)), _wcs(wcs) {}
	InteropString(const char *mbs) : _mbs(mbs), _wcs(mbs_to_wcs(mbs)) {}
	InteropString(const wchar_t *wcs) : _mbs(wcs_to_mbs(wcs)), _wcs(wcs) {}

	InteropString& operator=(InteropString const&) = default;
	InteropString& operator=(std::string const& mbs)
	{
		_mbs = mbs;
		_wcs = mbs_to_wcs(mbs);
		return *this;
	}
	InteropString& operator=(std::wstring const& wcs)
	{
		_mbs = wcs_to_mbs(wcs);
		_wcs = wcs;
		return *this;
	}

	operator std::string() const	{ return _mbs; }
	operator std::wstring() const	{ return _wcs; }

	InteropString& operator+=(std::string const& mbs)
	{
		_mbs += mbs;
		_wcs += mbs_to_wcs(mbs);
		return *this;
	}
	InteropString& operator+=(std::wstring const& wcs)
	{
		_mbs += wcs_to_mbs(wcs);
		_wcs += wcs;
		return *this;
	}

	std::string const&	get_mbs() const noexcept	{ return _mbs; }
	std::wstring const&	get_wcs() const noexcept	{ return _wcs; }

	static std::string	wcs_to_mbs(std::wstring const& wcs);
	static std::wstring	mbs_to_wcs(std::string const& wcs);
};

InteropString	operator+(InteropString const& lhs, std::string const& rhs);
InteropString	operator+(InteropString const& lhs, std::wstring const& rhs);
