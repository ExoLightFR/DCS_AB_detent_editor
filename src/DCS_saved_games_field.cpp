#include <string>
#include <filesystem>
#include "imgui.h"
#include "imgui_stdlib.h"
#include "DCS_AB_detent_editor.h"
#include "PerformanceTimer.hpp"

// ImGuiInputTextCallbackData* data

namespace fs = std::filesystem;

// Tries to get different Windows Registry entries to find a DCS install path
// Returns an empty string if nothing is found.
static bool check_DCS_installed()
{
	constexpr std::wstring_view try_registries[] = {
		L"SOFTWARE\\Eagle Dynamics\\DCS World",
		L"Software\\Eagle Dynamics\\DCS World",
		L"SOFTWARE\\Eagle Dynamics\\DCS World OpenBeta",
		L"Software\\Eagle Dynamics\\DCS World OpenBeta"
	};
	std::wstring    DCS_ws_path;

	for (auto& key : try_registries)
	{
		if (RegGetString(HKEY_CURRENT_USER, key.data(), L"Path", DCS_ws_path) == 0)
		{
			if (fs::exists(DCS_ws_path) && fs::is_directory(DCS_ws_path))
				return true;
		}
	}
	return false;
}

static std::string	try_get_DCS_Saved_Games_path()
{
	std::string_view	try_directories[] = {
		"\\DCS.openbeta",
		"\\DCS",
	};
	const std::string saved_games = get_saved_games_path();

	for (auto &dir : try_directories)
	{
		std::string try_dir = saved_games + dir.data();
		if (fs::exists(try_dir) && fs::is_directory(try_dir))
			return try_dir;
	}
	return "";
}

InteropString DCS_Saved_Games_field()
{
	static bool			DCS_install_detected = check_DCS_installed();
	static std::string	text_buf = try_get_DCS_Saved_Games_path();

	bool dir_exists = fs::exists(text_buf) && fs::is_directory(text_buf);

	if (!DCS_install_detected && dir_exists)
	{
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f),
			R"(Info: Your DCS installation was not found, but the software will continue to work
as usual and change settings in the DCS Saved Games folder.)"
		);
	}

	if (!dir_exists)
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.0f, 0.0f, 1.0f });

	ImGui::InputTextWithHint(
		"DCS Saved Games directory",
		R"(i.e. C:\Users\You\Saved Games\DCS.openbeta)",
		&text_buf);

	if (!dir_exists)
	{
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Directory doesn't exist");
	}
	return text_buf;
}
