#include "DCS_AB_detent_editor.h"

using std::min, std::max;

// Render throttle progress bar, Set AB button, and invert checkbox
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
	ImGui::ProgressBar(progress_bar, { 0, 0 }, bar_text.c_str());
	if (throttle_pos > detent)    // Pop green colour from Style stack if it was pushed
		ImGui::PopStyleColor();

	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

	if (!enable_buttons)
		ImGui::BeginDisabled();
	if (ImGui::Button("Set AB detent"))
	{
		module.set_detent(throttle_pos);
		std::clog << "New detend set at " << throttle_pos << std::endl;
	}

	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);

	ImGui::Checkbox("Invert", &invert);
	if (!enable_buttons)
		ImGui::EndDisabled();
}
