#pragma once

#include <string>
#include <array>
#include <string_view>
#include <memory>
#include "InteropString.hpp"

namespace v2 {

	class AModule
	{
	protected:
		std::string		_name;
		bool			_installed = false;
		bool			_enabled = false;
		float			_detent = 1.01f;	// When not set, detent is > 1, so we never show AB until we know detent
		InteropString	_conf_file;

	public:
		AModule() = default;
		virtual ~AModule() = default;

		std::string const&	name() const			{ return _name; }
		[[nodiscard]] bool	is_installed() const	{ return _installed; }
		[[nodiscard]] bool	is_enabled() const		{ return _enabled; }

		virtual float	get_detent() const	{ return _detent; }
		virtual bool	set_detent(float value) = 0;

		static std::unique_ptr<AModule>
			get_module(std::string_view module_name,
				InteropString const& DCS_install_path,
				InteropString const& DCS_saved_games_path);

	public:
		inline static const std::array<std::string_view, 3>	supported_modules = {
			"M-2000C",
			"Mirage-F1",
			"F-15E",
		};
	};

	class ModuleM2000C : public AModule
	{
		inline static constexpr std::string_view	DISPLAY_NAME = "Mirage 2000C";
		inline static constexpr std::string_view	MODULE_NAME = "M-2000C";

		[[nodiscard]] bool	update_detent_from_conf_file();
	public:
		ModuleM2000C(InteropString const& DCS_install_path,
			InteropString const& DCS_saved_games_path);
		~ModuleM2000C() = default;

		[[nodiscard]] bool	set_detent(float val_0_100) override final;
		float	get_detent() const override final		{ return _detent; }
	};

	class ModuleMirageF1 : public AModule
	{
		inline static constexpr std::string_view	DISPLAY_NAME = "Mirage F1 (all variants)";
		inline static constexpr std::string_view	MODULE_NAME = "Mirage-F1";

		[[nodiscard]] bool	update_detent_from_conf_file();
	public:
		ModuleMirageF1(InteropString const& DCS_install_path,
			InteropString const& DCS_saved_games_path);
		~ModuleMirageF1() = default;

		[[nodiscard]] bool	set_detent(float val_0_100) override final;
		float	get_detent() const override final		{ return _detent; }
	};

	class ModuleF15E : public AModule
	{
		inline static constexpr std::string_view	DISPLAY_NAME = "F-15E Strike Eagle";
		inline static constexpr std::string_view	MODULE_NAME = "F-15E";

		[[nodiscard]] bool	update_detent_from_conf_file();
	public:
		ModuleF15E(InteropString const& DCS_install_path,
			InteropString const& DCS_saved_games_path);
		~ModuleF15E() = default;

		[[nodiscard]] bool	set_detent(float val_0_100) override final;
		float	get_detent() const override final		{ return _detent; }
	};

}	// namespace v2
