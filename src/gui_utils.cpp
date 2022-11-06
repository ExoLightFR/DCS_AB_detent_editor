#include <DCS_AB_detent_editor.h>
#include <imgui_internal.h>

void TextCentered(const char *text)
{
    auto windowWidth = ImGui::GetWindowSize().x;
    auto textWidth = ImGui::CalcTextSize(text).x;

    ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
    ImGui::Text(text);
}

bool ButtonCentered(const char *label, float alignment)
{
    ImGuiStyle &style = ImGui::GetStyle();

    float size = ImGui::CalcTextSize(label).x + style.FramePadding.x * 2.0f;
    float avail = ImGui::GetContentRegionAvail().x;

    float off = (avail - size) * alignment;
    if (off > 0.0f)
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

    return ImGui::Button(label);
}

void    error_popup(std::string const &error_msg, bool *error_popup)
{
    ImGui::ErrorCheckEndFrameRecover(NULL);

    constexpr ImGuiWindowFlags    flags = ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove
        | ImGuiWindowFlags_NoDecoration
        | ImGuiWindowFlags_AlwaysAutoResize;

    ImGui::OpenPopup("ERROR");
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("ERROR", NULL, flags))
    {
        ImGui::Text("Error: %s", error_msg.c_str());
        if (ButtonCentered("OK"))
            *error_popup = false;
        ImGui::EndPopup();
    }
}
