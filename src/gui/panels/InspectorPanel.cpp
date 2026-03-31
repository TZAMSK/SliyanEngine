#include "gui/panels/InspectorPanel.hpp"

#include "gui/Gui.hpp"
#include "gui/GuiLayout.hpp"
#include "render/ViewportRenderer.hpp"
#include "scene/Scene.hpp"

#include "imgui.h"

void drawInspectorPanel(Gui &gui, Scene &scene, ViewportRenderer &renderer)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + GuiLayout::TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + mainViewport->WorkSize.x - GuiLayout::RIGHT_PANEL_WIDTH,
                                   mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(
        ImVec2(GuiLayout::RIGHT_PANEL_WIDTH, mainViewport->WorkSize.y - topOffset - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Inspector", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Renderer");
    ImGui::Separator();
    ImGui::ColorEdit3("Background", renderer.backgroundColor());

    ImGui::Spacing();
    ImGui::Text("Camera");
    ImGui::Separator();

    const auto &camera = scene.getCamera();
    ImGui::Text("Position: %.2f %.2f %.2f", camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);
    ImGui::Text("Target:   %.2f %.2f %.2f", camera.getTarget().x, camera.getTarget().y, camera.getTarget().z);

    if (camera.getStrategy())
    {
        ImGui::Text("View mode: %s", camera.getStrategy()->name());
    }

    ImGui::Spacing();
    ImGui::Text("Placement mode: %s", gui.isTrianglePlacementArmed() ? "Triangle" : "None");

    ImGui::End();
}
