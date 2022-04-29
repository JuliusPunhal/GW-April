
#include "April/Utility/DefaultTheme.h"


namespace {

	static_assert(
		sizeof( ImGuiStyle ) == 1048,
		"ImGuiStyle has been changed, update custom theme!" );

	constexpr auto neutral_darker =  ImVec4{ .06f, .06f, .06f, .90f };
	constexpr auto neutral_dark =    ImVec4{ .30f, .30f, .30f, 1.0f };
	constexpr auto neutral_lighter = ImVec4{ .73f, .73f, .73f, 1.0f };

	constexpr auto accent_dark =     ImVec4{ .09f, .43f, .02f, 1.0f };
	constexpr auto accent_light =    ImVec4{ .12f, .58f, .11f, 1.0f };
	constexpr auto accent_lighter =  ImVec4{ .08f, .73f, .15f, 1.0f };

}


April::DefaultTheme::DefaultTheme(
	std::shared_ptr<Config const> config,
	std::shared_ptr<FontAtlas const> fonts )
	: config{ config }, fonts{ fonts }
{
}

void April::DefaultTheme::ApplyThemeToImGui()
{
	ImGui::GetIO().FontDefault = fonts->Get( config->font );
	ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly =
		config->move_from_titlebar;
	ImGui::GetStyle() = config->style;
}

auto April::DefaultTheme::Config::default_style() -> ImGuiStyle
{
	auto result = ImGuiStyle{};

	result.Alpha                      = 1.0f;
	result.DisabledAlpha              = 0.60f;
	result.WindowPadding              = { 8, 8 };
	result.WindowRounding             = 3.0f;
	result.WindowBorderSize           = 1.0f;
	result.WindowMinSize              = { 32, 32 };
	result.WindowTitleAlign           = { .0f, .5f };
	result.WindowMenuButtonPosition   = ImGuiDir_Left;
	result.ChildRounding              = 3.0f;
	result.ChildBorderSize            = 1.0f;
	result.PopupRounding              = 3.0f;
	result.PopupBorderSize            = 1.0f;
	result.FramePadding               = ImVec2(4,3);
	result.FrameRounding              = 2.0f;
	result.FrameBorderSize            = 0.0f;
	result.ItemSpacing                = { 8, 4 };
	result.ItemInnerSpacing           = { 4, 4 };
	result.CellPadding                = { 4, 2 };
	result.TouchExtraPadding          = { 0, 0 };
	result.IndentSpacing              = 28.0f;
	result.ColumnsMinSpacing          = 6.0f;
	result.ScrollbarSize              = 14.0f;
	result.ScrollbarRounding          = 2.0f;
	result.GrabMinSize                = 10.0f;
	result.GrabRounding               = 2.0f;
	result.LogSliderDeadzone          = 4.0f;
	result.TabRounding                = 2.0f;
	result.TabBorderSize              = 0.0f;
	result.TabMinWidthForCloseButton  = 0.0f;
	result.ColorButtonPosition        = ImGuiDir_Right;
	result.ButtonTextAlign            = { .5f, .5f };
	result.SelectableTextAlign        = { .0f, .0f };
	result.DisplayWindowPadding       = { 19, 19 };
	result.DisplaySafeAreaPadding     = {  3,  3 };
	result.MouseCursorScale           = 1.0f;
	result.AntiAliasedLines           = true;
	result.AntiAliasedLinesUseTex     = true;
	result.AntiAliasedFill            = true;
	result.CurveTessellationTol       = 1.25f;
	result.CircleTessellationMaxError = 0.30f;

	result.Colors[ImGuiCol_Text]                  = White();
	result.Colors[ImGuiCol_TextDisabled]          = Grey();
	result.Colors[ImGuiCol_WindowBg]              = neutral_darker;
	result.Colors[ImGuiCol_ChildBg]               = Invisible();
	result.Colors[ImGuiCol_PopupBg]               = neutral_darker;
	result.Colors[ImGuiCol_Border]                = Grey();
	result.Colors[ImGuiCol_BorderShadow]          = Invisible();
	result.Colors[ImGuiCol_FrameBg]               = neutral_dark;
	result.Colors[ImGuiCol_FrameBgHovered]        = Grey();
	result.Colors[ImGuiCol_FrameBgActive]         = neutral_lighter;
	result.Colors[ImGuiCol_TitleBg]               = neutral_dark;
	result.Colors[ImGuiCol_TitleBgActive]         = accent_dark;
	result.Colors[ImGuiCol_TitleBgCollapsed]      = neutral_darker;
	result.Colors[ImGuiCol_MenuBarBg]             = neutral_dark;
	result.Colors[ImGuiCol_ScrollbarBg]           = Invisible();
	result.Colors[ImGuiCol_ScrollbarGrab]         = neutral_dark;
	result.Colors[ImGuiCol_ScrollbarGrabHovered]  = Grey();
	result.Colors[ImGuiCol_ScrollbarGrabActive]   = neutral_lighter;
	result.Colors[ImGuiCol_CheckMark]             = accent_lighter;
	result.Colors[ImGuiCol_SliderGrab]            = accent_light;
	result.Colors[ImGuiCol_SliderGrabActive]      = accent_lighter;
	result.Colors[ImGuiCol_Button]                = accent_dark;
	result.Colors[ImGuiCol_ButtonHovered]         = accent_light;
	result.Colors[ImGuiCol_ButtonActive]          = accent_lighter;
	result.Colors[ImGuiCol_Header]                = accent_dark;
	result.Colors[ImGuiCol_HeaderHovered]         = accent_light;
	result.Colors[ImGuiCol_HeaderActive]          = accent_lighter;
	result.Colors[ImGuiCol_Separator]             = Grey();
	result.Colors[ImGuiCol_SeparatorHovered]      = neutral_lighter;
	result.Colors[ImGuiCol_SeparatorActive]       = neutral_lighter;
	result.Colors[ImGuiCol_ResizeGrip]            = neutral_dark;
	result.Colors[ImGuiCol_ResizeGripHovered]     = Grey();
	result.Colors[ImGuiCol_ResizeGripActive]      = neutral_lighter;
	result.Colors[ImGuiCol_Tab]                   = neutral_dark;
	result.Colors[ImGuiCol_TabHovered]            = Grey();
	result.Colors[ImGuiCol_TabActive]             = accent_dark;
	result.Colors[ImGuiCol_TabUnfocused]          = neutral_dark;
	result.Colors[ImGuiCol_TabUnfocusedActive]    = accent_dark;
	result.Colors[ImGuiCol_PlotLines]             = accent_light;
	result.Colors[ImGuiCol_PlotLinesHovered]      = accent_lighter;
	result.Colors[ImGuiCol_PlotHistogram]         = accent_light;
	result.Colors[ImGuiCol_PlotHistogramHovered]  = accent_lighter;
	result.Colors[ImGuiCol_TableHeaderBg]         = accent_dark;
	result.Colors[ImGuiCol_TableBorderStrong]     = neutral_lighter;
	result.Colors[ImGuiCol_TableBorderLight]      = Grey();
	result.Colors[ImGuiCol_TableRowBg]            = Invisible();
	result.Colors[ImGuiCol_TableRowBgAlt]         = Grey( .1f );
	result.Colors[ImGuiCol_TextSelectedBg]        = accent_dark;
	result.Colors[ImGuiCol_DragDropTarget]        = accent_lighter;
	result.Colors[ImGuiCol_NavHighlight]          = neutral_lighter;
	result.Colors[ImGuiCol_NavWindowingHighlight] = neutral_lighter;
	result.Colors[ImGuiCol_NavWindowingDimBg]     = Grey( .5f );
	result.Colors[ImGuiCol_ModalWindowDimBg]      = Grey( .5f );

	return result;
}
