#include "stdafx.h"
#include <ImGuiStyle.h>
#include <imgui_internal.h>

void SpImguiCustom::StyleColorsSpicavirSakura(ImGuiStyle* dst)
{
    {
        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 0.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.48f, 0.16f, 0.36f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.98f, 0.26f, 0.85f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.98f, 0.26f, 0.92f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.48f, 0.16f, 0.38f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.98f, 0.26f, 0.98f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.86f, 0.24f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.89f, 0.26f, 0.98f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.91f, 0.26f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.26f, 0.94f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.06f, 0.78f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.98f, 0.26f, 0.96f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.98f, 0.26f, 0.71f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.98f, 0.26f, 0.69f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.38f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.73f, 0.10f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.75f, 0.10f, 0.61f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.93f, 0.26f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.98f, 0.26f, 0.76f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(0.98f, 0.26f, 0.69f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.54f, 0.18f, 0.58f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.93f, 0.26f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.85f, 0.21f, 0.69f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.37f, 0.14f, 0.42f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.96f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.77f, 0.26f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
}

void SpImguiCustom::StyleColorsDarkBlossom(ImGuiStyle* dst)
{
    {
        ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
        ImVec4* colors = style->Colors;

        colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
        colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 0.00f);
        colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg] = ImVec4(0.22f, 0.22f, 0.22f, 0.54f);
        colors[ImGuiCol_FrameBgHovered] = ImVec4(0.99f, 0.71f, 0.94f, 0.40f);
        colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.50f, 0.73f, 0.67f);
        colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark] = ImVec4(0.99f, 0.68f, 0.99f, 1.00f);
        colors[ImGuiCol_SliderGrab] = ImVec4(0.98f, 0.61f, 0.99f, 1.00f);
        colors[ImGuiCol_SliderGrabActive] = ImVec4(0.98f, 0.83f, 1.00f, 1.00f);
        colors[ImGuiCol_Button] = ImVec4(0.45f, 0.45f, 0.45f, 0.40f);
        colors[ImGuiCol_ButtonHovered] = ImVec4(0.99f, 0.67f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive] = ImVec4(0.99f, 0.85f, 0.96f, 1.00f);
        colors[ImGuiCol_Header] = ImVec4(0.46f, 0.46f, 0.46f, 0.31f);
        colors[ImGuiCol_HeaderHovered] = ImVec4(0.96f, 0.68f, 0.86f, 0.80f);
        colors[ImGuiCol_HeaderActive] = ImVec4(0.81f, 0.50f, 0.68f, 1.00f);
        colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.38f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered] = ImVec4(0.56f, 0.37f, 0.57f, 0.78f);
        colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.35f, 0.52f, 1.00f);
        colors[ImGuiCol_ResizeGrip] = ImVec4(0.71f, 0.55f, 0.72f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.00f, 0.70f, 0.91f, 0.67f);
        colors[ImGuiCol_ResizeGripActive] = ImVec4(1.00f, 0.79f, 0.91f, 0.95f);
        colors[ImGuiCol_Tab] = ImVec4(0.33f, 0.32f, 0.33f, 0.86f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.99f, 0.82f, 1.00f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.86f, 0.55f, 0.79f, 1.00f);
        colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.37f, 0.14f, 0.42f, 1.00f);
        colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg] = ImVec4(0.98f, 0.26f, 0.96f, 0.35f);
        colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight] = ImVec4(0.77f, 0.26f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        style->FrameRounding = 12.f;
        style->WindowRounding = 12.f;
    }
}

void SpImguiCustom::StyleColorsCyber(ImGuiStyle* dst)
{
    ImGuiStyle* style = dst ? dst : &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    const ImVec4 kColorPrimary = ImVec4((float)0x1c / 256.f, (float)0xfe / 256.f, (float)0xff / 256.f, (float)0xcc / 256.f);
    const ImVec4 kColorPrimaryA99 = ImVec4((float)0x1c / 256.f, 0xfe, (float)0xff / 256.f, (float)0x99 / 256.f);
    const ImVec4 kColorPrimaryA66 = ImVec4((float)0x1c / 256.f, 0xfe, (float)0xff / 256.f, (float)0x66 / 256.f);
    const ImVec4 kColorPrimaryA33 = ImVec4((float)0x1c / 256.f, (float)0xfe / 256.f, (float)0xff / 256.f, (float)0x33 / 256.f);
    const ImVec4 kColorPrimaryDark = ImVec4((float)0x03 / 256.f, (float)0x19 / 256.f, (float)0x1a / 256.f, (float)0xbb / 256.f);
    const ImVec4 kColorAccent = ImVec4((float)0xff / 256.f, (float)0xa1 / 256.f, (float)0x00 / 256.f, (float)0xee / 256.f);
    const ImVec4 kColorAccentAcc = ImVec4((float)0xff / 256.f, (float)0xa1 / 256.f, (float)0x00 / 256.f, (float)0xcc / 256.f);
    const ImVec4 kColorAccentA99 = ImVec4((float)0xff / 256.f, (float)0xa1 / 256.f, (float)0x00 / 256.f, (float)0x99 / 256.f);

    const ImVec4 kColorWhite = ImVec4((float)0xdd / 256.f, (float)0xdd / 256.f, (float)0xdd / 256.f, (float)0xcc / 256.f);
    const ImVec4 kColorBlackA55 = ImVec4((float)0x11 / 256.f, (float)0x11 / 256.f, (float)0x11 / 256.f, (float)0x55 / 256.f);
    
    colors[ImGuiCol_MenuBarBg] = kColorPrimaryA33;
    colors[ImGuiCol_TitleBg] =                kColorPrimaryDark;
    colors[ImGuiCol_TitleBgCollapsed] =       kColorPrimaryDark;
    colors[ImGuiCol_TitleBgActive] =          kColorPrimaryA99;
    colors[ImGuiCol_WindowBg] =               kColorPrimaryDark;
    colors[ImGuiCol_Border] =                 kColorPrimaryA99;
    colors[ImGuiCol_FrameBg] =                kColorPrimaryA33;
    colors[ImGuiCol_FrameBgHovered] =         kColorAccentAcc;
    colors[ImGuiCol_FrameBgActive] =          kColorAccent;
    colors[ImGuiCol_ScrollbarBg] =            kColorPrimaryA33;
    colors[ImGuiCol_ScrollbarGrab] =          kColorPrimaryA99;
    colors[ImGuiCol_ScrollbarGrabHovered] =   kColorPrimaryA99;
    colors[ImGuiCol_ScrollbarGrabActive] =    kColorPrimary;
    colors[ImGuiCol_CheckMark] =              kColorAccent;
    colors[ImGuiCol_SliderGrab] =             kColorPrimaryA99;
    colors[ImGuiCol_SliderGrabActive] =       kColorPrimary;
    colors[ImGuiCol_Button] =                 kColorPrimaryA33;
    colors[ImGuiCol_ButtonHovered] =          kColorAccentAcc;
    colors[ImGuiCol_ButtonActive] =           kColorAccent;
    colors[ImGuiCol_Header] =                 kColorAccentA99;
    colors[ImGuiCol_HeaderHovered] =          kColorAccentAcc;
    colors[ImGuiCol_HeaderActive] =           kColorAccent;
    colors[ImGuiCol_PlotLines] =              kColorPrimaryA99;
    colors[ImGuiCol_PlotLinesHovered] =       kColorPrimary;
    colors[ImGuiCol_PlotHistogram] =          kColorPrimaryA99;
    colors[ImGuiCol_PlotHistogramHovered] =   kColorPrimary;
    colors[ImGuiCol_Text] =                   kColorPrimary;
    colors[ImGuiCol_TextDisabled] =           kColorPrimaryA66;
    colors[ImGuiCol_TextSelectedBg] =         kColorAccent;
    colors[ImGuiCol_PopupBg] =                kColorPrimaryDark;

    style->AntiAliasedLines = true;
    style->AntiAliasedFill = true;
    style->FrameRounding = 0.f;
    style->WindowRounding = 0.f;
}
