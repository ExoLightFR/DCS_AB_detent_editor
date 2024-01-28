#ifndef UNICODE
# define UNICODE
#endif

#include <DCS_AB_detent_editor.h>
#include <AModule.h>
#include "InteropString.hpp"

#include "imgui.h"
#include <imgui_stdlib.h>

#include <windows.h>

#include <iostream>
#include <array>
#include <vector>
#include <string_view>

#include <locale>

static float   _normalize_axis_pos(uint16_t axis_pos)
{
    constexpr int   range = UINT16_MAX;
    long test = axis_pos + (UINT16_MAX / 2);
    return static_cast<float>(test) / range;
}

// Render throttle progress bar, Set AB button, and invert checkbox
static void _throttle_line(float throttle, ImVec4 AB_colour, bool *invert, std::shared_ptr<AModule> module,
    bool enable_AB_button)
{
    if (throttle > module->get_detent())    // Colour bar green if in AB
        ImGui::PushStyleColor(ImGuiCol_PlotHistogram, AB_colour);
    TextCentered("Throttle");
    ImGui::ProgressBar(throttle, { 0, 0 }, throttle > module->get_detent() ? "AB" : "");
    if (throttle > module->get_detent())    // Pop green colour from Style stack if it was pushed
        ImGui::PopStyleColor();

    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

    if (!enable_AB_button)
        ImGui::BeginDisabled();
    if (ImGui::Button("Set AB detent"))
    {
        throttle = min(max(throttle, 0.0f), 100.0f);
        module->set_detent(throttle);
        std::clog << "New detend set at " << throttle << std::endl;
    }

    ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

    ImGui::Checkbox("Invert", invert);
    if (!enable_AB_button)
        ImGui::EndDisabled();
}

// Tries to get different Windows Registry entries to find a DCS install path
// Returns an empty string if nothing is found.
static std::string _try_get_DCS_path()
{
    constexpr const std::wstring_view try_registries[] = {
        L"SOFTWARE\\Eagle Dynamics\\DCS World",
        L"Software\\Eagle Dynamics\\DCS World",
        L"SOFTWARE\\Eagle Dynamics\\DCS World OpenBeta",
        L"Software\\Eagle Dynamics\\DCS World OpenBeta"
    };
    std::wstring    DCS_ws_path;

    for (auto &key : try_registries)
    {
        if (RegGetString(HKEY_CURRENT_USER, key.data(), L"Path", DCS_ws_path) == 0)
        {
            return wcstring_to_mbstring(DCS_ws_path);
        }
    }
    return "";
}

// FIXME: Unicode support doesn't work.
static void _DCS_path_line(InteropString &DCS_path)
{
	std::string	text_buf = DCS_path;

    DWORD attribs = GetFileAttributesW(DCS_path.get_wcs().c_str());
	bool dir_exists = attribs != INVALID_FILE_ATTRIBUTES && (attribs & FILE_ATTRIBUTE_DIRECTORY);

    if (!dir_exists)
        ImGui::PushStyleColor(ImGuiCol_Text, {0.9f, 0.0f, 0.0f, 1.0f});

    ImGui::InputTextWithHint("DCS installation path",
        "i.e. C:\\Program Files\\Eagle Dynamics\\DCS World",  &text_buf);
	DCS_path = text_buf;

    if (!dir_exists)
    {
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Directory doesn't exist");
    }
}

[[nodiscard]] std::string	lua_testing_shit();

void    render_main_window(ImGuiIO &io, bool &show_demo_window)
{
    static float            throttle = 0.0f;
    static ImVec4           AB_colour = { RGB_TO_NORMED_FLOAT(32, 192, 32), 1.0f }; // strictly useless, just for debugging
    static bool             invert = false;
    // static std::string      DCS_mbs_path = _try_get_DCS_path();
	static InteropString	DCS_path = _try_get_DCS_path();

    ImGui::SetNextWindowSize(io.DisplaySize);
    ImGui::SetNextWindowPos({ 0, 0 });

    constexpr ImGuiWindowFlags    flags = ImGuiWindowFlags_NoResize
                                        | ImGuiWindowFlags_NoMove
                                        | ImGuiWindowFlags_NoDecoration;

    ImGui::GetStyle().WindowBorderSize = 0;

    ImGui::Begin("main", nullptr, flags);

    _DCS_path_line(DCS_path);

	std::string test = lua_testing_shit();

    std::shared_ptr<AModule> module = selected_module_Combo(DCS_path);

    ImGui::Separator();

	auto [axis_value, enable] = peripheral_block();
	throttle = 1 - float(axis_value) / UINT16_MAX;
	if (invert)
		throttle = 1 - throttle;

	if (module->is_installed())
	{
		_throttle_line(throttle, AB_colour, &invert, module,
			module->is_installed() && enable);
	}
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

#ifdef _DEBUG
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Dev options"))
    {
        ImGui::Checkbox("Demo", &show_demo_window);
        ImGui::ColorEdit3("AB colour", (float *)&AB_colour); // Edit 3 floats representing a color

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
#endif
    ImGui::End();
}
