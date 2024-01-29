#pragma once

#ifndef UNICODE
# define UNICODE
#endif

#include <AModule.h>

#include <string>
#include <memory>

#include <Windows.h>

#include <imgui.h>

#define RGB_TO_NORMED_FLOAT(x, y, z)    x / 255.0f, y / 255.0f, z / 255.0f
#define ARRAY_LEN(x)                    sizeof(x) / sizeof(x[0])

std::pair<InteropString, bool>	DCS_Saved_Games_path_field(InteropString const& DCS_install_path);
std::pair<InteropString, bool>	DCS_install_path_field();

auto    selected_module_Combo(std::string const &DCS_path) -> std::shared_ptr<AModule>;
void    render_main_window(ImGuiIO &io, bool &show_demo_window);

std::pair<uint16_t, bool>	peripheral_block();

void    error_popup(std::string const &error_msg, bool *error_popup);

// std::string     wcstring_to_mbstring(std::wstring const &src);
// std::wstring    mbstring_to_wcstring(std::string const &src);

void    TextCentered(const char *text);
bool    ButtonCentered(const char *label, float alignment = 0.5f);

/* UTILS */
InteropString	get_saved_games_path();
std::wstring    RegGetString(HKEY hKey, const std::wstring &subKey, const std::wstring &value);
int             RegGetString(HKEY hKey, const std::wstring &subKey, const std::wstring &value,
                    std::wstring &outstr);
std::string		trim_str(std::string_view str);
