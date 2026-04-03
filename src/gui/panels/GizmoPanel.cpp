#include "scene/selection/SelectionManager.hpp"

#include "gui/Gui.hpp"
#include "gui/GuiLayout.hpp"

#include "imgui.h"

void drawGizmoPanel(Gui &gui, const SelectionManager &selection)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + GuiLayout::TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(
        ImVec2(mainViewport->WorkPos.x + GuiLayout::LEFT_PANEL_WIDTH, mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(ImVec2(100.0f, 100.0f));

    if (!selection.hasSelection())
        return;
    
    ImGui::Begin(" ");

    if (ImGui::Button("Translation"))
    {
        return;
    }

    if (ImGui::Button("Rotation"))
    {
        return;
    }

    if (ImGui::Button("Scale"))
    {
        return;
    }

    ImGui::End();
}
