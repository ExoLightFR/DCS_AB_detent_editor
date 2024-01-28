#pragma once

#include <string>
#include <array>
#include <string_view>
#include <memory>
#include "InteropString.hpp"

namespace v2 {

	class AModule
	{
	private:
		std::string	_name;
		bool		_installed = false;
		float		_detent = 1.01f;	// When not set, detent is > 1, so we never show AB until we know detent

	public:
		virtual ~AModule() = default;

		std::string const&	name() const			{ return _name; }
		[[nodiscard]] bool	is_installed() const	{ return _installed; }

		virtual float	get_detent() const = 0;
		virtual bool	set_detent(float value) = 0;

		static std::unique_ptr<AModule>
			get_module(InteropString const& DCS_path, std::string_view module_name);

	public:
		inline static const std::array<std::string_view, 3>	supported_modules = {
			"M-2000C",
			"Mirage-F1",
			"F-15E",
		};
	};

	class ModuleM2000C : public AModule
	{
		~ModuleM2000C() = default;

		float	get_detent() const override final		{ return 0; }
		bool	set_detent(float val) override final	{ return false; }
	};

	class ModuleMirageF1 : public AModule
	{
		~ModuleMirageF1() = default;

		float	get_detent() const override final		{ return 0; }
		bool	set_detent(float val) override final	{ return false; }
	};

	class ModuleF15E : public AModule
	{
		~ModuleF15E() = default;

		float	get_detent() const override final		{ return 0; }
		bool	set_detent(float val) override final	{ return false; }
	};

}	// namespace v2
