#include "gui/Gui.hpp"
#include "gui/GuiLayout.hpp"
#include "render/ViewportRenderer.hpp"

#include "imgui.h"

#include <cstdint>

void drawViewportPanel(Gui &gui, ViewportRenderer &renderer)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + GuiLayout::LEFT_PANEL_WIDTH,
                                   mainViewport->WorkPos.y + GuiLayout::TOOLBAR_HEIGHT));
    ImGui::SetNextWindowSize(
        ImVec2(mainViewport->WorkSize.x - GuiLayout::LEFT_PANEL_WIDTH - GuiLayout::RIGHT_PANEL_WIDTH,
               mainViewport->WorkSize.y - GuiLayout::TOOLBAR_HEIGHT - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Viewport", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoBringToFrontOnFocus);

    const ImVec2 avail = ImGui::GetContentRegionAvail();

    if (avail.x > 0.0f && avail.y > 0.0f)
    {
        renderer.resizeFramebuffer(static_cast<int>(avail.x), static_cast<int>(avail.y));
    }

    const ImVec2 pos = ImGui::GetCursorScreenPos();
    gui.setViewportRect(pos, avail);

    ImGui::Image((ImTextureID)(intptr_t)renderer.getColorTexture(), avail, ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
}
