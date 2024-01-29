#include "DCS_AB_detent_editor.h"
#include "AModule2.hpp"
#include <memory>
#include <vector>

using std::vector, std::unique_ptr;

vector<unique_ptr<v2::AModule>>	get_modules_vector(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	vector<unique_ptr<v2::AModule>>	res;

	for (auto& i : v2::AModule::supported_modules)
		res.push_back(v2::AModule::get_module(i, DCS_install_path, DCS_saved_games_path));

	return res;
}

std::unique_ptr<v2::AModule>& selected_module_combo(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	static vector<unique_ptr<v2::AModule>>	modules = get_modules_vector(DCS_install_path, DCS_saved_games_path);

	if (ImGui::BeginCombo("Modules", "tg"))
	{
		// for (auto &module : modules)
		auto& module = modules[0];
		{
			if (!module->is_installed())
				ImGui::BeginDisabled();
			if (!module->is_enabled())
				ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));

			ImGui::Selectable(module->name().c_str());

			if (!module->is_enabled())
				ImGui::PopStyleColor();
			if (!module->is_installed())
				ImGui::EndDisabled();
		}
		ImGui::EndCombo();
	}
	return modules[0];
}

void	testos(InteropString const& DCS_install_path, InteropString const& DCS_saved_games_path)
{
	(void)selected_module_combo(DCS_install_path, DCS_saved_games_path);
}
