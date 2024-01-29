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
	using std::min, std::max;

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

[[nodiscard]] std::string	lua_testing_shit();
void	testos(InteropString const& DCS_install_path, InteropString const& DCS_saved_games_path);

void    render_main_window(ImGuiIO &io, bool &show_demo_window)
{
	static float            throttle = 0.0f;
	static ImVec4           AB_colour = { RGB_TO_NORMED_FLOAT(32, 192, 32), 1.0f }; // strictly useless, just for debugging
	static bool             invert = false;

	ImGui::SetNextWindowSize(io.DisplaySize);
	ImGui::SetNextWindowPos({ 0, 0 });

	constexpr ImGuiWindowFlags    flags = ImGuiWindowFlags_NoResize
										| ImGuiWindowFlags_NoMove
										| ImGuiWindowFlags_NoDecoration;

	ImGui::GetStyle().WindowBorderSize = 0;

	ImGui::Begin("main", nullptr, flags);

	auto [DCS_install_path, install_exists]	= DCS_install_path_field();
	auto [DCS_saved_games, sg_exists]		= DCS_Saved_Games_path_field(DCS_install_path);

	if (install_exists && sg_exists)
		testos(DCS_install_path, DCS_saved_games);

	// std::string test = lua_testing_shit();

	// std::shared_ptr<AModule> module = selected_module_Combo(DCS_path);

	ImGui::Separator();

	auto [axis_value, enable] = peripheral_block();
	throttle = 1 - float(axis_value) / UINT16_MAX;
	if (invert)
		throttle = 1 - throttle;

	// if (module->is_installed())
	// {
	// 	_throttle_line(throttle, AB_colour, &invert, module,
	// 		module->is_installed() && enable);
	// }
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
