#include "DCS_AB_detent_editor.h"

using std::min, std::max;

static float	calc_bar_width(std::string_view checkbox_label)
{
	ImVec2 text_size = ImGui::CalcTextSize(checkbox_label.data());
	return ImGui::GetContentRegionAvail().x - text_size.x;
}

static float	calc_checkbox_pos_y(int bar_height)
{
	ImVec2 text_size = ImGui::CalcTextSize("");
	assert(bar_height >= text_size.y);	// Else it's ugly
	return ImGui::GetCursorPosY() - text_size.y - (bar_height / 2.0f);
}

// Render throttle progress bar, Set AB button, and invert checkbox
// TODO: layout that doesn't suck ass
void	throttle_block(float axis_value, ImVec4 AB_colour, AModule& module,
	bool enable_buttons)
{
	static bool invert = false;
	const float	detent = module.get_detent();
	const float	progress_bar = invert ? (axis_value / UINT16_MAX) : (1 - axis_value / UINT16_MAX);
	const float throttle_pos = min(max(progress_bar * 100, 0.0f), 100.0f);
	std::string	bar_text = std::format("{:02.1f}% {:2s}", throttle_pos, throttle_pos > detent ? "AB" : "");

	if (throttle_pos > detent)    // Colour bar green if in AB
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, AB_colour);
	TextCentered("Throttle");
	float bar_width = calc_bar_width("    Invert axis");
	ImGui::ProgressBar(progress_bar, { bar_width, 30 }, bar_text.c_str());
	float checkbox_y = calc_checkbox_pos_y(30);
	if (throttle_pos > detent)    // Pop green colour from Style stack if it was pushed
		ImGui::PopStyleColor();

	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::SetCursorPosY(checkbox_y);
	ImGui::Checkbox("Invert axis", &invert);

	if (!enable_buttons)
		ImGui::BeginDisabled();
	if (ImGui::Button("Set AB detent"))
	{
		module.set_detent(throttle_pos);
		std::clog << "New detend set at " << throttle_pos << std::endl;
	}

	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	ImGui::Button("Reset to default");

	if (!enable_buttons)
		ImGui::EndDisabled();
}
