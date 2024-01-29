#include <string>
#include <string_view>
#include <filesystem>
#include "imgui.h"
#include "imgui_stdlib.h"
#include "DCS_AB_detent_editor.h"
#include "PerformanceTimer.hpp"

// ImGuiInputTextCallbackData* data

using namespace std::string_view_literals;
namespace fs = std::filesystem;

// Tries to get different Windows Registry entries to find a DCS install path
// Returns an empty string if nothing is found.
static InteropString try_get_DCS_install_path()
{
	constexpr std::wstring_view try_registries[] = {
		L"SOFTWARE\\Eagle Dynamics\\DCS World OpenBeta",
		L"Software\\Eagle Dynamics\\DCS World OpenBeta",
		L"SOFTWARE\\Eagle Dynamics\\DCS World",
		L"Software\\Eagle Dynamics\\DCS World",
	};
	std::wstring    DCS_ws_path;

	for (auto& key : try_registries)
	{
		if (RegGetString(HKEY_CURRENT_USER, key.data(), L"Path", DCS_ws_path) == 0)
		{
			if (fs::exists(DCS_ws_path) && fs::is_directory(DCS_ws_path))
				return DCS_ws_path;
		}
	}
	return "";
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

std::pair<InteropString, bool>	DCS_Saved_Games_path_field(InteropString const& DCS_install_path)
{
	static bool			DCS_install_detected = DCS_install_path.get_mbs().size();
	static std::string	text_buf = try_get_DCS_Saved_Games_path();

	bool dir_exists = fs::exists(text_buf) && fs::is_directory(text_buf);

	if (!dir_exists)
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.0f, 0.0f, 1.0f });

	ImGui::InputTextWithHint(
		"DCS Saved Games directory",
		R"(i.e. C:\Users\You\Saved Games\DCS.openbeta)",
		&text_buf);
	text_buf = trim_str(text_buf);

	if (!dir_exists)
	{
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Directory doesn't exist");
	}
	return { text_buf, dir_exists };
}

std::pair<InteropString, bool>	DCS_install_path_field()
{
	static std::string	text_buf = try_get_DCS_install_path();

	bool dir_exists = fs::exists(text_buf) && fs::is_directory(text_buf);

	if (!dir_exists)
		ImGui::PushStyleColor(ImGuiCol_Text, { 0.9f, 0.0f, 0.0f, 1.0f });

	ImGui::InputTextWithHint(
		"DCS installation path",
		R"(i.e. C:\Program Files\Eagle Dynamics\DCS World)",
		&text_buf);
	text_buf = trim_str(text_buf);

	if (!dir_exists)
	{
		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered())
			ImGui::SetTooltip("Directory doesn't exist");
	}
	return { text_buf, dir_exists };
}
