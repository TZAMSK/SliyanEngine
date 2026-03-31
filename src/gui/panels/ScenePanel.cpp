#include "gui/panels/ScenePanel.hpp"

#include "gui/GuiLayout.hpp"
#include "scene/Scene.hpp"

#include "imgui.h"

void drawScenePanel(Scene &scene)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + GuiLayout::TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(
        ImVec2(GuiLayout::LEFT_PANEL_WIDTH, mainViewport->WorkSize.y - topOffset - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Objects");
    ImGui::Separator();
    ImGui::Text("Triangles: %d", static_cast<int>(scene.getTriangles().size()));

    for (std::size_t index = 0; index < scene.getTriangles().size(); ++index)
    {
        const auto &triangle = scene.getTriangles()[index];
        ImGui::BulletText("Triangle %d  (%.2f, %.2f, %.2f)", static_cast<int>(index), triangle.getPosition().x,
                          triangle.getPosition().y, triangle.getPosition().z);
    }

    ImGui::End();
}
