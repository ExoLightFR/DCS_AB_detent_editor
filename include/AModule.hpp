#pragma once

class AModule
{
protected:
	std::string		_name;
	bool			_installed = false;
	bool			_enabled = false;
	float			_detent = 1.01f;	// When not set, detent is > 1, so we never show AB until we know detent
	InteropString	_conf_file;

	virtual [[nodiscard]] bool	safe_open_lua_context(sol::state& lua) const;

public:
	// Rust copium is fucking REAAAAAAL
	using result_t = tl::expected<void, std::string>;

	AModule() = default;
	virtual ~AModule() = default;

	std::string const&	name() const			{ return _name; }
	[[nodiscard]] bool	is_installed() const	{ return _installed; }
	[[nodiscard]] bool	is_enabled() const		{ return _enabled; }

	virtual float		get_detent() const	{ return _detent; }
	virtual result_t	set_detent(float value) = 0;
	virtual result_t	reset_detent() = 0;

	static std::unique_ptr<AModule>
		get_module(std::string_view module_name,
			InteropString const& DCS_install_path,
			InteropString const& DCS_saved_games_path);

public:
	inline static const std::array<std::string_view, 5>	supported_modules = {
		"M-2000C",
		"Mirage-F1",
		"F-15E",
		"F-4E",
		"F-15C"
	};
};

class ModuleM2000C : public AModule
{
	inline static constexpr std::string_view	DISPLAY_NAME = "Mirage 2000C";
	inline static constexpr std::string_view	MODULE_NAME = "M-2000C";
	inline static constexpr float	DEFAULT_DETENT = 90.0f;

	[[nodiscard]] bool	update_detent_from_conf_file();

public:
	ModuleM2000C(InteropString const& DCS_install_path, InteropString const& DCS_saved_games_path);
	~ModuleM2000C() = default;

	[[nodiscard]] result_t	set_detent(float val_0_100) override final;
	float	get_detent() const override final			{ return _detent; }
	virtual result_t	reset_detent() override final	{ return set_detent(DEFAULT_DETENT); }
};

class ModuleMirageF1 : public AModule
{
	inline static constexpr std::string_view	DISPLAY_NAME = "Mirage F1 (all variants)";
	inline static constexpr std::string_view	MODULE_NAME = "Mirage-F1";
	inline static constexpr float	DEFAULT_DETENT = 59.0f;

	[[nodiscard]] bool	update_detent_from_conf_file();
public:
	ModuleMirageF1(InteropString const& DCS_install_path, InteropString const& DCS_saved_games_path);
	~ModuleMirageF1() = default;

	[[nodiscard]] result_t	set_detent(float val_0_100) override final;
	float	get_detent() const override final			{ return _detent; }
	virtual result_t	reset_detent() override final	{ return set_detent(DEFAULT_DETENT); }
};

class ModuleF15E : public AModule
{
	inline static constexpr std::string_view	DISPLAY_NAME = "F-15E Strike Eagle";
	inline static constexpr std::string_view	MODULE_NAME = "F-15E";
	inline static constexpr float	DEFAULT_DETENT = 75.0f;

	[[nodiscard]] bool	update_detent_from_conf_file();
public:
	ModuleF15E(InteropString const& DCS_install_path, InteropString const& DCS_saved_games_path);
	~ModuleF15E() = default;

	[[nodiscard]] result_t	set_detent(float val_0_100) override final;
	float	get_detent() const override final			{ return _detent; }
	virtual result_t	reset_detent() override final	{ return set_detent(DEFAULT_DETENT); }
};

class ModuleF4E : public AModule
{
	inline static constexpr std::string_view	DISPLAY_NAME = "F-4E (two more weeks)";
	inline static constexpr std::string_view	MODULE_NAME = "F-4E";
	inline static constexpr float	DEFAULT_DETENT = 123.45f;

	// [[nodiscard]] bool	update_detent_from_conf_file();
public:
	ModuleF4E(InteropString const& DCS_install_path,
		InteropString const& DCS_saved_games_path)
	{
		_name = DISPLAY_NAME;
	}
	~ModuleF4E() = default;

	[[nodiscard]] result_t	set_detent(float val_0_100) override final { return tl::unexpected("what the fuck?"); }
	float	get_detent() const override final			{ return _detent; }
	virtual result_t	reset_detent() override final	{ return set_detent(DEFAULT_DETENT); }
};

class ModuleF15C : public AModule
{
	inline static constexpr std::string_view	DISPLAY_NAME = "F-15C Eagle";
	inline static constexpr std::string_view	MODULE_NAME = "F-15C";
	inline static constexpr float	DEFAULT_DETENT = 20.0f;

public:
	ModuleF15C(InteropString const& DCS_install_path, InteropString const& DCS_saved_games_path);
	~ModuleF15C() = default;

	[[nodiscard]] result_t	set_detent(float val_0_100) override final;
	float	get_detent() const override final			{ return _detent; }
	virtual result_t	reset_detent() override final	{ return set_detent(DEFAULT_DETENT); }
};
