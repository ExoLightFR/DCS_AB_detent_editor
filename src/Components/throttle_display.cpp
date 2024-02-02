#include "DCS_AB_detent_editor.h"

using std::clamp;
using namespace std::string_view_literals;

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

static void	detent_buttons(float avail_width, AModule::result_t &res,
	std::function<AModule::result_t()> a, std::function<AModule::result_t()> b)
{
	// float avail_width = ImGui::GetContentRegionAvail().x;
	float padding = ImGui::GetStyle().FramePadding.x;
	float buttons_w = (avail_width - padding) / 2;

	if (ImGui::Button("Set AB detent", { buttons_w, 0 }))
		res = a();
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	if (ImGui::Button("Reset to default", { buttons_w, 0 }))
		res = b();
}

static void    module_operation_error_popup(AModule::result_t &res)
{
	constexpr ImGuiWindowFlags    flags = ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		// | ImGuiWindowFlags_NoDecoration
		| ImGuiWindowFlags_AlwaysAutoResize;

	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, IM_COL32(255, 0, 0, 255));
	ImGui::OpenPopup("ERROR");
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
	if (ImGui::BeginPopupModal("ERROR", NULL, flags))
	{
		ImGui::Text(res.error().data());
		if (ButtonCentered("OK"))
			res = AModule::result_t();
		ImGui::EndPopup();
	}
	ImGui::PopStyleColor();
}


// Render throttle progress bar, Set AB button, and invert checkbox
// TODO: layout that doesn't suck ass
void	throttle_block(float axis_value, ImVec4 AB_colour, AModule& module,
	bool enable_buttons)
{
	static bool		invert		= false;
	static float	bar_width	= 0.0f;
	static AModule::result_t res;

	const float	detent = module.get_detent();
	const float	progress_bar = invert ? (axis_value / UINT16_MAX) : (1 - axis_value / UINT16_MAX);
	const float throttle_pos = clamp(progress_bar * 100, 0.0f, 100.0f);
	std::string	bar_text = std::format("{:02.1f}% {:2s}", throttle_pos, throttle_pos > detent ? "AB" : "");

	if (throttle_pos > detent)
		ImGui::PushStyleColor(ImGuiCol_PlotHistogram, AB_colour);
	// TextCentered("Throttle");
	ImGui::ProgressBar(progress_bar, { 0, 0 }, bar_text.c_str());
	if (throttle_pos > detent)
		ImGui::PopStyleColor();

	bar_width = ImGui::GetItemRectSize().x;
	ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
	if (!enable_buttons)
		ImGui::BeginDisabled();
	ImGui::Checkbox("Invert axis", &invert);

	detent_buttons(bar_width, res,
		[&] { return module.set_detent(throttle_pos); },
		[&] { return module.reset_detent(); }
	);
	if (!enable_buttons)
		ImGui::EndDisabled();

	if (!res)
		module_operation_error_popup(res);
}
