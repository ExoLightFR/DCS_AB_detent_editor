#include "DCS_AB_detent_editor.h"
#include "AModule2.hpp"
#include <memory>
#include <vector>

using std::vector, std::unique_ptr;

static void	imgui_selectable_style_prefix(v2::AModule const& mod)
{
	if (!mod.is_installed())
		ImGui::BeginDisabled();
	if (!mod.is_enabled() && mod.is_installed())
		ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
}

static void	imgui_selectable_style_postfix(v2::AModule const& mod)
{
	if (!mod.is_enabled() && mod.is_installed())
	{
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Error: module is installed, but detent data could not be found in files.");
	}
	if (!mod.is_installed())
		ImGui::EndDisabled();
}

static bool	check_changed_DCS_paths(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	static std::string	prev_install_path	= DCS_install_path;
	static std::string	prev_sg_path		= DCS_saved_games_path;

	if (prev_install_path != DCS_install_path.get_mbs() || prev_sg_path != DCS_saved_games_path.get_mbs())
	{
		prev_install_path = DCS_install_path;
		prev_sg_path = DCS_saved_games_path;
		return true;
	}
	else
		return false;
}

vector<unique_ptr<v2::AModule>>	get_modules_vector(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	vector<unique_ptr<v2::AModule>>	res;

	for (auto& i : v2::AModule::supported_modules)
		res.push_back(v2::AModule::get_module(i, DCS_install_path, DCS_saved_games_path));

	return res;
}

v2::AModule	*get_first_available_module(vector<unique_ptr<v2::AModule>> const& modules)
{
	for (auto &mod : modules)
	{
		if (mod->is_installed() && mod->is_enabled())
			return mod.get();
	}
	return nullptr;
}

v2::AModule	*selected_module_combo(InteropString const& DCS_install_path,
	InteropString const& DCS_saved_games_path)
{
	static vector<unique_ptr<v2::AModule>>	modules = get_modules_vector(DCS_install_path, DCS_saved_games_path);
	static v2::AModule *selected = get_first_available_module(modules);

	if (check_changed_DCS_paths(DCS_install_path, DCS_saved_games_path))
	{
		modules = get_modules_vector(DCS_install_path, DCS_saved_games_path);
		selected = get_first_available_module(modules);
	}

	std::string_view preview = selected ? selected->name().c_str() : "No compatible modules found!";

	if (ImGui::BeginCombo("Modules", preview.data()))
	{
		for (auto &mod : modules)
		{
			imgui_selectable_style_prefix(*mod);

			if (ImGui::Selectable(mod->name().c_str(), selected == mod.get()) && mod->is_enabled())
				selected = mod.get();
			if (selected == mod.get())
				ImGui::SetItemDefaultFocus();

			imgui_selectable_style_postfix(*mod);
		}
		ImGui::EndCombo();
	}
	return selected;
}
