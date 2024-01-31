ImGuiStyle	get_custom_imgui_style()
{
	ImGuiStyle	style = ImGui::GetStyle();

	style.FrameRounding = 3;
	// style.ItemSpacing.y = 6;
	style.GrabRounding = 4;

	style.ScaleAllSizes(1.5f);
	ImGui::StyleColorsDark(&style);

	return style;
}

void SetupImGuiStyle()
{
	// Bootstrap Dark style by Madam-Herta from ImThemes
	ImGuiStyle& style = ImGui::GetStyle();

	style.Alpha = 1.0f;
	style.DisabledAlpha = 0.5f;
	style.WindowPadding = ImVec2(11.69999980926514f, 6.0f);
	style.WindowRounding = 3.299999952316284f;
	style.WindowBorderSize = 0.0f;
	style.WindowMinSize = ImVec2(20.0f, 20.0f);
	style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
	style.WindowMenuButtonPosition = ImGuiDir_Left;
	style.ChildRounding = 0.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 0.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(20.0f, 9.899999618530273f);
	style.FrameRounding = 0.0f;
	style.FrameBorderSize = 0.0f;
	style.ItemSpacing = ImVec2(8.0f, 4.0f);
	style.ItemInnerSpacing = ImVec2(4.0f, 4.0f);
	style.CellPadding = ImVec2(4.0f, 2.0f);
	style.IndentSpacing = 21.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 14.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 10.0f;
	style.GrabRounding = 0.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 0.0f;
	style.TabMinWidthForCloseButton = 0.0f;
	style.ColorButtonPosition = ImGuiDir_Right;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.5843137502670288f, 0.5960784554481506f, 0.615686297416687f, 1.0f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.062745101749897f, 0.06666667014360428f, 0.08627451211214066f, 1.0f);
	style.Colors[ImGuiCol_ChildBg] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.1098039224743843f, 0.1137254908680916f, 0.1333333402872086f, 1.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.1098039224743843f, 0.1137254908680916f, 0.1333333402872086f, 1.0f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.062745101749897f, 0.06666667014360428f, 0.08627451211214066f, 1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.05882352963089943f, 0.529411792755127f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.05882352963089943f, 0.529411792755127f, 0.9764705896377563f, 0.0f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05098039284348488f, 0.062745101749897f, 1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.04313725605607033f, 0.0470588244497776f, 0.05882352963089943f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1098039224743843f, 0.1137254908680916f, 0.1333333402872086f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1450980454683304f, 0.1490196138620377f, 0.1843137294054031f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.4862745106220245f, 0.4862745106220245f, 0.4862745106220245f, 1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 1.0f, 1.0f, 0.2274678349494934f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.8196078538894653f, 0.8196078538894653f, 0.8196078538894653f, 0.3304721117019653f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.2274509817361832f, 0.4431372582912445f, 0.7568627595901489f, 1.0f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.2078431397676468f, 0.4705882370471954f, 0.8509804010391235f, 1.0f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.05882352963089943f, 0.529411792755127f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3100000023841858f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_Separator] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 0.6200000047683716f);
	style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1372549086809158f, 0.4392156898975372f, 0.800000011920929f, 0.7799999713897705f);
	style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1372549086809158f, 0.4392156898975372f, 0.800000011920929f, 1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.3490196168422699f, 0.3490196168422699f, 0.3490196168422699f, 0.1700000017881393f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
	style.Colors[ImGuiCol_Tab] = ImVec4(0.0f, 0.4745098054409027f, 1.0f, 0.9309999942779541f);
	style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_TabActive] = ImVec4(0.2078431397676468f, 0.2078431397676468f, 0.2078431397676468f, 1.0f);
	style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.9176470637321472f, 0.9254902005195618f, 0.9333333373069763f, 0.9861999750137329f);
	style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.7411764860153198f, 0.8196078538894653f, 0.9137254953384399f, 1.0f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.3882353007793427f, 0.3882353007793427f, 0.3882353007793427f, 1.0f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.4470588266849518f, 0.0f, 1.0f);
	style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.7764706015586853f, 0.8666666746139526f, 0.9764705896377563f, 1.0f);
	style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.5686274766921997f, 0.5686274766921997f, 0.6392157077789307f, 1.0f);
	style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.6784313917160034f, 0.6784313917160034f, 0.7372549176216125f, 1.0f);
	style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.2980392277240753f, 0.2980392277240753f, 0.2980392277240753f, 0.09000000357627869f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
	style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
	style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
	style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.6980392336845398f, 0.6980392336845398f, 0.6980392336845398f, 0.699999988079071f);
	style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f);
	style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2000000029802322f, 0.2000000029802322f, 0.2000000029802322f, 0.3499999940395355f);
}
