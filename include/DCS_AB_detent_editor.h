#pragma once

#ifndef UNICODE
# define UNICODE
#endif

#include "pch.h"

#define RGB_TO_NORMED_FLOAT(x, y, z)    x / 255.0f, y / 255.0f, z / 255.0f
#define ARRAY_LEN(x)                    sizeof(x) / sizeof(x[0])

extern const char Roboto_Medium_compressed_data_base85[148145 + 1];

ImGuiStyle	get_custom_imgui_style();

std::pair<InteropString, bool>	DCS_Saved_Games_path_field(InteropString const& DCS_install_path);
std::pair<InteropString, bool>	DCS_install_path_field();

void	throttle_block(float throttle, ImVec4 AB_colour, AModule& module,
			bool enable_buttons);
AModule	*selected_module_combo(InteropString const& DCS_install_path,
			InteropString const& DCS_saved_games_path);
void    render_main_window(ImGuiIO &io, bool &show_demo_window);

std::pair<uint16_t, bool>	peripheral_and_axis_combo();

void    error_popup(std::string const &error_msg, bool *error_popup);
void    TextCentered(const char *text);
bool    ButtonCentered(const char *label, float alignment = 0.5f);

/* UTILS */
InteropString	get_saved_games_path();
std::wstring    RegGetString(HKEY hKey, const std::wstring &subKey, const std::wstring &value);
int             RegGetString(HKEY hKey, const std::wstring &subKey, const std::wstring &value,
					std::wstring &outstr);
std::string		trim_str(std::string_view str);
