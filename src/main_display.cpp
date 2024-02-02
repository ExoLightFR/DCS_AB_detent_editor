#ifndef UNICODE
# define UNICODE
#endif

#include <iostream>
#include <locale>

#include <DCS_AB_detent_editor.h>

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
	AModule *module = nullptr;
	if (install_exists && sg_exists)
		module = selected_module_combo(DCS_install_path, DCS_saved_games);

	ImGui::Separator();

	auto [axis_value, valid_axis_value] = peripheral_and_axis_combo();
	if (module)
		throttle_block(axis_value, AB_colour, *module, module->is_enabled() && valid_axis_value);

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
