#include "gui/panels/InspectorPanel.hpp"

#include "gui/Gui.hpp"
#include "gui/GuiLayout.hpp"
#include "render/ViewportRenderer.hpp"
#include "scene/Scene.hpp"
#include "scene/shapes/Shape.hpp"
#include "scene/selection/SelectionManager.hpp"

#include "imgui.h"

static const char *shapeTypeName(ShapeType t)
{
    switch (t)
    {
    case ShapeType::Triangle:
        return "Triangle";
    case ShapeType::Rectangle:
        return "Rectangle";
    case ShapeType::Cube:
        return "Cube";
    }
    return "Shape";
}

void drawInspectorPanel(Gui &gui, Scene &scene, ViewportRenderer &renderer, const SelectionManager &selection)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + GuiLayout::TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x + mainViewport->WorkSize.x - GuiLayout::RIGHT_PANEL_WIDTH,
                                   mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(
        ImVec2(GuiLayout::RIGHT_PANEL_WIDTH, mainViewport->WorkSize.y - topOffset - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Inspector", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Renderer color
    ImGui::Text("Renderer");
    ImGui::Separator();
    ImGui::ColorEdit3("Background", renderer.backgroundColor());

    // Camera information
    ImGui::Spacing();
    ImGui::Text("Camera");
    ImGui::Separator();

    const auto &cam = scene.getCamera();
    ImGui::Text("Position: %.2f %.2f %.2f", cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
    ImGui::Text("Target:   %.2f %.2f %.2f", cam.getTarget().x, cam.getTarget().y, cam.getTarget().z);

    if (cam.getStrategy())
        ImGui::Text("View mode: %s", cam.getStrategy()->name());

    ImGui::Spacing();
    ImGui::Text("Placement mode: %s", gui.isTrianglePlacementArmed() ? "Armed" : "None");

    // Selected shape info
    if (selection.hasSelection())
    {
        Shape *sel = scene.findShapeById(selection.selectedId());
        if (sel)
        {
            ImGui::Spacing();
            ImGui::Text("Selected: %s  (id %u)", shapeTypeName(sel->getType()), sel->id);
            ImGui::Separator();

            ImGui::Text("Position: %.2f, %.2f, %.2f", sel->getPosition().x, sel->getPosition().y, sel->getPosition().z);

            float col[4] = {
                sel->getColor().r,
                sel->getColor().g,
                sel->getColor().b,
                sel->getColor().a,
            };
            if (ImGui::ColorEdit4("Color##sel", col))
            {
                sel->setColor(glm::vec4(col[0], col[1], col[2], col[3]));
            }
            // ImGui::SliderFloat("float", sel->getPosition().x, 0.0f, 1.0f);
        }
    }

    ImGui::End();
}
