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
static void _throttle_line(float throttle, ImVec4 AB_colour, v2::AModule& module,
	bool enable_AB_button)
{
	using std::min, std::max;
	static bool invert = false;

	// Normalize throttle and axis position to [0, 1] range
	if (invert)
		throttle = throttle / UINT16_MAX;
	else
		throttle = 1 - throttle / UINT16_MAX;
	const double detent = module.get_detent() / 100;

	if (throttle > detent)    // Colour bar green if in AB
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, AB_colour);
	TextCentered("Throttle");
	ImGui::ProgressBar(throttle, { 0, 0 }, throttle > detent ? "AB" : "");
	if (throttle > detent)    // Pop green colour from Style stack if it was pushed
		ImGui::PopStyleColor();

	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

	if (!enable_AB_button)
		ImGui::BeginDisabled();
	if (ImGui::Button("Set AB detent"))
	{
		throttle = min(max(throttle, 0.0f), 100.0f);
		module.set_detent(throttle * 100);
		std::clog << "New detend set at " << throttle << std::endl;
	}

	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

	ImGui::Checkbox("Invert", &invert);
	if (!enable_AB_button)
		ImGui::EndDisabled();
}

[[nodiscard]] std::string	lua_testing_shit();

void    render_main_window(ImGuiIO &io, bool &show_demo_window)
{
	static ImVec4           AB_colour = { RGB_TO_NORMED_FLOAT(32, 192, 32), 1.0f }; // strictly useless, just for debugging

	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos({ 0, 0 });

	constexpr ImGuiWindowFlags    flags = ImGuiWindowFlags_NoResize
										| ImGuiWindowFlags_NoMove
										| ImGuiWindowFlags_NoDecoration;

	ImGui::GetStyle().WindowBorderSize = 0;

	ImGui::Begin("main", nullptr, flags);

	auto [DCS_install_path, install_exists]	= DCS_install_path_field();
	auto [DCS_saved_games, sg_exists]		= DCS_Saved_Games_path_field(DCS_install_path);

	v2::AModule *module = nullptr;
	if (install_exists && sg_exists)
		module = selected_module_combo(DCS_install_path, DCS_saved_games);

	ImGui::Separator();

	auto [axis_value, valid_axis_value] = peripheral_and_axis_combo();

	if (module)
	{
		_throttle_line(axis_value, AB_colour, *module,
			module->is_enabled() && valid_axis_value);
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
