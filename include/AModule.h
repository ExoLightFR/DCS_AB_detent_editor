#pragma once

// Project headers
#include <DCS_AB_detent_editor.h>

// C++ standard headers
#include <string>
#include <memory>
#include <fstream>
#include <sstream>
#include <array>
#include <algorithm>
#include <string_view>

// Other headers (OS, 3rd party, etc)
#include <Windows.h>
#include <shlobj.h>

// Debug
#include <iostream>

class AModule
{
	protected:
		std::string _name;
		bool        _is_installed = false;
        float       _detent = 1.01f;

	public:
		static std::shared_ptr<AModule> getModule(std::string const& DCS_path, std::wstring const& module_name);
        static std::shared_ptr<AModule> getModule(std::string const& DCS_path, std::string const &module_name);
        static const std::array<const char*, 3> supported_modules;

        AModule(std::string name = "") : _name(name) {}

		virtual int     set_detent(float pos) = 0;
        virtual float   get_detent() const = 0;

		std::string const&  name() const;
		bool                is_installed() const;
};

// TODO: read detent from file on construction
class Module_M2000C : public AModule
{
	private:
		constexpr static const WCHAR    *_module_name = L"M-2000C";
        constexpr static const char     *_display_name = "Mirage 2000C";
        typedef std::pair<std::string::iterator, std::string::iterator>     iterator_pair;

        iterator_pair   _get_detent_from_file(std::string &file_content) const;

	public:
        Module_M2000C(std::wstring const& DCS_ws_path);
		int     set_detent(float pos);
        float   get_detent() const;
};

class Module_MirageF1 : public AModule
{
    private:
        constexpr static const WCHAR    *_module_name = L"Mirage-F1";
        constexpr static const char     *_display_name = "Mirage F1-CE";
        typedef std::pair<std::string::iterator, std::string::iterator>     iterator_pair;

        iterator_pair   _get_detent_from_file(std::string &file_content) const;

	public:
        Module_MirageF1(std::wstring const& DCS_ws_path);
		int     set_detent(float pos);
        float   get_detent() const;
};

class Module_NotFound : public AModule
{
    public:
        Module_NotFound();
        int     set_detent(float pos);
        float   get_detent() const;
};
