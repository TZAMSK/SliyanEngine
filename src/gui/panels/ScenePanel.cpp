#include "gui/panels/ScenePanel.hpp"

#include "gui/GuiLayout.hpp"
#include "scene/Scene.hpp"
#include "scene/shapes/Shape.hpp"

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

void drawScenePanel(Scene &scene)
{
    ImGuiViewport *mainViewport = ImGui::GetMainViewport();
    const float topOffset = ImGui::GetFrameHeight() + GuiLayout::TOOLBAR_HEIGHT;

    ImGui::SetNextWindowPos(ImVec2(mainViewport->WorkPos.x, mainViewport->WorkPos.y + topOffset));
    ImGui::SetNextWindowSize(
        ImVec2(GuiLayout::LEFT_PANEL_WIDTH, mainViewport->WorkSize.y - topOffset - GuiLayout::CONSOLE_HEIGHT));

    ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    ImGui::Text("Objects  (%d)", scene.getShapeCount());
    ImGui::Separator();

    const auto &shapes = scene.getShapes();
    for (std::size_t i = 0; i < shapes.size(); ++i)
    {
        const Shape &s = *shapes[i];
        ImGui::BulletText("%s %d  (%.2f, %.2f, %.2f)", shapeTypeName(s.getType()), static_cast<int>(i),
                          s.getPosition().x, s.getPosition().y, s.getPosition().z);
    }

    ImGui::End();
}
